---
title: "Apollo Client で GitHub GraphQL API を使う (Node & React)"
url: "/p/qcp2cnx"
date: "2020-07-31"
tags: ["Apollo", "GitHub", "GraphQL", "React"]
weight: 200
---

Apollo Client とは
----

Apollo パッケージは、GraphQL を使ったクライアントアプリやサーバーを作成するためのライブラリ群です。
クライアントアプリを作るためのライブラリは、__Apollo Client__ として __`@apollo/client`__ という NPM パッケージにまとめられています。

Web アプリのコンポーネントを作成するときは React がよく使われますが、Apollo は GraphQL を扱いやすくする React コンポーネント（`ApolloProvider`、`Query`、`Mutation`、`Subscription`）や React Hook 拡張（`useQuery`) などを提供しています。

ここでは、Apollo Client パッケージを使用して、

- Node.js アプリ（コマンドラインアプリの JS）から GraphQL API の実行
- React アプリ（Web サイトの JS）から GraphQL API の実行

を行ってみます。
呼び出す GraphQL API は何でもよいのですが、今回は __GitHub GraphQL API__ を利用することにします。


Node パッケージのインストール
----

### Apollo Client

Apollo Client 関連のパッケージとしては、__`@apollo/client`__ と、それが使用する __`graphql`__ をインストールします。

{{< code title="Apollo Client のインストール" >}}
### yarn の場合
$ yarn add @apollo/client graphql

### npm の場合
$ npm install @apollo/client graphql
{{< /code >}}

### fetch ポリフィル

Apollo クライアント内部の実装では、Web ブラウザの fetch API を利用しています。
React アプリから Apollo クライアントを利用する場合は問題ないのですが、コンソールで動作する Node.js アプリから利用する場合は、fetch API の埋め合わせ (polyfill) をするモジュールが必要になります。
ここでは、__`cross-fetch`__ パッケージをインストールしておきます。

{{< code title="fetch ライブラリのインストール" >}}
### yarn の場合
$ yarn add cross-fetch

### npm の場合
$ npm install cross-fetch
{{< /code >}}

### （コラム）古いパッケージ

Apollo のクライアント系パッケージはいろいろな名前で提供されていましたが、すべて `@apollo/client` 以下にまとめられました。
次のようなパッケージはもう使用しません。

- {{< label "apollo-client:" >}} すべて `@apollo/client` に統合されたので使いません
- {{< label "apollo-boost:" >}} 次のように `ApolloClient` コンストラクタを使えばOKです (`import {ApolloClient} from '@apollo/client/core'`)
- {{< label "graphql-tag:" >}} GraphQL をパースするための `gql` も `@apollo/client` に統合されています (`import {gql} from '@apollo/client'`)


GitHub アクセストークンの発行 {#token}
----

GitHub の GraphQL API を使用するには、下記のように POST リクエストのヘッダで GitHub アクセストークンを付加する必要があります。

{{< code >}}
authorization: Bearer a4304a13bc6cdd52509c90a38a676fce962ce518
{{< /code >}}

アクセストークンを付加しないと、API 実行時に次のような HTTP 401 エラーが返されます。

{{< code >}}
ApolloError: Response not successful: Received status code 401
{{< /code >}}

GitHub のアクセストークンは、下記の設定画面から __`Generate new token`__ ボタンを押すことで作成できます。

- [GitHub / Settings / Developer settings / Personal Access Tokens](https://github.com/settings/tokens)

このとき、公開するデータのスコープ設定を行うのですが、何もチェックしなくても Public なリポジトリ情報やユーザー情報は取得可能です。
__`repo`__ にチェックを入れると、Private なリポジトリの読み書きが可能になり、__`user`__ にチェックを入れると、ユーザー情報の読み書きが可能になります。
__`read:user`__ にだけチェックを入れると、ユーザー情報の読み込みだけが可能になります。
残念ながら、Private リポジトリをリードオンリーで扱うスコープは存在しないようです（設計ミス？）。

スコープの詳細については、下記の GitHub ドキュメントを参考にしてください。

- 参考: [Scopes for OAuth Apps - GitHub Docs](https://docs.github.com/en/developers/apps/scopes-for-oauth-apps)


Node.js アプリから Apollo Client を使用する
----

まずは、Node.js を使ったコマンドラインアプリから、__`ApolloClient`__ を使って GitHub GraphQL API を呼び出してみます。
下記のサンプルコードでは、指定した GitHub リポジトリ (`apollographql/apollo`) の最新 Issue 5件分を取得しています。
このコードを実行できるようになれば、あとはクエリ部分を変更することでいろいろな情報を取得できます。

ポイントとしては、下記のあたりでしょうか。

* `cross-fetch/polyfill` モジュールをインポートして、Web ブラウザの `fetch` 関数をエミュレートする
* `authorization` ヘッダで GitHub のアクセストークンを指定する
* GraphQL クエリを ``gql`クエリ文字列` `` という形で定義する

### サンプルコード

{{< code lang="typescript" title="main.ts" >}}
import {ApolloClient, ApolloError, InMemoryCache, gql} from '@apollo/client/core';
import 'cross-fetch/polyfill';  // グローバルな fetch 関数を定義する

// トークンは環境変数などから取得するのが常套手段
// const token = 'a4304a13bc6cdd52509c90a38a676fce962ce518';
const token = process.env.MYAPP_GITHUB_TOKEN;
if (typeof token === 'undefined') {
    throw new Error('MYAPP_GITHUB_TOKEN cannot be found');
}

// GraphQL クライアントを生成
const apolloClient = new ApolloClient({
  uri: 'https://api.github.com/graphql',
  headers: {authorization: `Bearer ${token}`},
  cache: new InMemoryCache(),
});

// 発行する GraphQL クエリ
const searchQuery = gql`
  query {
    search(query: "repo:apollographql/apollo is:issue", type: ISSUE, first: 5) {
      issueCount
      nodes {
        ... on Issue { number title }
      }
    }
  }
`;

// クエリを発行
apolloClient.query({query: searchQuery})
  .then(result => handleApolloResult(result.data))
  .catch(handleApolloError);

// GraphQL レスポンスをハンドル
function handleApolloResult(data: any) {
  const {issueCount, nodes} = data.search;
  console.log(`Num of issues: ${issueCount}`);
  for (const issue of nodes) {
    console.log(`* ${issue.number}: ${issue.title}`);
  }
}

// GraphQL のエラーをハンドル
function handleApolloError(err: ApolloError) {
  console.error(err.message);
}
{{< /code >}}

### 実行結果

{{< code >}}
Num of issues: 176
* 937: Broken code display in blog for scaling GraphQL
* 934: Unknown directives
* 929: start apollo gateway
* 926: using REST services in apolo federation
* 909: Get a Graph Manager API key instructions need updating
{{< /code >}}


React アプリから Apollo Client を使用する
----

次に、React アプリから Apollo Client を使って GraphQL API を実行してみます。
Apollo Client はもともと React アプリから使用することを想定して作られているため、とてもきれいに実装できます。
GitHub のアクセストークンは相変わらず必要なので、[上記の説明](#token) を参考にして取得しておいてください。

下記の例では、GitHub のアクセストークンを TypeScript コードに埋め込んでいますが、本番用の Web アプリでは、OAuth などの仕組みを使って動的にユーザーのアクセストークンを取得する必要があります。
OAuth 関連の実装を説明すると長くなってしまうので、そのあたりは下記の記事を参考にしてください。

- [GitHub OAuth トークンを取得する (1) 処理の流れを理解する](/p/ubkt3ai)
- [GitHub OAuth トークンを取得する (2) Azure Functions 経由で取得する](/p/ar2bjs2)

### サンプルコード

次の `App` コンポーネントでは、前述のコマンドラインアプリと同様に __`ApolloClient`__ インスタンスを生成しています。
このインスタンスを、__`ApolloProvider`__ コンポーネントの __`client`__ プロパティにセットすると、下位のコンポーネントから簡単に GraphQL API を呼び出せるようになります（後述）。

{{< code lang="jsx" title="App.tsx" >}}
import * as React from 'react';
import {ApolloClient, ApolloProvider, InMemoryCache} from '@apollo/client';
import {Issues} from './Issues';

// 注意: 本番環境ではアクセストークンは OAuth などで動的に取得すること
const GITHUB_TOKEN = 'a4304a13bc6cdd52509c90a38a676fce962ce518';

// GraphQL クライアントを生成
const apolloClient = new ApolloClient({
  uri: 'https://api.github.com/graphql',
  headers: {authorization: `Bearer ${GITHUB_TOKEN}`},
  cache: new InMemoryCache(),
});

export const App: React.FC = () => {
  return (
    <ApolloProvider client={apolloClient}>
      <Issues />
    </ApolloProvider>
  );
};
{{< /code >}}

次の `Issues` コンポーネントは、Apollo Client を使って GitHub GraphQL API を呼び出し、指定したリポジトリの最新 Issue 5件分を取得して表示します（前述のコマンドラインアプリと同様です）。
クエリの実行方法はとても簡単で、Apollo Client が提供する __`useQuery`__ フックを呼び出すだけです。
クエリの実行結果により、ロード中 (`loading`)、エラー発生 (`error`)、取得完了 (`data`) の戻り値を得られるので、現在の状況に応じて描画内容を切り替えることができます。

{{< code lang="jsx" title="Issues.tsx" >}}
import * as React from 'react';
import {gql, useQuery} from '@apollo/client';

// 発行する GraphQL クエリ
const GET_ISSUES = gql`
  query {
    search(query: "repo:apollographql/apollo is:issue", type: ISSUE, first: 5) {
      issueCount
      nodes {
        ... on Issue { number title }
      }
    }
  }
`;

export const Issues: React.FC = () => {
  // GraphQL のクエリを実行
  const {loading, error, data} = useQuery(GET_ISSUES);

  // クエリ実行中の表示
  if (loading) return <p>Loading ...</p>;

  // エラー発生時（レスポンスがないとき）の表示
  if (error) return <p style={{color: 'red'}}>{error.message}</p>;

  // クエリの結果が返ってきたときの表示
  const {issueCount, nodes: issues} = data.search;
  return <>
    <h2>Num of issues: {issueCount}</h2>
    <ul>
      { issues.map(i => <li key={i.number}>{i.number} - {i.title}</li>) }
    </ul>
  </>;
};
{{< /code >}}

### 実行結果

{{< image border="true" w="600" src="img-001.png" title="クエリ実行中" >}}

{{< image border="true" w="600" src="img-002.png" title="結果の取得後" >}}

ここでは、クエリ実行中に「Loading ...」と表示するだけにしていますが、ロード中のくるくるアイコンなどを表示すると、かなりそれっぽくなります。


ローカルストレージに保存したアクセストークンを使用する
----

上記の例では、GitHub のアクセストークンをハードコードしていますが、実運用ではこのようなコードはデプロイできません。
ここでは、ローカルストーレージ (`GITHUB_TOKEN`) に保存されたアクセストークンを使って、`ApolloClient` および `ApolloProvider` を生成する方法を紹介します。

OAuth によってアクセストークンを取得し、ローカルストレージに保存するまでの流れは次の記事を参考にしてください。

- [GitHub OAuth トークンを取得する (2) Azure Functions 経由で取得する](/p/ar2bjs2)

次のサンプルコンポーネント `GitHubApolloProvider` は、GitHub のアクセストークンを HTTP ヘッダに付けて GraphQL クエリを発行するための `ApolloProvider` を提供します。
このような `ApolloClient` のカスタマイズ方法は、Apollo 本家サイトの [Authentication のページ](https://www.apollographql.com/docs/react/networking/authentication/#header) で説明されています。

{{< code lang="jsx" title="GitHubApolloProvider.tsx" >}}
import * as React from 'react';
import {
  ApolloClient,
  ApolloProvider,
  createHttpLink,
  InMemoryCache,
} from '@apollo/client';
import { setContext } from '@apollo/client/link/context';

const httpLink = createHttpLink({
  uri: 'https://api.github.com/graphql'
});

const authLink = setContext((_, { headers }) => {
  // Get the authentication token from local storage if it exists
  const token = localStorage.getItem('GITHUB_TOKEN');
  // Return the headers to the context so httpLink can read them
  return {
    headers: {
      ...headers,
      authorization: token ? `Bearer ${token}` : '',
    }
  }
});

// Create a GraphQL client
const apolloClient = new ApolloClient({
  link: authLink.concat(httpLink),
  cache: new InMemoryCache()
});

export const GitHubApolloProvider: React.FC = (prop) => {
  return <ApolloProvider client={apolloClient}>
    {prop.children}
  </ApolloProvider>;
};
{{< /code >}}

あとは、`ApolloProvider` を使っていたところを次のように `GitHubApolloProvider` に置き換えれば、それ以下のコンポーネントでアクセストークン付きの GraphQL クエリを発行できるようなります。

{{< code lang="jsx" title="App.tsx" >}}
import * as React from 'react';
import {GitHubApolloProvider} from './GitHubApolloProvider';
import {Issues} from './Issues';

export const App: React.FC = () => {
  return (
    <GitHubApolloProvider>
      <Issues />
    </GitHubApolloProvider>
  );
};
{{< /code >}}

ローカルストレージに保存されたアクセストークンは、GraphQL のリクエストを実行するたびに参照してくれるので、`ApolloClient` インスタンスを生成した後でアクセストークンが設定された場合にもうまく動作します。

- 参考: [Apollo Client の ApolloLink チェーンで HTTP リクエストをカスタマイズする](/p/xa62yo4/)

### 環境変数に設定したアクセストークンを使用する

ローカル環境での開発時に、何らかの理由で、環境変数に保存した GitHub アクセストークン ([Personal access token](https://github.com/settings/tokens)) を使用したい場合は、`index.tsx` の最初の方で、次のような感じで環境変数の値をローカルストレージにコピーしてしまえば OK です。

{{< code lang="typescript" title="index.tsx" >}}
if (process.env.NODE_ENV === 'development' && process.env.MYAPP_GITHUB_TOKEN) {
  console.warn('In development mode, use MYAPP_GITHUB_TOKEN env variable.');
  localStorage.setItem('GITHUB_TOKEN', process.env.MYAPP_GITHUB_TOKEN);
}
{{< /code >}}


おまけ： GraphQL クエリにパラメータを設定する
----

パラメータ付きの GraphQL クエリを実行するには、[useQuery フック](https://www.apollographql.com/docs/react/api/react/hooks/#usequery) の第2パラメータで、次のような形で変数オブジェクト（変数名と値の連想配列）を渡します。

{{< code lang="typescript" >}}
const {loading, error, data} = useQuery(QUERY, {variables: 変数オブジェクト});
{{< /code >}}

下記のサンプルコードでは、変数で指定した GitHub リポジトリのイシューを取得しています。

{{< code lang="jsx" title="Issues.tsx" >}}
import * as React from 'react';
import {gql, useQuery} from '@apollo/client';

const QUERY = gql`
  query($searchQuery: String!) {
    search(query: $searchQuery, type: ISSUE, first: 5) {
      nodes {
        ... on Issue { number title }
      }
    }
  }
`;

export const Issues: React.FC = () => {
  // 変数を渡して GraphQL クエリを実行
  const queryVars = {
    searchQuery: 'repo:apollographql/apollo is:issue'
  };
  const {loading, error, data} = useQuery(QUERY, {variables: queryVars});

  if (loading) return <p>Loading ...</p>;
  if (error) return <p style={{color: 'red'}}>{error.message}</p>;
  const {nodes} = data.search;

  return <>
    <ul>
      {nodes.map(x =>
        <li key={x.number}>{x.number} - {x.title}</li>
      )}
    </ul>
  </>;
};
{{< /code >}}

