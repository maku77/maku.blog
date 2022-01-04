---
title: "GitHub OAuth トークンを取得する (2) Azure Functions 経由で取得する"
url: "/p/ar2bjs2"
date: "2020-08-18"
lastmod: "2020-08-30"
tags: ["Azure Functions", "GitHub", "OAuth", "React"]
weight: 102
---

何をするか？
----

ここでは、静的な Web サイト（の JavaScript) から、GitHub の OAuth トークンを取得できるようにしてみます。
この処理を実装すると、GitHub と連携した Web アプリを、GitHub Pages や Azure Static Web Apps などの、静的サイト用のホスティングサービス上で公開できるようになります。
汎用的な Web サーバー（VPSなど）でホスティングする場合と比べ、非常に安価に運用することができます。

前提知識として、下記の GitHub の OAuth トークン取得の流れを理解しているものとします。

- 参考: [GitHub OAuth トークンを取得する (1) 処理の流れを理解する](/p/ubkt3ai)

静的な Web サイトから GitHub のアクセストークンを取得するときにネックになるのが、クロスオリジン通信 (CORS) の制約です。
また、GitHub のアクセストークンリクエストには、クライアントシークレット情報が必須であり（2020年、2021年現在）、これをクライアントサイドの JavaScript にハードコードするわけにはいきません。
よって、ブラウザ上で実行される JavaScript からは、実質アクセストークンの取得ができないので、何らかのバックエンドサーバーを介す形でアクセストークンを取得する必要があります。

{{< image src="img-001.png" >}}

バックエンドサーバーはどのようなものでも構わないのですが、ここではサーバーレス環境である __Azure Functions__ を使ってアクセストークンの取得機能を実装します。


Azure Functions に関数を追加する
----

Azure Functions に HTTP トリガーで起動する関数を追加し、GitHub の OAuth トークンを取得する処理を実装します。
まずは下記の記事を参考にして、Functions アプリ（プロジェクト）を作成してください。

- 参考: [Azure Functions で簡単な関数を作ってみる](/p/vgt5g7f)

作成する関数の仕様は次のとおりとします。

- HTTP GET リクエストのクエリ文字列で、OAuth アプリのクライアント ID (`client_id`) と、GitHub から取得した一時コード (`code`) を受け取る。
- OAuth アプリのクライアントシークレット (`client_secret`) は Functions アプリの環境変数（アプリケーション設定）で設定しておく（ハードコードしない）。
- GitHub に対してアクセストークンの取得リクエスト (HTTP POST) を送り、その結果をクライアントへそのまま返す。ただし、クライアントから指定されたクライアント ID (`client_id`) がそもそも不正な場合は、アクセストークンの取得は行わず、直ちに 400 (Bad Request) エラーを返す。

### 環境変数の設定

Functions アプリの __`環境`__ → __`構成`__ を開き、GitHub に登録した OAuth アプリの「クライアントID」と「クライアントシークレット」を登録してください。
ここでは次のような名前で登録することにします。

- __`MYAPP_CLIENT_ID`__ ... クライアント ID
- __`MYAPP_CLIENT_SECRET`__ ... クライアントシークレット

{{< image w="600" border="true" src="img-002.png" >}}

アプリケーション設定を追加した場合は、最後に __`保存`__ ボタンを押して反映するのを忘れないでください（これを忘れてハマりがち...）。
ここで設定した変数は、関数の JavaScript コードから `process.env` で参照できます。

### CORS の設定

ブラウザ上の JavaScript から Azure Functions の関数を実行するときは、CORS 設定をしてクロスオリジン通信を許可しておく必要があります。
Functions アプリの __`API`__ → __`CORS`__ を開いて、Web サイトをホスティングするドメインを登録してください。
典型的には、

- `https://example.com`

といったドメイン名でアドレスを指定しますが、開発用のサーバーでテストする場合は、次のようなローカルアドレスを登録しておきます。

- `http://localhost:1234`

### Node モジュールのインストール

HTTP リクエスト用のモジュールとして __`node-fetch`__ を使用するので、Functions アプリの __`開発ツール`__ → __`コンソール`__ を開いてインストールしてください。

{{< code >}}
D:\home\site\wwwroot> npm init -y
D:\home\site\wwwroot> npm install node-fetch --save
{{< /code >}}

もちろん、ローカルで実行して `package.json` をデプロイする方法でも大丈夫です。
ここでは、サクッと Azure ポータル上で作業を完了させてしまいます。

### 関数を作成する

まずは Funtions アプリに任意の名前で HTTP trigger 型の関数を追加してください。
ここでは、__`GitHubToken`__ という名前で関数を作成し、下記のようなコードを入力します。
若干長いですが、やっていることは HTTP POST リクエストで GitHub のアクセストークン取得リクエストを送っているだけです。

{{< code lang="javascript" title="GitHubToken/index.js" >}}
const fetch = require('node-fetch');

const RESPONSE_400_BAD_REQUEST = {
    status: 400,
    body: 'Request paramters are invalid'
};

/** 正しく環境変数がセットされているか確認する */
function checkEnv() {
    if (!process.env.MYAPP_CLIENT_ID) return false;
    if (!process.env.MYAPP_CLIENT_SECRET) return false;
    return true;
}

/** クライアントIDに対応するクライアントシークレットを取得する */
function getClientSecret(clientId) {
    if (clientId === process.env.MYAPP_CLIENT_ID) {
        return process.env.MYAPP_CLIENT_SECRET;
    }
    return undefined;
}

/** GitHub からアクセストークンを取得する */
async function requestToken(clientId, clientSecret, code, context) {
    const URL = 'https://github.com/login/oauth/access_token';

    // HTTP リクエストの設定
    const option = {
        method: 'POST',
        headers: {
            Accept: 'application/json',
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({
            client_id: clientId,
            client_secret: clientSecret,
            code: code,
        })
    };

    // HTTP POST リクエストを送信
    try {
        const res = await fetch(URL, option);
        context.log.info(`Response from GitHub: status=${res.status}`);
        return {
            status: res.status,
            body: await res.json()
        };
    } catch (err) {
        context.log.error(JSON.stringify(err));
        throw err;
    }
}

/** 関数のエントリポイント */
module.exports = async function (context, req) {
    if (!checkEnv()) throw new Error('Environment variables are not set');

    const clientId = req.query.client_id;
    const code = req.query.code;
    context.log(`Request from client: client_id=${clientId}, code=${code}`);

    const clientSecret = getClientSecret(clientId);
    if (!clientSecret || !code) {
        context.log.warn(RESPONSE_400_BAD_REQUEST.body);
        context.res = RESPONSE_400_BAD_REQUEST;
        return;
    }

    context.res = await requestToken(clientId, clientSecret, code, context);
}
{{< /code >}}


Web アプリ側の JavaScript を実装する
----

以上で、Azure Functions に GitHub アクセストークンを取得するための関数を追加できました。
あとはこれを利用して、静的な Web サイトの JavaScript からアクセストークンを取得できます。

{{< image w="700" border="true" src="img-sign-in.png" >}}

ここでは、GitHub から取得したアクセストークンをローカルストレージに `GITHUB_TOKEN` というキーで保存し、その値の有無によって「サインイン済み」かどうかを判断しています。
画面上の `Sing In` ボタンを押すと、一時トークンの取得要求を送信し、そのコールバックを受けてアクセストークンを取得しに行きます。
アクセストークンの取得が完了すると、その値をローカルストレージに保存し、UI を更新します。
サインイン済みのときは、`Sign In` ボタンの代わりに `Sign Out` ボタンを表示するようにしています。

1 つの HTML ファイル（JS コード含む）でサインイン・アウト処理をひととおり実現しているので、若干スパゲッティコードになってます。すみません（^^;

{{< code lang="html" title="index.html" >}}
<!DOCTYPE html>
<html lang="ja">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>My App</title>
</head>
<body>
  <h1>My App</h1>
  <button id="btnSignIn" onclick="signIn()" hidden>Sign In</button>
  <button id="btnSignOut" onclick="signOut()" hidden>Sign Out</button>
  <div id="message"></div>

<script>
const GITHUB_CLIENT_ID = '29ed0f3a65af5342d5bd';
const GITHUB_API_SCOPE = 'repo';

window.addEventListener('DOMContentLoaded', (event) => {
  updateUi();
})

// 画面更新（サインイン済みかどうかで内容を変える）
function updateUi() {
  const btnSignIn = document.getElementById('btnSignIn')
  const btnSignOut = document.getElementById('btnSignOut')
  const message = document.getElementById('message')
  if (isSignedIn()) {
    btnSignIn.hidden = true
    btnSignOut.hidden = false
    message.innerText = 'サインイン済み: token=' + localStorage.getItem('GITHUB_TOKEN')
  } else {
    btnSignIn.hidden = false
    btnSignOut.hidden = true
    message.innerText = 'サインインしていません'
  }
}

// GitHub のサインイン（OAuth 認証処理を開始）
// すでに認可されているなら GitHub から即リダイレクトされる
function signIn() {
  window.location.href = 'https://github.com/login/oauth/authorize?' +
    `client_id=${GITHUB_CLIENT_ID}&scope=${GITHUB_API_SCOPE}`;
}

// サインアウト（ローカルストレージを削除するだけ）
function signOut() {
  localStorage.removeItem('GITHUB_TOKEN');
  updateUi()
}

function isSignedIn() {
  const token = localStorage.getItem('GITHUB_TOKEN')
  return token != null
}

// GitHub からのリダイレクト時にアクセストークンを取得する
function handleSignInCallback() {
  const params = window.location.search;
  const code = params.startsWith('?code=') ? params.split('=')[1] : undefined;
  if (code) {
    // 一時コードを取得したらアドレスを戻しておく
    window.history.replaceState(null, null, window.location.pathname);
    getGitHubToken(code);
  }
}

function getGitHubToken(code) {
  const URL = 'https://sample-xxx.azurewebsites.net/api/GitHubToken' +
    `?client_id=${GITHUB_CLIENT_ID}&code=${code}`;
  fetch(URL)
    .then(res => res.json())
    .then(json => {
      if (json.access_token) {
        localStorage.setItem('GITHUB_TOKEN', json.access_token)
        updateUi()
      } else {
        throw new Error('Could not obtain access token (bad request)');
      }
    })
    .catch(err => alert(err.name + ': ' + err.message));
}

handleSignInCallback();
</script>
</body>
</html>
{{< /code >}}

これで、Web サイト内の JavaScript から GitHub API を呼び出せるようになります。
ただ、これくらい複雑になってくると、React などを使ってコンポーネント化していった方がよいですね。


React を使って実装する
----

サインイン状態に応じた表示の切り替えなどは、React を使って実装するとスッキリします。
例えば React Router を使って、URL のパスとコンポーネントを次のように構成します。

### 全体構成

{{< image src="img-state-chart.png" >}}

現在サインイン状態かどうかは、Web ブラウザのローカルストレージに GitHub のアクセストークン (`GITHUB_TOKEN`) が保存されているかどうかで判断します（前述の例と同じ）。

- {{< label "/" >}} ... Top コンポーネント
    - {{< label "未サインインの場合：" >}} 「サインイン」ボタンが配置されたトップ画面を表示。「サインイン」ボタンを押すと `/signin` へリダイレクト。
    - {{< label "サインイン済の場合：" >}} `/main` へリダイレクト。
- {{< label "/signin" >}} ... SignIn コンポーネント
    - GitHub のサインインページ（認証・認可）へジャンプし、`/signin?code=XXX` というコールバックで一時コードを受け取る。一時コードを受け取ったら、HTTP リクエストでアクセストークンを取得し、ローカルストレージに保存する。最後に `/main`（あるいは `/`）へリダイレクト。
- {{< label "/signout" >}} ... SignOut コンポーネント
    - ローカルストレージに保存されたアクセストークンを削除して、`/` へリダイレクト。
- {{< label "/main" >}} ... Main コンポーネント
    - {{< label "未サインインの場合：" >}} `/` へリダイレクト。
    - {{< label "サインイン済の場合：" >}} GitHub API を利用するアプリのメイン画面を表示。

ここでは、`/signin` というパスで GitHub からの一時コードを受け取ることにしているので、最初に GitHub の [OAuth Apps の設定](https://github.com/settings/developers) で、コールバックアドレスを `/signin` に変更しておく必要があります。

以下、それぞれのコンポーネントを順番に見ていきます。

### App コンポーネント

まず、全体のルーティングを管理する `App` コンポーネントです。
React Router のパス指定によって、表示するコンポーネントを切り替えます。

{{< code lang="jsx" title="App.tsx" >}}
import * as React from 'react';
import { BrowserRouter as Router, Redirect, Route, Switch } from 'react-router-dom';
import { Main } from './Main';
import { SignIn } from './SignIn';
import { SignOut } from './SignOut';
import { Top as Top } from './Top';

export const App: React.FC = () => {
  return (
    <Router>
      <Switch>
        <Route path="/" exact={true} component={Top} />
        <Route path="/signin" component={SignIn} />
        <Route path="/signout" component={SignOut} />
        <Route path="/main" component={Main} />
        <Redirect to="/" />
      </Switch>
    </Router>
  );
};
{{< /code >}}

### Config クラス

下記の `Config` クラスは、React コンポーネントではありませんが、GitHub のアクセストークンをローカルストレージに保存するためのユーティリティとして作成しておきます。

{{< code lang="typescript" title="Config.ts" >}}
export class Config {
  /** ローカルストレージ用のキー */
  private static KEY_GITHUB_TOKEN = 'GITHUB_TOKEN';

  /** サインイン済みかどうか確認します。 */
  public static isSignedIn(): boolean {
    return !!this.getToken();
  }

  /** アクセストークンを保存します。 */
  public static setToken(token: string) {
    localStorage.setItem(this.KEY_GITHUB_TOKEN, token);
  }

  /** アクセストークンを取得します。 */
  public static getToken(): string {
    return localStorage.getItem(this.KEY_GITHUB_TOKEN);
  }

  /** アクセストークンを削除します。 */
  public static removeToken() {
    localStorage.removeItem(this.KEY_GITHUB_TOKEN);
  }
}
{{< /code >}}

### Top コンポーネント

{{< image border="true" w="400" src="img-react-001.png" >}}

最上位のパス (`/`) にアクセスしたときは、`Top` コンポーネントを表示します。
`Top` コンポーネントは、ユーザーがまだサインインしていない場合は Welcome メッセージ的なものを表示し、サインインするためのボタンを表示します。
サインイン済みであれば、メイン画面 (`/main`) にリダイレクトします。

{{< code lang="jsx" title="Top.tsx" >}}
import * as React from 'react';
import { Link, Redirect } from 'react-router-dom';
import { Config } from './Config';

export const Top: React.FC = () => {
  if (Config.isSignedIn()) {
    return <Redirect to="/main" />;
  }

  return <>
    <h1>Top（トップ画面）</h1>
    <p>
      サインインボタンを押すと、サインイン処理 (/signin) を行い、<br />
      メイン画面 (/main) へ遷移します。<br />
      すでにサインイン済みであれば、自動的にメイン画面へ遷移します。
    </p>
    <Link to="/signin">サインイン</Link>
  </>;
};
{{< /code >}}

### Main コンポーネント

{{< image border="true" w="400" src="img-react-002.png" >}}

サインイン後のメイン画面は `Main` コンポーネントが担当します。
まだサインインしていない場合は、トップ画面 (`/`) へ飛ばします。
つまり、`Top` コンポーネントと逆の振る舞いですね。

{{< code lang="jsx" title="Main.tsx" >}}
import * as React from 'react';
import { Link, Redirect } from 'react-router-dom';
import { Config } from './Config';

export const Main: React.FC<{}> = () => {
  if (!Config.isSignedIn()) {
    return <Redirect to="/" />;
  }

  return <>
    <h1>Main（メイン画面）</h1>
    <p>
      GitHub 認証後に表示可能なメイン画面です。<br />
      GitHub API で取得した情報を表示します。<br />
      サインアウトボタンを押すと、サインアウト処理 (/signout) を行い、<br />
      ようこそ画面 (/) に戻ります。
    </p>
    <Link to="/signout">サインアウト</Link>
  </>;
};
{{< /code >}}

### SignIn コンポーネント

React Router で `/signin` にアクセスすると、GitHub のサインイン処理（一時コード取得＆アクセストークン取得）が実行されます。
GitHub からのコールバック（リダイレクト）を処理するため、処理が若干複雑になっています。

{{< code lang="jsx" title="SignIn.tsx" >}}
import * as querystring from 'querystring';
import * as React from 'react';
import { Redirect } from 'react-router-dom';
import { Config } from './Config';

const GITHUB_CLIENT_ID = '29ed0f3a65af5342d5bd';
const GITHUB_API_SCOPE = 'repo';
//const GITHUB_API_SCOPE = 'repo read:user';

/**
 * GitHub のサインイン（OAuth 認証処理を開始）画面へ遷移します。
 * すでに認可されている場合は、GitHub からコールバック URL へ即リダイレクトされます。
 */
function signIn() {
  const query = querystring.stringify({
    client_id: GITHUB_CLIENT_ID,
    scope: GITHUB_API_SCOPE,
  });
  window.location.href = 'https://github.com/login/oauth/authorize?' + query;
}

/**
 * GitHub からのリダイレクト時に URL からアクセストークンを取得します。
 */
function handleSignInCallback() {
  const params = window.location.search;
  const code = params.startsWith('?code=') ? params.split('=')[1] : undefined;
  return code;
}

/**
 * Azure Functions 経由で GitHub のアクセストークンを取得します。
 */
function getGitHubToken(code: string, callback: (token: string) => void) {
  const query = querystring.stringify({
    client_id: GITHUB_CLIENT_ID,
    code: code,
  });
  const url = 'https://sample-xxx.azurewebsites.net/api/GitHubToken?' + query;

  fetch(url)
    .then(res => res.json())
    .then(json => {
      if (json.access_token) {
        callback(json.access_token);
      } else {
        throw new Error('Could not obtain access token (bad request)');
      }
    })
    .catch(err => alert(err.name + ': ' + err.message));
}

export const SignIn: React.FC = () => {
  const [isSignedIn, setIsSignedIn] = React.useState(false);

  // 非同期なサインイン処理は useEffect の中で行う
  React.useEffect(() => {
    // サインイン済みであればステートを変更して終了
    if (Config.isSignedIn()) {
      setIsSignedIn(true);
      return;
    }

    // URL から GitHub からの一時コードを抽出
    const code = handleSignInCallback();
    if (code) {
      // 一時コードを見つけたらアクセストークンの取得処理を行う
      // alert('一時コード: ' + code);
      getGitHubToken(code, (token: string) => {
        // alert('アクセストークン: ' + token);
        Config.setToken(token);
        setIsSignedIn(true);
      });
    } else {
      // 純粋な '/signin' へのアクセスであれば、GitHub の認証ページへジャンプ
      signIn();
    }
  }, []);

  if (isSignedIn) {
    return <Redirect to="/main" />;
  } else {
    return <p>サインイン処理中...</p>;
  }
};
{{< /code >}}

`GITHUB_CLIENT_ID` に関しては、ローカル開発用と本番サーバー用では違うものを登録することになるため、実際には次のような感じで切り替えて使用する必要があります。
この ID によって、GitHub から一時コードが返されるときのコールバック URL が切り替わります。

{{< code lang="typescript" >}}
// GitHub Client ID はローカル開発用と本番サーバ用で分ける
const GITHUB_CLIENT_ID = process.env.NODE_ENV === 'development' ?
  '29ed0f3a65af5342d5bd' : 'a18ace3b4680392f5225';
{{< /code >}}

### SignOut コンポーネント

パス `/signout` にアクセスすると、サインアウト処理が実行されます。
といっても、ローカルストレージに保存されたアクセストークンを削除し、トップ画面 (`/`) にリダイレクトするだけなので、サインイン処理ほど複雑ではありません。

{{< code lang="jsx" title="SignOut.tsx" >}}
import * as React from 'react';
import { Redirect } from 'react-router-dom';
import { Config } from './Config';

export const SignOut: React.FC<{}> = () => {
  Config.removeToken();
  return <Redirect to="/" />;
};
{{< /code >}}

- - - -

以上で、React アプリの中で GitHub の OAuth アクセストークンを取得する実装は完成です。
メイン画面 (`/main`) にアクセスしている最中は、サインイン状態であることが保証されているので、アクセストークンを利用して GitHub API を呼び出すことができます。

GraphQL 版の GitHub API (ver.4) を使用するときは、[Apollo Client と React を組み合わせて使う](/p/qcp2cnx) とシンプルに実装できます。

