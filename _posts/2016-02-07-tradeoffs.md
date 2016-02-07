---
layout: post
title: "Fragility versus rigidity trade-off"
summary: "One trade-off to consider in the static versus dynamic debate"
category: development
tags: [design]
---
{% include JB/setup %}

Recently I've been involved in a lot of discussions about dynamic
versus static type systems[^debating]. It is very difficult to have
constructive discussions about this subject because it touches
personal identification with tools, lack of knowledge about the
programming language design space, differences in vocabulary, group
thinking, emotions running amuk[^amuk]... 

[^debating]: We had a [podcast episode][nl2], a debate at JobAndTalent, coffee discussions and some mailing list threads...
[nl2]: http://www.spreaker.com/user/nacionlumpen/nl2-dinamico-vs-estatico-eterno-empate
[^amuk]: We developers are passionate about our tools in an exceptional way. Can you imagine a street sweeper spending the weekend configuring its new broom?

In the end people don't pay much attention to other people's points
and when the time for conclusion arrive a variant of *"there are
trade-offs, languages are tools and you should pick the one matching
your problem"* and saying goodbye.

What leaves me deeply unsatisfied is listening to such sensible final
remarks but not having spent most of the time discussing the
trade-offs! I refuse thinking those remarks are polite but empty
sentences not really meant and I blame it just lack of focus.

Leading by example, let's focus on one important trade-off we face when
we move through the dynamic-static axis: rigidity versus
fragility. There are many definitions of the software *-ilities*
(fragility, rigidity, viscosity, opacity...) but let's quote the ones
in an article about design principles[^design]:

> Rigidity is the tendency for software to be difficult to change,
> even in simple ways. Every change causes a cascade of subsequent
> changes in dependent modules.
>
> [...]
>
> Fragility is the tendency of the software to break in many places
> every time it is changed. Often the breakage occurs in areas that
> have no conceptual relationship with the area that was changed.

[^design]: [Design Principles and Design Patterns][dpdp], Robert C. Martin, 2000.
[dpdp]: http://mil-oss.org/resources/objectmentor_design-principles-and-design-patterns.pdf

To keep things concrete, we can use an imaginary codebase with the
same characteristics as the typical system you can find in real life.
Adding features to such codebase is not just writing more code since
you need to make changes to the existing design to accommodate for the
new code.
  
Those changes are more costly as the codebase grows (more than
linearly) and, critically, can become astronomical due to poor code
quality.

On one hand, if you real-life tech-indebted system is written in a
statically typed language, you will face significant rigidity. The
reason is that modifying a tightly coupled module can mean modifying
many, many files in a so called [shotgun surgery][shotgun] because
dependencies are explicit and checked at compile time. Depending on
the environment you won't be able to run tests unless everything
compiles again, making it even more hideous. You can alleviate you
pain with tools like the [mikado method][mikado] and the stuff in
the [Refactoring book][refactoring].

[shotgun]: http://www.c2.com/cgi/wiki?ShotgunSurgery
[mikado]: https://pragprog.com/magazines/2010-06/the-mikado-method
[refactoring]: http://martinfowler.com/books/refactoring.html

On the other hand, the same codebase could be written in a dynamically
typed language. The same far-from-ideal design will pose significant
fragility. Thanks to duck typing, the cross references producing
rigidity in the previous case won't stop you from modifying the code
but you will need to check all of them anyway. You should pay special
attention to all the mocked interactions, reflective uses and
double-check code not covered by tests[^realworld]. As a result,
innocent changes can and do break the code in many places sometimes
far removed from the original change. However, you can delay updating
the whole design (at the expense of inconsistency).

[^realworld]: The dynamic languages community emphasizes unit testing and TDD but practitioners sin.

Assuming all other things are equal[^ceteris], how to pick programming
languages based on this trade-off?

 - System size. As the system grow in size and complexity dependencies
   will be more remote. Here rigidity is a win as it signal when to
   invest in scaling the design.
 - System critically. Prefer fragility for non-critical, throwaway
   systems aimed at demonstrating concepts.
 - System longevity. The more fungible the codebase the more fragility
   you can tolerate.
   
Of course, there is much more to discuss within the umbrella of the
static versus dynamic debate.
 
[^ceteris]: This is not usually the case because many other important aspects will be different when you compare static language X with dynamic language Y.
