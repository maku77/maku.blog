---
title: "Astro アプリから D3.js を使う"
url: "p/a8rq9gs/"
date: "2023-08-14"
tags: ["Astro", "D3.js"]
---

D3.js はクライアントサイド JavaScript から SVG 画像を生成するためのライブラリです。
ここでは、Web サイト生成用のフレームワークである Astro 上で、D3.js を使う方法を紹介します。
前提条件として、Astro のプロジェクトはすでに作成済みであるとします。
まだ作成していない場合は、次のように作成できます。

{{< code lang="console" title="Astro プロジェクトの生成" >}}
$ npm create astro@latest
{{< /code >}}


D3.js はコンポーネントスクリプトからは使えない
----

D3.js はクライアントサイド JS として動作し、DOM ツリー上の SVG 要素を動的に操作することを想定して設計されています。
一方で、Astro コンポーネントのフロントマター部分に記述するスクリプトは、サーバーサイドで（ビルド時に）実行されることを想定しています。
つまり、__D3.js を使ったコードは、Astro のフロントマタースクリプトとして記述することはできません__。
残念！

{{< code lang="tsx" title="Chart.astro（間違った方法）" >}}
---
import * as d3 from "d3";
const svg = d3.select("#mysvg");  // ここでは DOM 要素にアクセスできない！
// ...
---

<svg id="mysvg" width="400" height="200"></svg>
{{< /code >}}

D3.js を使ったコードは、次のように `<script>` 要素の中に記述する必要があります。
ここに記述した JavaScript コードは、クライアントサイド（Web ブラウザ上）で実行されます。

{{< code lang="tsx" title="Chart.astro（正しい方法）" >}}
---
---

<svg id="mysvg" width="400" height="200"></svg>
<script>
import * as d3 from "d3";
const svg = d3.select("#mysvg");  // 正しく DOM 要素を参照できる
// ...
</script>
{{< /code >}}


D3.js のインストール
----

まず、`d3` モジュールと、TypeScript 用の型ファイルをインストールしておきます。

```console
$ npm install d3
$ npm install --save-dev @types/d3
```


Astro コンポーネントの作成
----

<svg id="mysvg" width="400" height="200" style="background: mistyrose; border: solid 1px orangered;"><circle cx="100" cy="100" r="10" fill="dodgerblue"></circle><circle cx="300" cy="50" r="10" fill="dodgerblue"></circle><circle cx="200" cy="150" r="10" fill="dodgerblue"></circle></svg>

下記は、D3.js を使って上のような SVG を描画する Astro コンポーネント (`Chart.astro`) の実装例です。
3 つの `circle` 要素の座標値は、別ファイルの `sample.json` ファイルから読み込んでいます。

{{< code lang="tsx" title="src/components/Chart.astro" >}}
---
interface Props {
  x: number;
  y: number;
}
const { w, h } = Astro.props as Props;
---

<svg id="mysvg" width={w} height={h}></svg>
<script>
  import * as d3 from "d3";
  import sampleData from "../data/sample.json";

  d3.select("#mysvg")
    .style("background", "mistyrose")
    .selectAll("circle")
    .data(sampleData.points)
    .join("circle")
    .attr("cx", (d) => d[0])
    .attr("cy", (d) => d[1])
    .attr("r", 10)
    .attr("fill", "dodgerblue");
</script>
<style>
  svg {
    border: solid 1px orangered;
  }
</style>
{{< /code >}}

あとは、任意のページコンポーネントから次のように `Chart.astro` コンポーネントを使用するだけです。

{{< code lang="tsx" title="src/pages/test.astro" >}}
---
import Chart from "../components/Chart.astro";
---

<!DOCTYPE html>
<meta charset="UTF-8" />
<title>D3.js test</title>
<Chart w="400" h="200" />
{{< /code >}}

できたー ٩(๑❛ᴗ❛๑)۶ わーぃ

