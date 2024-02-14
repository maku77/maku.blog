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

| find の第 1 引数 | 説明 |
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
| `{ $and: [{ k1: v1 }, { k2: v2 }] }` | AND 条件 |
| `{ $or: [{ k1: v1 }, { k2: v2 }] }` | OR 条件 |
| `{ key: null }` | `key` フィールドが `null` か `undefined`、あるいは存在しない |
| `{ key: { $type: "null" } }` | `key` フィールドが `null` （＝データ型が Null） |
| `{ key: { $exists: true }` | `key` フィールドが存在する |
| `{ key: { $exists: false }` | `key` フィールドが存在しない |

### ソート `sort()`

<table>
  <tr>
    <td>
{{< code lang="js" >}}
db.<coll>.find().sort({ price: 1 })
{{< /code >}}
    </td>
    <td><code>price</code> フィールドで昇順ソート</td>
  </tr>
  <tr>
    <td>
{{< code lang="js" >}}
db.<coll>.find().sort({ date: -1 })
{{< /code >}}
    </td>
    <td><code>date</code> フィールドで降順ソート</td>
  </tr>
  <tr>
    <td>
{{< code lang="js" >}}
db.<coll>.find().sort({
  price: 1,
  date: -1
})
{{< /code >}}
    </td>
    <td><code>price</code> フィールドで昇順ソート。<br/><code>price</code> フィールドの値が同じ場合は、さらに <code>date</code> フィールドで降順ソート。<code>price</code> と <code>date</code> の<a href="#index">複合インデックス</a>を張っておくと高速なソートが可能です。</td>
  </tr>
</table>

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
| `db.<coll>.deleteOne({ 検索条件 })` | 検索条件に一致するドキュメントを 1 件削除 |
| `db.<coll>.deleteMany({ 検索条件 })` | 検索条件に一致するドキュメントを全件削除 |
| `db.<coll>.deleteMany({})` | すべてのドキュメントを削除（危険） |
| <s>`db.<coll>.remove({ 検索条件 })`</s> | `remove()` は MongoDB 4.0 で deprecated |

### ドキュメント削除の例

```js
// 指定した ID のドキュメントを削除
db.books.deleteOne({ _id: ObjectId('6595345d07c7966c20e44b5d') })

// 指定した価格以上のドキュメントをすべて削除
db.books.deleteMany({ price: { $gte: 3000 } })

// すべてのドキュメントを削除（危険）
db.books.deleteMany({})
```


ドキュメントの更新 (Update)
----

<table>
  <tr><th>コマンド</th><th>説明</th></tr>
  <tr>
    <td>
{{< code >}}
db.<coll>.updateOne(
  { 検索条件 },
  { $set: { 更新内容 } }
)
{{< /code >}}
    </td>
    <td>見つかったドキュメントを 1 件更新</td>
  </tr>
  <tr>
    <td>
{{< code >}}
db.<coll>.updateOne(
  { 検索条件 },
  { $set: { 更新内容 } },
  { upsert: true }
)
{{< /code >}}
    </td>
    <td>upsert 処理。ドキュメントが見つかったらその内容を更新。 見つからなかったら新規追加（検索条件と更新内容に指定されているフィールドが登録される）</td>
  </tr>
  <tr>
    <td>
{{< code >}}
db.<coll>.updateMany(
  { 検索条件 },
  { $set: { 更新内容 } }
)
{{< /code >}}
    </td>
    <td>見つかったドキュメントをすべて更新</td>
  </tr>
  <tr>
    <td>
{{< code >}}
db.<coll>.updateMany(
  {},
  { $set: { 更新内容 } }
)
{{< /code >}}
    </td>
    <td>すべてのドキュメントを更新（検索条件なし）</td>
  </tr>
  <tr>
    <td>
{{< code >}}
db.<coll>.updateMany(
  {},
  { $set: { "myArrayField.$[elem]": "置換後" } },
  { arrayFilters: [{ "elem": { $eq: "置換前" } }] }
)
{{< /code >}}
    </td>
    <td><a href="#array-filters">配列フィールド内の値を置換</a></td>
  </tr>
  <tr>
    <td>
{{< code >}}
db.<coll>.replaceOne(
  { 検索条件 },
  { 更新内容 } }
)
{{< /code >}}
    </td>
    <td>見つかったドキュメントの内容を丸々置き換える（削除して追加するのと同じ）</td>
  </tr>
</table>

### 見つかったドキュメントを 1 件更新

{{< code lang="js" title="title が Title-1 の status を closed に更新" >}}
db.issues.updateOne(
  { title: "Title-1" },
  { $set: { status: "closed" } }
)
{{< /code >}}

__`$set`__ で指定したフィールドの値のみ更新されます。
`$set` で指定しなかったフィールドは元の値が維持されます。
元のドキュメントに存在しないフィールドを指定すると、フィールドが追加されます。

### すべてのドキュメントを更新

{{< code lang="js" title="すべてのドキュメントの stars をインクリメント (+1)" >}}
db.books.updateMany(
  {},
  { $inc: { stars: 1 } }
)
{{< /code >}}

検索条件に空オブジェクト (`{}`) を指定することで、すべてのドキュメントを更新対象としています。
`$set` の代わりに __`$inc`__ を使用すると、既存の値に対して値を加算できます。

### 配列フィールド内の値を置換 {#array-filters}

`genres` 配列フィールド内の `OTHER` という要素をまとめて `ETC` に置換する例です。

{{< code lang="js" title="テスト用 JavaScript コード" >}}
// test コレクションの準備
db.test.deleteMany({})
db.test.insertMany([
  { _id: 1, genres: ["ACT", "RPG"] },
  { _id: 2, genres: ["STG", "OTHER"] },
  { _id: 3, genres: ["OTHER", "ACT"] },
])

// genres 配列フィールド内の OTHER を ETC に置換
db.test.updateMany(
  { "genres": { $exists: true } },
  { $set: { "genres.$[elem]": "ETC" } },
  { arrayFilters: [{ "elem": { $eq: "OTHER" } }] }
)

// 結果確認
db.test.find()
{{< /code >}}

{{< code lang="js" title="実行結果" >}}
[
  { _id: 1, genres: [ 'ACT', 'RPG' ] },
  { _id: 2, genres: [ 'STG', 'ETC' ] },
  { _id: 3, genres: [ 'ETC', 'ACT' ] }
]
{{< /code >}}

`updateMany()` の検索条件（第 1 引数）には `{}` を渡すことも可能ですが、これだと `genres` フィールドが存在しないドキュメントがある場合にエラーになってしまうので、上記では `{ "genres": { $exists: true } }` として、`genres` フィールドが存在するドキュメントに対してのみ置換処理するようにしています。


スキーマの更新
----

| コマンド | 説明 |
| ---- | ---- |
| `db.<coll>.updateMany({}, { $rename: { oldKey: "newKey" } })` | フィールド名を変更 |

### フィールド名を変更

次の例では、1 つのフィールド名をリネームしています（複数のフィールド名をまとめて変更することも可能です）。
検索条件に空オブジェクト (`{}`) を指定することで、すべてのドキュメントを更新対象としています。

{{< code lang="js" title="categories フィールドの名前を genres に変更" >}}
db.books.updateMany(
  {},
  { $rename: { categories: "genres" } }
)
{{< /code >}}

すべてのドキュメントが `categories` フィールドを持つことが保証されていない場合は、次のように絞り込んで置換します。

{{< code lang="js" >}}
db.books.updateMany(
  { "categories": { $exists: true } },
  { $rename: { categories: "genres" } }
)
{{< /code >}}


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


インデックス {#index}
----

<table>
  <tr><th>コマンド</th><th>説明</th></tr>
  <tr>
    <td>
{{< code >}}
db.<coll>.createIndex({ field: 1 })
{{< /code >}}
    </td>
    <td>score フィールド用のインデックスを作成</td>
  </tr>
  <tr>
    <td>
{{< code >}}
db.<coll>.createIndex({
  genre: 1,
  price: -1
})
{{< /code >}}
    </td>
    <td>genre と price フィールドの複合インデックスを作成（昇順＋降順）</td>
  </tr>
  <tr>
    <td>
{{< code >}}
db.<coll>.createIndex({
  title: "text",
  comment: "text"
})
{{< /code >}}
    </td>
    <td>title と comment フィールド用にテキストインデックスを作成</td>
  </tr>
  <tr>
    <td>
{{< code >}}
db.<coll>.createIndex({ "$**": "text" })
{{< /code >}}
    </td>
    <td>全フィールド用のテキストインデックスを作成（ワイルドカードテキストインデックス）</td>
  </tr>
</table>

MongoDB のインデックスの詳細ついては[こちらを参照](/p/gii3wtb/)。

