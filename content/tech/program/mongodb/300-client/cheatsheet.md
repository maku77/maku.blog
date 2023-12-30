---
title: "MongoDB と mongosh のチートシート"
url: "p/j4c4vnq/"
date: "2023-12-28"
tags: ["MongoDB"]
---

全般（mongosh で接続した後のコマンド）
----

| コマンド | 説明 |
| ---- | ---- |
| `db` | 現在のデータベース名を表示 |
| `use <db>` | 現在のデータベースを切り替える |
| `show databases (dbs)` | データベースの一覧を表示 |
| `show collections (tables)` | コレクションの一覧を表示 |
| `show users` | ユーザーの一覧を表示 |
| `show log` | ログの出力 |
| `db.stats()` | データベースの統計情報を表示 |
| `db.<coll>.stats()` | コレクションの統計情報を表示 |
| `db.createCollection("<coll>")` | コレクションを作成（オプション指定付きで作成したいとき）|
| `db.getCollectionInfos()` | コレクション情報を JS 配列として取得 |
| `db.getCollectionNames()` | コレクション名の一覧を JS 配列として取得 |


ドキュメントの検索 (Find)
----

### find の基本

コレクション内のドキュメントを検索するには、__`find()`__ メソッドの第 1 引数に検索フィルターを指定します。

{{< code lang="ts" title="すべてのドキュメントを取得する" >}}
db.<coll>.find()
db.<coll>.find({})
{{< /code >}}

{{< code lang="ts" title="name フィールドの値が value に一致するドキュメントを検索する" >}}
db.<coll>.find({ name: "value" })
{{< /code >}}

{{< code lang="ts" title="awards.wins フィールドが 100 より大きいドキュメントを検索する" >}}
db.<coll>.find({ "awards.wins": { $gt: 100 } })
{{< /code >}}

{{< code lang="ts" title="languages フィールドに jp あるいは en を含むドキュメントを検索する" >}}
db.<coll>.find({ languages: { $in: ["jp", "en"] } })
{{< /code >}}

`find()` の第 2 引数 (project fields) では、取得するフィールドを指定できます。
基本的には取得したいフィールドに `1` を指定すれば OK ですが、`_id` フィールドだけはデフォルトで取得するようになっているので、`_id` フィールドが必要ない場合は明示的に `0` を指定する必要があります。

{{< code lang="ts" title="title フィールドと genres フィールドのみ取得する" >}}
db.<coll>.find({}, { _id: 0, title: 1, genres: 1 })
{{< /code >}}

`find()` メソッドには、他にもさまざまな検索条件を指定できます。

### フィルターの指定方法 `find()`

| フィルター | 説明 |
| ---- | ---- |
| `{ age: 10 }` | `age` フィールド（数値）が `10` に完全一致 |
| `{ age: { $gt: 10 } }` | `age` フィールド（数値）が `10` より大きい |
| `{ age: { $gte: 10 } }` | `age` フィールド（数値）が `10` 以上 |
| `{ age: { $lt: 20 } }` | `age` フィールド（数値）が `20` 未満 |
| `{ age: { $lte: 20 } }` | `age` フィールド（数値）が `20` 以下 |
| `{ age: { $gte: 10, $lte: 20 } }` | `age` フィールド（数値）が `10` 以上 `20` 以下 |
| `{ created: { $lt: ISODate("2000-01-01") } }` | `created` フィールド（日時）が 2000年1月1日 より前 |
| `{ name: "ほげ" }` | `name` フィールド（文字列）が `ほげ` に完全一致 |
| `{ name: /ほげ/ }`<br>`{ name: { $regex: "ほげ" } }` | `name` フィールド（文字列）に `ほげ` が含まれる |
| `{ name: /^ほげ/ }`<br>`{ name: { $regex: "^ほげ" } }` | `name` フィールド（文字列）が `ほげ` で始まっている |
| `{ name: /ほげ$/ }`<br>`{ name: { $regex: "ほげ$" } }` | `name` フィールド（文字列）が `ほげ` で終わっている |
| `{ genre: { $in: ["ACT", RPG"] } }` | `genre` フィールド（文字列）が `ACT` か `RPG` のいずれか |
| `{ genres: { $in: ["ACT", "RPG"] } }` | `genres` フィールド（配列）に少なくとも `ACT` か `RPG` が含まれる |
| `{ genres: { $in: ["ACT", "RPG"] } }` | `genres` フィールド（配列）に少なくとも `ACT` か `RPG` が含まれる |
| `{ genres: { $all: ["ACT", "RPG"] } }` | `genres` フィールド（配列）に `ACT` と `RPG` の両方が含まれる |
| `{ genres: { $size: 2 } }` | `genres` フィールド（配列）のサイズが 2 |
| `{ $and: [{ key1: val1 }, { key2: val2 }] }` | AND 条件 |
| `{ $or: [{ key1: val1 }, { key2: val2 }] }` | OR 条件 |
| `{ key: null }` | `key` フィールドが `null` か `undefined`、あるいは存在しない |
| `{ key: { $type: "null" } }` | `key` フィールドが `null` （＝データ型が Null） |
| `{ key: { $exists: true }` | `key` フィールドが存在する |
| `{ key: { $exists: false }` | `key` フィールドが存在しない |

### ソート `sort()`

| コマンド | 説明 |
| ---- | ---- |
| `db.<coll>.find().sort({ price: 1 })` | `price` フィールドで昇順ソート |
| `db.<coll>.find().sort({ date: -1 })` | `date` フィールドで降順ソート |
| `db.<coll>.find().sort({ price: 1, date: -1 })` | `price` フィールドで昇順ソートして、同じなら `date` フィールドで降順ソート |

### 取得件数を制限 `limit()`

| コマンド | 説明 |
| ---- | ---- |
| `db.<coll>.find().limit(3)` | ドキュメントを最大 3 件取得 |
| `db.<coll>.find().sort({ age: -1 }).limit(3)` | 年齢の高い順にドキュメントを最大 3 件取得 |

### スキップ（ページング処理） `skip()`

| コマンド | 説明 |
| ---- | ---- |
| `db.<coll>.find().skip(50).limit(10)` | 先頭の 50 件をスキップして次の 10 件を取得 |


ドキュメントの追加 (Insert)
----

| コマンド | 説明 |
| ---- | ---- |
| `db.<coll>.insertOne({...})` | 1 つのドキュメントを追加 |
| `db.<coll>.insertMany([...])` | 複数のドキュメントを追加 |

あるコレクションにドキュメントを追加しようとしたときに、コレクションが存在しない場合は自動的に生成されます。

### 実行例

{{< code lang="js" title="1 つのドキュメントを追加" >}}
db.people.insertOne({
  name: { first: "Alan", last: "Turing" },
  birth: new Date("Jun 23, 1912"),
  death: new Date("Jun 07, 1954"),
  contribs: [ "Turing machine", "Turing test", "Turingery" ],
  views: Long(1250000)
})
{{< /code >}}

{{< code lang="js" title="複数のドキュメントを追加" >}}
db.books.insertMany([
  { title: "Title-1", stars: 0 },
  { title: "Title-2", stars: 0 },
  { title: "Title-3", stars: 0 },
])
{{< /code >}}


ドキュメントの削除
----

| コマンド | 説明 |
| ---- | ---- |
| `db.<coll>.remove({ 検索条件 })` | 検索条件に一致したドキュメントを削除 |

### 実行例

{{< code lang="js" title="books コレクション内の指定した ID のドキュメントを削除" >}}
db.books.remove({ _id: "..." })
{{< /code >}}


ドキュメントの更新 (Update)
----

| コマンド | 説明 |
| ---- | ---- |
| `db.<coll>.updateOne({ 検索条件 }, { $set: { 更新内容 } })` | 見つかったドキュメントを 1 件更新 |
| `db.<coll>.updateMany({ 検索条件 }, { $set: { 更新内容 } })` | 見つかったドキュメントをすべて更新 |
| `db.<coll>.updateMany({}, { $set: { 更新内容 } })` | すべてのドキュメントを更新（検索条件なし） |
| `db.<coll>.updateMany({}, { $rename: { key: "newkey" } })` | フィールド名を更新 |
| `db.<coll>.replaceOne({ 検索条件 }, { 更新内容 } })` | 見つかったドキュメントの内容を丸々置き換える（削除して追加するのと同じ） |
| `db.<coll>.updateOne({ 検索条件 }, { $set: { 更新内容 } }, { upsert: true })` | upsert 処理。ドキュメントが見つかったらその内容を更新。 見つからなかったら新規追加（検索条件と更新内容に指定されているフィールドが登録される） |
| `db.<coll>.findAndModify()` | |

### 実行例

{{< code lang="js" title="title が Title-1 の status を closed に更新" >}}
db.issues.updateOne(
  { title: "Title-1" },
  { $set: { status: "closed" } }
)
{{< /code >}}

`$set` で指定したフィールドの値のみ更新されます。
`$set` で指定しなかったフィールドは元の値が維持されます。
元のドキュメントに存在しないフィールドを指定すると、フィールドが追加されます。

{{< code lang="js" title="すべてのドキュメントの stars をインクリメント (+1)" >}}
db.books.updateMany(
  {},
  { $inc: { stars: 1 } }
)
{{< /code >}}

検索条件に空オブジェクト (`{}`) を指定することで、すべてのドキュメントを更新対象としています。
`$set` の代わりに __`$inc`__ を使用すると、既存の値に対して値を加算できます。

{{< code lang="js" title="categories フィールドの名前を genres に変更" >}}
db.books.updateMany(
  {},
  { $rename: { categories: "genres" } }
)
{{< /code >}}

検索条件に空オブジェクト ({}) を指定することで、すべてのドキュメントを更新対象としています。
上記の例では、1 つのフィールド名だけをリネームしていますが、複数のフィールド名をまとめて変更することも可能です。


集計操作 (Aggregation Operations)
----

- __`db.<coll>.aggregate([...])`__
  - データの集計操作を行います。タグやジャンルの一覧情報を生成したいときなどに使えます。

よくある集計操作を行うためのメソッドが組み込みメソッドとして提供されています (single purpose aggregation methods)。

| 指定方法 | 説明 |
| ---- | ---- |
| `db.<coll>.estimatedDocumentCount()` | おおよそのドキュメント数を取得 |
| <s>`db.<coll>.count()`</s> | <s>正確なドキュメント数を取得</s> (deprecated) |
| `db.<coll>.distinct("tags")` | `tags` フィールドに現れる値を配列で取得 |


バックアップとリストア
----

### DB 全体をバイナリデータ (BSON) でバックアップ＆リストア

{{< code lang="console" title="バックアップ" >}}
$ mongodump --uri mongodb+srv://<user>:<pass>@<cluster-url>/<db>
{{< /code >}}

DB のダンプ結果は、`./dump/<db>/<collection>.bson` のようなパスに出力されます。

{{< code lang="console" title="リストア" >}}
$ mongorestore --uri mongodb+srv://<user>:<pass>@<cluster-url> --db <db> ./dump
{{< /code >}}

ユーザーに Database の書き込み権限が必要です。

### コレクションを JSON ファイルにバックアップ＆リストア

{{< code lang="console" title="バックアップ" >}}
$ mongoexport --uri mongodb+srv://<user>:<pass>@<cluster-url>/<db> --collection <coll> --out backup.json
{{< /code >}}

{{< code lang="console" title="リストア" >}}
$ mongoimport --uri mongodb+srv://<user>:<pass>@<cluster-url>/<db> --collection <coll> --file backup.json
{{< /code >}}

ユーザーに Database の書き込み権限が必要です。


その他
----

- `db.<coll>.createIndex({...})`
  - インデックスを生成します。
- `db.createView()`
  - ビュー作成します。

