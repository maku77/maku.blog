---
title: "QnA Maker (3) Node.js から QnA Maker の API を利用する"
url: "/p/rgnvp2r"
linkTitle: "Node.js から QnA Maker の API を利用する"
date: "2019-02-28"
tags: ["Azure", "Chatbot", "Node.js", "QnA Maker"]
description: "QnA Maker で作成したナレッジベースを、実際に API として使用する方法の説明です。"
weight: 403
---

{{% private %}}
- [REST API (V4) - Node.js - QnA Maker - Azure Cognitive Services | Microsoft Docs](https://docs.microsoft.com/ja-jp/azure/cognitive-services/qnamaker/quickstarts/nodejs)
{{% /private %}}

QnA Maker API を呼び出すためのエンドポイント情報を調べる
----

### QnA Maker ナレッジベースの Publish

REST API を使用して QnA Maker のナレッジベースを使用するためには、[QnA Maker ポータル](https://qnamaker.ai/)上で対象のナレッジベースを Publish しておく必要があります。

{{< image w="500" border="true" src="qna-from-nodejs.png" >}}

Publish 処理が完了すると、ナレッジベースにアクセスするための **Endpoint key** が発行されます。

### curl での QnA Maker API の呼び出しテスト

任意の質問文に対する回答文を得るには、REST API として下記のような HTTP POST リクエストを送ります。

```
curl -X POST
  https://xxx.azurewebsites.net/qnamaker/knowledgebases/＜ナレッジベースID＞/generateAnswer
  -H "Authorization: EndpointKey ＜上記で発行したキー＞"
  -H "Content-type: application/json"
  -d "{'question':'<質問文>'}"
```

Linux の `curl` コマンドを使用できる環境であれば、上記のように実行するだけで JSON 形式のレスポンスを確認することができます。


Node.js から QnA Maker の REST API を呼び出す
----

Node.js から HTTP POST リクエストを送って JSON レスポンスを取得してみます。
HTTP リクエストを行うためのモジュールとして、ここでは `request` モジュールを使用します。
JavaScript ファイルを作成するディレクトリと同じディレクトリ内で、下記のようにインストールしておいてください。

```
$ npm install request
```

QnAMaker の TypeScript (JavaScript) API ドキュメントはこちら。すごい探しにくいです。。。

- [QnAMaker class | Microsoft Docs](https://docs.microsoft.com/en-us/javascript/api/botbuilder-ai/qnamaker?view=botbuilder-ts-latest)

下記のサンプルコードは、コマンドライン引数で入力した「質問文」を QnA Maker REST API のリクエストとして送り、その結果を標準出力へ出力します。

{{< code lang="js" title="qna.js" >}}
const HOST_NAME = 'https://xxx.azurewebsites.net/qnamaker';
const KNOWLEDGE_BASE_ID = 'xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx';
const ENDPOINT_KEY = 'xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx';

// request モジュールの初期化
const request = require('request').defaults({
  headers: {
    'Authorization': `EndpointKey ${ENDPOINT_KEY}`,
    'Content-type': 'application/json',
  },
  // HTTP プロキシをハードコードする場合（HTTP_PROXY 環境変数でも設定可）
  // proxy: 'http://proxy.example.com:23400/'
})

// POST リクエスト用のデータを作成する
function createRequest(question) {
  return {
    uri: `${HOST_NAME}/knowledgebases/${KNOWLEDGE_BASE_ID}/generateAnswer`,
    body: JSON.stringify({question: question})
  }
}

// QnA Maker からのレスポンスを処理する（標準出力に出力するだけ）
function onResponse(err, res, body) {
  if (err) {
    console.error('Error: ' + err.message);
    return;
  }
  console.log(body);
}

// エントリポイント（REST API を呼び出す）
(function main() {
  const question = process.argv[2]
  request.post(createRequest(question), onResponse);
})();
{{< /code >}}

{{< code title="実行例" >}}
$ node qna.js "取扱説明書をなくしてしまったのですが"
{
  "answers": [
    {
      "questions": [
        "取扱説明書をなくしてしまったのですが、新たにもらえませんか。"
      ],
      "answer": "［こちら](#dummy)から最新の取扱説明書（PDF）をダウンロードして...",
      "score": 49.8,
      "id": 623,
      "source": "https://nestle.jp/brand/ndg/faq/expressly/",
      "metadata": []
    }
  ]
}
{{< /code >}}

