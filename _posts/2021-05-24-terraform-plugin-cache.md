---
layout: post
title: How to waste less time and space with Terraform
date: 2021-05-24 11:46
category: tools
tags: [iac, howto, trick]
---

I've been using [Terraform][tf] quite extensively at work for some time and,
over time, I've been interacting with more and more modules. At the same time,
I need to work with a rather small HDD for modern standards where every GB
counts.

Terraform's default configuration wastes a lot of space by keeping a separate
copy of the provider binary in each `.terraform/plugins`. In my case that was
about 20 GB.

```bash
$ find . -name .terraform -exec echo {}/plugins \; | xargs du -hs --total
19G
```

However, you can configure Terraform CLI to use a centalized cache directory as
long as your OS supports hardlinks. To do so, create `~/.terraformrc` with
contents `plugin_cache_dir = "$HOME/Library/Caches/terraform/plugin-cache"` or
some other directory that makes more sense to you. [Read more][doc] about 
this configuration.

After that, you can take advantage of the consolidated cache.

```bash
$ mkdir -p ~/Library/Caches/terraform/plugin-cache
$ find . -name .terraform -exec rm -r {}/plugins \;
$ ...some terraform inits...
$ du -hs ~/Library/Caches/terraform/plugin-cache
819M
```

On top of saved spaced you will have faster `terraform init`s.

[tf]: https://www.terraform.io/
[doc]: https://www.terraform.io/docs/configuration-0-11/providers.html#provider-plugin-cache