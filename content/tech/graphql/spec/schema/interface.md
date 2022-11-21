---
title: "GraphQL スキーマ仕様: インタフェース型 (interface type) を定義する"
url: "p/yp9mv5d/"
date: "2022-09-14"
tags: ["GraphQL"]
---

インタフェース型を定義する
----

GraphQL のインタフェース型は、複数の型が共通して持つフィールドを定義するための抽象型で、__`interface`__ キーワードを使って定義します。
次の `ScheduleItem` インタフェースは、2 つのフィールド（`id` と `title`）を持つことを示しています。

{{< code lang="graphql" title="スキーマ定義" >}}
# 共通のインタフェースを定義
interface ScheduleItem {
  id: ID!
  title: String!
}
{{< /code >}}

インタフェースを実装する (__`imlements`__) 側の型は、必ず `id` と `title` フィールドを持つ必要があります。
次の `Task` オブジェクト型と `Milestone` オブジェクト型は、`ScheduleItem` というインタフェースを実装しています。

{{< code lang="graphql" title="スキーマ定義（続き）" >}}
# Task 型は必ず id と title フィールドを持つ
type Task implements ScheduleItem {
  id: ID!
  title: String!
  content: String
}

# Milestone 型は必ず id と title フィールドを持つ
type Milestone implements ScheduleItem {
  id: ID!
  title: String!
  date: DateTime
}

scalar DateTime

type Query {
  allItems(): [ScheduleItem!]
}
{{< /code >}}

トップレベルの `allItems` クエリは、`Task` あるいは `Milestone` を要素とするリストを返すことを想定していますが、戻り値のリスト要素の型が `ScheduleItem` になっているため、必ず `id` と `title` フィールドが含まれることが保証されています。


クエリでインタフェース型を使う
----

GraphQL クエリでインタフェース型のオブジェクトを返すフィールドを参照する場合、インタフェースで定義されたフィールドは、次のように直接参照することができます。

{{< code lang="graphql" title="クエリ" >}}
query QueryAllItems {
  allItems {
    id
    title
  }
}
{{< /code >}}

一方、`Task` 型や `Milestone` 型にしか含まれないフィールドを取得したいときは、次のように [インラインフラグメント (`... on Xxx`)](/p/wiv7it5/#inline) の形で分岐させて参照する必要があります。

{{< code lang="graphql" title="クエリ" >}}
query QueryAllItems {
  allItems {
    id
    title
    ... on Task {
      content
    }
    ... on Milestone {
      date
    }
  }
}
{{< /code >}}


複数のインタフェースを実装するオブジェクト型
----

あるオブジェクト型に複数のインタフェースを実装したいときは、次のように `implements` の後ろにインタフェース名を __`&`__ で並べます。

```graphql
"""
Represents a Milestone object on a given repository.
"""
type Milestone implements Closable & Node & UniformResourceLocatable {
  # ...
}
```

インタフェース型とユニオン型の使い分け
----

インタフェース型とユニオン型は、どちらも抽象型 (abstract types) を構成するための仕組みですが、両者は全く逆の意味を持っています。
インタフェース型が、__複数の型が同じ振る舞い__ をすることを表現するのに対し、ユニオン型は、あるオブジェクトの実体が __異なる型になり得る__ ことを表現しています。

- 参考: [ユニオン型 (Union types) を定義する](/p/vqsyz9j/)

