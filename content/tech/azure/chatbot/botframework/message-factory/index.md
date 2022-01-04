---
title: "チャットボット: Bot Builder SDK で画像やリストなどのリッチなメッセージを送る (MessageFactory)"
url: "/p/q7vw95i"
date: "2019-07-04"
tags: ["Azure", "Bot Builder SDK", "Chatbot", "Node.js"]
weight: 261
---

Activity オブジェクトと MessageFactory
----

Bot Builder SDK によるボット実装において、ユーザーにメッセージを送るには [TurnContext クラス](https://github.com/microsoft/botbuilder-js/blob/master/libraries/botbuilder-core/src/turnContext.ts) の `sendActivity()` メソッドを使用します。
下記は、単純なテキストメッセージを送る例です。

{{< code lang="js" >}}
await context.sendActivity('Hello!');
{{< /code >}}

{{< image border="true" src="message-factory-001.png" >}}

`sendActivity()` の第一引数には、このように文字列を渡すことができますが、その名の通り **`Activity` オブジェクトを渡すこともできる**ようになっています。
`Activity` オブジェクトを使うと、単純なテキストよりもリッチな形式で表示を行うことができます（どう表示されるかは各チャンネルの実装によりますが）。

`Activity` インタフェースは [botframework-schema モジュール](https://github.com/microsoft/botbuilder-js/blob/master/libraries/botframework-schema/src/index.ts) で定義されていますが、このインタフェースを意識してオブジェクトを作成することはあまりありません。
というのも、いろいろな用途の `Activity` オブジェクトを生成するためのファクトリーである [MessageFactory クラス](https://github.com/microsoft/botbuilder-js/blob/master/libraries/botbuilder-core/src/messageFactory.ts) が用意されているからです。

例えば、`MessageFactory#text()` は単純なテキストメッセージを送るための `Activity` オブジェクトを生成します。

{{< code lang="js" >}}
// const { MessageFactory } = require('botbuilder');
const msg = MessageFactory.text('Hello!');
await context.sendActivity(msg);
{{< /code >}}

これは実は下記のようにするのと同じです。

{{< code lang="js" >}}
await context.sendActivity('Hello!');
{{< /code >}}

`TurnContext#sendActivity()` に直接文字列を渡した場合は、内部で前者のような `MessageFactory.text()` による `Activity` 生成が行われています。
単純なテキストを送るだけであれば、`sendActivity('Hello')` としてしまうのが早いでしょう。


MessageFactory でリッチなメッセージを作成する
----

`MessageFactory` が提供するファクトリメソッドを使って、リッチなメッセージを送る例をいくつか紹介します。
ここでは、Bot Framework Emulator の表示例を載せておきます。

### 画像・動画を表示する -- contentUrl()

{{< image border="true" src="message-factory-002.png" >}}

{{< code lang="js" >}}
const msg = MessageFactory.contentUrl(
  'https://maku.blog/assets/img/site-logo.png',  // 画像や動画のURL
  'image/png',  // 画像や動画のMIMEタイプ
  'タイトル',   // タイトル（画像が見つからないときの代替テキスト）
  '説明文'      // 説明文（オプション）
);
await context.sendActivity(msg);
{{< /code >}}

上記の例では png 画像を表示していますが、mp4 などの動画を指定することもできます。
Youtube のアドレスをそのまま記述してもいいみたいです。

{{< image border="true" src="message-factory-003.png" >}}

{{< code lang="js" >}}
const msg = MessageFactory.contentUrl(
  'https://youtu.be/Tu_c1PfJ0nE',
  'video/mp4',
  '上野さんは不器用'
);
await context.sendActivity(msg);
{{< /code >}}

Bot Framework Emulator では埋め込みで動画再生できましたが、Slack ではテキストリンクになりました（上記の例では“上野さんは不器用”というテキスト）。


### 選択肢を表示する -- suggestedActions()

{{< image border="true" src="message-factory-004.png" >}}

{{< code lang="js" >}}
const msg = MessageFactory.suggestedActions(
  ['Red', 'Blue', 'Yellow'],
  '好きな色を選んでください'
);
await context.sendActivity(msg);
{{< /code >}}

選択肢のボタンをクリック（あるいはタップ）すると、ユーザーがその文字列をタイプしたのと同様にメッセージが送られます。
ボットが想定する入力が限定されているシーンでは、このように選択肢を表示してあげるのがよいです。

