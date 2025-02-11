---
title: "Svelte 実装例: Svelte アプリから D3.js を使う"
url: "p/7acxmwo/"
date: "2023-09-04"
tags: ["D3.js", "Svelte"]
---

何をするか？
----

Svelte を使用した Web アプリケーションの中から、SVG 描画ライブラリである D3.js を使用する方法を説明します。
Svelte のプロジェクトは、`npm create svelte@latest` コマンドなどで既に作成されていることを前提とします。

- 参考: [Svelte を始める (SvelteKit で Hello World）](/p/qmy6cdh/)


D3.js のインストール
----

Svelte はビルド時に実装コードをバニラ JS の形にコンパイルするので、D3.js (__`d3`__) などの外部ライブラリは `devDependencies` としてインストールすれば OK です。
TypeScript を使う場合は、型情報の __`@types/d3`__ も一緒にインストールしておきます。

{{< code lang="console" title="d3 モジュールと型情報をインストール" >}}
$ npm install --save-dev d3 @types/d3
{{< /code >}}


D3.js による描画
----

ここでは、次のような簡単な棒グラフを Svelte アプリ内の D3.js で描画してみます。

<div style="text-align: center">
<svg width="150" height="100" style="background: rgb(255, 255, 204);"><rect x="0" y="70" width="30" height="30" fill="steelblue"></rect><rect x="30" y="40" width="30" height="60" fill="steelblue"></rect><rect x="60" y="10" width="30" height="90" fill="steelblue"></rect><rect x="90" y="60" width="30" height="40" fill="steelblue"></rect><rect x="120" y="30" width="30" height="70" fill="steelblue"></rect></svg>
<div>D3.js による棒グラフの例</div>
</div>

D3.js の __`d3.select()`__ 関数は DOM 要素を参照するため、クライアントサイド JavaScript として実行しなければいけないことに注意してください。
具体的には、Svelte の __`onMount()`__ コールバック内で呼び出すようにします。
直接 `d3.select()` 関数を呼び出してしまうと、`ReferenceError: document is not defined` のようなエラーが発生します。

下記が Svelte コンポーネントから D3.js で SVG 描画を行う実装例です。

{{< code lang="svelte" title="src/lib/components/MyChart.svelte" >}}
<script lang="ts">
	import { onMount } from 'svelte';
	import * as d3 from 'd3';

	// SVG のサイズやサンプルデータを定義
	export let width = 150;
	export let height = 100;
	const data = [30, 60, 90, 40, 70];

	// D3.js のスケール関数はこのタイミングで作成可能
	const scaleX = d3.scaleLinear().domain([0, data.length]).range([0, width]);
	const scaleY = d3.scaleLinear().domain([0, 100]).range([0, height]);

	// D3.js による描画はクライアントサイドで実行する
	onMount(draw);

	function draw() {
		d3.select('#mysvg')
			.style('background', '#ffc')
			.selectAll('rect')
			.data(data)
			.join('rect')
			.attr('x', (_d, i) => scaleX(i))
			.attr('y', (d) => height - scaleY(d))
			.attr('width', width / data.length)
			.attr('height', scaleY)
			.attr('fill', 'steelblue');
	}
</script>

<svg id="mysvg" {width} {height} />
{{< /code >}}

`onMount` のタイミングで `d3.select()` する点にだけ注意すれば、D3.js の公式サンプルをほぼそのまま動かせます。
DOM 要素に依存しない処理（スケール関数の構築など）は、できるだけコンパイル時に実行してしまった方が実行時の効率はよくなるはずです。


（おまけ）SVG 要素の表示サイズをレスポンシブにする
----

上記の例では、SVG 要素の表示サイズを固定にしていましたが、ちょっと手を加えれば、ブラウザのウィンドウサイズに応じて自動的にサイズ調整できるようになります。
次の例では、SVG のデフォルトの表示サイズ（幅）を `500px` に設定しつつ、ブラウザの横幅がそれより狭くなったときは、うまく収まるように縮小 (`max-width: 100%`) して表示するようにしています。

{{< code lang="svelte" title="MyChart.svelte（抜粋）" >}}
<script lang="ts">
	const VW = 150;  // viewBox の幅
	const VH = 100;  // viewBox の高さ
	// ...
</script>

<div style="width: 500px; max-width: 100%;">
	<svg id="mysvg" viewBox="0 0 {VW} {VH}" />
</div>
{{< /code >}}

上記のように `svg` 要素の __`viewBox`__ 属性を使用して論理的なキャンバスサイズを指定すると、D3.js による描画はそのサイズを基準に行われるようになります。
そのため、`svg` 要素の画面上での表示サイズが変わっても、内部の描画要素の再計算は必要ありません。
上記の例で言えば、サイズ 150x100 のキャンバスに描画する処理（`draw` 関数）は 1 度だけ実行すればよいということです。

٩(๑❛ᴗ❛๑)۶ わーぃ

