---
title: "GraphQL スキーマの Lint ツール (graphql-schema-linter) を導入する"
url: "p/adxcp3g/"
date: "2022-12-21"
tags: ["GraphQL"]
---

graphql-schema-linter とは？
----

[graphql-schema-linter](https://github.com/cjoudrey/graphql-schema-linter) は、GraphQL スキーマファイル (`*.graphqls`) の記述の不備を指摘してくれる Linter です。
スキーマファーストなアプローチで GraphQL サーバー／クライアントを実装している場合は、Linter は早い段階で導入しておくことをお勧めします。


graphql-schema-linter のインストール
----

`graphql-schema-linter` コマンドは、`yarn` あるいは `npm` でインストールできます。
依存パッケージである `graphql` も一緒にインストールする必要があります（`package.json` ファイルがない場合は、先に `npm init` で作成しておいてください）。

{{< code lang="console" title="graphql-schema-linter のインストール" >}}
# yarn の場合
$ yarn add --dev graphql graphql-schema-linter

# npm の場合
$ npm install --save-dev graphql graphql-schema-linter
{{< /code >}}

次のように実行できるようになっていれば OK です。

{{< code lang="console" title="コマンドを実行できるか確認" >}}
$ npx graphql-schema-linter --help
...（ヘルプ表示）...
{{< /code >}}

コマンド名が長いので、npm スクリプトを定義して、`yarn lint` だけで実行できるようにしておくと便利です。
最低限の `package.json` ファイルは次のような感じになります。

{{< code lang="json" title="package.json" >}}
{
  "license": "UNLICENSED",
  "scripts": {
    "lint": "graphql-schema-linter graph/*.graphqls"
  },
  "devDependencies": {
    "graphql": "^16.6.0",
    "graphql-schema-linter": "^3.0.1"
  }
}
{{< /code >}}


graphql-schema-linter の実行
----

前述のような `package.json` を定義していれば、__`yarn lint`__ でスキーマの lint チェックを実行できます。

{{< code lang="console" title="lint 実行例" >}}
$ yarn lint
yarn run v1.22.17
$ graphql-schema-linter graph/*.graphqls
D:\gitwork\maku77\myapp\graph\schema.graphqls
1:1  The object type `Query` is missing a description.                          types-have-descriptions
1:1  A `PageInfo` object type is required as per the Relay spec.                relay-page-info-spec
3:9  The `hardId` argument of `games` is missing a description.                 arguments-have-descriptions
7:1  The scalar type `Hardware` is missing a description.                       types-have-descriptions
31:3 The field `Game.serial` is missing a description.                          fields-have-descriptions
38:1 The fields of object type `Maker` should be sorted in alphabetical order.  type-fields-sorted-alphabetically

× 6 errors detected
error Command failed with exit code 1.
info Visit https://yarnpkg.com/en/docs/cli/run for documentation about this command.
{{< /code >}}

オブジェクト型のフィールドがアルファベット順にソートされていないとか、コメント (description) が抜けている、といった指摘を行ってくれます。
もちろん、これに従うだけでよいスキーマ設計ができるわけではありませんが、一定の品質を保つためには導入しておくのがよさそうです。
スキーマ定義のベストプラクティスは下記を参考にしてください。

- 参考: [GraphQL ベストプラクティス](/p/4reqy9i/)

