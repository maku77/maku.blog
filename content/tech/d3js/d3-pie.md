---
title: "D3.js で円グラフを描画する (d3.pie)"
url: "p/v3g55wz/"
date: "2023-11-30"
lastmod: "2024-01-14"
changes:
  - 2024-01-14: 半径を動的に求めるように変更。
tags: ["d3js"]
---

{{% private %}}
- https://d3js.org/d3-shape/pie
- https://d3js.org/d3-shape/arc
{{% /private %}}

D3.js を使って円グラフを描画するには、`d3.pie()` と `d3.arc()` を組み合わせて使用します。


d3.pie() 関数で扇形の角度を計算する
----

D3.js の __`d3.pie()`__ 関数を使うと、円グラフ (pie chart) やドーナツチャート (donut chart) を描画するための __角度を計算する関数オブジェクトを生成__ できます。
生成した関数にデータ配列を渡すと、各データに対応する扇形部分の開始角度 (`startAngle`) と終了角度 (`endAngle`) を計算してくれます。
角度はラジアンで表現されるので、180° は 3.1415 になります。

```js
const data = [5, 3, 2]  // 任意のデータ配列
const pie = d3.pie()    // pie 関数の生成
const arcs = pie(data)  // データ配列に角度情報を付加する

console.log(args);
```

```json
[
  {
    "data": 5, "index": 0, "value": 5, "startAngle": 0,
    "endAngle": 3.141592653589793, "padAngle": 0
  },
  {
    "data": 3, "index": 1, "value": 3, "startAngle": 3.141592653589793,
    "endAngle": 5.026548245743669, "padAngle": 0
  },
  {
    "data": 2, "index": 2, "value": 2, "startAngle": 5.026548245743669,
    "endAngle": 6.283185307179586, "padAngle": 0
  }
]
```

任意のデータ配列を `pie()` 関数に渡すと、上記のように `startAngle`、`endAngle` といった角度情報付きデータ配列を返してくれます。
元のデータ配列の各要素は `data` プロパティに格納されているので、これ以降は元のデータ配列の代わりにこの角度情報付きのデータ配列だけを使えばよいことになります（例えば、D3.js のコンポーネントの `data()` メソッドにはこの配列を渡せばよい）。

{{% note title="TypeScript を使う場合" %}}
TypeScript を使って、`pie()` 関数に渡すデータ配列の型を指定するには次のようにします。
配列型ではなく、個々の要素の型を指定することに注意してください。
単一のデータであることを強調するために、data の単数系である datum という単語を使っています。

```ts
/** 円グラフ内の各セグメントの値と装飾情報を表します。 */
type PieDatum = {
  value: number
  color: string
}

const data: PieDatum[] = [
  { value: 30, color: "red" },
  { value: 20, color: "blue" },
  { value: 10, color: "yellow" },
  // ...
]

// value() メソッドに角度計算用のプロパティを取り出すための関数を渡します
const pie = d3.pie<PieDatum>().value((d) => d.value)
const arcs = pie(data)  // 角度計算
```
{{% /note %}}


d3.arc() 関数で SVG の path 情報に変換する
----

SVG にはネイティブで円グラフを描画するための要素は存在しないので、扇形の図形などを表現するための __`path`__ 要素を作成しなければいけません。
D3.js の __`d3.arc()`__ 関数を使用すると、扇形用の `path` 要素の `d` 属性の値を求めるための関数 (__arc ジェネレーター__) を生成できます。
生成した関数には、各扇形の開始角度 (`startAngle`) と終了角度 (`endAngle`) を渡します。

{{< code lang="js" title="arc ジェネレーターで扇形用のパス情報を取得する" >}}
// 内側の半径 (innerRadius) を 0 より大きくするとドーナツチャートになる
const arc = d3.arc().innerRadius(0).outerRadius(radius)

// path 要素の d 属性用の値を求める
console.log(arc({ startAngle: 0, endAngle: 3.1415 }))
console.log(arc({ startAngle: 3.1415, endAngle: 5.0265 }))
console.log(arc({ startAngle: 5.0265, endAngle: 6.2831 }))
{{< /code >}}

{{< code title="実行結果" >}}
M0,-100A100,100,0,0,1,0.009,100L0,0Z
M0.009,100A100,100,0,0,1,-95.107,-30.897L0,0Z
M-95.107,-30.897A100,100,0,0,1,-0.009,-100L0,0Z
{{< /code >}}

{{% note title="TypeScript を使う場合" %}}
TypeScript を使用している場合は次のように型指定すると、生成された `arc()` 関数に正しく型情報が設定されます。
これは、`arc()` 関数の引数として、前述の `pie()` 関数で生成した角度情報付きデータ配列の各要素を渡すということを表しています（`pie()` 関数の戻り値は `d3.PieArcDatum<PieDatum>` の配列です）。

```ts
const arc = d3.arc<d3.PieArcDatum<PieDatum>>()
  .innerRadius(0)
  .outerRadius(radius)
```
{{% /note %}}


d3.pie() と d3.arc() を組み合わせて円グラフを描画する
----

{{< mm/d3 id="svg-7wvnsew" w="300" h="200" title="d3.pie() と d3.arc() による円グラフの描画" >}}
const svg = d3.select("#svg-7wvnsew")
const width = +svg.attr("width")
const height = +svg.attr("height")
const radius = Math.min(width, height) / 2

const data = [5, 3, 2]  // データ配列
const pie = d3.pie()  // 扇形の角度を求める関数
const arc = d3.arc().innerRadius(0).outerRadius(radius)  // 角度からパスを生成する関数
const color = d3.scaleOrdinal(d3.schemeCategory10)  // 色を生成する関数

// 円グラフ用の g 要素を作って SVG の中央に表示するよう位置調整
const pieChart = svg.append("g")
  .attr("transform", `translate(${width/2}, ${height/2}) scale(0.8)`)

pieChart
  .selectAll("path")
  .data(pie(data))
  .join("path")
    .attr("d", arc)  // 円グラフのお決まり
    .attr("fill", (_, i) => color(i.toString()))
{{< /mm/d3 >}}

上記で説明した関数を利用すれば、次のような手順でデータ配列を円グラフとして描画できます。

1. `d3.pie()` を使って、データ配列から角度情報（`startAngle` と `endAngle`）を生成する。
2. `d3.arc()` を使って、角度情報を `path` 要素用の `d` 属性の値に変換する。
3. 上記のように作成したデータをもとに `path` 要素を生成する。

次の例では、データ配列 (`[5, 3, 2]`) を入力情報として円グラフを描画しています。
ちなみに、扇形の色 (`fill`) の設定には、[`d3.scaleOrdinal()` で生成したカラースケール関数](/p/73b94gi/) を使用しています。

```html
<svg id="svg-7wvnsew" width="300" height="200"></svg>
<script>
const svg = d3.select("#svg-7wvnsew")
const width = +svg.attr("width")
const height = +svg.attr("height")
const radius = Math.min(width, height) / 2

const data = [5, 3, 2]  // データ配列
const pie = d3.pie()  // 扇形の角度を求める関数
const arc = d3.arc().innerRadius(0).outerRadius(radius)  // 角度からパスを生成する関数
const color = d3.scaleOrdinal(d3.schemeCategory10)  // 色を生成する関数

// 円グラフ用の g 要素を作って SVG の中央に表示するよう位置調整
const pieChart = svg.append("g")
  .attr("transform", `translate(${width/2}, ${height/2}) scale(0.8)`)

// 円グラフの各扇形の path 要素を作成する
pieChart
  .selectAll("path")
  .data(pie(data))
  .join("path")
    .attr("d", arc)  // 円グラフのお決まり
    .attr("fill", (_, i) => color(i.toString()))
</script>
```


表示方法のカスタマイズ
----

### ドーナツチャート

{{< mm/d3 id="svg-ng6veie" w="300" h="200" title="ドーナツチャートの描画" >}}
const svg = d3.select("#svg-ng6veie")
const width = +svg.attr("width")
const height = +svg.attr("height")
const radius = Math.min(width, height) / 2

const data = [5, 3, 2]
const pie = d3.pie()
const arc = d3.arc().innerRadius(radius / 2).outerRadius(radius)
const color = d3.scaleOrdinal(d3.schemeCategory10)
const pieChart = svg.append("g")
  .attr("transform", `translate(${width/2}, ${height/2}) scale(0.8)`)
pieChart.selectAll("path").data(pie(data)).join("path")
  .attr("d", arc)  // 円グラフのお決まり
  .attr("fill", (_, i) => color(i.toString()))
{{< /mm/d3 >}}

arc ジェネレーターの __`innerRadius()`__ メソッドで、内側の半径を設定すると、ドーナツチャートを描画することができます。

```js
const arc = d3.arc().innerRadius(radius / 2).outerRadius(radius)
```

### 扇形の隙間

{{< mm/d3 id="svg-zhxbqzj" w="300" h="200" title="扇形の境界線 (arc.padAngle() を使う方法）" >}}
const svg = d3.select("#svg-zhxbqzj")
const width = +svg.attr("width")
const height = +svg.attr("height")
const radius = Math.min(width, height) / 2

const data = [5, 3, 2]
const pie = d3.pie()
const arc = d3.arc().innerRadius(1).outerRadius(radius).padAngle(0.02)
const color = d3.scaleOrdinal(d3.schemeCategory10)
const pieChart = svg.append("g")
  .attr("transform", `translate(${width/2}, ${height/2}) scale(0.8)`)
pieChart.selectAll("path").data(pie(data)).join("path")
  .attr("d", arc)  // 円グラフのお決まり
  .attr("fill", (_, i) => color(i.toString()))
{{< /mm/d3 >}}

arc ジェネレーターの __`padAngle()`__ メソッドで、各扇形の間の隙間を指定することができます。
この隙間は、ラジアン単位の角度で指定するので、かなり小さい値を指定する必要があります（1 度は 0.017453 ラジアンです）。

```js
// padAngle() を設定するときは innerRadius() も調整する
const arc = d3.arc().innerRadius(1).outerRadius(radius).padAngle(0.02)
```

扇形の境界を表現する方法としては、各扇形の `path` 要素に `stroke` スタイルを設定する方法もあります。
むしろこちらの方がシンプルでよいかもしれません。
下記では分かりやすいように黄色で境界線を描画しています。

{{< mm/d3 id="svg-pqcngha" w="300" h="200" title="扇形の境界線（path の stroke 属性を使う方法）" >}}
const svg = d3.select("#svg-pqcngha")
const width = +svg.attr("width")
const height = +svg.attr("height")
const radius = Math.min(width, height) / 2

const data = [5, 3, 2]
const pie = d3.pie()
const arc = d3.arc().innerRadius(0).outerRadius(radius)
const color = d3.scaleOrdinal(d3.schemeCategory10)
const pieChart = svg.append("g")
  .attr("transform", `translate(${width/2}, ${height/2}) scale(0.8)`)
pieChart.selectAll("path").data(pie(data)).join("path")
  .attr("d", arc)  // 円グラフのお決まり
  .attr("fill", (_, i) => color(i.toString()))
  .attr("stroke", "yellow")
  .attr("stroke-width", 2)
{{< /mm/d3 >}}

{{< code lang="js" hl_lines="7 8" >}}
pieChart
  .selectAll("path")
  .data(pie(data))
  .join("path")
    .attr("d", arc)  // 円グラフのお決まり
    .attr("fill", (_, i) => color(i.toString()))
    .attr("stroke", "yellow")
    .attr("stroke-width", 2)
{{< /code >}}

### ドーナツチャートの中心にテキストを表示

{{< mm/d3 id="svg-2se7uux" w="300" h="200" title="中央にテキスト表示" >}}
const svg = d3.select("#svg-2se7uux")
const width = +svg.attr("width")
const height = +svg.attr("height")
const radius = Math.min(width, height) / 2

const data = [5, 3, 2]
const pie = d3.pie()
const arc = d3.arc().innerRadius(radius / 2).outerRadius(radius)
const color = d3.scaleOrdinal(d3.schemeCategory10)
const pieChart = svg.append("g")
  .attr("transform", `translate(${width/2}, ${height/2}) scale(0.8)`)
pieChart.selectAll("path").data(pie(data)).join("path")
  .attr("d", arc)  // 円グラフのお決まり
  .attr("fill", (_, i) => color(i.toString()))
  .attr("stroke", "white")
  .attr("stroke-width", 2)
pieChart.append("text")
  .attr("x", 0)
  .attr("y", 0)
  .attr("text-anchor", "middle") // 水平方向のアンカー
  .attr("dominant-baseline", "middle") // 垂直方向のアンカー
  .attr("font-size", 20)
  .attr("font-weight", 600)
  .attr("fill", "#666")
  .text("合計:" + d3.sum(data))
{{< /mm/d3 >}}

ドーナツチャートの中心の隙間にテキストを表示するには、単純に SVG の `text` 要素を追加するだけです。

```js
pieChart.append("text")
  .attr("x", 0)
  .attr("y", 0)
  .attr("text-anchor", "middle") // 水平方向のアンカー
  .attr("dominant-baseline", "middle") // 垂直方向のアンカー
  .attr("font-size", 20)
  .attr("font-weight", 600)
  .attr("fill", "#666")
  .text("合計:" + d3.sum(data))
```

### 扇形（円弧）の中にテキストを表示

{{< mm/d3 id="svg-t2om9my" w="300" h="200" title="各データの値を表示" >}}
const svg = d3.select("#svg-t2om9my")
const width = +svg.attr("width")
const height = +svg.attr("height")
const radius = Math.min(width, height) / 2

const data = [5, 3, 2]
const pie = d3.pie()
const arc = d3.arc().innerRadius(radius / 2).outerRadius(radius)
const color = d3.scaleOrdinal(d3.schemeCategory10)

// 円グラフ全体の g 要素
const pieChart = svg.append("g")
  .attr("transform", `translate(${width/2}, ${height/2}) scale(0.8)`)

// 各データごとに g 要素を作っておく
const pieElems = pieChart.selectAll(".pie-elems")
  .data(pie(data))
  .enter()
  .append("g")
  .attr("class", "pie-elems")

// 各扇形の path 要素を作成
pieElems
  .append("path")
  .attr("d", arc)  // 円グラフのお決まり
  .attr("fill", (_, i) => color(i.toString()))
  .attr("stroke", "white")
  .attr("stroke-width", 2)

// 各扇形の中央に text 要素を追加
pieElems
  .append("text")
  .attr("transform", (d) => `translate(${arc.centroid(d)})`)
  .attr("text-anchor", "middle") // 水平方向のアンカー
  .attr("dominant-baseline", "middle") // 垂直方向のアンカー
  .attr("font-size", 20)
  .attr("fill", "white")
  .text((d) => d.value)
{{< /mm/d3 >}}

`pie(data)` で生成した各データを __`arc.centroid()`__ メソッドに渡すと、扇形の領域の中央座標（重心）を返してくれます。
これを利用して、各扇形（あるいは円弧）の中央にテキストを配置することができます。
ちょっと複雑なので全体のコードを示しておきます。

{{< code lang="html" hl_lines="33" >}}
<svg id="svg-t2om9my" width="300" height="200"></svg>
<script>
const svg = d3.select("#svg-t2om9my")
const width = +svg.attr("width")
const height = +svg.attr("height")
const radius = Math.min(width, height) / 2

const data = [5, 3, 2]
const pie = d3.pie()
const arc = d3.arc().innerRadius(radius / 2).outerRadius(radius)
const color = d3.scaleOrdinal(d3.schemeCategory10)

// 円グラフ全体の g 要素
const pieChart = svg.append("g")
  .attr("transform", `translate(${width/2}, ${height/2}) scale(0.8)`)

// 各データごとに g 要素を作っておく
const pieElems = pieChart.selectAll(".pie-elems")
  .data(pie(data))
  .enter()
  .append("g")
  .attr("class", "pie-elems")

// 各扇形の path 要素を作成
pieElems
  .append("path")
  .attr("d", arc)  // 円グラフのお決まり
  .attr("fill", (_, i) => color(i.toString()))
  .attr("stroke", "white")
  .attr("stroke-width", 2)

// 各扇形の中央に text 要素を追加
pieElems
  .append("text")
  .attr("transform", (d) => `translate(${arc.centroid(d)})`)
  .attr("text-anchor", "middle") // 水平方向のアンカー
  .attr("dominant-baseline", "middle") // 垂直方向のアンカー
  .attr("font-size", 20)
  .attr("fill", "white")
  .text((d) => d.value)
</script>
{{< /code >}}
