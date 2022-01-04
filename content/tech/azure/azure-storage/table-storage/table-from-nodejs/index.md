---
title: "Azure Table Strage を使ってみる (4) Node.js からテーブル操作してみる"
url: "/p/ccoonon"
date: "2020-01-31"
tags: ["Azure Table Storage", "Node.js"]
weight: 104
---

azure-storage パッケージのインストール
----

Node.js から Azure Table Storage を操作するには、[`azure-storage`](https://www.npmjs.com/package/azure-storage) というライブラリを使用します。
`npm` コマンドで簡単にインストールすることができます。

{{< code title="azure-storage のインストール" >}}
$ npm install --save azure-storage
{{< /code >}}

- 参考: [npm によるパッケージのインストール | Node.js ノート](https://maku77.github.io/nodejs/npm/install-package.html)


TableService オブジェクトの生成
----

Azure Storage にアクセスするには、接続情報（ストレージアカウント名とキー）が必要になるので、[Azure ポータル](https://portal.azure.com/) で確認しておいてください。

- 参考: [Azure Storage の接続情報（キー）を確認する](/p/dofzeua)

### 接続情報としてデフォルトの環境変数を使用する

Node.js から Table Storage を扱うには、`azure-storage` モジュールが提供する [TableService クラス](https://azure.github.io/azure-storage-node/TableService.html) を使用します。
`TableService` のインスタンスは下記のように生成することができます。

{{< code lang="js" >}}
const azure = require('azure-storage');

const tableService = new azure.TableService();
{{< /code >}}

上記のように、`TableService` のコンストラクのパラメータを何も指定しないと、接続のために下記のような環境変数が参照されます。

1. {{< label "AZURE_STORAGE_ACCOUNT" >}} ... Azure Storage の「ストレージアカウント名」
2. {{< label "AZURE_STORAGE_ACCESS_KEY" >}} ... Azure Storage の「キー」
3. {{< label "AZURE_STORAGE_CONNECTION_STRING" >}} ... Azure Storage の「接続文字列」

1 と 2 を両方とも設定するか、3 を設定しておけば Azure Storage にアクセスできるようになります。

### 接続情報を明示的に指定する

`TableService` のコンストラクタの引数で、明示的に接続情報を渡すこともできます。

{{< code lang="js" >}}
// 接続文字列だけを渡す場合
const tableService = new azure.TableService(connectionString);

// ストレージアカウント名とキーのペアを渡す場合
const tableService = new azure.TableService(storageAccount, storageAccessKey);
{{< /code >}}

複数のストレージアカウントを使い分けたい場合は、**独自の環境変数に「接続文字列」を設定しておく**とよいでしょう。
ここでは、`MAKU_AZURE_STORAGE_CONNECTION_STRING` という環境変数に「接続文字列」を設定してみます。

{{< image src="img-env-var.png" >}}

{{< code lang="js" >}}
const azure = require('azure-storage');

// Azure Storage の接続文字列を環境変数から取得
const connectionString = process.env.MAKU_AZURE_STORAGE_CONNECTION_STRING;
if (typeof connectionString === 'undefined') {
  console.error('MAKU_AZURE_STORAGE_CONNECTION_STRING is not set');
  process.exit(1);
}

// TableService オブジェクトを取得
const tableService = new azure.TableService(connectionString);
{{< /code >}}

### プロキシ環境からアクセスする場合

`TableService` クラスをプロキシ環境内で使用する場合は、OS の **`https_proxy`** 環境変数で設定してしまうのが簡単です。
OS の `https_proxy` 環境変数を設定するのを避けたいときは、`TableService` クラスの `setProxy()` メソッドを使ってプロキシを設定することもできます。

{{< code lang="js" title="プロキシ情報をハードコードする" >}}
tableService.setProxy('http://proxy.example.com:12345');
{{< /code >}}

プロキシ情報は環境変数で定義したいけど、OS の `https_proxy` 環境変数を設定するのは避けたいということであれば、例えば、独自の `MAKU_AZURE_PROXY` のような **独自の環境変数** を用意するとよいでしょう。

{{< code lang="js" >}}
// MAKU_AZURE_PROXY 環境変数が設定されていたらプロキシ情報として使う
if (process.env.MAKU_AZURE_PROXY) {
  tableService.setProxy(process.env.MAKU_AZURE_PROXY);
}
{{< /code >}}


テーブルの操作
----

ここから先のテーブル操作に関しては、前述の `TableService` オブジェクトの生成が終わっていることを想定しています。

### テーブルを作成する (createTable, createTableIfNotExists)

Table Storage 上に、新しいテーブルを作成するには、`TableService` クラスの下記のメソッドを使用します。

- [createTable() メソッド](https://azure.github.io/azure-storage-node/TableService.html#createTable__anchor)
- [createTableIfNotExists() メソッド](https://azure.github.io/azure-storage-node/TableService.html#createTableIfNotExists__anchor)

{{< code lang="js" title="mytable テーブルを作成する" >}}
tableService.createTableIfNotExists('mytable', function(error, result, response) {
  if (error) {
    console.error(error);
    process.exit(1);
  }

  // result contains true if created; false if already exists
  console.log(result);
});
{{< /code >}}

{{< code lang="js" title="実行結果（テーブルが作成された場合）" >}}
{
  isSuccessful: true,
  statusCode: 204,
  TableName: 'mytable',
  created: true
}
{{< /code >}}

{{< code lang="js" title="実行結果（テーブルが既に存在していた場合）" >}}
{
  isSuccessful: true,
  statusCode: 200,
  TableName: 'mytable',
  created: false
}
{{< /code >}}


エンティティの操作
----

### エンティティを取得する (queryEntities)

条件を指定してエンティティのリストを取得するには、`TableService` クラスの [queryEntities() メソッド](https://azure.github.io/azure-storage-node/TableService.html#queryEntities__anchor) を使用します。

{{< code lang="js" title="books テーブルのエンティティをすべて取得" >}}
tableService.queryEntities('books', null, null, function (error, result) {
  if (error) {
    console.error(error);
    process.exit(1);
  }

  const entries = result.entries;
  console.log(entries);
});
{{< /code >}}

{{< code lang="js" title="実行結果" >}}
[
  {
    PartitionKey: { '$': 'Edm.String', _: 'book' },
    RowKey: { '$': 'Edm.String', _: '001' },
    Timestamp: { '$': 'Edm.DateTime', _: 2020-01-29T02:28:15.137Z },
    Title: { _: 'まくの秘密' },
    Author: { _: 'まく' },
    '.metadata': { etag: `W/"datetime'2020-01-29T02%3A28%3A15.1370872Z'"` }
  },
  {
    PartitionKey: { '$': 'Edm.String', _: 'book' },
    RowKey: { '$': 'Edm.String', _: '002' },
    Timestamp: { '$': 'Edm.DateTime', _: 2020-01-29T02:29:48.875Z },
    Title: { _: 'へむの秘密' },
    Author: { _: 'へむ' },
    '.metadata': { etag: `W/"datetime'2020-01-29T02%3A29%3A48.8751116Z'"` }
  },
  {
    PartitionKey: { '$': 'Edm.String', _: 'book' },
    RowKey: { '$': 'Edm.String', _: '003' },
    Timestamp: { '$': 'Edm.DateTime', _: 2020-01-29T02:30:28.677Z },
    Title: { _: 'ちいの秘密' },
    Author: { _: 'ちい' },
    '.metadata': { etag: `W/"datetime'2020-01-29T02%3A30%3A28.6771054Z'"` }
  }
]
{{< /code >}}

`queryEntities()` メソッドの第2パラメータに [TableQuery オブジェクト](https://azure.github.io/azure-storage-node/TableQuery.html) を渡すと、エンティティを検索するための条件を指定することができます。

{{< code lang="js" >}}
const query = new azure.TableQuery()
  .top(3)  // 最大 3 件まで取得
  .where('PartitionKey eq ?', 'book')  // PartitionKey が book である
  .and('RowKey >= ?', '2')  // かつ RowKey が 002 以上である
  .select('Title')  // Title プロパティのみを取得

tableService.queryEntities('books', query, null, function (error, result) {
  if (error) {
    console.error(error);
    process.exit(1);
  }

  const entries = result.entries;
  console.log(entries);
});
{{< /code >}}

{{< code lang="js" title="実行結果" >}}
[
  {
    Title: { _: 'へむの秘密' },
    '.metadata': { etag: `W/"datetime'2020-01-29T02%3A29%3A48.8751116Z'"` }
  },
  {
    Title: { _: 'ちいの秘密' },
    '.metadata': { etag: `W/"datetime'2020-01-29T02%3A30%3A28.6771054Z'"` }
  }
]
{{< /code >}}

