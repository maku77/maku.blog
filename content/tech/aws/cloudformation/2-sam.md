---
title: "AWS SAM の基本"
linkTitle: "SAM の基本"
url: "/p/4oybmv6"
date: "2021-04-06"
tags: ["AWS", "AWS/SAM"]
draft: true
weight: 2
---

AWS SAM とは
----

AWS SAM (Serverless Application Model) は CloudFormation を拡張するものであり、サーバレス環境をより簡単に構築するための SAM テンプレート構文を提供します。
主に Lambda や DynamoDB などを使ったアプリケーションのインフラ（AWS リソース群）を構築するために使用します。
SAM テンプレートの構文は CloudFormation テンプレートを少しだけ拡張したものになっており、CloudFormation テンプレートと同様に YAML あるいは JSON 形式で記述します。


SAM テンプレートの構文
----


Globals セクション
----

{{< private >}}
- [Globals セクション](https://docs.aws.amazon.com/ja_jp/serverless-application-model/latest/developerguide/sam-specification-template-anatomy-globals.html)
{{< /private >}}

SAM テンプレート独自のセクションとして、各リソースの共通設定を行うための __`Globals`__ セクションがあります。
ここでは、すべての Lambda 関数に共通の設定などを行えます。
次の例では、すべての Lambda 関数用のランタイムとして Node.js 14.x (`nodejs14.x`) を使用するように指定しています。

{{< code lang="yaml" >}}
Globals:
  Function:
    Runtime: nodejs14.x
    Timeout: 30
    Handler: index.handler
    Environment:
      Variables:
        TABLE_NAME: default-table

Resources:
  MyFunction1:
    Type: AWS::Serverless::Function
    Properties:
      Runtime: python3.6

  MyFunction2:
    Type: AWS::Serverless::Function
    Properties:
      Runtime: nodejs12.x
      Environment:
        Variables:
          TABLE_NAME: my-table
{{< /code >}}

実際には上記の例のように、ランタイムなどの指定は、各リソースのプロパティでオーバーライドできます。



Lambda 関数のプロパティ設定いろいろ
----

### S3 にアクセスするポリシー設定

{{< code lang="yaml" >}}
Type: AWS::Serverless::Function
Properties:
  Handler: index.handler
  ...
  Policies:
    - AWSLambdaExecute
    - Version: '2012-10-17'
      Statement:
        - Effect: Allow
          Action:
            - s3:GetObject
            - s3:GetObjectACL
          Resource: 'arn:aws:s3:::my-bucket/*'
{{< /code >}}

