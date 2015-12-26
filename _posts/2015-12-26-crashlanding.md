---
layout: post
title: "Crash-landing into Ruby"
summary: "A post-mortem of my first change to the Ruby codebase at JT"
category: development
tags: [ruby, postmortem]
---
{% include JB/setup %}

In my new job at [JT][jt] I'm very fortunate of working in a team in which
different technologies, disciplines and experience levels are melted together.
This is a great opportunity for learning if you don't mind biting the bullet
of going out of your comfort zone.

[jt]: https://www.jobandtalent.com/

The first opportunity for doing so was to take ownership of the
[Ansible][ansible] scripts that deploy the services of our team. It is quite
fulfilling to see how we are getting closer to the devops ideals.

[ansible]: http://www.ansible.com/

This crash-course on Ansible was intense but having used Puppet in the past
not so traumatic. A bigger challenge lies on the fact that 80% of JT's
codebase is written in Ruby and RoR culture permeates everything.  This
includes a non-trivial amount of the code owned by my team so I decided to
speed-learn Ruby to improve the communication and cohesion with my pals.

<div style="width: 100%; margin: 1em 0; text-align: center">
    <img src="/assets/crashlanding/ruby_tattoo.jpg"
         alt="Tattoo of the Ruby logo"/>
    <p style="text-align: center">I won't be surprised if someone at the
        office is tattooed this way.<br/>
        Source: the
        <a href="http://www.worldtattooconvention.com/?q=tattooPhotos&fid=1615"
            >tattoo world convention</a>.
    </p>
</div>

You can pick a new language blazingly fast if you have a solid background
in development including experience on the paradigm of the target language
(single-dispatch, single-thread OOP, *check*), its main features like higher
order programming (things like sandwich code are just the loan pattern,
*check*), reflection (*check*), etc.

What else you need to master? Syntax, semantic subtleties and what is
idiomatic in this new language[^and]. Fortunately you can get all of that by
[deliberately practicing][deliberate] some tutorials and koans. I got a
stupendous return on the [Neo Koans][neo] and the [Metaprogramming
Koans][meta]. I tried hard to understand every new piece of syntax the koans
presented, got immediate feedback and created [flashcards][anki] on the go to
commit those knowledge pills to my memory during the following days[^anki].

[^and]: And, to be fair, the frameworks, libraries and coding standards in use in your company.
[^anki]: Space-repetition systems like [Anki][anki] feel like cheating. Don't tell anyone.

[deliberate]: /tags/#deliberate-practice-ref
[neo]: http://www.rubykoans.com/
[meta]: https://github.com/sathish316/metaprogramming_koans
[anki]: http://ankisrs.net/


<div style="width: 100%; margin: 1em 0; text-align: center">
    <img src="/assets/crashlanding/i_know_ruby.png"
         alt="I know kung-fu... I mean ruby"/>
    <p style="text-align: center">Speed-learning Ruby!</p>
</div>

Apart from being able to better empathize with my colleagues I took a small
issue related with a Ruby service. Slightly changing how ElasticSearch was
invoked from the service didn't look daunting. What could go wrong?

As the relevant piece of code had no test, I tried to add an RSpec. To my
surprise, the query showing up at the test was incomplete, with no reference
to the most important details (this was a [more-like-this][morelikethis] query
and the text to be similar to was missing). I was puzzled because the code had
been in production for a long time without reported issues and the more I
looked at how [Rails Jbuilder][jbuilder] was used to generate the JSON query
the most convinced I got of its correctness.

[morelikethis]: https://www.elastic.co/guide/en/elasticsearch/reference/current/query-dsl-mlt-query.html
[jbuilder]: https://github.com/rails/jbuilder

At this point some coworkers tried to help me. After some cold sweating, a
coworker (thanks [Jose][jfcalvo]) confirmed that the query was correctly
generated in production boxes. Other colleagues taught me useful thinks like
[Pry][pry]-powered debugging (thanks [Francesc][francesc]) but the mystery
remained inscrutable for hours.

[jfcalvo]: https://twitter.com/jfcalvo
[pry]: http://pryrepl.org/
[francesc]: https://twitter.com/francesc_pla

Did I finally give up on the test and implemented the change right away? Nope.
At some point I realized that the magically disappearing part of the query had
`"should"` as key. If you are familiar with Jquery, you will know that it is a
DSL based on dynamic methods in which whatever you call gets transformed in
the attributes of a JSON.

    Jbuilder.new do |company|
      company.name name
      company.president president.to_builder
    end

In the previous example you are calling the undefined methods `name` and
`president` but instead of miserably failing, `method_missing` intercepts the
calls and the attributes are happily populated.

The reason this was working perfectly in production but failing in my test was
RSpec itself! It was monkey-patching every object to enable the `should`
matchers syntax with the unfortunate result of silently disabling the
generation of half of the JSON query[^2].

[^2]: You can disable this monkey patching by [configuration][nopatching].
[nopatching]: https://relishapp.com/rspec/rspec-core/v/3-3/docs/configuration/zero-monkey-patching-mode

<div style="width: 100%; margin: 1em 0; text-align: center">
    <img src="/assets/crashlanding/alien_vs_predator.jpg"
         alt="Monkey patching vs dynamic methods"/>
    <p style="text-align: center">Language features clashing in the wild</p>
</div>

Too much for [my first Ruby use story][firstday]! I finally rolled out a
minitest test and successfully implemented the issue. My takeout: expressive
and flexible languages are great but unsafe mechanisms can bite you hard.
Monkey patching is extremely dangerous because it erodes your ability to
reason about already written code[^alternatives].

[^alternatives]: There are safer alternatives such as Scala's pimps or Ruby's refinements.
[firstday]: https://www.youtube.com/watch?v=9ZlOhSt_qW0
