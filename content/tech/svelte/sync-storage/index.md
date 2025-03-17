---
title: "Svelte 実装例: $state ルーンによる状態変数を localStorage/sessionStorage と連動させる"
url: "p/tpjvh3q/"
date: "2025-03-17"
tags: ["Svelte"]
---

何をするか？
----

Svelte 5 のコンポーネントで使用する状態変数は `$state` ルーンを使って定義できますが、ページをリロードするとその値はリセットされてしまいます。
この記事では、`$state` ルーンで定義した状態変数の値を `localStorage` や `sessionStorage` と同期させる方法を紹介します。
`localStorage` と同期させた場合は、ブラウザを開き直したときにも以前の状態を復帰できます。
`sessionStorage` と同期させた場合は、ブラウザのタブを開いている間だけ状態を保持できます。

`$state` ルーンを使って状態変数を共有する方法は、下記の記事を参照してください。

- [Svelte 入門: 複数のコンポーネントで状態を共有する（$state ルーン）](/p/nhuh2if/)


実装例
----

{{< image border="true" src="img-001.png" title="サインイン状態を管理する AuthState クラスの実装と使用例" >}}

ここでは、サインイン状態を管理する `AuthState` クラスを作成し、その **`userName`** プロパティ（状態変数）を `localStorage` に保存する例を示します。
サインイン状態を示す **`signedIn`** プロパティも用意していますが、こちらは `$derived` ルーンで定義されており、`userName` プロパティの値に応じて自動的に値が決まるようになっています（`userName` がセットされているときに `true` になります）。

{{< code lang="ts" title="src/lib/auth-state.svelte.ts" >}}
import { onMount } from 'svelte';

const STORAGE_KEY = 'userName';

// 複数のコンポーネントで状態を共有するため、リアクティブな変数をグローバルに定義します
let userName = $state('');
let signedIn = $derived(!!userName);

export class AuthState {
	/** サインインしているユーザー名を参照するためのプロパティ (reactive) */
	get userName(): string {
		return userName;
	}

	/** サインインしているかどうかを示すプロパティ (reactive) */
	get signedIn(): boolean {
		return signedIn;
	}

	constructor() {
		// ページが読み込まれたときに localStorage の値で状態変数を初期化します
		// （このコードはコンポーネントの初期化中に呼び出す必要があります）
		onMount(() => {
			userName = localStorage.getItem(STORAGE_KEY) || '';
		});
	}

	signIn(): void {
		// 実際には正しいユーザー情報やトークンを取得して保存します
		userName = 'Maku';
		localStorage.setItem(STORAGE_KEY, userName);
	}

	signOut(): void {
		userName = '';
		localStorage.removeItem(STORAGE_KEY);
	}
}
{{< /code >}}

`AuthState` クラスのコンストラクターでは、Svelte のライフサイクルフックである `onMount()` 関数を使って、ページが読み込まれたときに `localStorage` から `userName` の値を取得しています。
これにより、ページをリロードしたときに以前の状態を復元できます。
Svelte の仕組み上、`onMount` 関数はコンポーネントの初期化中にしか呼び出せないため、`new AuthState()` というインスタンス化コードはコンポーネント側に記述する必要があります（後述）。

`signIn()` および `signOut()` メソッドは、`userName` 状態変数の値を変更すると同時に `localStorage` にもその値を保存することで、次回のページ読み込みに備えています。
ここでは、主な状態変数としてユーザー名だけを扱っていますが、実際のアプリケーションでは、サインイン時に取得したアクセストークンなどの情報を保存することが一般的です。

次に、この `AuthState` クラスを利用するコンポーネントを作成します。
サインイン／アウトのボタンを持つサイトヘッダーが典型的な例です。

{{< code lang="svelte" title="src/lib/components/SiteHeader.svelte" >}}
<script lang="ts">
	import { AuthState } from '$lib/auth-state.svelte';
	const authState = new AuthState();  // 内部で onMount フックがセットされる
</script>

<header>
	<h1>My Site</h1>
	{#if authState.signedIn}
		<div>Welcome, {authState.userName}!</div>
		<button onclick={() => authState.signOut()}>Sign out</button>
	{:else}
		<button onclick={() => authState.signIn()}>Sign in</button>
	{/if}
</header>

<style>
	* {
		margin: 0;
	}
	header {
		display: flex;
		justify-content: space-between;
		align-items: center;
		padding: 0.5rem 1rem;
		background-color: #f0f0f0;
	}
	button {
		font-size: 1rem;
		padding: 0.5rem 1rem;
		border: none;
		border-radius: 0.25rem;
		background-color: #007bff;
		color: white;
		cursor: pointer;
	}
	button:hover {
		background: #0065d8;
	}
</style>
{{< /code >}}

このコンポーネントを `+layout.svelte` に組み込むことで、全ページにサイトヘッダーを表示できます。

{{< code lang="svelte" title="src/routes/+layout.svelte" >}}
<script lang="ts">
  import SiteHeader from '$lib/components/SiteHeader.svelte';
</script>

<SiteHeader />
<slot />
{{< /code >}}

できたー ٩(๑❛ᴗ❛๑)۶ わーぃ

