---
title: "Amazon API Gateway をコマンドライン (CLI) から操作する"
url: "/p/qwb6v89"
date: "2022-04-11"
tags: ["AWS", "AWS/CLI", "AWS/API Gateway"]
---

{{% private %}}
- 下記はどちらも CLI V2 のコマンド（なぜか `apigateway` と `apigatewayv2` の 2 種類がある）
  - https://awscli.amazonaws.com/v2/documentation/api/latest/reference/apigateway/index.html
  - https://awscli.amazonaws.com/v2/documentation/api/latest/reference/apigatewayv2/index.html
- 下記ドキュメントは、CLI での REST API 構築の参考になる
  - https://docs.aws.amazon.com/ja_jp/apigateway/latest/developerguide/set-up-lambda-proxy-integrations.html
{{% /private %}}

AWS CLI（コマンドラインkツール）で Amazon API Gateway を操作するには、__`aws apigatewayv2`__ コマンドを使用します。
API Gateway の作成や更新を行うためには、IAM ユーザーに適切な権限が必要ですが、__`AmazonAPIGatewayAdministrator`__ 管理ポリシーを付けるとほとんどの操作が可能になります。
通常、REST API を作成する場合は、バックエンドの Lambda 関数も合わせて必要になるので、__`AWSLambdaFullAccess`__ などの管理ポリシーも必要になります。


REST API を作成する (apigateway create-rest-api)
----

リファレンス: [apigateway create-rest-api](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/apigateway/create-rest-api.html)

```console
$ aws apigateway create-rest-api --name "My First API" \
    --description "This is my first API" \
    --region ap-northeast-1

apiKeySource: HEADER
createdDate: '2022-04-18T15:46:00+09:00'
description: This is my first API
disableExecuteApiEndpoint: false
endpointConfiguration:
  types:
  - EDGE
id: mk6mj65po6
name: My First API
```

`--name` オプションのみが必須です。
API の作成に成功すると、上記のように ID (`mk6mj65po6`) が返されます。



REST API を削除する (apigateway delete-rest-api)
----

リファレンス: [apigateway delete-rest-api](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/apigateway/delete-rest-api.html)

```console
$ aws apigateway delete-rest-api --rest-api-id mk6mj65po6
```

REST API の名前ではなく、ID を指定することに注意してください。
ID が分からない場合は、`get-rest-apis` で列挙して調べることができます。


REST API の一覧を取得する (apigateway get-rest-apis)
----

リファレンス: [apigateway get-rest-apis](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/apigateway/get-rest-apis.html)

```console
$ aws apigateway get-rest-apis
```

