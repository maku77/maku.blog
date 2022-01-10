---
title: "Firebase CLI でコマンドラインから Firebase を操作する"
url: "/p/pamw7hr"
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

### Firestore のインデックス情報を取得する (firestore:indexes)

```console
$ firebase --project myapp1-12345 firestore:indexes
```

ほとんどのサブコマンドでは、このようにプロジェクト ID を指定する必要がありますが、`firebase init` で初期化したディレクトリ内であれば省略できます（デフォルトで操作対象とする Firebase プロジェクトが `.firebaserc` に保存されるため）。

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


Firebase プロジェクト用のディレクトリを作成する (firebase init)
----

任意のディレクトリ内で __`firebase init`__ コマンドを実行すると、__`firebase.json`__ ファイル（および `.firebaserc`）が作成されて、Firebase プロジェクト用のディレクトリと認識されるようになります。
すでに GitHub などで管理しているプロジェクトディレクトリに対しても実行することができます。

### Functions の雛形を生成してみる

例えば、次のようにして、Cloud Functions for Firebase のテンプレートコードを作成することができます。

{{< code lang="console" title="Functions のテンプレートを生成する" >}}
$ mkdir myapp （このディレクトリを Git 管理する想定）
$ cd myapp
$ firebase init functions
{{< /code >}}

`firebase init` を実行すると、対話形式で「どのプロジェクトを対象とするか？」「JavaScript と TypeScript のどちらを使用するか？」などを聞かれるので、順番に答えていくと、次のようなファイルおよびディレクトリが生成されます（選択によって微妙に変わってきます）。

```
myapp/
 +-- .firebaserc （デプロイ対象とするプロジェクトの情報）
 +-- .gitignore （Git 管理外にするファイルの情報）
 +-- firebase.json （Firebase へのデプロイ情報など）
 +-- functions/
      +-- .eslintrc.js （ESLint の設定）
      +-- package.json （Functions 用のプロジェクト設定）
      +-- src/ （Functions のテンプレートコード）
      +-- tsconfig.json （TypeScript を選択した場合はその設定ファイル）
```

### エミュレーターで Functions の動作を確認してみる

Hello World 的な Functions コードを実装して、エミュレーターで動作確認してみます。
`functions` ディレクトリを Visual Studio Code などで開いて、`src/index.ts` ファイルを次のような感じに修正してください。
このコードは、Functions サービスに `helloWorld` という名前の関数を生成することを示しています。

{{< code lang="ts" title="functions/src/index.ts" >}}
import * as functions from "firebase-functions";

export const helloWorld = functions.https.onRequest((request, response) => {
  functions.logger.info("Hello logs!", {structuredData: true});
  response.send("Hello from Firebase!");
});
{{< /code >}}

次のようにして、エミュレーターを起動します。
Functions コードのビルドも自動的に行われます。

```
$ cd functions
$ npm run serve
...
✔  functions[us-central1-helloWorld]: http function initialized
   (http://localhost:5001/myapp1-12345/us-central1/helloWorld).
...
```

無事エミュレーターが起動すると、上記のような感じで、Functions にアクセスするためのアドレスが表示されるので、Web ブラウザで開いて、次のように表示されれば成功です。

```
Hello from Firebase!
```

### Firebase にデプロイしてみる

エミュレーターでの動作確認が済んだら、Firebase にデプロイしてみます。
__`firebase deploy`__ コマンドを実行すると、`firebase.json` の情報に基づいてデプロイが実行されます（`--only functions` オプションで、Functions のみをデプロイ対象とすることもできます）。

```command
$ firebase deploy

=== Deploying to 'myapp1-12345'...
:
i  functions: creating Node.js 16 function helloWorld(us-central1)...
✔  functions[helloWorld(us-central1)] Successful create operation.
Function URL (helloWorld(us-central1)): https://us-central1-myapp1-12345.cloudfunctions.net/helloWorld
:
✔  Deploy complete!
```

デプロイが成功すると、上記のようにアクセス用のアドレスが表示されるので、Web ブラウザで開いて動作を確認することができます。

### デプロイした Functions を削除する

次のようにして、デプロイされた Functions を削除できます。

```command
$ firebase functions:delete helloWorld
```

