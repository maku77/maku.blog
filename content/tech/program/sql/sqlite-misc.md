---
title: "SQLite 雑多メモ"
url: "/p/w5x4797"
date: "2008-11-16"
tags: ["SQL", "SQLite"]
---

SQLite にできないこと
----

[SQL Features That SQLite Does Not Implement](http://www.sqlite.org/omitted.html) より。

- ユーザごとにアクセス制限ができない。
    - `GRANT` によるアクセス権限の制御はできない。
    - SQLite にはデータベースサーバが存在せず、クライアントが直接 DB ファイルにアクセスする形式のため、サーバでの `GRANT` によるアクセス権限の制御ができない。
- そのユーザーがファイルにアクセスできるということは、そのデータベースに対して何でもできるということを示す。
    - `VIEW` は使えるがリードオンリーである。


SQLite 2 と SQLite 3 の違い
----

- バージョン 3 では BLOB (Binary Large OBject) がサポートされた。
- バージョン 3 のほうがデータベースファイルのサイズがコンパクトになる。
- 整数の連番を生成する ROWID がバージョン 2 では 32bit、バージョン 3 では 64bit。


SQLite 2 のデータベースファイルを SQLite 3 のデーターベース形式に変換する
----

```
$ sqlite db2 .dump | sqlite3 db3
```


各言語用の SQLite ライブラリ
----

- [SQLite Wrappers](http://www.sqlite.org/cvstrac/wiki?p=SqliteWrappers.)


Python で SQLite を操作する
----

- [PEP 249 -- Python Database API Specification v2.0](https://www.python.org/dev/peps/pep-0249/)

