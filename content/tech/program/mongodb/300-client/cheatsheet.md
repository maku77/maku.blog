---
title: "mongo シェルのチートシート"
url: "p/j4c4vnq/"
date: "2023-02-14"
tags: ["MongoDB"]
draft: true
---

全般
----

- __`db`__
  - 現在のデータベース名を表示します。
- __`use <db>`__
  - データベースを切り替えます。


ドキュメントの追加 (Insert)
----

- __`db.<coll>.insertOne([...])`__
  - 1 つのドキュメントを追加します。
- __`db.<coll>.insertMany([...])`__
  - 複数のドキュメントを追加します。


ドキュメントの検索 (Find)
----

- __`db.<coll>.find({})`__
  - すべてのドキュメントを取得します。
- __`db.<coll>.find({"name": "value"})`__
  - `name` フィールドの値が `value` に一致するドキュメントを検索します。
- __`db.<coll>.find({"created": {$lt: ISODate("2000-01-01")}})`__
  - `created` フィールドの日付が指定した 2000-01-01 より前のドキュメントを検索します。
- __`db.<coll>.find({"awards.wins": {$gt: 100}})`__
  - `awards.wins` フィールドの値が 100 より大きいドキュメントを検索します。
- __`db.<coll>.find({"languages": {$in: ["jp", "en"]}})`__
  - `languages` フィールドの値として、`jp` あるいは `en` を含むドキュメントを検索します。
- __`db.<coll>.find({}, {"_id": 0, "title": 1, "genres": 1})`__
  -  指定したフィールドのみ取得します。2 番目の引数のことを project fields と呼びます。


ドキュメントの削除
----

- __`db.<coll>.remove({"_id": "..."})`__
  - 指定した ID のドキュメントを削除します。


集計 (Aggregation Operations)
----

- __`db.<coll>.aggregate([...])`__
  - データの集計操作を行います。タグやジャンルの一覧情報を生成したいときなどに使えます。


インデックス
----

- __`db.<coll>.createIndex({...})`__
  - インデックスを生成します。


コレクション
----

- __`db.craeteCollection()`__
  - オプション指定付きでコレクションを作成します。
- __`db.getCollectionInfos()`__
  - コレクション情報（UUID など）を表示します。
- __`db.getCollectionNames()`__
  - コレクション名の一覧を取得します。

その他
----

- __`db.createView()`__
  - ビュー作成します。
- __`db<coll>.mapReduce()`__
- __`db.<coll>.update({"_id": "...", ...}, {"$inc": {"stats.win": 1}})`__
  - 第 1 引数は filter 条件、第 2 引数は mutation 内容を示します。
- __`db.<coll>.updateMany()`__
- __`db.<coll>.findAndModify()`__


