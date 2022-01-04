---
title: "GitHub OAuth トークンを取得する (1) 処理の流れを理解する"
url: "/p/ubkt3ai"
date: "2020-08-16"
tags: ["GitHub", "OAuth"]
weight: 101
---

{{< private >}}
- [Authorizing OAuth Apps (Web application flow) - GitHub Docs](https://docs.github.com/en/developers/apps/authorizing-oauth-apps#web-application-flow)
{{< /private >}}


GitHub の OAuth トークンとは
----

GitHub API を使って GitHub 上の情報（リポジトリ情報やユーザー情報）を取得するには、GitHub によって発行されるアクセストークンが必要です。
GitHub API ver.3 (REST API) ではアクセストークンを必要としない API もありましたが、GitHub API ver.4 (GraphQL API) では必ずアクセストークンが必要です。

アクセストークンにはいくつか種類があり、GitHub の Web サイト上で作成する __パーソナルアクセストークン__ や、Web アプリの OAuth プロセスで取得する __OAuth アクセストークン__ などがあります。
アクセストークンの取得方法は異なりますが、いずれも取得した後は同じように使用できます。

- {{< label "パーソナルアクセストークン" >}}: ユーザーが GitHub サイト上で作成
- {{< label "OAuth アクセストークン" >}}: Web アプリ内の OAuth プロセスで取得

通常、Web アプリから GitHub API を使用する場合は、後者の OAuth を使用してアクセストークンの取得を自動化します。
ここでは、GitHub の OAuth アクセストークン取得の流れを、実装コードを示しながら説明していきます。


GitHub の OAuth の流れ (Web application flow)
----

Web アプリから OAuth トークンを取得するまでの流れはざっと次のような感じになります。

### (1) GitHub のログイン画面に遷移

Web アプリから次のようなアドレスで GitHub のログインページにジャンプします。

{{< code >}}
https://github.com/login/oauth/authorize?client_id=XXX&scope=YYY`
{{< /code >}}

すると、GitHub のサイト上で、この Web アプリに情報を提供してもいいかの確認が表示されるので、ここでユーザーが OK すると、あらかじめ登録してあったコールバックアドレスに Web ブラウザがリダイレクトされます。
OAuth アプリの登録方法に関しては後述しますが、上記 URL の `client_id` パラメータには登録済みの OAuth アプリの ID を指定し、`scope` パラメータには取得したい情報のスコープ（`repo` や `user`）を指定します。

### (2) 一時コードの取得

GitHub からのリダイレクト要求により、リダイレクト先のページ（自分の Web サイト内）に Web ブラウザが自動的にジャンプします。
このとき、URL の末尾にアクセストークン取得用の一時コード (temporary code) が次のような形で付いてきます。

{{< code >}}
http://localhost:1234?code=ABCDEABCDEABCDE
{{< /code >}}

この __一時コードはアクセストークンではない__ ので注意してください。
アクセストークンは、後述の HTTP POST メソッドで、一時コードを送ることで取得することができます。

この段階では、JavaScript などで URL をパースし、一時コードを抽出しておきます。
__一時コードの有効期間は 10 分間__ とされているので、すぐにアクセストークンを取得する必要があります。
アクセストークンの方は Revoke されるまで有効です。

### (3) アクセストークンの取得

前のステップで取得した一時コードを使って、次のアドレスに __POST__ リクエストを送ると、晴れて OAuth アクセストークンを取得することができます。

{{< code >}}
https://github.com/login/oauth/access_token
{{< /code >}}

POST リクエストには、次のようなパラメータを含めておく必要があります。

- {{< label "client_id" >}} ... 登録した OAuth アプリに割り当てられたクライアント ID（文字列）
- {{< label "client_secret" >}} ... 登録した OAuth アプリに割り当てられたクライアントシークレット（文字列）
- {{< label "code" >}} ... 前のステップで取得した一時コード（文字列）

レスポンスの形式は、POST リクエスト時に指定した `Accept` ヘッダの値によって変化します。
例えば、__`Accept: application/json`__ と指定してリクエストすると、次のような JSON 形式のレスポンスが返ってきます。

{{< code lang="json" >}}
{
  "access_token": "e72e16c7e42f292c6912e7710c838347ae178b4a",
  "scope": "repo,gist",
  "token_type": "bearer"
}
{{< /code >}}

この中の __`access_token`__ が、OAuth アクセストークンです。
これを GitHub API を呼び出すときのリクエストヘッダに指定することになります。

{{< note title="CORS 制約" >}}
残念ながら、最後のステップのアクセストークンを取得する API は、クロスオリジン通信 (CORS) の制約のため、Web ブラウザ上で動作する JavaScript からは呼び出せないようになっています。
GitHub のアクセストークンは、外部サーバなどで動作する Node.js プログラムなどを経由して取得しなければいけません。
ここでは、最後のステップの HTTP POST リクエストだけ Node.js プログラムを使って実行し、アクセストークンが正しく取得できるところまでを確認します。
{{< /note >}}


OAuth アプリの登録
----

GitHub から一時コードを発行してもらうには、下記の設定ページから OAuth アプリを登録して、クライアント ID を生成しておく必要があります。
登録と言っても、アプリ名と一時コード発行時のコールバックアドレスなどを設定するだけで簡単に終わります。

- [GitHub / Settings / Developer settings / OAuth App](https://github.com/settings/developers)

{{< image w="600" border="true" src="img-001.png" >}}

Web アプリでの GitHub 認証時には、ここで設定したアプリ名がユーザーに提示されることになります。
怪しいアプリだと思われないように、アプリ名はちゃんとしたものを設定しましょう（登録後も簡単に変更できますが）。
今回は、開発用 Web サーバーのローカルアドレス (`localhost`) の登録なので、アプリ名は適当に付けちゃって大丈夫です。
重要なのは __`Authorization callback URL`__ で、このアドレスが、一時コード取得時のリダイレクト先になります。

最後に `Register application` ボタンを押すと、次のように、クライアント ID とクライアントシークレットが発行されます。
これらの値を使って、一時コードやアクセストークンを取得することになります。

{{< image w="500" border="true" src="img-002.png" >}}

発行された値は、[登録済み OAuth アプリの一覧](https://github.com/settings/developers) からいつでも確認することができるので、特に保存しておく必要はありません。


Web アプリの実装
----

### (1) GitHub のログイン画面に遷移

まず、GitHub のログインサイトへのリンクを張ります。
認証後は GitHub からリダイレクトされて戻ってくるので、単純にページ遷移してしまえば OK です。
アドレス末尾のクエリ文字列として、前述の OAuth アプリ登録で発行されたクライアント ID (`client_id`) と、情報のスコープ (`scope`) を指定する必要があります。

{{< code lang="html" title="HTML" >}}
<a href="https://github.com/login/oauth/authorize?client_id=e971ec6b4a72cebc398c&scope=repo">Sign In</a>
{{< /code >}}

もちろん、JavaScript から次のようにページ遷移しても OK です。

{{< code lang="javascript" title="JavaScript" >}}
const CLIENT_ID = 'e971ec6b4a72cebc398c';
window.location.href = `https://github.com/login/oauth/authorize?client_id=${CLIENT_ID}&scope=repo`;
{{< /code >}}

{{< note title="クライアント ID は公開してよい？" >}}
クライアント ID は、GitHub に登録されたアプリの単なる識別子であり、秘密のキーではないので、上記のように HTML ファイルや JavaScript ファイルにハードコードしても問題ありません。
{{< /note >}}

### (2) 一時コードの取得

上記のリンクにアクセスすると、GitHub の認証（認可）画面が表示されます。
ユーザーが情報へのアクセスを許可すると、OAuth アプリ登録で指定しておいたコールバックアドレスにリダイレクトされます。
リダイレクトされた URL のクエリ文字列部分に一時コードが入ってくるので、これを `window.location.search` などから抽出すれば、一時コードの取得は完了です。

{{< code lang="javascript" title="JavaScript" >}}
const params = window.location.search;  //=> "?code=4f7b17572bee8cac9587"
const code = params.startsWith('?code=') ? params.split('=')[1] : undefined;
if (code) {
  alert('一時コード: ' + code);  //=> "4f7b17572bee8cac9587"
}
{{< /code >}}

URL に一時コードが含まれているときは、`code` 変数にその値が格納されます。
一時コードが含まれていないときは、`code` 変数の値は `undefined` になります。
実際の Web アプリの実装では、この `code` 変数の値が設定されている場合に次のアクセストークン取得のステップに進む、といった流れになると思います。

### (3) アクセストークンの取得

最後に、前のステップで取得した一時コード (`code`) を使ってアクセストークンを取得するのですが、クロスオリジン通信 (CORS) の制約があるため、残念ながら Web ブラウザ上の JavaScript からは、直接アクセストークンを取得することはできません。
ここでは、別途 Node.js のプログラムを作成し、そのプログラムを使ってアクセストークンを取得できることを確認してみます。

後述のプログラムでは、HTTP リクエストを発行するために `node-fetch` モジュールを使用しているので、先にインストールしておきます。
ちなみに、この `node-fetch` モジュールは、Web ブラウザの `fetch` 関数と同様の機能を Node.js アプリから使えるようにするためのものです。

{{< code >}}
$ npm install node-fetch
{{< /code >}}

下記のコードを実行すると、HTTP POST リクエストを発行してアクセストークンを取得します。

{{< code lang="javascript" title="get-token.js" >}}
const fetch = require('node-fetch');

// 下記の値は自分の OAuth アプリのものに変更してください
const URL = 'https://github.com/login/oauth/access_token';
const CLIENT_ID = 'e971ec6b4a72cebc398c';
const CLIENT_SECRET = 'fae0b936df633dc526ab754731b3015c6b14afcb';
const TEMP_CODE = '4f7b17572bee8cac9587';  // 一時コード

// HTTP リクエストのカスタマイズ
const fetchOption = {
  method: 'POST',
  headers: {
    Accept: 'application/json',
    'Content-Type': 'application/json',
  },
  body: JSON.stringify({
    client_id: CLIENT_ID,
    client_secret: CLIENT_SECRET,
    code: TEMP_CODE,
  })
};

// HTTP POST リクエストを送信
fetch(URL, fetchOption)
  .then(res => {
    if (!res.ok) {
      throw new Error(`${res.status} ${res.statusText}`);
    }
    return res.json();
  })
  // ここでレスポンスの JSON オブジェクトを出力
  .then(json => console.log(json))
  // エラーはまとめて処理
  .catch(err => console.error(err));
{{< /code >}}

コードをシンプルにするために、クライアント ID などをハードコードしているので、先頭部分の次の定数を適切な値に変更してから実行してください。

- {{< label "CLIENT_ID" >}} ... 登録した OAuth アプリのクライアント ID
- {{< label "CLIENT_SECRET" >}} ... 登録した OAuth アプリのクライアントシークレット
- {{< label "TEMP_CODE" >}} ... 前述のステップで取得した一時コード

特に、一時コード (`TEMP_CODE`) は発行後 10 分で期限切れになるので、素早くコード修正して実行してくださいｗ

アクセストークンの取得に成功すると、次のような結果が表示されます。

{{< code title="実行例" >}}
$ node get-token
{
  access_token: 'e08644f37543bf0722d9d79dcb64973cfdbcb9e4',
  token_type: 'bearer',
  scope: 'repo'
}
{{< /code >}}

この中の __`access_token`__ プロパティの値が、我々が望んでいた OAuth アクセストークンです。
GitHub API を実行するときは、このトークンを HTTP リクエストヘッダに指定して呼び出すことになります。


次のステップへ
----

これで、GitHub の OAuth アクセストークンの取得の流れがわかりました。
ただ今回は、最後のアクセストークンの発行で、ローカルの Node.js プログラムを使いました。
これでは静的な Web サイトにおいて自動でアクセストークンを取得することはできないので、実際にはアクセストークンを取得するための独自のバックエンドサーバーを用意して、Web サイトはそこ経由でアクセストークンを取得することになります。

その話はまた [次回](/p/ar2bjs2) 。。。

