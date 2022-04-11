---
title: "Lambda 実装例: S3 へのアップロードを SNS で通知して Lambda から読み込む"
url: "/p/f2fq2cn"
date: "2021-04-19"
tags: ["AWS"]
---

何をするか？
----

{{< image src="img-s3-sns-lambda.svg" >}}

ここでは Lambda 関数の実装例として、SNS トピックから S3 バケットの PutObject イベント通知を受けて、アップロードされたファイルを読み込む例を示します。
S3 バケット、および SNS トピックの作成と、S3 → SNS の通知設定は完了していると想定します。

{{% reference %}}
- [CloudFormation の設定例: S3 通知を SNS トピックに Publish する](/p/3o2dpyb)
- [CloudFormation の設定例: SNS トピックを Lambda 関数からサブスクライブする](/p/5q4epyb)
{{% /reference %}}



Lambda 関数の実装
----

### AWS SDK のインストール

ここでは、Node.js 用の AWS SDK ver.2 を使っているので、先にインストールしておく必要があります。
AWS 側の Lambda 実行環境には標準でインストールされているので、`--save-dev`（開発用）でインストールしておけば OK です。
ついでに TypeScript 用の型定義もインストールしておくと、Lambda ハンドラのパラメータを `any` 型ではなく、`SNSEvent` 型などで参照できて便利です。

{{< code >}}
$ npm install aws-sdk --save-dev
$ npm install @types/aws-lambda --save-dev
{{< /code >}}

### Lambda ハンドラの実装

先に、S3 バケット内のオブジェクトの内容を取得するユーティリティ関数を用意しておきます（AWS SDK ver.2 を使用しています）。
エクスポートされた `getS3Object` 関数を使う想定ですが、このコードを直接実行したときには末尾のテスト関数を実行するようにしてあります。

{{< code lang="ts" title="src/s3util.ts" >}}
import { S3 } from 'aws-sdk';
const s3 = new S3();

/** S3 バケット内のオブジェクト（テキスト）を読み込んでその内容を返します。 */
export async function getS3Object(
    bucketName: string,
    objectKey: string
): Promise<string | undefined> {
    const output: S3.GetObjectOutput = await s3.getObject({
        Bucket: bucketName,
        Key: objectKey,
    }).promise();
    return output.Body?.toString();
}

// テスト用 main 関数
if (require.main === module) (async () => {
    try {
        const bucket = 'bucket-123456789012-sample';
        const key = 'sample.txt';
        const body = await getS3Object(bucket, key);
        console.log(body);
    } catch (e) {
        console.err(e);
    }
})();
{{< /code >}}

次の Lambda 関数実装では、第一引数で受け取った `SNSEvent` オブジェクトから S3 バケット名とオブジェクト名を取り出して、そのオブジェクトの内容を S3 API（上記で定義したユーティリティ関数）を使って取得しています。

{{< code lang="ts" title="src/index.ts" >}}
import { SNSEvent, Context, Callback } from 'aws-lambda';
import * as s3util from './s3util';

/** SNS トピックの通知から抽出する S3 メタ情報 */
type BucketMeta = {
    /** S3 のイベント名 (例: `ObjectCreated:Put`) */
    eventName: string,
    /** S3 バケット名 */
    bucketName: string,
    /** S3 バケット内のオブジェクトキー */
    objectKey: string,
};

/** Lambda 関数に渡されるイベントデータから S3 のメタ情報を取得します。 */
function parseSnsEvent(snsEvent: SNSEvent): BucketMeta {
    const msg = JSON.parse(snsEvent.Records[0].Sns.Message) as any;
    return {
        eventName: msg.Records[0].eventName,
        bucketName: msg.Records[0].s3.bucket.name,
        objectKey: msg.Records[0].s3.object.key,
    } as BucketMeta;
}

/** Lambda 関数のエントリポイントです。 */
export async function handler(event: SNSEvent, ctx: Context, callback: Callback) {
    try {
        const meta: BucketMeta = parseSnsEvent(event);
        const body = await s3util.getS3Object(meta.bucketName, meta.objectKey)
        console.log(meta);
        console.log(body);
        callback(null, body);
    } catch (e) {
        console.error(e);
        callback(e);
    }
}
{{< /code >}}

### 解説

Lambda 関数の第一引数 (`event`) で渡されるオブジェクト (このケースでは `SNSEvent` 型) の `Records[0].Sns.Message` プロパティを参照すると、次のような JSON テキストを取得できます（オブジェクトではなく JSON 形式のテキストです）。

{{< accordion title="SNSEvent.Records[0].Sns.Message の内容" >}}
{{< code lang="json" >}}
{
  "Records": [
    {
      "eventVersion": "2.1",
      "eventSource": "aws:s3"
      "awsRegion": "ap-northeast-1",
      "eventTime": "2021-04-19T15:30:59.084Z",
      "eventName": "ObjectCreated:Put",
      "userIdentity": {
        "principalId": "AWS:AIDMAQAKLZ6V3HXINMCQG"
      },
      "requestParameters": {
        "sourceIPAddress": "110.76.125.97"
      },
      "responseElements": {
        "x-amz-request-id": "PR9N0NY46J4B4YM9",
        "x-amz-id-2": "IyRPxwo5CHEv4eB8Vd2JjL6kkxdJ0fm0S3qfkB1oKh3gBudpIomjSBvN77749yBgqxKtws7l+FbUhjgMATBf/1VnblothZ6v7DXmjCj5s8Y="
      },
      "s3": {
        "s3SchemaVersion": "1.0",
        "configurationId": "497e6001-fbc4-40df-939e-c09289477696",
        "bucket": {
          "name": "bucket-123456789012-sample",
          "ownerIdentity": {
            "principalId": "A2H51MMFJF0VI7"
          },
          "arn": "arn:aws:s3:::bucket-123456789012-sample"
        },
        "object": {
          "key": "sample.txt",
          "size": 5,
          "eTag": "dcd0cda10b5681c4a9e9ff6fe9e95990",
          "sequencer": "5500C45300607DA237"
        }
      }
    }
  ]
}
{{< /code >}}
{{< /accordion >}}

この JSON テキストをオブジェクト化すれば、次のような情報を参照できます。

- `Records[0].eventName` → S3 のイベント名 (`ObjectCreated:Put`)
- `Records[0].s3.bucket.name` → S3 バケット名 (`bucket-123456789012-sample`)
- `Records[0].s3.object.key` → S3 バケットに格納されたオブジェクト名 (`sample.txt`)

あとは、この情報を使って、S3 バケットに格納されたオブジェクト（ファイル）の内容を取得すれば OK です。
必要な情報は Lambda 関数の引数で受け取れるので、環境変数などで S3 バケットの情報を渡さずに済みます。


デプロイ用の情報
----

### CloudFormation テンプレート

ここでは CloudFormation (SAM) テンプレートを使って、Lambda 関数のリソースを生成します。
Lambda 関数リソースを生成するときに、S3 バケットへのアクセス権限と、SNS トピックへのサブスクリプション設定を行う必要があるので、これらの情報を入力パラメーター (`Parameters`) として定義しています。

- S3 バケット名: `bucket-123456789012-sample`
- SNS トピックの ARN: `arn:aws:sns:ap-northeast-1:123456789012:mytopic`

{{< code lang="yaml" title="template.yaml" >}}
AWSTemplateFormatVersion: 2010-09-09
Transform: AWS::Serverless-2016-10-31

Parameters:
  TopicArn:
    Type: String
    Default: arn:aws:sns:ap-northeast-1:123456789012:mytopic
  BucketName:
    Type: String
    Default: bucket-123456789012-sample

Resources:
  MyFunction:
    Type: AWS::Serverless::Function
    Properties:
      Runtime: nodejs14.x
      Handler: build/index.handler
      CodeUri: function.zip

      # S3 にアクセスするための権限（ポリシー）をロールに追加
      Policies:
        - S3CrudPolicy:
            BucketName: !Ref BucketName

      # この Lambda 関数を SNS トピックにサブスクライブする
      Events:
        MySnsEvent:
          Type: SNS
          Properties:
            Topic: !Ref TopicArn
{{< /code >}}

このテンプレートを使って実際に CloudFormation スタックを生成する方法をここで説明すると長くなってしまうので、そのあたりは下記の記事を参照してください。

{{% reference %}}
- [AWS CloudFormation で Lambda 関数のリソースを生成する](/p/w8s8kx9)
{{% /reference %}}

### package.json

参考までに `package.json` の一例を載せておきます。

{{< code lang="json" >}}
{
  "name": "function",
  "version": "0.0.1",
  "scripts": {
    "start": "node build/index.js",
    "build": "tsc",
    "build:watch": "tsc --watch",
    "zip": "npm-pack-zip",
    "cf:package": "aws cloudformation package --template-file template.yml --output-template-file template.packaged.yml --s3-bucket bucket-123456789012-functions",
    "cf:deploy": "aws cloudformation deploy --stack-name mystack --template-file template.packaged.yml --capabilities CAPABILITY_IAM"
  },
  "files": [
    "build"
  ],
  "bundledDependencies": [],
  "devDependencies": {
    "@types/aws-lambda": "^8.10.75",
    "@types/node": "^14.14.35",
    "npm-pack-zip": "^1.2.9",
    "typescript": "^4.2.3"
  },
  "dependencies": {
    "aws-sdk": "^2.884.0"
  }
}
{{< /code >}}

{{< code title="使用例" >}}
$ npm run install     # 依存パッケージのインストール
$ npm run build       # ビルド（トランスパイル）
$ npm run zip         # ビルド結果を ZIP 化
$ npm run cf:package  # ZIP をアップロード
$ npm run cf:deploy   # デプロイ（CloudFormation スタックを生成）
{{< /code >}}

TypeScript で実装した Lambda 関数を ZIP 化する方法に関しては、以下の記事を参考にしてください。

{{< reference >}}
- [AWS Lambda にデプロイするための ZIP パッケージを npm で作成する (npm-pack-zip)](/p/zmydq3f)
{{< /reference >}}

