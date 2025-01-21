---
title: "Cloudflare Workers の KV ストレージでアクセストークンをキャッシュする"
url: "p/q896mzu/"
date: "2025-01-19"
tags: ["Cloudflare", "Hono"]
---

何をするか
----

ここでは、Cloudflare Worker から KV ストレージを使用する例として、サードパーティ製 API のアクセストークンなど、**有効期限のある情報をキャッシュする方法** を紹介します。

サードパーティ API としてアクセストークン（あるいはリフレッシュトークン）を要求する API が定義されている場合、取得したトークンは有効期限内は再利用することが想定されています。
しかし、呼び出しごとに独立したコンテキストで動作する Cloudflare Workers の環境では、呼び出しのたびにトークンを取得することになってしまいます。
このようなケースでは、取得したトークンを KV ストレージにキャッシュしておくことで、次回のアクセス時に再利用することができます。

KV では、キーと値のペアを保存するときにオプション情報として TTL (Time To Live) を指定することができるので、こういった有効期限のある情報をキャッシュするのに適しています。
KV ストレージの基本的な使い方は下記を参照してください。

- 参考: [Cloudflare Workers の KV ストレージの使い方](/p/xb5cpr6/)



プロジェクトと KV namespace を作成する
----

Cloudflare Worker のプロジェクトをまだ作成していない場合は先に作成しておきます。
ここでは、[Hono フレームワークを使用する](/p/33cc7jy/)プロジェクトの雛形を作成します。

```console
$ pnpm create hono@latest hello-hono
$ cd hello-hono
```

次に、`wrangler kv` コマンドを使って KV namespace を作成します。
名前は単純に `KV` としておきます。

```console
$ wrangler kv namespace create KV
```

KV namespace が作成できたら、コマンドの出力を参考に、`wrangler.toml`（あるいは `wrangler.json`）に KV namespace のバインド設定を追加します。

{{< code lang="toml" title="wrangler.toml（追記）" >}}
[[kv_namespaces]]
binding = "KV"
id = "84a686e6263544f3cd772ad188d347bd"
{{< /code >}}

これで、Worker から KV ストレージを使う準備が整いました。


実装例
----

エントリーポイントとなる `index.ts` では、サードパーティ製の Web API を呼び出すための `FakeApi.getCurrentUser()` を呼び出すよう実装しておきます（`FakeApi` の実装は後述）。

{{< code lang="ts" title="src/index.ts" >}}
import { Hono } from "hono";
import { FakeApi } from "./fakeapi";

// KV ストレージを使うためのインターフェース定義
export interface Env {
  Bindings: {
    KV: KVNamespace;
  };
}

const app = new Hono<Env>();

app.get("/", async (c) => {
  const res = await FakeApi.getCurrentUser(c);
  return c.json(res);
});

export default app;
{{< /code >}}

次に、今回のポイントである `FakeApi` の実装です。

{{< accordion title="src/fakeapi.ts（全体のコード）" >}}
{{< code lang="ts" >}}
import { Context } from "hono";
import { Env } from "./index";

export class FakeApi {
  /**
   * カレントユーザーの情報を取得します。
   */
  public static async getCurrentUser(c: Context<Env>): Promise<string> {
    // キャッシュされたトークン、あるいは新しいトークンを使って外部 API を呼び出す
    const token = await FakeApi.getToken(c);
    const user = await fetch("https://dummyjson.com/user/me", {
      headers: { Authorization: `Bearer ${token}` },
    });

    return await user.json();
  }

  /**
   * 外部 API アクセス用のアクセストークンを取得します。
   *
   * KV にキャッシュとして保存されている場合はそれを使用します。
   */
  private static async getToken(c: Context<Env>): Promise<string> {
    const TOKEN_KEY = "token"; // KV のキー
    const TOKEN_TTL = 60; // 60 秒後に KV から自動削除（デフォルトは無期限）

    // KV にトークンが保存されている場合はそれを返す
    const cachedToken = await c.env.KV.get(TOKEN_KEY);
    if (cachedToken) {
      console.debug("Cached token found");
      return cachedToken;
    }

    // 保存されていない場合は新しいトークンを取得して KV に保存
    console.debug("Cached token not found, fetching new token");
    const newToken = await FakeApi.getNewToken();
    await c.env.KV.put(TOKEN_KEY, newToken, { expirationTtl: TOKEN_TTL });

    return newToken;
  }

  /**
   * 新しいトークンを取得します。
   */
  private static async getNewToken(): Promise<string> {
    const username = "emilys"; // c.env.FAKEAPI_USERNAME
    const password = "emilyspass"; // c.env.FAKEAPI_PASSWORD
    const res = await fetch("https://dummyjson.com/auth/login", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ username, password }),
    });

    const data = (await res.json()) as { accessToken: string };
    if (!data.accessToken) {
      // This leads to a 500 Internal Server Error
      throw new Error("Access token not found in response");
    }

    return data.accessToken;
  }
}
{{< /code >}}
{{< /accordion >}}

関数ごとに見ていきます。

{{< code lang="ts" >}}
/**
 * カレントユーザーの情報を取得します。
 */
public static async getCurrentUser(c: Context<Env>): Promise<string> {
  // キャッシュされたトークン、あるいは新しいトークンを使って外部 API を呼び出す
  const token = await FakeApi.getToken(c);
  const user = await fetch("https://dummyjson.com/user/me", {
    headers: { Authorization: `Bearer ${token}` },
  });

  return await user.json();
}
{{< /code >}}

`getCurrentUser()` は、アクセストークンを使って何らかの外部 API を呼び出す実装例を示しています。
他の API を呼び出す場合も同様の手順でトークンを取得して利用します。

{{< code lang="ts" hl_lines="11 20" >}}
/**
 * 外部 API アクセス用のアクセストークンを取得します。
 *
 * KV にキャッシュとして保存されている場合はそれを使用します。
 */
private static async getToken(c: Context<Env>): Promise<string> {
  const TOKEN_KEY = "token"; // KV のキー
  const TOKEN_TTL = 60; // 60 秒後に KV から自動削除（デフォルトは無期限）

  // KV にトークンが保存されている場合はそれを返す
  const cachedToken = await c.env.KV.get(TOKEN_KEY);
  if (cachedToken) {
    console.debug("Cached token found");
    return cachedToken;
  }

  // 保存されていない場合は新しいトークンを取得して KV に保存
  console.debug("Cached token not found, fetching new token");
  const newToken = await FakeApi.getNewToken();
  await c.env.KV.put(TOKEN_KEY, newToken, { expirationTtl: TOKEN_TTL });

  return newToken;
}
{{< /code >}}

`getToken()` では、KV ストレージにキャッシュされているアクセストークンを返します。
もしキャッシュされていない場合は、`getNewToken()` を呼び出して新しいトークンを取得します。
新しく取得したトークンは、有効期限 (**`expirationTtl`**) を設定して KV ストレージに保存します。
ここでは、テスト用に 60 秒という短い時間で自動削除するようにしていますが、実際のアプリケーションでは適切な TTL を設定してください。

{{< code lang="ts" >}}
/**
 * 新しいトークンを取得します。
 */
private static async getNewToken(): Promise<string> {
  const username = "emilys";  // c.env.FAKEAPI_USERNAME
  const password = "emilyspass";  // c.env.FAKEAPI_PASSWORD
  const res = await fetch("https://dummyjson.com/auth/login", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ username, password }),
  });

  const data = (await res.json()) as { accessToken: string };
  if (!data.accessToken) {
    // This leads to a 500 Internal Server Error
    throw new Error("Access token not found in response");
  }

  return data.accessToken;
}
{{< /code >}}

`getNewToken()` は、外部 API から新しいアクセストークンを取得するメソッドです。
ここではクレデンシャル情報（`username` と `password`）をハードコードしていますが、実際のアプリケーションでは環境変数などを使って安全に管理してください（参考: [Cloudflare Workers でのシークレットの使用例](/p/u6w6fq4/)）。


動作確認
----

最後に、`wrangler dev` で開発サーバーを立ち上げて動作確認します（`wrangler deploy` して本番環境で確認することもできます）。

{{< code lang="console" title="開発サーバーの起動" >}}
$ npm run dev  # npm の場合
$ pnpm dev     # pnpm の場合
{{< /code >}}

`b` キーを押してブラウザを開くと、サードパーティ API から取得した情報が以下のような感じで表示されるはずです。

```js
{
  "id": 1,
  "firstName": "Emily",
  "lastName": "Johnson",
  "maidenName": "Smith",
  "age": 28,
  "gender": "female",
  // ...
}
```

初回アクセス時には、サーバーログに `Cached token not found, fetching new token` と出力され、新しいトークンが取得されます。
2 回目以降のアクセスでは、`Cached token found` というログが出力され、KV ストレージに保存されたトークンが再利用されます。
`expirationTtl` で指定した時間（60 秒）が経過した後にブラウザをリロードすると、再度新しいトークンが取得されることを確認できます。

以上が、Cloudflare Workers の KV ストレージを使ったアクセストークンキャッシュの実装例です。

できた〜 ٩(๑❛ᴗ❛๑)۶ わーぃ

