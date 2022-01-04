---
title: "MongoDB の ObjectId について（Document の生成時間を調べる）"
url: "/p/wdbr8dx"
date: "2013-10-22"
tags: ["MongoDB"]
---

MongoDB の各 Document（RDB でいうレコード）は、それぞれ __`_id`__ というプライマリキーとなるフィールドを持っています。
この値は、何も指定しないと、自動的に **`ObjectId`** オブジェクトが生成されて設定されます。

{{< code >}}
> db.mycollection.insert({value:100})
> db.mycollection.findOne()
{ "_id" : ObjectId("5265e358c76d2a2f8b870057"), "value" : 100 }
{{< /code >}}

この `ObjectId` オブジェクトは [BSON 形式](http://bsonspec.org/) のオブジェクトで、内部的に以下のような値を保持しています。

| フィールド | サイズ | 内容 |
| ---- | ---- | ---- |
| <b>Timestamp</b> | 4 bytes | 生成時間（秒） |
| <b>Machine ID</b> | 3 bytes | マシン固有値 |
| <b>PID</b> | 2 bytes | プロセス ID |
| <b>Counter</b> | 3 bytes | ランダム値から始まってインクリメントされる |

`ObjectId` オブジェクト内に含まれているタイムスタンプは、**`getTimestamp()`** メソッドによって取得できます。
これを利用すると、Document の `_id` フィールドから、その Document が生成された時間を調べることができます。

{{< code >}}
> db.mycollection.findOne()._id.getTimestamp()
ISODate("2013-10-22T02:30:48Z")
{{< /code >}}

{{% reference %}}
- [ObjectId — MongoDB Manual](http://docs.mongodb.org/manual/reference/object-id/)
{{% /reference %}}

