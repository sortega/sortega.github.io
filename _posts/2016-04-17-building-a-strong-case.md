---
layout: post
title: "Building a strong case (class)"
summary: "Extending the guarantees your case classes provides you."
category: development
tags: [design, scala, patterns]
---
{% include JB/setup %}

Case classes and objects are one of the features of Scala that bring the most
benefit with the least added complexity. Having these value classes baked in the
syntax of the programming language brings many advantages: expressivity,
immutability, algebraic composition, `equals`/`hashCode`/`toString`,
`Serializable` for free...

Typically, I use case classes to model domain entities with strong guarantees by
creating classes closely matching concepts and letting static typing and
immutability making most of the invalid system states impossible to represent.
Nesting case classes using sealed traits can let you model intricate domains.

But Scala type system cannot easily represent all the properties we might like
to enforce and take for granted in the overall code base. Fortunately, there is
an easy way to extend the guarantees of a case class thanks to its immutability:
whatever property we enforce at the constructor will be an invariant.

<div style="margin: 1em 0; text-align: center">
    <img src="/assets/strong_case/columbo.jpg"
        alt="Columbo" style="width: 80%"/>
    <br/>
    <a href="https://en.wikipedia.org/wiki/Columbo">Columbo</a> was able to
    build a strong case like no one else.
</div>

Let's use as example something as simple as a case class representing an even
number.

{% highlight scala %}
case class Even(value: Int) {
  require(value % 2 == 0, s"Not even: $value")
}
{% endhighlight %}

The `require` statement guarantees that all instances of `Even` are actually
even.

{% highlight scala %}
scala> Even(2)
res0: Even = Even(2)

scala> Even(3)
java.lang.IllegalArgumentException: requirement failed: Not even: 3
  at scala.Predef$.require(Predef.scala:219)
  ... 34 elided
{% endhighlight %}

Apart from relying on run-time exceptions, the main difference with
type-enforced guarantees is that we are spending execution cycles to check the
property on every object creation. When the check is expensive this simple
solution might fall sort.

We can have more control over the case class initialization by making the
constructor private as follows.

{% highlight scala %}
case class Even private(value: Int)

// This won't compile now:
Even(2)
new Even(2)
{% endhighlight %}

Now we can add factory methods enforcing an even number and provide
invariant-preserving methods without any run-time check.

{% highlight scala %}
case class Even private(value: Int) {

  def multiply(factor: Int) = Even(value * factor)

  def copy(newValue: Int = value): Even = {
    Even.requireEven(newValue)
    new Even(newValue)
  }
}

case object Even {

  def of(even: Int) = {
    requireEven(even)
    new Even(even)
  }

  private def requireEven(even: Int): Unit = {
    require(even % 2 == 0, s"Not even: $even")
  }
}
{% endhighlight %}

Note that we need to close one final loophole: the `copy` method. Unless we
override it, we are exposed to having someone creating a valid object and then
an invalid copy of it.

{% highlight scala %}
scala> Even.of(2).copy(3)
java.lang.IllegalArgumentException: requirement failed: Not even: 3
  at scala.Predef$.require(Predef.scala:219)
  at Even$.$line3$$read$Even$$requireEven(<console>:28)
  at Even.copy(<console>:15)
  ... 33 elided
{% endhighlight %}

We can use this pattern for other purposes like controlling how instances of a
case class are created or hiding some other aspect of them in a module.



