---
title: "D3.js で SVG の右上に凡例 (legend) を表示する"
url: "p/ykjrz2b/"
date: "2023-10-18"
tags: ["D3.js"]
---

D3.js を使用してデータを色分けしてプロットする場合、グラフの右上などに凡例（色の意味）を表示したくなります。
ここでは、色を管理するために __`d3.scaleOrdinal()`__ を使用していると仮定し、凡例を表示する方法を説明します。
`d3.scaleOrdinal()` 関数についての詳細は、以下の記事をご参照ください。

- 参考: [D3.js のスケール関数: scaleOrdinal でカテゴリデータを別の値にマッピングする](/p/73b94gi/)


カラースケールから凡例に並べる項目を取り出す
----

`d3.scaleOrdinal()` 関数で作成したスケール関数（下記例では `color`）に何らかのデータを引数で渡すと、そのデータに対応する色情報を返してくれます。

```js
import * as d3 from 'd3';

// D3.js 組み込みのカラースケール
const color = d3.scaleOrdinal(d3.schemeCategory10);

const c1 = color('A');  //=> '#1f77b4'
const c2 = color('B');  //=> '#ff7f0e'
const c3 = color('C');  //=> '#2ca02c'
const c4 = color('A');  //=> '#1f77b4'
const c5 = color('B');  //=> '#ff7f0e'
```

上記の例の場合、`A`、`B`、`C` という 3 種類のデータに対して、3 種類の色が割り当てられています。
このとき、カラースケール (`color`) の内部では、これまで入力された `A`、`B`、`C` というキー情報が管理されており、__`domain()`__ メソッドを引数なしで呼び出すことで取り出すことができます。

```js
console.log(color.domain());  //=> ['A', 'B', 'C']
```

つまり、この配列データを凡例の項目として表示すればよいことになります。


実装例
----

{{< mm/d3 id="svg-u25y" w="280" h="90" title="scaleOrdinal を使った凡例描画" >}}
/* 右上に凡例を描画する */
function drawLegend(selection, colorScaler) {
  const legendLayer = selection.append('g')
    .style('transform', 'translate(calc(100% - 35px), 5px)') // 凡例全体を右上に表示
    .attr('opacity', 0.8); // ちょっと透過させてみる

  colorScaler.domain().forEach((val, i) => {
    legendLayer.append('rect')
      .attr('x', 0)
      .attr('y', i * 14)
      .attr('width', 15)
      .attr('height', 10)
      .attr('fill', colorScaler(val));
    legendLayer.append('text')
      .attr('x', 18)
      .attr('y', i * 14)
      .attr('font-size', 10)
      .attr('dominant-baseline', 'hanging') // テキストを上寄せ
      .attr('fill', colorScaler(val))
      .text(val);
  });
}

// 適当なデータを用意（同じキーのデータには同じ色が付く想定）
const data = Array.from({ length: 10 }, () => ['A', 'B', 'C']).flat();

// 組み込みのカラースケールを使用
const color = d3.scaleOrdinal(d3.schemeCategory10);

const svg = d3.select('#svg-u25y');
svg.selectAll('circle')
  .data(data)
  .join('circle')
    .attr('cx', () => Math.random() * 200 + 15)
    .attr('cy', () => Math.random() * 70 + 10)
    .attr('r', (_d, i) => (i % 4) + 3)
    .attr('fill', (d) => color(d))
    .attr('opacity', 0.7);

drawLegend(svg, color);
{{< /mm/d3 >}}

下記の関数は、与えられた D3.js セレクションに対して、凡例を描画するための描画要素を追加します。
ポイントは、カラースケールの `domain()` メソッドを使って、凡例に表示すべき項目のリストを取り出しているところです。
凡例自体の色指定にもカラースケールを利用しています。

{{< code lang="js" hl_lines="8" >}}
/* 右上に凡例を描画する */
function drawLegend(selection, colorScaler) {
  const legendLayer = selection.append('g')
    .style('transform', 'translate(calc(100% - 35px), 5px)') // 凡例全体を右上に表示
    .attr('opacity', 0.8); // ちょっと透過させてみる

  // カラースケールから凡例の項目リストを取得して描画
  colorScaler.domain().forEach((val, i) => {
    // 凡例の矩形
    legendLayer.append('rect')
      .attr('x', 0)
      .attr('y', i * 14)
      .attr('width', 15)
      .attr('height', 10)
      .attr('fill', colorScaler(val));
    // 凡例のテキスト
    legendLayer.append('text')
      .attr('x', 18)
      .attr('y', i * 14)
      .attr('font-size', 10)
      .attr('dominant-baseline', 'hanging') // テキストを上寄せ
      .attr('fill', colorScaler(val))
      .text(val);
  });
}
{{< /code >}}

あとは、何らかのデータをプロットした後に、この凡例描画関数を呼び出せば OK です。
カラースケールから表示項目を取り出しているので、先にカラースケールを使ってデータのプロットを済ませておく必要があります。

```js
// 適当なデータを用意（同じキーのデータには同じ色が付く想定）
const data = Array.from({ length: 10 }, () => ['A', 'B', 'C']).flat();

// 組み込みのカラースケールを使用
const color = d3.scaleOrdinal(d3.schemeCategory10);

// データを円でプロット（座標はランダム）
const svg = d3.select('#svg-u25y');
svg.selectAll('circle')
  .data(data)
  .join('circle')
    .attr('cx', () => Math.random() * 200 + 15)
    .attr('cy', () => Math.random() * 70 + 10)
    .attr('r', (_d, i) => (i % 4) + 3)
    .attr('fill', (d) => color(d))
    .attr('opacity', 0.7);

// 凡例を描画
drawLegend(svg, color);
```

