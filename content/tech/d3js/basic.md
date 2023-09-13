---
title: "D3.js による SVG 描画の基本"
url: "p/v38nmhw/"
date: "2023-08-08"
tags: ["D3.js", "SVG"]
weight: 1
---

[D3.js](https://d3js.org/) は、クライアントサイド JavaScript で SVG 描画を行うためのライブラリです。
素の JavaScript（バニラ JavaScript) だけでも `svg` 要素を動的に構築していくことはできますが、D3.js を使うことでよりシンプルな API を使って描画処理を記述することができます。
ここでは、D3.js で基礎的な SVG 描画を行う方法を紹介します。


矩形を 1 つ描く
----

D3.js で矩形を描画するには、__`d3.select()`__ で svg 要素を選択し（D3 セレクションオブジェクトを取得）、そこに __`append()`__ メソッドで `rect` 要素を追加します。
そして、メソッドチェーンさせる形で __`attr()`__ を呼び出し、`rect` の属性（サイズや色）を設定していきます。

次の例では、svg 要素に緑色の矩形を 1 つ追加しています。
`rect` 要素の塗り潰し色は `fill` 属性で指定することに注意してください（CSS の "background" プロパティとは名前が異なります）。

{{< maku-common/d3 id="mysvg" w="200" h="100" title="矩形の表示" >}}
d3.select("#mysvg")
  .append("rect")    // svg 内に rect 要素を追加
  .attr("x", 10)     // rect 要素の属性値を設定
  .attr("y", 10)
  .attr("width", 100)
  .attr("height", 50)
  .attr("fill", "green")
{{< /maku-common/d3 >}}

```html
<svg id="mysvg" width="200" height="100" />
<script>
d3.select("#mysvg")  // svg 要素を選択
  .append("rect")    // svg 内に rect 要素を追加
  .attr("x", 10)     // rect 要素の属性値を設定
  .attr("y", 10)
  .attr("width", 100)
  .attr("height", 50)
  .attr("fill", "green")
</script>
```

これは実質、次のような `svg` 要素を構成していることになります。

```html
<svg id="mysvg" width="200" height="100">
  <rect x="10" y="10" width="100" height="50" fill="green" />
</svg>
```


配列データをまとめてプロットする
----

配列に格納された複数の値をまとめてプロットするには、D3 セレクションオブジェクトの __`data()`__ メソッドに配列データをセットします。

{{< maku-common/d3 id="mysvg2" w="200" h="100" title="配列データをプロット" >}}
const arr = [25, 50, 75]

d3.select("#mysvg2")
  .selectAll("rect")
  .data(arr)
  .join("rect")
  .attr("x", (_d, i) => i * 60 + 20)
  .attr("y", (d) => 100 - d)
  .attr("width", 40)
  .attr("height", (d) => d)
  .attr("fill", "darkred")
{{< /maku-common/d3 >}}

```html
<div id="mysvg2" width="200" height="100" />
<script>
const arr = [25, 50, 75]

d3.select("#mysvg2")
  .selectAll("rect")
  .data(arr)
  .join("rect")
  .attr("x", (_d, i) => i * 60 + 20)
  .attr("y", (d) => 100 - d)
  .attr("width", 40)
  .attr("height", (d) => d)
  .attr("fill", "blue")
</script>
```

分かりにくいのは、`selectAll()` メソッドあたりですが、あらかじめ作りたい要素を宣言しておいて、データをセット、追加される各要素に対して属性を設定するという流れになります。
`.attr()` メソッドにコールバック関数を指定することで、現在処理中のデータを参照することができるので、その値を利用して座標値などを計算します。

- `.selectAll("rect")` ... この次に指定するデータで複数の `rect` を作るという宣言です。
- `.data(arr)` ... この配列データの情報をもとに各要素を作ります。
- `.join("rect")` ... 各データに対応する `rect` を作ります。これ以降の処理（`.attr()` など）では、各 `rect` ごとに属性値を設定していきます。ちなみに `join()` は `enter()` と `append("rect")` をまとめた記述方法です。

D3.js は要素の追加・更新・削除まわりの仕組みがとっつきにくいですね。
でも慣れてしまえば分かりやすいので、学習曲線がいきなりちょっと高いところから始まる感じ。

