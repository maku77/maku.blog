---
title: "チャットボット: ActivityHandler でボットのイベントハンドラ実装を簡略化する"
url: "/p/mgujykj"
date: "2019-06-09"
tags: ["Azure", "Bot Builder SDK", "Chatbot", "Node.js"]
weight: 241
---

Bot Builder SDK (Node.js) の `botbuilder-core` パッケージには、[ActivityHandler](https://github.com/microsoft/botbuilder-js/blob/master/libraries/botbuilder-core/src/activityHandler.ts) という、ボットのイベントハンドラ部分の実装を簡略化するためのライブラリが含まれています。
ボットの世界では、「Activity」はひとつのメッセージの処理単位のことを示しています。
この Activity をうまくハンドルするためのクラスだから `ActivityHandler` という名前が付けられているんですね。

ここでは、独自のボットクラス (`MyBot`) を、`ActivityHandler` を利用せずに実装した場合と、利用して実装した場合で比較してみたいと思います。


ActivityHandler を使わない場合
----

例えば、下記のように `BotFrameworkAdapter` で受信したイベントの処理を `MyBot.onTurn()` に委譲するとします。

{{< code lang="js" title="mybot.js" >}}
const myBot = new MyBot();
const adapter = new BotFrameworkAdapter({});
const server = require('restify').createServer();

server.post('/api/messages', (req, res) => {
  adapter.processActivity(req, res, async (context) => {
    await myBot.onTurn(context);  // あとは MyBot に丸投げ
  });
});
{{< /code >}}

このイベントは、ユーザからメッセージを送られたときだけでなく、ユーザがチャットに参加したとき (`ConversationUpdate`) などにも発生するため、`MyBot.onTurn()` の実装の中でアクティビティタイプを見て分岐処理を行わなければなりません。

{{< code lang="js" >}}
class MyBot {
  constructor() {
    // ...
  }

  async onTurn(context) {
    if (context.activity.type === ActivityTypes.Message) {
      const utterance = context.activity.text;
      await context.sendActivity(`You said: ${utterance}`);
    } else if (context.activity.type === ActivityTypes.ConversationUpdate) {
      await context.sendActivity(`[ConversationUpdate event detected]`);
    } else {
      await context.sendActivity(`[${turnContext.activity.type} event detected]`);
    }
  }
}
{{< /code >}}

このあたりの分岐処理を簡潔にしてくれるのが `ActivityHandler` クラスです。


ActivityHandler を使う場合
----

下記は、`ActivityHandler` を利用したボット実装の例です。
**`onMessage()`** メソッドを使って通常メッセージのハンドラを登録し、**`onConversationUpdate()`** メソッドを使って会話更新時のハンドラを登録します。

{{< code lang="js" title="mybot.js" >}}
class MyBot extends ActivityHandler {
  constructor() {
    super();
    // アクティビティタイプごとのイベントハンドラを登録する
    this.onMessage(this.handleMessage);
    this.onConversationUpdate(this.handleConversationUpdate);
  }

  async handleMessage(context, next) {
    const utterance = context.activity.text;
    await context.sendActivity(`You said: ${utterance}`);
    await next();
  }

  async handleConversationUpdate(context, next) {
    await context.sendActivity('[ConversationUpdate event detected]');
    await next();
  }
}
{{< /code >}}

Microsoft のサンプルコードでは、`onMessage()` のパラメータとしてラムダ式をそのまま渡しているものが多いのですが、ネストが深くなってしまうので、上記のようにメソッド化しておいた方がよいでしょう。

あとは、`BotFrameworkAdapter` でアダプタで受信したイベントを、`ActivityHandler.run()` に渡してやれば、アクティビティタイプに従って適切なハンドラを呼び出してくれます。

{{< code lang="js" >}}
server.post('/api/messages', (req, res) => {
  adapter.processActivity(req, res, async (context) => {
    await myBot.run(context);  //★
  });
});
{{< /code >}}

ちなみに、ここでは `ActivityHandler` クラスを継承する形でボットクラスを実装していますが、`ActivityHandler` をそのままインスタンス化して使用することもできます。

{{< code lang="js" >}}
const myBot = new ActivityHandler();

myBot.onMessage(async (context, next) => {
  const utterance = context.activity.text;
  await context.sendActivity(`You said: ${utterance}`);
  await next();
});
{{< /code >}}


### 全体のコード

`ActivityHandler` を使ったボットプログラムの全体のコードを示します。
ローカルでのテスト用なので、Azure 上のボットサービスとして動作させるための設定などは省略しています。

{{< code lang="js" title="mybot.js" >}}
const { ActivityHandler, BotFrameworkAdapter } = require('botbuilder');

// ボット実装
class MyBot extends ActivityHandler {
  constructor() {
    super();
    this.onMessage(this.handleMessage);
    this.onConversationUpdate(this.handleConversationUpdate);
  }

  async handleMessage(context, next) {
    const utterance = context.activity.text;
    await context.sendActivity(`You said: ${utterance}`);
    await next();
  }

  async handleConversationUpdate(context, next) {
    await context.sendActivity('[ConversationUpdate event detected]');
    await next();
  }
}

// 下記はほぼテンプレート
const myBot = new MyBot();
const adapter = new BotFrameworkAdapter({});
adapter.onTurnError = async (context, error) => {
   console.error(`[onTurnError]: ${error}`);
};
const server = require('restify').createServer();
server.post('/api/messages', (req, res) => {
  adapter.processActivity(req, res, async (context) => {
    await myBot.run(context);
  });
});
server.listen(process.env.port || process.env.PORT || 3978, () => {
  console.log(`Bot server listening to ${server.url}`);
});
{{< /code >}}

