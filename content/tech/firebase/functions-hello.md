---
title: "Firebase の Cloud Functions で Hello World"
url: "p/fvevcs8/"
date: "2022-06-04"
tags: ["Firebase/CloudFunctions"]
---

何をするか？
----

Cloud Functions に関数をデプロイすると、Google Cloud Platform 上でサーバーレス関数として実行できるようになります（Amazon の AWS Lambda や、Microsoft の Azure Functions に相当するものです）。
この関数は、次のように、様々なイベントをトリガーにして呼び出されます。

- HTTP でのアクセス
- スケジュールされたタイミング
- Firestore データベースの更新時

ここでは、HTTP によるアクセス時に `Hello from Firebase!` というレスポンスを返すだけの関数を Cloud Functions に追加してみます。
また、Firebase にデプロイする前に、エミュレーターでのローカルテストを行います。

{{% note title="Firebase なのか GCP なのか" %}}
Cloud Functions は Firebase プロジェクトからも使用できますが、実体は GCP で提供されているサービスです。
このようなサービスは他にもあり、Firebase のサービスのうち、頭に Cloud と付いてるものは実際には GCP が提供しています（例:「Cloud Firestore」「Cloud Functions」「Cloud Storage」）。
{{% /note %}}


事前準備
----

最初に Firebase プロジェクトを作成し、Firebase CLI をインストールしておきます。
既存の Firebase プロジェクトに Cloud Functions を追加する場合は、プロジェクトの作成は必要ありません。

1. [Firebase](https://firebase.google.com/) のアカウントを作成します
2. [Firebase コンソール](https://console.firebase.google.com/) から新規プロジェクトを作成します
   - `firebase` コマンドでもプロジェクトを作成できますが、上記サイトから作成してしまった方が楽です（名前の重複チェックなどをしてくれます）
3. [Firebase CLI をインストール](/p/pamw7hr/) して、`firebase` コマンドを実行できるようにします

Firebase CLI をインストールしたら、次のように Firebase アカウントでサインインしておきます。

```console
$ firebase login
```


Firebase プロジェクト用のディレクトリを作成する (firebase init)
----

任意のディレクトリ内で __`firebase init`__ コマンドを実行すると、__`firebase.json`__ ファイル（および __`.firebaserc`__）が作成されて、Firebase プロジェクト用のディレクトリと認識されるようになります。
すでに GitHub などで管理しているプロジェクトディレクトリに対しても実行することができます。

例えば、次のようにして、__Cloud Functions for Firebase のテンプレートコード__ を作成することができます。

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


エミュレーターで Functions の動作を確認してみる
----

簡単な Hello World 関数を実装して、エミュレーターで動作確認してみます。
`functions` ディレクトリを Visual Studio Code などで開いて、`src/index.ts` ファイルを次のように修正してください。

{{< code lang="ts" title="functions/src/index.ts" >}}
import * as functions from "firebase-functions";

export const helloWorld = functions
  .region("asia-northeast1")
  .https.onRequest((request, response) => {
    functions.logger.info("Hello logs!", { structuredData: true });
    response.send("Hello from Firebase!");
  });
{{< /code >}}

リージョンの指定方法は若干わかりにくいので要注意です。
上記では `asia-northeast1` を指定していますが、省略すると `us-central1` にデプロイされます。
上記のように `helloWorld` という名前のオブジェクトを export することで、Cloud Functions に `helloWorld` 関数を登録することになります。

Firebase にデプロイする前にエミュレーターで動作確認をしておきます。
次のようにして、エミュレーターを起動してください。
関数コードのビルドも自動的に行われます。

```console
$ cd functions
$ npm run serve
...
✔  functions[asia-northeast1-helloWorld]: http function initialized
   (http://localhost:5001/myapp1-12345/asia-northeast1/helloWorld).
...
```

無事エミュレーターが起動すると、上記のような感じで、関数を呼び出すための URL が表示されるので、Web ブラウザで開くか `curl` コマンドでアクセスします。
次のように `Hello` レスポンスが返って来れば成功です。

```console
$ curl http://localhost:5001/myapp1-12345/us-central1/helloWorld
Hello from Firebase!
```


Firebase にデプロイしてみる
----

エミュレーターでの動作確認が済んだら、Firebase にデプロイしてみます。
__`firebase deploy`__ コマンドを実行すると、`firebase.json` の情報に基づいてデプロイが実行されます（`--only functions` オプションで、Functions のみをデプロイ対象とすることもできます）。

```console
$ firebase deploy

=== Deploying to 'myapp1-12345'...
:
i  functions: creating Node.js 16 function helloWorld(us-central1)...
✔  functions[helloWorld(us-central1)] Successful create operation.
Function URL (helloWorld(us-central1)):
   https://us-central1-myapp1-12345.cloudfunctions.net/helloWorld
:
✔  Deploy complete!
```

デプロイが成功すると、上記のようにアクセス用の URL が表示されるので、Web ブラウザや `curl` コマンドで動作確認することができます。
ちなみに、URL は次のような構成になっています。

```
https://<LocationID>-<ProjectID>.cloudfunctions.net/<FunctionName>
```


デプロイした関数を削除する
----

Cloud Functions にデプロイした関数が必要なくなったら、次のようにして削除します。

```console
$ firebase functions:delete helloWorld
```

