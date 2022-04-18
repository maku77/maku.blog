---
title: "AWS CDK で API Gateway の REST API を作成する"
url: "/p/k7eoer5"
date: "2022-04-18"
tags: ["AWS", "AWS/API Gateway", "AWS/CDK"]
---

概要
----

### API Gateway は HTTPS で Lambda 関数を呼び出す

API Gateway で REST API のエンドポイントを定義すると、HTTPS リクエストで Lambda 関数を呼び出せるようになります。

{{< shout "クライアント ─(HTTPS)─> API Gateway ─(AWS API)─> Lambda 関数" >}}

API Gateway には Cognito のユーザープールと連携する機能を備えており、認証済みのユーザーにのみ API 呼び出しを許可するといったことが可能です（具体的に言うと、API Gateway が見えないところで ID トークンの正当性を確認してくれたりします）。


{{% note title="新しい Lambda 関数 URL" %}}
2022 年 4 月に公開された [Lambda 関数 URL](https://aws.amazon.com/jp/about-aws/whats-new/2022/04/aws-lambda-function-urls-built-in-https-endpoints/) の仕組みを使うと、直接 Lambda 関数にエンドポイント URL を割り当てて呼び出すことができます。
Cognito 連携などを使わないシンプルな Web API であれば、Lambda 関数 URL の仕組みで作ってしまうのが手っ取り早いかもしれません。
{{% /note %}}

### Lambda プロキシ統合

Lambda 関数は様々な AWS サービスからのイベント通知によって起動する仕組みになっており、API Gateway からの HTTPS リクエストもそのうちのひとつです。
Lambda 関数のハンドラが呼び出されるとき、そのパラメーターとして「イベントオブジェクト」を受け取ることができるのですが、このイベントオブジェクトの中身は発生源によって異なります。

```js
exports.handler = async function(event) {
  // event の中身は呼び出し元によってバラバラ
}
```

つまり、Lambda 関数は呼び出し側が想定しているリクエストとレスポンスの型を意識した実装をしなければいけません。
とはいえ、REST API の呼び出し方は大体決まっており、Lambda 関数側が欲しい情報も、URL パスやクエリ文字列、HTTP メソッドなど決まっています。
そこで、API Gateway はこういった REST API の実装に都合のよい情報をイベントオブジェクトに詰めてくれる「__Lambda プロキシ統合__」という仕組みを提供しています。
Lambda プロキシ統合を有効にすると、ハンドラ関数に渡されたイベントオブジェクトから、クエリ文字列などを簡単に取り出すことができます。
レスポンスも決まった型で返さないといけないことに注意してください（間違った形式で返すと、クライアントに 502 エラーが通知されます）。

{{< code lang="ts" title="Lambda プロキシ統合を有効にしたハンドラ関数のシグネチャ (TypeScript)" >}}
export const handler: APIGatewayProxyHandler = async (event) => {
  // event.pathParameters でアクセス時の URL パス情報を参照可能
  // event.queryStringParameters でクエリ文字列を参照可能

  return {
    statusCode: 200,
    body: JSON.stringify(data),
    headers: {
      "Access-Control-Allow-Origin": "*",
    },
    // isBase64Encoded: true,  // body が Base64 エンコードされている場合
  }
}
{{< /code >}}

以下の説明では、この Lambda プロキシ統合を有効にして API Gateway (REST API) の実装を行っています。


CDK プロジェクトを作成する
----

[CDK プロジェクトの作成](/p/nujyfsy) は次のような感じで完了しているものとします。

```console
$ mkdir myapi-sample
$ cd myapi-sample
$ cdk init app --language typescript
```

ここでは、Lambda 関数の実装にも TypeScript を使います。


Lambda 関数を実装する
----

API Gateway (REST API) 経由で呼び出される Lambda 関数を実装します。
最初に Lambda 関数用の TypeScript 型情報をインストールしておきます。

```console
$ npm install --save-dev @types/aws-lambda  # npm の場合
$ yarn add --dev @types/aws-lambda          # yarn の場合
```

Lambda 関数の実装ファイル (`.ts`) は、プロジェクトルートに `lambda` ディレクトリを作ってそこに入れることにします。
ここでは、REST API として書籍情報を返す `/books` と `/books/{id}` というエンドポイントを作成します。
それぞれのハンドラとして `getBooksHandler` 関数と `getBookHandler` 関数を定義します。

{{< code lang="ts" title="lambda/books.ts" >}}
import { APIGatewayProxyHandler, APIGatewayProxyResult } from "aws-lambda"

const BOOKS = [
  { id: "1", title: "Title-1" },
  { id: "2", title: "Title-2" },
  { id: "3", title: "Title-3" },
]

/** GET /books */
export const getBooksHandler: APIGatewayProxyHandler = async (event) => {
  console.log(
    "pathParameters = " + JSON.stringify(event.pathParameters, undefined, 2)
  )
  return createResponse(BOOKS)
}

/** GET /books/{id} */
export const getBookHandler: APIGatewayProxyHandler = async (event) => {
  console.log(
    "pathParameters = " + JSON.stringify(event.pathParameters, undefined, 2)
  )
  const id = event.pathParameters?.["id"]
  return createResponse(BOOKS.find((b) => b.id === id))
}

/** レスポンスデータを生成する */
function createResponse(body: any): APIGatewayProxyResult {
  return {
    statusCode: 200,
    body: JSON.stringify(body),
    headers: {
      "Access-Control-Allow-Origin": "*",
    },
  }
}
{{< /code >}}

Lambda プロキシ統合を前提とする場合、Lambda 関数の型は、AWS SDK によってあらかじめ定義された型（`APIGatewayProxyHandler` や `APIGatewayProxyResult`）を使う必要があります。
また、Web ブラウザ上の JavaScript からの CORS アクセスを許可するために、レスポンスオブジェクトの `headers` プロパティで、__`Access-Control-Allow-Origin`__ を指定しています。

- 参考: [Enabling CORS for a REST API resource - Amazon API Gateway](https://docs.aws.amazon.com/apigateway/latest/developerguide/how-to-cors.html)


スタックを定義する
----

`cdk init app` による CDK プロジェクトの初期化時に、`lib/myapi-sample-stack.ts` といった感じの CloudFormation スタック定義用のコードが生成されているので、これを修正して、__Lambda 関数と API Gateway の定義__ を行い、両者を Lambda プロキシ統合します。
TypeScript で実装した Lambda 関数を CDK で扱う方法の詳細は下記を参考にしてください。

- 参考: [AWS CDK で TypeScript で実装した Lambda 関数をデプロイする (NodejsFunction)](/p/cj9i4m3)

{{< code lang="ts" title="lib/myapi-sample-stack.ts" >}}
// CDK V2 系のインポート（追加インストールの必要なし）
import {
  Stack,
  StackProps,
  aws_apigateway as apigateway,
  aws_lambda_nodejs as lambda,
} from "aws-cdk-lib"
import { Construct } from "constructs"

export class MyapiSampleStack extends Stack {
  constructor(scope: Construct, id: string, props?: StackProps) {
    super(scope, id, props)

    // Lambda 関数（GET books/ 用）
    const getBooksHandler = new lambda.NodejsFunction(this, "getBooksHandler", {
      entry: "lambda/books.ts",
      handler: "getBooksHandler",
    })

    // Lambda 関数（GET books/{id} 用）
    const getBookHandler = new lambda.NodejsFunction(this, "getBookHandler", {
      entry: "lambda/books.ts",
      handler: "getBookHandler",
    })

    // API Gateway (REST API) の作成
    const api = new apigateway.RestApi(this, "BookApi")

    // リソースを定義して Lambda プロキシ統合 (GET books/)
    const books = api.root.addResource("books")
    books.addMethod("GET", new apigateway.LambdaIntegration(getBooksHandler))

    // リソースを定義して Lambda プロキシ統合 (GET book/)
    const singleBook = books.addResource("{id}")
    singleBook.addMethod("GET", new apigateway.LambdaIntegration(getBookHandler))
  }
}
{{< /code >}}

コードの末尾あたりの、__`apigateway.LambdaIntegration()`__ の部分が、API Gateway の Lambda プロキシ統合を行っている部分になります。
指定したリソース（URL パス）と HTTP メソッドを、特定のハンドラ関数に結びつけています。
こうすることで、その URL (REST API) にアクセスしたときに、扱いやすいイベントオブジェクトをハンドラ関数に渡してくれるようになります。


デプロイ
----

Lambda 関数の実装とスタックの定義が完了したら、CDK でデプロイを行います。
デプロイが完了すると、標準出力に API のエンドポイント (URL) を表示してくれます。

```console
$ cdk deploy

...

Outputs:
MyapiSampleStack.BookApiEndpointCA1C03A3 =
    https://a4chxhlkod.execute-api.ap-northeast-1.amazonaws.com/prod/
 ```

上記 URL の末尾に `/books` を付けて Web ブラウザからアクセスすれば REST API を呼び出せます。
次のような書籍一覧情報の JSON データが返ってくれば成功です。

```json
[{"id":"1","title":"Title-1"},{"id":"2","title":"Title-2"},{"id":"3","title":"Title-3"}]
```

単一の書籍情報を取得する場合は、`/books/1` のように、書籍 ID まで指定してアクセスします。

```json
{"id":"1","title":"Title-1"}
```

