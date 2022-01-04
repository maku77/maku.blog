---
title: "Node.js で Amazon S3 を操作する (AWS SDK)"
url: "/p/m8kv8it"
date: "2021-03-21"
tags: ["AWS", "TypeScript"]
weight: 2
---

{{% private %}}
- [@aws-sdk/client-s3](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-s3/index.html)
{{% /private %}}

ここでは、Node.js 用の AWS SDK を使って Amazon S3 を操作する方法を説明します。
[TypeScript の基本的な環境構築](/p/taiqx6d) は終わっているものとします。


S3 用の Node.js SDK をインストールする
----

AWS SDK version 3 の S3 用パッケージをインストールするには次のようにします。

{{< code >}}
### npm の場合
$ npm install @aws-sdk/client-s3

### yarn の場合
$ yarn add @aws-sdk/client-s3
{{< /code >}}

これで、TypeScript コードから次のようにパッケージ内のクラスをインポートできるようになります。

{{< code lang="typescript" title="main.ts" >}}
import { S3Client, ListBucketsCommand } from '@aws-sdk/client-s3';
{{< /code >}}


S3Client インスタンスの生成
----

AWS SDK で S3 の操作を行うには、[S3Client](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-s3/classes/s3client.html) オブジェクトを使って各種コマンドを送ります。

{{< code lang="ts" >}}
new S3Client(configuration: S3ClientConfig): S3Client
{{< /code >}}

`S3Client` コンストラクタには [S3ClientConfig](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-s3/globals.html#s3clientconfig) オブジェクトを渡すようになっており、接続情報などを指定できます。
空オブジェクト (`{}`) を渡すと、現在の環境の default プロファイルの設定が使用されます（`~/.aws/config` と `~/.aws/credentials` に設定されたもの）。

{{< code lang="ts" >}}
// default プロファイルで接続する場合
const s3 = new S3Client({});

// 接続情報をカスタマイズする場合
const s3 = new S3Client({
  region: 'ap-northeast-1',
  credentials: {
    accessKeyId: 'QABCSCRHWJAKIAQXINMC',
    secretAccessKey: 'TTey3DVmbrILhTforDDa8kR6+SL+47htmoJ+Vimv',
  },
});
{{< /code >}}

一時的にプロファイルを切り替えて実行したいときは、環境変数 __`AWS_PROFILE`__ でプロファイル名を指定します。

{{< code title="例: プロファイルを admin に切り替えて実行" >}}
$ export AWS_PROFILE=admin
$ npm start --silent
{{< /code >}}

### 設定情報を確認する

次のようにすれば、`S3Client` のコンフィギュレーション情報として、どのような設定が使用されるかを確認できます。

{{< code lang="ts" title="main.ts" >}}
import { S3Client } from '@aws-sdk/client-s3';

const s3 = new S3Client({});
(async () => {
  console.log(await s3.config.credentials());
  console.log(await s3.config.endpoint());
})();
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code >}}
{
  accessKeyId: 'QAGPX77PNSAKIAQXINMC',
  secretAccessKey: 'FegIdbCW2JK7Jiv9yfZGGH3Q2KAmp2ufOANToPDX',
  sessionToken: undefined
}
{
  hostname: 's3.ap-northeast-1.amazonaws.com',
  port: undefined,
  protocol: 'https:',
  path: '/',
  query: undefined
}
{{< /code >}}
{{< /accordion >}}


S3 バケットを作成する (CreateBucketCommand)
----

新しい S3 バケットを作成するには、`S3Client#send()` メソッドで [CreateBucketCommand](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-s3/classes/createbucketcommand.html) コマンドを送ります。

{{< code lang="ts" title="create-bucket.ts" >}}
import { S3Client, CreateBucketCommand, } from '@aws-sdk/client-s3';
const s3 = new S3Client({});

// S3 バケットを作成する
async function createBucket() {
  try {
    const output = await s3.send(
      new CreateBucketCommand({
        Bucket: 's3-123456789012-sample-bucket-1'
      })
    );
    console.log('SUCCESS - Bucket created:', output);
  } catch (err) {
    console.error('ERROR:', err);
  }
}

createBucket();
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code lang="js" >}}
SUCCESS - Bucket created: {
  '$metadata': {
    httpStatusCode: 200,
    requestId: undefined,
    extendedRequestId: 'X8LD9DYz4+UB8MbbwdfuH2nifmfLC0qJt0LcGhpRAZUDOBAYtrk/qsVqc/WLtYQj7qbIPmIk3gj=',
    cfId: undefined,
    attempts: 1,
    totalRetryDelay: 0
  },
  Location: 'http://s3-123456789012-sample-bucket-1.s3.amazonaws.com/'
}
{{< /code >}}
{{< /accordion >}}


S3 バケットを削除する (DeleteBucketCommand)
----

S3 バケットを削除するには、`S3Client#send()` メソッドで [DeleteBucketCommand](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-s3/classes/deletebucketcommand.html) コマンドを送ります。
バケットを削除するには、先にバケット内のオブジェクトをすべて削除しておく必要があります。

{{< code lang="ts" title="delete-bucket.ts" >}}
import { S3Client, DeleteBucketCommand, } from '@aws-sdk/client-s3';
const s3 = new S3Client({});

// S3 バケットを削除する（先にすべてのオブジェクトを削除しておく必要あり）
async function deleteBucket() {
  try {
    const output = await s3.send(
      new DeleteBucketCommand({
        Bucket: 's3-123456789012-sample-bucket-1'
      })
    );
    console.log('SUCCESS - Bucket deleted:', output);
  } catch (err) {
    console.error('ERROR:', err);
  }
}

deleteBucket();
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code lang="js" >}}
SUCCESS - Bucket deleted: {
  '$metadata': {
    httpStatusCode: 204,
    requestId: undefined,
    extendedRequestId: 'Ha4qd+x3FHsbi8WsdRu9GTXz5LhLl3s/m/yavxT6Nc9LOgjKNCG2Plk19V86hdeTyfKws6jfFF/=',
    cfId: undefined,
    attempts: 1,
    totalRetryDelay: 0
  }
}
{{< /code >}}
{{< /accordion >}}


S3 バケットにオブジェクトを追加する (PutObjectCommand)
----

### SDK V3 の場合

既存の S3 バケットにオブジェクトを追加するには、`S3Client#send()` メソッドで [PutObjectCommand](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-s3/classes/putobjectcommand.html) コマンドを送ります。

{{< code lang="ts" title="put-object.ts" >}}
import { S3Client, PutObjectCommand } from '@aws-sdk/client-s3';
const s3 = new S3Client({});

// S3 バケットにオブジェクトを追加する
async function putObject() {
  try {
    const output = await s3.send(
      new PutObjectCommand({
        Bucket: 's3-123456789012-sample-bucket-1',
        Key: 'object-key-1',
        Body: 'object-body-1'
      })
    );
    console.log('SUCCESS - Object added:', output);
  } catch (err) {
    console.error('ERROR:', err);
  }
}

putObject();
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code lang="js" >}}
SUCCESS - Object added: {
  '$metadata': {
    httpStatusCode: 200,
    requestId: undefined,
    extendedRequestId: 'M0oachGF8zXfHPrFA97PO2LNz/9dkinNBeCUz4vhaQLeorIdGkmLFqMAkhdhuEb1s9iu4JIC947=',
    cfId: undefined,
    attempts: 1,
    totalRetryDelay: 0
  },
  BucketKeyEnabled: undefined,
  ETag: '"f0d3e2d98146efec4959d6bf45924b7e"',
  Expiration: undefined,
  RequestCharged: undefined,
  SSECustomerAlgorithm: undefined,
  SSECustomerKeyMD5: undefined,
  SSEKMSEncryptionContext: undefined,
  SSEKMSKeyId: undefined,
  ServerSideEncryption: undefined,
  VersionId: undefined
}
{{< /code >}}
{{< /accordion >}}

### SDK V2 の場合

{{< code lang="ts" title="put-object-v2.ts" >}}
import * as AWS from 'aws-sdk';
const s3 = new AWS.S3({ region: 'ap-northeast-1' });

async function putObjectV2() {
  try {
    const output = await s3.putObject({
      Bucket: 's3-123456789012-sample-bucket-1',
      Key: 'object-key-1',
      Body: 'object-body-1'
    }).promise();
    console.log('SUCCESS - Object added:', output);
  } catch (err) {
    console.error('ERROR:', err);
  }
}

putObjectV2();
{{< /code >}}

{{< code title="実行結果 (SDK V2)" >}}
SUCCESS - Object added: { ETag: '"90ad571f0ce74c38c7eca7a806a7ce68"' }
{{< /code >}}


S3 バケット内のオブジェクトの一覧を取得する (ListObjectV2Command)
----

S3 バケット内のオブジェクトのリスト（最大1000件）を取得するには、`S3Client#send()` メソッドで [ListObjectsV2Command](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-s3/classes/listobjectsv2command.html) コマンドを送ります。

{{< code lang="ts" title="list-objects.ts" >}}
import { S3Client, ListObjectsV2Command } from '@aws-sdk/client-s3';
const s3 = new S3Client({});

// S3 バケット内のオブジェクトの一覧を取得する
async function listObjects() {
  try {
    const output = await s3.send(
      new ListObjectsV2Command({
        Bucket: 's3-123456789012-sample-bucket-1',
        MaxKeys: 10,  // 最大10件まで取得
      })
    );
    for (const obj of output.Contents || []) {
      console.log(obj);
    }
  } catch (err) {
    console.error('ERROR:', err);
  }
}

listObjects();
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code lang="js" >}}
{
  Key: 'object-key-1',
  LastModified: 2021-03-21T07:51:13.000Z,
  ETag: '"59d6bf45924b7ef0d3e2d98146efec49"',
  Size: 13,
  StorageClass: 'STANDARD',
  Owner: undefined
}
{
  Key: 'object-key-2',
  LastModified: 2021-03-21T08:12:11.000Z,
  ETag: '"3f895677e156d30464cb71606a3f0f46"',
  Size: 13,
  StorageClass: 'STANDARD',
  Owner: undefined
}
{
  Key: 'object-key-3',
  LastModified: 2021-03-21T08:12:18.000Z,
  ETag: '"a4d8c2240f532047c257f1208dd710c2"',
  Size: 13,
  StorageClass: 'STANDARD',
  Owner: undefined
}
{{< /code >}}
{{< /accordion >}}


オブジェクトの内容を取得する (GetObjectCommand)
----

### SDK V3 の場合

特定のオブジェクトの内容を取得するには、`S3Client#send()` メソッドで [GetObjectCommand](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-s3/classes/getobjectcommand.html) コマンドを送ります。

戻り値の `GetObjectCommandOutput` の `Body` プロパティを参照すると、バケットオブジェクトの内容を取得できるのですが、これはオブジェクトの内容そのものではなく、[ReadableStream オブジェクト](https://developer.mozilla.org/en-US/docs/Web/API/ReadableStream) になっているため、次のような感じで少しずつデータを取り出して結合する必要があります。
ここでは、オブジェクトの内容は UTF-8 形式のテキストであると想定しています。

{{< code lang="ts" title="get-object.ts" >}}
import { S3Client, GetObjectCommand } from '@aws-sdk/client-s3'
import config from './config'

// ReadableStream からバイトデータを少しずつ読み出します。
async function* yieldUint8Chunks(stream: ReadableStream<Uint8Array>) {
  const reader = stream.getReader()
  try {
    while (true) {
      const { done, value } = await reader.read()
      if (done) return
      yield value
    }
  } finally {
    reader.releaseLock()
  }
}

// ReadableStream からバイトデータを読み出して utf-8 文字列に結合して返します。
async function streamToString(
  stream: ReadableStream<Uint8Array>
): Promise<string> {
  const buf: Buffer[] = []
  for await (const chunk of yieldUint8Chunks(stream)) {
    buf.push(Buffer.from(chunk as Uint8Array))
  }
  return Buffer.concat(buf).toString('utf-8')
}

/** 指定した S3 バケットのオブジェクトの内容を取得します。 */
export async function getS3Object(
  bucketName: string,
  objectKey: string
): Promise<string> {
  try {
    const s3 = new S3Client({
      region: config.region,
      credentials: config.credentials,
    })
    const output = await s3.send(
      new GetObjectCommand({
        Bucket: bucketName,
        Key: objectKey,
      })
    )
    const stream = output.Body as ReadableStream<Uint8Array>
    return streamToString(stream)
  } catch (err) {
    throw err
  }
}

// 使用例
const text = await getS3Object('s3-123456789012-sample-bucket-1', 'object-key-1')
{{< /code >}}

{{< code title="実行結果" >}}
object-body-1
{{< /code >}}

S3 のドキュメントでは別の方法として、SDK が提供する [getSignedUrl 関数を使って署名付き URL を作成] (https://docs.aws.amazon.com/ja_jp/sdk-for-javascript/v3/developer-guide/s3-example-creating-buckets.html#s3-create-presigendurl) し、そのアドレスに対して `fetch` することでオブジェクトの内容を取得するサンプルが示されています。
確かにこれを使うとコードがめっちゃシンプルです。
署名付き URL ってこういった用途に使ってよいの？という疑問はありますが、下記のように有効期限を 60 秒くらいに設定しておけばおそらく問題ないかと思います。

{{< code lang="ts" >}}
import { S3Client, GetObjectCommand } from '@aws-sdk/client-s3'
import { getSignedUrl } from '@aws-sdk/s3-request-presigner'
import config from './config'

// 指定したオブジェクトの内容を取得する
export async function getS3Object(
  bucketName: string,
  objectKey: string
): Promise<string> {
  try {
    const s3 = new S3Client({
      region: config.region,
      credentials: config.credentials,
    })
    const command = new GetObjectCommand({
      Bucket: bucketName,
      Key: objectKey,
    })

    // Create the presigned URL and fetch the object
    const signedUrl = await getSignedUrl(s3, command, { expiresIn: 60 })
    return (await fetch(signedUrl)).text()
  } catch (err) {
    throw new Error(err)
  }
}

// 使用例
const text = await getS3Object('s3-123456789012-sample-bucket-1', 'object-key-1')
{{< /code >}}


### SDK V2 の場合

{{< code lang="ts" title="get-object-v2.ts" >}}
import { S3 } from 'aws-sdk';
const s3 = new S3();

async function getS3Object(): Promise<string | undefined> {
  const output: S3.GetObjectOutput = await s3.getObject({
    Bucket: 's3-123456789012-sample-bucket-1',
    Key: 'object-key-1',
  }).promise();

  return output.Body?.toString();
}

(async function main() {
  const body = await getS3Object();
  console.log(body);
})();
{{< /code >}}

{{< code title="実行結果 (SDK V2)" >}}
object-body-1
{{< /code >}}

