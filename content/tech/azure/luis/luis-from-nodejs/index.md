---
title: "LUIS (3) Node.js から LUIS の API を利用する"
url: "/p/tewj3gs"
date: "2019-03-14"
tags: ["Azure", "Chatbot", "LUIS", "Node.js"]
description: "LUIS ポータル上で作成した LUIS アプリ（自然言語モデル）を、実際に Web API として使用する方法の説明です。"
weight: 303
---

LUIS API を呼び出すためのエンドポイント情報を調べる
----

### LUIS アプリの Publish

REST API 経由で LUIS アプリによる発話解析を行うには、LUIS ポータル上で対象の LUIS アプリを <samp>Train</samp> し、<samp>Publish</samp> しておく必要があります（LUIS アプリというのは、いわゆる訓練されたモデルのことだと考えるとよいです）。

{{< image w="600" border="true" src="luis-from-nodejs.png" >}}


### API 実行用のエンドポイント URL とキーを確認する

Publish 処理が完了すると、**エンドポイント URL** と **エンドポイントキー (Endpoint key)** を使って、LUIS アプリに対してクエリ要求を投げることができるようになります。
テスト用途であれば、LUIS ポータル上で最初に作成される **オーサリングキー (Authoring key)** でもクエリを実行できますが、最終的なユーザ環境でのクエリ実行には Azure 上で作成したリソースに付けられたエンドポイント URL とエンドポイントキーのペアを使用する必要があります。

- 参考: [オーサリングキー、エンドポイントキーとは](/p/mdyedwq)

LUIS の REST API を呼び出すための URL は、下記のような情報から構成されています（下記例の ID はデタラメです）。

- Application ID: `5c548551-f6ba-4fc8-c695-529ac194317d`
- Application version: `0.1`
- エンドポイントキー: `ff824a1409f929c8e2a15301ccff431d`

Application ID と Application version は、<samp>MANAGE</samp> タブの <samp>Application Information</samp> のページで確認することができます。

{{< image w="600" border="true" src="luis-from-nodejs2.png" >}}

エンドポイントキーは、<samp>MANAGE</samp> タブの <samp>Keys and Endpoints</samp> のページで確認することができます。
下記の例では、2 つのエンドポイントが登録されています。
エンドポイント URL でアクセスする先の実体は**リソース**として管理されており、2 つのリソース上で LUIS アプリが動作していると考えることができます。

{{< image w="700" border="true" src="luis-from-nodejs3.png" >}}

左端のカラムはリソース名です。
下の `Starter_Key` という名前のリソースは、LUIS アプリを作成したときに自動的に作られるオーサリング用（管理用）のリソース／エンドポイントです。
オーサリング API は、各 LUIS ユーザに割り当てられたオーサリングキーを使って呼び出すことになるため、ここのキー情報（`Key 1` カラム）としては、LUIS ユーザのオーサリングキーが表示されています。
オーサリングキーは 1 つしかないので、ここの表示も 1 つだけになっています。

その上の、`maku-luis-resoure` という名前のリソースは、Azure 上に作成した LUIS リソースへアクセスするためのエンドポイント情報で、上にある <samp>Assign resource</samp> ボタンを使って [LUIS アプリにあらかじめ関連付けしておいた](/p/mdyedwq)リソースです。
ここに表示されるキー情報や、エンドポイント URL は、Azure ポータルで作成した LUIS リソースにより提供されているものです。
Azure 上の LUIS リソースにはデフォルトで 2 つのエンドポイントキーが割り当てられるため、ここの表示も 2 つになっています。


Web ブラウザで LUIS API を呼び出してみる
----

LUIS のドキュメントなどで登場する「予測クエリ」という単語は、LUIS の基本機能となる、発話テキストからのインテント＆エンティティへの変換機能を意味しています。
この予測クエリ REST API を実行するためのアドレス (URL) は、<samp>MANAGE</samp> タブの <samp>Keys and Endpoints</samp> のページの <samp>Endpoint</samp> の欄で確認することができます。

{{< image w="700" border="true" src="luis-from-nodejs4.png" >}}

上記の例では、2 つのリソースの Endpoint となる URL が表示されていますが、それぞれ次のようなアドレスになっています。

{{< code title="Azure LUIS リソース側のアドレス" >}}
https://japaneast.api.cognitive.microsoft.com/luis/v2.0/apps/＜AppID＞?verbose=true&timezoneOffset=-360&subscription-key=＜EndpointKey＞&q=
{{< /code >}}

{{< code title="Starter_Key リソース側のアドレス" >}}
https://westus.api.cognitive.microsoft.com/luis/v2.0/apps/＜AppID＞?verbose=true&timezoneOffset=-360&subscription-key=＜EndpointKey＞&q=
{{< /code >}}

Azure 上の LUIS リソース `maku-luis-resource` のリージョン（上記の例では `japaneast`）と、LUIS アプリのオーサリング用の `Starter_Key` リソースのリージョン（上記の例では `westus`）が異なっていることに注意してください。
このあたりの、Azure におけるリソースのリージョン配置に関してはいろいろと制約があるようなので、なるべく最新のドキュメントを確認するようにしてください。

予測クエリ用の REST API に関する詳細は下記のドキュメントに記載されています。

- [LUIS - Endpoint API](https://westus.dev.cognitive.microsoft.com/docs/services/5819c76f40a6350ce09de1ac/operations/5819c77140a63516d81aee78)

**このアドレスの末尾部分（`q=` の後ろ）に、発話テキストをつなげる形で HTTP GET リクエストを送れば、LUIS アプリによって解析された結果が JSON 形式のテキストで返ってきます。**
HTTP GET なので、Web ブラウザのアドレス欄に直接入力してテストすることができます。
例えば、下記のようなアドレスを Web ブラウザから開けば、Web ブラウザ上に解析結果の JSON テキストが表示されます。
ここでは、「XYZって何？」という発話テキストを送信しています。

```
https://japaneast.api.cognitive.microsoft.com/luis/v2.0/apps/855c5451-fa6b-8f4c-c695-194157d293ac?verbose=true&timezoneOffset=-360&subscription-key=55bb936a080577af2124fbb86be43123&q=XYZって何？
```

{{% note %}}
「XYZって何？」というテキストは実際には URL エンコードした状態で HTTP GET リクエストで送信しなければいけませんが、Web ブラウザのアドレス欄に入力した場合は自動的に URL エンコード処理が行われるため、特に意識する必要はありません。
プログラムから HTTP GET リクエストを実行するときは、明示的に URL エンコード処理を行ってください。
{{%/ note %}}

{{< code lang="json" title="結果の例" >}}
{
  "query": "XYZって何？",
  "topScoringIntent": {
    "intent": "SearchMeaning",
    "score": 0.921842635
  },
  "intents": [
    {
      "intent": "SearchMeaning",
      "score": 0.921842635
    },
    {
      "intent": "None",
      "score": 0.0479163341
    }
  ],
  "entities": [
    {
      "entity": "xyz",
      "type": "Word",
      "startIndex": 0,
      "endIndex": 2,
      "score": 0.5719831
    }
  ]
}
{{< /code >}}

2 つのエンドポイントは同じ LUIS アプリに関連付けられているため、どちらの URL を使用して API 呼び出ししても同じ結果が返ってきます。

テストの段階では `Starter_Key` リソースの方のエンドポイント URL を使用してもよいのですが、ユーザーに提供するアプリケーションから LUIS API を呼び出すときは、必ず Azure 上の LUIS リソースの方のエンドポイント URL を使うようにしてください。
`Starter_Key` リソースの方のキー（オーサリングキー）が漏れると、予測クエリ API だけではなく、LUIS アプリ管理用の API が実行されてしまいます。


オーサリング API を呼び出す
----

LUIS API には、LUIS アプリ自体を管理するためのオーサリング API もたくさん用意されています。

- [LUIS - Authoring API](https://westus.dev.cognitive.microsoft.com/docs/services/5890b47c39e2bb17b84a55ff/operations/5890b47c39e2bb052c5b9c2f)

こちらのオーサリング API を呼び出す場合は、クエリ API とは異なり、**エンドポイントキーとしてオーサリングキー**を使用しなければいけないことに注意してください。
エンドポイント URL も、LUIS ポータルで `Starter_Key` リソースに割り当てられている URL を使わなければいけません。

URL の構成もクエリ用 API のものとは若干異なっています。
例えば、下記のアドレスは、指定した LUIS アプリに登録されている[インテントの一覧を取得する REST API](https://westus.dev.cognitive.microsoft.com/docs/services/5890b47c39e2bb17b84a55ff/operations/5890b47c39e2bb052c5b9c0d) のアドレスです。

```
https://[location].api.cognitive.microsoft.com/luis/api/v2.0/apps/{appId}/versions/{versionId}/intents
```

また、オーサリング API を呼び出すときには、**`Ocp-Apim-Subscription-Key:` という HTTP ヘッダでオーサリングキー**を付加して呼び出す必要があります（ちなみに、クエリ API の場合も、エンドポイントキーをクエリパラメータではなく、このヘッダ情報として送ることができます）。

下記は、Linux の `curl` コマンドを使ってインテントの一覧を取得する LUIS API を呼び出す例です。

{{< code title="例: 登録されているインテントの一覧を取得（ID はデタラメです）" >}}
curl -s -k -X GET https://westus.api.cognitive.microsoft.com/luis/api/v2.0/apps/c5548551-6bfa-8f4c-695c-512919d34ac7/versions/0.1/intents  -H 'Ocp-Apim-Subscription-Key: 1408fd8e319434ccff15afa2214930f4'
{{< /code >}}

{{< code lang="json" title="正常なレスポンスの例" >}}
[{"id":"2e900016-9a0e-4704-bf99-7392a68767c0","name":"None","typeId":0,"readableType":"Intent Classifier"},{"id":"04082f4d-6b63-4058-a976-4289786821e4","name":"SearchMeaning","typeId":0,"readableType":"Intent Classifier"}]
{{< /code >}}

`None` インテントと `SearchMeaning` インテントの情報が返ってきていることを確認できます。

ちなみに、正しいオーサリングキーを使わずに API を呼び出すと、次のようなエラーレスポンスが返ってきます。

{{< code lang="json" title="エラーレスポンスの例" >}}
{ "statusCode": 401, "message": "Access denied due to invalid subscription key.Make sure to provide a valid key for an active subscription." }
{{< /code >}}


Node.js から LUIS の REST API を呼び出す
----

Node.js から HTTP GET リクエストを送って JSON レスポンスを取得してみます。
HTTP リクエストを行うためのモジュールとして、ここでは `request` モジュールを使用します。
JavaScript ファイルを作成するディレクトリと同じディレクトリ内で、下記のようにインストールしておいてください。

```
$ npm install request
```

下記のサンプルコードは、コマンドライン引数で入力した「発話テキスト」を LUIS API のリクエストとして送り、その結果を標準出力へ出力します。
ここでは、エンドポイントキーを `Ocp-Apim-Subscription-Key` ヘッダとして送っています。

{{< code lang="js" title="luis.js" >}}
const HOST_NAME = 'https://japaneast.api.cognitive.microsoft.com';
const LUIS_APP_ID = 'xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx';
const ENDPOINT_KEY = 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx';

// request モジュールの初期化
const request = require('request').defaults({
  headers: {
    'Ocp-Apim-Subscription-Key': `${ENDPOINT_KEY}`
  },
  // HTTP プロキシをハードコードする場合（HTTP_PROXY 環境変数でも設定可）
  // proxy: 'http://proxy.example.com:23400/'
})

// GET リクエスト用のデータを作成する
function createRequest(text) {
  const q = encodeURIComponent(text);
  return {
    uri: `${HOST_NAME}/luis/v2.0/apps/${LUIS_APP_ID}?verbose=true&timezoneOffset=-360&q=${q}`
  }
}

// LUIS からのレスポンスを処理する（標準出力に出力するだけ）
function onResponse(err, res, body) {
  if (err) {
    console.error('Error: ' + err.message);
    return;
  }
  console.log(body);
}

// エントリポイント（REST API を呼び出す）
(function main() {
  const text = process.argv[2]
  request.get(createRequest(text), onResponse);
})();
{{< /code >}}

{{< code title="実行例" >}}
$ node luis.js "XYZ って何？"
{
  "query": "XYZ って何？",
  "topScoringIntent": {
    "intent": "SearchMeaning",
    "score": 0.921842635
  },
  "intents": [
    {
      "intent": "SearchMeaning",
      "score": 0.921842635
    },
    {
      "intent": "None",
      "score": 0.0479163341
    }
  ],
  "entities": [
    {
      "entity": "xyz",
      "type": "Word",
      "startIndex": 0,
      "endIndex": 2,
      "score": 0.5719831
    }
  ]
}
{{< /code >}}

