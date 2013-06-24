---
layout: page
title: Show me da code
tagline: Talk is cheap
---
{% include JB/setup %}

## Recent posts

{% assign detailed_posts = 3 %}
{% for post in site.posts limit:detailed_posts %}
<div class="post">
  <h3><a href="{{ post.url }}">{{ post.title }}</a></h3>
  <p>{{ post.content | strip_html | truncatewords: 75 }}</p>
  <address class="signature">
    <span class="date">published {{ post.date | date_to_string }}</span>
  </address>
</div>
{% endfor %}

{% if site.posts.size > detailed_posts %}
## Older posts

<ul class="posts">
  {% for post in site.posts %}
  {% if forloop.index > detailed_posts %}
    <li><span>{{ post.date | date_to_string }}</span> &raquo; <a href="{{ BASE_PATH }}{{ post.url }}">{{ post.title }}</a></li>
  {% endif %}
  {% endfor %}
</ul>
{% endif %}
