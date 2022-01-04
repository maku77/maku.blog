---
title: "チャットボット: Bot Builder SDK で会話の状態を保存する (BotState)"
url: "/p/6wtzzq4"
date: "2019-06-28"
tags: ["Azure", "Bot Builder SDK", "Chatbot", "Node.js"]
weight: 252
---

BotState クラス
----

Bot Builder SDK の [BotState クラス](https://github.com/microsoft/botbuilder-js/blob/master/libraries/botbuilder-core/src/botState.ts) は、ボットとの会話内の特定のコンテキストにおける状態を保持するためのクラスです。

というと難しいですが、簡単に言うと、**会話ごとの状態保存**や、**ユーザーごとの状態保存**を行うための便利クラスです。

`BotState` クラスには次のようなサブクラスが定義されています。

- [ConversationState クラス](https://github.com/microsoft/botbuilder-js/blob/master/libraries/botbuilder-core/src/conversationState.ts) ... 会話ごとの状態を保存する
- [UserState クラス](https://github.com/microsoft/botbuilder-js/blob/master/libraries/botbuilder-core/src/userState.ts) ... ユーザーごとの状態を保存する
- [PrivateConversationState クラス](https://github.com/microsoft/botbuilder-js/blob/master/libraries/botbuilder-core/src/privateConversationState.ts) ... 会話ごとのユーザごとの状態を保存する

これらのクラスは、内部で [Storage](/p/3fnyk44) オブジェクトを利用します。

{{< image src="state-001.svg" code="state-001.puml.txt" >}}

`Storage` がグローバルに状態保存を行っていたのに対し、**`BotState` はネームスペースを考慮して状態保存を行うもの**だと考えることができます。
実際に、`ConversationState` クラスや `UserState` クラスの実装を覗いてみると、`getStorageKey()` というメソッドでストレージ用の保存キーを作成しており、それぞれ次のように構成しています。

- <b>ConversationState</b> が使用する保存キー
    - `${ channelId }/conversations/${ conversationId }/${ this.namespace }`
- <b>UserState</b> が使用する保存キー
    - `${ channelId }/users/${ userId }/${ this.namespace }`
- <b>PrivateConversationState</b> が使用するキー
    - `${ channelId }/conversations/${ conversationId }/users/${ userId }/${ this.namespace }`

実用的なボットの状態管理を行うには、`Storage` インタフェースをそのまま使うのではなく、`ConversationState` / `UserState` / `PrivateConversationState` などを使うことになるでしょう。


ConversationState と UserState クラスを使用する
----

### インスタンス化

`ConversationState` や `UserState` のインスタンスを作成するには、コンストラクタに `Storage` オブジェクトを渡します。

{{< code lang="js" >}}
const { ConversationState, UserState, MemoryStorage } = require('botbuilder');

const storage = new MemoryStorage();
const convState = new ConversationState(storage);
const userState = new UserState(storage);
{{< /code >}}

ここでは、ストレージとして `MemoryStorage` を使用していますが、`CosmosDbStorage` などを使用することもできます。


### UserState クラスでカウンタを保存する

下記のボットクラスでは、`UserState` オブジェクトを使って、ユーザーがメッセージを送った回数をカウントしています。
`UserState` に対して値を読み書きするには、**`UserState#createProperty()`** を使ってプロパティアクセサーを作成します。
ここでは、`counter` という名前のプロパティアクセサーを作成しています。

{{< code lang="js" title="mybot.js" >}}
const { ActivityHandler, BotFrameworkAdapter, UserState } = require('botbuilder');

// ボット実装
class MyBot extends ActivityHandler {
  constructor(storage) {
    super();
    this._createStateObjects(storage);

    // イベントハンドラの登録
    this.onMessage(async (context, next) => {
      const user = context.activity.from.name;
      const text = context.activity.text;

      // カウンタープロパティの値を読み取る（初期値は 1）
      const cnt = await this.counterProp.get(context, 1);

      // メッセージを返信
      await context.sendActivity(`${user}さん、${cnt}回目のメッセージですね: ${text}`);

      // カウンタープロパティの値を更新（+1 する）
      await this.counterProp.set(context, cnt + 1);

      // UserState をすべてストレージに保存する
      await this.userState.saveChanges(context);
    });
  }

  // 状態管理用のオブジェクトを作成
  _createStateObjects(storage) {
    this.userState = new UserState(storage);

    // UserState としてカウンターを保存するプロパティへのアクセサーを作成
    this.counterProp = this.userState.createProperty('counter');
  }
}

exports.MyBot = MyBot;
{{< /code >}}

これで、ユーザーごとに、ボットと会話した回数をカウントできるようになります。

上記のボットクラス実装では、`UserState` が使用する `Storage` オブジェクトをコンストラクタで受け取るようにしています。
`index.js` など、ボットクラスを生成する側で `MemoryStorage` や `CosmosDbStorage` などのインスタンスを渡すようにしてください。

{{< code lang="js" >}}
const myBot = new MyBot(new MemoryStorage());
{{< /code >}}


### ConversationState クラスでカウンタを保存する

上記の例では、`UserState` を使ってカウント値を保存していたので、ユーザーごとにカウント値がインクリメントされていきます。
`new UserState` という部分を、**`new ConversationState`** に変えるだけで、会話ごとにカンウント値を管理できるようになります。

例えば、Slack で考えてみるとわかりやすいです。

{{< image w="500" border="true" src="state-002.png" >}}

`UserState` にカウント値を保存した場合は、ユーザーがチャンネルを移動しながら会話をしたときに、カウント値が引き継がれてインクリメントされていきます。

```
Channel1: 1 → 2 → 7
Channel2: 3 → 5 → 9
Channel3: 4 → 6 → 8
```

一方、`ConversationState` にカウント値を保存した場合は、チャンネルごとにカウント値が管理されるので、各チャンネルごとに初期値の 1 からインクリメントされていきます。
ただし、このカウント値はチャンネルの中で一意なものになるので、ユーザー間で共有されます。

```
Channel1: 1 → 2 → 3
Channel2: 1 → 2 → 3
Channel3: 1 → 2 → 3
```

