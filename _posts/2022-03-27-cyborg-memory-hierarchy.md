---
layout: post
title: A bicycle for your memory
summary: How to improve and extend your memory with technology in a way that I have coined as the cyborg memory hierarchy.
date: 2022-03-27 12:00
category: other
tags: [concept, memory]
---
{% include JB/setup %}

Computers have the need to store huge amounts of information and, at the same time, being able to access that memory 
at blazingly fast rates. This poses the problem of how to get a fast and large memory when hardware memories are either fast, like registers or L1 cache lines; or big, like hard drives or solid state drives.

<table style="width: 75%; margin: 1em auto; font-size: 80%">
    <thead>
        <tr>
            <th>Memory</th>
            <th>Size</th>
            <th>Speed</th>
            <th>Comment</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>CPU register</td>
            <td>A few bytes</td>
            <td>One CPU cycle (200-300 picoseconds)</td>
            <td>A register holds the data the CPU can operate on</td>
        </tr>
        <tr>
            <td>RAM memory</td>
            <td>A few gigabytes</td>
            <td>100 nanoseconds</td>
            <td>Applications need their data to be here to be responsive</td>
        </tr>
        <tr>
            <td>Hard drive</td>
            <td>Many gigabytes</td>
            <td>150 microseconds</td>
            <td>All your applications and data being kept locally have a copy here</td>
        </tr>
        <tr>
            <td>Internet</td>
            <td>Infinite (for all purposes)</td>
            <td>10-40 milliseconds</td>
            <td>Technically <i>other people's hard drives</i></td>
        </tr>
    </tbody>
</table>

This is solved in computer architecture by the interplay of two ideas: a **memory hierarchy** and **virtual memory**.
The [memory hierarchy][mem_hier] consists of a pyramid with the fastest and smallest memories on the tip and slower 
but bigger ones at the base. This arrangement allows you to have a complete copy of the information at the base, which is the biggest. Then, you can keep copies of the actively used information closer to the pyramid apex for better performance. [Virtual memory][virtual] is the illusion of having a single memory space which is the result of the collaboration of specific hardware (MMU, CPU features) with the operative system to transparently move data across the levels of the pyramid. The result is that, for most purposes and most of the time, programs running in your computer get to use a single memory space that feels as big as the base of the pyramid and almost as fast as its top.

<div style="margin: 1em 0; text-align: center">
    <img src="/assets/cyborg/memory_hierarchy.png" 
         alt="Memory hierarchy with 4 levels: CPU registers, CPU cache, RAM memory and hard drive"
         style="width: 75%"/>
    <p style="text-align: center; font-size: 90%">
    Simplified memory hierarchy
    </p>
</div>

Apart from computers, humans are rich information processors[^human_comp] and memory plays a paramount role in what 
it means to be a human. On one hand it is an important element of personal identity. Imagine a movie villain using 
technology or black magic[^magic] to swap the memories of two twin brothers. In some philosophical sense we are what we remember, and we will have a hard time deciding who is who in this contrived situation.

On the other hand, memories are needed for our minds to work because without them, we will have only the raw input of our senses and no other concept or idea to work with. The extent to which we can understand the world, communicate with others and act depends on using those mental building blocks or [chunks][chunking].

Another aspect of being human not so directly related to information processing is tool making and tool use. Thanks 
to these, we have come a long way since we harnessed the power of fire and stone. Steve Jobs eloquently explained in 
[this talk][jobs] how humans will lose in almost all categories[^best] of an Olympic Games in which other animals 
were allowed to take part. We are not the fastest, we cannot travel the furthest, nor in the most efficient way. 
However, any cyclist is more efficient in energy per distance than a Condor, the most efficient animal. Then, he 
declared computers to be like _a bicycle for the mind_. Back then, personal computing was being rolled out, and we 
were oblivious to the directions these new machines would cybernetically extend us.

## The Cyborg Memory Hierarchy

At this point we have all the ingredients to define what I call the **cyborg memory hierarchy**: extending human capabilities with technology (cyborg) so we can enhance in-brain memory and extend it with external information (memory hierarchy).

<div style="margin: 1em 0; text-align: center">
    <img src="/assets/cyborg/cyborg_memory_hierarchy.png" 
         alt="Cyborg memory hierarchy with the levels described in the main text"
         style="width: 90%"/>
    <p style="text-align: center; font-size: 90%">
    Cyborg memory hierarchy
    </p>
</div>

### Level 0. Working memory

Our working memory is composed of a phonological loop, where you can keep repeating a phone number while you search 
for pen and paper; the visuo-spatial sketchpad, where you can mentally manipulate shapes; and some very 
limited number of slots that can hold ideas, or [chunks][chunking], for manipulation. Early George A. Miller work 
estimated that the typical mind was able to hold 5 &plusmn; 2 chunks[^magic_number], but more recent work suggests we 
are limited to about 4 chunks.

I doubt we will cybernetically extend our working memory until brain-computer interfaces become a reality, but I 
have some good news in the methodology front. The reason Miller got initially confused with the amount of mental 
slots is that we can do more with less by having more elaborate and high level chunks. For example, I might be able 
to work in my mind with only three digits (e.g. 7, 1, 4) but if they are dates, then I can hold many more digits 
(e.g. 1492, 2001, 1991).

In fact, you can see expertise in any field as the slow and effortful learning of ever higher-level chunks and 
mental models (read [Peak][peak] by Anders Ericsson). My advice: hunt all the important concepts in your 
field of interest and commit them to long-term memory (see next section).

### Level 1. Long-term memory

These are memories that we can use fluently, regardless of our smartphone batteries being dead. These are critical for anything we do with fluency like speaking a language, being expert at anything, or being creative, which implies [recombination of ideas that should be at the same time in our minds][creativity].

Some of these memories are _declarative_ and can be expressed as answers to explicit questions. "What's the capital of Georgia? [Tbilisi][tbilisi] or Atlanta". We can extend our declarative memories by reading and studying for which there is a wealth of technologies and channels but the main enemy to beat is how fast those new memories fade away from us.

We know thanks to Hermann Ebbinghaus and his tedious self-experimentation with memorizing long sequences of nonsense 
words that facts decay exponentially in our minds unless reviewed and inter-related to other memories. His work was published in the XIX century, but no specific software was created to optimize when to review which piece of knowledge until Piotr Wozniak wrote the [first spaced repetition software (SRS)][supermemo] in 1987. The underlying idea of this kind of software is that you encode what you have already understood and learned and want to keep in you memory as flash cards (short questions and answers) and the system keeps track of when to do reviews. Due to Ebbinghaus forgetting curve, the interval between reviews grows exponentially, so you see the typical card again after months or years. This means that you can keep many tens of thousands of cards in your level-zero memory at the cost of a few minutes of daily review.

For a nice explanation of how to implement this with [Anki][anki], one of the most popular SRS nowadays, I recommend reading [Augmenting Long-term Memory][ltm] by Michael Nielsen. He is also experimenting with articles that embed an SRS like [this one][quantum_country] about quantum computing.

If you want to read more about good practices with SRS, Wozniak's [Twenty Rules of Formulating Knowledge][20rules] is the most authoritative source and Soren Bjornstad's [Rules for Designing Precise Anki Cards][soren_rules] is a great complement.

Paraphrasing Nielsen, *SRS makes declarative memory a choice*. If we ever think that keeping a fact or a concept available in our mind is more valuable than 5 minutes of our time[^gwern] we can do it[^uni].

What about non-declarative memory? Muscle memory cannot be reduced to declarative facts, no one is able to learn how to bike by reading _How to ride a bike_. However, some creative thinking can help us to bridge the gap in some cases. For example, I use Anki to learn application shortcuts declaratively and then, as opportunity to use them in the day to day happens I build the muscle memory.

### Level 2. Personal notes and annotations

When we process information we tend to take notes or perform annotations in the medium itself, so we can come back to them without needing long-term memorization. Note that technically, pen and notebook are already cybernetic extensions for this purpose.

These notes are indeed very useful for us to solidify our understanding and organization of ideas, specially if we do a write-up like a blog post. The mere act of writing, even at book marginalia, also improves our initial memorization.

However, the value of these notes is limited unless they are connected and easily searchable. To get that extra value we need to up both our methodological and technological games.

The quintessential technology to fill this gap is the personal wiki in which you can seamlessly write notes, keep 
them inter-linked and securely access this second brain from all your devices. There is an inexhaustible offer of wikis out there ([open source][wikimatrix] and [SaaS][alternatives]). My recommendation is to pick one and try to use it unless blocked by a showstopper. A very common failure mode is to keep tuning your wiki configuration without really using it.

I personally use [TiddlyWiki][tiddlywiki][^tw] with the amazing Soren's [TiddlyRemember plugin][tiddly_remember] to define SRS cards directly from within my wiki notes. When reviewing them in Anki I get a valuable link to their original context.

On the methodology front, I recommend the philosophy of [Zettelkasten][zettelkasten] that was introduced by Niklas 
Luhmann, a very prolific German social scientist that implemented the logical equivalent to a wiki for his research 
notes using Victorian technology.  Luhmann was carefully digesting other academics books and articles into chunks or 
ideas written in index cards that were linked, tagged and annotated to form a web of knowledge that he traversed in a 
myriad ways, leading to serendipitous connections bringing a deeper understanding—and many publications! By the way, 
_zettel_ is German for note while _kasten_ means box.

<div style="margin: 1em 0; text-align: center">
    <img src="/assets/cyborg/zettlekasten.png" 
         alt="Drawing of furniture to hold index cards in small drawers"
         style="width: 80%"/>
    <p style="text-align: center; font-size: 90%">
    How a personal wiki looks like in steampunk style
    </p>
</div>

Soren Bjornstad adapted TiddlyWiki for Zettelkasten and he extensively explains the result in [this video][soren_zk_video]. The template is [available][soren_zk_template] for anyone to use.

### Level 3. Selected sources and everything else

At this level, we have all the source material—books[^plato], articles—we collected and totally or partially read and quite likely you took notes and made flashcards about. At the mid-level of technology, you can picture cozy bookshelves at home but to give you more value, you need to get it organized and searchable.

For this purpose I use [Calibre][calibre], which was presented to me as _the iTunes of ebooks_. Any other ebook organizer will do as long as you have a nice desktop search engine, so you can easily get back to the tidbits of your books that are neither in you head nor your notes. The spotlight search of Mac OS X is a good starting point for this.

Apart from these selected sources, we have ad-hoc information needs which we satisfy by using general search engines.
Google, Bing, DuckDuckGo... choose your poison but once chosen, learn to use it properly
([1][lifehack_search], [2][gwern_search]).

### Does it make sense a partial implementation of this Cyborg Memory Hierarchy?

I think that any missing piece is problematic.

If you have SRS without a personal wiki you will tend to either create flashcards for things that are not really meant for that, and you might burn out; or otherwise, you might let go many valuable things.

If you have a wiki without SRS you won't be fluid enough to take advantage of many things. For example, you can have great coding design patterns documented in your notes and never remember to apply them when the right conditions apply.

If you only have a search engine, you might not even know what to search about to solve a problem and your thinking will tend to be more shallow (related book: [The Shallows: What the Internet is Doing to Out Brains][shallows]).

### How Neuralink or some other technology will change this?

I have no idea. I can only speculate that tighter integration with our brains can collapse some levels or open new 
ones that we do not even understand today. Maybe the Neuralink-enabled Anki feels exactly like Neo's learning Kung 
Fu in The Matrix.

### Your personal cyborg memory hierarchy

If you have some concept or implementation similar to the cyborg memory hierarchy or this post has inspired you to 
implement one for yourself I am very interested in knowing about your experiences. Please write a comment about it!

[^human_comp]: This used to be a false dichotomy as _computer_ was a job title for humans! Until the invention of the modern electronic computer, a _computer_ was a person—most likely a woman—skilled enough at mathematical computation to, for example, slowly tabulate a logarithm table.
[^magic]: "Any sufficiently advanced technology is indistinguishable from magic" (Arthur C. Clarke)
[^best]: One exception is that we can keep walking and running sustainably way longer than other animals. When we were hunter-gatherers [persistence hunting][persistence_hunting]—hunting by exhausting your prey—was a thing. Another exception is our eye-to-hand coordination for throwing rocks and spears. I suppose people less apt at these things were wiped out from the gene pool by Mother Evolution.
[^uni]: What a great pity not having had SRS in my tool belt during my university years. I would now remember more of the original content, and I would not have needed re-learning it years after my graduation.
[^gwern]: Gwern estimates in [about 2 minutes][gwern] the total time that it takes remember a fact _foreverish_ with Anki. I am doubling that estimate to compensate form the rest of us not being as bright as him.
[^tw]: One of the strong points of TiddlyWiki is how you can deeply customize it for your purposes without leaving its markup language and dropping to JavaScript. Its plugin mechanism is also based in the same customizations that a regular user can afford and that means the small community around it has published a huge amount of plugins that you can mix and match.
[^plato]: A bookshelf was considered disruptive technology in times of Plato and Plato considered [books will make people forgetful][plato]. We can forgive him because there was no SRS back them to compensate for this effect.
[^magic_number]: This is the origin of the _magic number 7_ (5 + 2) that is used to prescribe no more than 7 levels or class inheritance, no more than 7 attributes per class, no more than...

[20rules]: https://www.supermemo.com/en/archives1990-2015/articles/20rules
[alternatives]: https://alternativeto.net/software/notion/?feature=wiki&platform=software-as-a-service
[anki]: https://apps.ankiweb.net/
[calibre]: https://calibre-ebook.com/
[chunking]: https://en.wikipedia.org/wiki/Chunking_(psychology)
[creativity]: http://www.pearlleff.com/in-praise-of-memorization
[gwern]: https://www.gwern.net/Spaced-repetition#how-much-to-add
[gwern_search]: https://www.gwern.net/Search
[jobs]: https://youtu.be/ob_GX50Za6c
[lifehack_search]: https://www.lifehack.org/articles/technology/20-tips-use-google-search-efficiently.html
[ltm]: http://augmentingcognition.com/ltm.html
[mem_hier]: https://en.wikipedia.org/wiki/Memory_hierarchy
[persistence_hunting]: https://en.wikipedia.org/wiki/Persistence_hunting
[peak]: https://www.goodreads.com/book/show/26312997-peak
[plato]: https://fs.blog/an-old-argument-against-writing/
[quantum_country]: https://quantum.country
[shallows]: https://www.goodreads.com/book/show/9778945-the-shallows
[soren_rules]: https://controlaltbackspace.org/memory/designing-precise-cards/
[soren_zk_template]: https://sobjornstad.github.io/tzk/
[soren_zk_video]: https://www.youtube.com/watch?v=GjpjE5pMZMI
[supermemo]: https://super-memory.com/english/history.htm
[tbilisi]: https://en.wikipedia.org/wiki/Tbilisi
[tiddly_remember]: https://sobjornstad.github.io/TiddlyRemember/
[tiddlywiki]: https://tiddlywiki.com/
[virtual]: https://en.wikipedia.org/wiki/Virtual_memory
[wikimatrix]: https://www.wikimatrix.org/
[zettelkasten]: https://writingcooperative.com/zettelkasten-how-one-german-scholar-was-so-freakishly-productive-997e4e0ca125
