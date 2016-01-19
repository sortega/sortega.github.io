---
layout: post
title: "Diffs are not enouggh"
summary: "Ruminations after being at a JT tech talk"
category: development
tags: [practices, tools, swift]
---
{% include JB/setup %}

At today's internal [JT tech talk][jt] we had some tangential remarks
on syntax highlighting in Github diffs. It was a snazzy presentation
on Swift by [Daniel Garcia][filito] and [Luis Recuenco][luis] and the
former was pointing out that it was sometimes difficult to tell apart
base classes from traits just looking at the code.

[jt]: https://twitter.com/jobandtalentEng
[filito]: https://twitter.com/fillito
[luis]: https://twitter.com/luisrecuenco

<blockquote class="twitter-tweet tw-align-center" lang="en"><p lang="en" dir="ltr">Today as part of our <a href="https://twitter.com/hashtag/jttechtalks?src=hash">#jttechtalks</a>, <a href="https://twitter.com/fillito">@fillito</a> and <a href="https://twitter.com/luisrecuenco">@luisrecuenco</a> introducing <a href="https://twitter.com/SwiftLang">@SwiftLang</a> ! <a href="https://t.co/12dRZM4R0u">pic.twitter.com/12dRZM4R0u</a></p>&mdash; JT Engineering (@jobandtalentEng) <a href="https://twitter.com/jobandtalentEng/status/689470237960486913">January 19, 2016</a></blockquote>
<script async src="//platform.twitter.com/widgets.js" charset="utf-8"></script>

Immediately, the slick audience asked whether the community had
settled with some naming convention to sort this issue out.  Almost as
automatically, I saw the parallel with the old practice of using a
prefix for member variables in C++/Java and the remarks of Uncle Bob
in Clean Code[^cc]:

[^cc]: "Clean Code", Chapter 2: Meaningful Names, Robert C. Martin and Timothy R. Ottinger.

> You also don’t need to prefix member variables with `m_`
> anymore. [...] And you should be using an editing environment that
> highlights or colorizes members to make them distinct.

So I proposed exactly that: "Why not opening a feature request on
whatever-IDE-you-use issue tracker asking for such highlighting?"
"What about Git diffs?", another colleague replied.

Like any standard Simpsons chapter, that brings us to the actual point
of this post: to what extent you can review PRs by just inspecting the
diff? (No matter how well highlighted).

For small or simple changes the diff should be enough, even more after
Github implemented diff context expansion.

<div style="margin: 1em 0; text-align: center">
    <img src="/assets/diffs/expand.gif" alt="Expanding diff context"
         style="width: 85%"/>
    <p style="text-align: center">
      Image from <a href="https://github.com/blog/1705-expanding-context-in-diffs">Github's blog</a>
    </p>
</div>

However, by reviewing plain diffs without the ease of navigation and
other goodies that we enjoy when having the code locally we risk doing
shallow reviews. It is pretty sad reporting a bunch of style nits
while we miss some architectural problem.

I download the changes when they are complex and/or I suspect a
package has grown too much or weird module dependencies[^lazy]. Do you
do the same?

[^lazy]: Unless I feel lazy.
