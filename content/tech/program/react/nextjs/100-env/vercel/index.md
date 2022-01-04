---
title: "Next.js アプリを Vercel で公開する"
url: "/p/j6ht5fq"
date: "2021-05-07"
tags: ["Next.js", "Vercel"]
weight: 111
---

Vercel とは？
----

Vercel は Next.js を開発している企業であり、同名の Vercel というサービスを提供しています。

- [Vercel - Develop. Preview. Ship. For the best frontend teams](https://vercel.com/)

Vercel サービスは Next.js アプリのホスティングに特化しており、GitHub などのリポジトリと連携させることにより、ビルドから Next.js サーバーによるホスティングまでを簡単に自動化することができます（実際には、Vercel サービスは、Next.js 以外で作成した Web サイト、例えば、[Hugo](https://gohugo.io/) で作成したブログなどのホスティングにも対応しています）。

Next.js アプリで SSR (Server-side Rendering) や API ルートなどの機能を使用している場合は、Next.js サーバー上で Web サイトをホスティングする必要があるのですが、現状そのような環境をサーバーレスなサービスとして提供しているのは Vercel のみです（一応 [Serverless Next.js](https://www.serverless.com/blog/serverless-nextjs) という AWS Lambda で動かすものもあったりしますが）。

Next.js の開発サーバー (`next dev`) で何らかのアプリをローカルで動かせているのであれば、Vercel を使って、ほぼそのままの形でネット上に公開することができます。
小規模な Web サイトであれば無料で動かせますし、とっても簡単なので、一度試してみるとよいです。


Vercel で Next.js アプリを公開する
----

ここでは、GitHub にプッシュした Next.js アプリを Vercel を使って公開してみます。
まずは、下記を参考にして、GitHub に何らかの Next.js コードをプッシュしてください（といっても `create-next-app` で生成したコードをそのままプッシュするだけで OK です）。

- [Next.js アプリのソースコードを GitHub で管理する](/p/xjw8it5)

### Vercel のアカウントを作成する

下記のページから、Vercel のアカウントを作成します。
メールアドレスでサインアップしてもいいですが、どうせ GitHub を使うので、__`Continue with GitHub`__ を選択すればいいでしょう。
クレジットカード登録などは必要ありませんが、スマホのショートメッセージによる認証だけ必要です。

- [Sign Up – Vercel](https://vercel.com/signup)

### GitHub リポジトリと連携させる

下記のページから、Next.js アプリの GitHub リポジトリを登録します。

- [New Project – Vercel](https://vercel.com/new)

{{< image w="500" border="true" src="img-001.png" >}}

ここで、ビルドに使用するコマンド（デフォルトは `npm run build`）や、環境変数の設定などを行うことができます。
特に設定の必要がなければ、そのまま __`Deploy`__ ボタンを押すと、リポジトリのクローンとビルドが始まります。

しばらくすると、次のような画面が表示されて、Web サイトが公開されます。
Congratulations!

{{< image border="true" src="img-002.png" >}}

__`Visit`__ ボタンを押すと、公開された Web サイトを開くことができます。
URL は、`https://<AppName>-<AccountName>.vercel.app/` のような感じになりますが、独自ドメインの設定も可能です。


トピックブランチによるプレビューサイト生成
----

Vercel は GitHub リポジトリにブランチが作成されると、そのブランチのコードをビルドして、プレビュー用のサイトを生成してくれます (Preview Deployment)。
プレビュー用の URL は、`https://<AppName>-<Hash値>-<AccountName>.vercel.app/` のようなテンポラリ URL になります（ブランチ名を含むエイリアス URL も作られたりします）。

生成されたプレビューサイトの一覧は、Vercel プロジェクトの __`Preview Deployments`__ や、GitHub リポジトリの __`Deployments (Environments)`__ から確認できます。
さらに、GitHub 上でこのブランチの PullRequest を作成すると、Vercel のボットが自動的にプレビューサイトの URL をコメント記入してくれます。
なので、基本的にはすべて GitHub から辿ることができます。

Vercel は、このプレビューを利用したサイト公開までのフローを、__DPS ワークフロー__ と読んでいます（Vercel のトップページで）。

- __D__: Develop ... 開発サーバー (`next dev` / `vercel dev`) での開発
- __P__: Preview ... プレビューサイトでの確認（と PullRequest レビュー）
- __S__: Ship ... 本番環境へ公開（main ブランチへのマージ）

公開中の Web サイトの内容をいきなり変更するのが心配な場合は、まずはトピックブランチの作成＆プレビューサイトによる確認を行ってから、main ブランチにマージするのがよいでしょう。

