---
title: "Node.js で GitHub REST API を使用する (@octokit/rest)"
url: "/p/7r6gr3d"
date: "2020-06-28"
lastmod: "2020-06-29"
tags: ["GitHub", "TypeScript"]
---

概要
----

[Octokit](https://developer.github.com/v3/libraries/) は、様々な言語から GitHub API を使用するためのライブラリを提供しています。
ここでは、TypeScript (JavaScript) 用の GitHub API v3 (REST API) ライブラリである、[@octokit/rest](https://github.com/octokit/rest.js) を使用する方法を紹介します。


インストール
----

`@octokit/rest` は次のようにインストールします（`npm init` で `package.json` を作成済みだと想定します）。

{{< code >}}
$ npm install @octokit/rest --save
{{< /code >}}


実装（基本）
----

基本的には下記の API ドキュメントを参照しながら実装していくことになります。

- [octokit/rest.js API ドキュメント](https://octokit.github.io/rest.js/)

### Octokit インスタンスを生成する

{{< code lang="typescript" title="main.ts" >}}
import { Octokit } from '@octokit/rest';
const octokit = new Octokit();
{{< /code >}}

このように生成した Octokit インスタンスを使って、様々な REST API を呼び出します。

### リポジトリの一覧を取得する (repos.listForOrg)

[API ドキュメント (Repos - Get a repository)](https://octokit.github.io/rest.js/v18#repos-get)

次の例では、組織名 `sony` のパブリックなリポジトリを 5 件分取得しています。

{{< code lang="typescript" >}}
octokit
  .repos.listForOrg({
    org: 'sony',        // 取得対象とする組織 (organization)
    type: 'public',     // public なリポジトリのみ取得
    sort: 'full_name',  // レスポンスの data 配列を full_name でソート
    per_page: 5,        // 1リクエストごとのデータ数（デフォルト:30、最大:100）
  })
  .then(response => {
    console.log(JSON.stringify(response, null, 2));
  });
{{< /code >}}

出力結果は長いので、[こちらにテキストファイル](result-list-for-org.txt) として置いておきます。

おそらく、JSON レスポンス内で実際に必要な情報は `data` プロパティの配列データなので、下記のように `then` コールバックのパラメータで `data` 配列の値を取り出すとコードがスッキリします。

{{< code lang="typescript" >}}
  .then(({ data }) => {
    for (const repo of data) {
      console.log(`${repo.full_name} - ${repo.description}`);
    }
  });
{{< /code >}}

{{< code title="実行結果" >}}
sony/ai-research-code - null
sony/appsync-client-go - AWS AppSync golang client library
sony/cdn-purge-control-php - Multi CDN purge control library for PHP
sony/cdp-cli - Command line tools for generating start point of ...
sony/cdp-js - Libraries/SDK modules for multi-platform ...
{{< /code >}}


### 指定したリポジトリの Issue 一覧を取得する (issues.listForRepo)

[API ドキュメント (Issues - List repository issues)](https://octokit.github.io/rest.js/v18#issues-list-for-repo)

次の例では、`octokit/rest.js` リポジトリのオープン状態の Issue を 5 件分取得しています。

{{< code lang="typescript" >}}
octokit
  .issues.listForRepo({
    owner: 'octokit',  // 取得対象とする組織 (organization)
    repo: 'rest.js',   // 取得対象のリポジトリ名
    state: 'open',     // オープン状態のIssueだけ取得
    per_page: 5,       // 1リクエストごとのデータ数（デフォルト:30、最大:100）
  })
  .then(response => {
    for (const issue of response.data) {
      console.log(`* ${issue.number}: ${issue.title}`);
    }
  })
  .catch(err => console.error(err));
{{< /code >}}

{{< code title="実行結果" >}}
* 1775: octokit.repos.getCommunityProfileMetrics() does not set ...
* 1773: Documentation: Broken link to `client-options`
* 1757: Error thrown for a not-found user (404) should have more details
* 1738: getSelfHostedRunner status is always online never active
* 1725: 422 on PUT /repos/:owner/:repo/branches/:branch/protection
{{< /code >}}


実装（応用）
----

### ページネーションの自動化 (octokit.pagenate)

`listXXX` 系の API では、一度に最大 100 件（`per_page: 100` 指定時）までの情報しか取得できません。
これは、GitHub REST API の制約であり、それ以上のデータを取得するには、__ページネーションの仕組みを利用して複数回に分けてリクエストを送る必要__ があります。

- [GitHub’s REST API pagination documentation](https://developer.github.com/v3/#pagination)

Octokit クラスは [pagenate メソッド](https://octokit.github.io/rest.js/v18#pagination) を提供しており、連続する REST API 呼び出しを自動化してくれます。
次の例では、`octokit/rest.js` リポジトリの全オープン Issue を取得しています（100件を超えていても一度に取得できます）。

{{< code lang="typescript" title="main.ts" >}}
import { Octokit } from '@octokit/rest';

const octokit = new Octokit();

octokit
  .paginate('GET /repos/:owner/:repo/issues', {
    owner: 'octokit',  // 取得対象とする組織 (organization)
    repo: 'rest.js',   // 取得対象のリポジトリ名
    state: 'open',     // オープン状態のIssueだけ取得
    per_page: 100,     // 1ページごとの件数は最大値にしておく
  })
  .then(issues => {
    for (const issue of issues) {
      console.log(`* ${issue.number} - ${issue.title}`);
    }
  })
  .catch(err => console.error(err));
{{< /code >}}

### Private リポジトリの情報を取得する（アクセストークン）

Private リポジトリの Issue リストなどを取得する場合は、OAuth トークンやパーソナル・アクセス・トークンを指定して GitHub API を呼び出す必要があります（権限がない場合の呼び出しは HTTP 404 エラーになります）。

例えば、パーソナル・アクセス・トークンは、GitHub の下記のユーザー設定画面から発行することができます。

- [GitHub / Settings / Personal Access Tokens](https://github.com/settings/tokens)

トークン生成時に権限のスコープを指定する必要があるのですが、Issue リストなどを取得するのであれば、__repo__ にチェックを入れておけばよいはずです。

取得したトークン（40文字の文字列）は、次のように `Octokit` クラスのコンストラクタで渡してやります。

{{< code lang="typescript" >}}
import { Octokit } from '@octokit/rest';

const octokit = new Octokit({
  auth: 'a0709c8d0ac21812d9c4b8511298b33ec0fd2813'
});
{{< /code >}}

これで、このインスタンスを使った GitHub API 呼び出しが、トークン付きで実行されるようになります。

