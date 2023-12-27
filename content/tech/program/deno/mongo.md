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

実運用を考えると、__`MONGODB_URI_MYAPP`__ のような環境変数で接続先の MongoDB サーバー（およびユーザー ID とパスワード）を指定できるようにしておいた方がよいでしょう。
例えば、MongoDB Atlas サービスを使用している場合は、次のような構成の接続文字列 (SRV URI) が発行されますが、ここには接続パスワードなどが含まれるので、この URI をハードコーディングすることはできません。

{{< code title="MongoDB 接続文字列 (SRV URI) の基本構成" >}}
mongodb+srv://<user>:<password>@<cluster-url>/
{{< /code >}}

{{% note title="SRV URI で authentication failed になる場合" %}}
SRV URI を使って MongoDB サーバーにアクセスしようとしたときに認証系のエラーが出る場合は、まずはユーザー ID とパスワードを確認してください。
正しく指定しているのにも関わらずエラーが出る場合は、使用する認証メカニズムの食い違いが発生している可能性があります。
[MongoDB はいくつかの認証メカニズム](https://www.mongodb.com/docs/drivers/node/current/fundamentals/authentication/mechanisms/) を備えており、MongoDB のバージョンによってどれを使うかが異なっています。
URL の末尾のクエリパラメーターで、__`?authMechanism=SCRAM-SHA-1`__ のように認証メカニズムを指定できます。
{{% /note %}}

次の `client.ts` モジュールは、`MongoClient` インスタンスを `MONGODB_URI_MYAPP` 環境変数が示す MongoDB サーバーに接続し、`export` しています。

{{< code lang="ts" title="client.ts" >}}
import { MongoClient } from "https://deno.land/x/mongo@v0.31.1/mod.ts";

const uri = Deno.env.get("MONGODB_URI_MYAPP");
if (!uri) {
  console.error("ERROR: MONGODB_URI_MYAPP environment variable is not set.");
  Deno.exit(1);
}
// あるいは環境変数がセットされていない場合に代替の URI をセットする
// const uri = Deno.env.get("MONGODB_URI_MYAPP") ?? "mongodb://127.0.0.1:27017";

export const client = new MongoClient();
await client.connect(uri);
{{< /code >}}

メインモジュールから次のようにインポートすることで、簡単に `MongoClient` インスタンスを使用できます。

{{< code lang="ts" title="main.ts" >}}
import { client } from "./client.ts";

// あとは client を使っていろいろな DB 操作
console.log(await client.listDatabases());
{{< /code >}}

__`deno run`__ でプログラムを実行するときは、`--allow-env` による環境変数へのアクセスと、`--allow-net` によるネットワークへのアクセスの許可が必要です。

```console
$ deno run --allow-env --allow-net main.ts
[
  { name: "admin", sizeOnDisk: 40960, empty: false },
  { name: "config", sizeOnDisk: 61440, empty: false },
  { name: "local", sizeOnDisk: 40960, empty: false }
]
```

次のように Deno のコンフィグファイルを作成しておけば、__`deno task main`__ とするだけで実行できるようになります。

{{< code lang="json" title="deno.jsonc" >}}
{
  "tasks": {
    "main": "deno run --allow-env --allow-net main.ts"
  }
}
{{< /code >}}


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


関連リンク
----

- [Golang で MongoDB を扱う (go.mongodb.org/mongo-driver)](https://maku77.github.io/p/uft7jv9/)
- [Rust で MongoDB を扱う (mongodb)](https://maku77.github.io/p/nzi6xbm/)

