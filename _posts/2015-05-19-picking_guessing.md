---
layout: post
title: "Deliberate picking and guessing"
author: sortega
description: "Introduction to the pick-a-number game from The art of the strategy"
category: development
tags: [deliberate-practice, scala]
---
{% include JB/setup %}

This is the first article of a series about a problem I have been
working on lately as part of my [deliberate practice][practice]
efforts. It is a nice problem that is simple in terms so the entry
barrier is low but, at the same time, you can go very far with it. For
me it is like the typical Pixar movie whose different narrative levels
please both children and adults.

<div style="width: 100%; margin: 1em 0; text-align: center">
    <img src="/assets/picking_guessing/fat-people.jpg" style="width: 80%" alt="Message!" />
    <p style="text-align: center">Those fat people are a message to the adults watching 
    <a href="http://www.imdb.com/title/tt0910970/?ref_=fn_al_tt_1">Wall-e</a>.</p>
</div>

# Background on the problem

The problem was proposed in an obscure programmers' mailing list as a
coding competition in which each participant writes a program to
play a game in their behalf[^1].

The problem was directly inspired by Dixit and Nalebuff's book
*[The art of the strategy][art]*. Within the book ([read that part][excerpt])
they describe a *pick-a-number* game that takes two players: the picker and
the guesser. First, the former should pick a number in the 1 to 100 range and
then, the latter should guess it in at most five attempts. To make things more
interesting, the guesser is told whether each guess is below or above the
target number.

<div style="width: 100%; margin: 1em 0">
    <img src="/assets/picking_guessing/pick-a-number.jpg" alt="Pick a number" style="width: 100%" />
    <p style="text-align: center">Image by <a
    href="https://donttouchmethere.wordpress.com/2015/02/09/pick-a-number/">Mark
    R. Biery</a>
    </p>
</div>

There are 100 points at stake to be distributed among the players.
Guessing the number at the first attempt means that the picker gets
them all and every failure means 20 points less for him. If the number
is not guessed, the picker gains 100 points.

| Attempts                     | Picker        | Guesser        |
|:----------------------------:|--------------:|---------------:|
| 1                            | 0             | 100            |
| 2                            | 20            | 80             |
| 3                            | 40            | 60             |
| 4                            | 60            | 40             |
| 5                            | 80            | 20             |
| Not guessed                  | 100           | 0              |

You can [play][play] the game as guesser on the webpage of the book to get the
idea before continuing.

# Guess my pick

The objective of this programming exercise was to create a program that would
be able to play either role of the game using the language and techniques that
each participant would choose as tools. To make every program understand each
other all of them were required to be a simple text-based line-oriented
command line applications: the greatest common denominator of interfaces.

The only argument for the command is just the role to adopt, either
"pick" or "guess". Then, the interaction consists on reading and
writing lines of text through standard I/O.

When picking numbers, a player just prints the chosen numbers and then
reads some feedback like "not guessed" or "guessed at X". Example
execution (lines preceded by > are inputs typed by me):

        $ ./player pick
        32
        >not guessed
        26
        >guessed at 2
        [...]

Number guessing is more interactive as the program can receive
intermediate ("+" or "-") and final ("=" or "<>") feedback. Example
of program acting as guesser:

        $ ./player guess
        67
        +
        82
        +
        87
        =
        87
        -
        45
        +
        73
        +
        82
        -
        79
        <>
        [...]

The evaluation of the programs, aka "the tournament", was run for two
consecutive months. The first time, the games were isolated with no
memory between executions. The second time, all matches of the same
two programs were running consecutive to allow more elaborate, hide-and-seek
strategies. In both cases, every player was matched against each other 20k
times (10k as guesser, 10k as picker) to tame the inherent randomness of the
game.

What would you would have implemented? How moving from isolated matches to
sequential ones whould have changed your strategy? More on this in the next
post.

[^1]: Not as violent as [robocode](http://robocode.sourceforge.net/)

[practice]: /peopleware/2015/04/21/deliberate_practice/
[art]: http://www.artofstrategy.net/
[excerpt]: http://www.artofstrategy.net/AoSexcerpt.pdf#page=4
[play]: http://www.artofstrategy.net/1to100.html

