---
layout: post
title: "You've got a monad in me"
summary: "Functional patterns in the real world"
category: development
tags: [design, fp, patterns]
---
{% include JB/setup %}

Recently, design patterns are terribly popular, especially on job interviews,
so let's take a look at its [Wikipedia entry][wiki]:

> In software engineering, a design pattern is a general reusable solution to
> a commonly occurring problem within a given context in software design.

[wiki]: https://en.wikipedia.org/wiki/Software_design_pattern

There are [many][apprenticeship] [good][enterprise] [books][gof] and
[articles][c2] on design patterns but you cannot reap the benefits just by
reading about them.

[gof]: http://www.amazon.es/Design-Patterns-Elements-Reusable-Object-Oriented/dp/0201633612/ref=sr_1_1?ie=UTF8&qid=1438527549&sr=8-1&keywords=design+patterns
[enterprise]: http://www.amazon.com/Patterns-Enterprise-Application-Architecture-Martin/dp/0321127420/ref=sr_1_1?s=books&ie=UTF8&qid=1438527655&sr=1-1&keywords=enterprise+design+patterns
[apprenticeship]: http://www.amazon.com/Apprenticeship-Patterns-Guidance-Aspiring-Craftsman/dp/0596518382/ref=sr_1_8?s=books&ie=UTF8&qid=1438527708&sr=1-8&keywords=refactoring+design+patterns
[c2]: http://c2.com/cgi/wiki?DesignPatterns

A prerequisite for truly assimilating patterns is to have a prior background:
having experience the problem in its context and, somehow solved or
sidestepped it.  Ideally, when you read about a pattern, you should recognize
it as an acquaintance you have met before.  However, you can get more or
less amazed about how close your solution was to the canonical one.

From this point on, the newly acquired pattern will yield dividends.  Not in
the form of letting you solve a problem you couldn't previously solve since
you need that prior experience to really embrace the pattern.  True
dividends include faster problem identification, easier communication[^1] and
being able to implement a less idiosyncratic solution.

[^1]: Software development is a social endeavor after all.

<div style="margin: 1em 0; text-align: center">
    <a href="https://www.youtube.com/watch?v=zB2gPZRsz0Q">
        <img src="/assets/functional_patterns/monads.jpg"
            alt="Monads, monads everywhere"
            style="width: 60%"/>
    </a>
</div>

Functors and monads are very common functional programming design patterns and
they are waiting for you to carve them out of your code pretty much
everywhere.  Once you get familiar with FP and read about these design
patterns[^2] you develop the same intuition about this that about when to use
a singleton.

[^2]: If you approach this by reading a monad tutorial as the first step you are doomed. You need to get some field experience on FP and only then, chase the monad.

I want to illustrate this with a recent example from my day job.

## Caching information

First, imagine you have some piece of data `Foo` that is obtained by a
fallible polling from a remote server and you should track if your cached data
is fresh or stale.  It is reasonable to use a `Boolean` for this purpose at
this stage.

{% highlight scala %}
case class Foo(
    accountId: String,
    balances: FiatAmounts,
    isFresh: Boolean
)
{% endhighlight %}

Later, you add another piece of information `Bar` with similar
characteristics.

{% highlight scala %}
case class Bar(
    accountId: String,
    activePeriod: Interval,
    transferLimits: FiatAmounts,
    isFresh: Boolean
)
{% endhighlight %}

After this change, you realize that the design is taking a wrong course: you
have duplication and primitive booleans spreading to more and more
places[^primitive].  So you replace the plain `Boolean` with the richer type
`CacheStatus`.

[^primitive]: This looks like [the primitive obsession smell][primitive-obsession]
[primitive-obsession]: https://sourcemaking.com/refactoring/primitive-obsession

{% highlight scala %}
sealed trait CacheStatus {
  def isFresh: Boolean
}

object CacheStatus {
  case object Fresh extends CacheStatus {
    override def isFresh = true
  }
  case object Stale extends CacheStatus {
    override def isFresh = false
  }
}
{% endhighlight %}

To remove the duplication we could introduce a generic type `Cached[T]` that
will allow you to annotate anything with freshness information:

{% highlight scala %}
case class Cached[A](cached: A, status: CacheStatus)
{% endhighlight %}

<img src="/assets/functional_patterns/spider-sense.jpg"
        alt="Spiderman senses a monad"
        style="width: 30%; margin: 0 0 1em 1em; float: right"/>

Now your spider-sense goes off yelling like mad at you, "monad! monad!".  In
practical terms a monad can be seen as any *container* or *context* holding
information (i.e. with one generic parameter) that supports and implementation
of `map` and `flatMap`[^3] that won't surprise you.

[^3]: There are alternative and more formal definitions of what a monad is and the monad laws in the [Typeclassopedia][typeclassopedia].
[typeclassopedia]: https://wiki.haskell.org/Typeclassopedia#Monad

If you add those methods to `Cached[T]` then you can transform cached values
without losing the information about freshness.

{% highlight console %}
scala> val cachedGreet = Cached.fresh("hello")
scala> cachedGreet.map(_.size)
Cached(5, Fresh)
{% endhighlight %}

And you can also combine cached values using the expressive comprehension
syntax having guaranteed that the result will be fresh only if all the inputs
are fresh.

{% highlight console %}
scala> val cachedName = Cached.stale("John")
scala> for {
     |   greet <- cachedGreet
     |   name <- cachedName
     | } yield s"$greet $name!"
res1 = Cached(hello world!, Stale)
{% endhighlight %}

You can take a look at the completed `Cached[T]` implementation
[here][Cached.scala].

[Cached.scala]: https://github.com/Coinffeine/coinffeine/blob/565bf9034c32a061aa13650a3702d047d9dd47f8/coinffeine-model/src/main/scala/coinffeine/model/util/Cached.scala

## No silver bullet

Design patterns, functional or OO, are not silver bullets that you can harness
just by invoking their names.  You need to invest time gaining context and
studying them before getting your spider-sense tuned for helping you when to
use them.

