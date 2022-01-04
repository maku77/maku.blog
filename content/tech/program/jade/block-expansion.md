---
title: "Jadeの構文: ブロック展開 (block expansion) によってネストを減らす"
url: "/p/ef8i8dx"
date: "2013-12-29"
tags: ["Jade"]
weight: 203
---

Jade の**ブロック展開 (block expansion)** を使うと、改行とインデントによるネストを一行で表現できます。
例えば、

{{< code >}}
div.box
    div.prompt Hello
{{< /code >}}

というネストされた構文は、下記のように一行で記述することができます。

{{< code >}}
div.box: div.prompt Hello
{{< /code >}}

ただし、以下のように、上位の要素にテキストノードが存在する場合は、ブロック展開を使った記述は行えません。

{{< code >}}
div.box Hello
    div.prompt World
{{< /code >}}

{{< code title="間違い" >}}
div.box Hello: div.prompt World
{{< /code >}}

