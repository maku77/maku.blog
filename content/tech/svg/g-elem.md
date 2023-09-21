---
title: "SVG の g 要素の使い方"
url: "p/rx9hdei/"
date: "2023-09-21"
tags: ["D3.js", "SVG"]
---

SVG の __`g`__ 要素は、子要素をグループ化して操作を行うための要素です。
ここでは、`g` 要素をどのような用途で利用できるのかをまとめておきます。

{{% private %}}
- https://developer.mozilla.org/en-US/docs/Web/SVG/Element/g
{{% /private %}}


子要素に共通のスタイル（色、太さなど）をセットする
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

この例では、2 つの `g` 要素の下に、`rect` と `circle` を 1 つずつ配置しています。
それらの塗りつぶし色 (`fill`) や枠線の色 (`stroke`) は、親の `g` 要素にセットしたものが使用されます。

多数の描画要素に同じスタイルを割り当てたいときは、`g` 要素でまとめてスタイル設定すると効率がよいです。


子要素の座標をまとめて移動する (transform)
----

<center>
<svg width="180" height="60" style="border: thin solid gray;">
  <g transform="translate(50 30)">
    <circle cx="0" cy="0" r="10" fill="red" />
    <text x="15" y="2" dominant-baseline="middle" font-size="20" font-weight="800" fill="blue">
      Hello!
    </text>
  </g>
</svg>
</center>

```html
<svg width="180" height="60" style="border: thin solid gray;">
  <g transform="translate(50 30)">
    <circle cx="0" cy="0" r="10" fill="red" />
    <text x="15" y="2" dominant-baseline="middle" font-size="20" font-weight="800" fill="blue">
      Hello!
    </text>
  </g>
</svg>
```

`g` 要素の下に配置した子要素は、`g` 要素の [__`transform`__](https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/transform) 属性を使ってまとめて移動させることができます。
上記のサンプルコードでは、`transform` 属性の値として `translate(50, 30)` を指定しており、`g` 要素でグルーピングされたすべての子要素が x 軸方向へ 50、y 軸方向へ 30 だけ平行移動しています。
`g` 要素をどこに配置するかは `transform` 属性で制御できるので、`g` 要素でグルーピングされた部分は、自分の座標系だけを意識したコンポーネントのように記述できます。

`transform` の値には、次のような変換処理を指定することができます。
複数の変換処理をスペース区切りで並べて指定できます。

- 移動 ... __`translate(30 10)`__ で x 軸方向にへ 30、y 軸方向に 10 だけ平行移動する
- 拡縮 ... __`scale(2 0.5)`__ で x 軸方向に 2 倍、y 軸方向に 0.5 倍にスケールする
- 回転 ... __`rotate(30 10 10)`__ で (10, 10) の座標を中心に 30 度回転する
- 歪み ... __`skewX(30)`__、__`skewY(30)`__ で x 軸あるいは y 軸方向に 30 度歪ませる

例えば、次のように指定すると、平行移動、スキュー、スケールを順番に適用します。

```html
<g transform="translate(35 30) skewX(-30) scale(1.5)">
```

<center>
<svg width="180" height="60" style="border: thin solid gray;">
  <g transform="translate(35 30) skewX(-30) scale(1.5)">
    <circle cx="0" cy="0" r="10" fill="red" />
    <text x="15" y="2" dominant-baseline="middle" font-size="20" font-weight="800" fill="blue">
      Hello!
    </text>
  </g>
</svg>
<div>図: 上記の transform 指定の結果</div>
</center>

ちなみに、このような `g` 要素を D3.js で生成する場合のコードは次のような感じになります。
複数の `g` 要素を参照する D3 セレクションオブジェクトを変数化して使いまわすところがポイントです（下記の例では `nodes` 変数）。

{{< code lang="html" title="D3.js で g 要素の transform 処理" >}}
<svg id="svg-hsae2we" w="200" height="80"></svg>
<script>
const svg = d3.select('#svg-hsae2we');

const nodesData = [
	{ label: 'AAA', x: 20, y: 20 },
	{ label: 'BBB', x: 40, y: 40 },
	{ label: 'CCC', x: 60, y: 60 }
];

// ノードを描画するための g 要素をノードデータの数だけ作成
const nodes = svg.selectAll('g').data(nodesData).join('g');

// 各 g 要素に circle 要素を追加
nodes.append('circle')
  .attr('cx', 0)
  .attr('cy', 0)
  .attr('r', 10)
  .attr('fill', 'red');

// 各 g 要素に text 要素を追加
nodes
  .append('text')
  .attr('x', 15)
  .attr('y', 2)
  .attr('dominant-baseline', 'middle')
  .attr('font-size', 20)
  .attr('font-weight', '800')
  .attr('fill', 'blue')
  .text((d) => d.label);

// 各 g 要素の transform 属性を設定
nodes.attr('transform', (d) => `translate(${d.x} ${d.y})`);
</script>
{{< /code >}}


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

