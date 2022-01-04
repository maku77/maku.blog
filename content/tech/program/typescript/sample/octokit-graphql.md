---
title: "Node.js で GitHub GraphQL API を使用する (@octokit/graphql)"
url: "/p/h5s6ht5"
date: "2020-09-10"
tags: ["GitHub", "GraphQL", "TypeScript"]
---

概要
----

GitHub 上の情報を扱う API として、GitHub は GraphQL API を提供しています。

- [GitHub GraphQL API v4 | GitHub Developer Guide](https://developer.github.com/v4/)

Node.js で GraphQL を扱う方法としては、[Apollo ライブラリを使った方法](/p/qcp2cnx) などがありますが、GitHub の GraphQL API を呼び出したいのであれば、GitHub が提供している GraphQL パッケージ __`@octokit/graphql`__ を使うのが手っ取り早いかもしれません（エンドポイントの URL などを省略できます）。

- [octokit/graphql.js: GitHub GraphQL API client for browsers and Node](https://github.com/octokit/graphql.js/)
- [@octokit/graphql - npm](https://www.npmjs.com/package/@octokit/graphql)

ここでは、TypeScript からこの `@octokit/graphql` パッケージを使用する方法を紹介します。

{{< note title="REST API と GraphQL API" >}}
GitHub API バージョン 3 は REST API でしたが、[柔軟性などの観点](https://github.blog/2016-09-14-the-github-graphql-api/) から、GitHub API バージョン 4 の GraphQL 版の API を使用することが推奨されています。
GraphQL API を使用することで、REST API で複数のリクエストが必要だったものを 1 度のリクエストで取得できたりします。
また、GraphQL API でしか取得できない情報もあったりします。
{{< /note >}}


セットアップ
----

まず、TypeScript のプロジェクトを作成します。

- 参考：[Visual Studio Code で TypeScript の開発環境を構築する](/p/ak7u3h3)

`@octokit/graphql` パッケージは次のようにインストールします。
このパッケージには TypeScript の型情報も含まれています。
あと、環境変数設定用のライブラリとして、`dotenv` もインストールしておきます。

{{< code >}}
$ npm install @octokit/graphql --save
$ npm install dotenv --save
{{< /code >}}

GitHub のアクセストークンが必要になるので、GitHub の設定画面からパーソナルアクセストークンを取得しておいてください。

- [Settings / Developer Settings / Personal access tokens](https://github.com/settings/tokens)

アクセストークンは、環境変数 __`MYAPP_GITHUB_TOKEN`__ か、プロジェクトルートの __`.env`__ ファイルに次のように保存しておきます。

{{< code title=".env" >}}
MYAPP_GITHUB_TOKEN=ccdccab3363b3554ebadc033fa8fe43403705301
{{< /code >}}

サンプルコード
----

ここでは、次のような方針で、`@octkit/graphql` モジュールを使ったコードを作成してみます。

- パーソナルアクセストークンは環境変数 `MYAPP_GITHUB_TOKEN`（あるいは `.env` ファイル）で設定する
- 上記トークンを使って GraphQL リクエストを送る `graphql` モジュールを作成し、それをメインプログラムから使用する

次の `graphql` モジュールは、アクセストークン付きの GraphQL リクエストを送るためのセットアップを行います。
この処理をメインプログラムから切り出しておくことで、メインプログラム側のコードをスッキリさせることができます。

{{< code lang="typescript" title="graphql.ts" >}}
import { graphql } from '@octokit/graphql';
import * as dotenv from 'dotenv';

// .env ファイルの内容を環境変数に反映
dotenv.config();

// graphql をアクセストークン付きで呼び出せるようにする
const graphqlWithAuth = graphql.defaults({
  headers: {
    authorization: `token ${process.env.MYAPP_GITHUB_TOKEN}`,
  },
});

export { graphqlWithAuth as graphql };
{{< /code >}}

メインプログラムからは次のように GraphQL リクエストを実行します。
ここでは、GitHub の `octokit/graphql.js` リポジトリの最新の Issue 情報を取得しています。

{{< code lang="typescript" title="main.ts" >}}
import { graphql } from './graphql';

const QUERY = `
  {
    repository(owner: "octokit", name: "graphql.js") {
      issues(last: 3, states: [OPEN]) {
        edges {
          node {
            number
            title
          }
        }
      }
    }
  }
`;

async function main() {
  try {
    const {repository} = await graphql(QUERY);
    for (const {node: issue} of repository.issues.edges) {
      console.log(`* ${issue.number}: ${issue.title}`);
    }
  } catch (err) {
    console.error(err.message);
  }
}

main();
{{< /code >}}

メインプログラムは GraphQL の呼び出しだけなので、とてもシンプルですね！
これで、GitHub GraphQL API を使った簡単なコマンドラインツールはサクッと作れると思います。

{{< code title="実行結果" >}}
* 78: Can't load client in Cloudflare workers
* 176: Can't use as Octokit with TypeScript
* 185: Setting baseUrl for GHES results in 406 when using with @octokit/auth-app
{{< /code >}}


（応用）GraphQL の変数を使う
----

変数を使った GraphQL クエリも簡単に扱うことができます。
変数の値は、`graphql` 関数の第 2 パラメータで指定します。

{{< code lang="typescript" title="main.ts" >}}
import { graphql } from './graphql';

const QUERY = `
  query getUser($login: String!) {
    user(login: $login) {
      login
      name
      url
      websiteUrl
      avatarUrl
    }
  }
`;

async function main() {
  try {
    const {user} = await graphql(QUERY, {login: 'maku77'});
    console.log(user);
  } catch (err) {
    console.error(err.message);
  }
}

main();
{{< /code >}}

{{< code title="実行結果" >}}
{
  login: 'maku77',
  name: 'Masatoshi Ohta',
  url: 'https://github.com/maku77',
  websiteUrl: 'https://maku77.github.io/',
  avatarUrl: 'https://avatars2.githubusercontent.com/u/5519503?v=4'
}
{{< /code >}}


（応用）ページネーションで 100 件を超えるデータを取得する
----

GitHub GraphQL API で何らかのリスト情報を取得する場合（Issue リストなど）、一度に取得できるデータ数は 100 件までに制限されています。
それより多くのデータを取得したい場合は、ページネーションによって繰り返しリクエストを送る必要があります。
次のページが存在するかどうかは __`pageInfo.hasNextPage`__、次のページの開始位置は __`pageInfo.endCursor`__ で取得することができます。

{{< code lang="typescript" title="main.ts" >}}
import { graphql } from './graphql';

const QUERY = `
  query getIssues($after: String) {
    repository(owner: "octokit", name: "graphql.js") {
      issues(first: 100, after: $after) {
        edges {
          node {
            number
            title
          }
        }
        pageInfo {
          endCursor
          hasNextPage
        }
      }
    }
  }
`;

async function main() {
  try {
    // ページネーション情報
    let hasNextPage = true;
    let endCursor: string | null = null;

    // 次のページがあれば GraphQL 呼び出し
    while (hasNextPage) {
      const {repository: {issues}} = await graphql(QUERY, {after: endCursor});

      // 現在のページの Issue 情報を出力
      for (const {node} of issues.edges) {
        console.log(`* ${node.number}: ${node.title}`);
      }

      // ページネーション情報を更新
      hasNextPage = issues.pageInfo.hasNextPage;
      endCursor = issues.pageInfo.endCursor;
    }
  } catch (err) {
    console.error(err.message);
  }
}

main();
{{< /code >}}


（応用）プロキシ経由で接続する
----

社内などのプロキシ環境から `@octokit/graphql` を使用するには、`graphql.defaults` 関数に渡すオプションオブジェクトの __`request.agent`__ プロパティを指定します。
この値には、`https-proxy-agent` モジュールが提供する __`HttpProxyAgent`__ オブジェクトを設定できます。

{{< code title="https-proxy-agent のインストール" >}}
$ npm install https-proxy-agent --save
{{< /code >}}

{{< code lang="typescript" >}}
const PROXY = 'https://proxy.example.com:8080';

const graphqlWithAuth = graphql.defaults({
  headers: {
    authorization: `token ${process.env.MYAPP_GITHUB_TOKEN}`,
  },
  request: {
    agent: new HttpsProxyAgent(PROXY);
  }
});
{{< /code >}}

下記は、環境変数 __`https_proxy`__ のプロキシ設定を GraphQL API の呼び出しに使用する例です。

{{< code lang="typescript" title="graphql.ts" >}}
import * as dotenv from 'dotenv';
import { HttpsProxyAgent } from 'https-proxy-agent';
import { graphql } from '@octokit/graphql';

// .env ファイルの内容を環境変数に反映
dotenv.config();

// graphql をアクセストークン付きで呼び出せるようにする
const PROXY = process.env.https_proxy;
const graphqlWithAuth = graphql.defaults({
    headers: {
        authorization: `token ${process.env.MYAPP_GITHUB_TOKEN}`,
    },
    request: {
        agent: PROXY ? new HttpsProxyAgent(PROXY) : undefined
    }
});

export { graphqlWithAuth as graphql };
{{< /code >}}

