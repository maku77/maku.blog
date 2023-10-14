---
title: "Next.js で HelloWorld（プロジェクト作成からサーバー起動まで）"
url: "p/ubmu3bj/"
date: "2021-04-18"
lastmod: "2021-04-24"
tags: ["Next.js", "React"]
weight: 101
---

Next.js とは
----

{{< image w="250" src="img-001.svg" >}}

[Next.js](https://nextjs.org/) は、React.js アプリ開発に必要なアレコレを詰め込んだパッケージです。
React.js を素の状態で導入すると、他にもいろいろなツールやライブラリを組み合わせていくことになるのですが、__Next.js を使うとモダンな Web アプリ開発に必要なものが一気に揃えられます__。

React.js は基本的に SPA（シングルページアプリ）を想定していますが、Next.js では複数ページによる構成や、サーバーサイドレンダリングなどをサポートしています。
これは、Next.js が単なる React.js 用のコンポーネントライブラリでないことを示しています。

Next.js には次のような特徴があります。

- __ゼロコンフィグ__: 何も設定しなくても、最初から開発サーバーの立ち上げ、プロダクト用ビルドを行えるようになっています。これらは、最適なパフォーマンスが出るようメンテナンスが続けられています。カスタマイズしたいときも、複雑怪奇な `webpack.config.js` を作成する必要はなく、`next.config.js` による最小限の設定で済みます。できれば何も設定しないのが理想です。
- __SSR/SSG によるプリレンダリング__: React.js はクライアントサイドで JavaScript を実行する CSR (Client Side Rendering) で動作しますが、Next.js は SSR (Server Side Rendering) や SSG (Static Site Generation) をサポートしています。これは、一般的な Node.js の API を利用してページを構築できることを意味します。開発元の Vercel は、パフォーマンスの面から SSG を推奨しています。
- __TypeScript をサポート__: いまどきの Web アプリ開発では TypeScript はほぼ必須なので、標準サポートはうれしいです。空の `tsconfig.js` を置くだけで、TypeScript 対応は完了です（設定は自動で行われます）。
- __ファイルベースのルーティング__: `pages` ディレクトリ以下の構成をそのままルーティング用のリンク名として使うことができます。例えば、`pages/xxx/yyy.js` は、`/xxx/yyy` というリンク名にマッピングされます。React Router によるルーティング設定が面倒だと感じている人にとってはうれしいかもしれません。
- __部分的な高速リロード (Fast Refresh)__: Next.js の開発サーバーを使って開発しているときに、React コンポーネントのステートを保ったままコード修正を反映できます。例えば、フォームに入力された値を保ったままページ内容を部分的に更新できます。
- __サーバーレス API__: アプリ作成用に、簡易な Web API を作る機能が備わっています。`pages/api` ディレクトリに JS ファイルを置くだけで API の完成です。
- __CSS Modules & Sass__: コンポーネントから CSS ファイルを import できるようにする CSS modules の仕組みを標準サポートしています。もちろん生の CSS ではなく、Sass で記述できます。生の CSS を使った場合も、ベンダープレフィックスや Flexbox の不具合などを考慮してくれるため、ブラウザの互換性を考慮する必要がありません。CSS-in-JS な styled-jsx もデフォルトで使えます。
- __コードスプリッティング__: 効率的なウェブサイト表示のためのコードスプリッティング機能をサポートしています。これは自動で行われるため、特に意識しなくても最適な形で Web サイトをデプロイできます。


Next.js の Hello World アプリを作成する
----

### create-next-app によるアプリ生成

次のように __`create-next-app`__ コマンドを実行すると、カレントディレクトリに Next.js アプリの雛形として `myapp` ディレクトリが作成されます（Node.js はインストール済みとします）。

{{< code title="Next.js アプリの新規作成" >}}
$ npx create-next-app myapp

Creating a new Next.js app in /Users/maku/myapp.
Installing react, react-dom, and next using npm...
...
Initialized a git repository.
Success! Created myapp at /Users/maku/myapp
{{< /code >}}

### 開発サーバーで動作確認

各種 NPM パッケージのインストールが完了したら、次のようにして開発用のサーバーを起動できます。

{{< code title="開発サーバーの起動" >}}
$ cd myapp
$ npm run dev
{{< /code >}}

Web ブラウザで http://localhost:3000/ にアクセスして、次のようなページが表示されれば成功です。

{{< image w="500" border="true" src="img-002.png" >}}

### ディレクトリ構成を見てみる

`create-next-app` で作成した Next.js アプリのディレクトリ構成は次のような感じになっています。
GitHub へ Push する準備までできています（`create-react-app` と同様）。

{{< code >}}
myapp/
  +-- .git/ ... Git リポジトリ
  +-- .next/ ... サーバー実行時に生成される
  +-- node_package/ ... NPM パッケージ
  +-- pages/ ... 表示する Web ページ（`.js` や `.ts` などを格納する）
  +-- public/ ... 画像などの静的リソース
  +-- styles/ ... スタイルシート
  +-- .gitignore ... Git 用の無視ファイルリスト
  +-- package.json ... Node.js 設定ファイル
  +-- README.md ... GitHub 用のリードミー
{{< /code >}}

Web サイトのトップページを提供するファイルは、__`pages/index.js`__ です。
Next.js の開発サーバーを起動した状態で、このファイルの内容を書き換えると、自動的に Web ブラウザがリロードされます。
特徴的なのは、__HTML ファイルが存在しない__ ということでしょうか。
Next.js では、コードの主体は JavaScript ファイルであり、HTML ファイルを作成する必要はありません。


Next.js アプリをホスティングする（Next.js サーバー）
----

Next.js の SSR（サーバーサイドレンダリング）機能やサーバーレス API を使ったアプリケーションは、Next.js サーバー上で動作させる必要があります。

### 自分でホスティングする場合 (next server)

任意の PC 上で本番環境用の Next.js サーバーを起動するには次のようにします。

{{< code >}}
$ npm run build  # ビルド（.next ディレクトリが使われます）
$ npm start      # サーバー起動
{{< /code >}}

開発時は `npm run dev` で自動ビルドまでセットになった開発サーバーを起動しましたが、リリース用には `npm run build` で明示的にビルドし、その内容を `npm start` の本番サーバーでホスティングします。

ちなみに、Next.js サーバーのポート番号を変更するには、`next start` コマンドの `-p` オプションを使います。

{{< code lang="js" title="package.json（抜粋）" >}}
"start": "next start -p 12340",
{{< /code >}}


### Vercel サービスを使う場合

Next.js の開発元である Vercel は、同名の __Vercel__ という Next.js アプリホスティング用のサービスを提供しています。
非営利目的で個人で使う分には無料で使えます。

Vercel のサイト上で GitHub リポジトリとの連携設定を行うだけで、簡単に Next.js アプリを公開できます。
Web サイトを更新したいときは、変更内容を GitHub へプッシュするだけで、Vercel サービスが自動的にビルドして反映してくれます（GitHub Actions の設定すらいりません）。
変更を事前確認したいときなどは、GitHub 上で PullRequest を作成すれば、確認用のサイトを生成してくれます。
いたれりつくせりです。

{{% maku-common/reference %}}
- [Next.js アプリを Vercel で公開する](/p/j6ht5fq)
{{% /maku-common/reference %}}


静的 HTML のエクスポート
----

Next.js の [Static HTML Export](https://nextjs.org/docs/advanced-features/static-html-export) 機能を使うと、Next.js アプリを静的な HTML ファイルとしてビルドできます。
これであれば、静的なファイルしかホスティングできない Web サーバーでも、Next.js アプリを公開できます。

Static HTML Export を行うには、Production ビルド (`next build`) を実行した後に、__`next export`__ コマンドを実行します。
すると、ビルド結果が __`out`__  ディレクトリに生成されます。

{{< code >}}
$ npx next build   # TypeScript (JavaScript) のビルド
$ npx next export  # 静的 HTML の形でエクスポート
...
$ ls out
404.html     _next/       favicon.ico  index.html   vercel.svg
{{< /code >}}

`package.json` の NPM スクリプトとして、次のように __`export`__ スクリプト（および `preexport`）を定義しておくと便利です。

{{< code lang="js" title="package.json" >}}
  ...
  "scripts": {
    "dev": "next dev",
    "build": "next build",
    "preexport": "npm run build",
    "export": "next export"
  },
  ...
{{< /code >}}

あとは、次のように実行すれば、デプロイ用の `out` ディレクトリが作成されます。

{{< code >}}
$ npm run export
{{< /code >}}

{{% maku-common/reference %}}
- [Next.js アプリを GitHub Actions でビルドして GitHub Pages で公開する](/p/au8ju6g)
{{% /maku-common/reference %}}


次のステップ
----

Next.js のアプリは TypeScript で作成することをオススメします。

- [Next.js のプロジェクトを TypeScript 化する](/p/ny9fmty)

