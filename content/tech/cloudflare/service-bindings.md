---
title: "Cloudflare Workers で別の Worker と連携する (Service Bindings)"
url: "p/mba7gp6/"
lastmod: "2025-02-01"
date: "2025-01-29"
tags: ["Cloudflare"]
changes:
  - 2025-02-01: TypeScript の型定義方法を追記
---

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
$ pnpm create cloudflare@latest worker-a
$ pnpm create cloudflare@latest worker-b
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

{{< code lang="ts" title="src/index.ts" >}}
import { WorkerEntrypoint } from "cloudflare:workers";

export default class extends WorkerEntrypoint {
  async fetch(req: Request): Promise<Response> {
    return new Response("Hello from B");
  }

  async add(a: number, b: number): Promise<number> {
    return a + b;
  }
}
{{< /code >}}

{{% note title="Env、ExecutionContext の参照方法" %}}
上記の実装では使っていませんが、`Env` オブジェクトや `ExecutionContext` オブジェクトを参照したくなったら、`WorkerEntrypoint` のプロパティ経由で参照できます。

- `Env` オブジェクト
  - `WorkerEntrypoint<Env>` のように型定義しておいて、**`this.env.<BINDING名>`** で参照。
- `ExecutionContext` オブジェクト
  - **`this.ctx.waitUntil()`** のように参照。
{{% /note %}}

Worker B をインターネット上で公開する必要がない場合は、`wrangler.toml` の中で以下のように設定しておきます。
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

TypeScript を使っている場合は、Worker B 側のメソッドを呼び出すための型定義が必要です。
モノレポで開発している場合は、Worker B 側の `WorkerEntrypoint` を継承したクラスをそのまま `import` して参照すればよいのですが、ここでは別のリポジトリで開発しているという前提で、次のような型定義ファイルを作成しておきます。

{{< code lang="ts" title="src/types.d.ts" >}}
import { WorkerEntrypoint } from 'cloudflare:workers';

export declare class WorkerB extends WorkerEntrypoint {
  async fetch(req: Request): Promise<Response>;
  async add(a: number, b: number): Promise<number>;
}
{{< /code >}}

{{< code lang="ts" title="src/index.ts" hl_lines="10 14" >}}
import { WorkerB } from './types';

interface Env {
  WORKER_B: Service<WorkerB>;
}

export default {
  async fetch(request, env, ctx): Promise<Response> {
    // fetch の呼び出し
    const res = await env.WORKER_B.fetch(request);
    const text = await res.text(); // "Hello from B"

    // add の呼び出し
    const sum = await env.WORKER_B.add(1, 2); // 3

    return new Response(text + ', ' + sum);
  },
} satisfies ExportedHandler<Env>;
{{< /code >}}

お手軽スギィィィ！


動作確認
----

ローカルで Service Bindings のテストを行うときは、Worker A と Worker B それぞれのプロジェクト下で開発サーバー (**`wrangler dev`**) を起動します。
どちらから起動してもうまく連携してくれますが、Worker B から起動するのが自然です。

{{< code lang="console" title="Worker B の起動" >}}
$ cd ~/worker-b
$ pnpm dev  # wrangler dev
{{< /code >}}

{{< code lang="console" title="Worker A の起動" >}}
$ cd ~/worker-a
$ pnpm dev  # wrangler dev
{{< /code >}}

Worker A 側のコンソールで、`b` キーを押してブラウザを開いて、以下のように表示されれば成功です！

```
Hello from B, 3
```


デプロイ
----

Cloudflare Workers にデプロイする場合は、依存関係の問題が発生しないように Worker B の方から **`wrangler deploy`** コマンドでデプロイします。

```console
$ cd ~/worker-b
$ pnpm run deploy  # wrangler deploy
$ cd ~/worker-a
$ pnpm run deploy  # wrangler deploy
```

Worker B の方は `wrangler.toml` で `workers_dev = false` と設定したので、公開 URL は生成されません。
Worker A のデプロイが完了すると公開 URL が表示されるので、ブラウザでアクセスして動作確認してください。

できた〜 ٩(๑❛ᴗ❛๑)۶ わーぃ

