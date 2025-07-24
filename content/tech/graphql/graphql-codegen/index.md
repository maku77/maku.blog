---
title: "GraphQL Code Generator でスキーマから TypeScript の型情報を生成する (graphql-codegen)"
url: "p/n2k2hxd/"
date: "2022-10-03"
tags: ["GraphQL"]
changes:
  - 2022-10-14: ESLint の無視設定について
---

GraphQL Code Generator とは
----

__GraphQL Code Generator (graphql-codegen)__ は、GraphQL スキーマ (`schema.graphqls`) とクエリ用のドキュメント (`query {...}`) を入力として与えることで、さまざまな言語とライブラリに対応した型情報を生成するツールです。

- [公式 Web サイト - GraphQL Code Generator](https://www.the-guild.dev/graphql/codegen)
- [GitHub リポジトリ - dotansimha/graphql-code-generator](https://github.com/dotansimha/graphql-code-generator)

{{< image src="img-001.drawio.svg" title="graphql-codegen の動作概要" >}}

C# や Dart 用のコードも生成できるようですが、ツール自体が NPM パッケージとして提供されているので、やはり TypeScript がメインターゲットですね。
クライアント側で、React + Apollo Client のフレームワークを使用している場合は、Apollo Client のフック API (`useQuery`) に渡すためのオブジェクトを生成してくれます。
GraphQL サーバー側のリゾルバー実装に使用する型情報を生成するのにも使用できますが、[GraphQL サーバーを Golang の gqlgen で作る](https://maku77.github.io/p/v48adgi/) 場合などは、スキーマから Golang コードを生成する仕組みがそれ自体に備わっていたりするので、そこではこのツールは使いません。
一方、フロントエンドは React + TypeScript で作ることが比較的多いので、このツールの出番も多くなります。

入力するスキーマ情報としては、スキーマファイル（`schema.graphqls` など）を指定することも、GraphQL サーバーのアドレスを指定して Introspection 機能で取得することもできます。
GraphQL クライアントの開発中は、接続先の GraphQL サーバーも動いているでしょうから、そこから直接スキーマ情報を取得するのは理にかなっています。
例えば、GraphQL サーバー側の Git リポジトリでスキーマファイルを管理している場合、クライアント開発時にこのスキーマファイル自体を共有しなくて済みます。

ここでは、クライアントアプリを __TypeScript + React (Apollo Client)__ の組み合わせで作ることを想定して、GraphQL クエリ用の型情報を自動生成してみます。


CLI ツールのインストール
----

GraphQL Code Generator のコマンドラインツール (CLI) は __`@graphql-codegen/cli`__ パッケージとして提供されています。
開発中にのみ使用するツールなので、__`-D`__ オプションを付けて `devDependencies` としてインストールします。

```console
# yarn の場合
$ yarn add graphql
$ yarn add -D typescript @graphql-codegen/cli

# npm の場合
$ npm install graphql
$ npm install -D typescript @graphql-codegen/cli
```

これにより、`node_modules/.bin` 以下に __`graphql-codegen`__ というコマンドがインストールされます。
`graphql-code-generator` というコマンドもインストールされますが、ただのエイリアスなので前者の短いコマンドを使えば OK です。
次のようにヘルプを確認できます（`-s` オプションは `yarn` の冗長な出力を抑制するオプションです）。

```console
$ yarn -s graphql-codegen --help
Options:
      --help         Show help                                         [boolean]
      --version      Show version number                               [boolean]
  -c, --config       Path to GraphQL codegen YAML config file, defaults to
                     "codegen.yml" on the current directory             [string]
  -w, --watch        Watch for changes and execute generation automatically. You
                     can also specify a glob expression for custom watch list.
  -r, --require      Loads specific require.extensions before running the
                     codegen and reading the configuration [array] [default: []]
  -o, --overwrite    Overwrites existing files                         [boolean]
  -s, --silent       Suppresses printing errors                        [boolean]
  -e, --errors-only  Only print errors                                 [boolean]
      --profile      Use profiler to measure performance               [boolean]
  -p, --project      Name of a project in GraphQL Config                [string]
  -v, --verbose      output more detailed information about performed tasks
                                                      [boolean] [default: false]
  -d, --debug        Print debug logs to stdout       [boolean] [default: false]
```


ツールの初期セットアップ
----

最初に __`graphql-codegen init`__ コマンドを使って、設定ファイル (`codegen.ts`) を生成します。
ウィザード形式で質問されるので、プロジェクトで使用しているライブラリなどを選択していきます。

```console
$ yarn graphql-codegen init  # yarn の場合
$ npx graphql-codegen init   # npm (npx) の場合
```

下記は回答項目の例ですが、指示に従っていけば簡単に終わります。

- What type of application are you building? Application built with __`React`__
- Where is your schema?: (path or url) __`http://localhost:8080/graphql`__
- Where are your operations and fragments?: __`src/**/*.tsx`__
- Where to write the output: __`src/gql/`__
- Do you want to generate an introspection file? __`No`__
- How to name the config file? __`codegen.ts`__
- What script in package.json should run the codegen? __`codegen`__

入力が終わると、設定ファイルが生成されます。

{{< code lang="ts" title="codegen.ts" hl_lines="5 6" >}}
import type { CodegenConfig } from '@graphql-codegen/cli';

const config: CodegenConfig = {
  overwrite: true,
  schema: "http://localhost:8080/graphql",
  documents: "src/**/*.tsx",
  generates: {
    "src/gql/": {
      preset: "client",
      plugins: []
    }
  }
};

export default config;
{{< /code >}}

__`schema`__ プロパティが「入力するスキーマ」の場所を示していて、__`documents`__ プロパティが「クエリ（ドキュメント）」を抽出する場所を示しています。
`documents` プロパティのデフォルトは `.tsx` ファイルのみを参照するようになっていますが、__`.ts` ファイルも参照するように__ 次のように修正しておいた方がよいかもしれません。

```ts
documents: "src/**/*.{ts,tsx}",
```

設定ファイルは `.ts` 形式ではなく、`.yml` 形式で作成することもできます。
設定ファイルの編集時に入力補完されなくてもよいのであれば、YAML 形式を使った方がスッキリするかもしれません。

{{< code lang="yaml" title="codegen.yml" >}}
overwrite: true
schema: 'http://localhost:8080/graphql'
documents: 'src/**/*.{ts,tsx}'
generates:
  src/gql/:
    preset: 'client'
{{< /code >}}

`graphql-codegen init` コマンドは `package.json` を更新することがあるので、依存パッケージをインストールしておきます（これも指示される通り実行すれば OK）。

```console
$ yarn         # yarn の場合
$ npm install  # npm の場合
```


型情報の自動生成
----

GraphQL Code Generator（`graphql-codegen` コマンド）のセットアップが完了したので、実際に React (Apollo Client) アプリの型情報を自動生成してみます。
React + Apollo Client のアプリのコードは次のような感じになっていると思います。
GraphQL API を呼び出すための、`useQuery` フックに渡す `Games` 型をマニュアル定義しています。

{{< code lang="tsx" title="src/components/MyComponent.tsx" hl_lines="27" >}}
import { FC } from 'react'
import { gql, useQuery } from '@apollo/client'

// GraphQL ドキュメント（これが graphql-codegen への入力になる）
export const QUERY_GAMES = gql`
  query QueryGames {
    games {
      title
      date
      genre
    }
  }
`

// 自力で型定義（これをやめたい！）
type Games = {
  games: Game[]
}

type Game = {
  title: string
  date?: string
  genre?: string
}

export const MyComponent: FC = () => {
  const { data, error, loading } = useQuery<Games>(QUERY_GAMES)
  if (error) return <b>Error: {error.message}</b>
  if (loading) return <b>Loading...</b>
  const games = data?.games ?? []
  return
    <ul>
      {games.map((g) => <li key={g.title}>{g.title} {g.date}</li>)}
    </ul>
  )
}
{{< /code >}}

このコードには、Apollo Client の __`gql`__ 関数の引数部分に GraphQL ドキュメントが記述されており、この文字列は __`graphql-codege`__ コマンドが型情報を生成するための入力情報として参照します。
次のように実行すると、型情報のコードを生成できます。

{{< code lang="console" title="コード生成" >}}
$ yarn graphql-codegen --config codegen.ts  # yarn の場合
$ npx graphql-codegen --config codegen.ts   # npm (npx) の場合
{{< /code >}}

実は、最初に `graphql-codegen init` コマンドを実行したときに、上記と同じことをする NPM スクリプトを `package.json` に追加してくれているので、次のようなショートカットで実行することもできます。

{{< code lang="console" title="コード生成（同上）" >}}
$ yarn codegen     # yarn の場合
$ npm run codegen  # npm の場合
{{< /code >}}

すると、設定ファイル (`codegen.ts`) で指定したディレクトリ (今回は `src/gql`) にいくつかの `.ts` ファイルが生成されます。
例えば次のような型が定義されています。

{{< code lang="ts" title="自動生成された型情報の一部" >}}
export type QueryGamesQuery = {
  __typename?: 'Query',
  games: Array<{
    __typename?: 'Game',
    title: string,
    date?: string | null,
    genre?: string | null
  }>
};
{{< /code >}}

型情報が生成されたあとは、元のコードを書き換えて Apollo Client が提供する `gql` 関数の代わりに、自動生成された __`graphql`__ 関数を使ってクエリ（ドキュメント）を定義するようにします。
この関数は `TypedDocumentNode` という型のオブジェクトを返すようになっており、これを `useQuery` フックに渡すと、戻り値の `data` オブジェクトが自動的に型付けされるようになります。

{{< code lang="ts" title="src/components/MyComponent.tsx" hl_lines="2 6 18" >}}
import { FC } from 'react'
import { graphql } from '../gql'
import { useQuery } from '@apollo/client'

// TypedDocumentNode という型のオブジェクトが返される
export const QUERY_GAMES = graphql(`
  query QueryGames {
    games {
      title
      date
      genre
    }
  }
`)

export const MyComponent: FC = () => {
  // こうするだけで型情報付きの data としてアクセスできる
  const { data, error, loading } = useQuery(QUERY_GAMES)
  // ...
}
{{< /code >}}

これで GraphQL クエリ用の型情報を自力で定義する必要がなくなりました！
ちなみに、自動生成された `graphql` 関数は、引数で渡す GraphQL ドキュメント（文字列リテラル）をキーにして戻り値を決めるという実装になっているため、文字列リテラルを 1 文字でも変えたら（スペースでも）、`graphql-codegen` によるコードの再生成が必要になることに注意してください。


カスタムスカラー型のマッピング
----

GraphQL のスキーマ定義の中で次のようにカスタムスカラー型が定義されている場合、`graphql-codegen` はデフォルトで TypeScript の __`any`__ 型にマッピングしようとします。

{{< code lang="graphql" title="GraphQL スキーマ定義（抜粋）" >}}
scalar DateTime
scalar Email
scalar URI
{{< /code >}}

これらのカスタムスカラー型を TypeScript の `string` 型にマッピングしたいときは、`codegen.yml` の中で次のように __`scalars`__ プロパティを設定します。

{{< code lang="yaml" title="codegen.yml" >}}
overwrite: true
schema: 'http://localhost:8080/graphql'
documents: 'src/**/*.{ts,tsx}'
generates:
  src/gql/:
    preset: 'client'
    config:
      scalars:
        DateTime: string
        Email: string
        URI: string
{{< /code >}}

注: `scalars` オプションを使うには `@graphql-codegen/client-preset` パッケージ v1.0.4 以上が必要です（v1.0.3 までは不具合で `scalars` オプションを認識せず）


自動生成されたコードを ESLint の解析対象外にする
----

ESLint による TypeScript コードの静的解析を行っている場合は、`graphql-codegen` で生成されたコードに対する警告がいっぱい出ると思います。
[ESLint の無視設定](https://eslint.org/docs/latest/user-guide/configuring/ignoring-code) で、`graphql-codegen` の出力先ディレクトリを無視するように設定しましょう。

{{< code lang="yaml" title=".eslintrc.yml" >}}
root: true
# 自動生成されるコードは ESLint の解析対象外にする
ignorePatterns:
  - src/gql/
{{< /code >}}

