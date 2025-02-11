---
title: "Svelte 実装例: テキストフィールドと URL のクエリ文字列を同期させる"
url: "p/uy89x6s/"
date: "2024-03-12"
tags: ["Svelte"]
---

Web サイトに検索用のテキストフィールドなどを配置する場合、入力内容と URL のクエリパラメーターを同期させると、ブラウザのブックマークなどで入力内容を保存することができます。

{{< image border="true" src="img-001.gif" title="input 要素とクエリパラメーターの同期" >}}


Svelte での実装例
----

下記は `input` 要素を 1 つだけ持つシンプルな Svelte ページコンポーネントの例です。

{{< code lang="svelte" title="src/routes/+page.svelte" >}}
<script lang="ts">
	import { onMount } from 'svelte';
	import { page } from '$app/stores';
	import { replaceState } from '$app/navigation';

	/** 入力フィールドのテキスト */
	let query = '';

	// マウント時に URL のクエリパラメーターを取得し、入力フィールドの内容 (query) として反映する。
	onMount(() => {
		const params = $page.url.searchParams;
		query = params.get('q') ?? '';
	});

	/** 入力フィールドの内容 (query) に合わせて URL のクエリパラメーターを変更する。 */
	function updateUrl() {
		const url = $page.url; // new URL(window.location.href); でも可
		if (query) {
			url.searchParams.set('q', query);
		} else {
			url.searchParams.delete('q');
		}
		replaceState(url, {});
	}
</script>

<input type="text" bind:value={query} on:input={updateUrl} />
{{< /code >}}

__`onMount()`__ でセットしたコールバック関数は、このコンポーネントがブラウザ上で表示されたときに呼び出されます。
このタイミングで、URL のクエリパラメーター（`?q=ABC`）を参照し、テキストフィールドの値（`query` 変数）に反映しています。

`input` 要素の __`on:input`__ プロパティには、ユーザーがテキスト入力したときに呼び出されるイベントハンドラーを設定します。
このタイミングで URL 末尾のクエリパラメーターを同期させています。
ページ遷移を発生させずに URL だけ書き換えるには、`$app/navigation` が提供する __`pushState()`__ か __`replaceState()`__ 関数を使用できます。
`pushState()` を使うと呼び出しごとにブラウザの履歴に積まれてしまうので、今回のケースでは `replaceState()` を使った方がよさそうです。

これら実装により、ブックマークやリンクの URL に検索文字列を含めることができるようになります。
もちろん、ブラウザ上で URL を直接入力した場合も、`input` 要素に検索文字列が入力された状態でページを開くことができます。

便利！ ٩(๑❛ᴗ❛๑)۶ わーぃ

