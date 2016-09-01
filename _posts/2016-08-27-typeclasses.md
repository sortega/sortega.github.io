---
layout: post
title: "Intro to type classes"
summary: "Introduction to the concept of type class guided by code samples"
category: development
tags: [scala, patterns, design]
---
{% include JB/setup %}

Have you ever wondered what is the essence of object-oriented programming? Some
people will tell you that object orientation is a [non-concept plagued by
epistemological problems][runar] while most of the people will pick different,
sometimes incompatible things.

[runar]: http://blog.higher-order.com/blog/2008/12/04/no-such-thing/

As Paul Graham [points out][graham], the vague concept of OO is typically a
subset of encapsulation, protection, several kinds of polymorphism,
everything-is-an-object-ness, message passing, inheritance, etc. In his ironic
words:

> Because OO is a moving target, OO zealots will choose some subset of this menu
> by whim and then use it to try to convince you that you are a loser.

[graham]: http://www.paulgraham.com/reesoo.html

This very old quote of [Alan Perlis][perlis] can be understood as a critique of
the object-oriented style:

> It is better to have 100 functions operate on one data structure than 10
> functions on 10 data structures.

[perlis]: https://en.wikipedia.org/wiki/Alan_Perlis

In fact, one problem of having rich object models with many classes is that we
lose the leverage that many functions operating on a single type could bring us.
The standard OO tool for this problem is the polymorphism. A single method call
(declared in an interface or by duck typing) can be directed to many different
implementations. In this spirit, Rich Hickey updated the old Perlis idea
(emphasis mine):

> It is better to have 100 functions operate on one **data abstraction** than 10
> functions on 10 data structures.

A magnificent example of this concept is the collections library of his own
programming language, Clojure. With a couple of functions from the about
100 functions summarized on [Clojure's documentation site][seqs], you can
express almost any collection transformation function you ever need to
write[^4clojure].

[seqs]: http://clojure.org/reference/sequences
[^4clojure]: I realized that by solving most problems in [4clojure](https://www.4clojure.com) that way.

However, doing this with classical polymorphism is only easy when you control
the code of all the classes that need to implement the interface. Hickey's can
easily make sets to comply with `ISeq` but you might be frustrated trying to get
a value from a library by creating subclasses (tricky and no always possible) or
the adapter pattern[^mum] (cumbersome and sometimes impractical).

[^mum]: Look mum! A GoF pattern other than singleton!

But there is a technique addressing these problems (and the more general
[expression problem][expression]) called ad-hoc polymorphism. Some programming
languages implement different flavors of it either directly or as a design
pattern.

[expression]: https://en.wikipedia.org/wiki/Expression_problem

## Motivating example in Haskell

Let's start with a Haskell example, as it is the programming language that has
popularized type classes[^confusion] as a form of ad-hoc polymorphism. Imagine
that you need to convert a list of strings into a JSON array.

[^confusion]: *class* in type class has nothing to do with object oriented classes. It should be understood as a family of types that can place a role.

{% highlight haskell %}
toJson1 :: [String] -> String
toJson1 elems = toArray (map quote elems)
    where quote elem = '"' : elem ++ "\""
          toArray elems = '[' : concat (intersperse ", " elems) ++ "]"
{% endhighlight %}

This code works as expected:

    *Main> toJson1 ["a", "b", "c"]
    "[\"a\", \"b\", \"c\"]"

What happens when we want to generalize the input from `[String]` to a generic
list? Most OO programming languages have a `toString` or `to_s` in the top 
type[^object] so you can always convert to string whatever you receive.
However, in Haskell there is no inheritance or subtype relations and so no
`Object.toString` to call, but there is a `show` function. Let's use it and
let the compiler infer the function signature.

[^object]: this base class is usually a dumpster of type unsafe stuff, like Java's comparison operator that allows you to compare `String` against `Int` as is it made sense.

{% highlight haskell %}
toJson2 elems = toArray $ map show elems
    where toArray elems = '[' : concat (intersperse ", " elems) ++ "]"
{% endhighlight %}

    *Main> toJson2 ["a", "b", "c"]
    "[\"a\", \"b\", \"c\"]"
    *Main> toJson2 [1, 2, 3]
    "[1, 2, 3]"
    *Main> :t toJson2
    toJson2 :: Show a => [a] -> [Char]

Now it works both for string and numbers! More interestingly, the `Show a =>`
part of the signature means that this function works with any type having an 
instance of the `Show` type class. Most standard types are show-ables but
functions are not.

    *Main> toJson2 [(+), (-)]
    <interactive>:23:1: error:
        • No instance for (Show (a0 -> a0 -> a0))

To make a new type show-able we just need to instantiate the class for it.
The relevant part of the `Show` class is:

{% highlight haskell %}
class Show a where
  show :: a -> String
{% endhighlight %}

We can introduce some domain specific type and give it a show instance:

{% highlight haskell %}
data Conversation = Conversation { from :: Int
                                 , to   :: Int
                                 , text :: String }

instance Show Conversation where
    show (Conversation from to text) =
      concat [ "{\"from\":", show from, ",",
                "\"to\":", show to, ",",
                "\"text\":", show text, "}" ]
{% endhighlight %}

And it works like a charm!

    *Main> putStrLn $ toJson2 [Conversation 1 2 "hi!", Conversation 2 1 "hello"]
    [{"from":1,"to":2,"text":"hi!"}, {"from":2,"to":1,"text":"hello"}]
    *Main> toJson2 [Conversation 1 2 "hi!", Conversation 2 1 "hello"]
    "[{\"from\":1,\"to\":2,\"text\":\"hi!\"}, {\"from\":2,\"to\":1,\"text\":\"hello\"}]"

However, we are misusing the `Show` instance. Its purpose is having canonical
string representation of data (that you might later parse with `Read`) and, for
most types, is not what we want. By the way, you can get a `Show` instance for
free by using the `deriving` keyword.

{% highlight haskell %}
data Conversation = Conversation { from :: Int
                                  , to :: Int
                                  , text :: String
                                  } deriving (Show)
{% endhighlight %}

    *Main> show (Conversation 1 2 "hello world")
    "Conversation {from = 1, to = 2, text = \"hello world\"}"
    
## Our own class

But nothing precludes us from creating a `EncodeJson` type class instead of
relying on `Show`.

{% highlight haskell %}
class EncodeJson a where
  toJson :: a -> String
{% endhighlight %}

The instances from integers and strings will delegate to `show` and the instance
for `Conversation` can be very similar to the earlier `Show Conversation`. For other types will be able to have arbitrary implementations that will be called
**polymorphically** regardless of having no inheritance mechanism.

At this point we can revisit our original function and use `EncodeJson` instead of `Show`.

{% highlight haskell %}
toJson3 :: EncodeJson a => [a] -> String
toJson3 elems = toArray $ map toJson elems
    where toArray elems = '[' : concat (intersperse ", " elems) ++ "]"
{% endhighlight %}

## Composing type classes

If you look carefully at `toJson3` you will find that it is exactly what you
would expect from the implementation of `EncodeJson` for a list of JSON-able
things. This is a very powerful pattern in which instances of more complex
types are created from instances of simpler ones. You just need to use a type
variable with a bound using the same syntax that in the function (`EncodeJson a
=>`):

{% highlight haskell %}
instance EncodeJson a => EncodeJson [a] where
    toJson elems = toArray $ map toJson elems
        where toArray elems = '[' : concat (intersperse ", " elems) ++ "]"
{% endhighlight %}

This is very powerful since we can uniformly use `toJson` for anything we can
combine from what we have defined. For example, a single string, lists of lists
of strings or a list of conversations.

    *Main> toJson "hi"
    "\"hi\""
    *Main> toJson [["a", "b"], ["a"]]
    "[[\"a\", \"b\"], [\"a\"]]"
    *Main> toJson [Conversation 1 2 "hi", Conversation 2 1 "hello"]
    "[{\"from\":1,\"to\":2,\"text\":\"hi\"}, {\"from\":2,\"to\":1,\"text\":\"hello\"}]"

## Flexibility and separation of concerns

One notable feature of type classes is that you can define class instances for
types defined in other modules and for types you have no control at all. You
might write a `EncodeJson` for a type from a third-party calendar library that
knows nothing about your API format.

You might want to have instances defined in a separate module to foster
separation of concerns. For example, you might have a core module with types
that model the domain and two other different modules defining how to map them
to JSON and how to persist them.

## Parting thoughts

Type classes are a powerful tool to build composable software, specially in
languages that implements them like Haskell or Rust. They are also extremely
useful in Scala despite being implemented as a design pattern on top of
implicits. They are more verbose but, believe it or not, more flexible than
Haskell's ones in some respects.

We will take a look at type classes in Scala in an upcoming article. Meanwhile, you can see the complete code of this one in this [gist][gist].

[gist]: https://gist.github.com/sortega/27f011a49d62305836d74bc2ba2bddeb
