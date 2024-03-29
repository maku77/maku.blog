---
title: "Deno Deploy で Deno のサーバープログラムを公開する"
url: "p/phz7fo3/"
date: "2022-12-24"
lastmod: "2024-03-08"
tags: ["Deno"]
weight: 100
changes:
  - 2024-03-08: deployctl コマンドのインストール方法を更新。
---

Deno Deploy とは？
----

[Deno Deploy](https://deno.com/deploy) を使うと、Deno で作成したサーバープログラムを簡単に公開することができます。
Deno Deploy はエッジサーバーで動作する Deno 実行環境を提供しており、ゼロコンフィグでデプロイすることができます。

- TypeScript コードで記述したプログラムがそのまま動きます（Deno なので）
- CLI ツール ([deployctl](https://github.com/denoland/deployctl)) や GitHub 統合、GitHub Actions で簡単にデプロイできます
- デプロイは数秒で終わります（GitHub 連携による Deno Deploy へのデプロイは、[Vercel や Cloudflare へのデプロイより 10 倍以上速い](https://deno.com/blog/fastest-git-deploys-to-the-edge)）
- サーバー上で公開されている `.ts` ファイルを直接デプロイすることができます
- プレビューサイト用の URL が自動生成されます（もちろん SSL 対応）
- 1 日に 10 万回までの呼び出しであれば無料で利用できます（それ以上は$10/月からの有料プラン）


deployctl のインストール
----

ローカルにある `.ts` ファイルを Deno Deploy へデプロイするには、CLI コマンドの [deployctl](https://github.com/denoland/deployctl) を使用します。
このコマンドは、`deno` コマンドでインストールできます。

{{< code title="deployctl のインストール（deno 1.41 で確認済）" >}}
$ deno install -Arf jsr:@deno/deployctl
{{< /code >}}

インストールに失敗する場合は、`deno upgrade` で `deno` 自体を更新してから実行してみてください。

`~/.deno/bin/` に `deployctl` コマンドが配置されれば成功です。

```console
$ deployctl --version
deployctl 1.11.0
```


簡単なサーバープログラムを用意する
----

Deno Deploy にデプロイするための、簡単なサーバープログラムを用意しておきます。
Deno の標準ライブラリ [std/http](https://deno.land/std/http) を使うと、簡単に Web サーバーをプログラムを作成できます。

{{< code lang="ts" title="mod.ts" >}}
function handler(_req: Request): Response {
  return new Response("Hello, World!", {
    headers: { "content-type": "text/plain" },
  });
}

Deno.serve(handler);
{{< /code >}}

{{< code lang="console" title="動作確認" >}}
$ deno run --allow-net mod.ts
Listening on http://localhost:8000/
{{< /code >}}

`serve` 関数はデフォルトで 8000 番ポートで待ち受けを行いますが、Deno Deploy を使う場合は、このポートを調整する必要はありません。
Deno Deploy は自動的に 80 番ポートで Web サーバーを公開してくれます。

これで、デプロイ用のファイルの準備ができました。


Deno Deploy にデプロイしてみる
----

### Sign Up

Deno Deploy を使用するには、GitHub アカウントで Sign Up する必要があります。
[Deno Deploy のサイト](https://dash.deno.com/signin) から Sign Up ボタンを押して、GitHub アカウントと Deno Deploy を連携すると、新しいプロジェクトを作成できるようになります。

### アクセストークンの発行

`deployctl` コマンドでデプロイするときに、アクセストークンが必要になるので、Deno Deploy の設定ページから [アクセストークンを発行](https://dash.deno.com/account#access-tokens) しておきます。
発行したアクセストークンは、環境変数 __`DENO_DEPLOY_TOKEN`__ に設定しておきます。

```bash
export ddp_3MkWxJK9NLRkbOZjObTDO1VWotofhR3xbGPz
```

### デプロイする

ローカルにある `.ts` ファイルをデプロイするには、__`deployctl deploy`__ コマンドを使用します。
プロジェクト名は、Deno Deploy 上で作成したプロジェクト名に合わせてください。

```console
$ deployctl deploy --project=hello mod.ts
 ✔ Project: hello
 ℹ Uploading all files from the current dir (/Users/maku/hello-server)
 ✔ Found 2 assets.
 ✔ Uploaded 2 new assets.
 ✔ Deployment complete.

View at:
 - https://hello-sgm5da8zw56g.deno.dev
```

上記のように、自動的にプレビュー URL が割り当てられるので、Web ブラウザーなどでアクセスして `Hello, World!` と表示されれば成功です。

٩(๑❛ᴗ❛๑)۶ わーぃ


Preview Deployment と Production Deployment
----

### Preview Deployment

`deployctl deploy` コマンドでのデプロイは、デフォルトでは __Preview Deployment__ が生成されるようになっており、次のような形式のプレビュー URL が割り当てられます。

```
{project_name}-{deployment_id}.deno.dev
```

例えば、`https://hello-sgm5da8zw56g.deno.dev` のような、ランダムな `{deployment_id}` の混ざった URL になっています。
`{deployment_id}` には、デプロイごとに新しい ID が割り当てられ、Deno Deploy の中でそれぞれ異なる Deployment として管理されます。
これらの Deployment は、いわゆるステージング環境であり、サービス公開前のテストに使用できます（URL がバレると自由にアクセスされてしまいますが）。

### Production Deployment

本番環境用のデプロイ (__Production Deployment__) を行いたい場合は、__`--prod`__ オプションを付けてデプロイを実行します。

```console
$ deployctl deploy --project=hello --prod mod.ts
```

Production Deployment を実行すると、アクセス用の URL は、`https://hello.deno.dev` のようなシンプルなものになります（独自ドメインを割り当てることもできます）。

すでにデプロイ済みの Preview Deployment を、Production Deployment に格上げすることもできます。
Deno Deploy のサイトの `Deployments` パネルから対象の Deployment を選択し、__`Promoto to Production`__ を選択すれば OK です。


deno task 用にタスク定義しておく
----

Deno のタスクランナー (`deno task`) で、デプロイ用のタスクを定義しておくと便利です。
タイプ数はそんなに変わらないかもしれませんが、プロジェクトで実行可能なタスクを `deno task` で一覧表示できるようにしておくのはよいプラクティスです。

{{< code lang="js" title="deno.jsonc" >}}
{
  "tasks": {
    "dev": "deno run -A --watch main.ts",
    "lint": "deno lint",
    "fmt": "deno fmt",

    // Preview deployment
    "deploy": "deployctl deploy --project=hello main.ts",

    // Production deployment
    "deploy:prod": "deno task deploy --prod",

    // Deploy by dry-run
    "deploy:dry": "deno task deploy --dry-run"
  }
}
{{< /code >}}

これで、次のようにデプロイできるようになります。

```console
$ deno task deploy       # Preview 環境へのデプロイ
$ deno task deploy:prod  # Production 環境へのデプロイ
```

参考: [Deno のタスクランナーの使い方 (deno task)](/p/ho4gs5h/)

