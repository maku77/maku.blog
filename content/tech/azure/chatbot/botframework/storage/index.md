---
title: "チャットボット: Bot Builder SDK で会話の状態を保存する (Storage)"
url: "/p/3fnyk44"
date: "2019-06-11"
tags: ["Azure", "Bot Builder SDK", "Chatbot", "Node.js"]
weight: 251
---

ボットは基本的に**ステートレスで動作する**ので、ユーザとの会話のコンテキストを把握するには、ステートの管理を明示的に行う必要があります。
Bot Builder SDK にはそのためのユーティリティクラスが用意されています。
ここでは、Node.js の `botbuilder` パッケージを使って説明します。


Storage インタフェース
----

`botbuilder` パッケージに含まれている [Storage インタフェース](https://github.com/microsoft/botbuilder-js/blob/master/libraries/botbuilder-core/src/storage.ts)は、抽象化されたストレージに JSON オブジェクトを保存するための API を定義しています。
`write`、`read`、`delete` の 3 つの API のみなのでとてもシンプルです。

### write メソッド

JSON オブジェクトをストレージに保存するための API です。
オブジェクトを保存するときには、名前（キー）を付けて、キー＆バリューの形のオブジェクトとして保存します。
下記の例では、保存したい `state` オブジェクトに、`botState` という名前を付けて保存しています。

{{< code lang="js" >}}
state.topic = 'someTopic';
await storage.write({ 'botState': state });
{{< /code >}}

### read メソッド

ストレージに保存されたオブジェクトを読み出すための API です。
読み出したいオブジェクトの名前を配列で渡すと、オブジェクトの連想配列が返ってきます。
下記の例では、`botState` という名前で保存されたオブジェクトを、`state` 変数に取り出しています。

{{< code lang="js" >}}
const items = await storage.read(['botState']);
const state = items['botState'] || {};
{{< /code >}}

### delete メソッド

ストレージに保存されたオブジェクトを削除するための API です。
削除したいオブジェクトの名前を配列で渡します。

{{< code lang="js" >}}
await storage.delete(['botState']);
{{< /code >}}


MemoryStorage クラス
----

実際にストレージを扱うには、`Storage` インタフェースを実装したクラスが必要です。
[MemoryStorage クラス](https://github.com/microsoft/botbuilder-js/blob/master/libraries/botbuilder-core/src/memoryStorage.ts) はローカルテスト用のインメモリストレージで、ボットのプロセスが終了するまで情報を保持します。

{{< code lang="js" title="MemoryStorage のインスタンス化" >}}
const { MemoryStorage } = require('botbuilder');
const storage = new MemoryStorage();
{{< /code >}}

### テスト実装

下記はローカル環境で `MemoryStorage` の振る舞いをテストするためのシンプルなボット実装です。
ユーザがメッセージを送るたびに、`state` オブジェクトに保持したカウンタの値が 1 ずつ増えていきます。

{{< code lang="js" title="ローカルテスト用のボット実装" >}}
const { ActivityTypes, BotFrameworkAdapter, MemoryStorage } = require('botbuilder');

// ストレージインスタンスの生成
const storage = new MemoryStorage();

// ストレージにアクセスカウンタを保存
async function onTurn(context) {
  const items = await storage.read(['state']);
  const state = items['state'] || { 'count': 0 };
  await context.sendActivity(`state.count = ${state.count}`);
  state.count += 1;
  await storage.write({'state': state});
}

// 以下ほぼテンプレート
const adapter = new BotFrameworkAdapter({});
adapter.onTurnError = async (context, error) => {
   console.error(`[onTurnError]: ${error}`);
};

const server = require('restify').createServer();
server.post('/api/messages', (req, res) => {
  adapter.processActivity(req, res, async (context) => {
    if (context.activity.type === ActivityTypes.Message) {
      await onTurn(context);
    }
  });
});
server.listen(process.env.port || process.env.PORT || 3978, () => {
  console.log(`Bot server listening to ${server.url}`);
});
{{< /code >}}

{{< image src="storage-001.png" >}}

### 別の Storage 実装

上記の例ではインメモリのストレージ実装として `MemoryStorage` クラスを使用しましたが、会話の状態を永続化するには別のストレージ実装を使用する必要があります。
例えば、Azure Blob Storage や Cosmos DB を使用するためのストレージ実装が用意されています。

- [BlobStorage クラス](https://github.com/microsoft/botbuilder-js/blob/master/libraries/botbuilder-azure/src/blobStorage.ts) ... Azure Blob Storage を保存先とする `Storage` 実装
- [CosmosDbStorage クラス](https://github.com/microsoft/botbuilder-js/blob/master/libraries/botbuilder-azure/src/cosmosDbStorage.ts) ... Azure Cosmos DB Storage を保存先とする `Storage` 実装


### （おまけ）ActivityHandler を使った実装

下記は、上記のボットプログラムを [ActivityHandler](/p/mgujykj) を使って実装し直したものです。
ボットのメイン処理をボットクラスとして実装すると、何をするボットなのかがわかりやすくなります。

{{< code lang="js" title="ローカルテスト用のボット実装" >}}
const { ActivityHandler, BotFrameworkAdapter, MemoryStorage } = require('botbuilder');

// ボット実装
class MyBot extends ActivityHandler {
  constructor() {
    super();

    // ストレージインスタンスの生成
    this.storage = new MemoryStorage();

    // イベントハンドラの登録
    this.onMessage(async (context, next) => {
      // ストレージにアクセスカウンタを保存
      const items = await this.storage.read(['state']);
      const state = items['state'] || { 'count': 0 };
      await context.sendActivity(`state.count = ${state.count}`);
      state.count += 1;
      await this.storage.write({'state': state});

      await next();
    });
  }
}

// 以下ほぼテンプレート
const bot = new MyBot();
const adapter = new BotFrameworkAdapter({});
adapter.onTurnError = async (context, error) => {
   console.error(`[onTurnError]: ${error}`);
};

const server = require('restify').createServer();
server.post('/api/messages', (req, res) => {
  adapter.processActivity(req, res, async (context) => {
    await bot.run(context);
  });
});
server.listen(process.env.port || process.env.PORT || 3978, () => {
  console.log(`Bot server listening to ${server.url}`);
});
{{< /code >}}


コンテキストごとに状態を保存する
----

ここまで見てきたように、`Storage` インタフェースを使用すると、複数の Activity（メッセージのやりとり）をまたぐように状態を保存できるようになります。

しかし、これはいわゆるグローバルな状態保持であり、会話ごとの状態や、ユーザーごとの状態を保存するためには追加の仕組みが必要になります。

Bot Builder SDK には、そのような用途を想定した [BotState クラス](https://github.com/microsoft/botbuilder-js/blob/master/libraries/botbuilder-core/src/botState.ts) が用意されています。
下記の記事では `BotState` クラスの使い方を説明します。

- [チャットボット: Bot Builder SDK で会話の状態を保存する (BotState, ConversationState, UserState)](/p/6wtzzq4)

