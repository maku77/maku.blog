---
title: "Apollo CLI の codegen で GraphQL クエリレスポンスの TypeScript 型を自動生成する"
url: "p/bnrrqpn/"
date: "2021-08-11"
tags: ["Apollo", "GitHub", "GraphQL"]
---

何をするか
----

TypeScript プロジェクトにおいて、Apollo Client の `useQuery` フックで GraphQL クエリ呼び出しを行っていると、レスポンスの型情報が `any` になってしまうことに悩むことになります。
例えば、GitHub の GraphQL クエリで、次のようにログイン中のユーザー情報を取得するとします。

{{< code lang="ts" >}}
import { gql, useQuery } from '@apollo/client'

export const QUERY_VIEWER = gql`
  query QueryViewer {
    viewer {
      login
      url
      avatarUrl
    }
  }
`

const Viewer: FC = () => {
  const { error, loading, data } = useQuery(QUERY_VIEWER)
  // ...
}
{{< /code >}}

`useQuery` 関数の戻り値の `data` はデフォルトで `any` 型なので、そのままだと ESLint などに怒られることになります。

> Unsafe array destructuring of a tuple element with an `any` value  @typescript-eslint/no-unsafe-assignment

これを解消するために、`useQuery` 関数の型パラメーターで `data` の型をセットできるのですが、

{{< code >}}
const { error, loading, data } = useQuery<ViewerData>(QUERY_VIEWER)
{{< /code >}}

各クエリごとにこういった型情報を定義するのは面倒ですし、戻り値のオブジェクトが複雑だったりすると、型定義そのものが複雑で大変です。

そこで、このような GraphQL クエリの戻り値の型を自動生成してくれるのが [Apollo CLI](https://github.com/apollographql/apollo-tooling) が提供する __`apollo client:codegen`__ コマンドです。
ここでは、GitHub の GraphQL スキーマ定義ファイルを使って、クエリレスポンスの型情報を自動生成してみます。


Apollo CLI で型情報を生成するための準備
----

`apollo client:codegen` コマンドを利用するには、`apollo` コマンド自体のインストールに加え、下記のような入力ファイルを用意してやる必要があります。

1. スキーマ定義ファイル (`*.graphql`)
2. クエリリテラルを含む TypeScript コード（デフォルトで `gql` を検索します）

### Apollo CLI のインストール

まず、Apollo CLI（`apollo` コマンド）自体をインストールします。
基本的にはプロジェクトごとに `devDependencies` インストールするのがよいと思いますが、型情報の生成はそこまで頻繁に行わないので、グローバルインストールでもよいかもしれません（`node_modules` が肥大化するのは嫌ですし）。

{{< code title="apollo コマンドのインストール" >}}
### yarn の場合
$ yarn add apollo --dev
$ yarn global add apollo（グローバルインストールする場合）

### npm の場合
$ npm install apollo --save-dev
$ npm install apollo -g（グローバルインストールする場合）
{{< /code >}}

### GitHub の GraphQL スキーマ定義をダウンロード

Apollo CLI への入力ファイルとして、GraphQL のスキーマ定義を用意します。
GitHub の GraphQL API の場合は、下記のサイトから __`schema.docs.graphql`__ というファイルをダウンロードすれば OK です（サイズは 1MB 弱です）。

- [パブリックスキーマ - GitHub Docs](https://docs.github.com/ja/graphql/overview/public-schema)

`curl` コマンドや、`apollo client:download-schema` コマンドを使ってエンドポイントから直接ダウンロードする方法もありますが、GitHub アクセストークンの設定が必要だったりして面倒なので、まずは上記サイトからファイルをダウンロードしてしまうのが手っ取り早いです。

ダウンロードしたファイルは、プロジェクト内のどこかに配置しておきます。
ここでは、`graphql` ディレクトリに入れることにします。

{{< code >}}
<project>
  +-- graphql/
        +-- schema.docs.graphql
{{< /code >}}

### クエリ文字列の含まれた TypeScript コード

Apollo Client (`useQuery`) を使ったプロジェクトであれば、すでに `gql` を使ったクエリ文字列の定義は TypeScript コード内に含まれていると思います。
例えば、`src` ディレクトリ以下に、次のようなコードを含む `.ts` ファイルが存在すれば準備 OK です。

{{< code lang="ts" title="src/useViewer.ts" >}}
import { gql } from '@apollo/client'

export const QUERY_VIEWER = gql`
  query QueryViewer {
    viewer {
      login
      url
      avatarUrl
    }
  }
`

// ...
{{< /code >}}


apollo client:codegen で型情報を生成する
----

上記の準備が済んだら、次のようなコマンドで GraphQL クエリレスポンス用の型情報を生成できます。

{{< code >}}
$ apollo client:codegen --target=typescript --localSchemaFile=graphql/schema.docs.graphql
  √ Loading Apollo Project
  √ Generating query files with 'typescript' target - wrote 12 files
{{< /code >}}

言語として TypeScript を指定して、スキーマ定義としてダウンロードした `schema.docs.graphql` を指定しています。
デフォルトで、__`src`__ ディレクトリ以下の `*.ts` ファイルが検索されますが、`--includes=foo/**/*.ts` オプションなどで調整できます。
その他のオプションは、`apollo client:codegen --help` や、[公式ページのヘルプ](https://github.com/apollographql/apollo-tooling#apollo-clientcodegen-output) で調べられます。

型情報ファイルの生成に無事成功すると、デフォルトで入力ファイル (`*.ts`) と同じディレクトリに、__`__generated__`__ というディレクトリが生成され、その中にクエリ単位で `.ts` ファイルが生成されます。
今回の例では、`src/userViewer.ts` 内に `query QueryViewer {...}` という記述があるので、このクエリ名称をもとに次のようなファイルが自動生成されます。

{{< code lang="ts" title="src/__generated__/QueryViewer.ts（自動生成）" >}}
/* tslint:disable */
/* eslint-disable */
// @generated
// This file was automatically generated and should not be edited.

// ====================================================
// GraphQL query operation: QueryViewer
// ====================================================

export interface QueryViewer_viewer {
  __typename: "User";
  /**
   * The username used to login.
   */
  login: string;
  /**
   * The HTTP URL for this user
   */
  url: any;
  /**
   * A URL pointing to the user's public avatar.
   */
  avatarUrl: any;
}

export interface QueryViewer {
  /**
   * The currently authenticated user.
   */
  viewer: QueryViewer_viewer;
}
{{< /code >}}

ドキュメンテーションコメントなども、入力したスキーマ定義ファイル (`schema.docs.graphql`) から自動で適用されていていい感じです（VS Code などで編集中にこれらのドキュメントを参照できます）。
`url` や `avatarUrl` の型が `any` になってしまっている部分の対応に関しては後述します。

あとは、次のようにこのファイルをインポートして `useQuery` の型パラメーターとして使うだけです。

{{< code lang="ts" title="src/useViewer.ts" >}}
import { gql, useQuery } from '@apollo/client'
import { QueryViewer, QueryViewer_viewer } from './__generated__/QueryViewer'

export const QUERY_VIEWER = gql`
  query QueryViewer {
    viewer {
      login
      url
      avatarUrl
    }
  }
`

/**
 * サインイン中の GitHub ユーザー情報を取得します。
 * ロード中やエラー時は undefined を返します。
 */
export function useViewer(): QueryViewer_viewer | undefined {
  const { data } = useQuery<QueryViewer>(QUERY_VIEWER)
  return data?.viewer
}
{{< /code >}}

{{% note title="さらにアプリドメインの独自型へ変換すべきか？" %}}
上記のカスタムフック `useViewer` では、戻り値の型として、Apollo CLI で生成したクエリレスポンス型をそのまま使っています。
実際のプロジェクトでは、アプリドメインで定義した独自型に変換した方が都合がよいかもしれません。
いずれにしても、そういった変換処理はカスタムフックのコード内に閉じて、UI コンポーネント側に染み出さないようにするのが保守性を高めるコツです。
{{% /note %}}


クエリに引数がある場合は、`XxxVariables` という型も生成されるので、次のような感じで第2型パラメーターに指定してやります。

{{< code lang="ts" >}}
const { data } = useQuery<QueryIssues, QueryIssuesVariables>(
  QUERY_ISSUES,
  {
    variables: { milestoneNumber: milestoneNumber }
  }
)
{{< /code >}}


（応用）Prettier の設定
----

[Prettier を使ってコードを自動フォーマット](/p/au8iu6u) している場合は、`__generated__` ディレクトリを無視設定しておきましょう。

{{< code title=".prettierignore" >}}
__generated__/
.next/
build/
*.html
*.md
{{< /code >}}

ESLint の無視設定に関しては、自動生成されるファイル内に `/* eslint-disable */` というコメントが入っているので気にしなくても大丈夫です。


（応用）npm scripts で簡単に codegen 実行できるようにしておく
----

`apollo client:codegen` コマンドは、NPM scripts として定義して簡単に実行できるようにしておきましょう。

{{< code lang="js" title="package.json" >}}
{
  // ...
  "scripts": {
    // ...
    "codegen": "apollo client:codegen --target typescript --localSchemaFile=./graphql/schema.docs.graphql"
  },
  // ...
}
{{< /code >}}

これで、次のように型情報を生成できるようになります。

{{< code >}}
### yarn の場合
$ yarn codegen

### npm の場合
$ npm run codegen
{{< /code >}}


（応用）独自のタイプが any にならないようにする
----

GraphQL のスキーマ定義ファイル (.graphql) の中で、次のような独自スカラー型 (custom scalar) が定義されていると、

{{< code title="schema.docs.graphql（抜粋）" >}}
scalar URI
{{< /code >}}

そのような型が使われている部分の型情報がデフォルトで `any` になってしまいます。

{{< code lang="ts" hl_lines="10 14" title="src/__generated__/QueryViewer.ts（抜粋）" >}}
export interface QueryViewer_viewer {
  __typename: "User";
  /**
   * The username used to login.
   */
  login: string;
  /**
   * The HTTP URL for this user
   */
  url: any;
  /**
   * A URL pointing to the user's public avatar.
   */
  avatarUrl: any;
}
{{< /code >}}

これでは意味がないので、なんとか他の TypeScript 型にマッピングしてやる必要があります。
上記のカスタムスカラー型の `URI` は、少なくとも TypeScript の `string` にマッピングしてやりたいところです。
これを実現するには、例えば次のようにします。

1. `apollo client:codegen` のオプションで __`--passthroughCustomScalars`__ を指定することで、`any` ではなくカスタムスカラー型のまま出力するようにする（上記の場合は `URL` 型で出力）。
2. `apollo client:codegen` のオプションで __`--customScalarsPrefix=CustomScalar`__ を指定することで、出力する型にプレフィックスを追加してやる（上記の場合は `URL` → `CustomScalarURL` 型になる）。これは、既存の TypeScript 型とのコンフリクトを防ぐため。
3. 出力された型 (`CustomScalarURL`) に対応する型のマッピングを `globals.d.ts` などに記述する。

今回の例の場合は、カスタムスカラー型の `URL` が `CustomScalarURL` 型として出力されることになるので、グローバルな型情報としてプロジェクトルートの `globals.d.ts` に、次のような感じでマッピングを定義してやります（この例では `DateTime` 用のマッピングも追加しています）。

{{< code lang="ts" title="globals.d.ts" >}}
// apollo client:codegen が生成するカスタムスカラー型用のマッピング
type CustomScalarURI = string
type CustomScalarDateTime = string
{{< /code >}}

あとは、次のように型情報をジェネレートしてやります。

{{< code >}}
$ apollo client:codegen --target=typescript \
    --localSchemaFile=./graphql/schema.docs.graphql \
    --passthroughCustomScalars \
    --customScalarsPrefix=CustomScalar \
{{< /code >}}

生成される型情報ファイルは次のように変化するので、無事 `url` プロパティと `avatarUrl` プロパティは `CustomScalarURI` 型（=`string` 型）として参照できるようになります。

{{< code lang="ts" hl_lines="10 14" title="src/__generated__/QueryViewer.ts（抜粋）" >}}
export interface QueryViewer_viewer {
  __typename: "User";
  /**
   * The username used to login.
   */
  login: string;
  /**
   * The HTTP URL for this user
   */
  url: CustomScalarURI;
  /**
   * A URL pointing to the user's public avatar.
   */
  avatarUrl: CustomScalarURI;
}
{{< /code >}}

本当はダイレクトにカスタムスカラー型の `URI` を、TypeScript の `string` にマッピングしたいところですけど、これはこれでまぁ分かりやすいのかもです。


（応用）Apollo の設定ファイル (apollo.config.js) 
----

前述の実行例では、`apollo client:codegen` のコマンドライン引数でスキーマ定義ファイルなどを指定していました。

{{< code >}}
$ apollo client:codegen --target=typescript --localSchemaFile=graphql/schema.docs.graphql
{{< /code >}}

プロジェクトのルートディレクトリに、__`apollo.config.js`__ というコンフィグファイルを作成しておくと、その設定を `apollo client:codegen` が読み込んでくれるようになります。
複雑な設定が必要になってきたら、このファイルを作成してしまうとよいです。

{{< code lang="js" title="apollo.config.js" >}}
module.exports = {
  client: {
    // includes: ['app/**/*.ts', 'app/**/*.tsx'],  // src 以外のディレクトリを検索したいとき
    // tagName: 'graphql',  // ts コード内のクエリリテラルで `gql` 以外を使う場合
    service: {
      name: 'github',
      localSchemaFile: './graphql/schema.docs.graphql'
    },
  }
};
{{< /code >}}

ここでは、`client.service.localSchemaFile` プロパティでスキーマ定義ファイルの場所を設定しているので、コマンド実行時のオプション指定を次のように省略できます。

{{< code >}}
$ apollo client:codegen --target=typescript
{{< /code >}}

`apollo.config.js` の詳しい設定方法は下記の公式ドキュメントを参照してください。

- [Configuring Apollo projects - Apollo Basics - Apollo GraphQL Docs](https://www.apollographql.com/docs/devtools/apollo-config/)


トラブルシューティング
----

### Apollo does not support anonymous operations

`apollo client:codegen` コマンドを実行したときに、次のようなエラーになったら、

{{< code >}}
$ apollo client:codegen --target=typescript
  √ Loading Apollo Project
  × Generating query files with 'typescript' target
    → Apollo does not support anonymous operations
    GraphQLError: Apollo does not support anonymous operations
{{< /code >}}

TypeScript コードの中で `gql` を使って定義したクエリに名前が付いていないものがあります。
次のように、名前を付けてやれば解決します。

{{< code lang="ts" >}}
const QUERY_TEAMS = gql`
  query {
    ...
  }
`
{{< /code >}}

（↑のコードを↓のように変更する）

{{< code lang="ts" >}}
const QUERY_TEAMS = gql`
  query QueryTeams {
    ...
  }
`
{{< /code >}}

これで、うまく実行できるようになります。

{{< code >}}
$ apollo client:codegen --target=typescript
  √ Loading Apollo Project
  √ Generating query files with 'typescript' target - wrote 12 files
{{< /code >}}

