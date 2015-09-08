---
layout: post
title: "The architecture metaphor"
summary: "Death and rebirth of the architecture metaphor"
category: development
tags: [design]
---
{% include JB/setup %}

The software world is rife with colorful metaphors, so rich in fact
that the following except from Code Complete[^cc] wont surprise any
developer:

> Computer science has some of the most colorful language of any
> field. In what other field can you walk into a sterile room,
> carefully controlled at 68ºF, and find viruses, Trojan horses,
> worms, bugs, bombs, crashes, flames, twisted sex changers, and
> fatal errors?

Some metaphors are so ingrained in our psyche that it is difficult to
sidestep them. Instead of thinking about my hard drive as a long
spiral of magnetic domains, I can only see a hierarchical structure of
files and folders. That is convenient and, sometimes, future-proof as
we can replace the long magnetic spiral by a flash memory without
changing our mental model.

Quite often, software development is explained, understood, organized
and exercised in terms of a parallel with the architecture
discipline. Within this cosmology there are two discrete
phases. First, an initial design phase in which a software architect
or team of architects forge the big ideas and take harsh decisions
that will be costly to revert. Then, the construction phase consists
on translating the design artifacts into code by the addition of
menial details by the less skilled construction workers, junior
developers in the enterprise parlance.

<div style="margin: 1em 0; text-align: center">
    <img src="/assets/architecture/blueprint.jpg" alt="Blueprint"
         style="width: 70%"/>
		 <p style="text-align: center">
		 By <a href="https://www.flickr.com/photos/hoshi7/">Babi Krishna</a> <a href="https://creativecommons.org/licenses/by-nd/2.0/">CC BY-ND 2.0</a>
    </p>
</div>

This metaphor is indeed at the core of how software engineering is
conceived. However, something feels wrong when you look at the
unrealized promises of this vision of our field. In this
[talk][perrotta] on the history of software engineering, Paolo Perrota
elaborates on the reasons of this failure. How we have misunderstood
and overstretched the architecture metaphor is one of them.

The crux of the misunderstanding consists on what in software
development is analogous to the blueprints and other design
documents. Contrary to the popular belief, architectural diagrams and
other similar documents don't fit the bill because you cannot
translate them mechanically to the final product, a working software
system. As Jack Reeves explained with detail in
[a seminal article][code-design] the only one artifact equivalent to
the design documents is the source code.

This might be hard to swallow[^ram] but once you do, interesting
insights can be derived from our improved version of this architecture
metaphor 2.0. If the code is the design, then the construction phase
is what happens when we press the *build* button of our IDE, type
`make` or `mvn package` or similar. This is done in almost no time and
at a negligible cost[^1].

Quite often, we developers self-deprecate ourselves as not being as
rigorous and structured as architects and engineers because they draft
models, compute the resistance of their structures, build prototypes
at scale and many similar things before passing to the construction
phase. Stop feeling ashamed. If building a bridge and trying it with
as many tons of weight as required was virtually free, they will do it
again and again as you run tests on your continuous integration
machine. Imagine the impact of being able to build and tear down
skyscrapers just to tweak the skyline of a city. In fact, testing the
real thing is better than using approximate models so you can look
down to the next engineer you find at a social party.

<div style="margin: 1em 0; text-align: center">
    <img src="/assets/architecture/bridge.jpg" alt="A Lego bridge"
         style="width: 70%"/>
		 <p style="text-align: center">
		 Cute, but I can have the real think for free.
		 By <a href="https://www.flickr.com/photos/liwnik/">Liwnik</a>
		 <a href="https://creativecommons.org/licenses/by-nd/2.0/">CC BY-ND 2.0</a>
    </p>
</div>


Other surprising consequence of architecture 2.0 is that all
developers, junior or senior, "architects" or foot soldiers, we all
are designing side by side. I'm sorry for the arrogant "architect", his
social pyramid has just collapsed and he has no laborers below him
but draftsmen rubbing elbows with him. Consequently, the typical
enterprise division of labor loses its sense. High-level design is
not mechanically translated into low-level design, it is instead
refined iteratively and details can and should influence higher-level
decisions.

Many more ideas sprout from architecture 2.0:

 * We know that documentation ages very fast and it is very difficult
   to be kept synced with the code. If the actual design is the
   source code, why not extracting other design documents
   automatically from it?
 * Going even further, if your language is expressive enough, would it
   be possible to represent your high-level architecture as a small
   subset of your code?
 * We should be suspicious of far-removed-from-code architects. How
   can they take so big responsibility on the design (i.e. the code)?
 * As well as construction, software distribution has also a
   negligible cost. This makes the software ecosystem very different
   to other technology-intensive markets.

In conclusion, metaphors can be enlightening but they can also be
limiting, misleading or just damaging. Choose them with care.


[^1]: You might complain that maven takes forever but it happens instantly compared with the months that building a bridge takes.
[^ram]: Seriously, the code is the design should be as obvious as [this][ram].
[^cc]: [Code Complete][code-complete], Chapter 2: Common Software Metaphors by Steve McConnell
[perrotta]: https://www.youtube.com/watch?v=9IPn5Gk_OiM
[code-complete]: http://cc2e.com/
[code-design]: http://www.developerdotstar.com/mag/articles/reeves_design.html
[ram]: https://www.youtube.com/watch?v=NdREEcfaihg
