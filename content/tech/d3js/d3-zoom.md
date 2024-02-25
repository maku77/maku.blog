---
title: "D3.js で描画内容をズームやパン操作できるようにする (d3-zoom)"
linkTitle: ""
url: "p/w2xmuae/"
date: "2023-09-22"
tags: ["D3.js"]
---

{{% private %}}
- https://d3js.org/d3-zoom
{{% /private %}}

d3-zoom とは
----

D3.js に組み込まれている `d3-zoom` モジュールは、下記のようなユーザー操作をハンドルためのモジュールです。

- マウスホイールによるズーム操作
- マウスドラッグによるパン操作
- タッチパネルのピンチイン、ピンチアウトによるズーム操作
- タッチパネルのスワイプによるパン操作

`d3-zoom` によるイベントハンドリング処理は描画処理とは独立しているので、SVG や Canvas、スケール (`d3-scale`)、軸 (`d3-axis`) などと自在に組み合わせて使うことができます。


d3-zoom の簡単な使用例
----

### 実装例

下記の `svg` 画像の中で、ドラッグやホイール操作を行うと、矩形のズームや移動を行うことができます。

{{< mm/d3 id="svg-gh4oas2" w="200" h="100" title="d3-zoom によるズーム／パン操作" >}}
const svg = d3.select("#svg-gh4oas2")
const topLayer = svg.append("g")

topLayer
  .append("rect")
  .attr("x", 80)
  .attr("y", 40)
  .attr("width", 40)
  .attr("height", 20)
  .attr("fill", "green")

svg.call(d3.zoom().on("zoom", zoomed))

function zoomed(event) {
  topLayer.attr("transform", event.transform);
}
{{< /mm/d3 >}}

{{< code lang="html" title="実装コード" >}}
<svg id="svg-gh4oas2" width="200" height="100"></svg>
<script>
const svg = d3.select("#svg-gh4oas2")
const topLayer = svg.append("g")  // このレイヤーごと動かすことにする

// 矩形を 1 つ追加
topLayer
  .append("rect")
  .attr("x", 80)
  .attr("y", 40)
  .attr("width", 40)
  .attr("height", 20)
  .attr("fill", "green")

svg.call(d3.zoom().on("zoom", zoomed))  // zoom behavior を登録する

function zoomed(event) {
  // イベントが発生したらイベントオブジェクトの内容に従って transform するだけ
  topLayer.attr("transform", event.transform);
}
</script>
{{< /code >}}

### zoom オブジェクトの作成

`d3-zoom` は次のような手順で使用します。

1. __`d3.zoom()`__ でズーム操作をハンドルする behavior オブジェクトを作成する（以下 `zoom` オブジェクトと呼びます）
2. `zoom` オブジェクトの __`on()`__ メソッドでイベントハンドラを設定する
3. 任意の D3 セレクションオブジェクトに対して __`selection.call(zoom)`__ でイベントハンドラを適用する
   - 実質 `zoom(selection)` を呼び出すの同じですが、メソッドチェーンに組み込むために `selection.call()` を使うことができます。

上記手順をまとめてコード化すると次のような感じになります。
多くの場合、`zoom()` 関数の適用対象は `svg` 要素のセレクションオブジェクトです。

```js
svg.call(d3.zoom().on("zoom", zoomed));
```

### ズームイベントの処理

`d3.zoom().on("zoom", zoomed)` のように登録されたイベントハンドラー `zoomed` には、ユーザーのズーム／パン操作を表現するイベントオブジェクトが渡されて呼び出されます。
このイベントオブジェクトには、`g` 要素などに対してどのような `transform` 処理を行えばよいかの情報 (__`event.transform`__) が含まれています。
つまり、基本的にはその内容に従って、`g` 要素などの `transform` 属性の値をアップデートしてやるだけで OK です。

```js
function zoomed(event) {
  topLayer.attr("transform", event.transform);
}
```


（応用）ズーム倍率やパン範囲を制限する
----

`zoom` オブジェクトの下記のメソッドを使うことで、ズームやパンをどの程度まで許容するかを設定できます。

- __`zoom.scaleExtent()`__ ... ズーム倍率を制限する
- __`zoom.translateExtent()`__ ... パン範囲を制限する

パン範囲 (`translateExtent`) の方は、現在のズーム倍率などによって調整が必要で難しかったりするので、とりあえずズーム倍率 (`scaleExtent`) の制限だけしておくのがよいかもしれません。

{{< mm/d3 id="svg-agsdno7" w="200" h="100" title="d3-zoom によるズーム／パン操作" >}}
const svg = d3.select("#svg-agsdno7")
const width = +svg.attr("width")
const height = +svg.attr("height")
const topLayer = svg.append("g")

topLayer.append("circle")
  .attr("cx", 100)
  .attr("cy", 50)
  .attr("r", 20)
  .attr("fill", "#f48")

const zoom = d3.zoom()
  .scaleExtent([1, 3])  // ズームは 1 倍 ～ 3 倍の範囲
  .translateExtent([  // パンは元の svg サイズの半分まで（中央の要素を端に移動できる範囲）
    [-width * 0.5, -height * 0.5],
    [width * 1.5, height * 1.5]
  ])
  .on("zoom", zoomed)

svg
  .call(zoom)
  .on("wheel", (event) => event.preventDefault())

function zoomed(event) {
  topLayer.attr("transform", event.transform);
}
{{< /mm/d3 >}}

{{< code lang="html" title="実装コード" hl_lines="14-24" >}}
<svg id="svg-agsdno7" width="200" height="100"></svg>
<script>
const svg = d3.select("#svg-agsdno7")
const width = +svg.attr("width")
const height = +svg.attr("height")
const topLayer = svg.append("g")

topLayer.append("circle")
  .attr("cx", 100)
  .attr("cy", 50)
  .attr("r", 20)
  .attr("fill", "#f48")

const zoom = d3.zoom()
  .scaleExtent([1, 3])  // ズームは 1 倍 ～ 3 倍の範囲
  .translateExtent([  // パンは元の svg サイズの半分まで（中央の要素を端に移動できる範囲）
    [-width * 0.5, -height * 0.5],
    [width * 1.5, height * 1.5]
  ])
  .on("zoom", zoomed)

svg
  .call(zoom)
  .on("wheel", (event) => event.preventDefault())

function zoomed(event) {
  topLayer.attr("transform", event.transform);
}
</script>
{{< /code >}}

上記の例では、
`svg` 要素の `wheel` イベントのデフォルト動作を抑制 (`preventDefault()`) していることに注意してください。
これを入れておかないと、マウスホイールをズーム倍率範囲を超えて回転させたときに、ページ全体がスクロールしてしまいます。

