---
title: "React アプリの中で D3.js を使ってチャートを描画する"
url: "p/j52iy4k/"
date: "2023-04-13"
tags: ["D3.js", "React"]
---

何をするか？
----

[D3.js](https://d3js.org/) は、Web ブラウザ上で様々なデータをビジュアライズするためのデファクトスタンダードな描画ライブラリです。
ここでは、React (Next.js) アプリ内で、D3.js を使って簡単なチャートを描画してみます。

{{% note title="ほかの描画ライブラリ" %}}
JavaScript による描画ライブラリには、他にも [Chart.js](https://www.chartjs.org/) や [Mermaid.js](https://mermaid.js.org/) などいろいろなものがあります。
Chart.js を使うと、は折れ線グラフや散布図などを簡単に描画することができます。
Mermaid.js を使うと、独自フォーマットのテキストをもとにフローチャートなどを描画することができます。
これらの描画ライブラリは、D3.js と比べて簡単に使うことができますが、描画可能な図の種類は限られています。
一方、D3.js は高度なカスタマイズや自由度の高いデータの可視化が可能ですが、初学者にとってはやや学習コストが高くなっています。
{{% /note %}}


D3.js のインストール
----

NPM あるいは Yarn で、プロジェクトに D3.js の依存を追加します。
TypeScript を使用している場合は、D3.js 本体 (__`d3`__) に加え、型情報 (__`@types/d3`__) もインストールします。

{{< code lang="console" title="D3.js のインストール（NPM の場合）" >}}
$ npm install d3
$ npm install --save-dev @types/d3
{{< /code >}}

{{< code lang="console" title="D3.js のインストール（Yarn の場合）" >}}
$ yarn add d3
$ yarn add --dev @types/d3
{{< /code >}}


React コンポーネント内で D3.js を使う
----

React アプリは作成済みであるとし、D3.js を使って描画を行うコンポーネントを新しく作成します。
次の `HelloChart` コンポーネントは、D3.js を使って簡単な棒グラフを描画します。
内部的に `svg` 要素を保持しており、その中に複数の `rect` を配置することで棒グラフを構築しています。
`useRef` フックで `svg` 要素の参照を取得し、D3.js の描画先として設定しています。

{{< code lang="tsx" title="src/components/HelloChart.tsx" >}}
import { FC, useEffect, useRef } from 'react'
import * as d3 from 'd3'

export const HelloChart: FC = () => {
  const svg = useRef<SVGSVGElement>(null)

  useEffect(() => {
    drawChart(svg)
  }, [svg])

  return <svg ref={svg} width="200" height="100" />
}

function drawChart(svgRef: React.RefObject<SVGSVGElement>) {
  const data = [10, 30, 60, 40, 90, 20, 50, 70, 10, 40]
  const svg = d3.select(svgRef.current)
  svg
    .style('background', '#ddd')
    .selectAll('rect')
    .data(data)
    .join('rect')
    .attr('x', (_d, i) => i * 20)
    .attr('y', (d) => 100 - d)
    .attr('width', 18)
    .attr('height', (d) => d)
    .attr('fill', 'steelblue')
}
{{< /code >}}

あとは、何らかのページでこの `HelloChart` コンポーネントを呼び出すだけです。

{{< code lang="tsx" title="src/pages/index.tsx" >}}
import { HelloChart } from '@/components/HelloChart'

export default function Home() {
  return (
    <>
      <h1>Hello, D3.js!</h1>
      <HelloChart />
    </>
  )
}
{{< /code >}}

次のように `svg` 要素が表示されれば成功です。

{{< image border="true" src="img-001.png" title="D3.js による棒グラフの描画" >}}

٩(๑❛ᴗ❛๑)۶ わーぃ
