---
title: "Cloudflare Workers をはじめる (wrangler)"
url: "p/rn7p7n5/"
date: "2022-12-11"
tags: ["Cloudflare"]
weight: 1
---

Workers とは？
----

[Cloudflare Workers](https://workers.cloudflare.com/) は、Cloudflare 社が提供しているサーバレスプラットフォームで、Web リクエストに応じたレスポンスを返すサービスを簡単に構築することができます。
Workers は世界中に配置されたエッジサーバーで実行されるため、クライアントからの要求に高速に応答することができます。
Azure Functions や AWS Lambda と同じようなサービスですが、それらに比べて Workers は、より高速かつ安価という特徴を持っています。
__毎日 10 万リクエストまで無料__ で使えるので、趣味用途であれば、無料枠だけでいろいろ試せます。

本記事の作業を進めるには、あらかじめ Cloudflare のアカウントを作成しておく必要があります。
下記から無料で作成できます。

- [Cloudflare Workers を開始する - Sign Up](https://dash.cloudflare.com/sign-up/workers)


Wrangler をインストールする
----

Worker の開発やデプロイには、__`wrangler`__ という公式のコマンドラインツールを使用します。
[Node.js](https://nodejs.org/ja/) の `npm` コマンドを使って、次のようにインストールできます。

{{< code lang="console" title="wrangler コマンドのインストール" >}}
$ npm install wrangler -g
{{< /code >}}


Cloudflont へのアクセスを許可する (wrangler login)
----

`wrangler` コマンドを実行できるようになったら、__`wrangler login`__ を実行して、Cloudflare のアカウントと関連づけます。

{{< code lang="console" title="Cloudflare へのサインイン" >}}
$ wrangler login
 ⛅️ wrangler 2.6.2
-------------------
Attempting to login via OAuth...
{{< /code >}}

Web ブラウザーが自動的に開いて、Wrangler から Cloudflare のデータにアクセスしてよいか尋ねられるので __`Allow`__ ボタンを押して許可します。
これで、`wrangler` の各種コマンドを実行できるようになります。
サインインした状態で __`wrangler whoami`__ コマンドを実行すると、どの Cloudflare ユーザーでサインインしているかを確認できます。

{{< code lang="console" title="現在のサインイン状態を確認" >}}
$ wrangler whoami
 ⛅️ wrangler 2.6.2
-------------------
Getting User settings...
👋 You are logged in with an OAuth Token, associated with the email 'XXXX@gmail.com'!
┌──────────────────────────┬──────────────────────────────────┐
│ Account Name             │ Account ID                       │
├──────────────────────────┼──────────────────────────────────┤
│ XXXX@gmail.com's Account │ a79fd54644844825ded71b264152dc7c │
└──────────────────────────┴──────────────────────────────────┘
🔓 Token Permissions: If scopes are missing, you may need to logout and re-login.
Scope (Access)
- account (read)
- user (read)
- workers (write)
- workers_kv (write)
- workers_routes (write)
- workers_scripts (write)
- workers_tail (read)
- d1 (write)
- pages (write)
- zone (read)
- offline_access
{{< /code >}}


Worker プロジェクトを作成する (wrangler init)
----

__wranger init <プロジェクト名>__ を実行すると、Worker プロジェクトのテンプレートコードを生成できます（いわゆる scaffold 処理です）。

{{< code lang="console" title="Worker プロジェクト (hello) を新規作成" >}}
$ wrangler init hello
 ⛅️ wrangler 2.6.2
-------------------
Using npm as package manager.
✨ Created hello/wrangler.toml
Would you like to use git to manage this Worker? (y/n)
✨ Initialized git repository at hello
No package.json found. Would you like to create one? (y/n)
✨ Created hello/package.json
Would you like to use TypeScript? (y/n)
✨ Created hello/tsconfig.json
Would you like to create a Worker at hello/src/index.ts?
  None
❯ Fetch handler
  Scheduled handler
{{< /code >}}

次のような質問をされますが、基本はすべて <kbd>Enter</kbd> でデフォルトのまま進めていけば OK です。

- Git リポジトリとして初期化するか？（デフォルト: Yes）
- package.json ファイルを作成するか？（デフォルト: Yes）
- TypeScript を使用するか？（デフォルト: Yes）
- どのタイプのハンドラーコードを生成するか？（デフォルト: Fetch handler）

プロジェクトの初期化が終了すると次のようなファイルが配置されます。

```
hello/
  +-- .git/
  +-- .gitignore
  +-- node_modules/
  +-- package-lock.json
  +-- package.json
  +-- src/index.tsx
  +-- tsconfig.json
  +-- wrangler.toml
```


開発用のローカルサーバーを起動する (wrangler dev)
----

`wrangler init` によって生成される `src/index.ts` ファイルは次のような感じの実装になっています。
HTTP リクエストを受けて `Hello World!` というレスポンスを返すだけですね。

{{< code lang="ts" title="src/index.ts" >}}
export default {
  async fetch(request: Request, env: Env, ctx: ExecutionContext): Promise<Response> {
    return new Response("Hello World!")
  }
}
{{< /code >}}

この Worker 実装をローカルサーバーでテストするには、__`wrangler dev`__ コマンドを実行します。
`package.json` で NPM スクリプトが定義されているので `npm start` でも OK です（`wrangler` コマンドをローカルインストールした場合は `npm start` を使います）。

{{< code lang="console" title="ローカルサーバーを起動" >}}
$ wrangler dev
 ⛅️ wrangler 2.6.2
-------------------
⬣ Listening at http://0.0.0.0:8787
- http://127.0.0.1:8787
- http://192.168.1.7:8787
Total Upload: 0.19 KiB / gzip: 0.16 KiB
╭────────────────────────────────────────────────────────────────────────────────────────────────╮
│ [b] open a browser, [d] open Devtools, [l] turn on local mode, [c] clear console, [x] to exit  │
╰────────────────────────────────────────────────────────────────────────────────────────────────╯
{{< /code >}}

デフォルトでは `http://localhost:8787` でアクセス可能なローカルサーバーが起動します。
コンソール上で <kbd>B</kbd> キーを入力すると、Web ブラウザでページを開くことができます。
次のように表示されれば成功です。

{{< image w="400" border="true" src="img-001.png" title="ローカルサーバーでの Worker のテスト" >}}


Cloudflare Workers へのデプロイ (wrangler publish)
----

ローカルサーバーでの動作確認が済んだら、__`wrangler publish`__ コマンドで Cloudflare Workers へデプロイできます。

{{< code lang="ts" title="Worker のデプロイ" >}}
$ wrangler publish
 ⛅️ wrangler 2.6.2
-------------------
Total Upload: 0.19 KiB / gzip: 0.16 KiB
Uploaded hello (0.61 sec)
Published hello (3.81 sec)
  https://hello.<ユーザー名>.workers.dev
Current Deployment ID: b7e28af5-f244-422b-ae64-00bd796e536e
{{< /code >}}

デプロイは一瞬で完了して、自動的に __`https://<プロジェクト名>.<ユーザー名>.workers.dev`__ という URL が割り当てられます。
デフォルトで公開状態になるので、Web ブラウザや `curl` コマンドでアクセスすれば、`Hello World!` というレスポンスが返ってきます。

```console
$ curl https://hello.XXXX.workers.dev
Hello World!
```

デプロイまでとても簡単にできました！
٩(๑❛ᴗ❛๑)۶ わーぃ

ちなみに、デプロイ先の Worker 名は __`--name`__ オプションで変更できます。

```console
$ wrangler publish --name hello-stg
```


あとかたづけ (wrangler delete)
----

Cloudflare Workers は呼び出しごとの従量課金なので、Worker をたくさん作って放置しておいても特にお金はかかりませんが、不要な Worker は削除しておいた方がよいでしょう。
Worker の削除には、__`wrangler delete`__ コマンドを使用します。

{{< code lang="console" title="Worker (hello) の削除" >}}
$ wrangler delete
 ⛅️ wrangler 2.6.2
-------------------
Are you sure you want to delete hello? This action cannot be undone. (y/n)

Successfully deleted hello
{{< /code >}}

スッキリスッキリ ( ⁎ᵕᴗᵕ⁎ )

