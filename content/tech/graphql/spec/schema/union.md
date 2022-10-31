---
title: "GraphQL スキーマ仕様: ユニオン型 (Union types) を定義する"
url: "p/vqsyz9j/"
date: "2022-10-10"
tags: ["GraphQL"]
---

GraphQL スキーマの __ユニオン型 (Union types)__ は、複数の型のうち、いずれかの型であることを表現する型です。
典型的には、検索系のフィールドにおいて、様々な種類のデータを返す場合に使われます。
ユニオン型は、__`union`__ キーワードを使って次のように定義します。

{{< code lang="graphql" title="ユニオン型の定義" >}}
union TaskOrTodo = Task | Todo

type Query {
    search(term: String!): [TaskOrTodo!]
}
{{< /code >}}

下記は GitHub の GraphQL スキーマで定義されているユニオン型の例です。

{{< code lang="graphql" title="GitHub API の union 型の例" >}}
"""
Used for return value of Repository.issueOrPullRequest.
"""
union IssueOrPullRequest = Issue | PullRequest


"""
Types that can be inside a Milestone.
"""
union MilestoneItem = Issue | PullRequest
{{< /code >}}

GraphQL クエリにおいてユニオン型のフィールドを参照するときは、[インラインフラグメント](/p/wiv7it5/#inline) を使って、具体的な型ごとにフィールドを参照する必要があります。

{{< code lang="graphql" title="ユニオン型のフィールドを参照するクエリ" >}}
query QueryRecentActivities {
  search(type: ISSUE, query: "repo:graphql/graphql-spec", last: 10) {
    nodes {
      ... on Issue {
        number
        title
      }
      ... on PullRequest {
        title
        baseRefName
      }
    }
  }
}
{{< /code >}}

ユニオン型は、エラーを表現するためにも使用できます。
次の `createBook` は新しい書籍データを登録するための mutation ですが、登録に成功したときは `Book` オブジェクトを返し、失敗したときは各種エラーを表すオブジェクトを返します。

```graphql
type Mutation {
  addBook(input: AddBookInput!): AddBookPayload!
}

union AddBookPayload = Book | AlreadyExistError | InvalidTitleError
```

