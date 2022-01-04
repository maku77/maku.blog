---
title: "チャットボット: 独自のミドルウェアを作成してログを記録する"
url: "/p/fn3amda"
date: "2019-07-23"
tags: ["Azure", "Bot Builder SDK", "Chatbot", "Node.js"]
weight: 271
---

ミドルウェアとは
----

Bot Framework において、クライアントから受信したメッセージはアダプターを介してボットに届けられますが、アダプターにミドルウェアを設定しておくことで、メッセージがボットに届く前に割り込んで処理を行うことができます。

{{% shout "Adapter → Middleware1 → Middleware2 → Middleware3 → ... → YourBot" %}}

ミドルウェアは上記のように複数登録することができ、登録された順に呼び出されていきます。
アダプターにミドルウェアを追加するには、**`BotFrameworkAdapter#use()`** メソッドを使用します。

{{< code lang="js" title="ミドルウェアの追加" >}}
const { BotFrameworkAdapter } = require('botbuilder');

const adpater = new BotFrameworkAdapter(endpoint);
adapter.use(new Middleware1());
adapter.use(new Middleware2());
adapter.use(new Middleware3());
{{< /code >}}


ミドルウェアを実装する
----

独自のミドルウェアを作成するには、[Middleware インタフェース](https://docs.microsoft.com/ja-jp/javascript/api/botbuilder-core/middleware?view=botbuilder-ts-latest) が提供する **`onTurn`** メソッドを実装します。

ここでは、ユーザーの入力をコンソールに出力するだけの `ConsoleLogger` というミドルウェアクラスを実装してみます。

{{< code lang="js" title="middlewares/consoleLogger.js" >}}
exports.ConsoleLogger = class ConsoleLogger {
  async onTurn(context, next) {
    if (context.activity.type === "message") {
      console.log(context.activity.text);
    }
    await next();  // Invoke a next middleware
  }
};
{{< /code >}}

とても簡単ですね。
あと、`onTurn()` を抜ける前に忘れずに **`next()`** を呼び出して、後続のミドルウェアが正しく呼び出されるようにしておく必要があります。

このミドルウェアをアダプターに登録するには次のようにします。

{{< code lang="js" >}}
// const { ConsoleLogger } = require('./middlewares/consoleLogger.js');

const adapter = new BotFrameworkAdapter(botEndpoint);
adapter.use(new ConsoleLogger());
{{< /code >}}

これで、ユーザーがチャットクライアント（チャンネル）から `こんにちは` と入力すると、ボットサーバー側のコンソールに `こんにちは` と出力されるようになります。

ちなみに、上記のミドルウェアを TypeScript で実装すると下記のような感じになります。

{{< code lang="typescript" title="./middlewares/consoleLogger.ts" >}}
import { Middleware, TurnContext } from "botbuilder";

export class ConsoleLogger implements Middleware {
  public async onTurn(context: TurnContext, next: () => Promise<void>) {
    if (context.activity.type === 'message') {
      console.log(context.activity.text);
    }
    await next();
  }
}
{{< /code >}}


BotBuilder SDK 組み込まれているロギング用ミドルウェアを使用する
----

### TranscriptLoggerMiddleware を使う

BotBuilder SDK の `botbuilder` モジュールには、組み込みのロギング用ミドルウェアとして **`TranscriptLoggerMiddleware`** が含まれています。

- [TranscriptLogger interface | Microsoft Docs](https://docs.microsoft.com/en-us/javascript/api/botbuilder-core/transcriptlogger?view=botbuilder-ts-latest)
- [botbuilder-js/transcriptLogger.ts](https://github.com/microsoft/botbuilder-js/blob/master/libraries/botbuilder-core/src/transcriptLogger.ts)

このミドルウェアをアダプターに登録するには次のようにします。

{{< code lang="js" >}}
const { ConsoleTranscriptLogger, TranscriptLoggerMiddleware } = require('botbuilder');
adapter.use(new TranscriptLoggerMiddleware(new ConsoleTranscriptLogger()));
{{< /code >}}

ここでは、出力先をコンソールにするために、パラメータとして `ConsoleTranscriptLogger` オブジェクトを渡しています。
このようにミドルウェアを登録してからボットにアクセスすると、次のようなログがコンソールに出力されるようになります（Bot Framework Emulator から「こんにちは」と入力した場合の例です）。

{{< code lang="js" >}}
Activity Log: { text: 'こんにちは',
  textFormat: 'plain',
  type: 'message',
  channelData: { clientActivityID: '15638683487740.6rm1ga5nebo' },
  channelId: 'emulator',
  from:
   { id: 'a586cec4-adad-4c63-b4ce-db0c15cdb093',
     name: 'User',
     role: 'user' },
  locale: '',
  timestamp: 2019-07-23T07:52:28.783Z,
  conversation: { id: '31bef070-ad1b-11e9-8f89-2777303f4e30|livechat' },
  id: 'd18d3ff0-ad1e-11e9-9bd2-c9d526d6b73b',
  localTimestamp: 2019-07-23T07:52:28.000Z,
  recipient: { id: '3', name: 'Bot', role: 'bot' },
  serviceUrl: 'http://localhost:51706' }
{{< /code >}}

ユーザーがチャンネルに送信したメッセージ（アクティビティ）と、ボットがチャンネルに送信したメッセージ（アクティビティ）は別々のログとして出力されることに注意してください。
Bot Framework において、「アクティビティ」はあくまで片道のメッセージを表す単位です。

### 独自の TranscriptLogger 実装

ちなみに、上記のサンプルコードで使用している `ConsoleTranscriptLogger()` の実装は下記のようになっています ([transcriptLogger.ts (L.162)](https://github.com/microsoft/botbuilder-js/blob/master/libraries/botbuilder-core/src/transcriptLogger.ts#L162))。

{{< code lang="ts" title="transcriptLogger.ts（抜粋）">}}
/**
 * ConsoleTranscriptLogger , writes activities to Console output.
 */
export class ConsoleTranscriptLogger implements TranscriptLogger {
    /**
     * Log an activity to the transcript.
     * @param activity Activity being logged.
     */
    public logActivity(activity: Activity): void | Promise<void> {
        if (!activity) { throw new Error('Activity is required.'); }

        // tslint:disable-next-line:no-console
        console.log('Activity Log:', activity);
    }
}
{{< /code >}}

`logActivity()` メソッドを持つクラスを作ればよいだけなので、下記のように簡単に独自の `TranscriptLogger` を実装できます。
ここでは、コンソールにログを出力していますが、必要に応じて Azure Storage や CosmosDB などに出力するとよいでしょう。

{{< code lang="js" >}}
class MyLogger {
  logActivity(activity) {
    if (!activity) { throw new Error('Activity is required.'); }
    console.log('Activity Log: ', JSON.stringify(activity, null, 2));
  }
}

// こうやって使う
adapter.use(new TranscriptLoggerMiddleware(new MyLogger()));
{{< /code >}}

この `MyLogger` は `ConsoletranscriptLogger` とほぼ同様の内容ですが、`Activity` オブジェクトの内容を `JSON.stringify()` を使って全階層出力するようにしています。


応用: ミドルウェア側でセットした値をボット側で参照する
----

ミドルウェアの実装（`onTurn()` 内）で、**`TurnContext#turnState.set()`** を使って何らかの値をセットしておくと、後続のボット本体の実装の中で、**`TurnContext#turnState.get()`** でその値を参照できるようになります。

{{< code lang="js" title="ミドルウェアの実装" >}}
class MyMiddleware {
  async onTurn(context, next) {
    context.turnState.set('testKey', 'testValue');
    await next();
  }
};
{{< /code >}}

{{< code lang="js" title="ボット本体の実装" >}}
class MyBot extends ActivityHandler {
  constructor() {
    super();
    this.onMessage(async (context, next) => {
      const value = context.turnState.get('testKey');
      console.log(value);  //=> 'testValue'
      // ...
    }
}
{{< /code >}}

