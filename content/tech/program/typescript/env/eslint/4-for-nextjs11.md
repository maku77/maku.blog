---
title: "ESLint (4) ESLint の設定方法まとめ (for Next.js 11)"
url: "/p/dexgg8o"
date: "2021-07-19"
tags: ["ESLint", "Next.js"]
weight: 304
---

Next.js 11 が ESLint を組み込みサポート
----

[Next.js 11 で ESLint を Next.js が組み込みでサポートしました](https://nextjs.org/docs/basic-features/eslint)。
これで、`create-next-app` で新規作成するアプリでは、ESLint に関してもゼロコンフィグで `next eslint` とするだけで実行できるようになります。

ただ、ESLint には色々な共有設定 (config) があるわけで、Next.js がデフォルトで設定してくれているもの以外（TypeScript や Prettier 関連）は自分で設定する必要があります。
ここでは、Next.js 11 がデフォルトで提供する `.eslintrc` にそれらの設定を追加します。


Next.js 11 デフォルトの ESLint 設定
----

Next.js 11 が生成する `.eslintrc` ファイルは次のようなシンプルなものです。

{{< code lang="json" title=".eslintrc" >}}
{
  "extends": ["next", "next/core-web-vitals"]
}
{{< /code >}}

問題はこれらがどのような共有設定を含んでいるかですが、[eslint-config-next](https://github.com/vercel/next.js/blob/canary/packages/eslint-config-next/index.js
) のコードを見ると、次のような感じになっています。

{{< code lang="js" >}}
module.exports = {
  extends: [
    'plugin:react/recommended',
    'plugin:react-hooks/recommended',
    'plugin:@next/next/recommended',
  ],
  // ...
}
{{< /code >}}

React 系の共有設定はここに含まれているので、`"extends": ["next"]` だけでカバーできそうです。
[公式ドキュメントの ESLint - Base Configuration](https://nextjs.org/docs/basic-features/eslint#base-configuration) にも同様の説明が書かれています。
逆に、TypeScript 関連や、Prettier 関連の共有設定は自分で追加しなければいけません。


ESLint 設定を追加する
----

ここでは、下記のような ESLint extends 設定を手動で追加してやります。

- ESLint 推奨 (`eslint:recommended`)
- TypeScript 関連 (`plugin:@typescript-eslint/recommended*`)
- Prettier 関連 (`prettier`)

`eslint:recommeded` 以外は外部パッケージとして提供されているので、先にインストールしておきます。

{{< code title="必要なパッケージ（Plugin & Config）のインストール" >}}
$ yarn add --dev @typescript-eslint/eslint-plugin
$ yarn add --dev eslint-config-prettier
{{< /code >}}

そして、ESLint 設定ファイルを次のように書き換えてやります。
Yaml 形式の方が記述が楽なので、`.eslintrc` を `.eslintrc.yml` に置き換えています。

{{< code lang="yaml" title=".eslintrc.yml" >}}
# これがルートの設定ファイル（上位ディレクトリを検索しない）
root: true

extends:
  - eslint:recommended
  - plugin:@typescript-eslint/recommended
  - plugin:@typescript-eslint/recommended-requiring-type-checking
  - next
  - next/core-web-vitals
  - prettier

parserOptions:
  # extends で指定している
  # plugin:@typescript-eslint/recommended-requiring-type-checking
  # に対して型情報を提供するため tsconfig.json の場所を指定。
  project: ./tsconfig.json
{{< /code >}}

あとは、`next lint` で実行すればよさそうですが、プロジェクト内の TypeScript 型情報に関する警告 (`plugin:@typescript-eslint/recommended-requiring-type-checking`) を有効にするには、__`eslint`__ コマンドを直接実行しないといけないみたいです（`next lint` では全ての警告が出ない…）。

{{< code >}}
$ npx eslint src --ext .ts,.tsx
{{< /code >}}

いずれにしても、最終的には、`package.json` の中で次のような感じで npm-scripts を定義して、__`yarn lint`__ (`npm run lint`) コマンド一発で各種 Lint ツールをまとめて実行できるようにしておくのがよいです。

{{< code lang="js" title="package.json（抜粋）" >}}
{
  "scripts": {
    // ...
    "lint": "run-p -c -l lint:**",
    "lint:prettier": "prettier --check src",
    "lint:eslint": "eslint src --ext .ts,.tsx",
    "lint:tsc": "tsc",
    "fix": "run-s -l fix:**",
    "fix:prettier": "prettier --write src",
    "fix:eslint": "eslint src --ext .ts,.tsx --fix",
    // ...
{{< /code >}}

上記で使用している `run-p` や `run-s` は、複数の npm-scripts を並列 or 順次実行するためのコマンドで、`npm-run-all` パッケージをインストールすると使えるようになります。

{{< code >}}
$ yarn add npm-run-all --dev
{{< /code >}}

