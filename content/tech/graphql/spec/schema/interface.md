---
title: "GraphQL スキーマ仕様: インタフェース型 (interface type)"
url: "p/yp9mv5d/"
date: "2022-09-14"
tags: ["GraphQL"]
---

GraphQL のインタフェース型は、複数の型が共通して持つフィールドを定義するための抽象型で、__`interface`__ キーワードを使って定義します。
次の `ScheduleItem` インタフェースは、2 つのフィールド（`id` と `title`）を持つことを示しています。
インタフェースを実装する (__`imlements`__) 側の型は、必ず `id` と `title` フィールドを持つ必要があります。

{{< code lang="graphql" title="スキーマ定義" >}}
# 共通のインタフェースを定義
interface ScheduleItem {
  id: ID!
  title: String!
}

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

上記の `Task` 型と `Milestone` 型は、`ScheduleItem` というインタフェースを実装しています。
`allItems` クエリは、`Task` と `Milestone` を要素に持つリストを返すことを想定していますが、戻り値のリスト要素の型が `ScheduleItem` になっているため、必ず `id` と `title` フィールドが含まれることが保証されています。
クエリ内でリスト要素を参照するときは、次のように直接これらのフィールドを参照できます。

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
    ... onMilestone {
      date
    }
  }
}
{{< /code >}}

複数のインタフェースを実装したいときは、次のように `implements` の後ろにインタフェース名を __`&`__ で並べます。

```graphql
"""
Represents a Milestone object on a given repository.
"""
type Milestone implements Closable & Node & UniformResourceLocatable {
  # ...
}
```

