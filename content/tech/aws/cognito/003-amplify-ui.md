---
title: "Amazon Cognito: Amplify SDK による認証まわりの UI 表示あれこれ"
url: "/p/24p8jjx"
date: "2021-06-09"
lastmod: "2021-11-01"
tags: ["AWS", "AWS/Amplify", "AWS/Cognito"]
---

認証済みユーザーが admin グループに属しているか調べて UI を切り替える
----

Cognito ユーザープールの「グループ」機能を使うと、既存のユーザーをグループ単位でまとめて、特別な IAM ロールを割り当てたりすることができます。
例えば、`admin` グループを作っておいて、そのグループに所属するユーザーに、DynamoDB の書き込み権限 (IAM role) を与える、といったことができます。

React アプリのレイヤでは、このような IAM ロールの権限うんぬんの前に、カレントユーザーがどのようなグループに所属しているかに応じて UI の表示分けをしたいことがあります。
例えば、`admin` グループのユーザーが Web ページを表示しているときは、管理者向けのメニュー表示をする、といったケースです。

幸い、`aws-amplify` モジュールの `Auth` オブジェクトが提供する認証情報を参照することで、認証済みのユーザーがどの Cognito グループに属しているかを調べることができます。
具体的には、`Auth.currentAuthenticatedUser()` などが返すユーザー情報のアクセストークンのペイロードに __`cognito:groups`__ というプロパティがあり、そこに Cognito グループ名のリストが格納されています。

- 参考: [Using the Access Token - Amazon Cognito](https://docs.aws.amazon.com/cognito/latest/developerguide/amazon-cognito-user-pools-using-the-access-token.html)

次のようなカスタムフック `useAuthState` を定義しておくと、戻り値の `isAdmin` プロパティを見るだけで、現在のユーザーが admin グループに属しているかを確認できます。

{{< code lang="ts" title="src/hooks/useAuthState.tsx" >}}
import { useEffect, useState } from 'react'
import { Auth } from 'aws-amplify'
import {
  AuthState, CognitoUserInterface, onAuthUIStateChange,
} from '@aws-amplify/ui-components'

// ユーザーがこのグループ名の Cognito グループに所属しているときに、
// useAuthState フックが返す isAdmin プロパティを true にします。
const ADMIN_GROUP_NAME = 'admin'

/** useAuthState フックの戻り値の型 */
export type UseAuthStateOutput = {
  isSignedIn: boolean
  isAdmin: boolean
  user: CognitoUserInterface | undefined
}

/** 現在の認証状態を取得するためのフック関数です。 */
export const useAuthState = (): UseAuthStateOutput => {
  // Cognito によるサインイン状態やユーザー情報を保持するステート
  const [authState, setAuthState] = useState<AuthState | undefined>()
  const [user, setUser] = useState<CognitoUserInterface | undefined>()

  useEffect(() => {
    // 画面遷移時に onAuthUIStateChange が呼ばれないことへの対応
    // https://github.com/aws-amplify/docs/issues/2895
    if (authState === undefined) {
      Auth.currentAuthenticatedUser()
        .then((authData) => {
          setAuthState(AuthState.SignedIn)
          setUser(authData)
        })
        .catch(() => { /* Nothing to do */ })
    }

    return onAuthUIStateChange((nextAuthState, authData) => {
      setAuthState(nextAuthState)
      setUser(authData as CognitoUserInterface)
    })
  }, [authState])

  return {
    isSignedIn: authState === AuthState.SignedIn,
    isAdmin: isAdmin(user),
    user,
  }
}

// アクセストークン内のグループ名リストに "admin" があるかを調べます。
function isAdmin(user: CognitoUserInterface | undefined): boolean {
  // eslint-disable-next-line @typescript-eslint/no-unsafe-member-access
  const groups = user?.signInUserSession?.accessToken?.payload[
    'cognito:groups'
  ] as Array<string>
  return groups?.includes(ADMIN_GROUP_NAME) ?? false
}
{{< /code >}}

上記のフック実装では、戻り値の `user` プロパティで `CognitoUserInterface` 情報を返すようにしていますが、Cognito に依存する情報はできるだけフック内に隠蔽してしまった方がいいですね。
下記は、このフック関数の使用例です。
`useAuthState()` が返す `isAdmin` の値が true のときに、管理者用のメニューを表示するという処理を想定しています。

{{< code lang="tsx" title="src/components/Sample.tsx（使用例）" >}}
// ...

export const Sample: FC = () => {
  const { isAdmin } = useAuthState()
  return (
    <>
      <p>If you are in admin group, you can see the following menu</p>
      {isAdmin && <p>Secret menu</p>}
    </>
  )
}
{{< /code >}}


Sign in、Sign out 処理を Next.js ページとして実装する
----

次のように、`pages/signin.tsx`、`pages/signout.tsx` といった Next.js ページを作っておくと、そのアドレスへルーティング (`Router.push('/signout')`) するだけで、サインイン／アウト処理を起動することができます。

{{< code lang="tsx" title="src/pages/signin.tsx" >}}
import { FC } from 'react'
import { AmplifyAuthenticator, AmplifySignIn } from '@aws-amplify/ui-react'
import Router from 'next/router'

// サインイン済みかどうかを調べるフックを用意しておきます
import { useAuthState } from '@/hooks/useAuthState'

const SignInPage: FC = () => {
  // サインイン状態ならトップページなどへ飛ばす
  const { isSignedIn } = useAuthState()
  if (isSignedIn) void Router.replace('/')

  // サインインの画面を表示する
  return (
    <AmplifyAuthenticator>
      <AmplifySignIn slot="sign-in" hideSignUp />
    </AmplifyAuthenticator>
  )
}

export default SignInPage
{{< /code >}}

{{< code lang="tsx" title="src/pages/signout.tsx" >}}
import { FC, useEffect } from 'react'
import Router from 'next/router'
import { Auth } from 'aws-amplify'

const SignOutPage: FC = () => {
  useEffect(() => {
    void Auth.signOut()
    void Router.replace('/')
  }, [])
  return null
}

export default SignOutPage
{{< /code >}}

上記のようなページを Next.js アプリ内に設置すれば、ブラウザの URL 欄に直接 `/signin` と入力してサインイン画面を表示したり、`/signout` と入力してサインアウトすることができます。
また、プログラムから `Router.push('/signin')` とすることでも同じ振る舞いを再現できます。
次のコンポーネントは、現在のサインイン状態に応じて「Sign out」あるいは「Sign in」のボタンを表示しています。

{{< code lang="tsx" title="src/components/SignInOrOutButton.tsx" >}}
import { FC } from 'react'
import Router from 'next/router'
import Button from '@mui/material/Button'

import { useAuthState } from '@/hooks/useAuthState'

/**
 * Shows a sign-in or sign-out button, depending on the current
 * authentication status.
 */
export const SignInOrOutButton: FC = () => {
  const { isSignedIn } = useAuthState()

  return isSignedIn ? (
    <Button onClick={() => Router.push('/signout')}>Sign out</Button>
  ) : (
    <Button onClick={() => Router.push('/signin')}>Sign in</Button>
  )
}
{{< /code >}}


Sign out ボタンの表示方法
----

{{< code lang="tsx" title="React コンポーネント (AmplifySignOut) を配置する方法" >}}
import { AmplifySignOut } from '@aws-amplify/ui-react'

export const MyComponent: React.FC = () => {
  // ...
  return (
    <AmplifySignOut />
  )
}
{{< /code >}}

{{< code lang="tsx" title="サインアウト関数 (Auth.signOut()) を呼び出す方法" >}}
import { Auth } from 'aws-amplify'
import Button from '@mui/material/Button'

export const MyComponent: React.FC = () => {
  // ...
  return (
    <Button onClick={() => Auth.signOut()}>Sign out</Button>
  )
}
{{< /code >}}

