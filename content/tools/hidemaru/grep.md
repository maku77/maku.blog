---
title: "秀丸: 秀丸で OR 検索する (grep)"
url: "/p/q5w35me"
date: "2011-04-18"
tags: ["エディタ", "秀丸"]
---

秀丸の検索 (grep) 機能で OR 検索をしたいときは、<samp>正規表現</samp>にチェックを入れて、以下のように<samp>検索する文字列</samp>を **`|`** で区切れば OK です。

{{< code >}}
Pattern A | Pattern B
{{< /code >}}

