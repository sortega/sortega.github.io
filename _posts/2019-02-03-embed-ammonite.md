---
layout: post
title: "Finatra meets Ammonite"
summary: "Give Ammonite super-powers to your Finatra server"
category: development
tags: [scala]
---
{% include JB/setup %}

<div style="width: 80%; margin: 1em auto; text-align: center">
    <img src="/assets/embed-ammonite/finatra-meets-ammonite.png" alt="Finatra meets Ammonite"/>
</div>

[Li Haoyi][li-haoyi]'s [Ammonite][ammonite] is the most user-friendly Scala REPL out there.
It has a more than decent syntax highlight, you can edit multi-line snippets, search the command
history, auto-complete expressions and much more!

It's remarkable how you can fiddle with a library without the hassle of creating a SBT project 
thanks to the magic `$ivy` imports. Let's say you want to try out some scalaz 7.2.23 for some
reason:

{% highlight scala %}
$ amm
Loading...
Welcome to the Ammonite Repl 1.1.2
(Scala 2.12.6 Java 1.8.0_161)
If you like Ammonite, please support our development at www.patreon.com/lihaoyi
@ import $ivy.`org.scalaz::scalaz-core:7.2.23`, scalaz._, Scalaz._
import $ivy.$                               , scalaz._, Scalaz._

@ Map('a -> 1, 'b -> 2) |+| Map('a -> 2, 'c -> 1)
res1: Map[Symbol, Int] = Map('a -> 3, 'c -> 1, 'b -> 2)

@
{% endhighlight %}

Importing ``$ivy.`org.scalaz::scalaz-core:7.2.23` `` is equivalent to adding 
`"org.scalaz" %% "scalaz-core" % "7.2.23"` to SBT's `libraryDependencies`.


[li-haoyi]: http://www.lihaoyi.com/
[ammonite]: https://ammonite.io/


Ammonite can be [embedded][embed-doc] in other applications and I find it useful to so when
developing [Finatra][finatra] services.

There are two interesting ways of doing the integration: as a **local development console** for 
debugging and exploration or as a mechanism to inspect or operate against **remote services**.


## Give your Finatra service a development console

If you have ever used Rails and enjoyed it, you will be offended when facing development
environments in which you cannot start a console and tinker with domain services, the database,
etc.  The first step to get Ammonite integrated into Finatra _a la Rails_ is to depend on
Ammonite with test scope so add to your `build.sbt` this:

{% highlight scala %}
libraryDependencies ++= Seq(
  "com.lihaoyi" % "ammonite" % versions.ammonite % Test cross CrossVersion.full
)
{% endhighlight %}

where `versions.ammonite` is a recent version (e.g. 1.4.0).

The idea is to have an alternative main class under `src/test` that will start the service and the
console. To make it easier, you might create an alias in your `build.sbt`:

{% highlight scala %}
fork in (Test, run) := true
addCommandAlias("run-with-console", "test:runMain ammonite.integration.ConsoleLauncher")
{% endhighlight %}

As you can see, we are using the `ammonite.integration` package. This is important because we need
to use some definitions that are package-private and only available from such package.

Then, you should create the launcher class. It should similar to your actual entry point (usually a
server class extending from `com.twitter.finatra.http.HttpServer` and a handful of Guice modules)
but instantiating Ammonite.

Let's take a look at `ConsoleLauncher` bit by bit.

{% highlight scala %}
package ammonite.integration

import com.google.inject.Module

import com.yourcompany.project.infrastructure.Server
import com.yourcompany.project.modules.FooModule
import com.yourcompany.project.modules.BarModule

object ConsoleLauncher extends Server {

  override protected def modules: Seq[Module] =
    super.modules ++ List(
      FooModule,
      BarModule
    )
{% endhighlight %}

Nothing special up to here.

{% highlight scala %}
  private val prelude = """
import scala.concurrent._
import scala.concurrent.duration._

implicit class ExtendedFuture[A](val f: Future[A]) {
  def await: A = Await.result(f, Duration.Inf)
}
"""
{% endhighlight %}

We can add a custom prelude to Ammonite. You can add common imports or even functions and implicit
conversions here. This prelude allows me to easily use services returning `Future` in the console.

{% highlight scala %}
  override protected def postWarmup(): Unit = {
    super.postWarmup()
    try ammonite
      .Main(predefCode = prelude, verboseOutput = true)
      .run(
        "injector" -> injector,
        "foo" -> injector.instance[FooService]
      )
    finally close()
  }
}
{% endhighlight %}

Finally, we instantiate the console. Note that we are exposing the Guice dependency injector so
we can lookup any component of our service in the console. For common things we can directly bind 
then to a name, exactly like the sample `FooService` is bound to `foo`.

{% highlight scala %}
$ sbt run-with-console
[info] Loading settings from idea.sbt,plugins.sbt ...
[info] Loading global plugins from /Users/sebastian.ortega/.sbt/1.0/plugins
[info] Updating ProjectRef(uri("file:/Users/sebastian.ortega/.sbt/1.0/plugins/"), "global-plugins")...
[info] Done updating.
[info] Loading settings from plugins.sbt ...
[info] Loading project definition from /Users/sebastian.ortega/Repositories/foo-service/project
[info] Loading settings from build.sbt ...
[info] Set current project to custom-feed (in build file:/Users/sebastian.ortega/Repositories/foo-service/)
[info] Compiling 12 Scala sources and 0 Java sources to /Users/sebastian.ortega/Repositories/foo-service/target/scala-2.12/classes ...
[info] Done packaging.
Compiling (synthetic)/ammonite/predef/replBridge.sc
Compiling (synthetic)/ammonite/predef/DefaultPredef.sc
Compiling (synthetic)/ammonite/predef/ArgsPredef.sc
Compiling /Users/sebastian.ortega/Repositories/letgo/custom-feed/(console)
Welcome to the Ammonite Repl 1.4.0
(Scala 2.12.7 Java 1.8.0_161)
If you like Ammonite, please support our development at www.patreon.com/lihaoyi

@  foo.checkHealth.await
res1: Health = Health.Sick("cannot reach server")

@
{% endhighlight %}

_Et voilà_, no need to miss the Rails console anymore.

[embed-doc]: https://ammonite.io/#Embedding
[finatra]: https://twitter.github.io/finatra/

## Remote Ammonite powers

The other interesting way to integrate Ammonite into your service is as a remote console. That way
you can connect to a remote server in the stating or even production machines and fiddle with them.
Maybe not a very good idea... but power to the people!

I'm going to demonstrate this approach by introducing a pluggable Guice module that you can include
or not in your server to enable or disable the server.

We will need an additional dependency but this time will have compile scope instead of test scope.

{% highlight scala %}
libraryDependencies ++= Seq(
  "com.lihaoyi"  % "ammonite-sshd" % versions.ammonite cross CrossVersion.full
)
{% endhighlight %} 

And a module named `AmmoniteServerModule` under `src/main/scala`:

{% highlight scala %}
package ammonite.integration

import javax.inject.Singleton

import ammonite.sshd.{SshServerConfig, SshdRepl}
import ammonite.util.Bind
import com.google.inject.Provides
import com.twitter.inject.{Injector, TwitterModule}
import org.apache.sshd.server.config.keys.DefaultAuthorizedKeysAuthenticator

import com.yourcompany.project.modules.FooModule

object AmmoniteServerModule extends TwitterModule {

  private val prelude = """
import scala.concurrent._
import scala.concurrent.duration._

implicit class ExtendedFuture[A](val f: Future[A]) {
  def await: A = Await.result(f, Duration.Inf)
}
"""
{% endhighlight %}

We have some elements in common: same prelude and the class is in the `ammonite.integration`
package. However, this is an independent Guice module.

{% highlight scala %}
  @Provides @Singleton
  def providesAmmoniteServer(injector: Injector, foo: FooModule): SshdRepl = {
    val config = SshServerConfig(
      address = "localhost",
      port = 22222,
      publicKeyAuthenticator = Some(new DefaultAuthorizedKeysAuthenticator(true))
    )
    new SshdRepl(
      config,
      predef = prelude,
      replArgs = List(
        Bind("injector", injector),
        Bind("foo", foo)
      )
    )
  }
{% endhighlight %}

The SSH server is defined as an application singleton (`@Provides @Singleton`) and configured to
listen in localhost on the port 22222 honoring `~/.ssh/authorized_keys` as system's SSH server.
Note also, how the injector and some services can be also bound for the session.

{% highlight scala %}
  override def singletonPostWarmupComplete(injector: Injector): Unit = {
    super.singletonPostWarmupComplete(injector)
    injector.instance[SshdRepl].start()
  }

  override def singletonShutdown(injector: Injector): Unit = {
    injector.instance[SshdRepl].stop()
    super.singletonShutdown(injector)
  }
}
{% endhighlight %}

We need also to start/stop the server using some lifecycle callback methods.

With this in place, we can SSH and enjoy of the same interactive experience as before but
potentially from the other side of the world:

{% highlight scala %}
$ ssh localhost -p 22222
Compiling (synthetic)/ammonite/predef/interpBridge.sc
Compiling (synthetic)/ammonite/predef/replBridge.sc
Compiling (synthetic)/ammonite/predef/DefaultPredef.sc
Compiling (synthetic)/ammonite/predef/ArgsPredef.sc
Compiling /Users/sebastian.ortega/Repositories/letgo/custom-feed/target/pack/(console)
Welcome to the Ammonite Repl 1.4.0
(Scala 2.12.7 Java 1.8.0_161)
If you like Ammonite, please support our development at www.patreon.com/lihaoyi

@ val bar = injector.instance[BarService]
bar: BarService = com.foo.BarService@26cfe278

@ bar.someAction.await
res1: String = "hello world"
{% endhighlight %}
