---
title: "D3.js で描画要素をクリックしたときに選択状態にする"
url: "p/6sijufz/"
date: "2023-09-25"
tags: ["D3.js"]
---

<style>
  .selected-qpi7j9n {
    fill: cyan;
    stroke: red;
    stroke-width: 5;
  }
</style>

SVG 描画要素の選択状態を表現する
----

D3.js で描画した要素（`rect` や `circle` など）をマウスクリックで選択できるようにすると、いろいろな操作を実現できます。

- 選択された要素に紐づけられたデータ (datum) を、別の場所に詳細情報として表示する
- 選択された要素に対して、一連の操作を行う

問題は、どのように要素の選択状態を表現するかですが、ここでは、要素をクリックしたときに、CSS のクラス `selected` を動的に付加することで選択状態を表現してみます。

次の例では、並んでいる丸をクリックすると見た目が変化します（選択状態を表現しています）。
逆に、別の丸の見た目はリセットされます。

{{< maku-common/d3 id="svg-uh6jd9p" w="200" h="50" title="丸をクリックして選択状態にする" >}}
const svg = d3.select("#svg-uh6jd9p")
const data = [{}, {}, {}]

// データ数だけ circle 要素を追加
const circles = svg.selectAll("circle")
  .data(data)
  .join("circle")
  .attr("cx", (_d, i) => i * 50 + 50)
  .attr("cy", 25)
  .attr("r", 15)
  .attr("fill", "dodgerblue")

circles.on("click", (event) => {
  // 先にすべての circle 要素の selected クラスをクリアしておく
  circles.classed("selected-qpi7j9n", false)

  // 今回クリックされた circle 要素に selected クラスをセットする
  d3.select(event.currentTarget)
    .classed("selected-qpi7j9n", true)
})
{{< /maku-common/d3 >}}

{{< code lang="html" title="実装コード" >}}
<style>
  /* 選択された要素に付加するクラス */
  .selected {
    fill: cyan;
    stroke: red;
    stroke-width: 5;
  }
</style>
<svg id="svg-uh6jd9p" width="200" height="50"></svg>
<script>
const svg = d3.select("#svg-uh6jd9p")
const data = [{}, {}, {}]

// データ数だけ circle 要素を追加
const circles = svg.selectAll("circle")
  .data(data)
  .join("circle")
  .attr("cx", (_d, i) => i * 50 + 50)
  .attr("cy", 25)
  .attr("r", 15)
  .attr("fill", "dodgerblue")

circles.on("click", (event) => {
  // 先にすべての circle 要素の selected クラスをクリアしておく
  circles.classed("selected", false)

  // 今回クリックされた circle 要素に selected クラスをセットする
  d3.select(event.currentTarget)
    .classed("selected", true)
})
</script>
{{< /code >}}

`click` イベントハンドラーの中では、D3 セレクションオブジェクトの __`classed()`__ メソッドを使って CSS クラス (`selected`) の on/off を切り替えています。
このメソッドを使うと、特定の CSS クラスのみを on/off できるので便利です。

{{< code lang="js" title="classed() メソッドの基本" >}}
selection.classed("foo", true);   // foo クラスを付ける
selection.classed("foo", false);  // foo クラスを外す
{{< /code >}}


選択された要素のデータ (datum) を表示する
----

`click` イベントハンドラーの第 2 引数には、クリックされた要素に紐づけられたデータ (`datum`) が渡されます。
ここから情報を取り出して、別の領域に詳細情報として表示するようにしておくと、インタラクティブな操作でたくさんの情報を確認できるようになります。

次のサンプルでは、クリックされた要素のデータから `name` プロパティを取り出して、`text` 要素で表示しています。

{{< maku-common/d3 id="svg-8bakmnb" w="200" h="80" title="クリックした要素のデータを表示" >}}
const svg = d3.select("#svg-8bakmnb")
const data = [
  { name: "Node-1" },
  { name: "Node-2" },
  { name: "Node-3" },
]

// データ数だけ circle 要素を追加
const circles = svg.selectAll("circle")
  .data(data)
  .join("circle")
  .attr("cx", (_d, i) => i * 50 + 50)
  .attr("cy", 25)
  .attr("r", 15)
  .attr("fill", "dodgerblue")

// データ表示用の text 要素を追加
const text = svg.append("text")
  .attr("x", 65)
  .attr("y", 70)
  .attr("font-size", 20)
  .attr("fill", "red")

circles.on("click", (event, d) => {
  // 先にすべての circle 要素の selected クラスをクリアしておく
  circles.classed("selected-qpi7j9n", false)

  // 今回クリックされた circle 要素に selected クラスをセットする
  d3.select(event.currentTarget)
    .classed("selected-qpi7j9n", true)

  text.text(d.name);
})
{{< /maku-common/d3 >}}

{{< code lang="html" title="実装コード" >}}
<svg id="svg-8bakmnb" w="200" h="80"></svg>
<script>
const svg = d3.select("#svg-8bakmnb")
const data = [
  { name: "Node-1" },
  { name: "Node-2" },
  { name: "Node-3" },
]

const circles = svg.selectAll("circle")
  .data(data)
  .join("circle")
  .attr("cx", (_d, i) => i * 50 + 50)
  .attr("cy", 25)
  .attr("r", 15)
  .attr("fill", "dodgerblue")

// データ表示用の text 要素を追加
const text = svg.append("text")
  .attr("x", 65)
  .attr("y", 70)
  .attr("font-size", 20)
  .attr("fill", "red")

circles.on("click", (event, d) => {
  circles.classed("selected", false)
  d3.select(event.currentTarget).classed("selected", true)

  // クリックした要素に結び付けられたデータを取り出して表示
  text.text(d.name);
})
</script>
{{< /code >}}

ここでは、詳細情報を SVG の `text` 要素で表示しましたが、作成するアプリケーションによっては、通常の DOM 要素（`div` 要素など）に表示するのがよいと思います。
D3.js では通常の DOM 要素も SVG 要素と同様に扱えます（例: `d3.select("#desc").text("ほげ")`）。

