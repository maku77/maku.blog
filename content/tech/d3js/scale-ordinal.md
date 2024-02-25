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

{{< mm/d3 id="svg-hfuouiw" w="200" h="80" title="scaleOrdinal による色のマッピング" >}}
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
{{< /mm/d3 >}}

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
    <td>schemeCategory10<br/>
      <svg width="150" height="15">
        <rect x="0" y="0" width="15" height="15"   fill="#1f77b4" />
        <rect x="15" y="0" width="15" height="15"  fill="#ff7f0e" />
        <rect x="30" y="0" width="15" height="15"  fill="#2ca02c" />
        <rect x="45" y="0" width="15" height="15"  fill="#d62728" />
        <rect x="60" y="0" width="15" height="15"  fill="#9467bd" />
        <rect x="75" y="0" width="15" height="15"  fill="#8c564b" />
        <rect x="90" y="0" width="15" height="15"  fill="#e377c2" />
        <rect x="105" y="0" width="15" height="15" fill="#7f7f7f" />
        <rect x="120" y="0" width="15" height="15" fill="#bcbd22" />
        <rect x="135" y="0" width="15" height="15" fill="#17becf" />
      </svg>
    </td>
    <td>schemeAccent<br/>
      <svg width="120" height="15">
        <rect x="0" y="0" width="15" height="15"   fill="#7fc97f" />
        <rect x="15" y="0" width="15" height="15"  fill="#beaed4" />
        <rect x="30" y="0" width="15" height="15"  fill="#fdc086" />
        <rect x="45" y="0" width="15" height="15"  fill="#ffff99" />
        <rect x="60" y="0" width="15" height="15"  fill="#386cb0" />
        <rect x="75" y="0" width="15" height="15"  fill="#f0027f" />
        <rect x="90" y="0" width="15" height="15"  fill="#bf5b17" />
        <rect x="105" y="0" width="15" height="15" fill="#666666" />
      </svg>
    </td>
    <td>schemeDark2<br/>
      <svg width="120" height="15">
        <rect x="0" y="0" width="15" height="15"   fill="#1b9e77" />
        <rect x="15" y="0" width="15" height="15"  fill="#d95f02" />
        <rect x="30" y="0" width="15" height="15"  fill="#7570b3" />
        <rect x="45" y="0" width="15" height="15"  fill="#e7298a" />
        <rect x="60" y="0" width="15" height="15"  fill="#66a61e" />
        <rect x="75" y="0" width="15" height="15"  fill="#e6ab02" />
        <rect x="90" y="0" width="15" height="15"  fill="#a6761d" />
        <rect x="105" y="0" width="15" height="15" fill="#666666" />
      </svg>
    </td>
  </tr>
  <tr>
    <td>schemePaired<br/>
      <svg width="180" height="15">
        <rect x="0" y="0" width="15" height="15"   fill="#a6cee3" />
        <rect x="15" y="0" width="15" height="15"  fill="#1f78b4" />
        <rect x="30" y="0" width="15" height="15"  fill="#b2df8a" />
        <rect x="45" y="0" width="15" height="15"  fill="#33a02c" />
        <rect x="60" y="0" width="15" height="15"  fill="#fb9a99" />
        <rect x="75" y="0" width="15" height="15"  fill="#e31a1c" />
        <rect x="90" y="0" width="15" height="15"  fill="#fdbf6f" />
        <rect x="105" y="0" width="15" height="15" fill="#ff7f00" />
        <rect x="120" y="0" width="15" height="15" fill="#cab2d6" />
        <rect x="135" y="0" width="15" height="15" fill="#6a3d9a" />
        <rect x="150" y="0" width="15" height="15" fill="#ffff99" />
        <rect x="165" y="0" width="15" height="15" fill="#b15928" />
      </svg>
    </td>
    <td>schemePastel1<br/>
      <svg width="135" height="15">
        <rect x="0" y="0" width="15" height="15"   fill="#fbb4ae" />
        <rect x="15" y="0" width="15" height="15"  fill="#b3cde3" />
        <rect x="30" y="0" width="15" height="15"  fill="#ccebc5" />
        <rect x="45" y="0" width="15" height="15"  fill="#decbe4" />
        <rect x="60" y="0" width="15" height="15"  fill="#fed9a6" />
        <rect x="75" y="0" width="15" height="15"  fill="#ffffcc" />
        <rect x="90" y="0" width="15" height="15"  fill="#e5d8bd" />
        <rect x="105" y="0" width="15" height="15" fill="#fddaec" />
        <rect x="120" y="0" width="15" height="15" fill="#f2f2f2" />
      </svg>
    </td>
    <td>schemePastel2<br/>
      <svg width="120" height="15">
        <rect x="0" y="0" width="15" height="15"   fill="#b3e2cd" />
        <rect x="15" y="0" width="15" height="15"  fill="#fdcdac" />
        <rect x="30" y="0" width="15" height="15"  fill="#cbd5e8" />
        <rect x="45" y="0" width="15" height="15"  fill="#f4cae4" />
        <rect x="60" y="0" width="15" height="15"  fill="#e6f5c9" />
        <rect x="75" y="0" width="15" height="15"  fill="#fff2ae" />
        <rect x="90" y="0" width="15" height="15"  fill="#f1e2cc" />
        <rect x="105" y="0" width="15" height="15" fill="#cccccc" />
      </svg>
    </td>
  </tr>
  <tr>
    <td>schemeSet1<br/>
      <svg width="135" height="15">
        <rect x="0" y="0" width="15" height="15"   fill="#e41a1c" />
        <rect x="15" y="0" width="15" height="15"  fill="#377eb8" />
        <rect x="30" y="0" width="15" height="15"  fill="#4daf4a" />
        <rect x="45" y="0" width="15" height="15"  fill="#984ea3" />
        <rect x="60" y="0" width="15" height="15"  fill="#ff7f00" />
        <rect x="75" y="0" width="15" height="15"  fill="#ffff33" />
        <rect x="90" y="0" width="15" height="15"  fill="#a65628" />
        <rect x="105" y="0" width="15" height="15" fill="#f781bf" />
        <rect x="120" y="0" width="15" height="15" fill="#999999" />
      </svg>
    </td>
    <td>schemeSet2<br/>
      <svg width="120" height="15">
        <rect x="0" y="0" width="15" height="15"   fill="#66c2a5" />
        <rect x="15" y="0" width="15" height="15"  fill="#fc8d62" />
        <rect x="30" y="0" width="15" height="15"  fill="#8da0cb" />
        <rect x="45" y="0" width="15" height="15"  fill="#e78ac3" />
        <rect x="60" y="0" width="15" height="15"  fill="#a6d854" />
        <rect x="75" y="0" width="15" height="15"  fill="#ffd92f" />
        <rect x="90" y="0" width="15" height="15"  fill="#e5c494" />
        <rect x="105" y="0" width="15" height="15" fill="#b3b3b3" />
      </svg>
    </td>
    <td>schemeSet3<br/>
      <svg width="180" height="15">
        <rect x="0" y="0" width="15" height="15"   fill="#8dd3c7" />
        <rect x="15" y="0" width="15" height="15"  fill="#ffffb3" />
        <rect x="30" y="0" width="15" height="15"  fill="#bebada" />
        <rect x="45" y="0" width="15" height="15"  fill="#fb8072" />
        <rect x="60" y="0" width="15" height="15"  fill="#80b1d3" />
        <rect x="75" y="0" width="15" height="15"  fill="#fdb462" />
        <rect x="90" y="0" width="15" height="15"  fill="#b3de69" />
        <rect x="105" y="0" width="15" height="15" fill="#fccde5" />
        <rect x="120" y="0" width="15" height="15" fill="#d9d9d9" />
        <rect x="135" y="0" width="15" height="15" fill="#bc80bd" />
        <rect x="150" y="0" width="15" height="15" fill="#ccebc5" />
        <rect x="165" y="0" width="15" height="15" fill="#ffed6f" />
      </svg>
    </td>
  </tr>
  <tr>
    <td>schemeTableau10<br/>
      <svg width="150" height="15">
        <rect x="0" y="0" width="15" height="15"   fill="#4e79a7" />
        <rect x="15" y="0" width="15" height="15"  fill="#f28e2c" />
        <rect x="30" y="0" width="15" height="15"  fill="#e15759" />
        <rect x="45" y="0" width="15" height="15"  fill="#76b7b2" />
        <rect x="60" y="0" width="15" height="15"  fill="#59a14f" />
        <rect x="75" y="0" width="15" height="15"  fill="#edc949" />
        <rect x="90" y="0" width="15" height="15"  fill="#af7aa1" />
        <rect x="105" y="0" width="15" height="15" fill="#ff9da7" />
        <rect x="120" y="0" width="15" height="15" fill="#9c755f" />
        <rect x="135" y="0" width="15" height="15" fill="#bab0ab" />
      </svg>
    </td>
    <td></td>
    <td></td>
  </tr>
</table>

<script>
// 各 rect の fill 属性の値（カラー）をマウスホバーで表示させるために title 要素を各 rect 以下に追加
window.addEventListener("DOMContentLoaded", () => {
  document.querySelectorAll("rect").forEach((rect) => {
    const title = document.createElementNS('http://www.w3.org/2000/svg','title');
    title.textContent = rect.getAttribute("fill");
    rect.appendChild(title);
  });
});
</script>

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


応用
----

応用例として、次の記事ではカラースケールを使った凡例の描画方法を紹介しています。

- [D3.js で SVG の右上に凡例 (legend) を表示する](/p/ykjrz2b/)

