---
title: "Next.js で Firebase: Authentication 機能でユーザー認証できるようにする"
url: "/p/8t6gq2b"
date: "2021-12-26"
tags: ["Firebase/Authentication", "Next.js"]
weight: 201
---

何をするか？
----

[Firebase の Authentication 機能](https://firebase.google.com/docs/auth) を使用すると、ウェブアプリ（やモバイルアプリ）にユーザー認証機能を付けて、各種リソースへのアクセスを制御できるようになります。
例えば、「Firestore に格納されているユーザー情報の編集は、そのユーザーにのみ許可する」といったことができます。
Firebase のクライアントアプリでユーザーデータを扱う場合は、Firebase Authentication はほぼ必須の機能だといえます。

ここでは、Next.js (React) アプリで Firebase Authentication を使い、ユーザー認証を行えるようにしてみます。
ユーザー認証に使う UI は、Firebase が用意している [FirebaseUI](https://firebase.google.com/docs/auth/web/firebaseui) を使って表示します。

事前準備として、Firebase プロジェクトの作成は済んでおり、Next.js アプリから各種 Firestore 関連インスタンスを取得できるようになっていると想定します。

- 参考: [Next.js で Firebase: プロジェクトの作成と接続準備](/p/73eq2cm)


ログインプロバイダの設定
----

Firebase Authentication では、サインイン方法として、新規に登録するメールアドレスや電話番号を使う方法（ネイティブプロバイダ）と、既存の Google アカウントや Facebook アカウントなどを使う方法（追加のプロバイダ）が準備されています。
まずは、シンプルに「メールアドレス」でユーザー登録できるようにしてみます。

1. [Firebase コンソール](https://console.firebase.google.com/) の __Authentication__ タブを選択して機能を有効化する。
2. ログインプロバイダ (Sign-in method) で __メール／パスワード__ を選択する。

    {{< image border="true" w="700" src="img-001.png" >}}

3. __有効にする__ にチェックを入れて __保存__ をクリック。

    {{< image border="true" w="500" src="img-002.png" >}}

これで、Firebase Authentication で「メールアドレス」による認証を行えるようになります。


サインイン状態を扱う React フックを作成する
----

Next.js (React) アプリから Firebase Authentication を使う場合は、サインイン状態を扱うためのカスタムフックを作成しておくと便利です。
サインインボタンなどの UI を先に作りたいところですが、ボタンを表示すべきかどうかの判断のために結局このカスタムフックが必要になるので、先にカスタムフックを作成します。

認証状態の変化は __`onAuthStateChanged`__ コールバックでハンドルすることができるので、これを利用して `useAuthState` カスタムフックを実装します。

{{< code lang="ts" title="hooks/useAuthState.ts" >}}
import { useEffect, useState } from 'react'
import { getAuth, onAuthStateChanged } from 'firebase/auth'

import '../utils/firebase/init' // Initialize FirebaseApp

/**
 * useAuthState フックの戻り値の型。
 */
export type AuthState = {
  isSignedIn: boolean
  isLoading: boolean
  userId: string | undefined
  userName: string | undefined
  avatarUrl: string | undefined
}

/**
 * useAuthState が返す初期値。
 * Next.js のサーバーサイドレンダリング時もこの値になる。
 */
const INITIAL_AUTH_STATE: AuthState = {
  isSignedIn: false,
  isLoading: true,
  userId: undefined,
  userName: undefined,
  avatarUrl: undefined,
}

/**
 * ユーザーのサインイン状態を取得するためのカスタムフック。
 */
export function useAuthState(): AuthState {
  const [authState, setAuthState] = useState(INITIAL_AUTH_STATE)

  // サインイン状態の変化を監視する
  useEffect(() => {
    const unsubscribe = onAuthStateChanged(getAuth(), (user) => {
      if (user) {
        setAuthState({
          isSignedIn: true,
          isLoading: false,
          userId: user.uid,
          userName: user.displayName || undefined,
          avatarUrl: user.photoURL || undefined,
        })
      } else {
        setAuthState({ ...INITIAL_AUTH_STATE, isLoading: false })
      }
    })

    // ページ遷移時にサインイン状態の監視を解除
    return () => unsubscribe()
  }, [])

  return authState
}
{{< /code >}}


サインイン画面用のコンポーネントを作成する
----

Firebase はサインイン画面やユーザー登録画面を提供するライブラリとして、[firebaseui](https://github.com/firebase/firebaseui-web/) パッケージを提供しています。
Next.js (React) を使っている場合は、このパッケージを React コンポーネント化した [reactfirebaseui](https://github.com/firebase/firebaseui-web-react/) パッケージを使うと便利です。

```
### npm の場合
$ npm install react-firebaseui

### yarn の場合
$ yarn add react-firebaseui
```

`react-firebaseui` パッケージは、`FirebaseAuth` コンポーネントと `StyledFirebaseAuth` コンポーネントを提供していますが、後者を使った方が表示スタイルを直接設定できて便利です。
下記は、メールアドレスによるサインイン画面を表示するコンポーネントの実装例です。

{{< code lang="tsx" title="components/SignInForm.tsx" >}}
import { FC } from 'react'
import {
  getAuth,
  EmailAuthProvider,
  // FacebookAuthProvider,
  // GoogleAuthProvider,
  // TwitterAuthProvider,
} from 'firebase/auth'
import { auth } from 'firebaseui'
import StyledFirebaseAuth from 'react-firebaseui/StyledFirebaseAuth'

import '../utils/firebase/init' // Initialize FirebaseApp

const uiConfig: auth.Config = {
  signInFlow: 'popup',
  signInOptions: [
    EmailAuthProvider.PROVIDER_ID,
    // FacebookAuthProvider.PROVIDER_ID,
    // GoogleAuthProvider.PROVIDER_ID,
    // TwitterAuthProvider.PROVIDER_ID,
  ],
  signInSuccessUrl: '/',
}

export const SignInForm: FC = () => {
  return (
    <StyledFirebaseAuth firebaseAuth={getAuth()} uiConfig={uiConfig} />
  )
}
{{< /code >}}

どのような認証方法を有効にするかは、__`signInOptions`__ プロパティで指定します。
この例では、認証プロバイダーとしてメールアドレス用の `EmailAuthProvider` しか指定していないため、このコンポーネントを表示すると、次のようなメールアドレスの入力フォームだけが表示されます。

{{< image border="true" w="373" src="img-003.png" >}}

2 つ以上の認証プロバイダーを有効にした場合は、次のように認証方法を選択する画面が最初に表示されます（あらかじめ Firebase コンソール上で、各プロバイダーの設定をしておく必要があります）。

{{< image border="true" w="281" src="img-004.png" >}}

Sign in with email の画面で、ユーザー登録されていないメールアドレスを入力すると、自動的に新規ユーザーの登録フォームが表示されます。
つまり、これだけで「サインイン画面」と「ユーザー登録画面」の完成です。
表示内容のカスタマイズ方法（上記の `uiConfig` 部分）は、[FirebaseUI のページの Configuration](https://github.com/firebase/firebaseui-web/blob/master/README.md#configuration) を参照してください。

実際にここで作成した `SignInForm` を表示するには、次のような感じで Next.js のページコンポーネントから使用します。

{{< code lang="tsx" title="pages/signin.tsx" >}}
import type { NextPage } from 'next'
import { SignInForm } from '../components/SignInForm'

const SignInPage: NextPage = () => {
  return (
    <div style={{ margin: '1rem' }}>
      <h2>サインイン</h2>
      <SignInForm />
    </div>
  )
}

export default SignInPage
{{< /code >}}

こうしておけば、`/signin` という URL でアクセスしたときにサインイン画面を表示できます。


Sign-in ボタンと Sign-out ボタンを表示する
----

最後に、現在のサインイン状態によって「サインイン」あるいは「サインアウト」のボタンを表示するコンポーネントを作成します。
サインインボタンが押されたときは、`Router.push('/signin')` でサインイン画面に遷移し、サインアウトボタンが押されたときは、Firebase が提供する `signOut` 関数を呼び出すようにしています。

{{< code lang="tsx" title="components/SignInOrOutButton.tsx" >}}
import { FC } from 'react'
import Router from 'next/router'
import { getAuth, signOut } from 'firebase/auth'

import '../utils/firebase/init' // Initialize FirebaseApp
import { useAuthState } from '../hooks/useAuthState'

export const SignInOrOutButton: FC = () => {
  const { isSignedIn } = useAuthState()

  if (isSignedIn) {
    return <button onClick={() => signOut(getAuth())}>Sign-out</button>
  } else {
    return <button onClick={() => Router.push('/signin')}>Sign-in</button>
  }
}
{{< /code >}}

あとはこのコンポーネントを各ページのヘッダ部分などに配置してやれば、どのページからでもサインイン／アウトのできる Web サイトの完成です。

ここではロジックを簡単に示すために、ネイティブな `button` コンポーネントを使用しましたが、かっこいい UI を表示したければ、[MUI (Material-UI) などを導入する](/p/s6djqw3) のが簡単です。

