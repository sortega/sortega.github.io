---
layout: post
title: "When to pair"
author: sortega
summary: "A couple of thoughts on when to pair"
category: peopleware
tags: [practices, patterns]
---
{% include JB/setup %}

<div style="width: 100%; text-align: center; margin: 1em 0">
    <img style="width: 90%" src="/assets/pair_programming/ikea.jpg" alt="Even Ikea knows pair programming works" />
	<p style="text-align: center">
		 by
		 <a href="http://pairprogramminglife.tumblr.com/post/97514880523/even-ikea-knows-pair-programming-works">pair programming life</a>
    </p>
</div>

[Pair programming][pairing], or just pairing, is one of the most
resisted practices that the agile community is pushing to become
mainstream.  Many colleagues have an initial knee-jerk reaction
against it.  "Do you want to waste 50% of the productivity?", they
often argue.

Of course, there are obvious circumstances in which pairing is
blatantly wasteful such as in simple, no-brainer tasks. In my own
experience, there are other circumstances in which it is beneficial:

 * **To deal with error-prone stuff**.  Development is a bit like
   trying to juggle many balls at the same time.  If you miss one it
   might be caught early on by the compiler or the tests... or it
   could ruin your vacations abroad two months from now.  Pairing
   helps here by providing immediate feedback and by having the
   navigator--the one not typing--keeping notes and cross-checking
   documentation.

 * **When you need readable, high-quality code**.  You might determine
   that not the entire codebase needs the highest quality level, but
   even in that case a portion--the most architectural pieces--should
   be carefully crafted.  You should consider pairing for its benefit
   of getting outputs that at least two people understand and are
   likely to have fewer defects than individually-written code.

 * **To spread the word**.  Working side-by-side is a wonderful way to
   learn from colleages not only technologies and conventions but also
   values and team culture.

 * **To handle a situation**.  From time to time you have to work
   under a deadline to fix a situation in which a mistake can have
   further consequences.  The higher the stake, the more you will
   appreciate having a peer helping you to decide if you should cut
   the red or the blue wire.  By the way, many people who are
   reluctant to try pairing in normal situations opt for it during a
   crisis in an implicit recognition of pairing effect on the rate of
   defects.

These are some examples of when I find pairing useful but your mileage
may vary.  In the end, practices should match the task at hand and the
social context, so different teams use different practices for similar
problems.  For some teams, code reviews are a substitute for pairing
that provides a portion of the benefits in less time[^1].

<div style="width: 100%; text-align: center; margin: 1em 0">
    <img style="width: 60%" src="/assets/pair_programming/wtfm.jpg" alt="Good code / bad code" />
</div>


[pairing]: http://www.extremeprogramming.org/rules/pair.html
[^1]: [GitHub](https://github.com/)'s pull-request review system is a great tool for doing this.
