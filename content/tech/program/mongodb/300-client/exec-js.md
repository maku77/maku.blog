---
title: "mongo シェルで JavaScript ファイル（スクリプト）を実行する"
url: "/p/2thipwb"
linkTitle: "mongo シェルで JavaScript ファイル（スクリプト）を実行する"
date: "2014-03-31"
tags: ["MongoDB"]
weight: 2
---

例えば、`input.js` というスクリプトを `mongo` シェルで実行するには以下のようにします。
ここでは、`mydb` という名前のデータベースの `books` コレクションに、テスト用のドキュメントを追加しています。

```console
$ mongo mydb input.js
```

{{< code lang="js" title="input.js" >}}
db.books.insert({title: 'Title 1'});
db.books.insert({title: 'Title 2'});
db.books.insert({title: 'Title 3'});
{{< /code >}}

ドキュメントには書いてありませんが、標準入力から読み込ませることもできるようです。
この方法を使うと、`mongo` コマンドのオプションのフォーマット的に、データベース名を省略してスクリプトファイル名を指定できるようになるので、**スクリプトの中でデータベースを切り替えることができます**。

```console
$ mongo < input2.js
$ mongo --port 40001 < input2.js
$ mongo localhost:40001 < input2.js
```

{{< code lang="js" title="input2.js" >}}
use testdb;
db.books.insert({title: 'Title 1', tags: ['AAA', 'BBB']});
db.books.insert({title: 'Title 2', tags: ['BBB', 'CCC']});
db.books.insert({title: 'Title 3', tags: ['CCC', 'AAA']});
{{< /code >}}

