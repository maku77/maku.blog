---
title: "Amazon Cognito (1) サインイン可能な Web サイトを作る (Cognito User Pool)"
url: "/p/pufs8kx"
date: "2021-05-28"
tags: ["AWS", "AWS/Cognito"]
weight: 1
---

Cognito とは？
----

[Amazon Cognito](https://aws.amazon.com/jp/cognito/) は、各種アプリケーションにユーザーの概念を取り入れて、サインアップやサインインなどを行えるようにするサービスです（OpenID Connect 準拠の Identity Provider (IdP) のひとつです）。
Cognito は大きく下記の 2 つの機能から成ります。

- __ユーザープール__: ユーザーを管理する。任意のアプリに、ユーザー登録（サインアップ）や、認証（サインイン）の機能を付けることができる。
- __ID プール__: 上記の仕組みでサインインしたユーザーに対して、AWS サービスへのアクセス権限を付加する。

単純にユーザー管理だけをしたいのであれば、前者の「ユーザープール」の機能だけを使うことができます。
実際には、認証後に AWS のリソースにアクセスさせることが多いと思いますので、後者の「ID プール」の仕組みを使って IAM の権限割り当てを行うことになります。

認証の仕組みに関しては、5 万 MAU（月間アクティブユーザー）まで無料で使えるので、個人で作成しているアプリでも使いやすいと思います。
ただし、無料枠を超えると 1 ユーザーあたり 1 円くらいかかり、多要素認証などを使おうとするともう少しお金がかかります。
詳しくは、[Cognito の料金](https://aws.amazon.com/jp/cognito/pricing/)）のページを参照してください。

通常、Web サイトに認証機能を付けたいと思うと、ユーザー管理の仕組みや、サインイン画面などの UI を作成する必要がありますが、Cognito はこれらをまとめて提供してくれます。
ここでは、Cognito のユーザープールの作成から、React アプリでの認証までを説明します。


Cognito にユーザープールを作成する
----

準備として、Cognito のユーザープールを作成しておきます。

{{< image border="true" w="600" src="img-001.png" >}}

1. [Cognito のマネージメントコンソール](https://console.aws.amazon.com/cognito/home)にアクセス
2. `ユーザープールの管理` → `ユーザープールを作成する` を選択します
3. ユーザー作成時の条件 (Attribute) などを設定して作成ボタンを押します

プール名は、適当に __`myapp-test`__ とでも付けておけば大丈夫です。
本番環境用のユーザープールと、開発／テスト用のユーザープールは分けて作っておくのがよいので、ここではサフィックスとして `-test` を付けておきました。
デフォルト設定でサクッと作成してしまうこともできるし、いろいろな条件（メールアドレス必須とかパスワード強度とか）を指定することもできます。
ここでは、とりあえずデフォルト設定で適当に作成してしまいましょう。
ユーザー名でなく、メールアドレスでもログインできるようにしたいときは、`属性 (Attribte)` のタブで、__`ユーザー名 - 検証済みの E メールアドレスでのサインインも許可`__ にチェックをいれておきます。

デフォルト設定は次のような感じになってます。

- ユーザーは自由にサインアップできる（Eメールアドレスが必須）
- ユーザー名の大文字・小文字は区別しない (Maku = maku = mAkU）
- パスワードは 8 文字以上（大文字 + 小文字 + 特殊文字 + 数字）

ユーザープールを作成すると、Cognito のマネージメントコンソール上から、ユーザーの追加や削除ができるようになります。

次に、「アプリクライアントの登録」という作業を行います。
これは、Web アプリやモバイルアプリなど、独自のアプリから Cognito のユーザープール（および ID プール）を利用するための設定です。


（参考）OAuth 2.0 の認可コードフロー
----

ここは読み飛ばしても構いませんが、[OAuth 2.0 の認可コード・グラントタイプ・フロー (RFC 6749 4.1)](https://openid-foundation-japan.github.io/rfc6749.ja.html#grant-code) を大まかにでも理解していると、Cognito での各種設定の意味がよく分かるようになります。

OAuth では認可サーバーを用いて、Web API の呼び出しに必要なアクセストークンを取得するまでの仕組みを定義しており、Cognito の認証＆認可もこの仕組みを利用しています。
正確には、OAuth はアプリにアクセス権限を与える「認可」の仕組みであって、サインインなどの「ユーザー認証」に関しては定義していませんが、ここではわかりやすさのために記載しています。

下記は、OAuth の __認可コードフロー (Authorization code grant)__ で定義されているアクセストークン取得の流れです。
Cognito の設定項目の中にも、まさに、OAuth の認可コードフロー (Authorization code grant) を有効にする、というチェックボックスがあります、

1. アプリが認可サーバー（認可エンドポイント）に認可リクエスト (with クライアント ID）を投げると、リソース参照の同意を求める画面が表示される。通常は、そのリソースを提供するサービスへのサインイン（認証）を伴うことになる（他のユーザーのリソースを勝手にアクセス許可できないので当たり前と言えば当たり前）。
1. ユーザーがサインインしてリソースアクセスを了承すると、認可サーバー（認可エンドポイント）から __認可コード (Authorization code)__ が返される。このとき、あらかじめクライアント ID に紐づけられている URL にリダイレクトされて、認可コード付きアドレスで Web アプリ側の画面に戻ってくる。
1. Web アプリは認可サーバー（トークンエンドポイント）に認可コードを送って __アクセストークン (Access token)__ を受け取る。

アクセストークンを取得したアプリは Web API を呼び出して、リソースサーバーから情報を取得できるようになります。
リソースサーバーは送られてきたアクセストークンが正しいものか判断した上で、問題なければ情報を返します（例えば、認可サーバーのイントロスペクションエンドポイントにアクセストークンを送ることで有効性を判断します）。
リソースサーバーと認可サーバーは同一であることがよくあります。

アクセストークンというのは、限られたリソースのみ（スコープといいます）にアクセスを許可するためのキーのようなものです。
このアクセストークンの定義と取得の流れが OAuth 認可フレームワークの要です。

{{% private %}}
- [OAuth 2.0のフローの種類と2つのエンドポイントについて解説 - ログミーTech](https://logmi.jp/tech/articles/322829)
{{% /private %}}


ユーザープールにアプリクライアント情報を登録する
----

### クライアント ID の発行

任意のクライアントから Cognite のユーザープールにアクセスするには、あらかじめクライアント情報を登録して、接続用の __クライアント ID__ を発行しておく必要があります。
複数のクライアントタイプ（Web アプリやモバイルアプリ）からのアクセスを想定しているのであれば、それぞれを別のクライアント情報として登録することが推奨されています。
これは、アクセストークン取得のフローなどが異なるからです（参考: [OAuth 2.0 - 2.1 クライアントタイプ](https://openid-foundation-japan.github.io/rfc6749.ja.html#client-types)）。

1. [Cognito のマネージメントコンソール](https://console.aws.amazon.com/cognito/home)で対象のユーザープールを選択します
2. __`アプリクライアント`__ → __`アプリクライアントの追加`__ を選択します
3. 適当な「アプリクライアント名」を入力します（`myapp-test-browser`) など
4. __`クライアントシークレットを作成`__ のチェックを外します（これはサーバーサイドにキーを埋め込んだりして、AWS の API 呼び出し時に直接指定したいときのもの）
5. __`アプリクライアントの作成`__ ボタンを押します

{{< image border="true" w="600" src="img-003.png" title="発行されたクライアント ID" >}}

上記のようにアプリクライアントの ID が発行されるので、これをメモしておきます。

### サインイン画面用のドメイン名を設定

Cognito はサインアップやサインインのための Web ページを提供してくれるのですが、そのための一意なアドレスを設定しておく必要があります。

{{< image border="true" w="500" src="img-004.png" >}}

1. ユーザープールのサイドバーから、__`アプリの統合／ドメイン名`__ を選択します
2. 適当なドメイン名を設定して `変更の保存` をクリックします


### コールバックアドレスの登録

次に、Web ブラウザ上のアプリから認証＆認可を行うために、作成しようとしている Web サイトの URL をコールバックアドレスとして登録します。

{{% note title="なぜコールバックアドレスの登録が必要か？" %}}
Cognito でのサインインはあくまで AWS 上のサイト（`https://<任意>.auth.ap-northeast-1.amazoncognito.com` など）で行われるため、一時的に Web ブラウザの URL は、自分のサイトから離脱することになります。
サインインが終了したあとに、もとの Web サイトに戻ってくるために、このコールバックアドレスに対してリダイレクトしてもらう必要があります。
{{% /note %}}

{{< image border="true" w="700" src="img-005.png" >}}

1. ユーザープールのサイドバーから、__`アプリの統合／アプリクライアントの設定`__ を選択します。
2. ID プロバイダ (IdP) として、今回作成した __`Cognito User Pool`__ を選択します（Cognito は他にも Google や Facebook のアカウントを使えるよう設定できます）。
3. サインイン後のリダイレクト先 URL を指定します（例: `https://myapp.example.com/`)。基本的に `https://` で始まるアドレスが必要ですが、テスト用クライアントであれば、__`http://localhost:3000/`__ のようなローカルアドレスを指定できます。
4. OAuth 2.0 の設定では、認証フローとして __`Authorization code grant`__ （前述の認可コードフロー）を選択し、必要なスコープ（アクセストークンで扱える権限）を選択します。スコープは迷ったらとりあえず全部選択しておきます。
5. 最後に __`変更の保存`__ を押して設定完了です。

{{% note title="Authorization code grant と Implicit grant" %}}
昔は Web ブラウザだけでのトークン取得には、__暗黙的フロー (Implicit grant)__ が使われることがありましたが、これは認可時に認可コードを経由してトークンを取得するというステップを踏まず、直接トークンをリダイレクト URL のフラグメントに付加してしまう方法です。
このフローはセキュリティホールになりやすく、現在では Web ブラウザでのトークン取得においても __認可コードフロー (Authorization code grant)__ を使うのが主流になっています。
{{% /note %}}

### サインイン画面を表示してユーザー登録してみる

上記の設定が完了すると、「アプリクライアントの設定」の下の方の __`ホストされた UI を起動`__ というリンクが有効になります。
このリンクをクリックすると、Cognito が提供してくれるサインイン（サインアップ）画面が表示されます。
デフォルトのサインイン画面はこのようにシンプルですが、もちろんカスタマイズできます。

{{< image border="true" w="400" src="img-006.png" >}}

今回はまだユーザーを登録していないと思いますので、ここの __`Sign up`__ リンクから、新しいユーザーをユーザープールに登録してみましょう。
登録時に指定したメールに送られてくる確認コード (verification code) を入力すれば、ユーザーの登録が完了するはずです。

確認コード入力後に、先ほど設定したリダイレクト先アドレス（例: `http://localhost:3000/?code=3bc09fe0-ba9e-4b88-98c2-58eacdc99006`）に飛ぶので、Web ブラウザではページが見つからないというエラーになりますが、この段階では無視して大丈夫です。
Cognito サービス側のユーザーの登録処理と、Eメールアドレスの検証処理自体は正しく完了しています。
Cognito のコンソールで「ユーザーとグループ」タブを選択すれば、そのユーザーが登録されていることを確認できるはずです。


Node.js アプリからサインインしてみる
----

{{% private %}}
- [Authentication - Sign up, Sign in & Sign out - Amplify Docs](https://docs.amplify.aws/lib/auth/emailpassword/q/platform/js)
{{% /private %}}

いよいよ、独自アプリから Cognito による認証を使用してみます。
React アプリ内からサインイン画面を表示したりする前に、まずは単純な Node.js のコンソールアプリで Cognito 認証を試してみます。
[aws-amplify](https://www.npmjs.com/package/aws-amplify) という npm パッケージを使うと簡単です。
AWS Amplify サービス自体を使うわけではないのですが、Cognito 関連の SDK もここに含まれているのでこれを使います。

{{< code >}}
$ npm install aws-amplify （あるいは yarn add）
{{< /code >}}

次のコードを実行してユーザー認証に成功すると、標準出力にアクセストークンなどの情報が出力されます。
もちろんユーザー情報は、前述の作業で登録したユーザーのものを指定してください。

{{< code lang="ts" title="signin.ts" >}}
import { Amplify, Auth } from 'aws-amplify';

// 使用するユーザープール、クライアントの ID を指定
Amplify.configure({
  Auth: {
    region: 'ap-northeast-1',
    userPoolId: 'ap-northeast-1_w4Lb7OMrk',
    userPoolWebClientId: '7egbq6mnk61udtpa0v7qr5u96o',
  }
});

async function signIn() {
  const username = 'User-1';
  const password = 'Password-1';
  try {
    const user = await Auth.signIn(username, password);
    console.log(user);
    // console.log(user.attributes.email);
  } catch (error) {
    console.error(error);
  }
}

signIn();
{{< /code >}}

`Amplify.configure()` で指定する ID 情報は、Cognito マネージメントコンソールの、ユーザープール管理画面で確認できます。

- __`userPoolId`__: `全般設定` タブの `プール ID`
- __`userPoolWebClientId`__: `アプリクライアント` タブの `アプリクライアント ID`


React アプリでサインインが必要な画面を作る
----

[@aws-amplify/ui-react](https://docs.amplify.aws/ui/q/framework/react) パッケージは、Cognito を利用するための React コンポーネントを提供します。
`aws-amplify` パッケージに加えて、このパッケージをインストールします。

{{< code >}}
$ npm install aws-amplify @aws-amplify/ui-react
{{< /code >}}

このパッケージが提供する [AmplifyAuthenticator](https://docs.amplify.aws/ui/auth/authenticator/q/framework/react) コンポーネントを使用すると、Cognito で __サインイン済みでないと表示できないページ__ を作成することができます。
下記は Next.js アプリのカスタム App (`pages/_app.tsx`) 実装例ですが、このように全体を `AmplifyAuthenticator` で囲むと、ページ全体がサインイン済みのときのみ表示されるようになります。
Web ブラウザ上で動作する React アプリにおいても、Cognito ユーザープールにアクセスする前に、[Amplify.configure 関数](https://docs.amplify.aws/lib/client-configuration/configuring-amplify-categories/q/platform/js) で接続情報をセットしておかなければいけないのは同様です。

{{< code lang="tsx" title="pages/_app.tsx" >}}
import type { AppProps } from 'next/app';
import { Amplify } from 'aws-amplify';
import { AmplifyAuthenticator, AmplifySignOut } from '@aws-amplify/ui-react';

// Cognito ユーザープールとクライアントの ID 設定
Amplify.configure({
  Auth: {
    region: 'ap-northeast-1',
    userPoolId: 'ap-northeast-1_w4Lb7OMrk',
    userPoolWebClientId: '7egbq6mnk61udtpa0v7qr5u96o',
  }
});

function MyApp({ Component, pageProps }: AppProps): JSX.Element {
  return (
    <AmplifyAuthenticator>
      <AmplifySignOut />
      <Component {...pageProps} />
    </AmplifyAuthenticator>
  );
}

export default MyApp;
{{< /code >}}

サインインしていない状態では、`AmplifyAuthenticator` コンポーネントは、次のようにサインイン画面を表示してくれます。

{{< image border="true" w="400" src="img-007.png" >}}

サインインすると、次のように通常のページが表示されます。
今回は、先頭に `AmplifySingOut` コンポーネントを配置しているので、サインアウト用のボタンが表示されています。

{{< image border="true" w="400" src="img-008.png" >}}

上記のコードで何となくサインインが必要な Web サイトができた感じがしますが、実際には `@aws-amplify/ui-components` モジュールが提供する __`AuthState`__ の状態を見て、サインイン済みかどうかを判断して表示処理を分岐させた方がよいようです（おそらく、`AmplifyAuthenticator` コンポーネントのレンダリングが常に走ってしまうのを防ぐため）。
このあたりは、[Amplify SDK の公式サイト](https://docs.amplify.aws/ui/auth/authenticator/q/framework/react#recommended-usage) にも recommended way として記載されています。
例えば、次のような感じで表示部分の場合分けをすればよさそうです。

{{< code lang="tsx" title="pages/_app.tsx" >}}
import React from 'react'
import Head from 'next/head'
import type { AppProps } from 'next/app'
import { Amplify } from 'aws-amplify'
import { AmplifyAuthenticator } from '@aws-amplify/ui-react'
import config from '../libs/config'
import { useAuthState } from '../libs/useAuthState'

// Cognito ユーザープールとクライアントの ID 設定
Amplify.configure(config.amplify)

export default function MyApp({ Component, pageProps }: AppProps): JSX.Element {
  const { isSignedIn } = useAuthState()

  return isSignedIn ? (
    <>
      <Head>
        <title>My App</title>
        <meta
          name="viewport"
          content="minimum-scale=1, initial-scale=1, width=device-width"
        />
      </Head>
      <Component {...pageProps} />
    </>
  ) : (
    <AmplifyAuthenticator />
  )
}
{{< /code >}}

上記の例では、`useAuthState()` というカスタムフックを作成して、現在のサインイン状態やユーザー情報 (`CognitoUserInteface`) を簡単に取得できるようにしています。
下記はこのカスタムフックの実装例ですが、もう少し綺麗に書けるうまいやり方があるといいなぁ。。。
副作用フック (`useEffect`) の中の非同期 API 呼び出しで、状態セットしているのも若干問題ありですがとりあえずは動作します。
できれば公式 SDK でこういうフックサポートして欲しい。

{{< accordion title="libs/useAuthState.ts" >}}
{{< code lang="ts" >}}
import { useEffect, useState } from 'react'
import { Auth } from 'aws-amplify'
import { AuthState, CognitoUserInterface, onAuthUIStateChange } from '@aws-amplify/ui-components'

export type UseAuthStateOutput = {
  isSignedIn: boolean
  user: CognitoUserInterface | undefined
}

export const useAuthState = (): UseAuthStateOutput => {
  // Cognito によるサインイン状態やユーザー情報を保持するステート
  const [authState, setAuthState] = useState<AuthState>()
  const [user, setUser] = useState<CognitoUserInterface | undefined>()

  // サインイン状態やユーザー情報の変化をハンドル
  useEffect(() => {
    // 画面遷移時に onAuthUIStateChange が呼ばれないことへの対応
    // https://github.com/aws-amplify/docs/issues/2895
    if (authState === undefined) {
      Auth.currentAuthenticatedUser()
        .then((authData) => {
          setAuthState(AuthState.SignedIn)
          setUser(authData)
        })
        .catch(() => {
          /* Nothing to do */
        })
    }

    // 公式にはこの呼び出しだけでよいような記述がありますが。。。
    return onAuthUIStateChange((nextAuthState, authData) => {
      setAuthState(nextAuthState)
      setUser(authData as CognitoUserInterface)
    })
  }, [authState])

  return { isSignedIn: authState === AuthState.SingedIn, user }
}
{{< /code >}}
{{< /accordion >}}


（おまけ）サインイン状態とユーザー名の取得
----

次の `UserInfo` コンポーネントは、Cognito でサインインが完了して、そのユーザー情報が取得できているときは、`Hello, <ユーザー名>` と表示し、それ以外の場合は `Not signed in` と表示します。
もっとも、サインイン後にしか描画しないページコンポーネントの中でこのコンポーネントを使うのであれば、`Not signed in` の文字を見ることはありません。

{{< code lang="tsx" title="components/UserInfo.tsx" >}}
import React from 'react'
import { useAuthState } from '../libs/useAuthState'

export const UserInfo: React.FC = () => {
  const { user } = useAuthState()

  // サインイン状態によって表示内容を分ける
  return user ? (
    <h1>Hello, {user.username}</h1>
  ) : (
    <h1>Not signed in</h1>
  )
}
{{< /code >}}


（おまけ）Sign-In 画面や Sign-Up 画面をカスタマイズする
----

Amplify ライブラリが提供するデフォルトの [ユーザー作成画面 (Sign Up)](https://docs.amplify.aws/ui/auth/authenticator/q/framework/react/#sign-up) には、「電話番号 (Phone Number)」の入力欄などが表示されます。
この画面の表示項目をカスタマイズするには、`AmplifyAuthenticator` コンポーネントの下に __`AmplifySignUp`__ コンポーネントを配置して、その属性で表示内容を指定します。
次の例では、「サインアップ画面 (`AmplifySignUp`)」と「サインイン画面 (`AmplifySignIn`)」をカスタマイズしています。


{{< code lang="tsx" >}}
// import {
//   AmplifyAuthenticator, AmplifySignIn, AmplifySignUp
// } from '@aws-amplify/ui-react'

<AmplifyAuthenticator>
  <AmplifySignUp
    slot="sign-up"
    headerText="ユーザーの新規登録"
    formFields={[
      { type: 'username', label: 'ユーザー名 *' },
      { type: 'email', label: 'メールアドレス *' },
      { type: 'password', label: 'パスワード *' },
    ]}
  />
  <AmplifySignIn
    slot="sign-in"
    headerText="MyApp サインイン"
    submitButtonText="サインイン"
    formFields={[
      { type: 'username', label: 'ユーザー名' },
      { type: 'email', label: 'メールアドレス' },
    ]}
  />
</AmplifyAuthenticator>
{{< /code >}}

サインイン画面から、「Create account」のリンクを消すには、`AmplifySignIn` コンポーネントの `hideSignUp` 属性を true に設定します（`=` 以降は省略可）。

{{< code lang="tsx" >}}
<AmplifyAuthenticator>
  <AmplifySignIn slot="sign-in" hideSignUp />
</AmplifyAuthenticator>
{{< /code >}}

{{% private %}}
次のステップ
----

- [Cognito ユーザープールを使ったサインイン後に AWS リソースへのアクセス権限を与える (Cognito Identity Pool)](/p/3mx8hr2)
{{% /private %}}


参考
----

- [Amazon Cognito (2) サインイン後に AWS リソースへのアクセス権限を与える (Cognito Identity Pool)](/p/3mx8hr2)

