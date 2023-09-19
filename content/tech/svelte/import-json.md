---
title: "Svelte 応用: JSON ファイルをインポートして使う"
url: "p/pzrza7m/"
date: "2023-09-19"
tags: ["Svelte"]
---

{{% private %}}
- [Static Asset Handling | Vite](https://vitejs.dev/guide/assets.html)
{{% /private %}}

Svelte (SvelteKit) アプリ内の Svelte コンポーネントや JavaScript/TypeScript ライブラリでは、JSON ファイルを __`import`__ 文で簡単に読み込めるようになっています（正確には Vite の Static Asset Handling の仕組みを利用しています）。

ここでは、サンプルデータとして下記のようなゲームソフト情報を含んだ JSON ファイルを用意します。
この JSON ファイルはビルド時にインポートするので、`static` ディレクトリではなく `src` ディレクトリのどこかに配置します（ここでは、`lib/assets` というディレクトリに配置しています）。

{{< code lang="json" title="src/lib/assets/games.json" >}}
[
	{
		"title": "シャドウラン",
		"date": "1996-02-23",
		"price": 7800
	},
	{
		"title": "トムキャットアレイ",
		"date": "1994-12-22",
		"price": 7800
	},
	{
		"title": "真・女神転生",
		"date": "1994-02-25"
	}
]
{{< /code >}}

このファイルを Svelte コンポーネントから直接インポートしてもよいのですが、次のように明示的に TypeScript の型情報を付加するライブラリでラップしておくと便利かもしれません。

{{< code lang="ts" title="src/lib/games.ts" >}}
import rawJson from '$lib/assets/games.json';

/** ゲームソフトの情報 */
export type Game = {
	/** ゲームタイトル */
	title: string;
	/** 発売日 */
	date: string;
	/** 価格 */
	price?: number;
};

// 型情報を付けて games という名前で export する
export const games = rawJson as Game[];
{{< /code >}}

実際には、明示的に型情報を付加しなくても、JSON ファイルの内容から自動的に型情報を推測してくれます。
ただ、上記のように型情報を定義することで、ドキュメントテーションコメントを記述できたり、JSON ファイル内のデータが想定外の型で記述されているときに警告してくれるというメリットがあります。

あとは、このモジュールを任意の Svelte コンポーネントからインポートして使います。

{{< code lang="svelte" title="src/routes/sample/+page.svelte" >}}
<script lang="ts">
	import { games } from '$lib/games';
</script>

<ul>
	{#each games as g}
		<li>{g.title} ({g.date}) {g.price ? `${g.price}円` : '価格不明'}</li>
	{/each}
</ul>
{{< /code >}}

次のような感じの内容で出力されれば成功です。

```html
<ul>
  <li>シャドウラン (1996-02-23) 7800円</li>
  <li>トムキャットアレイ (1994-12-22) 7800円</li>
  <li>真・女神転生 (1994-02-25) 価格不明</li>
</ul>
```

この JSON インポート処理はビルド時に実行されるので、Svelte アプリを [SSG (Static Site Generation)](/p/4oudmxy/) ビルドするときにも使うことができます。
