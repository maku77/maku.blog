---
title: "Cloudflare Workers で別の Worker と連携する (Service Bindings)"
url: "p/mba7gp6/"
date: "2025-01-29"
tags: ["Cloudflare"]
---

{{% private %}}
- https://developers.cloudflare.com/workers/runtime-apis/bindings/service-bindings/
{{% /private %}}

Service Bindings とは
----

Cloudflare Workers は、JavaScript などで実装されたコードを実行可能なサーバーレスプラットフォームです。
Cloudflare Workers の **Service Bindings** という仕組みを利用すると、**ある Worker から別の Worker を簡単に呼び出すことができます**。
通常のメソッド呼び出しと同じ感覚で別の Worker で定義されたメソッドを呼びすことができ、料金上の呼び出し回数も増えません。
しかも同じスレッド上で実行されるので、呼び出しのオーバヘッドがありません。

よーするに、マイクロサービスを Cloudflare Workers で実装しておけば、パフォーマンスやコストの心配をすることなく、簡単に連携できてしまうすごいやつです（<s>AWS などで同じようなことをしようとすると大変</s>）。


テスト用プロジェクトの作成
----

ここでは、Worker A から Worker B を呼び出すよう実装してみます。
まずはそれぞれの Worker 用のプロジェクトを作成しておきます。

{{< code lang="console" title="2 つの Worker プロジェクトを作成" >}}
$ wrangler init worker-a
$ wrangler init worker-b
{{< /code >}}

Service Bindings では、Worker 名を使って Bindings の設定を行うことになるので、ここで指定した名前は重要です。
Worker 名を変えたくなったら、`wrangler.toml`（あるいは `wrangler.jsonc`）内の `name` のエントリーを編集してください。


Worker B の実装（呼び出される側）
----

先に、呼び出される側の Worker B を実装します。
通常の Worker 実装と同様に、下記のように `fetch` メソッドを公開するだけでも連携できるのですが、

```ts
export default {
  async fetch(request, env, ctx) { ... }
}
```

この代わりに、**`WorkerEntrypoint`** を継承したクラスを export すると任意のメソッドを公開することができて便利です（公式ドキュメントでは、後者を **RPC** と呼んでいます）。
プロキシのようなものを作りたい場合は、従来の `fetch` ハンドラーを実装するだけで十分かもしれません。

次の例では、簡単なテキストレスポンスを返す `fetch()` ハンドラーと、足し算を行う `add()` メソッドを実装しています。

{{< code lang="ts" title="src/main.ts" >}}
import { WorkerEntrypoint } from "cloudflare:workers";

export default class extends WorkerEntrypoint {
  async fetch(req: Request): Promise<Response> {
    return new Response("Hello from B");
  }

  add(a: number, b: number): number {
    return a + b;
  }
}
{{< /code >}}

この実装では使っていませんが、`ExecutionContext` オブジェクトを参照したくなったら、任意の場所から **`this.ctx`** で参照できます。

Worker B を public な URL として公開する必要がない場合は、`wrangler.toml` の中で以下のように設定しておきます。
また、開発時に混乱しないように、ポート番号をデフォルトの `8787` から変えておくとよいです。

{{< code lang="toml" title="wrangler.toml（抜粋）" >}}
# workers.dev のサブドメインで公開しない
workers_dev = false

# 開発時にポート番号が被らないよう変えておく
[dev]
port = 18787
{{< /code >}}


Worker A の実装（呼び出す側）
----

次に、Worker B を呼び出す Worker A の実装です。
`wrangler.toml` に Bindings 情報として Worker B の名前を設定します。

{{< code lang="toml" title="wrangler.toml（抜粋）" >}}
[[services]]
binding = "WORKER_B"
service = "worker-b"
{{< /code >}}

すると、Worker A の中から、`Env` オブジェクト経由で **`env.WORKER_B.fetch()`** や **`env.WORKER_B.add()`** のように Worker B のメソッドを呼び出せるようになります。

{{% note title="TypeScript 用の型定義" %}}
TypeScript を使っているときは、`wrangler types` コマンドで `Env` の型情報を生成できます。
また、`env.WORKER_B` の型は、今回は次のように手動で定義しました（いずれ自動生成できるようになりそう）。

{{< code lang="ts" title="src/worker-b.d.ts" >}}
export interface WorkerB {
  fetch(request: Request): Promise<Response>;
  add(a: number, b: number): number;
}
{{< /code >}}
{{% /note %}}

{{< code lang="ts" title="src/main.ts" hl_lines="9 13" >}}
import type { WorkerB } from "./worker-b";

export default {
  async fetch(request, env, ctx): Promise<Response> {
    // TypeScript 用の型付けのため
    const workerB = env.WORKER_B as unknown as WorkerB;

    // fetch の呼び出し
    const res = await workerB.fetch(request);
    const text = await res.text(); // "Hello from B"

    // add の呼び出し
    const sum = await workerB.add(1, 2); // 3

    return new Response(text + ", " + sum);
  },
} satisfies ExportedHandler<Env>;
{{< /code >}}

簡単スギィィィ！


動作確認
----

ローカルで Service Bindings のテストを行うときは、Worker A と Worker B それぞれのプロジェクト下で開発サーバー (`wrangler dev`) を起動します。
どちらから起動してもうまく連携してくれますが、Worker B から起動するのが自然です。

{{< code lang="console" title="Worker B の起動" >}}
$ ~/worker-b
$ pnpm dev
{{< /code >}}

{{< code lang="console" title="Worker A の起動" >}}
$ ~/worker-a
$ pnpm dev
{{< /code >}}

Worker A 側のコンソールで、`b` キーを押してブラウザを開いて、以下のように表示されれば成功です！

```
Hello from B, 3
```


デプロイ
----

Cloudflare Workers にデプロイする場合は、依存関係の問題が発生しないように Worker B の方からデプロイします。

```console
$ cd ~/worker-b
$ pnpm run deploy
$ cd ~/worker-a
$ pnpm run deploy
```

できた〜 ٩(๑❛ᴗ❛๑)۶ わーぃ

