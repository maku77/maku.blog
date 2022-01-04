---
title: "NoSQL データベースのまとめ"
url: "/p/26vmfa8"
date: "2020-02-03"
tags: ["NoSQL"]
draft: true
---

ほとんどの NoSQL データベースは、従来の RDB とは異なり、**BASE** という考え方を採用しています。

- {{< label "Basically Available" >}} ... 可用性を重視し、
- {{< label "Soft state" >}} ... 厳密性は追及せず、
- {{< label "Eventually consistent" >}} ... 最終的に一貫性がとれればよい。


key-value ストア
----

### 特徴
- 検索方法は、キーを使う検索に限定される。

### 例
- memcached ... Facebook、Twitter、YouTube などで使用。
- Redis
- Cassandra ... Facebook が使っていたものを 2008 年にオープンソース化。Twitter などで使用。レプリケーションにより、サーバーがダウンしても自動復帰可能。キーを4次元や5次元にすることができる。
- ROMA ... Rakuten On-Memory Architecture。楽天が開発し、2009年に楽天トラベルに導入、オープンソース化した。
- Tokyo Tyrant


ドキュメント指向データーベース
----

### 特徴
- key-value ストアの value 部分が構造を持つドキュメントになっている。JSON や XML のようなドキュメントを直接データベース化するイメージ。
- ドキュメント内の内容に基づいた検索が可能。

### 例
- CouchDB ... RESTful インタフェース。Erlang により実装。JSON を使ってレコードを表現。
- MongoDB ... リレーショナルデータベースに近い使い勝手を提供。ほとんどの言語用にドライバ（ライブラリ）が提要されている。[MongoDB のメモ](/p/qikq9o8)
- 各種 XML データベース


オブジェクト指向データベース
----
### 特徴
オブジェクト指向言語の「オブジェクト」をそのまま永続化するためのもの。

### 例
- Db4o
- ZopeDB
- ObjectStore

