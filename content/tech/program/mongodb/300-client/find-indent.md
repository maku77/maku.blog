---
title: "mongo シェルで find() の結果をインデントして見やすく表示する"
url: "/p/uvusqb7"
date: "2014-03-31"
tags: ["MongoDB"]
---

`mongo` シェルで `find()` を実行した結果を見やすいように整形するには、実行結果に対して __`pretty()`__ を適用します。

{{< code >}}
> db.books.find().pretty()
{
     "_id" : ObjectId("533973de50b35840de2dda25"),
     "title" : "Title 1",
     "tags" : [
          "AAA",
          "BBB"
     ]
}
{
     "_id" : ObjectId("533973de50b35840de2dda26"),
     "title" : "Title 2",
     "tags" : [
          "BBB",
          "CCC"
     ]
}
{
     "_id" : ObjectId("533973de50b35840de2dda27"),
     "title" : "Title 3",
     "tags" : [
          "CCC",
          "AAA"
     ]
}
{{< /code >}}

