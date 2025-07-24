---
title: "Cloudflare Workers と Hono で軽量な Web API を作る"
url: "p/33cc7jy/"
date: "2025-01-11"
tags: ["Cloudflare", "Hono"]
---

- 参考: [Cloudflare Workers をはじめる (wrangler)](/p/rn7p7n5/)

Hono とは
----

[Hono](https://hono.dev/) は、Cloudflare Workers で使える Web アプリ用のフレームワークで、軽量な Web API を実装するときに便利です。
Node.js の Express に似た API を提供しており、ルーティングやミドルウェアを少ないコードで実装することができます。
[Cloudflare Workers の Runtime API](https://developers.cloudflare.com/workers/runtime-apis/) だけでも Web API を実装できますが、Hono を使うとよりシンプルなコードで実装できます。

{{% note title="Cloudflare Workers ≠ Node.js" %}}
JavaScript のランタイム環境としては Node.js が有名ですが、Cloudflare Workers は V8 エンジンをベースとした **独自の Workers ランタイム環境** です。
ローカルでの開発中には Node.js のツール群を使ったりするので余計に混乱しますが、デプロイするコードは Cloudflare Workers 上で動作することを意識して実装する必要があります。
具体的には、[Node.js の Runtime API](https://nodejs.org/docs/latest/api/) は使えず、Cloudflare Workers の Runtime API を使う必要があります（参考: [Workers - Node.js compatibility](https://developers.cloudflare.com/workers/runtime-apis/nodejs/)）。
Hono はもともと Cloudflare Workers 上で動作させることを考えて作られているので安心して使えます。
Web 標準機能を使った実装をウリにしており、Deno Deploy、AWS Lambda など他の環境でも動かすことができます。
{{% /note %}}


Hono で Hello World
----

### Hono プロジェクトの作成

Hono 用のプロジェクトは **`npm create`** で作れるようになっているので、基本的にはこれを使ってサクッと作ります（参考: [Hono - Getting Started](https://hono.dev/docs/getting-started/basic)）。

{{< code lang="console" title="Hono プロジェクトの作成" >}}
$ npm create hono@latest my-hono-app   # npm の場合
$ pnpm create hono@latest my-hono-app  # pnpm の場合
{{< /code >}}

ウィザード形式で想定環境を聞かれるので、Cloudflare Workers で動かしたい場合は次のような感じで答えていきます。

- Which template do you want to use? **`cloudflare-workers`**
- Do you want to install project dependencies? **`yes`**
- Which package manager do you want to use? **`pnpm`**

Cloudflare Workers 用の `wrangler` コマンドの設定ファイル (`wrangler.toml`) も自動生成されます。

{{< code lang="console" hl_lines="9" >}}
$ tree -L 1
.
├── README.md
├── node_modules
├── package.json
├── pnpm-lock.yaml
├── src
├── tsconfig.json
└── wrangler.toml
{{< /code >}}

### 開発サーバーで動作確認

プロジェクトを作成したら、開発用サーバーを立ち上げて動作確認します（内部的に `wrangler dev` が実行されます）。

{{< code lang="console" title="開発用サーバーの起動" >}}
$ npm run dev  # npm の場合
$ pnpm dev     # pnpm の場合
{{< /code >}}

続けて <kbd>b</kbd> キーを押すと、Web ブラウザが開きます。
`Hello Hono!` と表示されれば成功です。

### Cloudflare Workers へデプロイ

動作確認が終わったら Cloudflare Workers にデプロイしてみます（内部的に `wrangler deploy --minify` が実行されます）。

{{< code lang="console" title="デプロイ" >}}
$ npm run deploy   # npm の場合
$ pnpm run deploy  # pnpm の場合（`pnpm deploy` じゃないので注意）
{{< /code >}}

自動的に **`https://my-hono-app.<ACCOUNT>.workers.dev`** といった URL が発行されるので、Web ブラウザでアクセスできれば成功です。
Cloudflare の[ダッシュボード](https://dash.cloudflare.com/) を開くと、デプロイされていることを確認できます。
必要なくなったら `wrangler delete` でサクッと削除できます。


実装例
----

{{< code lang="ts" title="src/index.ts" >}}
import { Hono } from "hono";

const app = new Hono();

app.get("/", (c) => {
  return c.text("Hello Hono!");
});

export default app;
{{< /code >}}

上記は自動生成された最小限の実装です。
あとはこのコードをベースにして、Hono のドキュメント（例: [Examples](https://hono.dev/examples/) / [API](https://hono.dev/docs/api/hono/) / [Guides](https://hono.dev/docs/guides/middleware/)）を参考にして少しずつ機能を追加していけば OK です。
以下にいくつか実装例を抜粋しておきます。

### テキスト／JSONデータを返す

```ts
app.get("/text", (c) => {
  // 自動でレスポンスヘッダー `Content-Type: text/plain` が付与される
  return c.text("Hello");
});

app.get("/json", (c) => {
  // 自動でレスポンスヘッダー `Content-Type: application/json` が付与される
  return c.json({ ok: true, message: "Hello" });
});
```

- 参考: https://hono.dev/docs/api/context

### パスパラメーター／クエリパラメーターを取得

```ts
// Path params
app.get("/books/:id", (c) => {
  const id = c.req.param("id");
  return c.text(`Book-${id}`);
});

// Query params
app.get("/search", async (c) => {
  const query = c.req.query("q");
});

// Get all params at once
app.get("/search", async (c) => {
  const { q, limit, offset } = c.req.query();
});
```

- 参考: https://hono.dev/docs/api/request

### POST メソッドでデータを受け取る

```ts
app.post("/posts", async (c) => {
  // Body で送られてきたデータをパースする（型パラメーターの指定も可能）
  const body = await c.req.parseBody();
  const title = body.title;
  const message = body.message;

  if (!title || !message) {
    return c.json({ message: "Title and message are required" }, 400);
  }
  console.log(`Received: ${title}, ${message}`);
  return c.json({ message: "Created" }, 201);
});
```

{{< code lang="console" title="curl で POST リクエストを送信" >}}
$ curl -d 'title=Title-1' -d 'message=Message-1' localhost:8787/posts
{"message":"Created"}

$ curl -X POST localhost:8787/posts
{"message":"Title and message are required"}
{{< /code >}}

JSON 形式で送られてきたデータを扱うには次のようにします。

```ts
interface PostBody {
  title: string;
  message: string;
}

app.post("/posts", async (c) => {
  const body = await c.req.json<PostBody>();
  const title = body.title;
  const message = body.message;
  // ...
});
```

{{< code lang="console" title="curl で POST リクエストを送信（JSON 形式）" >}}
$ curl --json '{"title":"Title-1", "message":"Message-1"}' localhost:8787/posts
{"message":"Created"}
{{< /code >}}

- 参考: https://hono.dev/examples/web-api
- 参考: https://hono.dev/docs/api/routing


### 外部 API を呼び出す (fetch)

```ts
// ポケモンの情報を取得するエンドポイント（例: /pokemon/pikachu）
app.get("/pokemon/:id", async (c) => {
  // Get the `id` path parameter and encode it to prevent injection
  const id = c.req.param("id");
  const safeId = encodeURIComponent(id);

  // Call the PokeAPI
  const res = await fetch(`https://pokeapi.co/api/v2/pokemon/${safeId}`);
  if (!res.ok) {
    const errorText = await res.text();
    console.log(`Pokemon API error: ${errorText}`);
    return c.json({ message: errorText }, 500);
  }

  const data = (await res.json()) as Record<string, unknown>;
  return c.json(data);
});
```

### Header をセットする

```ts
c.header("X-Message", "Hello!");
```

### シークレット情報を設定＆参照する {#secret}

Cloudflare Workers では、Bindings という仕組みでシークレット情報（外部 API のキーなど）を参照できるようになっています。
下記は、`HOGE_API_KEY` というシークレット情報を定義して、Hono アプリからその値を参照する方法です。

開発環境 (`wrangler dev`) で使うシークレット情報は、**`.dev.vars`** というファイルで定義しておきます。
このファイルは `.gitignore` に登録されていて Git にコミットされないようになっています。

{{< code lang="ini" title=".dev.vars" >}}
HOGE_API_KEY="NI6IkpXeiI...省略...leiOiJUzI1"
{{< /code >}}

本番環境 (Cloudflare Workers) にシークレット情報を登録するには、**`wrangler secret`** コマンドを使います。
Cloudflare の[ダッシュボード](https://dash.cloudflare.com/)でも設定できますが、コマンドを使った方が楽です。

{{< code lang="console" title="シークレット情報の登録" >}}
$ wrangler secret put HOGE_API_KEY
Enter a secret value: **********************
{{< /code >}}

シークレット情報は下記のように **`c.env.変数名`** で参照できます。
ここでは、シークレット情報が取得できない場合に 500 エラーを返すようにミドルウェア実装を追加しています。

{{< code lang="ts" hl_lines="12 20" >}}
import { Hono } from "hono";

// 環境変数やシークレットの型定義
type Bindings = {
  HOGE_API_KEY: string;
};

const app = new Hono<{ Bindings: Bindings }>();

// 必要な環境変数が設定されていない場合は 500 エラーを返すミドルウェア
app.use(async (c, next) => {
  if (!c.env.HOGE_API_KEY) {
    console.error("HOGE_API_KEY is not set");
    return c.json({ message: "The server setup is not complete" }, 500);
  }
  await next();
});

// 各ハンドラーの中でシークレットを参照
app.get("/", (c) => {
  const apiKey = c.env.HOGE_API_KEY;
  const masked = apiKey.substring(0, 4) + "****";
  return c.json({ message: `API key: ${masked}` });
});

export default app;
{{< /code >}}

- 参考: https://hono.dev/docs/getting-started/cloudflare-workers#bindings
- 参考: https://developers.cloudflare.com/workers/configuration/secrets/

### （アプリ用の）環境変数を設定＆参照する

シークレット情報ではない、公開可能な環境変数は **`wrangler.toml`** の中の **`[vars]`** セクションで定義します。

{{% note title="システムの環境変数ではない" %}}
ここで言う環境変数というのは、あくまで Workers のコード（Hono のアプリ）から参照可能な変数のことであり、`wrangler` コマンドに渡される[システム環境変数](https://developers.cloudflare.com/workers/wrangler/system-environment-variables/) とは異なることに注意してください。
開発中の PC に設定されているシステム環境変数を Workers のコードから参照することはできません。
{{% /note %}}

{{< code lang="toml" title="wrangler.toml" >}}
# ...
[vars]
API_BASE_URL = "https://api.example.com/"
{{< /code >}}

参照方法は、シークレット情報と同じく **`c.env.変数名`** です。

{{< code lang="ts" title="src/index.ts" hl_lines="12 20" >}}
import { Hono } from "hono";

// 環境変数やシークレットの型定義
type Bindings = {
  API_BASE_URL: string;
};

const app = new Hono<{ Bindings: Bindings }>();

// 必要な環境変数が設定されていない場合は 500 エラーを返すミドルウェア
app.use(async (c, next) => {
  if (!c.env.API_BASE_URL) {
    console.error("API_BASE_URL is not set");
    return c.json({ message: "The server setup is not complete" }, 500);
  }
  await next();
});

// 各ハンドラーの中で環境変数を参照
app.get("/", (c) => {
  return c.json({ message: `API_BASE_URL: ${c.env.API_BASE_URL}` });
});

export default app;
{{< /code >}}

特定の環境で別の値を使いたい場合は、`wrangler.toml` の中で、**`[env.<環境名>.vars]`** というセクションを作って環境変数を定義します。
次の例では、**`local`** という環境用の環境変数を定義しています。

{{< code lang="toml" title="wrangler.toml" >}}
[vars]
API_BASE_URL = "https://api.example.com/"

[env.local.vars]
API_BASE_URL = "http://localhost:3000/"
{{< /code >}}

環境を切り替えて開発サーバーを起動するには、`wrangler dev` コマンドに **`--env <環境名>`** オプションをつけて起動します。
`npm` や `pnpm` のスクリプト経由で起動するときは以下のような感じで起動します。

{{< code lang="console" title="開発用サーバーの起動（local という名前の環境で起動）" >}}
$ npm run dev -- --env local  # npm の場合 （-- を挟むことに注意）
$ pnpm dev --env local        # pnpm の場合
{{< /code >}}

開発環境では常に `--env local` で起動したいという場合は、次のように `package.json` の中でスクリプト定義してしまえばよいでしょう。

{{< code lang="js" title="package.json" hl_lines="4" >}}
{
  "name": "my-hono-app",
  "scripts": {
    "dev": "wrangler dev --env local",
    "deploy": "wrangler deploy --minify"
  },
  // ...
}
{{< /code >}}

### その他

- ミドルウェアから route ハンドラーにデータを渡す
  - https://hono.dev/docs/api/context#set-get
- ミドルウェアから 401 エラーを返す
  - https://hono.dev/docs/api/exception
- CORS 対応
  - https://hono.dev/docs/guides/middleware#context-access-inside-middleware-arguments

