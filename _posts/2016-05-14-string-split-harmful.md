---
layout: post
title: "String.split considered harmful"
summary: "It violates your intuitive understanding"
category: development
tags: [scala]
---
{% include JB/setup %}

Following the tradition started by [Dijkstra in the sixties][harmful], I want
to warn you all: Java's `String.split` should be considered harmful. But first,
some context.

[harmful]: https://en.wikipedia.org/wiki/Considered_harmful

As Ben Moseley and Peter Marks explain in [*Out of the Tar Pit*][tarpit] there
are two main techniques used to understand a piece of software: informal
reasoning and testing. They bet on the former as being the most important
(emphasis in the original):

> Of the two informal reasoning is the most important by far. This is because
> --as we shall see below-- there are inherent limits to what can be achieved by
> testing, and because informal reasoning (by virtue of being an inherent part
> of the development process) is *always* used.

[tarpit]: http://shaffner.us/cs/papers/tarpit.pdf

Even testing is, in some sense, guided by informal reasoning. For this reason,
we should be very wary of how designs aid or hamper our reasoning about the
code. `String.split` is an unfortunate example of the latter and I frequently
see related bugs when doing code reviews.

Apart from stark simplicity (less stuff, less intertwined components) one thing
that fosters our ability to reason is relying on properties that the different
designs have. These can be things like knowing that the returned value is never
empty, there is symmetry if we switch the arguments order, there is independence
between calls, data structures are immutable and we not worry about other parts
of the program changing them, some invariant holds, etc.

Let's imagine that we need a function to split some input string in words
discarding the white space. If we don't believe in testing (this function is so
simple, what can go wrong?) this is what we are going to write and ship to
production:

{% highlight scala %}
def split(text: String): List[String] =
  text.split("\\s+").toList
{% endhighlight %}

However, this implementation is wrong because `String.split` does not behave
like we expect.

<blockquote class="twitter-tweet tw-align-center" data-lang="en"><p lang="en" dir="ltr">Number one mistake in TDD: Underestimating how effective trivial tests can be in catching bugs.</p>&mdash; Computer Science (@CompSciFact) <a href="https://twitter.com/CompSciFact/status/730755611290415105">May 12, 2016</a></blockquote>
<script async src="//platform.twitter.com/widgets.js" charset="utf-8"></script>

A classical move when doing TDD is starting with the degenerate case and then
more complicated ones.

{% highlight scala %}
scala> split("")
res0: List[String] = List("")

scala> split("hello")
res1: List[String] = List(hello)

scala> split("hello world")
res2: List[String] = List(hello, world)
{% endhighlight %}

Through testing we have learned about the exceptional behavior of `String.split`
for the empty input so we improve our function.

{% highlight scala %}
def split(text: String): List[String] =
  if (text.isEmpty) List.empty
  else text.split("\\s+").toList
{% endhighlight %}

But, are we done yet? We could examine the
[implementation of `String.split`][sources] but the code is a little bit
convoluted so we might be overlooking something. How can we gain confidence?

[sources]: http://grepcode.com/file/repository.grepcode.com/java/root/jdk/openjdk/8u40-b25/java/lang/String.java#2317

Property-based testing can be a useful tool in situations like this. We can
directly specify the property we want to rely on and let the test environment to
generate random inputs to test it. In this case, the property we were expecting to hold expressed in plain English was:

> Given any list of words (contiguous non white space), no matter how
> interleaved with white space I can recover the original list of words with the
> `split` function.

We can easily define a couple of [scalacheck](http://scalacheck.org/) generators
for the text and the white space:

{% highlight scala %}
val words = Gen.listOf(Gen.identifier.suchThat(_.nonEmpty))
val delimiter = Gen.nonEmptyListOf(Gen.oneOf(' ', '\t', '\n')).map(_.mkString)
{% endhighlight %}

And then the property itself:

{% highlight scala %}
it should "split text on whitespace" in {
  forAll(
    (delimiter, "prefix"),
    (delimiter, "separator"),
    (delimiter, "suffix"),
    (words, "words")
  ) { (prefix, separator, suffix, words) =>
    val input = words.mkString(prefix, separator, suffix)
    split(input) should be === words
  }
}
{% endhighlight %}

Immediately, scalacheck finds a counterexample for our test.

    Message: List("", "e") was not equal to List("e")
    Location: (StringSplitTest.scala:22)
    Occurred when passed generated values (
      prefix = " ",
      separator = "", // 1 shrink
      suffix = " ",
      words = List(e) // 1 shrink
    )
    
Looks like prefixed white space creates an initial empty string out of the blue.
Next iteration:

{% highlight scala %}
def split(text: String): List[String] =
  if (text.isEmpty) List.empty
  else text.trim.split("\\s+").toList
{% endhighlight %}

Are we lucky?

    Message: List("") was not equal to List()
    Location: (StringSplitTest.scala:22)
    Occurred when passed generated values (
      prefix = " ",
      separator = " ",
      suffix = " ",
      words = List()
    )
    
Aaaarrg! Let's change our strategy to just filtering empty strings in the
output.

{% highlight scala %}
def split(text: String): List[String] =
  text.split("\\s+").filter(_.nonEmpty).toList
{% endhighlight %}

This time the tests pass (no counterexample found) and we have more confidence
on our code supporting the property we initially were assuming. We have learned
how pernicious `String.split` is. Avoid it and be very suspicious of any code
review in which it shows up.

<div style="margin: 1em 0; text-align: center">
    <img src="/assets/split/dijkstra_split.jpg"
        alt="Disjkstra: therefore String.split is dangerous as hell"
        style="width: 80%"/>
</div>

But we have learned something way more valuable through this (contrived)
example: property based testing is a bridge between informal reasoning
understanding and testing-based understanding. **We enhance our reasoning thanks
to a property that is "verified" through testing**.

Note the quotes in "verified". We cannot exhaustively check all inputs in the
general case so we are highly confident but not this-is-proven confident. This
is nice for properties that escape static analysis but we are better off when we
can rely on the compiler or linter.

<blockquote class="twitter-tweet tw-align-center" data-lang="en"><p lang="en" dir="ltr">If only there were a way for a computer to check that objects have the expected type ... <a href="https://t.co/aCYz9Z3M0Q">https://t.co/aCYz9Z3M0Q</a></p>&mdash; Computer Science (@CompSciFact) <a href="https://twitter.com/CompSciFact/status/730769733092544512">May 12, 2016</a></blockquote>
<script async src="//platform.twitter.com/widgets.js" charset="utf-8"></script>
