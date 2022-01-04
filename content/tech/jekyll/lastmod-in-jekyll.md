---
title: "Jekyll (GitHub Pages) でページの最終更新日を表示する"
url: "/p/6ez7nuf"
date: "2019-09-12"
description: "GitHub Pages のサイトジェネレータとして採用されている Jekyll で、ページのフロントマターに最終更新日を入れる方法です。"
tags: ["GitHub", "Jekyll"]
---

Jekyll では、HTML ファイルを生成するために Markdown 形式で記事を作成するのですが、先頭のフロントマターと呼ばれる部分に任意の変数を定義することができます。

下記の例には、記事の作成日 (`date`) と、最終更新日 (`lastmod`) を入れてあります。

{{< code lang="yaml" title="sample.md" >}}
---
title: "ページタイトル"
date: "2015-01-01"
lastmod: "2019-09-12"
---

本文...
{{< /code >}}

これらの情報をレイアウトファイルから参照するには、下記のようにします。
ここでは、最終更新日 (`lastmod`) の情報があればそちらを、なければ作成日 (`date`) を表示するようにしています。
フロントマターでの `lastmod` の指定はオプショナル扱いということです。

{{< code lang="html" >}}
{% if page.lastmod %}
  {% assign lastmod = page.lastmod %}
{% else %}
  {% assign lastmod = page.date %}
{% endif %}

<span class="date">{{ lastmod }}</span>
{{< /code >}}

HTML5 の time 要素や、schema.org の構造化データマークアップを使ってちゃんと SEO 対策するのであれば、下記のような感じでテンプレートを作成するのがよいでしょう。

{{< code lang="html" title="default.html（抜粋）" >}}
{% if page.lastmod %}
  {% assign lastmod = page.lastmod %}
{% else %}
  {% assign lastmod = page.date %}
{% endif %}

<article itemscope itemtype="https://schema.org/BlogPosting">
  <header>
    <h1 itemprop="headline">{{ page.title }}</h1>
    <span>最終更新:
      <time itemprop="dateModified" datetime="{{ lastmod }}">{{ lastmod }}</time>
    </span>
  </header>
  <div itemprop="articleBody">
    {{ content }}
  </div>
</article>
{{< /code >}}

- 参考: [HTML5 の time 要素の正しい使い方](/p/ukama8m)

