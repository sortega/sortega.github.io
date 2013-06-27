---
layout: post
title: "Language terseness: comparing Java and Scala"
author: sortega
description: ""
category: development
tags: [java, scala, fp]
---
{% include JB/setup %}

Martin Odersky claims in his book *Programming in Scala*[^scala] that Scala
is terser than Java. Practice confirms this claim as comparable listings tend
to be half or a third of what you need on Java (but your mileage can vary).

Let's look at an example to try to find out what is the cause of such big
measure difference.  The following code is the result of a TDD tutorial by
Uncle Bob[^cc] consisting in creating name-inverter function able to discard
honorifics (e.g. Mr. or Ms.), retain post-nominals and cover several border
cases.  For example, we should get `Odersky, Martin` from `Martin Odersky`.

This is the production code followed by the list of test cases:

<script src="https://gist.github.com/sortega/5841498.js"> </script>

After watching the TDD tutorial and following the same practices but using
Scala and ScalaTest instead of Java and JUnit I got the following result:

<script src="https://gist.github.com/sortega/5841506.js"> </script>

Effectively, line counts meets the expectations.  This is explained in Odersky
book by simply stating that Scala is *higher-level* than Java but, in my
opinion, we can be more precise by looking at the code from different angles:

 * In Java, it is common to pass `null` when a value is missing, that forces
   to defensively code null checks all over the place as it is motivated by
   the first test case of the Java version.  In Scala, the ubiquitous use of
   the `Optional` type for this purpose make this unnecessary.  This removes a
   test case and a conditional for free!

 * Both JUnit and ScalaTest are internal DSLs with the advantage of not
   needing a different compiler than the production code and the inconvenience
   of needing to accommodate to the host language syntax.  As Scala offers lots
   of configurable syntax sugar such as implicit conversions, operators, infix
   notation and so on, the results are much more terse.  The JUnit version
   encodes each test case into a method annotated with `@Test` while ScalaTest
   leverages higher-order functions to define the test cases.

 * All exceptions are unchecked in Scala so there is no need but API
   documentation to use them.  This adds some noise to the JUnit version that
   needs to accept any exceptions to maximize test independence.

 * Primitives and Objects are two different kinds of animals in Java, being
   the former second class citizens unable to participate on all the things
   that the latter can.  For instance, you cannot create a `List<int>` you
   should use a wrapper class and end with `List<Int>`.

   More relevant to the code we are studying, primitive arrays are not part of
   the collections framework and conversions as cumbersome and wordy as you
   can see in `splitNames` (`NameInverter.java:50`).  Scala designers pushed
   for a unified class hierarchy and collections framework in which all the
   types can play all the roles.  Such uniformity promotes orthogonality and
   you need to type `toList` no matter what collection you start with. Compare
   with `new ArrayList<String>(Arrays.asList(x))`.

 * When functions are first class and you can create functions literals
   cheaply and pass them as arguments, lots of common collection manipulation
   patterns can be abstracted and reused.  For example, removing the
   honorifics is implemented with a conditional that removes elements of a
   collection in the Java version. A simple `dropWhile(isHonorific)` can
   do the job in Scala.

 * Pattern matching is used in `swapFirstAndLast` (`NameInverter.scala:8`) to
   *deconstruct* the list in two ways (first-second-rest and any-other-list)
   and manipulate them accordingly.  This is very data-centric and easy to
   grasp visually (to the trained eye, as everything!).

 * Other source of verbosity in Java comes from the lack of some methods in
   their APIs.  In this case the lack of something similar to `mkString`
   forces you to write loops, using `String.format` or recurring to Apache
   Commons or Guava for alternatives.

Most of the reviewed points to the *elements of programming* as explained in
the timeless SICP[^sicp]:

> A powerful programming language is more than just a means for
> instructing a computer to perform tasks.  The language also serves as a
> framework within which we organize our ideas about processes.  Thus,
> when we describe a language, we should pay particular attention to the
> means that the language provides for combining simple ideas to form
> more complex ideas.  Every powerful language has three mechanisms for
> accomplishing this:
>
>  * "primitive expressions"  which represent the simplest entities the
>    language is concerned with,
>
>  * "means of combination" by which compound elements are built from simpler
>    ones, and
>
>  * "means of abstraction" by which compound elements can be named and
>    manipulated as units.

While Scala and Java have very similar primitive expressions their main
differences come from the available means of combination and abstraction.
Richer means of combination like higher-order functions lead to terser code
just by allowing for easier composition of more versatile modules.  Richer
means of abstraction allow for terser code as you don't need to deal with
irrelevant details such as looping a list to selecting some elements.

In my opinion the Java-Scala contrast is a wonderful setting for measuring
the relative merits of the functional-language features that are slowly
pouring into the mainstream.


[^scala]: Programming in Scala, Odersky et al, artima.
[Available at Amazon.](http://www.amazon.com/Programming-Scala-Comprehensive-Step-Step/dp/0981531644)
[^cc]: *Clean Code Episode 19, Part I: Advanced TDD*
   <http://www.cleancoders.com/codecast/clean-code-episode-19-p1/show>
   The whole video series is very interesting and funny to watch!
[^sicp]: *Structure and Interpretation of Computer Programs*,
   Abelson, H. and Sussman, G.J. Full text for free at
   <http://mitpress.mit.edu/sicp/>
