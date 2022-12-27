---
title: "Deno Deploy の例: JSON ファイルを読み込んで返す REST API を作る (oak)"
url: "p/zt3birz/"
date: "2022-12-27"
tags: ["Deno"]
---

何をするか？
----

Deno Deploy を使うと、Web API を簡単にデプロイすることができます。
ここでは、よくある例として、サーバー側で JSON ファイルを読み込んで、その内容をレスポンスとして返す REST API を作ってみます。
ここでは、次のような関数やモジュールを使用します。

- [Deno.readTextFile](https://deno.land/api?s=Deno.readTextFile)
  - テキストファイルを読み込むために使用する Deno の標準 API です。
- [x/oak モジュール](https://deno.land/x/oak)
  - HTTP サーバーを作るときに使えるミドルウェアフレームワークです。リクエストのパスに応じて、処理を簡単に振り分けることができます。Node の express と同じように使えます。


前提条件
----

Deno Deploy の基本的な使い方は下記ページを参考にしてください。

- 参考: [Deno Deploy で Deno のサーバープログラムを公開する](/p/phz7fo3/)

後述の記事では、デプロイ用の __`deployctl`__ コマンドを使用するので、上記ページに従って先にインストールしておいてください。


JSON ファイル
----

サーバー側で読み込む、サンプルの JSON ファイルを用意しておきます。

{{< code lang="json" title="data/books.json" >}}
[
  {
    "id": "1",
    "title": "Title 1"
  },
  {
    "id": "2",
    "title": "Title 2"
  },
  {
    "id": "3",
    "title": "Title 3"
  }
]
{{< /code >}}

3 冊の書籍データが格納されているという想定です。
実際のアプリケーションでは、RDB や mongoDB などのデータベースから情報取得することになると思います。


サーバーの実装
----

先に、`data/books.json` ファイルの情報を読み込んで返す `getAllBooks` 関数と `getBook` 関数を作成しておきます。
テキストファイルは、__`Deno.readTextFile`__ 関数で読み込むことができます。

{{< code lang="ts" title="booksDb.ts" >}}
// 読み込むデータの型情報
type Book = {
  id: string;
  title: string;
};

// サンプルの JSON ファイルを読み込んでおく
const booksJson = await Deno.readTextFile("./data/books.json");
const books = JSON.parse(booksJson) as Book[];

/** 全ての書籍データを取得します */
export function getAllBooks() {
  return books;
}

/** 指定した ID の書籍データを取得します */
export function getBook(id: string | undefined): Book | undefined {
  return books.find((b) => b.id === id);
}
{{< /code >}}

下記は、サーバーのエントリポイントとなるファイルで、oak の `Router` クラスを使って、次のようにリクエストを振り分けています。

- `/books` ... すべての書籍情報を返す。
- `/books/1` ... 指定された ID (=1) に対応する書籍情報を返す。

{{< code lang="main.ts" >}}
import { Application, Router } from "https://deno.land/x/oak@v11.1.0/mod.ts";
import { getAllBooks, getBook } from "./booksDb.ts";

// Router を作成して、リクエストに応じて処理を振り分ける
const router = new Router();
router
  .use((ctx, next) => {
    // レスポンスのデフォルトの content-type を application/json にする
    ctx.response.type = "json";
    next();
  })
  .get("/books", (ctx) => {
    ctx.response.body = getAllBooks();
  })
  .get("/books/:id", (ctx) => {
    const book = getBook(ctx?.params?.id);
    ctx.response.body = book ?? { error: "Not Found" };
  });

// Create Application and register middlewares
const app = new Application();
app.use(router.routes());
app.use(router.allowedMethods());

// Start the server
await app.listen({ port: 8000 });
{{< /code >}}


ローカルでのテストとデプロイ
----

サーバーの実装が終わったら、__`deno run`__ でローカル実行して動作確認します。
ネットワークアクセスと、ファイルアクセスがあるので、`--allow-net` と `--allow-read` を指定して起動する必要があります（面倒であれば `-A` であらゆるアクセスを許可しても OK）。

```console
$ deno run --allow-net --allow-read main.ts
```

Web ブラウザや `curl` コマンドでアクセスして、正しく JSON データが返ってくるか確認します。

```console
$ curl localhost:8000/books
[{"id":"1","title":"Title 1"},{"id":"2","title":"Title 2"},{"id":"3","title":"Title 3"}]

$ curl localhost:8000/books/1
{"id":"1","title":"Title 1"}
```

うまく動いているようなので、__`deployctl deploy`__ コマンドで、Deno Deploy にデプロイします。

```console
$ deployctl deploy --project your-project main.ts
```

デプロイ時に、カレントディレクトリにある `data/books.json` ファイルも同時に転送してくれるので、Deno Deploy サービス上でも正しく JSON ファイルを読み込むことができます。
デプロイが完了したら、発行された URL でアクセスしてみます。

```console
$ curl https://your-project-a93nne0yv79j.deno.dev/books
[{"id":"1","title":"Title 1"},{"id":"2","title":"Title 2"},{"id":"3","title":"Title 3"}]

$ curl https://your-project-a93nne0yv79j.deno.dev/books/1
{"id":"1","title":"Title 1"}
```

バッチシ動きました！ ٩(๑❛ᴗ❛๑)۶ わーぃ

