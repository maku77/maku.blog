---
title: "LUIS (4) botbuilder-ai ライブラリを使って LUIS の API を利用する"
url: "/p/dtwckb9"
date: "2019-04-18"
lastmod: "2019-07-03"
tags: ["Azure", "Bot Builder SDK", "Chatbot", "LUIS", "Node.js"]
description: "Microsoft は Node.js で作成したボットアプリから LUIS API を利用するための botbuilder-ai パッケージを提供しています。この Node パッケージを使用することで、REST API を意識せずに LUIS の機能を利用できるようになります。"
weight: 304
---

{{% private %}}
- [ボットに自然言語の理解を追加する - Bot Service | Microsoft Docs](https://docs.microsoft.com/ja-jp/azure/bot-service/bot-builder-howto-v4-luis?view=azure-bot-service-4.0&tabs=js)
{{% /private %}}

[こちらの記事（Node.js から LUIS の API を利用する）](/p/tewj3gs) では、自力で LUIS の REST API を呼び出すための URL を構築していました。
ここでは、**`botbuilder-ai`** パッケージを使用して、もっと手軽に LUIS の機能を呼び出してみます。

{{< note >}}
残念ながら <code>botbuilder-ai</code> が提供している <code>LuisRecognizer</code> などのクラスは、チャットボットの実装に使用する <code>TurnContext</code> オブジェクトに依存した設計になっています。
そのため、単純なコンソールアプリケーションから <code>botbuilder-ai</code> パッケージを使用することは難しく、チャットボットの実装でしか利用できません。
{{< /note >}}


LUIS のエンドポイント情報（接続情報）を確認しておく
----

LUIS API を使用するには、下記のような LUIS アプリの APP ID やエンドポイント情報が必要です。

- <b>APP ID:</b> `c39eb4df-fbcf-224f-b8b7-a0ee445d11b3`
- <b>エンドポイント:</b> `https://japaneast.api.cognitive.microsoft.com`
- <b>エンドポイントキー（サブスクリプションキー）:</b> `c9162c5c0b5edff5270feb6145618acb`

APP ID とエンドポイントキーは、[LUIS ポータル](https://luis.ai/) から対象のアプリケーションを開き、下記のように確認できます。

- <b>APP ID:</b> <samp>MANAGE</samp>タブ → <samp>Application Information</samp>
- <b>エンドポイント:</b> <samp>MANAGE</samp>タブ → <samp>Keys and Endpoints</samp> → <samp>Endpoint</samp> カラムの URL の前半部分。
- <b>エンドポイントキー:</b> <samp>MANAGE</samp>タブ → <samp>Keys and Endpoints</samp> → <samp>Key 1</samp> カラム

LUIS のエンドポイントキーは、[Azure ポータル](https://portal.azure.com/) に作成した <samp>LUIS リソース</samp> の <samp>キー</samp> の項目に表示されるものと同じです。
念のため、同一のものが表示されているか確認しておくとよいでしょう。

{{% note %}}
LUIS ポータル上の Endpoint Keys で、<code>Starter_Key</code> （管理者用キー）以外のリソースが表示されない場合は、Azure ポータルで作成した LUIS リソース（のキー）と、LUIS ポータルで作成した LUIS アプリの関連付けができていない可能性があります。
先に、LUIS ポータル上の <samp>Keys and Endpoints</samp> から関連付けを行ってください。
{{% /note %}}

- 参考: [LUIS のオーサリングキー、エンドポイントキーとは](/p/mdyedwq)


botbuilder-ai パッケージのインストール
----

`LuisRecognizer` クラスは、`botbuilder-ai` という Node パッケージで提供されています。
`botbuilder-ai` パッケージは、`npm` コマンドで簡単にインストールすることができます。

{{< code title="botbuilder-ai のインストール" >}}
$ npm install botbuilder-ai --save
{{< /code >}}

上記のように `--save` オプションを付けて実行することで、ついでに `package.json` に依存情報を追記しておくことができます。


botbuilder-ai を使用する
----

LUIS サービスへの接続情報は、最終的には環境変数（Azure 上では App Service で設定できる）や `.bot` ファイルで管理することになると思いますが、最初はハードコーディングして動作確認するのがよいでしょう。
まずは、`botbuild-ai` モジュールの基本的な使い方を学ぶことにフォーカスしましょう。


### ベースとする Bot クラス

ボット本体は、[こちらで作成した](/p/iob68qa) `Bot` クラス (`bot.js`) をベースに作成することにします。
もともとのコードは下記のようなオウム返し実装になっており、ここに LUIS API の呼び出しを加えていきます。

{{< code lang="js" title="bot.js （元のコード）" >}}
const { ActivityTypes } = require('botbuilder');

class Bot {
  async onTurn(turnContext) {
    const type = turnContext.activity.type;
    if (type === ActivityTypes.Message) {
      const text = turnContext.activity.text;
      await turnContext.sendActivity(`You said "${text}"`);
    } else {
      await turnContext.sendActivity(`[${type} event detected]`);
    }
  }
}

exports.Bot = Bot;
{{< /code >}}

このクラスの `onTurn` メソッドは、パラメータで `TurnContext` オブジェクトを受け取るので、これを `LuisRecognizer` クラスに渡すことで LUIS への問い合わせを行うことができます。


### LuisRecognizer クラスをインスタンス化する

LUIS の API を利用するには、`botbuilder-ai` パッケージに含まれている `LuisRecognizer` クラスを使用します。
このクラスのインスタンスを作成するには、下記のような接続情報オブジェクトが必要です。

{{< code lang="js" >}}
const luisEndpoint = {
  applicationId: 'c39eb4df-fbcf-224f-b8b7-a0ee445d11b3',
  endpoint: 'https://japaneast.api.cognitive.microsoft.com',
  endpointKey: 'c9162c5c0b5edff5270feb6145618acb'
};
{{< /code >}}

[こちらの記事](/p/o2bqajv)で作成した `config.js` モジュールを使用すると、`.bot` ファイルに記述した LUIS 接続情報を簡単に取得することができます。

{{< code lang="js" >}}
const config = require('./config.js');
const luisEndpoint = config.loadLuisEndpoint('your-luis-app-name');
{{< /code >}}

例えば、`Bot` クラスのコンストラクタ内で、下記のように `LuisRecognizer` インスタンスを生成すればよいでしょう。

{{< code title="bot.js" lang="js" >}}
const { ActivityTypes } = require('botbuilder');
const { LuisRecognizer } = require('botbuilder-ai');

class Bot {
  /**
   * Bot コンストラクタ。
   * LUIS の接続情報を使って、LuisRecognizer オブジェクトを初期化します。
   */
  constructor(config) {
    const luisEndpoint = {
      applicationId: 'c39eb4df-fbcf-224f-b8b7-a0ee445d11b3',
      endpoint: 'https://japaneast.api.cognitive.microsoft.com',
      endpointKey: 'c9162c5c0b5edff5270feb6145618acb'
    };
    const luisOptions = {};
    this.luisRecognizer = new LuisRecognizer(luisEndpoint, luisOptions, true);
  }

  // ...
}
{{< /code >}}


LuisRecognizer クラスの基本的な使い方
----

**`LuisRecognizer#recognize()`** メソッドに `TurnContext` オブジェクトを渡すと、**LUIS による自然言語解析の結果** [RecognizerResult](https://github.com/microsoft/botbuilder-js/blob/master/libraries/botbuilder-core/src/recognizerResult.ts) を取得することができます。
`RecognizerResult` オブジェクトの **`intents`** プロパティを参照すると、LUIS によって認識されたユーザーの意図（インテント）の配列データを取得できます。

```
[
  { "intent": "SearchMeaning", "score": 0.8440653 },
  { "intent": "BookConference", "score": 0.1042322 },
  ...
]
```

一番スコアの高い（それらしい）トップインテントは、**`LuisRecognizer.topIntent()`** ユーティリティ関数を使って取り出すことができます。
まずはトップインテントを取り出し、その意図に応じて各エンティティを取り出すとよいでしょう。

{{< code title="bot.js（応答部分の抜粋）" lang="js" >}}
class Bot {
  /**
   * Every conversation turn calls this method.
   * @param {TurnContext} turnContext Contains all the data needed for
   * processing the conversation turn.
   */
  async onTurn(turnContext) {
    if (turnContext.activity.type === ActivityTypes.Message) {
      // LUIS による認識結果を取得
      const result = await this.luisRecognizer.recognize(turnContext);
      const topIntent = LuisRecognizer.topIntent(result);
      const topIntentName = topIntent.intent;
      const topIntentScore = topIntent.score;
      const entities = result.entities;

      // どんなインテントだと認識したのか表示（デバッグ用）
      await turnContext.sendActivity(`意図: ${JSON.stringify(topIntent)}`);

      // インテントの種類に応じてエンティティを取り出す
      let response = '';
      switch (topIntentName) {
      case 'SearchMeaning':
        const words = entities['Word'];
        if (words) {
          response = `${words[0]} の意味は〇〇〇です。`;
        } else {
          response = '単語の意味を知りたいのですね。';
        }
        break;
      case 'None':
        response = '理解できませんでした。';
        break;
      }
      await turnContext.sendActivity(response);
    } else {
      await turnContext.sendActivity(`[${turnContext.activity.type} event detected]`);
    }
  }
}
{{< /code >}}

例えば、チャットクライアントから `XYZの意味は何ですか？` と入力すると、このボットは下記のように応答します。

{{< code >}}
意図: {"intent": "SearchMeaning", "score": 0.8440653}
xyz の意味は〇〇〇です。
{{< /code >}}

LUIS サービスが行ってくれるのは、上記のような意図の認識（例では `SearchMeaning`）と、単語の抽出（例では `xyz`）だけなので、ここから先の「単語の意味を検索する部分」は別の仕組みを使って実装する必要があります。
その仕組みが完成すれば、上記の○○○の部分に実際の意味を表示することができます。

ちなみに、LUIS は意図を認識できないと、デフォルトで `None` インテントを返します。
例えば、チャットクライアントから `いあ！いあ！くとぅるふ ふたぐん！` と入力すると、ボットは下記のような応答を返すことになります。

{{< code >}}
意図: {"intent": "None", "score": 0.489056647}
理解できませんでした。
{{< /code >}}

{{< note >}}
「いあ！いあ！くとぅるふ ふたぐん！」は、クトゥルフ神話に登場する詠唱呪文のひとつです。
{{< /note >}}


(応用) デフォルトインテントを置き換える
----

LUIS はインテントを判別できなかった場合、デフォルトインテントとして `{ intent: "None", score: 0 }` を返します。
`LuisRecognizer.topIntent()` メソッドで、`RecognizerResult` オブジェクトから
トップインテントを取り出す時に、オプションパラメーターを指定すると、デフォルトのインテントを `None` 以外の任意のインテントに置き換えることができます。

{{< code lang="js" >}}
const result = await this.luisRecognizer.recognize(turnContext);
const topIntent = LuisRecognizer.topIntent(result, 'Greet', 0.75);
{{< /code >}}

