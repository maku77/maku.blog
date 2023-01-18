---
title: "Deno で MongoDB にアクセスする"
url: "p/3pwojuj/"
date: "2023-01-18"
tags: ["Deno", "MongoDB"]
---

Deno 用の MongoDB ドライバーとして、__deno_mongo__ が公開されています。
[Deno Deploy](/p/phz7fo3/) 上でも使えるので、Deno Deploy で公開する Web API から MongoDB Atlas に接続する、といったことが簡単に行えます。

- [mongo | Deno](https://deno.land/x/mongo)
- [GitHub - denodrivers/mongo: MongoDB driver for Deno](https://github.com/denodrivers/mongo)


MongoDB サーバーへ接続する
----

ローカルホスト上で起動した MongoDB サーバーへ接続するには次のようにします。

```ts
import { MongoClient } from "https://deno.land/x/mongo@v0.31.1/mod.ts";

const client = new MongoClient();
await client.connect("mongodb://127.0.0.1:27017");

// 後は client インスタンスを使って MongoDB を操作する
```

実運用を考えると、__`MONGO_URI`__ のような環境変数で接続先の MongoDB サーバーを指定できるようにしておいた方がよいでしょう。
例えば、MongoDB Atlas サービスを使用している場合は、次のような接続文字列 (SRV URI) が発行されますが、ここには接続パスワードなどが含まれるので、この URI をハードコーディングすることはできません。

```
mongodb+srv://<user>:<password>@cluster-name.abcde.mongodb.net/?retryWrites=true&w=majority
```

次のモジュールは、`MongoClient` インスタンスを `MONGO_URI` 環境変数が示す MongoDB サーバーに接続し、`export` しています。
`MONGO_URI` 環境変数がセットされていない場合は、代わりに `127.0.0.1:27017` へ接続するようにしています。

{{< code lang="ts" title="client.ts" >}}
import { MongoClient } from "https://deno.land/x/mongo@v0.31.1/mod.ts";

const uri = Deno.env.get("MONGO_URI") ?? "mongodb://127.0.0.1:27017";
export const client = new MongoClient();
await client.connect(uri);
{{< /code >}}

このモジュールを使えば、メインモジュールから次のように簡単に `MongoClient` インスタンスを使用できます。

{{< code lang="ts" title="main.ts" >}}
import { client } from "./client.ts";

// あとは client を使っていろいろな DB 操作
console.log(await client.listDatabases());
{{< /code >}}

__`deno run`__ でプログラムを実行するときは、`--allow-env` による環境変数へのアクセス、`--allow-net` によるネットワークへのアクセスの許可が必要です。

```console
$ deno run --allow-env --allow-net main.ts
[
  { name: "admin", sizeOnDisk: 40960, empty: false },
  { name: "config", sizeOnDisk: 61440, empty: false },
  { name: "local", sizeOnDisk: 40960, empty: false }
]
```


コレクションを参照する
----

MongoDB データベース内のコレクションを参照するときは、そこに含まれるドキュメントの型をあらかじめ定義しておくことができます。
次の例では、書籍データ（`books` コレクション）のスキーマ定義を行っています。

{{< code lang="ts" title="main.ts" >}}
import { ObjectId } from "https://deno.land/x/mongo@v0.31.1/mod.ts";
import { client } from "./client.ts";

// books コレクションのスキーマ定義（含まれるドキュメントの型）
interface BookSchema {
  _id: ObjectId;
  title: string;
  author: string;
}

// mydb データベースの books コレクションを参照する
const db = client.database("mydb");
const booksCollection = db.collection<BookSchema>("books");
{{< /code >}}

このようにして `Collection` インスタンスを取得したら、あとは自由にドキュメントの追加や検索を行えます。


コレクション内のドキュメントを操作する
----

### ドキュメントを追加する (insertOne, insertMany)

ドキュメントを追加するには、`Collection` クラスの __`insertOne`__ や __`insertMany`__ メソッドを使用します。

```ts
booksCollection.insertMany([
  { title: "Title-1", author: "Author-1" },
  { title: "Title-2", author: "Author-2" },
  { title: "Title-3", author: "Author-3" },
]);
```

### ドキュメントを検索する (find, findOne)

ドキュメントを取得するには、`Collection` クラスの __`find`__ メソッドや __`findOne`__ メソッドを使用します。

{{< code lang="ts" title="すべてのドキュメントを取得" >}}
const books: BookSchema[] = await booksCollection.find({}).toArray();
{{< /code >}}

{{< code lang="ts" title="title フィールドに特定の文字列が含まれているものを検索" >}}
const books: BookSchema[] = await booksCollection
  .find({ title: { $regex: "itle-3" } })
  .toArray();
{{< /code >}}

その他のメソッドは、[公式サイト](https://deno.land/x/mongo) を参考にしてください。

