---
layout: post
title: "Summing for fun"
summary: "How to sum doubles in Java, 10 versions"
category: development
tags: [java]
---
{% include JB/setup %}

Selection processes for software developers can get very
colorful. Sometimes, lots of weird stuff unrelated with the actual job
get asked by people that can't tell apart code from magic spells.

However, what intrigues me the most are the nonsensical "technical"
questions made by allegedly technical interviewers.

Recently, some friend of mine got asked to implement in any
programming language a function summing a sequence of floating point
numbers. The catch? You should implement at least 10 versions and they
should be as different as possible.

I didn't see what they were after. You can ask for a very simple thing
like summing numbers in the spirit of [FizzBuzz][FizzBuzz], but why
asking for many different versions?

[FizzBuzz]: http://blog.codinghorror.com/why-cant-programmers-program/

The funny thing is that the job opening was related with Java
development and thanks to the accidental complexity of arrays vs
collections and the streams of Java 8 you can have many, many ways of
summing numbers. For example:

<script
src="https://gist.github.com/sortega/d5bc1785db97eba14d31.js"></script>

I wonder if, after all, this was deliberate... 
