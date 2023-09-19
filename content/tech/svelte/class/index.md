---
title: "Svelte 入門: 動的に class 属性を off/on する"
url: "p/x3vpkn4/"
date: "2023-08-22"
tags: ["Svelte"]
---

HTML 要素の class 属性の値を動的に変化させることで、スタイルの切り替えをしたいことはよくあります。
ナビゲーションメニューで、現在表示しているページをハイライトするのは典型的な例です。

ここでは、ボタンを押すことで Off/On できる簡単なランプ (`Lamp`) コンポーネントを作ってみます。

{{< image border="true" src="img-001.png" title="class 属性の変更による表示切り替え" >}}

{{< code lang="svelte" title="Lamp.svelte" hl_lines="7" >}}
<script lang="ts">
	let active = false; // ランプの On/Off 状態
	function toggle() { active = !active; }
</script>

<div class="container">
	<div class="lamp {active ? 'active' : ''}" />
	<button on:click={toggle}>
		{#if active}OFF{:else}ON{/if}
	</button>
</div>

<style>
	.container {
		display: flex;
		align-items: center;
		gap: 0.5rem;
	}

	.lamp {
		display: inline-block;
		width: 30px;
		height: 30px;
		box-sizing: border-box;
		border: 4px ridge rgba(0, 0, 0, 0.3);
		border-radius: 10px;
		background: gray;
	}

	.active {
		background: lightgreen;
	}
</style>
{{< /code >}}

ポイントは下記の部分で、`active` 変数の値が `true` のときに、`class` 属性に　`active` を追加するようにしています。

```svelte
<div class="lamp {active ? 'active' : ''}" />
```

ただし、これは Svelte では冗長な書き方です。
上記のように、boolean 変数名と CSS のクラス名が同じ場合（ここでは `active`）、Svelte では __`class:クラス名`__ という省略記述が可能です。

```svelte
<div class="lamp" class:active />
```

変数名が異なる場合（例えば `active` ではなく `isActive` である場合）は、__`class:クラス名={変数名}`__ のように指定することができます。

```svelte
<div class="lamp" class:active={isActive} />
```

このような機能がライブラリではなく、言語的に組み込まれているのが Svelte の強みですね。

