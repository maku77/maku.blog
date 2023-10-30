---
title: "Svelte 応用: インクリメンタルサーチによる頻繁な API 呼び出しを防ぐ debounce 処理"
url: "p/c3ckhdf/"
date: "2023-10-30"
tags: ["Svelte"]
---

debounce 処理とは
----

Web アプリケーションにインクリメンタルサーチの機能を組み込む際、検索 API の頻繁な呼び出しが課題となることがあります。
たとえば、"abcde" というキーワードで検索したいときに、"a" → "ab" → "abc" のように 1 文字入力するたびに API 呼び出しが走ると検索サーバーに負荷がかかってしまいます。

このような場合に、最後のユーザー入力から一定時間待つ __debounce__ 処理を入れます。
次の Svelte コンポーネントでは、入力エリアでキーボード入力するたびに `handleKeyUp` 関数を呼び出していますが、入力内容を `query` 変数に反映するのを 1 秒間待つようにしています。

{{< code lang="html" title="debounce 処理" hl_lines="10-13" >}}
<script lang="ts">
	let query = ''; // クエリ文字列（この値が変化したらサーチ API を呼び出す想定）
	let timeoutId: number; // debounce 処理用のタイマー ID

	const handleKeyUp = (event: KeyboardEvent) => {
		// リアルタイムなユーザー入力テキスト
		const text: string = (event.target as HTMLInputElement).value;

		// query 変数への代入はユーザー入力が終わってから 1 秒後に行う（debounce 処理）
		clearTimeout(timeoutId);
		timeoutId = setTimeout(() => {
			query = text;
		}, 1000);
	};
</script>

<input on:keyup={handleKeyUp} />
<p>query: {query}</p>
{{< /code >}}

こうすることで、ユーザーが一連のテキストを入力し終わったタイミングで、一度だけ検索 API を呼び出すことができるようになります（もちろん、1 秒以上の間隔を上げずにキーボード入力がされた場合ですが）。


fetch と組み合わせた使用例
----

上記の例では、最後のユーザー入力から 1 秒経過したときに `query` 変数にその入力内容を反映するようにしています。
この内容が変化するたびに API 呼び出しを行うには、次のようにリアクティブステートメント (__`$:`__) 内で `query` 変数を参照します。
ここでは、[PokéAPI](https://pokeapi.co/) でポケモンを検索するようにしてみました。

{{< image w="229" border="true" src="img-001.png" title="pikachu の検索結果" >}}

```html
<script lang="ts">
	import { browser } from '$app/environment'; // ブラウザ上での実行かを判別

	let query = ''; // クエリ文字列（この値が変化したらサーチ API を呼び出す）
	let timeoutId: number; // debounce 処理用のタイマー ID
	let apiResult: any;  // PokeAPI の検索結果

	const handleKeyUp = (event: KeyboardEvent) => {
		// リアルタイムなユーザー入力テキスト
		const text: string = (event.target as HTMLInputElement).value;

		// query 変数への代入はユーザー入力が終わってから 1 秒後に行う（debounce 処理）
		clearTimeout(timeoutId);
		timeoutId = setTimeout(() => {
			query = text;
		}, 1000);
	};

	// PokeAPI でポケモンを検索
	$: {
		if (browser && query) {
			fetch(`https://pokeapi.co/api/v2/pokemon/${query}`)
				.then((res) => res.json())
				.then((data) => { apiResult = data; });
		}
	}
</script>

<input on:keyup={handleKeyUp} />

{#if apiResult}
	<div>id: {apiResult.id}</div>
	<div>name: {apiResult.name}</div>
	<div><img src={apiResult.sprites.front_default} alt="" /></div>
{/if}
```

