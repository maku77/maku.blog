---
title: "MongoDB の特徴"
url: "/p/xzc5z98"
date: "2019-05-20"
tags: ["MongoDB"]
weight: 1
---

MongoDB はオープンソースの NoSQL データベースで、MongoDB Inc.（旧 10gen）によって開発とサポートが行われています。

- https://www.mongodb.com/

MongoDB は次のような特徴を持っています。

* **ドキュメント指向型**のデータベースで、BSON 形式 (Binary JSON) でオブジェクトを格納する。
* スキーマ定義が必要なく、**データ構造が柔軟**。
* **マスタ・スレーブ型**のアーキテクチャを取り、書き込みはマスタノードに対してのみ、読み出しは複製されたスレーブノードからのみ行われる。非同期に複製が行われ、一時的に**複数のデータバージョンが混在する可能性がある**ため、完璧な整合性が求められる用途には向かない。
* バージョン 1.6 以降はレプリカ・セットという方式を採用し、プライマリ＆セカンダリの役割に基づいて複製制御が行われる。
* **MapReduce** を使ったバッチ処理を利用できる。
* セカンダリ・インデックスという機能により、複数の索引を使ってデータを検索できる。
* RDB と比較して大量のデータを扱える。Mongo という名前は、英語の humongous（巨大な）から来ている。
* JavaScript コードをストアド・プロシージャとして使用できる。

