---
title: "DynamoDB を Node.js で操作する（SDK ver.3 の場合）"
url: "/p/5mv5dkt"
date: "2021-03-02"
tags: ["AWS", "AWS/DynamoDB", "TypeScript"]
weight: 101
---

{{% private %}}
- [SDK v3 開発者ガイド - DynamoDB 例 (ver.2)](https://docs.aws.amazon.com/ja_jp/sdk-for-javascript/v3/developer-guide/dynamodb-examples.html)
- [ドキュメントクライアントの使用例](https://docs.aws.amazon.com/ja_jp/sdk-for-javascript/v3/developer-guide/dynamodb-example-dynamodb-utilities.html)
{{% /private %}}

ここでは、Node.js 用の AWS SDK ver.3 を使って Amazon DynamoDB を操作する方法を説明します。
[TypeScript の基本的な環境構築](/p/taiqx6d) は終わっているものとします。


DynamoDB 用の Node.js SDK (ver.3) をインストールする
----

まずは、AWS SDK version 3 の DynamoDB 用パッケージをインストールします。
DynamoDB を操作するときに主に次のようなクライアントクラスを使用するのですが、後者の `DynamoDBDocumentClient` の方は、前者の `DynamoDBClient` インスタンスをラップして扱いやすくするためのクラスなので、必要に応じてインストールしてください（主にテーブル内のアイテムを扱うときに便利です）。

- [DynamoDBClient](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-dynamodb/index.html)
    - DynamoDB を扱うための基本クラス（DB クライアントと呼ばれる）
    - __`@aws-sdk/client-dynamodb`__ パッケージが必要
- [DynamoDBDocumentClient](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/modules/_aws_sdk_lib_dynamodb.html)
    - 上記を扱いやすくするためのクラス（Document クライアントと呼ばれる）
    - __`@aws-sdk/lib-dynamodb`__ パッケージが必要
    - __`@aws-sdk/util-dynamodb`__ パッケージも必要っぽい

{{< code >}}
### yarn の場合
$ yarn add @aws-sdk/client-dynamodb
$ yarn add @aws-sdk/lib-dynamodb @aws-sdk/util-dynamodb  # Document クライアント

### npm の場合
$ npm install @aws-sdk/client-dynamodb
$ npm install @aws-sdk/lib-dynamodb @aws-sdk/util-dynamodb  # Document クライアント
{{< /code >}}

これで、TypeScript コードから次のようにパッケージ内のクラスをインポートできるようになります。

{{< code lang="ts" title="main.ts" >}}
import { DynamoDBClient } from '@aws-sdk/client-dynamodb'
import { DynamoDBDocumentClient } from '@aws-sdk/lib-dynamodb'
{{< /code >}}


DynamoDBClient / DynamoDBDocumentClient インスタンスの生成
----

DynamoDB の API を呼び出すには、まずは `DynamoDBClient` インスタンスを生成する必要があります。
さらに、抽象度の高い API を使用したい場合は、`DynamoDBDocumentClient` インスタンスも生成します。

```ts
import { DynamoDBClient, DynamoDBClientConfig } from '@aws-sdk/client-dynamodb'
import { DynamoDBDocumentClient } from '@aws-sdk/lib-dynamodb'

const config: DynamoDBClientConfig = {
  // ...接続設定...
}
const dbClient = new DynamoDBClient(config)
const documentClient = DynamoDBDocumentClient.from(dbClient)
```

`DynamoDBClient` のコンストラクタには、[DynamoDBClientConfig](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-dynamodb/globals.html#dynamodbclientconfig) オブジェクトを渡すようになっており、これで接続先のリージョンやエンドポイント URL などを設定できるようになっています。

下記に、いくつか接続設定の例を示します。

### DynamoDB Local（テスト用ローカルサーバー）に接続する場合

[DynamoDB Local テストサーバ (localhost:8000)](/p/rdq4eq2) に接続するには次のように設定します。
ポイントは `endpoint` プロパティにローカルアドレスを指定するところです。
`region` や `credentials` はダミーの値で大丈夫です。

```ts
const config: DynamoDBClientConfig = {
  endpoint: 'http://localhost:8000',
  region: 'ap-northeast-1',
  credentials: { accessKeyId: 'FAKE', secretAccessKey: 'FAKE' },
}
```

### 環境変数で設定した Credential 情報で接続する

```ts
const config: DynamoDBClientConfig = {
  endpoint: process.env.DYNAMODB_ENDPOINT,
  region: process.env.DYNAMODB_REGION ?? 'ap-northeast-1',
  credentials: {
    accessKeyId: process.env.DYNAMODB_ACCESS_KEY_ID ?? 'FAKE',
    secretAccessKey: process.env.DYNAMODB_SECRET_ACCESS_KEY ?? 'FAKE',
  },
}
```

DynamoDB Local ではなく、本物の DynamoDB サービスに接続する場合は、`endpoint` プロパティを `undefined` にしておけば OK です（つまり、`prcess.env.DYNAMODB_ENDPOINT` 環境変数を設定しなければ OK）。

### Cognito 認証＆認可で取得したアクセストークンで接続する

Web アプリなどから AWS 上の DynamoDB にアクセスしたいのであれば、Cognito による認証情報などを使って、次のようなコンフィギュレーションを行えば接続できます。

```ts
// import { Auth } from 'aws-amplify'

const config: DynamoDBClientConfig = {
  region: 'ap-northeast-1',
  credentials: await Auth.currentCredentials() // Cognito サインイン済みと仮定
}
```

### 認証情報ファイルの設定で接続する

[aws configure コマンドなど](https://docs.aws.amazon.com/ja_jp/cli/latest/userguide/cli-configure-quickstart.html) で設定した認証情報を使って DynamoDB に接続する場合は、コンフィグ情報に何も設定しなければ OK です。

```ts
const config: DynamoDBClientConfig = {}
```

### （おまけ）SDK version 2 時代のコンフィグ

{{% accordion title="SDK version 2 版" %}}
古い AWS SDK (version 2) では、接続情報を `AWS.config` を使ってグローバルに設定していました。

{{< code lang="js" >}}
AWS.config.update({
  region: 'ap-northeast-1',
  accessKeyId: 'FAKE',
  secretAccessKey: 'FAKE',
  dynamodb: {
    apiVersion: '2012-08-10',
    endpoint: 'http://localhost:8000'
  }
})
{{< /code >}}

この方法は、プログラムの途中で設定を変更できてしまうため、全体の振る舞いを把握しにくくなってしまう問題がありました。
AWS SDK version 3 では、各クライアントクラスのコンストラクタでコンフィグ情報を渡すことを強制しているため、一貫した動作が保証されます。
{{% /accordion %}}


テーブルを作成する (CreateTableCommand)
----

DynamoDB のテーブルを作成するには、`DynamoDBClient#send` メソッドで、[CreateTableCommand](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-dynamodb/classes/createtablecommand.html) を送ります。

次の例では、`Games` という名前のテーブルを作成しています。
DynamoDB は NoSQL なので、基本的に属性を前もって定義しておく必要はありませんが、プライマリーキー（パーティションキー、およびソートキー）だけはテーブル作成時に指定する必要があります。
`Games` テーブルでは、次のようなプライマリーキーを定義しています。

- `Hardware`: 文字列型 (S) のパーティションキー (HASH)
- `GameId`: 文字列型 (S) のソートキー (RANGE)

{{< code lang="typescript" title="create-table.ts" >}}
import { CreateTableCommand, DynamoDBClient } from '@aws-sdk/client-dynamodb'

const dbClient = new DynamoDBClient({
  endpoint: 'http://localhost:8000',
})

// テーブルを作成する
async function createTable() {
  try {
    const command = new CreateTableCommand({
      TableName: 'Games', // テーブル名
      KeySchema: [
        { AttributeName: 'Hardware', KeyType: 'HASH' }, // パーティションキー
        { AttributeName: 'GameId', KeyType: 'RANGE' }, // ソートキー
      ],
      AttributeDefinitions: [
        { AttributeName: 'Hardware', AttributeType: 'S' }, // 文字列属性
        { AttributeName: 'GameId', AttributeType: 'S' }, // 文字列属性
      ],
      ProvisionedThroughput: {
        ReadCapacityUnits: 1,
        WriteCapacityUnits: 1,
      },
      StreamSpecification: {
        StreamEnabled: false,
      },
    })
    const output = await dbClient.send(command)
    console.log('SUCCESS: Table created:', output)
  } catch (err) {
    console.error('ERROR:', err)
  }
}

createTable()
{{< /code >}}

`DynamoDBClient#send` メソッドの戻り値は `Promise` 型になるので、コマンドの結果を参照する場合は、上記のように `await` で待機する必要があります。

{{< accordion title="実行結果" >}}
{{< code >}}
SUCCESS: Table created: {
  '$metadata': {
    httpStatusCode: 200,
    requestId: '0074dc26-ba2e-4eab-b432-14874d58fd91',
    extendedRequestId: undefined,
    cfId: undefined,
    attempts: 1,
    totalRetryDelay: 0
  },
  TableDescription: {
    ArchivalSummary: undefined,
    AttributeDefinitions: [ [Object], [Object] ],
    BillingModeSummary: undefined,
    CreationDateTime: 2021-02-27T14:44:46.724Z,
    GlobalSecondaryIndexes: undefined,
    GlobalTableVersion: undefined,
    ItemCount: 0,
    KeySchema: [ [Object], [Object] ],
    LatestStreamArn: undefined,
    LatestStreamLabel: undefined,
    LocalSecondaryIndexes: undefined,
    ProvisionedThroughput: {
      LastDecreaseDateTime: 1970-01-01T00:00:00.000Z,
      LastIncreaseDateTime: 1970-01-01T00:00:00.000Z,
      NumberOfDecreasesToday: 0,
      ReadCapacityUnits: 1,
      WriteCapacityUnits: 1
    },
    Replicas: undefined,
    RestoreSummary: undefined,
    SSEDescription: undefined,
    StreamSpecification: undefined,
    TableArn: 'arn:aws:dynamodb:ddblocal:000000000000:table/Games',
    TableId: undefined,
    TableName: 'Games',
    TableSizeBytes: 0,
    TableStatus: 'ACTIVE'
  }
}
{{< /code >}}
{{< /accordion >}}

### 例外

すでに同名のテーブルが存在するときに `CreateTableCommand` を送ると、`ResourceInUseException` という例外が発生します。

- `err.code === 'ResourceInUseException'` ... Table already exists


テーブルを削除する (DeleteTableCommand)
----

DynamoDB から既存のテーブルを削除するには、`DynamoDBClient.send` で [DeleteTableCommand](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-dynamodb/classes/deletetablecommand.html) を送ります。
パラメーターはテーブル名 (`TableName`) だけでいいので簡単です（もちろんテーブルの削除は慎重に行わなければいけませんが）。

{{< code lang="typescript" title="delete-table.ts" >}}
import {DeleteTableCommand, DynamoDBClient} from '@aws-sdk/client-dynamodb'

const dbClient = new DynamoDBClient({
  endpoint: 'http://localhost:8000'
})

// テーブルを削除する
async function deleteTable() {
  try {
    const command = new DeleteTableCommand({
      TableName: 'Games',
    })
    const output = await dbClient.send(command)
    console.log('SUCCESS: Table deleted:', output)
  } catch (err) {
    console.error('ERROR:', err)
  }
}

deleteTable()
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code >}}
SUCCESS: Table deleted: {
  '$metadata': {
    httpStatusCode: 200,
    requestId: '99d6402d-5de7-4b3f-91b6-c11e97aae417',
    extendedRequestId: undefined,
    cfId: undefined,
    attempts: 1,
    totalRetryDelay: 0
  },
  TableDescription: {
    ArchivalSummary: undefined,
    AttributeDefinitions: [ [Object], [Object] ],
    BillingModeSummary: undefined,
    CreationDateTime: 2021-02-27T14:16:30.122Z,
    GlobalSecondaryIndexes: undefined,
    GlobalTableVersion: undefined,
    ItemCount: 0,
    KeySchema: [ [Object], [Object] ],
    LatestStreamArn: undefined,
    LatestStreamLabel: undefined,
    LocalSecondaryIndexes: undefined,
    ProvisionedThroughput: {
      LastDecreaseDateTime: 1970-01-01T00:00:00.000Z,
      LastIncreaseDateTime: 1970-01-01T00:00:00.000Z,
      NumberOfDecreasesToday: 0,
      ReadCapacityUnits: 1,
      WriteCapacityUnits: 1
    },
    Replicas: undefined,
    RestoreSummary: undefined,
    SSEDescription: undefined,
    StreamSpecification: undefined,
    TableArn: 'arn:aws:dynamodb:ddblocal:000000000000:table/Games',
    TableId: undefined,
    TableName: 'Games',
    TableSizeBytes: 0,
    TableStatus: 'ACTIVE'
  }
}
{{< /code >}}
{{< /accordion >}}

### 例外

削除しようとしたテーブルが存在しない場合や、使用中の場合は例外が発生します。

- `err.code === 'ResourceNotFoundException'` ... Table not found
- `err.code === 'ResourceInUseException'` ... Table in use


テーブルの一覧を取得する (ListTablesCommand)
----

DynamoDB に存在するテーブルの一覧を取得するには、`DynamoDBClient#send` で [ListTablesCommand](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-dynamodb/classes/listtablescommand.html) を送ります。
パラメータは空っぽの [ListTablesInput](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-dynamodb/modules/listtablesinput.html) オブジェクトを渡しておけば OK です。
戻り値の [ListTablesCommandOutput](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-dynamodb/modules/listtablesoutput.html) オブジェクトの `TableNames` プロパティに、テーブル名のリストが格納されています。

{{< code lang="typescript" title="list-tables.ts" >}}
import { DynamoDBClient, ListTablesCommand } from '@aws-sdk/client-dynamodb'

const dbClient = new DynamoDBClient({
  endpoint: 'http://localhost:8000',
})

// テーブルの一覧を取得する
async function listTables() {
  try {
    const command = new ListTablesCommand({})
    const output = await dbClient.send(command)
    if (output.TableNames) {
      console.log(output.TableNames.join(', '))
    }
    console.log(output)
  } catch (err) {
    console.error('ERROR', err)
  }
}

listTables()
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code >}}
Games
{
  '$metadata': {
    httpStatusCode: 200,
    requestId: 'f80bd8f7-788f-4f17-b81e-dd6e2f529ebf',
    extendedRequestId: undefined,
    cfId: undefined,
    attempts: 1,
    totalRetryDelay: 0
  },
  LastEvaluatedTableName: undefined,
  TableNames: [ 'Games' ]
}
{{< /code >}}
{{< /accordion >}}


テーブルの詳細情報を取得する (DescribeTableCommand)
----

指定したテーブルの詳細情報（スキーマなど）を調べるには、`DynamoDBClient#send` で [DescribeTableCommand](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-dynamodb/classes/describetablecommand.html) を送ります。
パラメータには `TableName` を指定します。

{{< code lang="typescript" title="describe-table.ts" >}}
import { DescribeTableCommand, DynamoDBClient } from '@aws-sdk/client-dynamodb'

const dbClient = new DynamoDBClient({
  endpoint: 'http://localhost:8000',
})

// テーブルの詳細情報を取得する
async function describeTable() {
  try {
    const command = new DescribeTableCommand({
      TableName: 'Games',
    })
    const output = await dbClient.send(command)
    console.log('SUCCESS (describe table):', JSON.stringify(output, null, 2))
  } catch (err) {
    console.error('ERROR:', err)
  }
}

describeTable()
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code >}}
SUCCESS (describe table): {
  "$metadata": {
    "httpStatusCode": 200,
    "requestId": "b700a435-c7ee-4231-a010-6e05dbda9ab6",
    "attempts": 1,
    "totalRetryDelay": 0
  },
  "Table": {
    "AttributeDefinitions": [
      {
        "AttributeName": "Hardware",
        "AttributeType": "S"
      },
      {
        "AttributeName": "GameId",
        "AttributeType": "S"
      }
    ],
    "CreationDateTime": "2021-02-27T15:07:34.953Z",
    "ItemCount": 0,
    "KeySchema": [
      {
        "AttributeName": "Hardware",
        "KeyType": "HASH"
      },
      {
        "AttributeName": "GameId",
        "KeyType": "RANGE"
      }
    ],
    "ProvisionedThroughput": {
      "LastDecreaseDateTime": "1970-01-01T00:00:00.000Z",
      "LastIncreaseDateTime": "1970-01-01T00:00:00.000Z",
      "NumberOfDecreasesToday": 0,
      "ReadCapacityUnits": 1,
      "WriteCapacityUnits": 1
    },
    "TableArn": "arn:aws:dynamodb:ddblocal:000000000000:table/Games",
    "TableName": "Games",
    "TableSizeBytes": 0,
    "TableStatus": "ACTIVE"
  }
}
{{< /code >}}
{{< /accordion >}}


アイテムを追加する (PutItemCommand)
----

テーブルに新しいアイテムを追加したいとき（あるいは既存のアイテムの属性値を上書きしたいとき）は、`DynamoDBClient#send` メソッドで [PutItemCommand](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-dynamodb/classes/putitemcommand.html) を送ります。

追加するアイテムのプライマリキー属性（パーティションキー、およびソートキー）の指定は必須ですが、その他の属性はオプショナルです。
プライマリキーと一致するアイテムがまだ存在しない場合は、新規アイテムの追加になり、プライマリキーと一致するアイテムがすでに存在する場合は、属性値のリプレースになります（このとき、指定しなかった属性値は消えてしまうことに注意してください）。

{{< code lang="typescript" title="put-item.ts" >}}
import {DynamoDBClient, PutItemCommand} from '@aws-sdk/client-dynamodb'

const dbClient = new DynamoDBClient({
  endpoint: 'http://localhost:8000'
})

// テーブルにアイテムを追加する
async function putItem() {
  try {
    const command = new PutItemCommand({
      TableName: 'Games',
      Item: {
        Hardware: { S: 'SNES' },
        GameId: { S: '1990-SuperMarioWorld' },
        Title: { S: 'Super Mario World' },
        Players: { N: '2' },
        Genre: { S: 'ACT' },
      },
    })
    const output = await dbClient.send(command)
    console.log('SUCCESS (put item):', output)
  } catch (err) {
    console.log('ERROR:', err)
  }
}

putItem()
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code >}}
SUCCESS (put item): {
  '$metadata': {
    httpStatusCode: 200,
    requestId: 'e230fba2-732b-4f44-a0f2-ddca742f63fe',
    extendedRequestId: undefined,
    cfId: undefined,
    attempts: 1,
    totalRetryDelay: 0
  },
  Attributes: undefined,
  ConsumedCapacity: undefined,
  ItemCollectionMetrics: undefined
}
{{< /code >}}
{{< /accordion >}}

### 上書きされる前のアイテムの内容を取得する

`PutItemCommand` コマンドを送信したときに、プライマリキーと一致するアイテムがすでに存在する場合は、新しく指定した属性値でその内容が置き換えられます。
このとき、もとのアイテムがどのような属性を持っていたかを調べたいときは、`PutItemCommand` を生成するときに、__`ReturnValues`__ プロパティを指定します。
コマンド送信の結果の __`Attributes`__ プロパティでもとの属性値を参照できます。

{{< code lang="typescript" >}}
async function putItem() {
  try {
    const command = new PutItemCommand({
      TableName: 'Games',
      Item: {
        Hardware: { S: 'SNES' },
        GameId: { S: '1990-SuperMarioWorld' },
        Title: { S: 'XXXXX' }, // 値を適当に変えてみる
        Players: { N: '2' },
        Genre: { S: 'ACT' },
      },
      ReturnValues: 'ALL_OLD',  // もとの属性値を取得
    });
    const output = await dbClient.send(command);
    console.log('SUCCESS (put item):', output.Attributes);
  } catch (err) {
    console.log('ERROR:', err);
  }
}
{{< /code >}}

{{< code lang="js" title="実行結果" >}}
SUCCESS (put item): {
  Title: { S: 'Super Mario World' },
  Hardware: { S: 'SNES' },
  Genre: { S: 'ACT' },
  GameId: { S: '1990-SuperMarioWorld' },
  Players: { N: '2' }
}
{{< /code >}}

### すでにアイテムが存在する場合に無視する

`PutItemCommand` コマンドはデフォルトではアイテムの内容を上書きしますが、この振る舞いを抑制したいときは、コマンドのパラメータに次のように __`ConditionExpression`__ を指定します。
このようにすると、プライマリキーが一致するアイテムがすでに存在する場合に `ConditionalCheckFailedException` 例外が発生するようになります。

{{< code lang="typescript" >}}
const command = new PutItemCommand({
  TableName: 'Games',
  Item: {
    Hardware: { S: 'SNES' },
    GameId: { S: '1990-SuperMarioWorld' },
    Title: { S: 'XXXXX' }, // 値を適当に変えてみる
    Players: { N: '2' },
    Genre: { S: 'ACT' },
  },
  ConditionExpression: 'attribute_not_exists(Hardware)',
});
{{< /code >}}


アイテムの属性値を部分的に更新する (UpdateItemCommand)
----

### DynamoDBClient クラスを使う方法（複雑）

既存のアイテムの内容（属性値）を部分的に更新したいときは、`DynamoDBClient#send` メソッドで [UpdateItemCommand](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-dynamodb/classes/updateitemcommand.html) を送ります。
`PutItemCommand` でも既存のアイテムを更新できますが、`PutItemCommand` は完全に上書きのため、すべての属性値を指定し直さないといけないません。
一方、`UpdateItemCommand` を使用すると、指定した属性のみを部分的に更新できます。
なお、どちらも指定したアイテム自体が存在しない場合にアイテムが生成されるのは同様です。

`UpdateItemCommand` で属性値を更新するときのパラメータの指定方法は若干複雑で、プレースホルダを使って新しい属性名 (`#xxx`) と属性値 (`:xxx`) を指定する必要があります。
次の例では、既存のアイテムの `Title` 属性の値を更新し、さらに新しい属性 `Maker` を追加しています。

{{< code lang="typescript" title="update-item.ts（DynamoDBClient を使う方法）" >}}
import { DynamoDBClient, UpdateItemCommand } from '@aws-sdk/client-dynamodb'

const dbClient = new DynamoDBClient({
  endpoint: 'http://localhost:8000',
})

// アイテムの属性値を部分的に更新する
async function updateItem() {
  try {
    const command = new UpdateItemCommand({
      TableName: 'Games',
      Key: {
        Hardware: { S: 'SNES' },
        GameId: { S: '1990-SuperMarioWorld' },
      },
      UpdateExpression: 'set Title = :x, #a = :y',
      ExpressionAttributeNames: {
        '#a': 'Maker',
      },
      ExpressionAttributeValues: {
        ':x': { S: 'Mario 4' },
        ':y': { S: 'Nintendo' },
      },
    })
    const output = await dbClient.send(command)
    console.log('SUCCESS (update item):', output)
  } catch (err) {
    console.log('ERROR:', err)
  }
}

updateItem()
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code lang="js" >}}
SUCCESS (update item): {
  '$metadata': {
    httpStatusCode: 200,
    requestId: 'b2bae6de-ceed-42c6-8946-455b75d2472e',
    extendedRequestId: undefined,
    cfId: undefined,
    attempts: 1,
    totalRetryDelay: 0
  },
  Attributes: undefined,
  ConsumedCapacity: undefined,
  ItemCollectionMetrics: undefined
}
{{< /code >}}
{{< /accordion >}}

### DynamoDBDocumentClient クラスを使う方法（簡単）

`DynamoDBClient` をラップする `DynamoDBDocumentClient` を使うと、もう少し簡単にアイテム内容（属性値）の更新を行うことができます。
コマンドとしては、`@aws-sdk/lib-dynamodb` モジュールが提供する [UpdateCommand](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/classes/_aws_sdk_lib_dynamodb.updatecommand-1.html) を使用します（なぜ対称性のある `UpdateItemCommand` という名前にしなかったのかは不明）。

{{< code lang="typescript" title="update-item.ts（DynamoDBDocumentClient を使う方法）" hl_lines="18 19 26 27" >}}
import { DynamoDBClient } from '@aws-sdk/client-dynamodb'
import {
  DynamoDBDocumentClient,
  UpdateCommand,
  UpdateCommandInput,
} from '@aws-sdk/lib-dynamodb'
const dbClient = new DynamoDBClient({
  endpoint: 'http://localhost:8000',
})
const documentClient = DynamoDBDocumentClient.from(dbClient)

// アイテムの属性値を部分的に更新する
async function updateItem() {
  try {
    const command = new UpdateCommand({
      TableName: 'Games',
      Key: {
        Hardware: 'SNES', // Partition key
        GameId: '1990-SuperMarioWorld', // Sort key
      },
      UpdateExpression: 'set Title = :x, #a = :y',
      ExpressionAttributeNames: {
        '#a': 'Maker',
      },
      ExpressionAttributeValues: {
        ':x': 'Mario 4',
        ':y': 'Nintendo',
      },
    } as UpdateCommandInput)
    const output = await documentClient.send(command)
    console.log('SUCCESS (update item):', output)
  } catch (err) {
    console.log('ERROR:', err)
  }
}

updateItem()
{{< /code >}}

プライマリキーの指定 (`Key`) や、新しい属性値の指定 (`ExpressionAttributeValues`) において、各値の型（`S` など）を指定する必要がなくなり、コードがスッキリしました！（というか標準のクラスの方でこうなっているべきな気がしますけど...）。


アイテムを取得する (GetItemCommand, GetCommand)
----

### DynamoDBClient を使う方法（複雑）

DynamoDB のテーブルから既存のアイテムを 1 つ取得するときは、`DynamoDBClient#send` メソッドで [GetItemCommand](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-dynamodb/classes/getitemcommand.html) を送ります。
パラメータには、テーブル名 (`Table`) と、アイテムを特定するためのプライマリキー情報 (`Key`) を指定します。

{{< code lang="typescript" title="get-item.ts" >}}
import { DynamoDBClient, GetItemCommand } from '@aws-sdk/client-dynamodb'

const dbClient = new DynamoDBClient({
  endpoint: 'http://localhost:8000'
})

// アイテムを取得する
async function getItem() {
  try {
    const command = new GetItemCommand({
      TableName: 'Games',
      Key: {
        Hardware: {S: 'SNES'},
        GameId: {S: '1990-SuperMarioWorld'},
      }
    })
    const output = await dbClient.send(command)
    console.log('SUCCESS (get item):', output)
  } catch (err) {
    console.log('ERROR:', err)
  }
}

getItem();
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code lang="js" >}}
SUCCESS (get item): {
  '$metadata': {
    httpStatusCode: 200,
    requestId: '162f4a24-8b5c-4174-a83d-c581df44ca80',
    extendedRequestId: undefined,
    cfId: undefined,
    attempts: 1,
    totalRetryDelay: 0
  },
  ConsumedCapacity: undefined,
  Item: {
    Title: { S: 'Super Mario World' },
    Hardware: { S: 'SNES' },
    Genre: { S: 'ACT' },
    GameId: { S: '1990-SuperMarioWorld' },
    Players: { N: '2' }
  }
}
{{< /code >}}
{{< /accordion >}}

指定したプライマリキーに一致するアイテムが見つからないときは、`Item` プロパティの値が `undefined` になります。

### DynamoDBDocumentClient を使う方法（簡単）

`DynamoDBDocumentClient` を使うと、プライマリキーや戻り値の値の型指定（`S` など）を省略することができます。
DynamoDB テーブルから 1 つのアイテムを取得するには、`DynamoDBDocumentClient#send` メソッドで [GetCommand](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/classes/_aws_sdk_lib_dynamodb.getcommand-1.html) を送ります。

{{< code lang="ts" title="get-item.ts" hl_lines="14 15" >}}
import { DynamoDBClient } from '@aws-sdk/client-dynamodb'
import { DynamoDBDocumentClient, GetCommand } from '@aws-sdk/lib-dynamodb'

const dbClient = new DynamoDBClient({
  endpoint: 'http://localhost:8000'
})
const documentClient = DynamoDBDocumentClient.from(dbClient)

async function getItem() {
  try {
    const command = new GetCommand({
      TableName: 'Games',
      Key: {
        Hardware: 'SNES',
        GameId: '1990-SuperMarioWorld',
      }
    })
    const output = await documentClient.send(command)
    console.log('SUCCESS (get item):', output)
  } catch (err) {
    console.log('ERROR:', err)
  }
}
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code lang="js" hl_lines="12-16" >}}
SUCCESS (get item): {
  '$metadata': {
    httpStatusCode: 200,
    requestId: '162f4a24-8b5c-4174-a83d-c581df44ca80',
    extendedRequestId: undefined,
    cfId: undefined,
    attempts: 1,
    totalRetryDelay: 0
  },
  ConsumedCapacity: undefined,
  Item: {
    Title: 'Super Mario World',
    Hardware: 'SNES',
    Genre: 'ACT',
    GameId: '1990-SuperMarioWorld',
    Players: 2
  }
}
{{< /code >}}
{{< /accordion >}}


アイテムを削除する (DeleteItemCommand)
----

DynamoDB のテーブルからアイテムを削除するには、`DynamoDBClient#send` メソッドで [DeleteItemCommand](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-dynamodb/classes/deleteitemcommand.html) を送ります。
コマンドのパラメータには、テーブル名 (`Table`) と、削除対象のアイテムを特定するためのプライマリキー情報 (`Key`) を渡す必要があります。
複合プライマリキーを使用している場合は、パーティションキーとソートキーの両方を指定する必要があります。

{{< code lang="typescript" title="delete-item.ts" >}}
import {DeleteItemCommand, DynamoDBClient} from '@aws-sdk/client-dynamodb';

const dbClient = new DynamoDBClient({
  endpoint: 'http://localhost:8000'
});

// アイテムを削除する
async function deleteItem() {
  try {
    const command = new DeleteItemCommand({
      TableName: 'Games',
      Key: {
        Hardware: {S: 'SNES'},
        GameId: {S: '1990-SuperMarioWorld'},
      }
    });
    const output = await dbClient.send(command);
    console.log('SUCCESS (delete item):', output);
  } catch (err) {
    console.log('ERROR:', err);
  }
}

deleteItem();
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code lang="js" >}}
SUCCESS (delete item): {
  '$metadata': {
    httpStatusCode: 200,
    requestId: 'cdfcb727-b39e-413a-a643-c3f6a11b95c0',
    extendedRequestId: undefined,
    cfId: undefined,
    attempts: 1,
    totalRetryDelay: 0
  },
  Attributes: undefined,
  ConsumedCapacity: undefined,
  ItemCollectionMetrics: undefined
}
{{< /code >}}
{{< /accordion >}}

指定したプライマリキーに一致するアイテムが存在しない場合でも、特に例外が発生するということはないようです。


テーブル内のアイテムをすべて取得する (ScanCommand)
----

### DynamoDBClient を使う方法（複雑）

DynamoDB のテーブルからすべてのアイテムを取得（＝スキャン）するには、`DynamoDBClient#send` メソッドで [ScanCommand](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-dynamodb/classes/scancommand.html) を送ります。
コマンドのパラメータには、テーブル名 (`Table`) を指定します。

上記ドキュメントサイトにも記載がありますが、1 度のスキャンで取得できる最大データサイズは 1MB です。
それを超える場合は、レスポンスに含まれている `LastEvaluatedKey` を使って続きのデータを要求できます（ページング処理）。
最初から何件ずつアイテムを取得すべきかが決まっている（例えば、テーブルに 10 件ずつ表示する）のであれば、`ScanCommand` の `Limit` プロパティにその数を指定できます。

{{< code lang="typescript" title="scan.ts（DynamoDBClient を使う場合）" >}}
import { DynamoDBClient, ScanCommand } from '@aws-sdk/client-dynamodb'
const dbClient = new DynamoDBClient({...})

// 全てのアイテムをスキャンする
async function scan() {
  try {
    const command = new ScanCommand({
      TableName: 'Games',
      // Limit: 10,
    })
    const output = await dbClient.send(command)
    console.log(JSON.stringify(output, null, 2))
  } catch (err) {
    console.log('ERROR:', err)
  }
}

scan()
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code lang="js" >}}
{
  "$metadata": {
    "httpStatusCode": 200,
    "requestId": "V4KQNSO5AEMVJF66Q9ASUAAJGJ3B11BRMBKKTVASP614LRKL6D7V",
    "attempts": 1,
    "totalRetryDelay": 0
  },
  "Count": 2,
  "Items": [
    {
      "Title": {
        "S": "Super Mario World"
      },
      "Hardware": {
        "S": "SNES"
      }
    },
    {
      "Title":  {
        "S": "Dr. Mario"
      },
      "Hardware": {
        "S": "NES"
      }
    }
  ],
  "ScannedCount": 2
}
{{< /code >}}
{{< /accordion >}}

### DynamoDBDocumentClient を使う方法（簡単）

上記の実行結果を見ると分かるように、`DynamoDBClient#scan(ScanCommand)` で返される JSON データは扱いやすい形式になっていません（いちいち `S` とか `N` とかフィールドタイプが入る）。
そこで、ドキュメントクライアント (`DynamoDBDocumentClient`) の出番です。
`DynamoDBDocumentClient` は `DynamoDBClient` を扱いやすくするためのクラスで、これを使うと、テーブルのスキャン結果を扱いやすい JavaScript オブジェクトとして取得することができます。

下記は、`DynamoDBDocumentClient` を使ったテーブルスキャン（全アイテム取得）の例です。
`ScanCommand` は `@aws-sdk/client-dynamodb` モジュールが提供するものではなく、`@aws-sdk/lib-dynamodb` が提供するものを使う必要があります。

{{< code lang="typescript" title="scan.ts（DynamoDBDocumentClient を使う場合）" >}}
import { DynamoDBClient } from '@aws-sdk/client-dynamodb'
import {
  DynamoDBDocumentClient,
  ScanCommand,
  ScanCommandInput,
  ScanCommandOutput,
} from '@aws-sdk/lib-dynamodb'

const dbClient = new DynamoDBClient({
  endpoint: 'http://localhost:8000',
})
const documentClient = DynamoDBDocumentClient.from(dbClient)

async function scan() {
  try {
    const command = new ScanCommand({
      TableName: 'Games',
    } as ScanCommandInput)
    const output: ScanCommandOutput = await documentClient.send(command)
    console.log(JSON.stringify(output, null, 2))
  } catch (err) {
    console.log('ERROR:', err)
  }
}

scan()
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code lang="js" >}}
{
  "$metadata": {
    "httpStatusCode": 200,
    "requestId": "V4KQNSO5AEMVJF66Q9ASUAAJGM9T8IOD909C85GIL0MDN8F27NFV",
    "attempts": 1,
    "totalRetryDelay": 0
  },
  "Count": 2,
  "Items": [
    {
      "Title": "Super Mario World",
      "Hardware": "SNES"
    },
    {
      "Title": "Dr. Mario",
      "Hardware": "NES"
    }
  ],
  "ScannedCount": 2
}
{{< /code >}}
{{< /accordion >}}

こちらの実行結果を見ると、`Items` プロパティの下に、JavaScript オブジェクトの配列が直感的な形式で格納されています。
このプロパティ値に TypeScript の型情報を付けてやれば、あとは簡単にデータ参照できます。

{{< code lang="typescript" >}}
const games = output.Items as Game[]
{{< /code >}}

上記で実装した `scan()` 関数を外から呼び出したい場合は、次のような感じで `Promise` オブジェクトを返すように実装します。

{{< code lang="typescript" >}}
export async function getGames(): Promise<Game[]> {
  const command = new ScanCommand({
    TableName: 'Games',
  } as ScanCommandInput)
  const output: ScanCommandOutput = await documentClient.send(command)
  return output.Items as Game[]
}
{{< /code >}}

