---
title: "D3.js のスケール関数: scaleOrdinal でカテゴリデータを別の値にマッピングする"
url: "p/73b94gi/"
date: "2023-09-30"
tags: ["D3.js"]
---

scaleOrdinal の基本
----

D3.js のスケールモジュール (`d3-scale`) が提供する __`scaleOrdinal()`__ 関数を使用すると、カテゴリデータを別の値にマッピングする関数を簡単に作成することができます。

{{% note title="カテゴリデータと数量データ" %}}
__カテゴリデータ (categorical data)__ というのは、統計学やデータ分析において使用されるデータ型のひとつで、通常、カテゴリを示すラベルや文字列で表現されます。
カテゴリデータは、定性データまたは質的データとも呼ばれます。
例えば、血液型や都道府県などがカテゴリデータです。

一方、価格や身長のような __数量データ (quantitative data)__ は本来はカテゴリデータではありませんが、特定の範囲でグルーピングすることで、カテゴリ化することができます（価格(円) → 低価格／中価格／高価格）。
カテゴリ化することを離散化 (discretization) やビン化 (binning) と呼ぶこともあります。
{{% /note %}}

次の例では、`AWS`/`Azure`/`GCP` という入力値を、`#ff9900`/`#0072c6`/`#db4437` というカラーコードに変換するスケール関数を作成しています。

```js
const colorScale = d3.scaleOrdinal()
  .domain(["AWS", "Azure", "GCP"])  // この入力が
  .range(["#ff9900", "#0072c6", "#db4437"]);  // この出力にマッピングされる

console.log(colorScale("AWS"));    //=> "#ff9900"
console.log(colorScale("Azure"));  //=> "#0072c6"
console.log(colorScale("GCP"));    //=> "#db4437"
```

上記の例では、入力値をカラーコードの文字列にマッピングしていますが、描画要素の座標値やサイズなど、任意の型の値にマッピングできます。
次の例では、実際に 2 つのスケール関数を作成して、SVG の描画に使用しています。

{{< maku-common/d3 id="svg-hfuouiw" w="200" h="80" title="scaleOrdinal による色のマッピング" >}}
const svg = d3.select("#svg-hfuouiw")

const data = [
  { brand: "aws", label: "Amazon Web Services" },
  { brand: "azure", label: "Microsoft Azure" },
  { brand: "gcp", lable: "Google Cloud Platform" },
]

const colorScale = d3.scaleOrdinal()
  .domain(["aws", "azure", "gcp"])
  .range(["#ff9900", "#0072c6", "#db4437"]);

const xScale = d3.scaleOrdinal()
  .domain(["aws", "azure", "gcp"])
  .range([40, 100, 160]);

svg.selectAll("circle")
  .data(data)
  .join("circle")
  .attr("cx", (d) => xScale(d.brand))
  .attr("cy", 40)
  .attr("r", 20)
  .attr("fill", (d) => colorScale(d.brand))
{{< /maku-common/d3 >}}

{{< code lang="html" title="上記の実装コード" hl_lines="11 15" >}}
<svg id="svg-hfuouiw" width="200" height="80"></svg>
<script>
const svg = d3.select("#svg-hfuouiw")

const data = [
  { brand: "aws", label: "Amazon Web Services" },
  { brand: "azure", label: "Microsoft Azure" },
  { brand: "gcp", lable: "Google Cloud Platform" },
]

const colorScale = d3.scaleOrdinal()
  .domain(["aws", "azure", "gcp"])
  .range(["#ff9900", "#0072c6", "#db4437"]);

const xScale = d3.scaleOrdinal()
  .domain(["aws", "azure", "gcp"])
  .range([40, 100, 160]);

svg.selectAll("circle")
  .data(data)
  .join("circle")
  .attr("cx", (d) => xScale(d.brand))
  .attr("cy", 40)
  .attr("r", 20)
  .attr("fill", (d) => colorScale(d.brand))
</script>
{{< /code >}}


未知の値が入力されたとき
----

スケール関数の入力値として、`domain()` で指定されていない未知の値が渡された場合、`range()` で設定した出力値が自動的にマッピングされていきます。
次の例では、`A`/`B`/`C`/`D`/`E` といった未知の入力値に対して、3 種類のカラー値を順番に割り当てています。
同じ値が入力値として渡された場合、以前割り当てた値が返されることに注目してください。

```js
const colorScale = d3.scaleOrdinal().range(["blue", "yellow", "red"]);

console.log(colorScale("A"));  //=> blue
console.log(colorScale("B"));  //=> yellow
console.log(colorScale("C"));  //=> red
console.log(colorScale("D"));  //=> blue
console.log(colorScale("E"));  //=> yellow
console.log(colorScale("A"));  //=> blue
console.log(colorScale("C"));  //=> red
```

未知の値が入力された場合に、指定した値を返すようにするには、__`unknown()`__ 関数を使用します。

{{< code lang="js" hl_lines="4" >}}
const colorScale = d3.scaleOrdinal()
  .domain(["A", "B", "C"])
  .range(["blue", "yellow", "red"])
  .unknown("gray");

console.log(colorScale("A"));  //=> blue
console.log(colorScale("B"));  //=> yellow
console.log(colorScale("C"));  //=> red
console.log(colorScale("D"));  //=> gray
console.log(colorScale("E"));  //=> gray
{{< /code >}}


カテゴリデータの着色に使えるカラースキーム
----

カテゴリデータの色分けは一般的な操作なので、D3.js は `scaleOrdinal()` と組み合わせて使用できる [カラースキーム](https://d3js.org/d3-scale-chromatic/categorical) をデフォルトで提供しています。

<table>
  <tr>
    <th colspan="3">デフォルトで使用できるカラースキーム</th>
  </tr>
  <tr>
    <td>schemeCategory10</td>
    <td>schemeAccent</td>
    <td>schemeDark2</td>
  </tr>
  <tr>
    <td>schemePaired</td>
    <td>schemePastel1</td>
    <td>schemePastel2</td>
  </tr>
  <tr>
    <td>schemeSet1</td>
    <td>schemeSet2</td>
    <td>schemeSet3</td>
  </tr>
  <tr>
    <td>schemeTableau10</td>
    <td></td>
    <td></td>
  </tr>
</table>

次の例では、__`schemeCategory10`__ というカラースキームを使用しています。

```js
const colorScale = d3.scaleOrdinal(d3.schemeCategory10)

console.log(colorScale("A"));  //=> #1f77b4
console.log(colorScale("B"));  //=> #ff7f0e
console.log(colorScale("C"));  //=> #2ca02c
console.log(colorScale("A"));  //=> #1f77b4
```

これを使えば、データの種類ごとに簡単に色分け表示できますね。

٩(๑❛ᴗ❛๑)۶ わーぃ

