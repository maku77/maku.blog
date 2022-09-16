---
title: "GraphQL スキーマ仕様: インタフェース型 (interface type)"
url: "p/yp9mv5d/"
date: "2022-09-14"
tags: ["GraphQL"]
draft: true
---

インタフェース型は、それを `implements` する具象型が必ず備えているべきフィールド群を定義します。

```graphql
interface SearchResultItem {
    id: ID!
    content: String!
}

type Task implements SearchResultItem {
    # ...
    approachCount: Int!
}

type Approach implements SearchResultItem {
    # ...
    task: Task!
}

type Query {
    # ...
    search(term: String!): [SearchResultItem!]
}
```

上記の `Task` 型と `Approach` 型は、`SearchResultItem` というインタフェースを備えていることを保証しています。
`search` クエリは `Task` 型と `Approarch` 型のどちらかのオブジェクトを返すことを想定していますが、いずれも `SearchResultItem` インタフェースを備えているため、インラインフィールド (`... on Task`) による分岐を行わずにそのフィールドを参照できます。

