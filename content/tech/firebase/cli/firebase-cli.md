---
title: "Firebase CLI でコマンドラインから Firebase を操作する"
url: "p/pamw7hr/"
date: "2022-01-08"
tags: ["Firebase"]
---

Firebase CLI のインストール
----

[Firebase CLI](https://firebase.google.com/docs/cli) をインストールすると、__`firebase`__ というコマンドで Firebase の各種サービスを操作できるようになります。
インストール用に各 OS 用のバイナリが用意されていますが、どの OS でも共通で使える [Node.js](https://nodejs.org/) の `npm` でインストールすることをお勧めします。

```console
$ npm install -g firebase-tools
```

Firebase CLI をバージョンアップしたいときも同じコマンドでいけます。
インストールが終わったら、`firebase` コマンドを実行できるか確認しておきます。

```console
$ firebase --version
10.0.1
```


firebase コマンドの使い方
----

`firebase` コマンドは、__`firebase <サブコマンド>`__ という形で使用します。
`firebase` コマンドを引数なしで実行するとヘルプを表示できますが、[公式サイトのコマンドリファレンス](https://firebase.google.com/docs/cli#command_reference) の方がわかりやすいです。
以下に、いくつかのサブコマンドを紹介しておきます。

### サインイン (firebase login)

Firebase プロジェクトの情報を取得するには、まずは Firebase アカウントにサインインしておく必要があります。
次のように実行すると、Web ブラウザが開き、Firebase にサインインできます。

```console
$ firebase login
```

サインインとアクセス権限が終わったら、Web ブラウザは閉じて大丈夫です。

### Firebase プロジェクトの一覧を取得する (projects:list)

```console
$ firebase projects:list
✔ Preparing the list of your Firebase projects
┌──────────────────────┬──────────────┬────────────────┬──────────────────────┐
│ Project Display Name │ Project ID   │ Project Number │ Resource Location ID │
├──────────────────────┼──────────────┼────────────────┼──────────────────────┤
│ MyApp1               │ myapp1-12345 │ 123456789001   │ asia-northeast1      │
├──────────────────────┼──────────────┼────────────────┼──────────────────────┤
│ MyApp2               │ myapp2-12345 │ 123456789002   │ asia-northeast1      │
└──────────────────────┴──────────────┴────────────────┴──────────────────────┘

2 project(s) total.
```

`firebase` コマンドで操作対象のプロジェクトを指定するときは、__`--project`__ オプションで上記の Project ID の欄に表示されている ID を指定します。
ただし、`firebase init` で初期化したディレクトリ内でコマンド実行する場合は、通常は `--project` オプションは省略できます（デフォルトで操作対象とする Firebase プロジェクトが `.firebaserc` に保存されるため）。

- 参考: [Firebase の Cloud Functions で Hello World](/p/fvevcs8/)

### Firestore のインデックス情報を取得する (firestore:indexes)

```console
$ firebase --project myapp1-12345 firestore:indexes
```

### Firebase Auth のユーザーリストを取得する (auth:export)

```console
$ firebase --project myapp1-12345 auth:export users.json
Exporting accounts to users.json
✔  Exported 3 account(s) successfully.
```

出力ファイル名の拡張子によって、JSON 形式あるいは CSV 形式で出力されます。
`--format json` のように、明示的にフォーマットを指定することもできます。

### Functions の一覧を取得する (functions:list)

```console
$ firebase --project myapp1-12345 functions:list
┌────────────┬─────────┬─────────────┬────────┬──────────┐
│ Function   │ Trigger │ Location    │ Memory │ Runtime  │
├────────────┼─────────┼─────────────┼────────┼──────────┤
│ helloWorld │ https   │ us-central1 │ 256    │ nodejs16 │
└────────────┴─────────┴─────────────┴────────┴──────────┘
```

