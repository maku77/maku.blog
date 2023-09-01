---
title: "Svelte 応用: Hamburger ボタンを作る"
url: "p/z5cwfjm/"
date: "2023-09-01"
tags: ["Svelte"]
---

Svelte コンポーネントによる Hamburger アイコンの実装例です。
ボタンをクリックすると `open` 変数の値が true になり、CSS アニメーションで×印に変化します。

{{< image src="img-001.gif" title="Svelte コンポーネントによるハンバーガーアイコン" >}}

{{< code lang="svelte" title="src/lib/components/Hamburger.svelte" >}}
<script lang="ts">
	export let open = false;
</script>

<button class:open on:click={() => (open = !open)}>
	<svg width="40" height="40">
		<line id="top" x1="9" y1="10" x2="31" y2="10" />
		<line id="middle" x1="9" y1="20" x2="31" y2="20" />
		<line id="bottom" x1="9" y1="30" x2="31" y2="30" />
	</svg>
</button>

<style>
	button {
		display: inline-block;
		width: 40px;
		height: 40px;
		padding: 0;
		cursor: pointer;
		border: none;
		background: #333;
		box-shadow: inset 0 0 0 2px #cccccc; /* 内側に枠を付ける */
		z-index: 20; /* （これは必要に応じて）他の要素より上に表示する */
	}

	button:hover {
		background: #555;
	}

	svg line {
		stroke: currentColor;
		stroke-linecap: round;
		stroke-width: 3;
		stroke: #cccccc;
		transition: transform 0.2s ease-in-out;
	}

	.open #top {
		transform: translate(13px, -1px) rotate(45deg);
	}

	.open #middle {
		opacity: 0;
	}

	.open #bottom {
		transform: translate(-15px, 13px) rotate(-45deg);
	}
</style>
{{< /code >}}

1 つの `.svelte` ファイルでコンパクトに実装できていい感じです。

この `Hamburger` コンポーネントは、サイドバーメニューなどのコンポーネントと連携させて使うことを想定しています。
Svelte では、各コンポーネントが export している変数 (prop) に、上位コンポーネントの変数を簡単にバインドできるので、例えば次のようにして複数のコンポーネントの `open` 状態を連携させることができます。

{{< code lang="svelte" >}}
<script lang="ts">
	import Hamburger from '$lib/components/Hamburger.svelte';
	import Sidebar from '$lib/components/Sidebar.svelte';

	let open = false; // この変数で Hamburger と Sidebar の状態を同期する
</script>

<Hamburger bind:open />
<Sidebar bind:open />
{{< /code >}}

もし、コンポーネントが export している 変数の名前が異なる場合（例えば `Sidebar` が `isOpen` という名前で公開している場合）は、次のように変数をバインドすれば OK。

```svelte
<Sidebar bind:isOpen={open} />
```

{{< accordion title="src/lib/components/Sidebar.svelte（サイドバーの実装例）" >}}
{{< code lang="svelte" >}}
<script lang="ts">
	export let open = false;
</script>

<aside class:open>
	<nav>
		<a href="#about">About</a>
		<a href="#blog">Blog</a>
	</nav>
</aside>

<style>
	aside {
		position: fixed;
		top: 0;
		left: -200px;
		width: 200px;
		height: 100vh;
		background: #333333;
		transition: left 0.2s ease-in-out;
	}

	nav {
		padding: 2em 0;
	}

	a {
		display: block;
		padding: 0.5em 0.8em;
		color: white;
		text-decoration: none;
	}

	a:hover {
		background: #555555;
	}

	.open {
		left: 0;
	}
</style>
{{< /code >}}
{{< /accordion >}}

うごく～ ٩(๑❛ᴗ❛๑)۶ わーぃ

