---
title: "D3.js で各要素から任意のデータを取り出す (datum)"
url: "p/8m4drzr/"
date: "2023-09-10"
tags: ["D3.js"]
---

{{< maku-common/d3 id="svg-vmhww5v" w="200" h="100" title="D3 セレクションのデータを参照する" >}}
const svg = d3.select("#svg-vmhww5v");

// 3 つの circle 要素に紐づけるデータ
const data = [
  { name: "circle-1", color: "cyan" },
  { name: "circle-2", color: "yellow" },
  { name: "circle-3", color: "magenta" },
];

// 情報表示用の text 要素
const text = svg.append("text")
  .attr("x", "50%")
  .attr("y", "20")
  .attr("text-anchor", "middle")
  .text("円に触れてみて！")

svg
  .selectAll("circle")
  .data(data)  // ここでセットしたデータはあとから参照できる
  .join("circle")
  .attr("cx", (_d, i) => 65 * i + 35)
  .attr("cy", 60)
  .attr("r", 30)
  .attr("fill", (d) => d.color)
  .on("mouseover", handleMouseOver)
  .on("mouseout", handleMouseOut)

function handleMouseOver(event) {
  const d = d3.select(event.currentTarget).datum()  // カーソル位置の要素のデータを参照
  // const d = d3.select(this).datum()
  text.text(`${d.name}, ${d.color}`)  // 情報表示
}

function handleMouseOut() {
  text.text("")
}
{{< /maku-common/d3 >}}

D3 セレクションオブジェクトの __`datum()`__ メソッドを使うと、任意のデータ（オブジェクト）を紐づけて、後から自由に参照できるようになります。
例えば、任意の要素上でマウスイベントが発生した場合に、その要素のデータを参照するというのが典型的な使用方法です。
D3 セレクションオブジェクトに紐づけられたデータを参照するときは、`datum()` メソッドを引数なしで呼び出します。
D3.js で複数要素をまとめて追加する際には `data()` メソッドを使用しますが、それでセットしたデータも `datum()` メソッドで参照することができます。

下記の例では、3 つの `circle` 要素のデータとして、`name` プロパティと `color` プロパティを持つ独自のオブジェクトを紐づけています。

```html
<svg id="svg-vmhww5v" w="200" h="100" />
<script>
const svg = d3.select("#svg-vmhww5v");

// 3 つの circle 要素に紐づけるデータ
const data = [
  { name: "circle-1", color: "cyan" },
  { name: "circle-2", color: "yellow" },
  { name: "circle-3", color: "magenta" },
];

// 情報表示用の text 要素
const text = svg.append("text")
  .attr("x", "50%")
  .attr("y", "20")
  .attr("text-anchor", "middle")
  .text("円に触れてみて！")

svg
  .selectAll("circle")
  .data(data)  // ここでセットしたデータはあとから参照できる
  .join("circle")
  .attr("cx", (_d, i) => 65 * i + 35)
  .attr("cy", 60)
  .attr("r", 30)
  .attr("fill", (d) => d.color)
  .on("mouseover", handleMouseOver)
  .on("mouseout", handleMouseOut)

function handleMouseOver(event) {
  const d = d3.select(event.currentTarget).datum()  // カーソル位置の要素のデータを参照
  // const d = d3.select(this).datum()
  text.text(`${d.name}, ${d.color}`)  // 情報表示
}

function handleMouseOut() {
  text.text("")
}
</script>
```

{{% note %}}
D3 セレクションオブジェクトが複数の要素を選択している場合、`datum()` メソッドは最初の要素に紐づけられたデータを返します。
そのため、通常は 1 つの要素だけを選択したセレクションオブジェクトに対して `datum()` メソッドを呼び出します。
{{% /note %}}

