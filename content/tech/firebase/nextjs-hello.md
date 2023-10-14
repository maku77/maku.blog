---
title: "Next.js で Firebase: プロジェクトの作成と接続準備"
url: "p/73eq2cm/"
date: "2021-12-26"
tags: ["Firebase", "Next.js"]
---

何をするか？
----

ここでは、__Firebase を Next.js (React) ウェブアプリから使用するための準備__ として、「Firebase プロジェクトの作成」「Next.js アプリの作成」「`FirebaseApp` インスタンスの初期化」までを行います。
なお、ここでは Firebase JS SDK ver.9 以降を対象とします（ver.8 以前は初期化方法が若干異なります）。

Firebase サービスを使うと、Web アプリやモバイルアプリに必要なバックエンド環境を簡単に整えることができます。
例えば、Firebase は次のような機能を提供しており、小規模のアプリであれば無料の Spark プランで動かすことができます（参考: [Firebase の料金プラン](https://firebase.google.com/pricing)）。

- Firebase Authentication ... ユーザー管理と認証（ログイン UI もある）
- Firebase Hosting ... Web アプリのホスティング（独自ドメインにも対応）
- Cloud Firestore ... NoSQL データベース
- Cloud Function ... サーバレス関数
- Cloud Storage for Firebase ... ファイル管理

Web アプリから上記のような機能にアクセスするには、まずは `FirebaseApp` インスタンスの設定（初期化）が必要になります。
以下では、Next.js アプリから各種 Firebase インスタンスにアクセスするところまでの準備を行います。


Firebase プロジェクトの作成
----

1. [Firebase コンソールにサインイン](https://console.firebase.google.com) して、Firebase のプロジェクトを作成します。

プロジェクト名は、自分の Google アカウント内で一意の名前になっていれば OK です。
例えば、`MyApp` のような名前を付けて作成してください。
Google アナリティクスは有効にしなくても OK です。


Firebase にウェブアプリを追加する
----

Firebase プロジェクトを作成したら、そのプロジェクトにクライアントアプリを登録します。
クライアントアプリというのは、ウェブアプリやモバイルアプリ、Unity アプリといった、ユーザーが使うアプリのことで、各アプリごとに情報を登録しておく必要があります。
ここでは、Next.js を使ったクライアントアプリから Firebase を使うことを想定しているので、「ウェブアプリ」として登録します。

1. [Firebase コンソール](https://console.firebase.google.com/) から対象のプロジェクトを選択します。
1. __`プロジェクトの概要`__ ページで __`アプリを追加`__ ボタンを押して、__ウェブアプリ__ を選択します。
1. アプリ名を適当に入力して __`アプリを登録`__ ボタンを押します。

ウェブアプリを作成すると、次のような接続用の JavaScript コードが表示されます。
これが Next.js アプリ内で `FirebaseApp` インスタンスを生成するためのコードになります。

{{< code lang="ts" >}}
import { initializeApp } from "firebase/app";

const firebaseConfig = {
  apiKey: "AIzaSyCt1.....................VjbRJAicg",
  authDomain: "myapp-99999.firebaseapp.com",
  projectId: "myapp-99999",
  storageBucket: "myapp-99999.appspot.com",
  messagingSenderId: "123456789012",
  appId: "1:123456789012:web:ab47d539998fdf2c427784"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
{{< /code >}}

このコードはいつでも参照できるので、このタイミングで保存しておく必要はありません。


Next.js プロジェクトの作成と firebase パッケージのインストール
----

まだ Next.js プロジェクトを作成していなければ、`create-next-app` コマンドを使ってサクッとプロジェクトの雛形を生成してしまいましょう。
次のように実行すると、カレントディレクトリに `myapp` というディレクトリ（Next.js プロジェクト）が生成されます。

```
$ npx create-next-app myapp --typescript
```

次に、Next.js プロジェクトに __`firebase`__ パッケージをインストールします。

```
$ cd myapp

### npm の場合
$ npm install firebase

### yarn の場合
$ yarn add firebase
```

あとは、必要に応じて [Prettier](https://maku.blog/p/au8iu6u/)（コード整形ツール）の設定や、ESLint（静的解析）の設定などをしておきましょう。


Firebase 初期化コードの作成
----

Next.js アプリから Firebase にアクセスできるようにするため、__`FirebaseApp`__ インスタンスを初期化するモジュールを作成します。
下記コードはほぼ、Firebase プロジェクトにウェブアプリを追加したときに自動生成されたコードのままです。

{{< code lang="ts" title="utils/firebase/init.ts" >}}
import { initializeApp } from 'firebase/app'

// FirebaseApp インスタンスを初期化する
initializeApp({
  apiKey: 'AIzaSyCt1.....................VjbRJAicg',
  authDomain: 'myapp-99999.firebaseapp.com',
  projectId: 'myapp-99999',
  storageBucket: 'myapp-99999.appspot.com',
  messagingSenderId: '123456789012',
  appId: '1:123456789012:web:ab47d539998fdf2c427784'
})
{{< /code >}}

{{% note title="Firebase の apiKey は Git にコミットして OK" %}}
上記のコード内には `apiKey` プロパティの値がハードコードされていますが、これはいわゆる __秘密鍵ではない__ ので、GitHub などにそのままコミットして公開しても大丈夫です。
Firebase のクライアントアプリにおけるアクセス権限は、[Firestore などのセキュリティルール](https://firebase.google.com/docs/firestore/security/get-started) の仕組みを使って、ユーザーの認証状態などに基づいて制御します。

一方で、管理用のアプリを作るための [Firebase Admin SDK](https://firebase.google.com/docs/admin/setup) を使用する場合は、Firebase コンソール上で生成した秘密鍵を使って接続することになります。
こちらはもちろん Git にはコミットしてはいけないものなので混同しないようにしてください。
{{% /note %}}

この初期化コードは、次のような感じでインポートすることで実行します。
初期化された `FirebaseApp` インスタンスは Firebase ライブラリ内で保持されているため、いつでも __`getApp()`__ 関数で参照できるようになります。

{{< code lang="ts" title="pages/index.tsx" >}}
import type { NextPage } from 'next'
import { getApp, FirebaseApp } from 'firebase/app'

import '../utils/firebase/init' // Initialize FirebaseApp

const Home: NextPage = () => {
  const app: FirebaseApp = getApp()
  return (
    <ul>
      <li>name = {app.name}</li>
      <li>appId = {app.options.appId}</li>
      <li>apiKey = {app.options.apiKey}</li>
    </ul>
  )
}

export default Home
{{< /code >}}

あとは、`yarn dev` `(npm run dev)` で Next.js 開発サーバーを起動して、`http://localhost:3000/` にアクセスすれば、次のように `FirebaseApp` インスタンスの情報が表示されるはずです。

```
* name = [DEFAULT]
* appId = 1:123456789012:web:ab47d539998fdf2c427784
* apiKey = AIzaSyCt1.....................VjbRJAicg
```


各種 Firebase サービスにアクセスする方法
----

Firebase の各種サービス用のインスタンスを取得するためには、専用のモジュールをインポートして、`getXxx()` 系のメソッドを呼び出します。
例えば、次のコードでは、Firebase Authentication を使うための `Auth` インスタンスと、Cloud Firestore を使うための `Firestore` インスタンスを取得しています。

```ts
import { getAuth, Auth } from 'firebase/auth'
import { getFirestore, Firestore } from 'firebase/firestore'

import '../utils/firebase/init' // Initialize FirebaseApp

// Firebase Authentication を使うコード
const auth: Auth = getAuth()

// Cloud Firestore を使うコード
const firestore: Firestore = getFirestore()
```

これらの `getXxx()` 系関数を呼び出す前に、`utils/firebase/init.ts` をインポートして、`FirebaseApp` インスタンスを初期化しておくことを忘れないでください。
`initializeApp()` を実行する前に `getXxx()` 系関数を呼び出すと、次のようなエラーが発生します。

```
FirebaseError: Firebase: No Firebase App '[DEFAULT]' has been created
  - call Firebase App.initializeApp() (app/no-app).
```

Next.js アプリで全面的に Firebase を使うのであれば、`pages/_app.tsx` あたりで初期化してしまうのがよいかもしれません。

{{< code lang="tsx" hl_lines="3" title="pages/_app.tsx" >}}
import type { AppProps } from 'next/app'
import '../styles/globals.css'
import '../utils/firebase/init' // Initialize FirebaseApp

function MyApp({ Component, pageProps }: AppProps) {
  return <Component {...pageProps} />
}

export default MyApp
{{< /code >}}

{{% note title="init.ts の中に Auth や Firestore 関連のコードを記述しない" %}}
アプリのコードを簡潔にするために、`init.ts` の中で `getFirestore()` などを呼び出して
 `Firestore` インスタンスを `export` したくなるかもしれませんが、そのようにすると、`init.ts` をインポートする全てのページのバンドルサイズが大きくなってしまいます。
`init.ts` 内の処理は `FirebaseApp` インスタンスの初期化にとどめておき、必要に応じて `getAuth()` や `getFirestore()` を呼び出すことをお勧めします。
{{% /note %}}

{{% maku-common/reference %}}
- [Next.js で Firebase: Authentication 機能でユーザー認証できるようにする](/p/8t6gq2b)
{{% /maku-common/reference %}}

