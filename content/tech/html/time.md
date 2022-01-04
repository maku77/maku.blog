---
title: "HTML5 の time 要素の正しい使い方"
url: "/p/ukama8m"
date: "2019-04-27"
tags: ["HTML/CSS"]
---

time 要素とは
----

HTML5 には、機械（コンピュータ）の読み取りのための要素として `data` 要素が定義されています。
中でも時刻はよく使用する情報なので、特別に **`time`** 要素として定義されています。

この要素を記事内に含めておくことで、`article` 要素の作成日時や、ページ自体の作成日時を表現することができます。
正しいフォーマットで記述しておけば、検索エンジンの検索結果に日付を表示してもらえるかもしれません。
SEO 対策のためにも、`time` 要素を正しく記述できるようにしておきましょう。

time 要素の記述方法
----

{{< code lang="html" >}}
<time datetime="日時情報">表示される日時</time>
{{< /code >}}

`time` 要素のフォーマットは上記のようになっており、`datetime` はオプショナルな属性です。
`datetime` を省略する場合は、`表示される日時` の部分が、[RFC 3339 - Date and Time on the Internet](https://tools.ietf.org/html/rfc3339) で定義された形式で記述されている必要があります。
典型的な書き方は下記のような感じ。

| 書式 | 意味 |
| ---- | ---- |
| `2019-04-27` | 日付（UTC あるいは日本時間） |
| `2019-04-27T21:30Z` | 日付＆時刻（UTC） |
| `2019-04-27T21:30+0900` | 日付＆時刻（日本時間） |

逆に、`datetime` オプションに上記のような日時情報を含めておけば、`表示される日時` の部分には任意のテキストを指定することができます。
なので、下記の 2 つの `time` 要素はどちらも正しい記述です。

{{< code title="正しい書き方" lang="html" >}}
<time>2019-01-01</time>
<time datetime="2019-01-01">2019年の元日</time>
{{< /code >}}

`datetime` 属性を省略している場合は、`表示される日時` の部分を独自形式で記述してはいけません。

{{< code title="間違った書き方" lang="html" >}}
<time>2019年の元日</time>
{{< /code >}}

正しく `time` 要素を使って、人にも機械にもやさしい Web サイトを作成したいですね！


schema.org の構造化データマークアップ
----

HTML データに、より明確な意味を持たせるための [schema.org](https://schema.org/) の構造化データマークアップというものがあります。
この情報を付加しておくことで、検索エンジンなどはさらに詳しくサイト情報を表示できるようになります。

日付情報には、BlogPosting タイプなどの `datePublished` や `dateModified` プロパティを付けておくのがよいでしょう。
下記は 1 つのブログ記事をマークアップする例です。

{{< code title="scheme.org の構造化データマークアップを付加" lang="html" >}}
<article itemscope itemtype="https://schema.org/BlogPosting">
  <header>
    <h1 itemprop="headline">記事タイトル</h1>
    <span>最終更新:
      <time itemprop="dateModified" datetime="2019-01-01">2019-01-01</time>
    </span>
  </header>
  <div itemprop="articleBody">
    記事本文
  </div>
</article>
{{< /code >}}


参考リンク
----

- [time – date and/or time (NEW) - HTML5](https://www.w3.org/TR/2012/WD-html-markup-20120329/time.html)
- [The time element — HTML5: Edition for Web Authors](https://www.w3.org/TR/2011/WD-html5-author-20110705/the-time-element.html)
- [HTML: Hypertext Markup Language | MDN](https://developer.mozilla.org/en-US/docs/Web/HTML/Element/time)

