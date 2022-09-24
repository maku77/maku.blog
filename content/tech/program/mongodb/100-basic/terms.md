---
title: "MongoDB と RDB の用語比較"
url: "/p/gbs79ua"
date: "2013-10-22"
tags: ["MongoDB"]
weight: 2
---

MongoDB で使用する **Database** や **Collection** などの用語は、RDB の用語と以下のような関係にあります。

| MongoDB の用語 | RDB の用語 |
| ---- | ---- |
| __Database__ | Database |
| __Collection（コレクション）__ | Table（テーブル） |
| __Document（ドキュメント）__ | Record / Row（行） |
| __Field（フィールド）__ | Column（列） |

ある Database の Collection に Document を追加するときは、`mongo` クライアントでは以下のように実行します。

{{< code >}}
> use <Database名>
> db.<Collection名>.insert(<Document の Field セット>)
{{< /code >}}

