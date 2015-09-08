---
layout: post
title: "Functional trees"
summary: "Can you tri-force?"
category: development
tags: [fp, kata, books]
---
{% include JB/setup %}

Recently, a group of coworkers and me have published [a book][historias] on
various themes around development very much in the style of the famous 97
Things Every Programmer Should Know (available as [a wiki][97things]) but in
Spanish and with longer chapters.

One of the chapters accounts for a programming exercise that was proposed just
before last year Xmas.  We were asked to create a tree with zeroes and a star
on the top as follows:

             *
             0
            000
           00000
          0000000
         000000000
        00000000000
       0000000000000
      000000000000000
     00000000000000000

Dozens of solutions were proposed in a very short time and employing almost
a programming language per submission.  Most of the programs were written
by my colleagues with a code-golf mentality, trying to trade readability for
size and brag about their languages of choice. Of course, some of the programs
had more exotic purposes like laughing about PowerPoint programming, the
verbosity of enterprise software or demonstrating MSX assembler among other
nerdy stuff.

We can use this small and simple problem to compare the imperative and
functional solutions for the case in which the tree is parameterized with `n`,
the number of stores or layers or zeroes.

As an example of the imperative approach I reproduce here the submission in
C++ of Luis Osa, a close coworker, edited to accept the number of tree stores
as argument (see the original on the book):

{% highlight cpp %}
#include <iostream>
#include <string>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int n = atoi(argv[1]);
    std::cout << std::string(n, ' ') << "*" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << std::string(n - i, ' ')
                  << std::string(2 * i + 1, '0')
                  << std::endl;
    }
    return 0;
}
{% endhighlight %}

Three main characteristics can be highlighted:

 * *Effectful computation*. The tree is directly written to the standard output
   line by line. That is a side effect, as the returned value is 0.
 * *Looping*. Every tree line but the first is printed by a loop that
   structures the whole computation.
 * *Monolithic logic*. This simple program is structured as single monolithic
   function as it is difficult to extract worthy auxiliary functions.

So far, so good and more readable than the shortest submission in GolfScript
(``" "9*"*"9,{n\.4$>\.+)"0"*}%``) but if this wasn't fire-and-forget code we
would be worried about testability and modularity for the long-term
readability.  The functional style can help here by the following means:

 * *Referential transparency*. In FP style, the tree function would return a
   sequence of tree lines instead of printing it directly as transparent
   functions can only compute results from their inputs and no other thing.
   This will save the day for the testing at the price of constructing the
   necessary datastructures.
 * *Higher order functions*. Low level looping is not necessary as the
   standard library of functions (`map`, `reduce` et al.) provide all the
   "looping" patterns to be parameterized with expressions.
 * *Modular design*. Both referential transparency and the higher order
   encourage the use of small functions that factor out duplicated code. As a
   result, amounts of similar code that wouldn't raise an eyebrow in the
   imperative style are extracted and reused in FP style.

The result of applying this principles using my last toy language, Scala,
follow this principles.

<script src="https://gist.github.com/sortega/5941704.js"> </script>

As you can see, looping has been replaced by the higher-order function
`Seq.tabulate` that accept a function of type `Int => T` to fill a sequence,
in this case the sequence of tree lines.  This design can be easily upgraded
to generate lazy or even infinite trees by changing `Seq` by `Stream`.

Other important point is than several functions and values have been extracted
to modularize the tree generation logic. This is easier to extend/refine with
more details.

And last but not least, a test suite is included since `Tree(n)` is very easy
to test anyway.

Some of this characteristics can be observed on different submissions you can
find on the book: referential transparency on another Scala version by
[Juan de Bravo][juandebravo] and an R version by [Arturo Canales][canales],
and higher order with Haskell by [Ignacio Blasco][elnopintan] and Clojure by
[me][me].


[historias]: http://www.lulu.com/shop/alberto-de-vega-luna-and-rafael-de-las-heras-del-dedo-and-carlos-domingo-soriano-and-juan-lambea-rueda/historias-de-developers/ebook/product-21088065.html
[juandebravo]: https://twitter.com/juandebravo
[97things]: http://programmer.97things.oreilly.com/wiki/index.php/97_Things_Every_Programmer_Should_Know
[canales]: https://twitter.com/kallaghaan
[elnopintan]: https://twitter.com/elnopintan
[me]: https://twitter.com/_sortega
