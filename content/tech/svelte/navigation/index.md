---
title: "Svelte 応用: ページ遷移用のナビゲーションメニューを作成する"
url: "p/2nrgf2m/"
date: "2023-08-30"
tags: ["Svelte"]
---

{{% private %}}
- https://learn.svelte.dev/tutorial/page-store
{{% /private %}}

Svelte サイト内でページ遷移を行うためのナビゲーションコンポーネントの作成例です。


現在の URL（パス）の取得方法
----

SvelteKit の __`$app/stores`__ モジュールが提供する __`page`__ ストアを利用すると、現在表示中のページの URL や、その一部であるパス情報を取得することができます。
この情報を利用することで、ナビゲーションコンポーネント内のカレントページに相当する項目をハイライト表示します（この URL 情報は、パンくずリストなどでも利用できます）。

現在のページのパス情報は、次のように __`$page.url.pathname`__ 参照できます。

```svelte
<script>
  import { page } from '$app/stores';
</script>

<b>pathname = {$page.url.pathname}</b>
```

ページ遷移すると、`pathname` の値が、`/` → `/about` → `/blog` のような感じで変化します。


Nav コンポーネントの実装例
----

次の Svelte コンポーネント (`Nav`) は、サイト内でページ遷移を行うためのボタンを表示します。
現在のページに相当するボタンには、赤色の下線を表示しています。

{{< image border="true" src="img-001.png" title="Nav コンポーネントの表示" >}}

{{< code lang="svelte" title="src/lib/components/Nav.svelte" >}}
<script>
	import { page } from '$app/stores';

	const pages = [
		{ name: 'Home', path: '/' },
		{ name: 'About', path: '/about' },
		{ name: 'Blog', path: '/blog' }
	];
</script>

<nav>
	{#each pages as p}
		{#if p.path === $page.url.pathname}
			<span class="active">{p.name}</span>
		{:else}
			<a href={p.path}>{p.name}</a>
		{/if}
	{/each}
</nav>

<style>
	nav {
		display: flex;
		gap: 0.5rem;
	}

	a, span {
		color: white;
		background: #333;
		padding: 0.5em 1em;
		text-decoration: none;
	}

	a:hover {
		filter: brightness(1.4); /* ホバー時に少し明るくする */
		text-decoration: underline;
	}

	.active {
		box-shadow: inset 0 -4px 0 red; /* 赤い下線を引く */
	}
</style>
{{< /code >}}

あとは、SvelteKit のレイアウトコンポーネント (`+layout.svelte`) に次のように記述しておけば、全ページにナビゲーションメニューを表示できます。

{{< code lang="svelte" title="src/routes/+layout.svelte" >}}
<script>
	import Nav from '$lib/components/Nav.svelte';
</script>

<Nav />
<slot />
{{< /code >}}

٩(๑❛ᴗ❛๑)۶ わーぃ

- 参考: [複数のページで共通のレイアウトを定義する (+layout.svelte)](/p/vwyvd5x/)

