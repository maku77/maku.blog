---
title: "AWS CDK で API Gateway の REST API を作成する"
url: "/p/k7eoer5"
date: "2022-04-18"
lastmod: "2022-06-13"
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

{{< code lang="json" title="/books のレスポンス" >}}
[{"id":"1","title":"Title-1"},{"id":"2","title":"Title-2"},{"id":"3","title":"Title-3"}]
{{< /code >}}

単一の書籍情報を取得する場合は、`/books/1` のように、書籍 ID まで指定してアクセスします。

{{< code lang="json" title="/books/1 のレスポンス" >}}
{"id":"1","title":"Title-1"}
{{< /code >}}


（応用）プロキシリソースで複数階層の URL をまとめてハンドルする
----

{{% private %}}
- [プロキシリソースとのプロキシ統合を設定する - Amazon API Gateway](https://docs.aws.amazon.com/ja_jp/apigateway/latest/developerguide/api-gateway-set-up-simple-proxy.html)
{{% /private %}}

### プロキシリソースとは

前述の例では、ひとつの書籍情報を返す REST API のパスとして`/books/{id}` を設定し、パスパラメーター `{id}` で書籍 ID を受け取るようにしていました。
API Gateway の __プロキシリソース__ の仕組みを利用すると、複数階層の URL（例: `/aaa/bbb/ccc`）を 1 つのパスパラメーター（__`{proxy+}`__）で受け取ることができます。
__プロキシリソースは、前述の Lambda プロキシ統合とは別物__ なので注意してください。
Lambda プロキシ統合の方は、単に REST API 呼び出しを Lambda 関数の呼び出しに変換する機能です。

プロキシリソースを使うと、1 つのハンドラー関数実装で様々な URL での API 呼び出しを処理することができます。
例えば、`/aaa` 、`/aaa/bbb/ccc` 、`/foo/bar` といった URL でのアクセスを、1 つのハンドラー関数でまとめて処理できます。
さらに、ハンドルする HTTP メソッドとして __`ANY`__ を指定することで、複数の HTTP メソッド（`GET` や `POST` など）もまとめて 1 つの関数で処理できます。

プロキシリソースの `{proxy+}` の部分のことを __greedy パスパラメーター (greedy path parameter)__ とか、__greedy パス変数__ と呼びます。
greedy とは「貪欲な」とか「食いしん坊な」という意味で、いろんな URL を飲み込んじゃうことを示しています。
個人的には「食いしん坊パス」と呼びたいところです。

### CDK でプロキシリソースを定義する

先に CloudFormation のスタックを作る CDK コードを見ていきます。
ここでは、API Gateway の REST API を作成し、プロキシリソース (`/greedy/{proxy+}`) を追加し、そこにハンドラー関数 (`greedyHandler`) を Lambda プロキシ統合します（ハンドラーは後ほど実装）。

{{< code lang="ts" title="lib/myapi-sample-stack.ts（抜粋）" >}}
// CDK V2 系のインポート
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

    // API Gateway (REST API) を作成
    const api = new apigateway.RestApi(this, "SampleApi")

    // プロキシリソース (/greedy/{proxy+}) を扱うハンドラー関数を作成
    const greedyHandler = new lambda.NodejsFunction(this, 'GreedyHandler', {
      entry: 'lambda/api.ts',
      handler: 'greedyHandler',
    })

    // `/greedy/{proxy+}` というプロキシリソースを Lambda プロキシ統合して追加
    api.root.addResource('greedy').addProxy({
      anyMethod: true, // すべての HTTP メソッド (ANY) をハンドル
      defaultIntegration: new apigateway.LambdaIntegration(greedyHandler),
    })
  }
}
{{< /code >}}

プロキシリソースを定義している部分は最後の `api.root.addResource('greedy').addProxy(...)` という部分ですが、greedy パス名としては __`{proxy+}`__ が自動的に割り当てられるようです。

ちなみに、API に Cognito ユーザープール認可などの Authorizer を設定するときは、OPTIONS メソッドに Authorizer が設定されないようにしなければいけません。
上記コードの `addProxy` 部分に単純に Authorizer を設定してしまうと、OPTIONS メソッドにまで Authorizer が設定されてしまい、HTTP のプリフライトリクエスト（OPTIONS リクエスト）が弾かれてしまいます。
簡単に言うと、API Gateway で定義した API が Web ブラウザ上の JavaScript から呼び出せなくなります。

{{< code lang="ts" title="ダメな Authorizer 設定方法" >}}
api.root.addResource('greedy').addProxy({
  anyMethod: true, // OPTIONS メソッドまで Authorizer が設定されてしまう
  defaultIntegration: new apigateway.LambdaIntegration(greedyHandler),
  defaultMethodOptions: { authorizer },
})
{{< /code >}}

なので、Authorizer は何とかして OPTIONS メソッド以外に設定してやる必要があります。

{{< code lang="ts" title="例えばこうすれば動く" >}}
const greedyResource = api.root.addResource('greedy').addProxy({
  anyMethod: false, // ここでは HTTP メソッドを追加しない
  defaultIntegration: new apigateway.LambdaIntegration(greedyHandler),
})

// OPTIONS 以外の各メソッドに個別に Authorizer を設定する
greedyResource.addMethod('GET', undefined, { authorizer })
greedyResource.addMethod('POST', undefined, { authorizer })
greedyResource.addMethod('PUT', undefined, { authorizer })
greedyResource.addMethod('DELETE', undefined, { authorizer })
{{< /code >}}

API Gateway に Cognito 認証をかける方法の詳細は [こちらを参照](/p/vujw9jv/) してください。

### プロキシリソース用のハンドラー（Lambda 関数）を実装する

上記で定義したプロキシリソース URL (`/greedy/{proxy+}`) にアクセスしたときに呼び出される Lambda 関数（ハンドラー）を実装します。

{{< code lang="ts" title="lambda/api.ts" >}}
import { APIGatewayProxyHandler } from 'aws-lambda'

export const greedyHandler: APIGatewayProxyHandler = async (event) => {
  // レスポンス本文用の JSON テキストを作る
  const responseBody = JSON.stringify({
    message: 'I am greedyHandler!',
    httpMethod: event.httpMethod,
    path: event.path,
    pathParameters: event.pathParameters,
    queryStringParameters: event.queryStringParameters,
  })

  // APIGatewayProxyResult 型のレスポンスを返す
  return {
    statusCode: 200,
    body: responseBody,
    headers: {
      'Access-Control-Allow-Origin': '*',
    },
  }
}
{{< /code >}}

実際にどのような URL で REST API が呼び出されたかは、ハンドラー関数に渡されるイベントオブジェクト ([APIGatewayProxyEvent](https://github.com/DefinitelyTyped/DefinitelyTyped/blob/48a5faec8eb951a173129e8dfe285a00ca0a426f/types/aws-lambda/trigger/api-gateway-proxy.d.ts#L109)) を参照すると分かります。
例えば、`/greedy/aaa/bbb/ccc/?foo=100&bar` という URL でアクセスした場合、イベントオブジェクトの値は次のようになります。

| プロパティ | 値 |
| ---- | ---- |
| `event.httpMethod` | `GET` |
| `event.path` | `/greedy/aaa/bbb/ccc/` |
| __`event.pathParameters`__ | __`{ proxy: "aaa/bbb/ccc" }`__ |
| `event.queryStringParameters` | `{ bar: "", foo: "100" }` |

つまり、__`event.pathParameters.proxy`__ の値を見ることで、パスに応じた処理を行えるということですね。

