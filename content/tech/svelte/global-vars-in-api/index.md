---
title: "Svelte 入門: API ルートのグローバル変数の生存期間"
url: "p/c28msnk/"
date: "2025-02-18"
tags: ["Svelte"]
---

何をするか？
----

Svelte/SvelteKit を使った Web アプリケーションでは、`src/routes` 以下のサーバーモジュール (**`+server.ts`**) で **`GET`** や **`POST`** 関数をエクスポートするだけで、API ルートとして動作させることができます（Web API のエンドポイントとして動作します）。

このサーバーモジュール内でグローバル変数を定義すると、その値は **サーバーが起動している間だけ保持されます**。
ここでは、この振る舞いを確認するためのサンプルコードを示します。

複数のクライアントからのリクエスト間で同じ変数が共有されるため、ユーザー情報などを格納してしまうと、セキュリティ上のリスクが生じる可能性があることに注意してください。


実装例
----

下記のサーバールート実装 (`/api/messages`) では、POST リクエストで送られてきたメッセージを最大 5 件までグローバル変数 (`messages`) に保持しています。
GET リクエストでアクセスすると、保持しているメッセージを JSON 形式で返します。

{{< code lang="ts" title="src/routes/api/messages/+server.ts" >}}
import { error, json } from '@sveltejs/kit';

// API route のグローバル変数はサーバーが起動している間のみ保持される
let messages: string[] = [];

/** /api/messages への GET リクエストを処理する */
export async function GET() {
	return json({ messages });
}

/** /api/messages への POST リクエストを処理する */
export async function POST({ request }) {
	const msg = (await request.text()).trim();
	if (!msg) {
		error(400, 'Text is required in the request body');
	}
	// 最新の 5 つのメッセージを保持
	messages = [...messages.slice(-4), msg];
	return json({ messages }, { status: 201 });
}
{{< /code >}}

ページコンポーネントからこの API ルートにアクセスするコードは以下のようになります。
テキストボックスに入力したメッセージを送信すると、API ルート側で保持しているメッセージリストが更新され、その内容が画面上に表示されます。

{{< code lang="ts" title="src/routes/+page.svelte" >}}
<script lang="ts">
	import { onMount } from 'svelte';

	/** サーバーから取得したメッセージのリスト */
	let receivedMessages: string[] = $state([]);

	/** ユーザーが入力したメッセージ */
	let userInput = $state('');

	async function fetchMessages() {
		const res = await fetch('/api/messages');
		const jsonData = await res.json();
		receivedMessages = jsonData.messages;
	}

	async function handleSubmit(e: Event) {
		e.preventDefault();
		const newMessage = userInput.trim();
		userInput = '';

		// 入力欄が空の場合は何もしない
		if (newMessage === '') {
			return;
		}

		// API エンドポイントに新しいメッセージを POST で送信
		const res = await fetch('/api/messages', {
			method: 'POST',
			headers: { 'Content-Type': 'application/json' },
			body: newMessage
		});

		// 新しいデータで表示を更新
		const jsonData = await res.json();
		receivedMessages = jsonData.messages;
	}

	// ページが読み込まれたときにメッセージを取得
	onMount(fetchMessages);
</script>

<main>
	<form>
		<input bind:value={userInput} placeholder="メッセージを入力" />
		<button type="submit" onclick={handleSubmit}>送信</button>
	</form>
	<ul>
		{#each receivedMessages.toReversed() as message}
			<li>{message}</li>
		{/each}
	</ul>
</main>
{{< /code >}}

SvelteKit のサーバーを起動して Web ブラウザーでアクセスすると、次のように最大 5 件のメッセージが表示されます。
サーバーが起動している間はこのメッセージリストは保持されており、別のブラウザーからアクセスしても同じメッセージが表示されます。

{{< image border="true" src="img-001.gif" title="API ルートでのデータ保持" >}}

理解したー ٩(๑❛ᴗ❛๑)۶ わーぃ

よく考えたら `messages` まわりの処理がスレッドセーフになっていないので、複数のリクエストが同時に来た場合に備えて Mutex などで排他制御を行わなきゃですね。
いずれにしても、このようなコードを実際のプロダクション環境で使うことは稀で、多くの場合は、データベースや KV ストアライブラリなどを利用してデータの永続化を行うことになります。

