---
title: "Svelte 入門: 複数のページで共通のレイアウトを定義する (+layout.svelte)"
url: "p/vwyvd5x/"
date: "2023-08-22"
tags: ["Svelte"]
---

レイアウトファイル (+layout.svelte) の基本
----

Svelte のフレームワークである SvelteKit は、複数のページで使用する共通レイアウトを定義するための仕組みを提供しています。
共通レイアウトを定義するのは簡単で、__`src/routes/+layout.svelte`__ というファイルを作成するだけで、そのレイアウトが全ページ (`src/routes/**/+page.svelte`) に適用されます。

{{< code lang="svelte" title="src/routes/+layout.svelte（共通レイアウト）" hl_lines="5" >}}
<header>
	<h1>サイト名</h1>
</header>
<div class="main">
	<slot />
</div>

<style>
	header {
		position: fixed;
		top: 0;
		left: 0;
		width: 100%;
		padding: 1rem 1.5rem;
		background-color: #333;
		color: #fff;
		box-shadow: 0 2px 4px rgba(0, 0, 0, 0.5);
		z-index: 1000;
	}
	h1 {
		margin: 0;
	}
	.main {
		margin: 7rem 2rem 4rem 2rem;
	}
</style>
{{< /code >}}

各ページのコンテンツは、レイアウトファイル内の __`<slot />`__ 部分に展開されます（参考: [スロットの仕組み](/p/gcdawrj/)）。

実際には SvelteKit で作成したアプリケーションは、最上位のページテンプレートとして __`src/app.html`__ を使うので、次のような入れ子構造でコンテンツが挿入されていくことになります。

{{< image w="700" src="img-001.drawio.svg" title="Svelte の +layout.svelte ファイルの仕組み" >}}

`+layout.svelte` ファイルが存在しない場合は、`<slot />` とだけ記述されたファイルが存在するかのように振る舞います。
ようするに、`+page.svelte` の内容がそのまま `%sveltekit.body%` の位置に展開されます。


レイアウトファイルをネストする
----

`src/routes` のディレクトリ階層内に複数の `+layout.svelte` を配置すると、そのレイアウトは上位のディレクトリに配置されたレイアウトを継承します。
例えば、次のようなディレクトリ構成にすると、下位の `src/routes/admin/+layout.svelte` は、上位の `src/routes/+layout.svelte` を継承します。

- `src/routes/`
  - __`+layout.svelte`__
  - admin/
    - __`+layout.svelte`__

`src/routes/admin` ディレクトリ以下に配置されたすべてのページ (`+page.svelte`) は、`src/routes/admin/+layout.svelte` レイアウトを使ってレンダリングされ、その結果が上位のレイアウトの `<slot />` 部分に展開されるという振る舞いになります。

{{< image w="700" src="img-002.drawio.svg" title="レイアウトの継承" >}}

スロットの連チャンだー！ ٩(๑❛ᴗ❛๑)۶

