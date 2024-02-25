---
title: "D3.js による Force Simulation (3) 各ノードをドラッグして動かせるようにする"
url: "p/8dmb73t/"
date: "2023-09-18"
tags: ["D3.js"]
---

d3-drag モジュールとは
----

D3.js は、マウスによるドラッグ操作や、タブレットなどでのタッチドラッグ操作を扱うための `d3-drag` モジュールを提供しています（`d3` モジュールに内包されています）。
`d3-drag` は Force Simulation (`d3-force`) 用のモジュールではありませんが、よく `d3-force` と組み合わせて使用されます。
Force Simulation では、各ノードの座標値が `tick` イベントと連動して刻々と変化するため、ドラッグ中はノードの座標値をマウスカーソル位置に固定するなどの処理が必要です。

- 参考: [Force Simulation (d3-force) の基本](/p/6kavdch/)


d3-drag の基本的な使い方
----

次のサンプルでは、`svg` 内の `circle` 要素をドラッグして動かせるようにしています。

{{< mm/d3 id="svg-p4vddjh" w="300" h="200" title="d3-drag によるノードのドラッグ" >}}
const svg = d3.select("#svg-p4vddjh")
const width = +svg.attr("width")
const height = +svg.attr("height")

// ノード配列
const nodesData = [{}, {}, {}, {}]

// ノードを描画するための circle 要素を svg に追加しておく
const circles = svg.selectAll("circle")
  .data(nodesData)
  .join("circle")
  .attr("r", 30)
  .attr("fill", "blue")
  .call(d3.drag()
    .on("start", dragStarted)
    .on("drag", dragged)
    .on("end", dragEnded)
  )

// Simulation オブジェクトの作成とフォース設定
const simulation = d3.forceSimulation()
  .force("center", d3.forceCenter(width / 2, height / 2))
  .force("charge", d3.forceManyBody().strength(1000))
  .force("collide", d3.forceCollide().radius(30))

// ドラッグ開始時のイベント
function dragStarted(event, d) {
  if(!event.active) simulation.alphaTarget(0.3).restart();
  d.fx = d.x;
  d.fy = d.y;
}

// ドラッグ中のイベント
function dragged(event, d) {
  d.fx = event.x;
  d.fy = event.y;
}

// ドラッグ終了時のイベント
function dragEnded(event, d) {
  if(!event.active) simulation.alphaTarget(0);
  d.fx = null;
  d.fy = null;
}

// Simulation オブジェクトにノード配列をセットして tick イベントをハンドル開始
simulation.nodes(nodesData).on("tick", tickHandler)

// tick 毎のレイアウト
function tickHandler() {
  circles
    .attr("cx", (d) => d.x)
    .attr("cy", (d) => d.y)
}
{{< /mm/d3 >}}

{{< code lang="html" title="ソースコード" >}}
<svg id="svg-p4vddjh" width="300" height="200"></svg>
<script>
const svg = d3.select("#svg-p4vddjh")
const width = +svg.attr("width")
const height = +svg.attr("height")

// ノード配列
const nodesData = [{}, {}, {}, {}]

// ノードを描画するための circle 要素を svg に追加しておく
const circles = svg.selectAll("circle")
  .data(nodesData)
  .join("circle")
  .attr("r", 30)
  .attr("fill", "blue")
  .call(d3.drag()
    .on("start", dragStarted)
    .on("drag", dragged)
    .on("end", dragEnded)
  )

// Simulation オブジェクトの作成とフォース設定
const simulation = d3.forceSimulation()
  .force("center", d3.forceCenter(width / 2, height / 2))
  .force("charge", d3.forceManyBody().strength(1000))
  .force("collide", d3.forceCollide().radius(30))

// ドラッグ開始時のイベント
function dragStarted(event, d) {
  if(!event.active) simulation.alphaTarget(0.3).restart();
  d.fx = d.x;
  d.fy = d.y;
}

// ドラッグ中のイベント
function dragged(event, d) {
  d.fx = event.x;
  d.fy = event.y;
}

// ドラッグ終了時のイベント
function dragEnded(event, d) {
  if(!event.active) simulation.alphaTarget(0);
  d.fx = null;
  d.fy = null;
}

// Simulation オブジェクトにノード配列をセットして tick イベントをハンドル開始
simulation.nodes(nodesData).on("tick", tickHandler)

// tick 毎のレイアウト
function tickHandler() {
  circles
    .attr("cx", (d) => d.x)
    .attr("cy", (d) => d.y)
}
</script>
{{< /code >}}

### ドラッグ挙動オブジェクトの作成

マウスによるドラッグ（およびタブレットでのタッチドラッグ）の挙動を定義するためのオブジェクトは、__`d3.drag()`__ で生成することができます。
このオブジェクトは、公式には drag behavior と呼ばれていますが、ここでは「ドラッグ挙動オブジェクト」と呼ぶことにします。
作成したドラッグ挙動オブジェクトは、D3 セレクションオブジェクトの `call()` メソッドに渡すことでアタッチできます。
次のコードでは、`circle` セレクションオブジェクトにドラッグ挙動オブジェクトをアタッチしています。

{{< code lang="js" title="ドラッグ挙動のセット" hl_lines="6-9" >}}
const circles = svg.selectAll("circle")
  .data(nodesData)
  .join("circle")
  .attr("r", 30)
  .attr("fill", "blue")
  .call(d3.drag()
    .on("start", dragStarted)
    .on("drag", dragged)
    .on("end", dragEnded)
  )
{{< /code >}}

{{% note title="call メソッドとは" %}}
上記のコードでは、`circle` 要素群のセレクションオブジェクトの `call()` メソッドに、`d3.drag()` で作成したドラッグ挙動オブジェクトを渡しています。
これは、ドラッグ挙動オブジェクトを関数として呼び出してセレクションオブジェクトを引数として渡す書き方 (`dragBehavior(circles)`) と同じ意味になりますが、上記のように `call()` メソッドを使うことで、メソッドチェーンに組み込む形でドラッグ挙動を設定することができます。

- 参考: [D3.js の call メソッドを使ってメソッドチェーン処理をスマートに記述する](/p/994xjma/)
{{% /note %}}

### ドラッグ関連のイベントハンドラーの実装

下記はドラッグ開始時に呼び出されるイベント (`start`) のハンドラー定義です。
ドラッグを開始したノードオブジェクトがパラメーター (`d`) として渡されるので、現在の座標値を __`fx`__ および __`fy`__ プロパティにセットしています。
ノードオブジェクトのこれらのプロパティをセットすると、Force Simulation による座標値更新を一時停止し、そのノードの座標を固定するという意味になります。
また、__`alphaTarget`__ 値を 0.3 に設定して（デフォルトは 0）シミュレーションを再開することで、ドラッグ中にシミュレーションが停止 (`alpha=0`) してしまわないようにします。

```js
// ドラッグ開始時のイベント
function dragStarted(event, d) {
  if(!event.active) simulation.alphaTarget(0.3).restart();
  d.fx = d.x;
  d.fy = d.y;
}
```

下記はドラッグ中に繰り返し呼び出されるイベント (`drag`) のハンドラー定義です。
イベントオブジェクトから現在のマウス座標を取得し、ノードの固定座標値としてセットすることで、ノードをマウスカーソル位置に追従させます。

```js
// ドラッグ中のイベント
function dragged(event, d) {
  d.fx = event.x;
  d.fy = event.y;
}
```

下記はドラッグ終了時に呼び出されるイベント (`end`) のハンドラー定義です。
ノードの `fx` および `fy` プロパティを `null` にリセットすることで、Force Simulation による座標値更新を有効化します。
また、`alphaTarget` 値を 0 に戻し、時間経過でシミュレーションが自動的に停止するようにします（`alpha` 値は `alphaTarget` に向かって減少していき、`alphaMin` 値を下回ったときに計算が停止します）。

```js
// ドラッグ終了時のイベント
function dragEnded(event, d) {
  if(!event.active) simulation.alphaTarget(0);
  d.fx = null;
  d.fy = null;
}
```

{{% note title="ドラッグ中の動きを穏やかにしたいとき" %}}
ドラッグ中にノードが激しく動き回ってしまう場合は、`start` イベントハンドラで指定する `alphaTarget` の値を小さい値（0.05 ～ 0.1 くらい）に設定すると、動きが安定しやすくなります。

{{< code lang="ts" hl_lines="2" >}}
function dragStarted(event, d) {
  if(!event.active) simulation.alphaTarget(0.05).restart();
  d.fx = d.x;
  d.fy = d.y;
}
{{< /code >}}
{{% /note %}}


ドラッグ挙動オブジェクトを生成する関数を定義する
----

多くの場合、ドラッグ挙動オブジェクトの設定コードは似たような実装になります。
ここでは、ドラッグ挙動オブジェクトを生成するコードを関数化してみます。

```js
/** ドラッグ挙動オブジェクトを作成する */
function createDragBehavior(simulation) {
  return d3.drag()
    .on("start", function(event, d) {
      if(!event.active) simulation.alphaTarget(0.3).restart()
      d.fx = d.x
      d.fy = d.y
    }).on("drag", function(event, d) {
      d.fx = event.x
      d.fy = event.y
    }).on("end", function(event, d) {
      if(!event.active) simulation.alphaTarget(0)
      d.fx = null
      d.fy = null
    })
}
```

あとは、この関数によって作成したドラッグ挙動オブジェクトを、任意の D3 セレクションオブジェクトに設定してやれば OK です。

{{< code lang="js" hl_lines="6" >}}
const circles = svg.selectAll("circle")
  .data(nodesData)
  .join("circle")
  .attr("r", 30)
  .attr("fill", "blue")
  .call(createDragBehavior(simulation))
{{< /code >}}

{{% maku-common/reference %}}
- [D3.js による Force Simulation (1) d3-force の基本](/p/6kavdch/)
- [D3.js による Force Simulation (2) link force でノードを結合する](/p/9ujohp6/)
{{% /maku-common/reference %}}

