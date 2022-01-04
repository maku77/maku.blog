---
title: "GraphQL のクエリの一部をフラグメント化して再利用する (Fragments)"
url: "/p/wiv7it5"
date: "2021-01-19"
tags: ["GitHub", "GraphQL"]
---

GraphQL のフラグメントを定義する
----

GraphQL クエリの中で、同じようなフィールドの指定（選択セット）を複数回使用する場合、それを __フラグメント (Fragment)__ という再利用可能な選択セットとして切り出して定義しておくことができます。

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
フラグメントを使用する場所では、`...userFragment` のようにドットを 3 つ付けて参照します（JavaScript のスプレッド構文と同じです）。

{{< code lang="graphql" title="GraphQL クエリ" >}}
query QueryTwoUsers {
  viewer {
    ...userFragment
  }

  user(login: "ログインID") {
    ...userFragment
  }
}

fragment userFragment on User {
  login       # ログインID
  name        # ユーザー名
  url         # ユーザーの GitHub ホームページ
  websiteUrl  # ユーザーの Web サイト
  avatarUrl   # ユーザーのアバター画像
}
{{< /code >}}

`fragment` の定義は、`query` の定義と同じ階層に記述することに注意してください。
また、__`on User`__ というのは、`User` オブジェクトのフィールドを選択するフラグメントであることを示しており、このフラグメントは `User` オブジェクトのフィールド部分でしか使えません。
上記の例では、`viewer` も `user` も `User` 型のフィールドなので、その中で問題なく `...userFragment` と参照できています。

上記のサンプルコードはあまり実用的じゃなかったので、もう少し本物のアプリで使いそうなクエリを載せておきます。
次の GraphQL クエリでは、ログイン中のユーザー情報と、そのユーザーの Issue 情報を取得しています。
Issue がアサインされているユーザーの情報を取得するために、`userFragment` フラグメントを使っています。

{{< code lang="graphql" title="GraphQL クエリ" >}}
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


Apollo Client で GraphQL のフラグメントを使用する
----

React を使った Web アプリでは、GraphQL ライブラリとして Apollo Client が使用されることが多いと思います。
もちろん、Apollo Client で使用する GraphQL クエリの中でもフラグメントを使用できます。

TypeScript (JavaScript) では、クエリ文字列の中から変数展開することができるので、フラグメントの定義は文字列変数（定数）の形で定義しておくと便利です。
次の例では、`rateLimitFragment` という GraphQL フラグメントを、`RATE_LIMIT_FRAGMENT` という文字列定数として定義しています。

{{< code lang="typescript" title="queries.ts" >}}
import {gql} from '@apollo/client';

const RATE_LIMIT_FRAGMENT = gql`
  fragment rateLimitFragment on Query {
    rateLimit {
      cost
      remaining
    }
  }
`;

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
`;
{{< /code >}}

文字列定数 `RATE_LIMIT_FRAGMENT` で定義されたフラグメントを使用するために、`QUERY_MY_PULLS` の中で `${RATE_LIMIT_FRAGMENT}` と展開しています。
つまり、そこに `fragment` の定義を記述したのと同じ振る舞いになります。
あとは適切な場所で `...rateLimitFragment` と参照すれば OK です。

このようにフラグメント定義部分を文字列定数として切り出しておけば、別の GraphQL クエリの中からも同様に参照することができます。

一応、`QUERY_MY_PULLS` の使用例も。

{{< code lang="typescript" title="components/MyPullRequests.tsx" >}}
import * as React from 'react';
import {useQuery} from '@apollo/client';
import {PullRequestLink} from './PullRequestLink';
import {QUERY_MY_PULLS} from '../queries';

export const MyPullRequests: React.FC = () => {
  const {loading, error, data} = useQuery(QUERY_MY_PULLS);
  if (loading) return <p>Loading ...</p>;
  if (error) return <p>Error: {error.message}</p>;
  const pulls = createPullRequests(data);
  return <>
    <h3>自分のレビュー待ち PR</h3>
    <ul>
      {pulls.map(x => <PullRequestLink key={x.number} pr={x}/>)}
    </ul>
  </>;
};

// ...
{{< /code >}}

