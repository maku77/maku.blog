---
title: "Svelte 入門: 開発サーバーで動作中かどうかを調べる (dev)"
url: "p/wu4bn29/"
date: "2023-11-04"
tags: ["Svelte"]
---

{{% private %}}
- https://kit.svelte.dev/docs/modules#$app-environment-dev
{{% /private %}}

SvelteKit の __`$app/environment`__ モジュールが提供する __`dev`__ 変数 (boolean) の値を見ると、開発サーバー (`npm run dev`) で動作しているかどうかを調べることができます。
このフラグを利用することで、次のように開発環境と本番環境における振る舞いを切り替えることができます。

- 開発環境と本番環境で、異なる API エンドポイントに接続する
- 開発環境で動作しているときは、作成途中のコンポーネントを表示する

次の Svelte コンポーネントでは、`dev` 変数の値に応じて処理を分岐しています。

{{< code lang="svelte" title="+page.svelte" >}}
<script lang="ts">
	import { dev } from '$app/environment';

	let envName = dev ? '開発環境' : '本番環境';
</script>

<p>現在<b>{envName}</b>で動作しています。</p>

{#if dev}
	<p>このメッセージは<b>開発環境</b>でのみ表示されます。</p>
{:else}
	<p>このメッセージは<b>本番環境</b>でのみ表示されます。</p>
{/if}
{{< /code >}}

`npm run dev` による開発サーバー上で実行しているときは次のように出力されます。

```html
<p>現在<b>開発環境</b>で動作しています。</p>
<p>このメッセージは<b>開発環境</b>でのみ表示されます。</p>
```

`npm run build` でビルドし、`npm run preview` でプレビュー用サーバーを起動した場合は次のように出力されます。

```html
<p>現在<b>本番環境</b>で動作しています。</p>
<p>このメッセージは<b>本番環境</b>でのみ表示されます。</p>
```

