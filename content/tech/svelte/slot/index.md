---
title: "Svelte 入門: Svelte コンポーネントに子要素を渡せるようにする (<slot />)"
url: "p/gcdawrj/"
date: "2023-08-21"
tags: ["Svelte"]
---

スロットの基本 &lt;slot /&gt;
----

Svelte コンポーネントを使用するときに子要素として渡した内容は、Svelte コンポーネントの中から __`<slot />`__ で参照することができます。
`<slot />` の仕組みは、Vue.js や Astro でも採用されています（React.js では `children` prop で参照します）。

次の `ExtLink` コンポーネントは、外部サイトへのリンクを表示するコンポーネントの実装例です。

{{< code lang="svelte" title="src/lib/ExtLink.svelte" hl_lines="5" >}}
<script lang="ts">
	export let href: string;
</script>

<a {href} target="_blank" rel="noopener noreferrer"><slot /></a>
{{< /code >}}

この `ExtLink` コンポーネントは次のように使用します。

{{< code lang="svelte" title="src/routes/+page.svelte" hl_lines="6" >}}
<script lang="ts">
	import ExtLink from '$lib/ExtLink.svelte';
</script>

<h1>Hello</h1>
<ExtLink href="https://www.bing.com/">Bing</ExtLink>
{{< /code >}}

この例では、コンポーネント内の `<slot />` という部分に `Bing` というテキストが展開されることになります。
よって、リンク部分は次のような HTML にコンパイルされます。

```html
<a href="https://www.bing.com/" target="_blank" rel="noopener noreferrer">Bing</a>
```


名前付きスロット &lt;slot name="name" /&gt;
----

子要素を複数のパートに分けてコンポーネントに渡したいことがあります。
このような場合は、__名前付きスロット__ という仕組みを使います。
次の `Box` コンポーネントは、3 つのスロットを受け取れるようにしています。

- `header` という名前のスロット
- 名前なしスロット
- `footer` という名前のスロット

{{< code lang="html" title="src/lib/Box.svelte" hl_lines="3 6 9" >}}
<div class="container">
	<div class="header">
		<slot name="header">No header was provided</slot>
	</div>
	<div class="body">
		<slot />
	</div>
	<div class="footer">
		<slot name="footer" />
	</div>
</div>

<style>
	.container {
		display: flex;
		flex-direction: column;
		gap: 1rem;
		width: 300px;

		color: #333;
		border: 1px solid #333;
		border-radius: 0.5rem;
		box-shadow: 2px 2px 8px rgba(0, 0, 0, 0.2);
		padding: 1em;
	}
	.header {
		font-size: 1.5rem;
	}
	.footer {
		color: #666;
	}
</style>
{{< /code >}}

この `Box` コンポーネントを使うときは、次のように何らかの要素の __`slot`__ prop でどのスロットに子要素を渡すかを指定します。

{{< code lang="html" title="Box コンポーネントの使用例" >}}
<script lang="ts">
	import Box from '$lib/Box.svelte';
</script>

<Box>
	<div slot="header">ヘッダーテキスト</div>
	<p>本文 本文 本文 本文 本文 本文 本文</p>
	<p>本文 本文 本文 本文 本文 本文 本文</p>
	<p>本文 本文 本文 本文 本文 本文 本文</p>
	<div slot="footer">フッターテキスト</div>
</Box>
{{< /code >}}

{{< image w="320" src="img-001.png" title="Box コンポーネントの表示例" >}}

これで目的は達成されているように見えるのですが、名前付きスロットを使うときに `slot` prop を指定するための余計な `div` 要素が追加されています。
余計な `div` 要素を追加しないようにするには、代わりに __`svelte:fragment`__ という要素を使います。

{{< code lang="html" title="Box コンポーネントの使用例（修正版）" hl_lines="6 10" >}}
<script lang="ts">
	import Box from '$lib/Box.svelte';
</script>

<Box>
	<svelte:fragment slot="header">ヘッダーテキスト</svelte:fragment>
	<p>本文 本文 本文 本文 本文 本文 本文</p>
	<p>本文 本文 本文 本文 本文 本文 本文</p>
	<p>本文 本文 本文 本文 本文 本文 本文</p>
	<svelte:fragment slot="footer">フッターテキスト</svelte:fragment>
</Box>
{{< /code >}}

