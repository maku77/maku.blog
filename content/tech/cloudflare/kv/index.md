---
title: "Cloudflare Workers の KV ストレージの使い方"
url: "p/xb5cpr6/"
date: "2025-01-19"
tags: ["Cloudflare", "Hono"]
---

Cloudflare Workers KV とは
----

Cloudflare Workers は基本的にステートレスなサーバーレスプラットフォームですが、[KV](https://developers.cloudflare.com/kv/) というデータストアを使うことで、複数のアクセス間でデータを共有することができます。
KV という名前の通り、キー (key) と値 (value) のペアを保存することができ、Worker のコードから簡単にアクセスできるようになっています。

Cloudflare Workers の基本的な使い方は[こちらを参照](/p/rn7p7n5/)してください。
下記作業では、すでに Cloudflare Workers のプロジェクト (`hello-worker`) が作成されていることを前提としています。
まだプロジェクトが作成されていない場合は、下記のように `wrangler` コマンドでプロジェクトを作成してください。

{{< code lang="console" title="Cloudflare Workers プロジェクトの作成" >}}
$ wrangler init hello-worker
$ cd hello-worker
{{< /code >}}


KV namespace の作成
----

KV にデータを保存するためには、まずは key & value の入れ物である **KV namespace** を作成する必要があります。
アカウント内に KV namespace を作成するには、**`wrangler kv namespace create`** コマンドを使います。
ここでは、単純に `KV` という名前の KV namespace を作成してみます。

{{< code lang="console" title="KV namespace の作成" >}}
$ wrangler kv namespace create KV
...
✨ Success!
Add the following to your configuration file in your kv_namespaces array:
[[kv_namespaces]]
binding = "KV"
id = "2161175c318445cfa3d1dac2f20ee660"
{{< /code >}}

KV namespace の作成に成功すると、上記のように一意な ID が割り当てられます。
この ID は、Worker のコード内で使用する変数名と、実際の KV namespace を紐付けるために使われます。
上記コマンドの出力には、そのための `wrangler.toml` での設定方法が示されています。
[Cloudflare のダッシュボード](https://dash.cloudflare.com/)上でも、KV namespace が作成されていることを確認できます。

{{< image border="true" src="img-001.png" title="Cloudflare ダッシュボード上で KV namespace を確認" >}}

コマンド実行時には `KV` という名前しか指定しませんでしたが、実際にはプレフィックスとして Worker 名が付加されます。
例えば、`wrangler.toml` に `name = "hello-worker"` と書いてある場合、`hello-worker-KV` という名前の KV namespace が作成されます（プロジェクトの外で実行すると、`worker` というプレフィックスが付くようです）。
KV namespace の名前はアカウント内で一意でなければいけませんが、このように一意な Worker 名がプレフィックスに付くため、名前が被ってしまう心配はないでしょう。

KV namespace の一覧は **`wrangler kv namespace list`** で確認できます。

{{< code lang="console" title="KV namespace の一覧" >}}
$ wrangler kv namespace list
[
  {
    "id": "2161175c318445cfa3d1dac2f20ee660",
    "title": "hello-worker-KV",
    "supports_url_encoding": true
  }
]
{{< /code >}}

KV namespace が必要なくなったときは、**`wrangler kv namespace delete`** コマンドで簡単に削除できます。

{{< code lang="console" title="KV namespace の削除" >}}
$ wrangler kv namespace delete --namespace-id 2161175c318445cfa3d1dac2f20ee660

# 下記のようにも指定できるけどプロジェクト間違いを避けるために ID 指定の方が無難
$ wrangler kv namespace delete --binding KV
{{< /code >}}


Worker から KV にアクセスする
----

### Binding 設定

Worker のコードから KV namespace にアクセスするには、**`wrangler.toml`**（あるいは `wrangler.json`）に Worker と KV のバインド設定 (binding) を追記しておく必要があります。
追記する内容は、先ほどの KV namespace 作成時に表示された下記のような内容です。

{{< code lang="toml" title="wrangler.toml の場合" >}}
# ...
[[kv_namespaces]]
binding = "KV"
id = "2161175c318445cfa3d1dac2f20ee660"
{{< /code >}}

{{< code lang="js" title="wrangler.json の場合" >}}
{
  // ...
  "kv_namespaces": [
    {
      "binding": "KV",
      "id": "2161175c318445cfa3d1dac2f20ee660"
    }
  ]
}
{{< /code >}}

- **`id`** プロパティ
  - 接続先の KV namespace の ID を指定します。
- **`binding`** プロパティ
  - Worker コード内から KV namespace を参照するときに使う変数名を設定します。
    `wrangler kv` コマンドの `--binding` オプションで指定する名前にも使われます。
    任意の変数名を割り当てられますが、通常は KV namespace の名前と合わせておけばよいです（今回の例では `KV`）。

{{% note title="Binding 設定は面倒くさい？" %}}
Cloudflare Workers の Binding 設定は一見面倒に感じるかもしれませんが、よく考えると、Worker と外部リソースとの接続設定をこれだけの作業で行えるのはとても効率的です。
AWS Lambda などのサーバーレスプラットフォームでは、このような設定を行うために、IAM ロールの設定やポリシーの作成など、より複雑な手順が必要です。
{{% /note %}}

### Worker からの参照

Worker のコード内から KV namespace にアクセスするには、下記のように **`Env`** オブジェクトのプロパティとして参照します。
今回は `binding = "KV"` と設定したので、**`env.KV`** で参照できます。

{{< code lang="ts" title="src/index.ts" hl_lines="10 19 28" >}}
export default {
  async fetch(request, env): Promise<Response> {
    const url = new URL(request.url);

    // "/put?key=HELLO&value=WORLD" にアクセスすると、KV に key & value を保存します。
    if (url.pathname === "/put") {
      const key = url.searchParams.get("key");
      const value = url.searchParams.get("value");
      if (key && value) {
        await env.KV.put(key, value);
        return new Response("Saved");
      }
    }

    // "/get?key=HELLO" にアクセスすると、KV から "HELLO" というキーの値を取得します。
    if (url.pathname === "/get") {
      const key = url.searchParams.get("key");
      if (key) {
        const value = await env.KV.get(key);
        return new Response(value ?? "Not found");
      }
    }

    // "/delete?key=HELLO" にアクセスすると、KV から "HELLO" というキーの値を削除します。
    if (url.pathname === "/delete") {
      const key = url.searchParams.get("key");
      if (key) {
        await env.KV.delete(key);
        return new Response("Deleted");
      }
    }

    return new Response("Hello World!");
  },
} satisfies ExportedHandler<Env>;
{{< /code >}}

### 動作確認

**`wrangler dev`** で開発サーバーを起動し、`curl` コマンドで以下のようにアクセスすると、KV namespace への key & value の保存と取得を確認できます。
Web ブラウザーでアクセスしても OK です。

{{< code lang="console" title="KV namespace への key & value の保存と取得" >}}
$ curl "http://localhost:8787/put?key=HELLO&value=WORLD"
Saved

$ curl "http://localhost:8787/get?key=HELLO"
WORLD
{{< /code >}}

できたっ！ ٩(๑❛ᴗ❛๑)۶ わーぃ


開発サーバーはローカルの KV にアクセスする
----

`wrangler dev`（あるいは `npm run dev`）で起動した開発サーバーは、**デフォルトではローカル環境の KV ストレージ** にアクセスするようになっています。
実体は、**`.wrangler/state/v3/kv/{namespace_id}`** というディレクトリのファイルです。
`wrangler deploy`（あるいは `npm run deploy`）でデプロイした本番環境の Worker が参照する KV ストレージとは別のものを参照しているので注意してください。

KV 内に格納された key & value ペアを扱うためのコマンドとして **`wrangler kv key`** コマンドがありますが、このコマンドでローカル環境の KV ストレージを参照したいときは、**`--local`** オプションを指定する必要があります。

{{< code lang="console" title="ローカル環境の KV ストレージの参照" >}}
# ローカル KV のネームスペース "KV" 内のキーを列挙
$ wrangler kv key list --local --binding KV

# ローカル KV のネームスペース "KV" 内のキー "KEY" の値を取得
$ wrangler kv key get --local --binding KV "KEY"

# ローカル KV のネームスペース "KV" 内のキー "KEY" に値 "VALUE" を設定
$ wrangler kv key put --local --binding KV "KEY" "VALUE"
{{< /code >}}

逆に `--local` オプションを指定せずに `wrangler kv key` コマンドを実行した場合は、本番環境のグローバルな KV ストレージを参照することになります。

{{< code lang="console" title="本番環境の KV ストレージの参照" >}}
$ wrangler kv key list --binding KV
$ wrangler kv key get --binding KV "aaa"
$ wrangler kv key put --binding KV "aaa" "bbb"
{{< /code >}}


プレビュー用の KV namespace を用意する
----

KV namespace の ID は、デフォルトでは開発環境（`wrangler dev`）と本番環境（`wrangler deploy`）で同じものが使用されます。
開発環境用に KV namespace を分けたい場合は、KV namespace を作成するときに **`--preview`** オプションを指定します。

{{< code lang="console" title="プレビュー用の KV namespace の作成" >}}
$ wrangler kv namespace create KV --preview
...
🌀 Creating namespace with title "hello-worker-KV_preview"
✨ Success!
Add the following to your configuration file in your kv_namespaces array:
{
  "kv_namespaces": [
    {
      "binding": "KV",
      "preview_id": "6ab74fcf66d84b7da978e5410ec1261d"
    }
  ]
}
{{< /code >}}

`wrangler.toml`（あるいは `wrangler.json`）には、上記コマンドで表示された `preview_id` を追記します。

{{< code lang="toml" title="wrangler.toml（追記）" >}}
# ...
[[kv_namespaces]]
binding = "KV"
id = "2161175c318445cfa3d1dac2f20ee660"
preview_id = "6ab74fcf66d84b7da978e5410ec1261d"
{{< /code >}}

{{< code lang="js" title="wrangler.json（追記）" >}}
{
  // ...
  "kv_namespaces": [
    {
      "binding": "KV",
      "id": "2161175c318445cfa3d1dac2f20ee660",
      "preview_id": "6ab74fcf66d84b7da978e5410ec1261d"
    }
  ]
}
{{< /code >}}

これで、開発サーバー (`wrangler dev` / `npm run dev`) が参照する KV namespace と、本番環境 (`wrangler deploy` / `npm run deploy`) が参照する KV namespace が分かれるようになります。
Worker コード内からの参照方法は、`env.KV` で共通です。

さらに、プレビュー用の KV namespace を作成した後は、開発サーバーを Cloudflare 側の KV ストレージに接続できるようになります（開発サーバーの起動時に **`--remote`** オプション）。
この場合は、Cloudflare 側に作成されたプレビュー用の KV namespace が参照されます。

{{< code lang="console" title="開発サーバーから Cloudflare 側のプレビュー用 KV namespace を参照" >}}
$ wrangler dev --remote
{{< /code >}}

ローカルやらプレビューやらで混乱するのでまとめておきます。

|  | プレビュー用の<br>KV namespace なし | プレビュー用の<br>KV namespace あり |
| ---- | ---- | ---- |
| 開発サーバー<br>(`wrangler dev`) | ローカル KV が参照される<br>（本番用 KV namespace） | ローカル KV が参照される<br>（プレビュー KV namespace） |
| 開発サーバー<br>(`wrangler dev --remote`) | （起動できず） | リモート KV が参照される<br>（プレビュー KV namespace） |
| 本番環境<br>(`wrangler deploy`) | リモート KV が参照される<br>（本番用 KV namespace） | ─ |

プレビュー用の KV namespace の Binding を設定しておくと、振る舞いが変わるところがポイントですね。
おすすめはしませんが、`wrangler dev --remote` で開発サーバーを起動したときに、Cloudflare 側の本番用の KV namespace に接続したいときは、設定ファイル (`wrangler.toml`) 内の `preview_id` の参照先を `id` と同じものに設定します。

{{< code lang="toml" title="wrangler.toml" hl_lines="4" >}}
[[kv_namespaces]]
binding = "KV"
id = "2161175c318445cfa3d1dac2f20ee660"
preview_id = "2161175c318445cfa3d1dac2f20ee660"
{{< /code >}}


応用）Hono フレームワークと組み合わせ KV を使用する
----

Web API や Web アプリを作るときは、Hono フレームワークを使うと便利です（参考: [Hono で軽量な Web API を作る](/p/33cc7jy/)）。
ここでは、Hono を使った API 実装内から KV namespace にアクセスしてみます。

まずは、Hono 用のプロジェクトを作成します。

{{< code lang="console" title="Hono プロジェクトの作成" >}}
$ pnpm create hono@latest hello-hono
$ cd hello-hono
{{< /code >}}

KV namespace（名前: `KV`）を作成し、`wrangler.toml` にバインド設定を追記します。

{{< code lang="console" title="KV namespace の作成とバインド設定" >}}
$ wrangler kv namespace create KV
{{< /code >}}

{{< code lang="toml" title="wrangler.toml（追記）" >}}
[[kv_namespaces]]
binding = "KV"
id = "cd772ad188d347bd84a686e6263544f3"
{{< /code >}}

Hono 用の Worker コード内から KV namespace にアクセスするときは、各ハンドラー関数に渡される `Context` オブジェクト経由で、**`c.env.KV`** のように参照します。

{{< code lang="ts" title="src/index.ts" hl_lines="16 25" >}}
import { Hono } from "hono";

// Automatically generate with `wrangler types`
interface Bindings {
  KV: KVNamespace;
}

const app = new Hono<{ Bindings: Bindings }>();

app.get("/put", (c) => {
  const key = c.req.query("key");
  const value = c.req.query("value");
  if (!key || !value) {
    return c.text("key and value are required");
  }
  c.env.KV.put(key, value);
  return c.text("Saved");
});

app.get("/get", async (c) => {
  const key = c.req.query("key");
  if (!key) {
    return c.text("key is required");
  }
  const value = await c.env.KV.get(key);
  return c.text(value ?? "Not found");
});

export default app;
{{< /code >}}

慣れるととても簡単でいいですね！
ちなみに上記のサンプルコードは RESTful な感じではなく、すべて GET メソッドとしてアクセスするよう実装されていることに注意してください。

