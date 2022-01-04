---
title: "DynamoDB を Node.js で操作する（SDK ver.2 の場合）"
url: "/p/8t7iu6g"
date: "2021-05-01"
tags: ["AWS", "AWS/DynamoDB", "TypeScript"]
weight: 102
---

{{% private %}}
- [SDK v2 開発者ガイド - DynamoDB 例 (ver.2)](https://docs.aws.amazon.com/ja_jp/sdk-for-javascript/v2/developer-guide/dynamodb-examples.html)
{{% /private %}}

ここでは、Node.js 用の AWS SDK ver.2 を使って Amazon DynamoDB を操作する方法を説明します。
[TypeScript の基本的な環境構築](/p/taiqx6d) は終わっているものとします。

[SDK ver.3 を使う方法はこちらの記事](/p/5mv5dkt) を参照してください。
基本的には ver.3 の使用が推奨されていますが、AWS の Lambda 実行環境は現時点（2021年5月）でも ver.2 がインストールされていたりするので、ver.2 の需要はまだあると思います。


Dynamo DB 用の SDK (ver.2) をインストールする
----

AWS SDK version 2 で DynamoDB を扱うには、次のように AWS SDK パッケージ全体をインストールする必要があります（version 3 では DynamoDB サービスなどのパッケージを個別にインストールできます）。

{{< code >}}
$ npm install aws-sdk --save
{{< /code >}}

これで、TypeScript コードから次のように SDK モジュールをインポートできるようになります。

{{< code lang="typescript" >}}
import * as AWS from 'aws-sdk';
{{< /code >}}

`AWS.DynamoDB` だけ参照したければ、次のようにインポートできます。

{{< code lang="ts" >}}
import { DynamoDB } from 'aws-sdk';
{{< /code >}}


DynamoDB インスタンスの生成
----

### 基本

DynamoDB の API を呼び出すには、まずは [AWS.DynamoDB](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB.html) インスタンスを生成する必要があります。

{{< code lang="ts" >}}
import * as AWS from 'aws-sdk';

const dynamoDb = new AWS.DynamoDB();
{{< /code >}}

### 接続設定 (config)

[DynamoDB コンストランクタ](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB.html#constructor-property) の引数でオプションオブジェクトを渡すと、接続先のリージョンやエンドポイントを設定することができます。
例えば、エンドポイントを `http://localhost:8000` に設定すれば、[DynamoDB Local](/p/rdq4eq2) によるテスト用サーバに接続できます。

{{< code lang="ts" >}}
import * as AWS from 'aws-sdk';

const dynamoDb = new AWS.DynamoDB({
  apiVersion: '2012-08-10',
  region: 'ap-northeast-1',
  endpoint: 'http://localhost:8000',
  accessKeyId: 'fakeMyKeyId',
  secretAccessKey: 'fakeSecretAccessKey',
});
{{< /code >}}

[認証情報ファイルなど](/p/j5iu7it) で実行ユーザーに適切な接続情報が設定されていれば、上記のようにコード内で接続設定する必要はありませんが、少なくとも __`apiVersion`__ は明示しておくとが推奨されています。
環境変数（`AWS_REGION`、`AWS_ACCESS_KEY_ID`、`AWS_SECRET_ACCESS_KEY` など）で接続情報を設定することもできます。

接続情報の優先順位は次の通りです。

1. プログラム内での設定（ローカル設定 ＞ グローバル設定）
1. 環境変数での設定 (`AWS_REGION`, `AWS_ACCESS_KEY_ID`, `AWS_SECRET_ACCESS_KEY`, `AWS_SESSION_TOKEN`)
1. 認証情報ファイルでの設定 (`~/.aws/credentials`, `~/.aws/config`)

SDK 全体のグローバル設定を変更する場合は、__`AWS.config.update`__ メソッドに [AWS.Config](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/Config.html) オブジェクトを渡します。
このグローバル設定は、これ以降に生成されるクライアントオブジェクトのデフォルト設定として使用されます。

{{< code lang="ts" >}}
AWS.config.update({
  region: 'ap-northeast-1',
  accessKeyId: 'fakeMyKeyId',
  secretAccessKey: 'fakeSecretAccessKey',
  dynamodb: {
    apiVersion: '2012-08-10',
    endpoint: 'http://localhost:8000'
  }
});
{{< /code >}}

### プロキシ設定

社内のプロキシ環境などから接続するときは、`AWS.Config` の __`httpOptions`__ プロパティを使用します。

{{< code title="proxy-agent のインストール" >}}
$ npm install proxy-agent --save
{{< /code >}}

{{< code lang="ts" >}}
import * as AWS from 'aws-sdk';
import * as proxy from 'proxy-agent';

AWS.config.update({
  httpOptions: { agent: proxy('http://internal.proxy.com') }
});
{{< /code >}}

### DynamoDB.DocumentClient インスタンス

`AWS.DynamoDB` インスタンスの代わりに、`AWS.DynamoDB.DocumentClient` インスタンスを使うと、もう少し抽象度の高い API で CRUD 系操作を行えるようです。
用途によってはこちらを使った方が楽かもしれませんが、`createTable` などテーブル自体の操作はできません。

- [Class: AWS.DynamoDB.DocumentClient — AWS SDK for JavaScript](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB/DocumentClient.html)

インスタンスの作成方法は `AWS.DynamoDB` と同様です。

{{< code lang="ts" >}}
import * as AWS from 'aws-sdk';

const dynamoDb = new AWS.DynamoDB.DocumentClient(option);
{{< /code >}}


テーブルを作成する (createTable)
----

{{< code lang="ts" >}}
import * as AWS from 'aws-sdk';
const dynamoDb = new AWS.DynamoDB({
  apiVersion: '2012-08-10',
  region: 'ap-northeast-1'
});

async function createTable() {
  // 作成するテーブルの情報
  const params: AWS.DynamoDB.CreateTableInput = {
    TableName: 'Games',  // テーブル名
    BillingMode: 'PAY_PER_REQUEST',  // 課金方法
    KeySchema: [
      { AttributeName: 'Hardware', KeyType: 'HASH' },  // パーティションキー
      { AttributeName: 'GameId', KeyType: 'RANGE' },  // ソートキー
    ],
    AttributeDefinitions: [
      { AttributeName: 'Hardware', AttributeType: 'S' },  // 文字列属性
      { AttributeName: 'GameId', AttributeType: 'S' },  // 文字列属性
    ],
    StreamSpecification: {
      StreamEnabled: false,
    },
  };

  try {
    const result = await dynamoDb.createTable(params).promise();
    // テーブルの作成に成功したら、ARN 情報を取得してみる
    console.log(result.TableDescription?.TableArn);
  } catch (err) {
    console.error(err);
  }
}
{{< /code >}}

{{< code title="実行結果" >}}
arn:aws:dynamodb:ap-northeast-1:123456789012:table/Games
{{< /code >}}

- 参考: [DynamoDB.createTable](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB.html#createTable-property)


テーブルを削除する (deleteTable)
----

{{< code lang="ts" >}}
async function deleteTable() {
  const params: AWS.DynamoDB.DeleteTableInput = {
    TableName: 'Games' // 削除するテーブルの名前
  };

  try {
    const result = await dynamoDb.deleteTable(params) .promise();
    console.log(result.TableDescription?.TableArn)
  } catch (err) {
    console.error(err);
  }
}

deleteTable();
{{< /code >}}

{{< code title="実行結果" >}}
arn:aws:dynamodb:ap-northeast-1:123456789012:table/Games
{{< /code >}}

- 参考: [DynamoDB.deleteTable](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB.html#deleteTable-property)


テーブルの一覧を取得する (listTables)
----

{{< code lang="ts" >}}
async function listTables() {
  const params: AWS.DynamoDB.ListTablesInput = {
    Limit: 10  // 一度に取得するテーブル数
  }

  try {
    const result = await dynamoDb.listTables(params).promise();
    console.log(result.TableNames?.join('\n'));
  } catch (err) {
    console.error(err);
  }
}

listTables();
{{< /code >}}

- 参考: [DynamoDB.listTables](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB.html#listTables-property)


テーブルの詳細情報を取得する (describeTable)
----

指定したテーブルのスキーマ情報などを取得できます。

{{< code lang="ts" >}}
async function describeTable() {
  const params: AWS.DynamoDB.DescribeTableInput = {
    TableName: 'Games'
  };

  try {
    const result = await dynamoDb.describeTable(params).promise();
    console.log(result.Table?.KeySchema);
  } catch (err) {
    console.error(err);
  }
}

describeTable();
{{< /code >}}

{{< code lang="js" title="実行結果" >}}
[
  { AttributeName: 'Hardware', KeyType: 'HASH' },
  { AttributeName: 'GameId', KeyType: 'RANGE' }
]
{{< /code >}}

- 参考: [DynamoDB.describeTable](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB.html#deleteTable-property)


アイテムを追加する (putItem)
----

### DynamoDB を使う場合

`putItem` メソッドでテーブルに要素を追加するときは、少なくともプライマリキー属性（パーティションキー、およびソートキー）の指定が必要です（`Games` テーブルの例では `Hardware` と `GameId`）。
すでに同じキーの要素が存在する場合、デフォルトでは新しい値で上書きされます。
`ReturnValues` プロパティに `ALL_OLD` を指定しておくと、もともと格納されていた値を戻り値として取得できます。

{{< code lang="ts" >}}
async function putItem() {
  // 追加先のテーブル名や、追加するアイテムを指定
  const params: AWS.DynamoDB.PutItemInput = {
    TableName: 'Games',
    Item: {
      Hardware: { S: 'SFC' },
      GameId: { S: '1990-SuperMarioWorld' },
      Title: { S: 'Super Mario World' },
      Players: { N: '2' },
      Genre: { S: 'ACT' },
    },
    ReturnValues: 'ALL_OLD' // 同じプライマリキーの値があったら、過去の値を返す
  };

  try {
    const result = await dynamoDb.putItem(params).promise();
    console.log(result);
  } catch (err) {
    console.error(err);
  }
}

putItem();
{{< /code >}}

{{< code lang="js" title="実行結果（2回目以降の実行時）" >}}
{
  Attributes: {
    Title: { S: 'Super Mario World' },
    Genre: { S: 'ACT' },
    GameId: { S: '1990-SuperMarioWorld' },
    Hardware: { S: 'SFC' },
    Players: { N: '2' }
  }
}
{{< /code >}}

`putItem` メソッドはデフォルトではアイテムの内容を上書きしますが、この振る舞いを抑制したいときは、コマンドのパラメータに次のように __`ConditionExpression`__ を指定します。
このようにすると、プライマリキーが一致するアイテムがすでに存在する場合に、`ConditionalCheckFailedException` 例外が発生するようになります。

{{< code lang="ts" >}}
async function putItem() {
  const params: AWS.DynamoDB.PutItemInput = {
    TableName: 'Games',
    Item: {
      Hardware: { S: 'SFC' },
      GameId: { S: '1990-SuperMarioWorld' },
      Title: { S: 'Super Mario World' },
      Players: { N: '2' },
      Genre: { S: 'ACT' },
    },
    ConditionExpression: 'attribute_not_exists(Hardware)'
  };
  // ...
}
{{< /code >}}

- 参考: [DynamoDB.putItem](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB.html#putItem-property)

### DynamoDB.DocumentClient を使う場合

`DocumentClient` インスタンスを使用すると、属性値の指定が若干シンプルになります（`S` や `N` などのタイプ指定が必要なくなります）。
それ以外は同じです。

{{< code lang="ts" >}}
import * as AWS from 'aws-sdk';
const documentClient = new AWS.DynamoDB.DocumentClient({
  apiVersion: '2012-08-10',
  region: 'ap-northeast-1'
});

async function documentClient_put() {
  const params: AWS.DynamoDB.DocumentClient.PutItemInput = {
    TableName: 'Games',
    Item: {
      Hardware: 'SFC',
      GameId: '1990-SuperMarioWorld',
      Title: 'Super Mario World',
      Players: 2,
      Genre: 'ACT',
    },
    ReturnValues: 'ALL_OLD' // 同じプライマリキーの値があったら、過去の値を返す
  };

  try {
    const result = await documentClient.put(params).promise();
    console.log(result);
  } catch (err) {
    console.error(err);
  }
}

documentClient_put();
{{< /code >}}

実行結果の形式も少しだけシンプルになります。

{{< code lang="js" title="実行結果（2回目以降の実行時）" >}}
{
  Attributes: {
    Title: 'Super Mario World',
    Genre: 'ACT',
    GameId: '1990-SuperMarioWorld',
    Hardware: 'SFC',
    Players: 2
  }
}
{{< /code >}}

- 参考: [DynamoDB.DocumentClient.put](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB/DocumentClient.html#put-property)


アイテムの属性値を部分的に更新する (updateItem)
----

### DynamoDB を使う場合

`putItem` に似たメソッドに `updateItem` があります。
`putItem` はプライマリーキー属性が一致するアイテムがあると、その内容を完全に置き換えますが、`updateItem` の場合は、__指定した属性の値のみを更新__ します（まだ存在しない属性を指定すると追加されます）。
プライマリーキー属性に一致するアイテムが見つからない場合に、新しいアイテムを追加するのは `putItem` も `updateItem` も同様です。

属性値の更新内容は、SQL のプレースホルダーを使った構文のような感じで指定する必要があります。
DynamoDB の API は複雑すぎるとは聞いていましたが、確かにこの API 仕様はクレイジーですね・・・（AWS SDK が低レベルすぎるだけかもしれませんが）。
次の例では、既存のアイテムの `Title` 属性の値を更新し、さらに新しい属性 `Maker` を追加しています。
新しい属性名は __`#xxx`__、新しい属性値は __`:xxx`__ といった形のプレースホルダーで指定する必要があります。

{{< code lang="ts" >}}
async function updateItem() {
  const params: AWS.DynamoDB.UpdateItemInput = {
    TableName: 'Games',
    // 更新するアイテムを特定するプライマリーキー属性
    Key: {
      Hardware: { S: 'SFC' },
      GameId: { S: '1990-SuperMarioWorld' }
    },
    // 属性値の更新方法を下記 3 プロパティで設定
    UpdateExpression: 'set Title = :x, #a = :y',
    ExpressionAttributeNames: {
      '#a': 'Maker',
    },
    ExpressionAttributeValues: {
      ':x': { S: 'Mario 4' },
      ':y': { S: 'Nintendo' }
    },
    // 更新後の内容を戻り値で知りたいとき（ALL_OLD なら更新前の値が返される）
    ReturnValues: 'ALL_NEW',
  };

  try {
    const result = await dynamoDb.updateItem(params).promise();
    console.log(result);
  } catch (err) {
    console.error(err);
  }
}

updateItem();
{{< /code >}}

{{< code lang="js" title="実行結果" >}}
{
  Attributes: {
    Title: { S: 'Mario 4' },
    Maker: { S: 'Nintendo' },
    GameId: { S: '1990-SuperMarioWorld' },
    Hardware: { S: 'SFC' },
    Players: { N: '2' }
  }
}
{{< /code >}}

- 参考: [DynamoDB.updateItem](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB.html#updateItem-property)

### DynamoDB.DocumentClient を使う場合

`DocumentClient` インスタンスを使用すると、属性値の指定が若干シンプルになります（`S` や `N` などのタイプ指定が必要なくなります）。
それ以外は同じです。

{{< code lang="ts" >}}
import * as AWS from 'aws-sdk';
const documentClient = new AWS.DynamoDB.DocumentClient({
  apiVersion: '2012-08-10',
  region: 'ap-northeast-1'
});

async function documentClient_update() {
  const params: AWS.DynamoDB.DocumentClient.UpdateItemInput = {
    TableName: 'Games',
    Key: { Hardware: 'SFC', GameId: '1990-SuperMarioWorld' },
    UpdateExpression: 'set Title = :x, #a = :y',
    ExpressionAttributeNames: { '#a': 'Maker' },
    ExpressionAttributeValues: { ':x': 'Mario 4', ':y': 'Nintendo' },
    ReturnValues: 'ALL_NEW',
  };

  try {
    const result = await documentClient.update(params).promise();
    console.log(result);
  } catch (err) {
    console.error(err);
  }
}

documentClient_update();
{{< /code >}}

実行結果の形式も少しだけシンプルになります。

{{< code lang="js" title="実行結果" >}}
{
  Attributes: {
    Title: 'Mario 4',
    Maker: 'Nintendo',
    GameId: '1990-SuperMarioWorld',
    Genre: 'ACT',
    Hardware: 'SFC',
    Players: 2
  }
}
{{< /code >}}

- 参考: [DynamoDB.DocumentClient.update](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB/DocumentClient.html#update-property)


アイテムを取得する (getItem)
----

### DynamoDB を使う場合

アイテムを 1 つ取得するときは、テーブル名 (`Table`) と、アイテムを特定するためのプライマリーキー (`Key`) を指定します。
複合プライマリキーを使用している場合は、パーティションキーとソートキーの両方を指定する必要があります（`ValidationException` が発生します）。

{{< code lang="ts" >}}
async function getItem() {
  const params: AWS.DynamoDB.GetItemInput = {
    TableName: 'Games',
    Key: {
      Hardware: { S: 'SFC' },
      GameId: { S: '1990-SuperMarioWorld' }
    }
  };

  try {
    const result = await dynamoDb.getItem(params).promise();
    console.log(result);
  } catch (err) {
    console.error(err);
  }
}
{{< /code >}}

{{< code lang="js" title="実行結果" >}}
{
  Item: {
    Title: { S: 'Super Mario World' },
    GameId: { S: '1990-SuperMarioWorld' },
    Hardware: { S: 'SFC' },
    Players: { N: '2' }
  }
}
{{< /code >}}

- 参考: [DynamoDB.getItem](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB.html#getItem-property)

### DynamoDB.DocumentClient を使う場合

`DocumentClient` インスタンスを使用すると、プライマリーキーの指定が若干シンプルになります（`S` や `N` などのタイプ指定が必要なくなります）。
それ以外は同じです。

{{< code lang="ts" >}}
import * as AWS from 'aws-sdk';
const documentClient = new AWS.DynamoDB.DocumentClient({
  apiVersion: '2012-08-10',
  region: 'ap-northeast-1'
});

async function documentClient_get() {
  const params: AWS.DynamoDB.DocumentClient.GetItemInput = {
    TableName: 'Games',
    Key: {
      Hardware: 'SFC',
      GameId: '1990-SuperMarioWorld'
    }
  };

  try {
    const result = await documentClient.get(params).promise();
    console.log(result);
  } catch (err) {
    console.error(err);
  }
}

documentClient_get();
{{< /code >}}

実行結果の形式も少しだけシンプルになります。

{{< code lang="js" title="実行結果" >}}
{
  Item: {
    Title: 'Super Mario World',
    Genre: 'ACT',
    GameId: '1990-SuperMarioWorld',
    Hardware: 'SFC',
    Players: 2
  }
}
{{< /code >}}

- 参考: [DynamoDB.DocumentClient.get](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB/DocumentClient.html#get-property)


アイテムを削除する (deleteItem)
----

### DynamoDB を使う場合

{{< code lang="ts" >}}
async function deleteItem() {
  const params: AWS.DynamoDB.DeleteItemInput = {
    TableName: 'Games',
    Key: {
      Hardware: { S: 'SFC' },
      GameId: { S: '1990-SuperMarioWorld' }
    },
    ReturnValues: 'ALL_OLD' // 削除されたアイテムの内容を戻り値で取得
  };

  try {
    const result = await dynamoDb.deleteItem(params).promise();
    console.log(result);
  } catch (err) {
    console.error(err);
  }
}
{{< /code >}}

{{< code lang="js" title="実行結果" >}}
{
  Item: {
    Title: { S: 'Super Mario World' },
    GameId: { S: '1990-SuperMarioWorld' },
    Hardware: { S: 'SFC' },
    Players: { N: '2' }
  }
}
{{< /code >}}

アイテムを削除するときは、テーブル名 (`Table`) と、アイテムを特定するためのプライマリーキー (`Key`) を指定します。
複合プライマリキーを使用している場合は、パーティションキーとソートキーの両方を指定する必要があります（`ValidationException` が発生します）。

- 参考: [DynamoDB.deleteItem](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB.html#deleteItem-property)

### DynamoDB.DocumentClient を使う場合

`DocumentClient` インスタンスを使用すると、プライマリーキーの指定が若干シンプルになります（`S` や `N` などのタイプ指定が必要なくなります）。
それ以外は同じです。

{{< code lang="ts" >}}
import * as AWS from 'aws-sdk';
const documentClient = new AWS.DynamoDB.DocumentClient({
  apiVersion: '2012-08-10',
  region: 'ap-northeast-1'
});

async function documentClient_delete() {
  const params: AWS.DynamoDB.DocumentClient.DeleteItemInput = {
    TableName: 'Games',
    Key: {
      Hardware: 'SFC',
      GameId: '1990-SuperMarioWorld'
    },
    ReturnValues: 'ALL_OLD' // 削除されたアイテムの内容を戻り値で取得
  };

  try {
    const result = await documentClient.delete(params).promise();
    console.log(result);
  } catch (err) {
    console.error(err);
  }
}

documentClient_delete();
{{< /code >}}

実行結果の形式も少しだけシンプルになります。

{{< code lang="js" title="実行結果" >}}
{
  Attributes: {
    Title: 'Super Mario World',
    Genre: 'ACT',
    GameId: '1990-SuperMarioWorld',
    Hardware: 'SFC',
    Players: 2
  }
}
{{< /code >}}

- 参考: [DynamoDB.DocumentClient.delete](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB/DocumentClient.html#delete-property)


バッチ処理 - 複数のアイテムを追加・削除する (batchWriteItem)
----

DynamoDB テーブルに複数のアイテムを追加、削除したいときは、`putItem` や `deleteItem` を何度も呼び出すよりも、`batchWriteItem` を使うのが効率的です。
ただし、最大でも 25 項目までといった細かな制約があります。
その他の制約などは、[BatchWriteItem の仕様](https://docs.aws.amazon.com/ja_jp/amazondynamodb/latest/APIReference/API_BatchWriteItem.html) を確認してください。

次の例では、`Games` テーブルに 2 つの項目を追加し、1 つの項目を削除しています。

### DynamoDB を使う場合

{{< code lang="ts" >}}
async function batchWriteItem() {
  const params: AWS.DynamoDB.BatchWriteItemInput = {
    RequestItems: {
      'Games': [
        {
          PutRequest: {
            Item: {
              Hardware: { S: 'FC' },
              GameId: { S: '1984-lode-runner' },
              Title: { S: 'Lode Runner' },
              Players: { N: '1' },
              Genre: { S: 'ACT' },
            }
          },
        },
        {
          PutRequest: {
            Item: {
              Hardware: { S: 'FC' },
              GameId: { S: '1985-exed-exes' },
              Title: { S: 'Exed Exes' },
              Players: { N: '2' },
              Genre: { S: 'STG' },
            }
          }
        },
        {
          DeleteRequest: {
            Key: {
              Hardware: { S: 'SFC' },
              GameId: { S: '1990-SuperMarioWorld' },
            }
          }
        }
      ]
    }
  };

  try {
    const result = await dynamoDb.batchWriteItem(params).promise();
    console.log(result);
  } catch (err) {
    console.error(err);
  }
}

batchWriteItem()
{{< /code >}}

{{< code lang="js" title="実行結果" >}}
{ UnprocessedItems: {} }
{{< /code >}}

- 参考: [DynamoDB.batchWriteItem](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB.html#batchWriteItem-property)

### DynamoDB.DocumentClient を使う場合

`DocumentClient` インスタンスを使用すると、属性値の指定が若干シンプルになります（`S` や `N` などのタイプ指定が必要なくなります）。
それ以外は同じです。

{{< code lang="ts" >}}
async function documentClient_batchWrite() {
  const params: AWS.DynamoDB.DocumentClient.BatchWriteItemInput = {
    RequestItems: {
      'Games': [
        {
          PutRequest: { Item: { Hardware: 'FC', GameId: '1984-lode-runner', Title: 'Lode Runner', Players: 1, Genre: 'ACT' }}
        },
        {
          PutRequest: { Item: { Hardware: 'FC', GameId: '1985-exed-exes', Title: 'Exed Exes', Players: 1, Genre: 'STG' }}
        },
        {
          DeleteRequest: { Key: { Hardware: 'SFC', GameId: '1990-SuperMarioWorld' }}
        }
      ]
    }
  };

  try {
    const result = await documentClient.batchWrite(params).promise();
    console.log(result);
  } catch (err) {
    console.error(err);
  }
}
{{< /code >}}

- 参考: [DynamoDB.DocumentClient.batchWrite](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB/DocumentClient.html#batchWrite-property)

### batchWrite のパラメーターを作るユーティリティ関数を作る

`DynamoDB.DocumentClient.batchWrite` メソッドに渡すパラメーターをハードコードするのは大変なので、現実的には、複数アイテムをまとめて追加するためのラッパー関数的なものを用意することになると思います。

{{< code lang="ts" title="こんな感じ" >}}
type Game = {
  Hardware: string,
  GameId: string,
  Title: string,
  Players: number,
  Genre: string
};

/** DocumentClient.batchWrite に渡すオブジェクトを生成します */
function createParams(games: Game[]): AWS.DynamoDB.DocumentClient.BatchWriteItemInput {
  return {
    RequestItems: {
      'Games': games.map(g => ({
        PutRequest: { Item: g }
      }))
    }
  };
}
{{< /code >}}


テーブル内の要素をすべて取得する (scan)
----

DynamoDB テーブル内のすべてのアイテムを取得するには `scan` メソッドを使用します。
フィルタ条件を指定することもできますが、内部的には「全アイテムの取得 → フィルタ」という動作になるため、`scan` メソッドは基本的にはアイテム数が少ない場合にのみ使用できます（返却するデータは 1MB までなどの制約があります）。
プライマリーキーで検索条件を指定できる場合は、`scan` ではなく `query` メソッドを使うことで効率的な取得が可能です。

`DynamoDB` クラスにも `DynamoDB.DocumentClient` クラスにも同名の `scan` メソッドがありますが、後者の方が若干シンプルに記述できる（`S` や `N` などの属性タイプ指定を省略できる）ので、そちらを使ったサンプルコードを示します。

{{< code lang="ts" >}}
async function scan() {
  const params: AWS.DynamoDB.DocumentClient.ScanInput = {
    TableName: 'Games'
  };

  try {
    const result = await documentClient.scan(params).promise();
    console.log(result);
  } catch (err) {
    console.error(err);
  }
}

scan();
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code lang="js" >}}
{
  Items: [
    {
      Title: 'Lode Runner',
      Genre: 'ACT',
      GameId: '1984-lode-runner',
      Hardware: 'FC',
      Players: 1
    },
    {
      Title: 'Exed Exes',
      Genre: 'STG',
      GameId: '1985-exed-exes',
      Hardware: 'FC',
      Players: 2
    },
    {
      Title: 'Super Mario World',
      Genre: 'ACT',
      GameId: '1990-SuperMarioWorld',
      Hardware: 'SNES',
      Players: 2
    }
  ],
  Count: 3,
  ScannedCount: 3
}
{{< /accordion >}}
{{< /code >}}

`scan` 結果を何らかの条件で絞り込みたい場合は、`FilterExpression` パラメーターを使用します。
次の例では、`Genre` 属性が `ACT` のアイテムだけを取得しています。
ただし前述のように、`FilterExpression` による絞り込みは、全アイテム取得後にフィルターしているだけなので注意してください（アイテム数が増えてくると機能しなくなります）。

{{< code lang="ts" title="scan 用のフィルター条件を追加" >}}
const params: AWS.DynamoDB.DocumentClient.ScanInput = {
  TableName: 'Games',
  FilterExpression: 'Genre = :genre',
  ExpressionAttributeValues: { ':genre': 'ACT' }
};
{{< /code >}}

- 参考: [DynamoDB.scan](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB.html#scan-property)
- 参考: [DynamoDB.DocumentClient.scan](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB/DocumentClient.html#scan-property)
- 参考: [Scan API 仕様](https://docs.aws.amazon.com/ja_jp/amazondynamodb/latest/APIReference/API_Scan.html)


テーブル内の要素をプライマリーキーでフィルタして取得する (query)
----

DynamoDB テーブルから、指定したプライマリーキーに一致するアイテムを検索するには、`query` メソッドを使用します。
主に、複合キー（パーティションキー + ソートキー）で構成されているテーブルから、

- パーティションキーのみの指定
- パーティションキー + ソートキーの条件指定

といった検索を行います。
ソートキーの方は部分一致や大小比較など、ある程度柔軟なフィルタ条件を指定できますが、パーティションキーの方は、あくまで完全一致させる必要があります。

{{< code lang="ts" >}}
async function query() {
  const params: AWS.DynamoDB.DocumentClient.QueryInput = {
    TableName: 'Games',
    KeyConditionExpression: 'Hardware = :hard and GameId > :game',
    ExpressionAttributeValues: { ':hard': 'FC', ':game': '1984-' }
  };

  try {
    const result = await documentClient.query(params).promise();
    console.log(result);
  } catch (err) {
    console.error(err);
  }
}

query();
{{< /code >}}

{{< code lang="js" title="実行結果" >}}
{
  Items: [
    {
      Title: 'Lode Runner',
      Genre: 'ACT',
      GameId: '1984-lode-runner',
      Hardware: 'FC',
      Players: 1
    },
    {
      Title: 'Exed Exes',
      Genre: 'STG',
      GameId: '1985-exed-exes',
      Hardware: 'FC',
      Players: 2
    }
  ],
  Count: 2,
  ScannedCount: 2
}
{{< /code >}}

- 参考: [DynamoDB.query](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB.html#query-property)
- 参考: [DynamoDB.DocumentClient.query](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/DynamoDB/DocumentClient.html#query-property)
- 参考: [Query API 仕様](https://docs.aws.amazon.com/ja_jp/amazondynamodb/latest/APIReference/API_Query.html)

