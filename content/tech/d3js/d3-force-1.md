---
title: "D3.js による Force Simulation (1) d3-force の基本"
url: "p/6kavdch/"
date: "2023-09-13"
tags: ["D3.js"]
---

{{% private %}}
- https://observablehq.com/@d3/mobile-patent-suits?collection=@d3/d3-force
- https://observablehq.com/@d3/clustered-bubbles?collection=@d3/d3-force
{{% /private %}}

d3-force とは
----

D3.js の [Force Simulation (`d3-force`)](https://d3js.org/d3-force) モジュールは、粒子（ノード）間の引力を考慮した座標計算を行うためのライブラリです（`d3` モジュールに含まれています）。
主に、ノードとエッジで表現されるネットワークモデルをビジュアライズするために利用されます。
`d3-force` によって逐次計算される座標をもとにノードやエッジを描画することで、レイアウトが収束していく過程をアニメーションさせて見せることができます。

次の例では、`d3-force` を使って 5 つのノードの座標値を計算して、SVG の `circle` 要素として描画しています。

{{< mm/d3 id="svg-gi3ajww" w="300" h="200" title="d3-force によるレイアウト" >}}
const svg = d3.select("#svg-gi3ajww")
const width = +svg.attr("width")
const height = +svg.attr("height")

// ノード配列（どんなオブジェクトでもよい）
const nodesData = [{}, {}, {}, {}, {}]

// ノードを描画するための circle 要素を svg に追加しておく
const circles = svg.selectAll("circle")
  .data(nodesData)
  .join("circle")
  .attr("r", 10)
  .attr("fill", "blue")

// Simulation オブジェクトの作成とフォース設定
const simulation = d3.forceSimulation()
  .force("center", d3.forceCenter(width / 2, height / 2))
  .force("charge", d3.forceManyBody().strength(-10))

// Simulation オブジェクトにノード配列をセットして tick イベントをハンドル開始
simulation.nodes(nodesData).on("tick", tickHandler)

function tickHandler() {
  circles
    .attr("cx", (d) => d.x)
    .attr("cy", (d) => d.y)
}
{{< /mm/d3 >}}

{{< code lang="html" title="実装コード" >}}
<svg id="#svg-gi3ajww" width="300" height="200"></svg>
<script>
const svg = d3.select("#svg-gi3ajww")
const width = +svg.attr("width")
const height = +svg.attr("height")

// ノード配列（どんなオブジェクトでもよい）
const nodesData = [{}, {}, {}, {}, {}]

// ノードを描画するための circle 要素を svg に追加しておく
const circles = svg.selectAll("circle")
  .data(nodesData)
  .join("circle")
  .attr("r", 10)
  .attr("fill", "blue")

// Simulation オブジェクトの作成とフォース設定
const simulation = d3.forceSimulation()
  .force("center", d3.forceCenter(width / 2, height / 2))
  .force("charge", d3.forceManyBody().strength(-10))

// Simulation オブジェクトにノード配列をセットして tick イベントをハンドル開始
simulation.nodes(nodesData).on("tick", tickHandler)

function tickHandler() {
  circles
    .attr("cx", (d) => d.x)  // d3-force が求めた X 座標をそのまま採用
    .attr("cy", (d) => d.y)  // d3-force が求めた Y 座標をそのまま採用
}
</script>
{{< /code >}}


Force Simulation の使い方の概要
----

Force Simulation (`d3-force`) で各ノードの座標値計算を行うには、次のような手順を踏みます。

1. __`d3.forceSimulation()`__ 関数で Simulation オブジェクトを生成します。
1. __`simulation.force()`__ メソッドで各種フォース（物理的な力のかかり方）を設定します。
   - 各種フォースの設定により、ノードがどのように動くかが決まります。
   - 例えば、`d3.forceCenter(x, y)` を設定すると、指定した中央座標 (x, y) に集まる力が働きます。
1. __`simulation.nodes()`__ メソッドで、ノードオブジェクトの配列を渡します。
   - 任意のオブジェクトの配列で OK ですが、`x` および `y` プロパティで初期座標をセットしておくこともできます。
1. __`simulation.on("tick", ...)`__ で `tick` イベントハンドラーをセットします。
   - この `tick` イベントハンドラーは座標値が収束するまで頻繁に呼び出されます。その中で描画要素を適切な位置に移動させることで、ノードの移動をアニメーションで表現できます。
   - Simulation オブジェクトが計算した座標値は、各ノードオブジェクトの `x` および `y` プロパティに随時格納されます（元のノードオブジェクトが変更されることに注意してください）。

`d3-force` はレイアウト用の座標値を計算するだけで、本質的に描画処理とは独立しています（これが D3.js の設計の美しいところです）。
描画用の論理的な座標は `d3-force` が計算してくれるので、あとは `circle` 要素などを自由に作成し、その座標に配置すればよいということです。


コードの説明
----

### ノード配列の準備

```js
const nodesData = [{}, {}, {}, {}, {}]
```

最初に、各ノードの情報を保持するノードオブジェクト配列を作成します。
ここでは、とりあえず空のオブジェクトの配列を作っていますが、アプリケーションが必要とする情報を含んだ任意のオブジェクトの配列を用意すれば OK です。
典型的には、ノード上に表示するラベル（`name` プロパティ）や、ノードの半径（`r` プロパティ）などを持たせることになると思います。

### 描画用の circle 要素を作成

```js
const circles = svg.selectAll("circle")
  .data(nodesData)
  .join("circle")
  .attr("r", 10)
  .attr("fill", "blue")
```

ノード配列のサイズ分の `circle` 要素を `svg` 要素内に作成しておきます。
座標値（`cx` と `cy`）は、後から `d3-force` で求めたものを設定すればよいので、ここでは設定しません。

###  Simulation オブジェクトの作成と設定

```js
const simulation = d3.forceSimulation()
  .force("center", d3.forceCenter(width / 2, height / 2))
  .force("charge", d3.forceManyBody().strength(-10))
```

ここがポイントとなる処理で、Force Simulation オブジェクトを作成し、各種フォースオブジェクトを設定します。
`force()` メソッドの第 1 引数にはフォース名を指定しますが、これはただの識別子でどんな名前でもよいみたいです。
フォースオブジェクトを作成する関数は D3.js が用意してくれているので、多くの場合は、それらに適切なパラメーターを設定するだけで済みます。
ここでは、次のような関数を使ってフォースオブジェクトを作成しています。

`d3.forceCenter(x, y)`
: 各ノードが指定された中心座標 (x, y) に引き付けられるような中心力を表現します。
デフォルトでは中心座標は (0, 0) であり、左上にノードが集まってしまうので、通常は `svg` 要素の中心座標を設定します。
生成されるフォースオブジェクトは、`.x()` や `.y()` メソッドを備えており、それを使って中心座標を取得および設定できるようになっています。

`d3.forceManyBody()`
: 各ノードが引き付け合う力（引力）、あるいは反発する力を表現します。
`strength` に正の値が設定されている場合は各ノードは引き付け合い、`strength` に負の値が設定されている場合は各ノードは反発し合います。
デフォルトでは `-30` となっているので、各ノードは反発します。

まとめると、今回の設定では、各ノードは中心力 (`d3.forceCenter()`) により中央に引き寄せられつつ、反発力 (`d3.forceManyBody()`) によりある程度の間隔を保って配置されることになります（記事先頭の表示例を確認してみてください）。

{{% private %}}
- https://d3js.org/d3-force/center
- https://d3js.org/d3-force/many-body
{{% /private %}}

### シミュレーションの開始とアニメーション

```js
simulation.nodes(nodesData).on("tick", tickHandler)

function tickHandler() {
  circles
    .attr("cx", (d) => d.x)
    .attr("cy", (d) => d.y)
}
```

Simulation オブジェクトの __`nodes()`__ メソッドでノード配列をセットすると、自動的に各ノードの座標値の計算が始まります。
各オブジェクトが最終的な座標値に収束してく過程は、`tick` イベントとしてハンドルできます。
`tick` イベントハンドラーが呼び出されるとき、各ノードの最新の X、Y 座標は、__`.x`__ プロパティと __`.y`__ プロパティに設定されています（`nodesData` 配列内の各ノードオブジェクトの内容が勝手に更新されることに注意してください）。
これらの座標値を参照して、`svg` の中の各描画要素を動かせば、ノードをアニメーションさせることができます。

ちなみに、上記サンプルコードの `circles` 変数は、複数の `circle` 要素の D3 セレクションオブジェクトです。
この `cx` 属性や `cy` 属性を設定してやることで、各 `circle` 要素の位置を動かしています。


次のステップ
----

これで、D3.js の Force Simulation (`d3-force`) の基本的な使い方が分かりました。
Force Simulation では、他にも次のようなフォースを設定することができます。

- 各ノードをリンクさせてその距離を保つ (link force) ... `d3.forceLink()`
- 各ノードに半径を持たせて重ならないように配置する (collide force) ... `d3.forceCollide()`
- 指定した X 座標、あるいは Y 座標に引き寄せる (position forces) ... `d3.forceX()` / `d3.forceY()`

さらに、ノードオブジェクトの `fx` および `fy` プロパティに座標値を設定することにより、そのノードの座標計算を一時的に停止（つまり、位置を固定）することができるようになっています。
これを利用すると、各ノードをドラッグして動かすことができるようになります。
ただし、前述の通り `d3-force` はあくまで座標値の計算をするレイアウトモジュールであり、ドラッグ操作に関する機能は提供していません。
ドラッグ関連のイベントは別の D3.js 機能 (`d3-drag`) でハンドルしつつ、`d3-force` の `fx`、`fy` プロパティの仕組みを利用する、という構成になります。

`d3-force` には多くの設定項目があって複雑な印象を受けるかもしれませんが、ここで学んだ基本原則を理解していれば、応用的なレイアウトも比較的容易に実現できるはずです。

٩(๑❛ᴗ❛๑)۶ わーぃ

下記の応用編へ続く。

- [D3.js による Force Simulation (2) link force でノードを結合する](/p/9ujohp6/)
- [D3.js による Force Simulation (3) 各ノードをドラッグして動かせるようにする](/p/8dmb73t/)

