---
title: "Svelte 応用: YAML ファイルをインポートして使う"
url: "p/umhwooq/"
date: "2023-09-25"
tags: ["Svelte", "YAML"]
---

{{% private %}}
- [Static Asset Handling | Vite](https://vitejs.dev/guide/assets.html)
{{% /private %}}

Svelte で YAML ファイルをインポートして使う方法
----

Svelte (SvelteKit) には YAML ファイルを直接インポートして JavaScript オブジェクトして扱う仕組みはありませんが、`raw` インポートの仕組みを使えば、YAML ファイルを文字列データとしてそのまま読み込むことができます（これは SvelteKit がビルドに使っている Vite の仕組みです）。

{{< code lang="js" title="YAML ファイルを文字列としてインポートする" >}}
import gamesYaml from '$lib/assets/games.yaml?raw'
{{< /code >}}

このようにして読み込んだ YAML テキストを、Node.js の YAML ライブラリなどでパースすれば、YAML ファイルの内容を JavaScript オブジェクトとして扱うことができます。
ビルド時に処理を完結させることができるので、例えば、[static-adapter を使った静的ビルド (SSG)](/p/4oudmxy/) が必要な環境でも利用できます。


実装例
----

YAML ファイルを扱うために `js-yaml` ライブラリを使うので、次のようにしてインストールしておきます。

{{< code lang="console" title="js-yaml（およびその型ファイル）のインストール" >}}
$ npm install --save-dev js-yaml @types/js-yaml
{{< /code >}}

サンプルの YAML ファイルとして、下記のようなゲームソフト情報を含んだ `games.yaml` を使います。
この YAML ファイルはビルド時にインポートするので、`static` ディレクトリではなく `src` ディレクトリのどこかに配置します（ここでは、`lib/assets` というディレクトリに配置しています）。

{{< code lang="yaml" title="src/lib/assets/games.yaml" >}}
- title: シャドウラン
  date: 1996-02-23
  price: 7800
- title: トムキャットアレイ
  date: 1994-12-22
  price: 7800
- title: 真・女神転生
  date: 1994-02-25
{{< /code >}}

次の Svelte コンポーネントでは、ビルド時に YAML ファイルを読み込んで、JavaScript オブジェクトと扱えるようにしていします。
実装には TypeScript を使っていますが、`raw` インポートした YAML ファイルからは型情報を推測してくれないようなので、自分で `Game` 型を定義しています。

{{< code lang="svelte" title="src/routes/+page.svelte" >}}
<script lang="ts">
	import yaml from 'js-yaml';
	import gamesYaml from '$lib/assets/games.yaml?raw';

	/** ゲームソフトの情報 */
	type Game = {
		/** ゲームタイトル */
		title: string;
		/** 発売日 */
		date: string;
		/** 価格 */
		price?: number;
	};

	// "2023-01-31" のような日付文字列が Date 型に変換されるのを防ぐために
	// オプションで JSON_SCHEMA を指定しています
	const games = yaml.load(gamesYaml, { schema: yaml.JSON_SCHEMA }) as Game[];
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

なお、JSON ファイルであれば、`raw` インポートを使わずに、そのままインポートすることができます。

- 参考: [Svelte 応用: JSON ファイルをインポートして使う](/p/pzrza7m/)

