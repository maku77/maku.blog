---
title: "AWS CDK で API Gateway に Cognito 認証によるアクセス制御を追加する"
url: "/p/vujw9jv"
date: "2022-04-18"
tags: ["AWS", "AWS/API Gateway", "AWS/CDK", "AWS/Cognito"]
---

何をするか？
----

ここでは、API Gateway で提供している REST API にアクセス制御を追加するため、既存の Cognito ユーザープールによるオーソライザーを API Gateway に設定してみます。
これにより、Cognito のユーザープールで認証済みのユーザーのみが REST API を呼び出せるようになります。

後述の CDK コードでは、API Gateway と Lambda 関数、オーソライザーを生成していますが、Cognito ユーザープールは既存のものを参照しています（こういったユースケースは多いと思います）。

なお、CDK による API Gateway の作成方法（Lambda プロキシ統合）については下記の記事を参考にしてください。
ここでは、Cognito ユーザープールによるオーソライザーの作成方法にフォーカスします。

- [AWS CDK で API Gateway の REST API を作成する](/p/k7eoer5)


Lambda 関数を作成する
----

REST API のバックエンドである Lambda 関数は最低限の実装で用意します。
ユーザー認証後に、API Gateway 経由で正しくこのハンドラを呼び出せるかの確認用です。

{{< code lang="ts" title="lambda/info.ts" >}}
import { APIGatewayProxyHandler } from "aws-lambda"

/** GET /info */
export const handler: APIGatewayProxyHandler = async (event) => {
  console.log("headers: " + JSON.stringify(event.headers, undefined, 2))

  return {
    statusCode: 200,
    body: JSON.stringify({ message: "Hello, API Gateway!" }),
    headers: {
      "Access-Control-Allow-Origin": "*",
    },
  }
}
{{< /code >}}


Cognito ユーザープールによるオーソライザーの追加
----

下記は、`cdk init app` で作成した CDK アプリのスタック生成コードに手を入れたものです。

{{< code lang="ts" title="lib/myapi-sample-stack.ts" >}}
import {
  Stack,
  StackProps,
  aws_apigateway as apigateway,
  aws_cognito as cognito,
  aws_lambda_nodejs as lambda,
} from "aws-cdk-lib"
import { Construct } from "constructs"

// 既存の Cognito ユーザープール
const USER_POOL_ID = "ap-northeast-1_xxxxxxxxx"
// const USER_POOL_ARN =
//   "arn:aws:cognito-idp:ap-northeast-1:123456789012:userpool/ap-northeast-1_xxxxxxxxx"

export class MyapiSampleStack extends Stack {
  constructor(scope: Construct, id: string, props?: StackProps) {
    super(scope, id, props)

    // Lambda 関数（GET /info 用）
    const getInfoHandler = new lambda.NodejsFunction(this, "GetInfoHandler", {
      entry: "lambda/info.ts",
      handler: "handler",
    })

    // 既存のユーザープールを参照する（fromUserPoolId か fromUserPoolArn を使う）
    const userPool = cognito.UserPool.fromUserPoolId(
      this, "MyUserPool", USER_POOL_ID
    )

    // ユーザープールを使うオーソライザーを作成
    const authorizer = new apigateway.CognitoUserPoolsAuthorizer(
      this, "MyAuthorizor", { cognitoUserPools: [userPool] }
    )

    // API Gateway (REST API) を作成して Lambda プロキシ統合
    const api = new apigateway.RestApi(this, "MyApi", {
      // デフォルトで Cognito 認証を必須とする
      defaultMethodOptions: { authorizer },
      // プリフライトリクエスト時の CORS アクセスを許可
      defaultCorsPreflightOptions: {
        allowOrigins: apigateway.Cors.ALL_ORIGINS,
        allowMethods: apigateway.Cors.ALL_METHODS,
        allowHeaders: apigateway.Cors.DEFAULT_HEADERS,
        statusCode: 200,
      },
    })
    const books = api.root.addResource("info")
    books.addMethod("GET", new apigateway.LambdaIntegration(getInfoHandler))
  }
}
{{< /code >}}

ポイントは、__`UserPool.fromUserPoolId()`__ で既存の Cognito ユーザープールを参照し、それを使って __`CognitoUserPoolsAuthorizer`__ オブジェクトを生成する部分です。
作成したオーソライザーは、`RestApi` インスタンス生成時に `defaultMethodOptions` で設定することができます。
リソース（URL パス）ごとにアクセス制限をかけたいときは、各リソースの `addMethod` 時にオーソライザーを設定すれば OK です。
これで、REST API へのアクセス時に、Cognito 認証によるアクセス制限がかかるようになります。

`defaultCorsPreflightOptions` の設定はちょっと事情が複雑ですが、Web ブラウザ上の JavaScript から認証付き API の呼び出しを行う際に必要になります。
クライアントサイド JS から API を呼び出すときに HTTP リクエストヘッダーに `Authorization` 情報を付加することになるのですが、この場合は GET 要求の前にプリフライトリクエストという OPTIONS 要求を行うことが HTTP の仕様で決められています。
このプリフライトリクエストはブラウザが自動的に行ってくれるのですが、このリクエストにもクロスドメインでのアクセスを許可しておかないといけません。
この設定を行わないと、CORS エラーが発生して API 呼び出しが失敗します。


デプロイ
----

Lambda 関数とスタック定義の実装が済んだら、CDK でデプロイします。

```console
$ cdk deploy
```

最後の方に、次のようにエンドポイント URL が表示されるのでコピーしておきます。

```
Outputs:
MyapiSampleStack.BookApiEndpointXXXXXXXX =
    https://xxxxxxxxxx.execute-api.ap-northeast-1.amazonaws.com/prod/
```


REST API アクセスのテスト
----

`curl` コマンドで REST API の `/info` リソースを参照すると、うまくアクセス制限がかかっていることが分かります。
HTTP  レスポンスコードは [401 Unauthorized](https://developer.mozilla.org/ja/docs/Web/HTTP/Status/401) です。

```console
$ curl https://xxxxxxxxxx.execute-api.ap-northeast-1.amazonaws.com/prod/info
{"message":"Unauthorized"}
```

情報を取得するには、Cognito ユーザープールで認証して取得した ID トークンを、REST API リクエスト時の __`Authorization`__ ヘッダーで指定する必要があります。

```console
$ curl https://xxxxxxxxxx.execute-api.ap-northeast-1.amazonaws.com/prod/info -H "Authorization:XXXX...XXXX"
{"message":"Hello, API Gateway!"}
```

### ID トークンの取得方法

ID トークンは、実際に Web サイト上の UI からサインインしてしまうか、AWS CLI (`aws cognito-idp`) などで認証処理を行うことで取得することができます。

- 参考: [Amazon Cognito でサインイン可能な Web サイトを作る (Amplify)](/p/pufs8kx)
- 参考: [Amazon Cognito をコマンドライン (CLI) から操作する](/p/yzjj4c7)

下記は、コマンドラインで認証処理を行って ID トークンを取得する例です。
実際には、この前にチャレンジレスポンスに返答する必要があったりしてとても面倒ですが、まぁがんばれば取得できます。

```console
$ aws cognito-idp admin-initiate-auth
    --user-pool-id ap-northeast-1_XXXXXXXXX
    --client-id XXXXXXXXXXXXXXXXXXXXXXXXX
    --auth-flow ADMIN_USER_PASSWORD_AUTH
    --auth-parameters USERNAME=username,PASSWORD=password

AuthenticationResult:
  AccessToken: eyJraWQiOiJ2aldabmd1meejRSauF5Z43Ez_9LWAxfBP...（省略）
  ExpiresIn: 3600
  IdToken: eyJraWQiOiJvVU9NU1QyZHhvQVArcnsmwD0WbYqhZppSDVNg...（省略）
  RefreshToken: eyJjdHkiOiJKV1QiLC3YjEmAndRqakoOhw4O9al0z1V...（省略）
  TokenType: Bearer
ChallengeParameters: {}
```

上記レスポンスにある、__`IdToken`__ の値を、`Authorization` ヘッダーでそのまま送れば、REST API は正しいレスポンスを返してくれます。

```console
$ curl https://xxxxxxxxxx.execute-api.ap-northeast-1.amazonaws.com/prod/info -H "Authorization:eyJraWQiOiJv..."
{"message":"Hello, API Gateway!"}
```

送った ID トークンが妥当なものかは、API Gateway が内部で自動で調べてくれるので、Lambda 関数でチェックしたりする必要はありません。


（応用）Web アプリからの REST API 呼び出し
----

Web アプリからオーソライザー設定された API Gateway を呼び出すには、HTTP リクエスト（`fetch` 関数呼び出し）時に、`Authorization` ヘッダーで Cognito ユーザープールから取得した ID トークンを付加する必要があります。
Web アプリからの Cognito 認証に Amplify ライブラリを使用している場合は、次のような感じで簡単に認証情報付き HTTP リクエストを発行できます。

```ts
import { Auth } from 'aws-amplify'

export const fetchWithAuth = async (url: string) => {
  const token = (await Auth.currentSession()).getIdToken().getJwtToken()
  return fetch(url, { headers: { Authorization: token } }).then((r) => r.json())
}
```

あとは、このフェッチ関数を通常の `fetch` 関数の代わりに React カスタムフックなどから呼び出すだけで OK です。
下記は、今回作成した REST API を呼び出す `useInfo` カスタムフックの実装例です。

{{< code lang="ts" title="<Next.jsアプリ>/src/hooks/useInfo.ts（抜粋）" >}}
/** useInfo フックの戻り値の型 */
type UseInfoOutput = {
  error?: Error
  message?: string
}

/** REST API の戻り値の型 */
type DataType = {
  message: string
}

export const useInfo = (): UseInfoOutput => {
  const { data, error } = useSWR<DataType, Error>(GET_INFO_URL, fetchWithAuth)
  return { error, message: data?.message }
}
{{< /code >}}

- 参考: [Amazon Cognito (1) サインイン可能な Web サイトを作る (Cognito User Pool)](/p/pufs8kx/)
