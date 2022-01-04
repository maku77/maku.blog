---
title: "チャットボット: ユーザーの参加／離脱のイベントをハンドルする"
url: "/p/onctywi"
date: "2019-06-28"
tags: ["Azure", "Bot Builder SDK", "Chatbot", "Node.js"]
weight: 242
---

Bot Builder SDK の `ActivityHandler` を使って、ユーザーが会話に参加したこと、離脱したことをハンドルする方法を説明します。
`ActivityHandler` を使ったボット実装の基本に関しては下記を参照してください。

- [チャットボット: ActivityHandler でボットのイベントハンドラ実装を簡略化する](/p/mgujykj)

下記は、ユーザーが新しく会話に参加したときに、ボットから挨拶するように実装した例です。
ユーザー参加のイベントをハンドルするには、[ActivityHandler#onMembersAdded()](https://docs.microsoft.com/en-us/javascript/api/botbuilder-core/activityhandler?view=botbuilder-ts-latest#onmembersadded-bothandler-) で、イベントハンドラを登録します。

{{< code lang="js" title="mybot.js" >}}
const { ActivityHandler, BotFrameworkAdapter } = require('botbuilder');

// ボット実装
class MyBot extends ActivityHandler {
  constructor() {
    super();
    this.onMessage(this.handleMessage);
    this.onMembersAdded(this.handleMembersAdded);
  }

  async handleMessage(context, next) {
    const name = context.activity.from.name;
    const utterance = context.activity.text;
    await context.sendActivity(`${name}さんは、${utterance}と言いました。`);
    await next();
  }

  async handleMembersAdded(context, next) {
    const members = context.activity.membersAdded;
    for (let i = 0; i < members.length; ++i) {
      const m = members[i];
      if (m.id !== context.activity.recipient.id) {
        // ボット以外のユーザが参加したときにメッセージを表示
        await context.sendActivity(`${m.name}さん、こんにちは！`);
      }
    }
    await next();
  }
}

exports.MyBot = MyBot;
{{< /code >}}

ちなみに、このイベントハンドラは、**ボット自身が会話に参加したときにも呼び出されます**。
ボット以外のユーザーが参加したときだけメッセージを送るには、`context.activity.recipient.id` と参加者の ID を比較し、通常のユーザーであることを確認します（`recipient.id` にはボットの ID が入っています）。

上記の例では、ユーザーが参加したときのイベントをハンドルしていますが、ユーザー離脱時のイベントも [ActivityHandler#onMembersRemoved()](https://docs.microsoft.com/en-us/javascript/api/botbuilder-core/activityhandler?view=botbuilder-ts-latest#onmembersremoved-bothandler-) を使って同様にハンドルすることができます。

Bot Framework Emulator を使っている場合は、画面上部の <samp>Restart conversation</samp> ボタンを押せば、ボットからのメッセージを確認することができます。

{{< image src="handle-members-added-001.png" >}}

Slack をチャンネル（クライアント）として使用している場合は、あるチャンネルにボットを招待したときや、そのチャンネルに新しくユーザーを招待したときに `onMembersAdded()` が呼び出されるようです。
なので、このイベントハンドラはそんなに頻繁に呼び出されるものではありません。

