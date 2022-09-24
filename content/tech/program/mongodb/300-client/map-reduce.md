---
title: "mongo シェルで MapReduce を実行する"
url: "/p/qvkkcuj"
date: "2014-03-31"
tags: ["MongoDB"]
---

`mongo` シェルを使って、下記のような `books` コレクションに対して **MapReduce** をかけて、タグ情報のリスト（`tags` コレクション）を生成する方法を示します。

{{< code lang="js" >}}
{ title: 'Title 1', tags: ['AAA', 'BBB'] }
{ title: 'Title 2', tags: ['BBB', 'CCC'] }
{ title: 'Title 3', tags: ['CCC', 'AAA'] }
{{< /code >}}

作成される `tags` コレクションは、以下のようになることを想定しています。

{{< code lang="js" >}}
{ "_id" : "AAA", "value" : 2 }
{ "_id" : "BBB", "value" : 2 }
{ "_id" : "CCC", "value" : 2 }
{{< /code >}}

まずは、サンプルデータとなる `books` コレクションを作成します。

{{< code lang="js" title="books.js" >}}
use testdb;

// books コレクションの初期化
db.books.drop();
db.books.save({title: 'Title 1', tags: ['AAA', 'BBB']});
db.books.save({title: 'Title 2', tags: ['BBB', 'CCC']});
db.books.save({title: 'Title 3', tags: ['CCC', 'AAA']});

// 確認
print('books collection has been created');
db.books.find();
{{< /code >}}

{{< code >}}
$ mongo < books.js
{{< /code >}}

次に、`books` コレクションに MapReduce をかけて、`tags` コレクションを生成します。

{{< code lang="js" title="mapreduce.js" >}}
use testdb;

var map = function() {
  if (!this.tags) {
    return;
  }
  this.tags.forEach(function(tag) {
    emit(tag, 1);
  });
};

var reduce = function(key, values) {
  var sum = 0;
  values.forEach(function(val) {
    sum += val;
  });
  return sum;
};

db.runCommand({
  mapReduce: 'books',  // MapReduce をかけるコレクション
  map: map,            // Map 関数
  reduce: reduce,      // Reduce 関数
  out: 'tags'          // MapReduce の結果を格納するコレクション
});
{{< /code >}}

{{< code >}}
$ mongo < mapreduce.js
{{< /code >}}


実際に `tags` コレクションが生成されているか確認してみます。

```console
$ mongo testdb
> db.tags.find()
{ "_id" : "AAA", "value" : 2 }
{ "_id" : "BBB", "value" : 2 }
{ "_id" : "CCC", "value" : 2 }
```

できてますね！

- 参考: [mapReduce — MongoDB Manual](http://docs.mongodb.org/manual/reference/command/mapReduce/)

