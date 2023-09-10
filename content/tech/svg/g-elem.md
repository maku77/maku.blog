---
title: "SVG の g 要素の使い方"
url: "p/rx9hdei/"
date: "2023-09-10"
tags: ["D3.js", "SVG"]
draft: true
---

SVG の __`g`__ 要素は、子要素をグループ化して操作を行うための要素です。
ここでは、`g` 要素をどのような用途で利用できるのかをまとめておきます。

子要素に共通の色をセットする
----

<center>
<svg width="195" height="60" style="border: thin solid gray;">
  <g fill="#cef" stroke="blue" stroke-width="1.5">
    <rect x="15" y="15" width="30" height="30" />
    <circle cx="75" cy="30" r="15" />
  </g>
  <g fill="#ffc" stroke="darkorange" stroke-width="1.5">
    <rect x="105" y="15" width="30" height="30" />
    <circle cx="165" cy="30" r="15" />
  </g>
</svg>
</center>

この例では、2 つの `g` 要素の下に、`rect` と `circle` を 1 つずつ配置しています。
それらの塗りつぶし色 (`fill`) や枠線の色 (`stroke`) は、親の `g` 要素にセットしたものが使用されます。

```html
<svg width="195" height="60" style="border: solid 1px gray;">
  <g fill="#cef" stroke="blue" stroke-width="1.5">
    <rect x="15" y="15" width="30" height="30" />
    <circle cx="75" cy="30" r="15" />
  </g>
  <g fill="#ffc" stroke="darkorange" stroke-width="1.5">
    <rect x="105" y="15" width="30" height="30" />
    <circle cx="165" cy="30" r="15" />
  </g>
</svg>
```


transform による座標変換の適用単位としてまとめる
----


レイヤー構造を作り表示順序を制御する
----

D3.js を使った JavaScript コードなどで SVG を動的に構築する場合、各要素の表示順序（どちらが手前に表示されるか）が問題になったりします。
このようなケースでは、`g` 要素で表示順序を制御するだけのレイヤー構造を作り、そこに子要素を追加していくというテクニックが使えます。

次の例では、2 つの `g` 要素（`layer1` と `layer2`）を作成し、その子要素として `circle` や `rect` 要素を配置しています。
`layer1`、`layer2` の順番で `g` 要素を追加しているので、`layer2` に配置した子要素（この場合は `circle`）の方が、手前に表示されることが保証されます。

{{< maku-common/d3 id="svg-rub6v9m" w="120" h="80" title="レイヤー構造による表示順序の制御" >}}
const svg = d3.select("#svg-rub6v9m")
const layer1 = svg.append("g");
const layer2 = svg.append("g");

layer2.append("circle")
    .attr("cx", 75)
    .attr("cy", 40)
    .attr("r", 20)
    .attr("fill", "red")

layer1.append("rect")
    .attr("x", 25)
    .attr("y", 15)
    .attr("width", 50)
    .attr("height", 50)
    .attr("fill", "blue");
{{< /maku-common/d3 >}}

```html
<svg id="svg-rub6v9m" width="120" height="80" />

<script>
const svg = d3.select("#svg-rub6v9m")
const layer1 = svg.append("g");
const layer2 = svg.append("g");

// layer2 に追加したものは手前に表示される
layer2.append("circle")
    .attr("cx", 75)
    .attr("cy", 40)
    .attr("r", 20)
    .attr("fill", "red")

// layer1 に追加したものは奥に表示される
layer1.append("rect")
    .attr("x", 25)
    .attr("y", 15)
    .attr("width", 50)
    .attr("height", 50)
    .attr("fill", "blue");
</script>
```

