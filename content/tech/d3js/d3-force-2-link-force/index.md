---
title: "D3.js による Force Simulation (2) link force でノードを結合する"
url: "p/9ujohp6/"
date: "2023-09-14"
tags: ["D3.js"]
---

link force とは
----

D3.js の Force simulation (`d3-force`) には様々なフォースを設定することができ、それにより各ノードの動き（レイアウト）を制御できるようになっています。
__link force__ はそのようなフォースのひとつで、ノード間にリンク情報を設定することにより、バネのような力を発生させます。
接続された 2 つのノードのうち、一方の位置を動かすと、もう一方のノードが引っ張られて動くようになります。

{{< image src="img-001.drawio.svg" title="link force のイメージ" >}}

- 参考: [Force Simulation (d3-force) の基本](/p/6kavdch/)


link force の基本的な使い方
----

次の例では、4 つのノードに環状に繋がるような link force を設定しています（ここではリンクの可視化はしていません）。

{{< maku-common/d3 id="svg-a2f28wm" w="300" h="200" title="link force を設定したフォースシミュレーション" >}}
const svg = d3.select("#svg-a2f28wm")
const width = +svg.attr("width")
const height = +svg.attr("height")

// ノード配列
const nodesData = [{}, {}, {}, {}]

// リンク配列
const linksData = [
  { source: 0, target: 1 },
  { source: 1, target: 2 },
  { source: 2, target: 3 },
  { source: 3, target: 0 },
]

// ノードを描画するための circle 要素を svg に追加しておく
const circles = svg.selectAll("circle")
  .data(nodesData)
  .join("circle")
  .attr("r", 10)
  .attr("fill", "blue")

// Simulation オブジェクトの作成とフォース設定
const simulation = d3.forceSimulation()
  .force("center", d3.forceCenter(width / 2, height / 2))
  .force("charge", d3.forceManyBody().strength(-1000))
  .force("link", d3.forceLink())

// Simulation オブジェクトにノード配列をセットして tick イベントをハンドル開始
simulation.nodes(nodesData).on("tick", tickHandler)
simulation.force("link").links(linksData)

// tick 毎のレイアウト
function tickHandler() {
  circles
    .attr("cx", (d) => d.x)
    .attr("cy", (d) => d.y)
}
{{< /maku-common/d3 >}}

{{< code lang="html" title="ソースコード" >}}
<svg id="svg-a2f28wm" width="300" height="200"></svg>
<script>
const svg = d3.select("#svg-a2f28wm")
const width = +svg.attr("width")
const height = +svg.attr("height")

// ノード配列
const nodesData = [{}, {}, {}, {}]

// リンク配列
const linksData = [
  { source: 0, target: 1 },
  { source: 1, target: 2 },
  { source: 2, target: 3 },
  { source: 3, target: 0 },
]

// ノードを描画するための circle 要素を svg に追加しておく
const circles = svg.selectAll("circle")
  .data(nodesData)
  .join("circle")
  .attr("r", 10)
  .attr("fill", "blue")

// Simulation オブジェクトの作成とフォース設定
const simulation = d3.forceSimulation()
  .force("center", d3.forceCenter(width / 2, height / 2))
  .force("charge", d3.forceManyBody().strength(-1000))
  .force("link", d3.forceLink())

// Simulation オブジェクトにノード配列をセットして tick イベントをハンドル開始
simulation.nodes(nodesData).on("tick", tickHandler)
simulation.force("link").links(linksData)

// tick 毎のレイアウト
function tickHandler() {
  circles
    .attr("cx", (d) => d.x)
    .attr("cy", (d) => d.y)
}
</script>
{{< /code >}}

ノード間のリンクを視覚的に表現するかどうかは実装者の判断に任されているので、必要に応じて `svg` の `line` 要素などで描画する必要があります（後述）。

### リンク情報を定義する

リンク情報は、次のように __`source`__ と __`target`__ プロパティを持つリンクオブジェクトの配列で表現します。
この例では、4 つのノード間に 4 つのリンクを設定しています。

{{< code lang="js" title="リンク情報の定義" >}}
const linksData = [
  { source: 0, target: 1 },
  { source: 1, target: 2 },
  { source: 2, target: 3 },
  { source: 3, target: 0 },
]
{{< /code >}}

接続するノードのキーとしては、デフォルトではノードの `index` プロパティの値を指定するようになっています（各ノードの `index` プロパティは、`d3-force` がノードのレイアウト計算を行うときに自動的に割り当てます）。

### link force（フォースオブジェクト）を作成する

link force を表現するフォースオブジェクトは、__`d3.forceLink()`__ 関数で作成することができます。
作成したフォースオブジェクトは、`Simulation` オブジェクトの __`force`__ メソッドで、一意の名前を付けて設定します。
慣例に従って `"link"` という名前を付けておけばよいです。

{{< code lang="js" title="Simulation に link force を設定する" hl_lines="4" >}}
const simulation = d3.forceSimulation()
  .force("center", d3.forceCenter(width / 2, height / 2))
  .force("charge", d3.forceManyBody().strength(-1000))
  .force("link", d3.forceLink())
{{< /code >}}

多くの場合、link force は他のフォースと組み合わせて使用します。
この例では、全ノードが中央に向かう力 (`d3.forceCenter()`) と、各ノードが反発し合う力 (`d3.forceManyBody()`) を同時に設定しています。

### リンク情報をセットする

`Simulation` オブジェクトにノード配列を設定するのと同様に、link force のフォースオブジェクトには、リンク配列を設定してやる必要があります。
次の例では、`Simulation` オブジェクトに設定済みの link force オブジェクトを `simulation.force("link")` で取り出し、その __`links()`__ メソッドでリンク配列を渡しています。
link force は内部でノードの情報を参照するため、先にノード配列を設定しておく必要があります。

{{< code lang="js" title="リンク配列を link force にセットする" hl_lines="2" >}}
simulation.nodes(nodesData).on("tick", tickHandler)
simulation.force("link").links(linksData)
{{< /code >}}

この操作により、link force を考慮したノードのレイアウトが開始されます。


応用 - リンクを線で表示する
----

ノード間のリンク情報を可視化するには、明示的に `svg` の `line` 要素などを作成して描画する必要があります。
次の例では、リンク情報を赤色の線で表現しています。

{{< maku-common/d3 id="svg-qne6apk" w="300" h="200" title="ノード間のリンクを line 要素で描画する" >}}
const svg = d3.select("#svg-qne6apk")
const width = +svg.attr("width")
const height = +svg.attr("height")

// ノード配列
const nodesData = [{}, {}, {}, {}]

// リンク配列
const linksData = [
  { source: 0, target: 1 },
  { source: 1, target: 2 },
  { source: 2, target: 3 },
  { source: 3, target: 0 },
]

// リンクを描画するための line 要素を svg に追加しておく
const lines = svg.selectAll("line")
  .data(linksData)
  .join("line")
  .attr("stroke", "red")
  .attr("stroke-width", 2)

// ノードを描画するための circle 要素を svg に追加しておく
const circles = svg.selectAll("circle")
  .data(nodesData)
  .join("circle")
  .attr("r", 10)
  .attr("fill", "blue")

// Simulation オブジェクトの作成とフォース設定
const simulation = d3.forceSimulation()
  .force("center", d3.forceCenter(width / 2, height / 2))
  .force("charge", d3.forceManyBody().strength(-1000))
  .force("link", d3.forceLink())

// Simulation オブジェクトにノード配列をセットして tick イベントをハンドル開始
simulation.nodes(nodesData).on("tick", tickHandler)
simulation.force("link").links(linksData)

function tickHandler() {
  circles
    .attr("cx", (d) => d.x)
    .attr("cy", (d) => d.y)
  lines
    .attr("x1", (d) => d.source.x)
    .attr("y1", (d) => d.source.y)
    .attr("x2", (d) => d.target.x)
    .attr("y2", (d) => d.target.y)
}
{{< /maku-common/d3 >}}

{{< code lang="html" title="ソースコード" >}}
<svg id="svg-qne6apk" width="300" height="200"></svg>
<script>
const svg = d3.select("#svg-qne6apk")
const width = +svg.attr("width")
const height = +svg.attr("height")

// ノード配列
const nodesData = [{}, {}, {}, {}]

// リンク配列
const linksData = [
  { source: 0, target: 1 },
  { source: 1, target: 2 },
  { source: 2, target: 3 },
  { source: 3, target: 0 },
]

// リンクを描画するための line 要素を svg に追加しておく
const lines = svg.selectAll("line")
  .data(linksData)
  .join("line")
  .attr("stroke", "red")
  .attr("stroke-width", 2)

// ノードを描画するための circle 要素を svg に追加しておく
const circles = svg.selectAll("circle")
  .data(nodesData)
  .join("circle")
  .attr("r", 10)
  .attr("fill", "blue")

// Simulation オブジェクトの作成とフォース設定
const simulation = d3.forceSimulation()
  .force("center", d3.forceCenter(width / 2, height / 2))
  .force("charge", d3.forceManyBody().strength(-1000))
  .force("link", d3.forceLink())

// Simulation オブジェクトにノード配列をセットして tick イベントをハンドル開始
simulation.nodes(nodesData).on("tick", tickHandler)
simulation.force("link").links(linksData)

// tick 毎のレイアウト
function tickHandler() {
  circles
    .attr("cx", (d) => d.x)
    .attr("cy", (d) => d.y)
  lines
    .attr("x1", (d) => d.source.x)
    .attr("y1", (d) => d.source.y)
    .attr("x2", (d) => d.target.x)
    .attr("y2", (d) => d.target.y)
}
</script>
{{< /code >}}

`line` 要素を `circle` 要素より奥に描画するには、`svg` 要素内で `line` 要素を先に追加しておく必要があります。
下記のコードが、リンク配列の要素数と同じ数の `line` 要素を追加している部分です。
各 `line` の座標情報 (`x1`, `y1`, `x2`, `y2`) は、後ほど自動計算された値をセットするのでここではセットしていません。

```js
// リンクを描画するための line 要素を svg に追加しておく
const lines = svg.selectAll("line")
  .data(linksData)
  .join("line")
  .attr("stroke", "red")
  .attr("stroke-width", 2)
```

`d3-force` によるシミュレーションが開始されると、リンク配列の各オブジェクトにセットされていた __`source`__ と __`target`__ プロパティの値が、具体的なノードオブジェクトへの参照に置き換えられます。
リンク配列の内容は、`lines` セレクションオブジェクト経由で参照できるので、`tick` イベントごとに次のようにして各 `line` 要素の始点 (`x1`, `y1`) と終点 (`x2`, `y2`) を更新することができます。

{{< code lang="js" title="line 要素を移動させる" hl_lines="5-9" >}}
function tickHandler() {
  circles
    .attr("cx", (d) => d.x)
    .attr("cy", (d) => d.y)
  lines
    .attr("x1", (d) => d.source.x)
    .attr("y1", (d) => d.source.y)
    .attr("x2", (d) => d.target.x)
    .attr("y2", (d) => d.target.y)
}
{{< /code >}}


応用 - ノードを表すキーとして任意のプロパティを使用する (id)
----

前述の例では、リンク配列要素の `source` と `target` プロパティで指定するノードのキーとして、ノードのインデックス (`index`) を指定していました。
ここで指定するキーとしてどのプロパティを使用するかは自由に変更することができます。
例えば、次のようにリンク情報を各ノードの `label` プロパティをキーにして設定したいとします。

```js
// ノード配列
const nodesData = [
  { label: "node-A" },
  { label: "node-B" },
  { label: "node-C" },
  { label: "node-D" },
]

// リンク配列
const linksData = [
  { source: "node-A", target: "node-B" },
  { source: "node-B", target: "node-C" },
  { source: "node-C", target: "node-D" },
  { source: "node-D", target: "node-A" },
]
```

`d3.forceLink()` 関数で作成したフォースオブジェクトの、__`id()`__ メソッドを使うと、各ノードのどのプロパティをキーにしてリンクを表現するかを指定できます。

{{< code lang="js" title="label プロパティをリンクのキーとする" hl_lines="4" >}}
const simulation = d3.forceSimulation()
  .force("center", d3.forceCenter(width / 2, height / 2))
  .force("charge", d3.forceManyBody().strength(-1000))
  .force("link", d3.forceLink().id((d) => d.label))
{{< /code >}}

{{% reference %}}
- [D3.js による Force Simulation (1) d3-force の基本](/p/6kavdch/)
- [D3.js による Force Simulation (3) 各ノードをドラッグして動かせるようにする](/p/8dmb73t/)
{{% /reference %}}

