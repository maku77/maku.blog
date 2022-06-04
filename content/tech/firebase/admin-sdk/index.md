---
title: "Firebase Admin SDK で Firebase の各種サービスを操作する"
url: "/p/yfov4bi"
date: "2021-12-31"
tags: ["Firebase"]
---

Firebase Admin SDK とは
----

[Firebase Admin SDK](https://firebase.google.com/docs/reference/admin) を使うと、Firebase の管理者（おそらくあなた）用に提供された API キーを使って、Firebase 上のデータを管理することができます。
例えば、ローカル PC にある JSON データを解析して Firestore データベースのドキュメントとして登録するといったことができます。

Firebase Admin SDK がサポートしている言語としては、Node.js、Java、Python、Go、C# などがありますが、Node.js から順番に機能提供されていくようです。
ここでは Node.js (TypeScript) で Firestore の API を呼び出してみます。

{{% note title="サーバーライブラリとクライアントライブラリ" %}}
Firebase Admin SDK はしばしば __サーバーライブラリ__ とも呼ばれます。
一方で、Web アプリやモバイルアプリなどで使う SDK は __クライアントライブラリ__ と呼ばれ、Firebase が提供するセキュリティルールという仕組みでアクセス制御を行います。
具体的には、Firebase Auth などでユーザー認証を行うことで、各クライアントに対してアクセス権限を付与します。
これに対して、Firebase Admin SDK はあらかじめローカルに保持しているアクセスキーを使って API を呼び出すので、セキュリティルールを無視した特権アクセスが可能です。
{{% /note %}}


秘密鍵のダウンロード
----

Firebase Admin SDK を使って API 呼び出しを行うには、秘密鍵（API キー）を取得する必要があります。

{{< image w="600" border="true" src="img-001.png" >}}

1. [Firebase コンソール](https://console.firebase.google.com/) にサインインして、対象のプロジェクトを選択する。
2. __プロジェクトの設定__ を開き、__サービスアカウント__ タブを選択する。
3. __新しい秘密鍵の生成__ をクリックして、秘密鍵ファイル (.json) をダウンロード。

ダウンロードされる .json ファイルは、次のような感じの内容になっています。
このファイルは、__Credential ファイル__ や __Configuration ファイル__ と呼ばれます。

{{< code lang="json" title="myapp-12345-firebase-adminsdk-xxxxx.json" >}}
{
  "type": "service_account",
  "project_id": "myapp-12345",
  "private_key_id": "14b01f11bc7f484d42fab494dd0bdb7818c93f3b",
  "private_key": "-----BEGIN PRIVATE KEY-----\n ... \n-----END PRIVATE KEY-----\n",
  "client_email": "firebase-adminsdk-xxxxx@myapp-12345.iam.gserviceaccount.com",
  "client_id": "123456789012345678901",
  "auth_uri": "https://accounts.google.com/o/oauth2/auth",
  "token_uri": "https://oauth2.googleapis.com/token",
  "auth_provider_x509_cert_url": "https://www.googleapis.com/oauth2/v1/certs",
  "client_x509_cert_url": "https://www.googleapis.com/robot/v1/metadata/x509/firebase-adminsdk-xxxxx%40myapp-12345.iam.gserviceaccount.com"
}
{{< /code >}}

このファイルには、管理者権限で Firebase を操作するための秘密鍵が含まれているので、外部に公開されないように保管してください。
間違って Git コミットしないように、ホームディレクトリなどに配置しておくのがいいです。
ここでは、次のように Dropbox に保存してみました。

- `~/Dropbox/firebase/myapp-12345-firebase-adminsdk-xxxx-abcde12345.json`


Node.js (TypeScript) プロジェクトの作成
----

Firebase Admin SDK を使うための Node.js プロジェクトを作成していきます。
まずは、プロジェクト用のディレクトリを作成して、必要な NPM パッケージをインストールします。
ここでは、TypeScript コードをビルドなしで手軽に実行するために、[ts-node](https://www.npmjs.com/package/ts-node) を使います。
Firebase Admin SDK は、[firebase-admin](https://www.npmjs.com/package/firebase-admin) という NPM パッケージとして提供されています。

```
$ mkdir myapp && cd myapp

### npm の場合
$ npm init -y
$ npm install -D typescript ts-node
$ npm install firebase-admin

### yarn の場合
$ yarn init -y
$ yarn add -D typescript ts-node
$ yarn add firebase-admin
```

ついでに NPM スクリプトを追加して、`yarn start` (`npm start`) コマンドで `main.ts` を起動できるようにしておきます。

{{< code lang="js" title="package.json" >}}
{
  // ...
  "scripts": {
    "start": "ts-node main"
  },
{{< /code >}}


Firebase Admin SDK の初期化
----

Firebase Admin SDK の各種サービスにアクセスするには、最初に __`initializeApp`__ 関数を呼び出して、Firebase の `App` インスタンスを初期化しておく必要があります。
`initializeApp` 関数は、デフォルトで __`GOOGLE_APPLICATION_CREDENTIALS`__ 環境変数に設定したパスから、Credential ファイルをロードしようとするので、先にこの環境変数を設定しておきます。

```bash
export GOOGLE_APPLICATION_CREDENTIALS=~/Dropbox/firebase/myapp-12345-firebase-adminsdk-xxxx-abcde12345.json
```

次の TypeScript コードは、この Credential ファイルを使って Firebase の `App` インスタンスを初期化し、その設定内容を表示します。

{{< code lang="ts" title="main.ts" >}}
import { initializeApp, App } from 'firebase-admin/app'

const app: App = initializeApp()
console.log(app.options)
{{< /code >}}

次のように実行して、秘密鍵の情報などが表示されればうまく設定できています。

{{< code title="実行例" >}}
$ yarn -s start
{
  credential: ServiceAccountCredential {
    httpAgent: undefined,
    implicit: true,
    projectId: 'myapp-12345',
    privateKey: '-----BEGIN PRIVATE KEY-----\n' +
      'MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQC4PB8Dwzy7J0tg\n' +
      '................................................................\n' +
      'I/cr9iy94UUMtXq5Y18aDe2aKg7//GDdtCGrQ4OicOojvv/BqujDZ3ZL0b3dD/yP\n' +
      'AzWyaB/GnfIEPKIyWM7LOPl2\n' +
      '-----END PRIVATE KEY-----\n',
    clientEmail: 'firebase-adminsdk-xxxxx@myapp-12345.iam.gserviceaccount.com',
    httpClient: HttpClient { retry: [Object] }
  }
}
{{< /code >}}

複数の Firebase プロジェクトを運用している場合は、`GOOGLE_APPLICATION_CREDENTIALS` という 1 つの環境変数を使いまわすのは都合が悪いかもしれません。
そのような場合は、次のようにすれば、任意の環境変数に Credential ファイルのパスを設定しておくことができます。
この例ではまず __`FIREBASE_MYAPP_CREDENTIALS`__ 環境変数を参照し、見つからなかった場合はフォールバックとして `GOOGLE_APPLICATION_CREDENTIALS` 環境変数を参照するようにしています。

{{< code lang="ts" title="initFirebase.ts" >}}
import { cert, initializeApp } from 'firebase-admin/app'

const credPath =
  process.env['FIREBASE_MYAPP_CREDENTIALS'] ||
  process.env['GOOGLE_APPLICATION_CREDENTIALS']

if (!credPath) {
  console.error('Error: FIREBASE_MYAPP_CREDENTIALS env is not set')
  process.exit(1)
}

initializeApp({ credential: cert(credPath) })
{{< /code >}}

これで、Firebase の各種 API を呼び出す準備ができました。


Admin SDK で Firestore データベースにアクセスしてみる
----

上記で作成した `initFirebase.ts` モジュールを読み込めば、Firebase の各種サービスにアクセスできるようになります。
例えば、[firebase-admin/firestore モジュール](https://firebase.google.com/docs/reference/admin/node/firebase-admin.firestore) が提供する __`getFirestore`__ 関数を使って、`Firestore` インスタンスを取得すれば、Firestore データベースを操作することができます。

次の例では、`addSampleBooks` 関数で `books` コレクションにサンプルデータを登録し、`dumpSampleBooks` 関数でその情報を取得しています。

{{< code lang="ts" title="main.ts" >}}
import { getFirestore } from 'firebase-admin/firestore'
import './initFirebase' // Initialize Firebase App

const db = getFirestore()
const booksCollection = db.collection('/books')

/** books コレクションに 1 つのドキュメントを追加 */
async function addBook(id: string, title: string) {
  await booksCollection.doc(id).set({ title })
}

/** books コレクションにサンプルデータを追加 */
async function addSampleBooks() {
  await Promise.all([
    addBook('id-001', 'Title-001'),
    addBook('id-002', 'Title-002'),
    addBook('id-003', 'Title-003'),
  ])
}

/** books コレクションの全てのドキュメントを取得して表示する */
async function dumpSampleBooks() {
  const snapshot = await booksCollection.get()
  snapshot.docs.forEach((doc) => {
    const book = doc.data()
    console.dir(book)
  })
}

/** エントリーポイント */
void (async function main() {
  await addSampleBooks()
  await dumpSampleBooks()
})()
{{< /code >}}

{{< code title="実行結果" >}}
$ yarn -s start
{ title: 'Title-001' }
{ title: 'Title-002' }
{ title: 'Title-003' }
{{< /code >}}

