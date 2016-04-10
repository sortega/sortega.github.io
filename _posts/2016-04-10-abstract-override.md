---
layout: post
title: "Baking plugins"
summary: "Moving the decorator pattern to compile time"
category: development
tags: [design, scala, patterns]
---
{% include JB/setup %}

Scala is a never ending source of new syntax, tools and ideas. Some of them are
the bread and butter of the daily work (like cases classes, sealed traits and
higher order functions) but other language features are useful less often. This
is the case for the abstract overriding of functions.

## Type-safe bakery

A context in which this feature can be useful is the cake pattern in which our
application is a "cake" in which layered components, the "layers", are baked
in... at compile time. Jonas Bonér has a really [nice explanation][cake] of this
pattern so I'm going just to sketch the key points to make my point.

[cake]: http://jonasboner.com/real-world-scala-dependency-injection-di/

<div style="margin: 1em 0; text-align: center">
    <img src="/assets/stackable/cake.jpg"
        alt="A three-layer cake"
        style="width: 80%"/>
    <br/>
    <a href="https://www.flickr.com/photos/jshj/4619958228/">@jshj</a>'s rendition to layered architectures.
</div>

If we want to have type-safe dependency injection we need to represent the
application components with types, both interfaces and implementations. Let's
say we have a trait representing the configuration source.

{% highlight scala %}
trait ConfigComponent {
  def config: Config
}
{% endhighlight %}

And we have an implementation using some properties files to back it up (we
might have a different one using environment variables or an in-memory map).

{% highlight scala %}
trait PropertiesConfigComponent {
  def config: Config = {
    // read config from some properties files
  }
}
{% endhighlight %}

Next thing to represent with types: component dependencies. To such effect we
can use just inheritance or type self-annotations. The following network
component require to be mixed in a class having an implementation of
`ConfigComponent` and that closes the deal.

{% highlight scala %}
trait NetworkComponent { this: ConfigComponent =>
  lazy val gateway = ???
}
{% endhighlight %}

And finally we can bake our cake by mix all the components into a main class
or main object. It won't compile unless all component dependencies are
successfully resolved ('referenced bean not found' no more).

{% highlight scala %}
object Application extends App
  with PropertiesConfigComponent
  with NetworkComponent
  with FooUI {

  // start the app here
}
{% endhighlight %}

## Plugin architectures

Component traits offer methods to instantiate the application objects but what
if we want to have a plugin architecture in which many different components are
able to contribute instances of a given class? For instance, in a ranking system
we might have many relevance signals provided by different components.

A first shot to this might start with the following component interface:

{% highlight scala %}
trait SignalsComponent {
  def signals: Seq[Signal]
}
{% endhighlight %}
    
And we might want to mix in two different components providing implementations.

{% highlight scala %}
trait BehaviorSignalsComponent extends SignalsComponent {
  def signals: Seq[Signal] = Seq(
    Signal("b1"), Signal("b2")
  )
}

trait ContentSignalsComponent extends SignalsComponent {
  def signals: Seq[Signal] = Seq(
    Signal("c1"), Signal("c2")
  )
}
{% endhighlight %}

Unfortunately this won't work as one of the implementations will override the
second. We might try to have a different method for the signals provided by each
component and a third one merging all of them but that is a fragile piece of
boilerplate that should be edited every time we modify the system.

## The stackable trait pattern

At this point we have our motivating example! (if you are still reading me). If
you qualify a method with `abstract override` in a trait, Scala will allow you
to access `super` to decorate the other implementations of the method. This way
you can "stack" as many implementations of the trait as you want and, therefore,
it is known as the _stackable trait pattern_.

{% highlight scala %}
trait BehaviorSignalsComponent extends SignalsComponent {
  abstract override def signals: Seq[Signal] = super.signals ++ Seq(
    Signal("b1"), Signal("b2")
  )
}

trait ContentSignalsComponent extends SignalsComponent {
  abstract override def signals: Seq[Signal] = super.signals ++ Seq(
    Signal("c1"), Signal("c2")
  )
}
{% endhighlight %}

There is only an additional requirement: there must be a base implementation
somewhere. In this case we might add an empty list in the `SignalsComponent`
itself.

{% highlight scala %}
trait SignalsComponent {
  def signals: Seq[Signal] = Seq.empty
}
{% endhighlight %}
    
If you think about this pattern, it is just moving the decorator pattern from
run to compile time. This is analogous to the cake pattern moving dependency
resolution from run to compile time.
