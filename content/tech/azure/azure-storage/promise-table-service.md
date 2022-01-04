---
title: "Azure Table Stroage を使ってみる: TableService を Promise 化して使いやすくする"
url: "/p/4m96s2r"
date: "2020-06-04"
tags: ["Azure Table Storage", "Node.js", "TypeScript"]
---

PromiseTableService クラスの概要
----

Node.js から Azure Table Storage を操作する場合は、`azure-storage` パッケージの [TableService クラス](https://azure.github.io/azure-storage-node/TableService.html) を使用するのですが、このクラスは残念ながら [Promise 対応](https://maku77.github.io/js/async/promise.html) 対応されておらず、旧式のコールバック形式での呼び出しが強制されます。

下記の `azure-table-promise` パッケージが提供している __`PromiseTableService`__ クラスを使用すると、`TableService` を `Promise` 化して使用することができます。

- [azure-table-promise - npm パッケージ](https://www.npmjs.com/package/azure-table-promise)

ちなみに、下記の Issue で公式パッケージの `Promise` 化の議論がされているのですが、`TableService` クラスはいまだに対応されてませんね（2020年6月現在）。

- 参考: [Promise support · Issue #110 · Azure/azure-storage-node](https://github.com/azure/azure-storage-node/issues/110)

こういった対応は本家の方でサクッとやってくれれば 3rd パーティライブラリの乱立が防げるんですけどね。。。


PromiseTableService を使ってみる
----

まず必要なモジュールをインストールします。
`azure-storage` は本家 Microsoft の `TableService` クラスを使うためのモジュールで、`azure-table-promise` がそれを `Promise` ラップするためのモジュールです。
ここでは TypeScript を使うので、Node.js 型定義もインストールしておきます。

{{< code title="npm モジュールのインストール" >}}
$ npm install --save-dev @types/node
$ npm install --save azure-storage
$ npm install --save azure-table-promise
{{< /code >}}

次の `MyTableStorage` クラスは、`PromiseTableService` を使って TableStroage から情報を取得するサンプルです。
コンストラクタで `PromiseTableService` インスタンスを生成し、`getRandomMessage()` メソッドで、`randommessage` テーブルの値をランダムに取得しています。

{{< code lang="typescript" title="myTableStorage.ts" >}}
import * as azure from 'azure-storage';
import { PromiseTableService } from 'azure-table-promise';

class MyTableStorage {
  private tableService: azure.TableService;
  private promiseTableService: PromiseTableService;
  private static TABLE_RANDOMMESSAGE = 'randommessage'; // サンプルテーブル名

  constructor() {
    // Azure Storage の接続文字列を環境変数から取得
    const connectionString = process.env.AZURE_STORAGE_CONNECTION_STRING;
    if (typeof connectionString === 'undefined') {
      console.error('AZURE_STORAGE_CONNECTION_STRING is not set');
      process.exit(1);
    }
    this.tableService = new azure.TableService(connectionString);
    this.promiseTableService = new PromiseTableService(this.tableService);
  }

  /**
   * randommessage テーブルからランダムに RowKey カラムの値を取得します。
   * テーブルが空のときは空文字列を返します。
   */
  async getRandomMessage(): Promise<string> {
    try {
      const result = await this.promiseTableService.queryEntities<any>(
        MyTableStorage.TABLE_RANDOMMESSAGE, null as any, null as any);
      const size = result.entries.length;
      if (size == 0) return '';
      const index = Math.floor(Math.random() * size);
      return result.entries[index].RowKey._;
    } catch (err) {
      console.error(err);
      throw new Error(
        'queryEntities failed - ' + MyTableStorage.TABLE_RANDOMMESSAGE);
    }
  }
}
{{< /code >}}


`PromiseTableService` が提供するメソッド名は、`TableService` が提供するメソッド名と同じになっているので、直感的に `Promise` バージョンのメソッドを使用することができます。
上記の例では、`TableService.queryEntities()` の代わりに `PromiseTableService.queryEntities()` を使用しています。
違いは、最後のパラメーターでコールバック関数を渡すのではなく、戻り値で `Promise` オブジェクトを受け取るという点です。
ECMAScript 2017 の `async/await` キーワードを使えば、上記のように同期呼び出しを行えるため、処理の流れが分かりやすくなります。

上記の `MyTableStorage` の使い方はこんな感じです。

{{< code lang="typescript" >}}
async function main() {
    const table = new MyTableStorage()
    try {
        const message = await table.getRandomMessage()
        console.log(message);
    } catch (err) {
        console.error(err);
    }
}

main();
{{< /code >}}

ちなみに、`PromiseTableService` は独自の拡張として __`queryEntitiesAll`__ というメソッドも用意していて、これを使うと 1000 レコードの壁を越えて一度に大量の結果を取得することができます（通常は continuation token を使って、繰り返し `queryEntities` を呼び出す必要があります）。

