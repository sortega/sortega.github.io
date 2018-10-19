---
layout: post
title: "Scala mangling versus Mockito"
summary: "Scala-to-Java interop is a leaky abstraction that can bite you. In
this post I share what you need to understand how and when."
category: development
tags: [scala, java, testing]
---
{% include JB/setup %}

One of the main strengths of Scala is that you can parasite the ecosystem of
libraries of Java and infrastructure and tooling of the JVM[^1].  Of course,
this is not a free lunch as some gotchas of Scala come from that permeability
to Java (like nullability, universal equality and the like).

Interop, or the ability or use Java code from Scala and vice versa, works
reasonably well in the Scala-calling-Java direction and not so well in the
opposite direction and that is a conscious design decision of the language. To
understand why, we need to deep dive in how Scala and Java share the JVM.

## Name mangling

In order to execute in the JVM, `scalac` is forced to compile to the same
bytecode and artefacts that `javac`.  Scala concepts that can be mapped
directly to Java are no surprise. For instance, you are going to see something
like `def square(n: Int): Int` as `int square(int n)` from Java. But, what
about concepts that cannot be cleanly mapped?

{% highlight scala %}
final case class Point(x: Int, y: Int)

object Point {
  val Origin = Point(0, 0)
}
{% endhighlight %}

This little snipped is going to be compiled into two classes: an unsurprising
`Point.class` and a more interesting `Point$.class`. Scala's compiler needs to
generate a class for the companion object and it's using the reserved
character `$`[^2] to avoid colliding with user defined names. This process is
called [name mangling][mangling] and is at least as older as compilers are.

Using [javap][javap] we can take a look at the `class` files:

{% highlight bash %}
$ javap 'Point$.class'
Compiled from "Point.scala"
public final class mockpost.Point$ implements scala.Serializable {
  public static mockpost.Point$ MODULE$;
  public static {};
  public mockpost.Point Origin();
  public mockpost.Point apply(int, int);
  public scala.Option<scala.Tuple2<java.lang.Object, java.lang.Object>>
    unapply(mockpost.Point);
}
{% endhighlight %}

After looking at this it is evident[^3] how to use the companion object from Java.

{% highlight java %}
Point origin = Point$.MODULE$.Origin();
System.out.println(String.format(
        "Origin is at x=%d y=%d", origin.x(), origin.y()));
{% endhighlight %}


## Leaky abstractions are going to leak

When mangling is a leaky abstraction? If you limit yourself to calling Java
code from Scala you need to consider mangling almost never. Almost.

Recently I got bitten by this leaky abstraction[^4] while using
[Mockito][mockito] from Scala.  For the sake of simplicity (and to avoid
exposing my employer's <acronym title="Intellectual Property">IP</acronym>),
let's see that we have this service interface:

{% highlight scala %}
trait Service {
  def request(id: String,
              tags: Map[String, String] = Map.empty): List[Int]
}
{% endhighlight %}

We want to unit test this class that gets one instance of the service
injected:

{% highlight scala %}
final class OtherService(service: Service) {
  def request(id: String, maybeTags: Option[String]): Int =
    (maybeTags match {
      case Some(tags) => service.request(id, parseTags(tags))
      case None       => service.request(id)
    }).sum

  private def parseTags(tags: String): Map[String, String] =
    (for {
      pair <- tags.split(",").filter(_.nonEmpty)
      Array(key, value) = pair.split(":")
    } yield key.trim -> value.trim).toMap
}
{% endhighlight %}

Note that we have two cases to test: when tags are passed explicitly and when
a default argument is used.  Let's say we write two test cases to cover both
cases:

{% highlight scala %}
"Mocking service" should "work when all arguments are explicitly passed" in {
  val service = mock[Service]
  when(service.request("id1", Map("k1" -> "v1", "k2" -> "v2")))
    .thenReturn(List(42))
  val otherService = new OtherService(service)

  otherService.request("id1", maybeTags = Some("k1:v1, k2:v2")) shouldBe 42
}

it should "work when using default arguments" in {
  val service = mock[Service]
  when(service.request("id1", Map.empty)).thenReturn(List(42))
  val otherService = new OtherService(service)

  otherService.request("id1", maybeTags = None) shouldBe 42
}
{% endhighlight %}

What do we get when running them? The first passes but the second produces a
null pointer exception out of the blue.

    [info] Mocking service
    [info] - should work when all arguments are explicitly passed
    [info] - should work when using default arguments *** FAILED ***
    [info]   java.lang.NullPointerException:
    ...

This kind of problem can be a huge time hole as the source of the exception is
literally invisible, in **code we cannot see**.  Let's use `javap` on the
service (slightly cleaned up for readability):

{% highlight java %}
public interface Service {
  public abstract List<Object> request(String id, Map<String, String> tags);
  public static Map request$default$2$(Service service);
  public Map<String, String> request$default$2();
}
{% endhighlight %}

Wow, default arguments become new "invisible" methods with a mangled name in
the form `<method_name>$default$<argument_pos>$`[^6].  And we are not telling
Mockito to what to return in that case... so we get `null`.

## Workarounds and solutions

There are several unsatisfactory workarounds we can use to overcome this gotcha:

* We can avoid using default arguments in our production code. This is
  difficult to enforce and frankly depressing.
* We can expect `null`s when mocking the main method of the service. Be
  prepared to write an apologizing comment in your test.

{% highlight scala %}
it should "work when using default arguments" in {
  val service = mock[Service]
  // We are going to receive null instead of the default argument.
  // Sorry 'bout this. Trust me!
  when(service.request("id1", null)).thenReturn(List(42))
  val otherService = new OtherService(service)

  otherService.request("id1", maybeTags = None) shouldBe 42
}
{% endhighlight %}

* We can mock the default argument... from Java! I'm mentioning it just for
  completeness (makes my eyes bleed!).

{% highlight java %}
public class MockitoHelper {
    public static void setupDefaultArguments(Service mock) {
        Mockito.when(mock.request$default$2())
            .thenReturn(Map$.MODULE$.empty());
    }
}
{% endhighlight %}

However, the real solution to this problem[^5] is to use a mocking library
designed for Scala rather than Java. With [ScalaMock][scalamock] both test
cases will work seamlessly:

{% highlight scala %}
"Mocking service" should "work when all arguments are explicitly passed" in {
  val service = mock[Service]
  (service.request _)
    .expects("id1", Map("k1" -> "v1", "k2" -> "v2"))
    .returning(List(42))
  val otherService = new OtherService(service)

  otherService.request("id1", maybeTags = Some("k1:v1, k2:v2")) shouldBe 42
}

it should "work when using default arguments" ignore {
  val service = mock[Service]
  (service.request _)
    .expects("id1", Map.empty[String, String])
    .returning(List(42))
  val otherService = new OtherService(service)

  otherService.request("id1", maybeTags = None) shouldBe 42
}
{% endhighlight %}

Check out [this repo][repo] if you want to run this and the previous sample snippets.

In conclusion, it's wonderful to piggy-back on the JVM ecosystem but sometimes
we got bitten by leaky abstractions. It's in those cases when knowing what's
under the hood can save you hours of frustration.

[^1]: You can also parasite the Javascript ecosystem with [scala.js][scala-js] or fly by yourself with [scala native][scala-native]
[scala-js]: https://www.scala-js.org/
[scala-native]: https://www.scala-native.org
[mangling]: https://en.wikipedia.org/wiki/Name_mangling
[mangling-bug]: https://issues.scala-lang.org/browse/SI-3623
[^2]: You can actually try to use it in your class and variable names but it might interfere with mangling. We can say that it is "reserved" in the spirit of JavaScript's `undefined`
[javap]: https://docs.oracle.com/javase/7/docs/technotes/tools/windows/javap.html
[^3]: Evident but "ugly as a bare foot" as we say in Spanish
[^4]: Almost all abstractions are leaky. Almost.
[mockito]: https://site.mockito.org/
[^5]: Apart from not using mocks...
[scalamock]: http://scalamock.org/
[repo]: https://github.com/sortega/mockpost
[^6]: These names can grow so large when you have lambdas within lambdas that you might [hit the path length limit of Linux][mangling-bug]
