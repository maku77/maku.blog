---
title: "GraphQL クエリ仕様: イントロスぺクション (introspection) でスキーマ定義を参照する"
url: "p/oo8mydq/"
date: "2022-09-14"
tags: ["GraphQL"]
draft: true
---

GraphQL の __イントロスペクション (introspection)__ 機能を使用すると、スキーマの情報を参照することができます。
GraphiQL（グラフィカル）などの開発ツールでは、この機能を利用してクエリの入力補完を実現しています。

GraphQL API サーバーに対して次のようなクエリを投げると、どんなフィールドを指定できるかを調べることができます。

```graphql
__schema { ... }
__type(name: "User") { ... }
```

