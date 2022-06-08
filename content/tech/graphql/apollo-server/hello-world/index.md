---
title: "Apollo Server で GraphQL サーバーを作成する (Hello World)"
url: "p/q7q4ahp/"
date: "2022-06-08"
tags: ["Apollo Server"]
---

何をするか？
----

[apollo-server](https://www.npmjs.com/package/apollo-server) NPM パッケージは、Node.js で GraphQL サーバーを実装するためのライブラリです。
Apollo Graph 社が OSS として公開しており、2022 年時点で最も使用されている JavaScript 用の GraphQL ライブラリです。

ここでは、簡単な GraphQL スキーマとリゾルバーを定義して、GraphQL サーバーを立ち上げてみます。
`apollo-server` は標準で TypeScript の型情報に対応しているため、ここでは TypeScript を使って実装しますが、JavaScript でもほぼ同様に実装できます。


TypeScript 用プロジェクトの準備
----

TypeScript の実行環境自体は、下記の記事などを参考にしてセットアップします。

- 参考: [TypeScriptの環境: Visual Studio Code で TypeScript の開発環境を構築する](/p/ak7u3h3/)

でもここでは、`.ts` ファイルをそのまま実行できる [ts-node](https://typestrong.org/ts-node/) を使って楽をしちゃいます。

{{< code lang="console" title="プロジェクトの作成" >}}
$ mkdir myserver && cd myserver
$ npm init -y
$ npm install --save-dev typescript ts-node
{{< /code >}}

`package.json` を少し修正して、`npm run dev` で `main.ts` を起動できるようにしておきます。

{{< code lang="js" title="package.json" >}}
// ...
"scripts": {
  "dev": "ts-node main"
}
{{< /code >}}

これで準備完了です。


apollo-server と graphql のインストール
----

GraphQL サーバーインスタンスを立ち上げるための [apollo-server](https://www.npmjs.com/package/apollo-server) パッケージと、GraphQL スキーマを扱うための [graphql](https://www.npmjs.com/package/graphql) パッケージをインストールします。

```console
$ npm install --save-dev apollo-server graphql
```


GraphQL サーバーを実装する
----

Apollo Server ライブラリを使って GraphQL サーバーを実装していきます。
ここでは、`Query` のルートフィールドとして `hello` フィールドを定義して、そのリゾルバー関数で `world` を返すように実装します。

{{< code lang="ts" title="main.ts" >}}
import { ApolloServer, gql } from 'apollo-server'

// GraphQL スキーマの定義
const typeDefs = gql`
  type Query {
    hello: String!
  }
`

// リゾルバーの定義
const resolvers = {
  Query: {
    hello: () => 'world',
  },
}

// GraphQL サーバーを起動する
const server = new ApolloServer({ typeDefs, resolvers })
server.listen().then(({ url }) => {
  console.log(`🚀 Server ready at ${url}`)
})
{{< /code >}}

Hello World なのでとてもシンプル！
次のように実行すれば GraphQL サーバーが立ち上がります。

```console
$ npm -s run dev
🚀 Server ready at http://localhost:4000/
```

デフォルトで 4000 番ポートで起動しますが、`server.listen({ port: 5000 })` のように変更できます。


GraphQL クエリを実行してみる
----

立ち上げた GraphQL サーバーに対してクエリ実行してみます。
どんな GraphQL クライアントを使ってもよいのですが、[Apollo Studio Explorer](https://studio.apollographql.com/sandbox?endpoint=http%3A%2F%2Flocalhost%3A4000%2F) というサイトから、簡単に `localhost:4000` に対して GraphQL クエリを投げられます。

{{< image border="true" src="img-001.png" title="Apollo Studio Explorer からのクエリ実行" >}}

`hello` フィールドを参照すると、正しく `world` という値が返ってきていることを確認できます。

`curl` コマンドで直接 GraphQL API を呼び出して確認することもできます。

```console
$ curl --request POST \
       --header 'content-type: application/json' \
       --url http://localhost:4000/ \
       --data '{"query":"query { hello }"}'

{"data":{"hello":"world"}}
```

