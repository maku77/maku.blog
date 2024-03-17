---
title: "Svelte 実装例: チャットボット用の UI を作る"
url: "p/t6f8b4s/"
date: "2024-03-11"
tags: ["Svelte"]
---

{{< image border="true" src="img-001.gif" title="Svelte によるチャットボット用 UI の実装例" >}}

Svelte で ChatGPT のようなチャットボット UI を作るときの実装例です。
UI ライブラリなどを使わずに、プレーンな Svelte (+SvelteKit) だけで実装しています。
シンプルなチャットボットを作りたいときに参考になるかもしれません。

- [ソースコード（GitHub リポジトリ）](https://github.com/maku77/p-t6f8b4s-svelte-chatbot-ui)
- [デモサイト (Cloudflare Pages)](https://p-t6f8b4s-svelte-chatbot-ui.pages.dev/)

実際にはボットサーバーは存在しないので、ユーザーの入力したテキストを 1 秒後にオウム返しする関数を作っています。
下記のトップページ (`+page.svelte`) で呼び出している `sendMessageToBot()` 関数がそれです。

{{< code lang="svelte" title="src/routes/+page.svelte" >}}
<script lang="ts">
	import './global.css';
	import { sendMessageToBot } from '$lib/utils';
	import Chat, { type ChatBubbleData } from '$lib/Chat/Chat.svelte';

	/** 表示する会話の内容（ユーザーと Bot のメッセージのリスト） */
	let bubbles: ChatBubbleData[] = [];

	/**
	 * ユーザーがメッセージを入力し終わったときに呼ばれるコールバック関数です。
	 *
	 * ユーザーの入力内容は直ちにチャットバブルとして表示し、ボットに入力内容を送ります。
	 * ボットからの応答が返ってきたときに、その内容をチャットバブルとして表示します。
	 */
	async function handleSend(userMessage: string): Promise<void> {
		bubbles = [...bubbles, { name: 'You', content: userMessage }];
		const botMessage = await sendMessageToBot(userMessage);
		bubbles = [...bubbles, { name: 'Bot', content: botMessage }];
	}
</script>

<Chat {bubbles} onSend={handleSend} />
{{< /code >}}

