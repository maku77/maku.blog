---
title: "Cloudflare Workers からの fetch 呼び出しをキャッシュする"
url: "p/d5mqo6c/"
date: "2025-01-30"
tags: ["Cloudflare"]
draft: true
---

{{% private %}}
- https://developers.cloudflare.com/workers/reference/how-the-cache-works/
- https://developers.cloudflare.com/workers/examples/cache-using-fetch/
- https://developers.cloudflare.com/workers/examples/cache-post-request/
{{% /private %}}

コンピューターサイエンスにおける難題のひとつであるキャッシュのお話です。
ここでは、サーバーレス関数のプラットフォームである Cloudflare Workers で、サードパーティ API の fetch レスポンスのキャッシュをどのように扱うかを説明します。


Cloudflare Workers でキャッシュするとはどういうことか
----

Cloudflare Workers にデプロイされた JavaScript 関数は、単一のオリジンサーバーで実行されるのではなく、世界中に分散された Cloudflare CDN 上で実行されます。
つまり、Cloudflare Workers 上でキャッシュメカニズムを動作させるということは、実質的にはクライアントに近いエッジサーバーからキャッシュを返すことを意味します（つまり、どこからでも速い）。

{{< mermaid >}}
flowchart LR
  A[Client] --> B["Worker (Edge)"]
  subgraph Cloudflare
    B
    C[(Cache)]
  end
  B --> A
  B -->|fetch| D["3rd Party API (Origin)"]
  D -->|response| B
{{< /mermaid >}}

Cloudflare Workers 上で扱える Runtime API として [Cache API](https://developers.cloudflare.com/workers/runtime-apis/cache/) が提供されており、これを使ってエッジサーバー上のキャッシュを制御することができます。
この API は、Web 標準の [Cache](https://developer.mozilla.org/en-US/docs/Web/API/Cache) インタフェースを踏襲しており、`Response` オブジェクトの内容をキャッシュする仕組みになっています。
例えば、バックエンド API のレスポンス（あるいはそれを加工したデータ）をキャッシュすることで、API レスポンスのレイテンシを低減すると同時に、API リクエスト数を削減することができます。

実は、Cloudflare Workers 上での `fetch` 呼び出しはデフォルトでキャッシュされるようになっており、明示的に Cache API を呼ぶ必要はなかったりします。
ただし、これは主に静的なリソースを対象としており、Web API などの JSON レスポンスがキャッシュされるかどうかは、ちょっと複雑な条件によって決まります。

- 参考: [Default Cache Behavior · Cloudflare Cache (CDN) docs](https://developers.cloudflare.com/cache/concepts/default-cache-behavior/)
  > - Cloudflare does not cache the resource when:
  >   - The Cache-Control header is set to private, no-store, no-cache, or max-age=0.
  >   - The Set-Cookie header exists.
  >   - The HTTP request method is anything other than a GET.
  > - Cloudflare does cache the resource when:
  >   - The Cache-Control header is set to public and max-age is greater than 0.
  >   - The Expires header is set to a future date.

例えば、`fetch` リクエストのメソッドが `GET` じゃなかったり、ユーザー固有の情報（クッキーや秘匿情報）が含まれていると、`fetch` レスポンスはキャッシュされないようになっています。
オリジンサーバーが public にはキャッシュするなという情報を返している（例: `Cache-Control: private`）のであれば、Cloudflare はそれに従ってキャッシュしない、ということですね。
ごもっともです。

バックエンド API のレスポンスを Worker からの `fetch` で自動的にキャッシュするには、オリジンサーバーが返すレスポンスヘッダーに `Cache-Control: public, max-age=3600` など、いわゆる共有キャッシュ (shared cache) を有効にするディレクティブが含まれている必要があります。
例えば、下記の Poke API のレスポンスの `Cache-Control` ヘッダーには、共有キャッシュが有効であるという情報が含まれているため、Cloudflare の `fetch` 呼び出しは自動的にキャッシュされます。
つまり、2 回目以降の `fetch` 呼び出しは Poke API にアクセスしません。

{{< code lang="ts" title="自動的にキャッシュされる fetch 呼び出しの例" >}}
const res = await fetch("https://pokeapi.co/api/v2/generation/");
if (!res.ok) {
  throw new Error("Poke API Error: " + res.statusText);
}

console.log(res.headers.get("cache-control"));  // public, max-age=86400, s-maxage=86400
console.log(res.headers.get("cf-cache-status"));  // HIT
console.log(await res.text());
{{< /code >}}

`fetch` 関数がキャッシュされたレスポンスを返したかどうかは、レスポンスヘッダーの **`CF-Cache-Status:`** の値で判断できます。
上記のコードを 2 回動かすと、このヘッダーの値が **`HIT`** になり、キャッシュされていることを確認できます。
逆に、前述のような条件を満たさないバックエンド API のレスポンスをキャッシュするには、`Cache` API を明示的に呼び出すことでキャッシュを制御する必要があります。


Cloudflare Workers での fetch キャッシュ実装例
----

下記の `cache` モジュールは、任意の `Response` オブジェクトの `Cache-Control` ヘッダーを強制的に変更して、Cache API でキャッシュする実装例です。
Cache オブジェクトの以下のメソッドを使用しています（参考: [Workers - Runtime APIs - Cache](https://developers.cloudflare.com/workers/runtime-apis/cache/)）。

- `cache.put(request, response)` -- キャッシュへの保存
- `cache.match(request, options)` -- キャッシュの参照
- `cache.delete(request, options)` -- キャッシュの削除 (Purge)

{{< code lang="ts" title="src/cache.ts" hl_lines="28 37 44" >}}
import { Context } from "hono";

// Default cache TTL for all responses
const CACHE_TTL_SECONDS = 60 * 10; // 10 minutes

/**
 * Store a response object in the cache.
 */
export function putCache(
  c: Context,
  url: URL | string,
  res: Response,
  ttl: number = CACHE_TTL_SECONDS
): void {
  // Add "Cache-Control" header to enable caching
  const newHeaders = new Headers(res.headers);
  newHeaders.append("Cache-Control", `public, s-maxage=${ttl}`);

  // Create a new response object with the modified headers
  const newResponse = new Response(res.clone().body, {
    headers: newHeaders,
    status: res.status,
    statusText: res.statusText,
  });

  // Save the response object to the cache. Use waitUntil to
  // prevent the worker from being killed before the cache is updated.
  c.executionCtx.waitUntil(caches.default.put(url, newResponse));
}

/**
 * Retrieve a response object from the cache.
 *
 * "Cf-Cache-Status" header is set to "HIT" when the response is cached.
 */
export async function getCache(url: URL | string): Promise<Response | undefined> {
  return await caches.default.match(url);
}

/**
 * Purge the cache for a given URL.
 */
export function deleteCache(c: Context, url: URL | string): void {
  c.executionCtx.waitUntil(caches.default.delete(url));
}
{{< /code >}}

ポイントは、キャッシュの保存時には `await` ではなく、Worker のハンドラー関数に渡される `ExecutionContext` の **`waitUntil()`** メソッドを使用するところです。
Hono フレームワークを使っているときは、上記のように Hono の `Context` オブジェクトから `executionCtx` プロパティを参照します。

`waitUntil()` メソッドに `Promise` オブジェクトを渡すと、Worker の処理をそこでブロックせずに、すぐにレスポンスを返すことができます。
Worker インスタンスはレスポンスを返し終わると、通常はすぐに破棄されてしまいますが、`waitUntil()` に渡された `Promise` 処理（ここではキャッシュへの保存処理）は、最後まで実行してくれます。
`waitUntil()` ではなく `await` を使ってしまうと、キャッシュの保存処理が終わるまで Worker からレスポンスを返せなくなってしまいます。

ここで実装した、`putCache()` と `getCache()` は次のような感じで使用します。

{{< code lang="ts" hl_lines="3 11" >}}
async function fetchWithCache(c: Context<Env>, url: string): Promise<Response> {
  // キャッシュに Response オブジェクトがあればそれを返す
  let res = await getCache(url);
  if (res) return res;

  // アクセストークンの必要な 3rd Party API の呼び出し
  const token = await getToken(c);
  res = await fetch(url, { headers: { Authorization: `Bearer ${token}` } });

  // キャッシュに Response オブジェクトを保存
  if (res.ok) putCache(c, url, res);

  return res;
}
{{< /code >}}

レスポンスのキャッシュは、基本的には GET リクエストのみに適用することになります（GraphQL API でクエリに POST メソッドを使っているときなどはキャッシュできそう）。
あと、当然ですが、キャッシュされてはいけないレスポンスがキャッシュされないように十分に気を付けてください。


fetch キャッシュの別の実装方法
----

### KV ストレージを使う方法

Cloudflare のキーバリューストアである KV もエッジ上に配置されるので高速なキャッシュ用途として利用できます（参考: [Cloudflare Workers の KV ストレージの使い方](/p/xb5cpr6/)）。

{{< code lang="toml" title="wrangler.toml（Binding 設定）" >}}
[[kv_namespaces]]
binding = "KV"
id = "40798a14ee51b2b6ed39f3c363ca5388"
{{< /code >}}

{{< code lang="ts" title="Worker からの KV の利用" >}}
// API で取得した JSON データを KV に保存
env.KV.put(urlStr, jsonData, { expirationTtl: 600 });

// KV から JSON データを取得
const jsonData = await env.KV.get(urlStr, { type: "json" });
{{< /code >}}

`Response` オブジェクトをキャッシュする Cache API と比べ、KV には任意の JSON オブジェクトを保存することができます。
Cache API のような複雑なキャッシュ条件も絡まないので、特定のデータのみをキャッシュする用途では KV の方が扱いやすいかもしれません。

### 3rd Party API 呼び出しのキャッシュ専用 Worker を作る方法

TODO: 書く

