---
title: "mongo シェルの基本的な使い方"
url: "/p/pwx5kf7"
date: "2015-04-13"
tags: ["MongoDB"]
weight: 1
---

MongoDB サーバーへの接続
----

`mongo` シェルで MongoDB サーバーへ接続するには次の用に実行します。
先に [MongoDB サーバーを起動](/p/3ucs8n3) しておく必要があります）。

{{< code lang="console" >}}
$ mongo       # 単純に接続する場合
$ mongo mydb  # 使用するデータベース名を指定する場合
{{< /code >}}

ポート番号（デフォルトは `27017`）や接続先の IP アドレスを指定することもできます。
下記はすべて正しい接続方法です。

{{< code lang="console" >}}
$ mongo --port 40001
$ mongo --port 40001 mydb

$ mongo localhost:40001
$ mongo localhost:40001/mydb

$ mongo 192.168.0.100
$ mongo 192.168.0.100/mydb
$ mongo 192.168.0.100:40001/mydb
{{< /code >}}


ヘルプの表示 (help)
----

`mongo` シェルを起動したあとに、__`help`__ コマンドを実行するとヘルプを表示することができます。

```console
> help
db.help()                    help on db methods
db.mycoll.help()             help on collection methods
sh.help()                    sharding helpers
rs.help()                    replica set helpers
help admin                   administrative help
help connect                 connecting to a db help
help keys                    key shortcuts
help misc                    misc things to know
help mr                      mapreduce

show dbs                     show database names
show collections             show collections in current database
show users                   show users in current database
show profile                 show most recent system.profile entries with time >= 1ms
show logs                    show the accessible logger names
show log [name]              prints out the last segment of log in memory, 'global' is default
use <db_name>                set current database
db.mycoll.find()             list objects in collection mycoll
db.mycoll.find( { a : 1 } )  list objects in mycoll where a == 1
it                           result of the last line evaluated; use to further iterate
DBQuery.shellBatchSize = x   set default number of items to display on shell
exit                         quit the mongo shell
```


コレクションにドキュメントを追加する (db.coll.insert)
----

まず、データベース名を指定して MongoDB サーバーに接続してください。
ここでは、`mydb` という名前のデータベースを使うことにします（実体は必要に応じて自動的に作成されます）。

```console
$ mongo mydb
```

データベース名を指定せずに接続した場合は、`use <DB名>` で切り替えられます。
現在操作対象としているデータベースは __`db`__ と入力することで確認できます。

```console
> use mydb
switched to db mydb

> db
mydb
```

データベースの「コレクションにドキュメントを追加する」には、次のようなコマンドを実行します。
これは、RDB でいうところの「テーブルにレコードを追加する」ことに相当します。

{{< code lang="ts" >}}
db.<コレクション名>.insert(<ドキュメント>)
{{< /code >}}

コレクションはあらかじめ生成しておく必要はなく、必要に応じて自動的に生成されます（データベースと同様）。
追加するデータ（ドキュメント）は、JavaScript のオブジェクト形式で指定します。
次の例では、`books` コレクションに、3 つのドキュメントを追加しています。

{{< code lang="console" >}}
> db.books.insert({title: 'Title 1'})
> db.books.insert({title: 'Title 2'})
> db.books.insert({title: 'Title 3'})
{{< /code >}}


コレクション内のドキュメントを取得する (db.coll.find)
----

コレクションに格納されているドキュメントを取得（検索）するには、__`find`__ を使用します。
引数なしで実行すると、すべてのドキュメントを取得します。

{{< code lang="console" title="books コレクション内の全ドキュメントを取得" >}}
> db.books.find()
{ "_id" : ObjectId("6096b7e313fb77617c8a6a6c"), "title" : "Title 1" }
{ "_id" : ObjectId("6096b7ea13fb77617c8a6a6d"), "title" : "Title 2" }
{ "_id" : ObjectId("6096b7eb13fb77617c8a6a6e"), "title" : "Title 3" }
{{< /code >}}

特定のドキュメントを検索したいときは、__`find`__ の引数で検索条件を指定します。

{{< code lang="console" title="title フィールドでドキュメントを検索" >}}
> db.books.find({title: 'Title 1'})
{ "_id" : ObjectId("6096b7e313fb77617c8a6a6c"), "title" : "Title 1" }
{{< /code >}}

`books` コレクションに対して、他にどのようなコマンドを実行できるかは、次のようにして確認できます。

{{< code lang="console" title="コレクションの操作に関するヘルプ" >}}
> db.books.help()
{{< /code >}}


コレクション内のドキュメントを削除する (db.coll.drop, db.coll.remove)
----

ドキュメントの削除には、__`drop`__ や __`remove`__ を使用します。

{{< code lang="console" title="全てのドキュメントを削除する" >}}
> db.books.drop()
{{< /code >}}

{{< code lang="console" title="条件に一致するドキュメントを削除する" >}}
> db.books.remove({title: 'Title 1'})
{{< /code >}}

`remove` の引数に空オブジェクト (`{}`) を渡すことでも全てのドキュメントを削除することができますが、`drop` を使った方が効率的です。

