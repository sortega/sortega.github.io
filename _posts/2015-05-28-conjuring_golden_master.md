---
layout: post
title: "Conjuring a golden master"
author: sortega
summary: "Relating my experience pairing to solve the gilded rose kata"
category: development
tags: [deliberate-practice, kata]
---
{% include JB/setup %}

You can [deliberately practice][dp] any aspect of the software development
process, even activities with low or no coding load.  In fact,
[the original series of katas][katas] by Dave Thomas begins with a domain
modeling exercise with no actual coding required.  We should not be shocked by
this because the point is to focus on one aspect and improve at it.

Last week I [paired][pairing] with [Omar](https://twitter.com/oagudow)[^1] to
work on the [Gilded Rose problem][gildedrose] ([our code][code]).  This
problem is extraordinary, not because of being unrelated with coding, but for
its starting point.  Your mission consists on adding a simple case to a
smelly, highly-nested spaghetti mass of legacy code.  The focus is to learn
how to modify existing code in very bad shape with no tests to back you up,
just like the typical working day of the less fortunate developers[^2].

Just like in the real work, you might be tempted to carefully add just the
new, simple case to the existing logic and, as there is no test infrastructure
yet, skip writing automated tests.  But this is very dangerous as you could
not fully understand the complicated code and any regression could silently
hide in your changes.

<div style="width: 100%; margin: 1em 0; text-align: center">
    <img src="/assets/golden_master/handson_approach.gif"
         alt="A hands on approach"/>
    <p style="text-align: center">
      Hands on approach: don't let lack of tests delay you.<br/>
      Lessons from
      <a href="http://www.imdb.com/title/tt0375466/?ref_=fn_al_tt_2">Takeshi's Clastle</a>
    </p>
</div>

The next idea coming into the mind might be to extract pieces from the
spaghetti mass and provide unit tests for them.  This is also risky because
you don't get test coverage until you extract the pieces and by then you might
have screw up.  The recommended approach is to produce an integrated
regression test that is created by producing as many random inputs as possible
and keeping the results of the original code as the truth.  This relation of
inputs and outputs that substitutes our understanding of the code while
refactoring is known as the golden master[^3].

<div style="width: 100%; margin: 1em 0; text-align: center">
    <img src="/assets/golden_master/golden_master.jpg"
         alt="The golden master" style="width: 200px"/>
    <p style="text-align: center">
      All praise to the might golden master.<br/>
      (Actually a <a href="http://legoindianajones.wikia.com/wiki/Golden_Idol">lego idol</a>)
    </p>
</div>

It is pretty standard to use a golden master for this exercise but we decided
to give it a twist by using generative tests with [scalacheck][scalacheck].
Generative testing relies on three principles:

 * **Random input generators.** You don't explicitly write the inputs for your
   tests, they are generated randomly. In the case of scalacheck, a generator
   is anything extending `Gen[A]` and you get number, string and collection
   generators out of the box.

   {% highlight scala %}
val sellInDays = Gen.chooseNum[Int](-10, 10)
val qualities = Gen.posNum[Int]
val itemNames = Gen.oneOf(
  "Normal item",
  "Aged Brie",
  "Sulfuras, Hand of Ragnaros",
  "Backstage passes to a TAFKAL80ETC concert",
  "+5 Dexterity Vest"
)
   {% endhighlight %}

   Generators are composable by means of the classic combinators (`map`,
   `flatMap`, `filter` and the like) and by `for` comprehensions.

   {% highlight scala %}
val items = for {
  name <- itemNames
  sellIn <- sellInDays
  quality <- qualities
} yield new Item(name, sellIn, quality)
val itemArrays = Gen.containerOf[Array, Item](items)
val daysToPass = Gen.posNum[Int]
   {% endhighlight %}

 * **Properties.** Instead of defining concrete inputs and outputs, you define
   the properties (in the sense of rules or laws) that the code should satisfy
   and it is the test runner job to find a counterexample by generating inputs.
   In out case, we use a copy of the original algorithm, `GoldenMaster`, to
   detect any regression.  In other words, our property is *"The refactored
   gilded rose should behave like the golden master"*:

   {% highlight scala %}
"The refactored gilded rose" should "behave like the golden master" in {
  forAll(daysToPass, itemArrays) { (days, items) =>
    val master = new GoldenMaster(items)
    val instance = new GildedRose(items.map(copyItem))
    for (_ <- 1 to days) {
      master.updateQuality()
      instance.updateQuality()
    }
    instance.items.toList shouldBe master.items.toList
  }
}
   {% endhighlight %}

 * **Counterexample minimization.** Once the test runner finds a counterexample
   it tries to find a smaller input that also violates the same property.
   This is important to make this testing strategy useful. A failed test
   showing hundreds of randoms items of different types can be very difficult
   to understand and fix.  Thanks to the minimization process you usually get
   error messages with just one element and small numbers and strings are
   involved. Note the "shrink" labels in this example:

       [info] The refactored gilded rose
       [info] - should behave like the golden master *** FAILED ***
       [info]   TestFailedException was thrown during property evaluation.
       [info]     Message: List(Item(Aged Brie, -5, 17)) was not equal to
                    List(Item(Aged Brie, -5, 25))
       [info]     Location: (GildedRoseTest.scala:34)
       [info]     Occurred when passed generated values (
       [info]       arg0 = 4, // 1 shrink
       [info]       arg1 = Array(Item(Aged Brie, -5, 25)) // 2 shrinks
       [info]     )

    This is a huge advantage over other ways of implementing a golden master
    because you get helpful and minimal error cases instead of random
    monstrosities.

In the end, we didn't introduce any regression bug during our refactoring that
wasn't also caught by the unit tests we were adding to the new classes but
the generative golden master was an important and very cheap[^4] source of
confidence.

[^1]: Not to confuse with [Omar Little][olittle]. We were programming, not robbing street-level drug dealers.
[^2]: If this is the norm in your company and there is no force changing it, either start a rebellion or try to move elsewhere.
[^3]: This technique is known as characterization test in Michael Feathers' [Working effectively with legacy code][legacycode]
[^4]: A few minutes from adding the library dependencies to having the test successfully failing

[dp]: /peopleware/2015/04/21/deliberate_practice/
[pairing]: /peopleware/2015/05/05/pair_programming/
[code]: https://github.com/sortega/GildedRoseWithOmar
[katas]: http://codekata.com/
[gildedrose]: https://github.com/emilybache/GildedRose-Refactoring-Kata
[olittle]: https://en.wikipedia.org/wiki/Omar_Little
[legacycode]: http://www.amazon.com/Working-Effectively-Legacy-Michael-Feathers/dp/0131177052
[scalacheck]: http://www.scalacheck.org/
