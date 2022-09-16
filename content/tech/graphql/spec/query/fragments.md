---
title: "GraphQL クエリ仕様: フラグメント (Fragments) とインラインフラグメント (Inline Fragments)"
url: "p/wiv7it5/"
date: "2021-01-19"
tags: ["GitHub", "GraphQL"]
---

GraphQL フラグメントでクエリをパーツ化する
----

GraphQL クエリの中で、同じようなフィールドの指定（選択セット）を複数回使用する場合、それを [フラグメント (Fragments)](https://graphql.org/learn/queries/#fragments) という再利用可能な選択セットとして切り出して定義しておくことができます。

例えば次の GraphQL クエリは、GitHub から自分のユーザー情報 (`viewer`) と、特定のユーザーの情報 (`user`) を一度に取得しています。

{{< code lang="graphql" title="GraphQL クエリ" >}}
query QueryTwoUsers {
  viewer {
    login       # ログインID
    name        # ユーザー名
    url         # ユーザーの GitHub ホームページ
    websiteUrl  # ユーザーの Web サイト
    avatarUrl   # ユーザーのアバター画像
  }

  user(login: "ログインID") {
    login
    name
    url
    websiteUrl
    avatarUrl
  }
}
{{< /code >}}

`viewer` フィールドと `user` フィールドは、両方とも [User 型](https://docs.github.com/en/graphql/reference/objects#user) のフィールドで、しかも、上記の例では `User` オブジェクトの中の同じフィールドを参照しています。
明らかに冗長な書き方です。

このようなケースでは、あるオブジェクトの特定のフィールドを参照するための選択セット (selection set) を、フラグメントの形で定義することができます。
次の例では、`User` オブジェクトの特定のフィールドを選択するための `userFragment` というフラグメントを定義しています。
フラグメントを使用する場所では、__`...userFragment`__ のようにドットを 3 つ付けて参照します。

{{< code lang="graphql" title="GraphQL クエリ" >}}
query QueryTwoUsers {
  viewer {
    ...userFragment
  }

  user(login: "ログインID") {
    ...userFragment
  }
}

# User オブジェクトの中のフィールドを選択するためのフラグメント
fragment userFragment on User {
  login       # ログインID
  name        # ユーザー名
  url         # ユーザーの GitHub ホームページ
  websiteUrl  # ユーザーの Web サイト
  avatarUrl   # ユーザーのアバター画像
}
{{< /code >}}

`...userFragment` のような記述を、GraphQL 用語で __フラグメント・スプレッド（fragment spread)__ といいます。
その場所にフラグメントの内容を「展開する」という意味です。
JavaScript でオブジェクトを展開するときのスプレッド構文 (`...obj`) と同様のフォーマットが採用されています。

`fragment` の定義は、`query` の定義と同じ階層に記述することに注意してください。
また、__`on User`__ というのは、`User` オブジェクトのフィールドを選択するフラグメントであることを示しており、このフラグメントは `User` オブジェクトのフィールド部分でしか使えません。
上記の例では、`viewer` も `user` も `User` 型のフィールドなので、その中で問題なく `...userFragment` と参照できています。

上記のサンプルコードはあまり実用的じゃなかったので、もう少し実際のアプリで使いそうなクエリを載せておきます。
次の GraphQL クエリでは、ログイン中のユーザー情報と、そのユーザーの Issue 情報を取得しています。
Issue がアサインされているユーザーの情報を取得するために、`userFragment` フラグメントを使っています。

{{< code lang="graphql" title="GraphQL クエリ" hl_lines="3 9" >}}
query QueryLoginUser {
  viewer {
    ...userFragment
    issues(states: OPEN, last: 10) {
      nodes {
        id
        title
        assignees(last: 10) {
          nodes { ...userFragment }
        }
      }
    }
  }
}

fragment userFragment on User {
  login name url websiteUrl avatarUrl
}
{{< /code >}}

ちなみに、GraphQL ドキュメントの中でフラグメントを定義した場合は、必ずどこかでフラグメント・スプレッドを使って参照しなければいけないというルールがあります。
これは、フラグメントの定義だけを GraphQL サーバーに送ることが無駄だからです。
例えば、GitHub の GraphQL API では、フラグメントの定義だけして参照しないと、次のようなエラーが返されます。

> "Fragment basicUserInfo was defined, but not used"


インライン・フラグメント (Inline fragments)
----

__インライン・フラグメント (Inline fragments)__ は、インタフェース型や union 型として定義されたデータの中から、特定の型のオブジェクトのフィールドを参照するときに使用します（オブジェクト指向言語におけるダウンキャストと同じ概念です）。
インライン・フラグメントを使用するときは、クエリの選択セットの中に、次のように名前なしのフラグメントを定義するような構文で記述します。

```graphql
... on 型名 {
  フィールド名
  フィールド名
  フィールド名
}
```

下記は、GitHub の GraphQL API を使って、特定のリポジトリの Issue と PullRequest の一覧を取得するクエリの例です。

```graphql
query {
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
```

検索結果は `nodes` フィールドで配列として参照できるのですが、その中の各要素は `SearchResultItem` 型のオブジェクトになっています。
`SearchResultItem` は次のような union 型として定義されており、実際に各要素がどの型のオブジェクトとして返されるかは、クエリを実行してみるまで分かりません。

```graphql
"""
The results of a search.
"""
union SearchResultItem = App | Discussion | Issue | MarketplaceListing | Organization | PullRequest | Repository | User
```

それぞれの型のフィールドは当然異なっているので、そのまま汎用的な `SearchResultItem` のフィールドとして参照することはできないようになっています。
そこで次のようにインライン・フラグメントを使うことで、そのオブジェクトが `Issue` 型の場合は `number` と `title` フィールドを参照し、`PullRequest` 型の場合は `title` と `baseRefName` フィールドを参照することを宣言します。

```graphql
... on Issue {
  number
  title
}
... on PullRequest {
  title
  baseRefName
}
```

参照しようとしたオブジェクトが `Issue` 型だったときは、GraphQL サーバーは `... on PullRequest` で指定されたフィールドを返しません（その逆も同様です）。
つまり、インライン・フラグメントは、__実際に返されるオブジェクトの型によって参照するフィールドを場合分けする__ 役割を持っています。
インライン・フラグメントはこのように union 型のフィールドを参照する場合や、インタフェースのフィールドを参照する場合に使用しますが、union 型のフィールドを参照する場合は、次のように名前付きのフラグメントを使用することもできます。

```graphql
query today {
  agenda {
    ...workout
    ...study
  }
}

fragment workout on Workout {
  name
  reps
}

fragment study on StudyGroup {
  name
  subject
  students
}
```

ちなみに、union 型のフィールドを要求したときに、実際にどの型のオブジェクトが返されたかは、次のように __`typename`__ メタフィールドを参照すると分かります。
これは、GraphQL のイントロスペクション機能のひとつです。

{{< code lang="graphql" hl_lines="4" >}}
query GetIssueOrPullRequest {
  repository(owner: "facebook", name: "graphql") {
    issueOrPullRequest(number: 3) {
      __typename
      ... on Issue {
        closed
        closedAt
      }
      ... on PullRequest {
        merged
        mergedAt
      }
    }
  }
}
{{< /code >}}

{{% private %}}
- 参考: [Introspection](/p/oo8mydq/)
- 参考: [GitHub GraphQL クエリ例: イシュー情報を取得する (search)](/p/i5ht5ep/)
{{% /private %}}


Apollo Client で GraphQL のフラグメントを使用する
----

React を使った Web アプリでは、GraphQL ライブラリとして Apollo Client が使用されることが多いと思います。
もちろん、Apollo Client で使用する GraphQL クエリの中でもフラグメントを使用できます。

TypeScript (JavaScript) では、クエリ文字列の中から変数展開することができるので、フラグメントの定義は文字列変数（定数）の形で定義しておくと便利です。
次の例では、`rateLimitFragment` という GraphQL フラグメントを、`RATE_LIMIT_FRAGMENT` という文字列定数として定義しています。

{{< code lang="typescript" title="queries.ts" >}}
import {gql} from '@apollo/client'

const RATE_LIMIT_FRAGMENT = gql`
  fragment rateLimitFragment on Query {
    rateLimit {
      cost
      remaining
    }
  }
`

// 自分にアサインされた PullRequest の一覧
export const QUERY_MY_PULLS = gql`
  ${RATE_LIMIT_FRAGMENT}
  query {
    ...rateLimitFragment
    rateLimit { cost remaining }
    search(type: ISSUE, last: 100, query: "is:open is:pr review-requested:@me") {
      issueCount
      nodes {
        ... on PullRequest { number title bodyText url }
      }
    }
  }
`
{{< /code >}}

文字列定数 `RATE_LIMIT_FRAGMENT` で定義されたフラグメントを使用するために、`QUERY_MY_PULLS` の中で `${RATE_LIMIT_FRAGMENT}` と展開しています。
つまり、そこに `fragment` の定義を記述したのと同じ振る舞いになります。
あとは適切な場所で `...rateLimitFragment` と参照すれば OK です。

このようにフラグメント定義部分を文字列定数として切り出しておけば、別の GraphQL クエリの中からも同様に参照することができます。

一応、`QUERY_MY_PULLS` の使用例も。

{{< code lang="typescript" title="components/MyPullRequests.tsx" >}}
import { FC } from 'react'
import { useQuery } from '@apollo/client'
import { PullRequestLink } from './PullRequestLink'
import { QUERY_MY_PULLS } from '../queries'

export const MyPullRequests: FC = () => {
  const {loading, error, data} = useQuery(QUERY_MY_PULLS)
  if (loading) return <p>Loading ...</p>
  if (error) return <p>Error: {error.message}</p>
  const pulls = createPullRequests(data)

  return <>
    <h3>自分のレビュー待ち PR</h3>
    <ul>
      {pulls.map(x => <PullRequestLink key={x.number} pr={x}/>)}
    </ul>
  </>
}

// ...
{{< /code >}}

