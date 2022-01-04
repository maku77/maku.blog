---
title: "チャットボット (1-2) Bot Builder SDK で Echo ボットを作成する"
url: "/p/you6q5r"
linkTitle: "Echo ボットを作成する"
date: "2019-03-04"
tags: ["Azure", "Chatbot", "Bot Builder SDK"]
weight: 212
---

ここでは、Microsoft の **Bot Builder SDK** を使ったボット作成のファーストステップとして、チャットクライアントから入力されたテキストをそのままオウム返しするだけの Echo ボットを作成します。
言語としては JavaScript (Node.js) を使用することにします。

ここで作成するのはボットの本体（サービス側）で、クライアントとしては Microsoft が提供している **Bot Framework Emulator** を使用します。


Bot Builder SDK をインストールする
----

Node.js 版の Bot Builder SDK（**`botbuilder`** パッケージ）は、`npm` コマンドを使ってインストールすることができます。
パッケージの依存関係を管理するための `package.json` ファイルも、`npm init` コマンドで作成しておきます。

```
$ mkdir mybot
$ cd mybot
$ npm init -y
$ npm install --save botbuilder@4.x
```

Bot Service のインタフェースは REST API として提供することが定められているのですが、Bot Builder SDK には REST API サーバを作成する機能は含まれていません。
そこで、REST API サーバを作成するための **`restify`** パッケージも一緒にインストールしておきます（`express` などでも実装できると思いますが、リファレンス実装では `restify` が使用されています）。

```
$ npm install --save restity
```


Echo ボットの作成
----

下記の実装は、`http://localhost:3978/api/messages` というアドレスで、チャットクライアントからのメッセージを待ち受けてオウム返しで応答する最低限の実装です。

{{< code lang="js" title="app.js" >}}
const botbuilder = require('botbuilder');
const restify = require('restify');

// Server settings
const PORT = 3978;

// Create a bot adapter, which defines how the bot sends and receives messages.
const adapter = new botbuilder.BotFrameworkAdapter();

// Create an HTTP server
const server = restify.createServer();
server.listen(PORT, () => {
  console.log('%s listening to %s', server.name, server.url);
});

// Listen for incoming requests at /api/messages.
server.post('/api/messages', (req, res) => {
  // Use the adapter to process the incoming web request into a TurnContext object.
  adapter.processActivity(req, res, async (turnContext) => {
    if (turnContext.activity.type === 'message') {
      const utterance = turnContext.activity.text;
      await turnContext.sendActivity(`You said: ${ utterance }`);
    }
  });
});
{{< /code >}}

ネット上にあるサンプルコードでは、`BotFrameworkAdapter` コンストラクタのパラメータとして `appId` や `appPassword` を指定しているものがありますが、ローカル PC 上で Bot サーバを立ち上げるのであれば、上記のようなコードだけで十分です。
最初は最小限のコードで動かしてみましょう。

このボットは下記のように起動します。

{{< code >}}
$ node app.js
restify listening to http://[::]:3978
{{< /code >}}


Bot Framework Emulator から接続してみる
----

ボットのサーバーが無事に立ち上がったら、Bot Framework Emulator を使って接続のテストを行ってみます。
Emulator は下記からダウンロードしてインストールします。

- [Microsoft/BotFramework-Emulator: Bot Framework Emulator](https://github.com/Microsoft/BotFramework-Emulator)

Emulator を起動したら、メニューの <samp>File</samp> → <samp>New Bot Configuration</samp> を選択し、下記のように接続設定を行います。
入力するのは、Bot name（任意のボット名）と Endpoint URL（restify サーバで公開した URL）です。

{{< image border="true" src="echo1.png" >}}

ここで入力した Bot name は、ボットの設定ファイルの名前に使用されます（上記の例の場合、`MyBot.bot` というファイル名になる）。

<samp>Save and connect</samp> のボタンを押すと、`MyBot.bot` ファイルが保存され、Bot サーバに接続されます。
次回からは、この `MyBot.bot` ファイルを使って接続することができます。

{{< image src="echo2.png" >}}

メッセージ欄に `Hello` と入力して送信すると、Bot サーバから正しく `You said: Hello` という応答が返ってきていることがわかります。

これで、ローカルでの Echo ボットサーバの立ち上げと接続テストは成功です (^o^)/

