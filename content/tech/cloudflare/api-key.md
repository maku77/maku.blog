---
title: "Cloudflare Workers で作成した Web API に API キーによるアクセス制限をかける"
url: "p/u6w6fq4/"
date: "2025-01-13"
tags: ["Cloudflare", "Hono"]
---

{{% private %}}
- 参考: https://developers.cloudflare.com/workers/examples/auth-with-headers/
{{% /private %}}

Cloudflare Worker で作成した Web API を公開するときに、固定の API キーによるアクセス制限をかける方法です。
独自サービス用のバックエンド API など、一般公開しない API の実装で使うことを想定しています。


準備
----

### プロジェクトの作成

Cloudflare Workers のプロジェクトをまだ作っていないときは、[wrangler を使って作成](/p/rn7p7n5/) しておきます。

{{< code lang="console" title="プロジェクトを作成" >}}
$ wrangler init hello-api
$ cd hello-api
{{< /code >}}

### API キーを生成して登録する

アクセス制御用の独自の API キーを用意して、それを Workers に登録して使います。
次のように、ランダムな文字列を生成して API キーにしてしまえばよいです。
自分で考えた文字列でも構いませんが、第三者が想像しにくい文字列にしてください。

{{< code lang="console" title="ランダムな API キーを生成" >}}
$ openssl rand -base64 32
mmdkR+mMvBUnYeu2sn1kMqlXjK9Q4A0Os3I4M4aiMQs=
{{< /code >}}

用意した API キーは、サーバー側の **`API_KEY`** という環境変数にセットすることにします。
開発サーバー用の環境変数は **`.dev.vars`** ファイル、本番環境用の環境変数は **`wrangler secret`** コマンドで設定します。

{{< code lang="ini" title=".dev.vars（ローカル環境用のシークレット設定）" >}}
API_KEY="mmdkR+mMvBUnYeu2sn1kMqlXjK9Q4A0Os3I4M4aiMQs="
{{< /code >}}

{{< code lang="console" title="Cloudflare Workers 用のシークレット設定" >}}
$ wrangler secret put API_KEY
Enter a secret value: **********************
{{< /code >}}

これらの値は、Workers のプログラム内で **`env.API_KEY`** で参照することができます。


x-api-key ヘッダーで API キーを送る方法
----

下記は、クライアントから **`x-api-key`** ヘッダーで API キーを送ってもらう場合の実装例です。

### 素の Cloudflare Worker で実装する場合

シークレット (`env.API_KEY`) の値と、`x-api-key` ヘッダーの値が一致しないときにアクセスを拒否するように実装します。

{{< code lang="ts" title="src/index.ts" >}}
export default {
  async fetch(req: Request, env: Env, ctx: ExecutionContext): Promise<Response> {
    // Check if the API key is valid
    if (!env.API_KEY) {
      console.error("API_KEY is not set");
      return new Response("The server setup is not complete", { status: 500 });
    }
    const apiKey = req.headers.get("x-api-key");
    if (apiKey !== env.API_KEY) {
      return new Response("Access denied", { status: 403 });
    }

    // API key is valid, return a response
    return new Response("Hello World!");
  },
};
{{< /code >}}

サーバー側で環境変数 (`API_KEY`) が定義されていないときは **Internal Server Error (500)** を返し、クライアントから正しい `x-api-key` ヘッダーが送られてこなかったときは **Forbidden (403)** を返ようにしています。

{{% note title="env の型が不明なとき" %}}
TypeScript を使っているときに、`env.API_KEY` の型が不明でエラーになるときは、**`wrangler types`** コマンドを実行すると、`.dev.vars` や `wrangler.toml` ファイルの内容から型情報を自動生成してくれます。
{{% /note %}}

`pnpm dev` (`npm run dev`) で開発サーバーを起動して、次のようにアクセス制御されていれば成功です。

```console
# x-api-key ヘッダーを送らなかったとき
$ curl localhost:8787
Access denied

# x-api-key ヘッダーを送ったとき
$ curl -H "x-api-key: ..." localhost:8787
Hello World!
```

### Hono フレームワークを使って実装する場合

Hono フレームワークで Web API を作っている場合もほぼ同様に実装できます（[参考](/p/33cc7jy/#secret)）。
アクセス制御する部分を次のようにミドルウェアモジュールとして切り出しておくと分かりやすくなります。

{{< code lang="ts" title="src/middleware.ts" >}}
import { createMiddleware } from "hono/factory";

interface Bindings {
  API_KEY: string;
}

/**
 * Middleware to check if the corect API key is received.
 */
export const apiKeyMiddleware = createMiddleware<{ Bindings: Bindings }>(
  async (c, next) => {
    if (!c.env.API_KEY) {
      console.error("API_KEY is not set");
      return c.json({ message: "The server setup is not complete" }, 500);
    }

    if (c.req.header("x-api-key") !== c.env.API_KEY) {
      return c.json({ message: "Access denied" }, 403);
    }

    await next();
  }
);
{{< /code >}}

{{< code lang="ts" title="src/index.ts（Hono の場合）" >}}
import { Hono } from "hono";
import { apiKeyMiddleware } from "./middleware";

const app = new Hono();
app.use(apiKeyMiddleware);
app.get("/", (c) => {
  return c.json({ message: "Hello" });
});

export default app;
{{< /code >}}

メインロジックがすっきりするのがいいですね！


Authorization ヘッダーで API キーを送る方法
----

{{% private %}}
- 参考: https://developers.cloudflare.com/d1/tutorials/build-an-api-to-access-d1/#4-initialize-the-application
{{% /private %}}

独自ヘッダーで送るとのほとんど変わらないですが、`Authorization: Bearer ...` というヘッダーで API キーを送る方法もあります。
Hono フレームワークは、[`bearerAuth` ミドルウェア](https://hono.dev/docs/middleware/builtin/bearer-auth) を標準で用意しているので、これを利用すると楽に実装できます。

{{< code lang="ts" title="src/index.ts" >}}
import { Hono } from "hono";
import { bearerAuth } from "hono/bearer-auth";

type Bindings = {
  API_KEY: string;
};

const app = new Hono<{ Bindings: Bindings }>();

app.use(async (c, next) => {
  const auth = bearerAuth({ token: c.env.API_KEY });
  return auth(c, next);
});

app.get("/", (c) => {
  return c.json({ message: "Hello" });
});

export default app;
{{< /code >}}

{{< code lang="console" title="Bearer トークンを使ってアクセス" >}}
$ curl -H "Authorization: Bearer ..." localhost:8787
{"message":"Hello"}
{{< /code >}}

ただ、`Authorization: Bearer ...` ヘッダーは、主に OAuth のシーケンスなどでアクセストークンを送るときに使われるヘッダーなので、API キーを送るときに使うのは少し違和感があるかもしれません。
そんなときは、前述の `x-api-key` 独自ヘッダーを使っておけばよいです。
ちなみに、最近は `x-` プレフィックスではなく、**`{サービス名}-api-key`** のように、独自サービスの名前をプレフィックスに付けることが推奨されているようです。

