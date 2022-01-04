---
title: "AWS CloudFormation の設定例: Lambda 関数から S3 にアクセスできるようにする"
linkTitle: "CloudFormation の設定例: Lambda 関数から S3 にアクセスできるようにする"
url: "/p/dzdq3ep"
date: "2021-04-12"
tags: ["AWS", "AWS/Lambda", "AWS/CloudFormation", "AWS/SAM"]
weight: 203
---

CloudFormation (SAM) を利用して、Lambda 関数および S3 バケットを作成し、Lambda 関数から S3 バケットにアクセスできるようにポリシー設定するテンプレートの例です。
参考までに Lambda 関数の TypeScript コードも載せましたが、あくまで SAM テンプレートの記述例を示すことを目的としています。


SAM テンプレートの記述例
----

次の SAM テンプレートでは、AWS リソースとして S3 バケット (`MyBucket`) と Lambda 関数 (`MyFunction`) を定義しています。

{{< code lang="yaml" title="template.yml" >}}
AWSTemplateFormatVersion: '2010-09-09'
Transform: AWS::Serverless-2016-10-31

Resources:
  # S3 バケットの定義
  MyBucket:
    Type: AWS::S3::Bucket

  # Lambda 関数の定義
  MyFunction:
    Type: AWS::Serverless::Function
    Properties:
      Runtime: nodejs14.x
      Handler: build/index.handler
      CodeUri: function.zip
      Policies:
        - S3CrudPolicy:
            BucketName: !Ref MyBucket
      Environment:
        Variables:
          BUCKET_NAME: !Ref MyBucket
{{< /code >}}

S3 バケットは単純にスタック内に新規作成するだけなので、何もプロパティ設定していません。
ポイントは、Lambda 関数側のポリシー設定（`Policies` プロパティ）です。

{{< code lang="yaml" >}}
Policies:
  - S3CrudPolicy:
      BucketName: !Ref MyBucket
{{< /code >}}

ここでは、AWS が標準で用意している [S3CrudPolicy](https://docs.aws.amazon.com/ja_jp/serverless-application-model/latest/developerguide/serverless-policy-templates.html) というポリシーを使っています。
このポリシーの `BucketName` パラメータとして、作成した S3 バケットの物理 ID (Physical ID) を渡すことで、Lambda 関数から S3 バケットの読み書き（CRUD 操作）が可能になります。
S3 バケットの Physical ID は、`Ref` 関数に Logical ID を渡すことで取得することができます。

{{% note title="ポリシーは実行ロールに追加される" %}}
SAM テンプレートで Lambda 関数を定義すると、自動的にデフォルトの実行ロールが生成されてアタッチされます（`Role` プロパティを指定した場合を除く）。
このデフォルトロールには、CloudWatch Logs へのログの書き込み権限など、最低限の権限が付与されています。
`Policies` プロパティでポリシーを定義すると、このデフォルトロールに対して追加する形でポリシーが設定されます。
{{% /note %}}

SAM テンプレートで S3 バケットを作成する場合、そのバケット名 (Physical ID) を何らかの方法で Lambda 関数のコードに知らせてやる必要があります。
S3 バケット (`AWS::S3::Bucket`) の定義時に、`BucketName` プロパティで固定のバケット名を指定してやる方法もありますが、上記の例では、動的に生成された S3 バケット名を Lambda 関数に環境変数 (`BUCKET_NAME`) として渡しています。

{{< code lang="yaml" >}}
Environment:
  Variables:
    BUCKET_NAME: !Ref MyBucket
{{< /code >}}


Lambda 関数の実装コード
----

参考までに、S3 バケットにアクセスする Lambda 関数の実装コード (TypeScript) を載せておきます。
環境変数 `BUCKET_NAME` で渡されたバケットに対して、オブジェクトを追加しています。

{{< code lang="ts" title="src/index.ts" >}}
import { S3 } from 'aws-sdk';
const s3 = new S3();

export async function handler(): Promise<any> {
    if (!process.env.BUCKET_NAME) {
        throw new Error('Env variable BUCKET_NAME is not set');
    }
    const request: S3.PutObjectRequest = {
        Bucket: process.env.BUCKET_NAME,
        Key: 'Key1',
        Body: 'Body1',
    };
    return s3.putObject(request).promise();
}
{{< /code >}}

