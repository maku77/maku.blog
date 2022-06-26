---
title: "Jadeの構文: ブロック展開 (block expansion) によってネストを減らす"
url: "/p/ef8i8dx"
date: "2013-12-29"
tags: ["Jade"]
weight: 203
---

Jade の __ブロック展開 (block expansion)__ を使うと、改行とインデントによるネストを一行で表現できます。
例えば、

```css
div.box
    div.prompt Hello
```

というネストされた構文は、下記のように一行で記述することができます。

```css
div.box: div.prompt Hello
```

ただし、以下のように、上位の要素にテキストノードが存在する場合は、ブロック展開を使った記述は行えません。

```css
div.box Hello
    div.prompt World
```

{{< code lang="css" title="間違い" >}}
div.box Hello: div.prompt World
{{< /code >}}

