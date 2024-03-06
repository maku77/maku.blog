---
title: "Svelte 入門: Svelte コンポーネントからイベントを発生させる"
url: "p/6d8gjyg/"
date: "2024-03-06"
tags: ["Svelte"]
---

Svelte コンポーネント内で発生するイベント（ボタンクリックなど）を親コンポーネントでハンドルできるようにすると、親子コンポーネント間で連携が可能になります（コンポーネント内の値そのものを `bind` プロパティで連携させる方法もあります）。


イベントをそのまま伝搬させる方法
----

Svelte が提供する HTML 要素は、__`on:イベント名`__ というプロパティでそのイベントを補足するためのハンドラー関数を設定できるようになっています（例: `<button on:click={handleClick}>`）。
イベントハンドラーを指定せずに `on:イベント名` というプロパティだけを指定した場合は、そのイベントがそのまま親コンポーネントに伝搬されます。
次の `MyButton` コンポーネントは、`button` 要素で発生する `click` イベントを伝搬させています。

{{< code lang="html" title="src/lib/MyButton.svelte" >}}
<button on:click>MyButton</button>
{{< /code >}}

この `click` イベントは、親コンポーネントから次のようにハンドルすることができます。

{{< code lang="svelte" title="src/routes/+page.svelte" >}}
<script lang="ts">
	import MyButton from '$lib/MyButton.svelte';

	function handleClick() {
		alert('Clicked!');
	}
</script>

<MyButton on:click={handleClick} />
{{< /code >}}

シンプルですね！
でもこれだとあまり汎用性がありません。


独自のインベントをディスパッチする方法
----

Svelte コンポーネントから独自のイベントをディスパッチするには、__`createEventDispatcher()`__ 関数で作成したディスパッチ関数を呼び出します。
ディスパッチ関数の第 1 引数はイベント名で、必要があれば第 2 引数で送信するデータを指定します。

次の `MyPrompt` コンポーネントは、ユーザーメッセージの入力エリアと、送信ボタンを備えたコンポーネントです（チャットの入力エリアのようなイメージです）。
送信ボタンを押すと、ユーザーが入力したメッセージと一意の ID を、独自の `send` イベントとしてディスパッチします。

{{< code lang="svelte" title="src/lib/MyPrompt.svelte" >}}
<script lang="ts">
	import { createEventDispatcher } from 'svelte';

	let message = ''; // ユーザーが入力したテキスト
	let messageId = 0; // 一意の ID

	// イベントディスパッチャーを作成（イベント名とデータ型のペアを並べる）
	const dispatch = createEventDispatcher<{
		send: { message: string; messageId: number }; // "send" イベントの定義
	}>();

	function handleSubmit() {
		if (message) {
			dispatch('send', { message, messageId }); // イベントを送信
			message = ''; // ユーザーが入力したテキストをクリア
			messageId += 1; // ID を更新
		}
	}
</script>

<form on:submit|preventDefault={handleSubmit}>
	<input type="text" bind:value={message} placeholder="Write a message..." />
	<button type="submit">Send</button>
</form>
{{< /code >}}

<div style="text-align: center">
  <div>
    <input type="text" placeholder="Write a message..." />
    <button>Send</button>
  </div>
  <div>図: 表示イメージ（動作しません）</div>
</div>

ディスパッチャー関数を作成する `createEventDispatcher()` の型パラメーターを指定することで、イベント名と送信するデータの型を定義することができます。
上記の例では、`send` という名前のイベント型を定義していますが、他のイベント型の定義も追加することができます。
もちろん、TypeScript を使わない場合は型パラメーターは省略できます。

この `MyPrompt` コンポーネントは次のように使用します。

{{< code lang="svelte" title="src/routes/+page.svelte" >}}
<script lang="ts">
	import type { ComponentEvents } from 'svelte';
	import MyPrompt from '$lib/MyPrompt.svelte';

	function handleSend(event: ComponentEvents<MyPrompt>['send']) {
		alert(`Message[${event.detail.messageId}]: ${event.detail.message}`);
	}
</script>

<MyPrompt on:send={handleSend} />
{{< /code >}}

`send` イベントに乗せて送られてきたデータの中身は、__`detail`__ プロパティで参照することができます。
TypeScript を使用している場合は、イベントの型を __`ComponentEvents`__ を使って表現することができます。
表現方法は上記の例を見てください。

これで、どんなイベントでも扱えますね！

٩(๑❛ᴗ❛๑)۶ わーぃ

