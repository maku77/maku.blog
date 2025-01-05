---
title: "Amazon API Gateway で API キーでアクセス制限された Web API を作成する"
url: "p/j2ij4dv/"
date: "2025-01-05"
tags: ["AWS", "AWS/API Gateway"]
---

Amazon API Gateway で作成した Web API には、**独自の API キーによるアクセス制限** をかけることができます。
これにより、**`x-api-key`** ヘッダーが含まれていない HTTP リクエストを拒否することができます。
例えば、AWS 外の特定のバックエンドサービスからのアクセスのみを許可して連携させたいときに便利です。
ここでは、簡単な Hello World API を作成して、API キーによるアクセス制限をかけてみます。


Lambda 関数を作成する
----

最初に、API 実装として Lambda 関数を作成しておきます。

1. [AWS マネジメントコンソール](https://aws.amazon.com/console/) にサインインして、**Lambda** サービスを開きます。
1. **`関数の作成`** をクリックして、`一から作成 (Author from scratch)` で次のように作成します。
   - 関数名 (Function name): `hello`
   - ランタイム (Runtime): `Node.js 22.x`
   - アーキテクチャ (Architecture): `arm64` （intel アーキテクチャより少し安い）

次のような Lambda 関数のコードが生成されたら成功です。

{{< code lang="js" title="index.mjs" >}}
export const handler = async (event) => {
  // TODO implement
  const response = {
    statusCode: 200,
    body: JSON.stringify('Hello from Lambda!'),
  };
  return response;
};
{{< /code >}}


API Gateway で Web API を作成する
----

次に、API Gateway で API を作成して、上記の Lambda 関数を HTTP 経由で呼び出せるようにします。

1. [AWS マネジメントコンソール](https://aws.amazon.com/console/) にサインインして、**API Gateway** サービスを開きます。
1. **`API の作成`** をクリックして、次のように作成します。
   - API タイプ: `REST API` （`HTTP API` だと API キーが設定できないっぽい）
   - API の詳細: `新しい API`
   - API 名: `hello-api`
1. **`API: hello-api のリソース`** のページから、**`メソッドの作成`** をクリックして、次のように作成します。
   - メソッドタイプ: `GET`
   - 統合タイプ: `Lambda 関数`
   - Lambda プロキシ統合: ON
   - Lambda 関数: 先ほど作成した `hello` 関数を選択
1. **`API のデプロイ`** ボタンをクリックして API をデプロイします。
   - 初回は新しいステージ名を入力して作成します（ここでは **`prod`** とします）。

以下のような URL が発行されるので、Web ブラウザや `curl` コマンドでアクセスして、"Hello from Lambda!" と表示されればデプロイ成功です。

{{< code title="API エンドポイント" >}}
https://abcde12345.execute-api.ap-northeast-1.amazonaws.com/prod/
{{< /code >}}

URL の先頭の 10 文字は、作成した API の ID に読み替えてください。


API キーによるアクセス制限をかける
----

API Gateway で API キーによるアクセス制限をかけるには、次のような構成を作成する必要があります。
はっきりいって分かりにくいです（^^；

- 各 API リソースの設定で **API キーの必須設定** を ON にする
- API Gateway 上で **使用量プラン** を作成し、**API ステージ**（`hello-api` の `prod` など）を関連づける
- API Gateway 上で **API キー** を作成し、「使用量プラン」に関連づける

図にすると下記のような感じ。
カッコの中は、今回設定する値を示しています。

{{< mermaid title="API キーと API ステージの関係" >}}
graph LR
    A["API キー<br>(HELLO_API_KEY)"] --- B["使用量プラン<br>(HELLO_API_PLAN)"]
    B-->C["API ステージ<br>(hello-api/prod)"]
{{< /mermaid >}}

まず、次のように設定して、API 呼び出し時の API キー（`x-api-key` ヘッダー）の指定を必須にします。

1. API Gateway の **`API`** メニューから、作成した `hello-api` を選択します。
1. 各リソースのメソッド（`GET` など）を選択し、メソッドリクエストの設定の **`編集`** をクリックします。
1. **`API キー必須`** を ON にして **`保存`** します。
1. **`API をデプロイ`** ボタンを押して、再度 API をデプロイします。

これで、単純な HTTP リクエストが次のように弾かれるようになります。

{{< code lang="console" title="API キーが指定されていないのでエラー" >}}
$ curl https://abcde12345.execute-api.ap-northeast-1.amazonaws.com/prod/
{"message":"Forbidden"}
{{< /code >}}

次に、API キーを作成して、使用量プラン（API ステージ）と関連づけます。

1. API Gateway の **`使用量プラン`** メニューで、**`使用量プランを作成`** をクリックし、次のように作成します。
   - 名前: `HELLO_API_PLAN`
   - スロットリング: OFF にするか適当に設定
   - クォータ: OFF にするか適当に設定
1. 使用量プラン `HELLO_API_PLAN` のページで、**`API ステージを追加`** をクリックし、次のように設定します。
   - API: `hello-api` を選択
   - ステージ: `prod` を選択
1. API Gateway の **`API キー`** メニューで、**`API キーの作成`** をクリックし、次のように作成します。
   - 名前: `HELLO_API_KEY`
   - APIキー: `自動生成`
1. API キー `HELLO_API_KEY` のページで、**`使用量プランに追加`** をクリックし、次のように設定します。
   - 使用量プラン: `HELLO_API_PLAN`

これで、API キーを使って API を呼び出せるようになります。

{{< code lang="console" title="API キーを指定した場合" >}}
$ API_KEY=jQjvIkSylW.......(省略).......Py6WFBxJMa
$ curl https://abcde12345.execute-api.ap-northeast-1.amazonaws.com/prod/ -H "X-API-KEY:$API_KEY"
"Hello from Lambda!"
{{< /code >}}

できた！ ٩(๑❛ᴗ❛๑)۶ わーぃ

