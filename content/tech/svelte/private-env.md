---
title: "Svelte 入門: 外部 API 用のキーを Private な環境変数で定義する ($env)"
url: "p/nmdte68/"
date: "2025-02-09"
tags: ["Svelte"]
---

{{% private %}}
- https://svelte.dev/docs/kit/routing#page-page.server.js
- https://svelte.dev/docs/kit/$env-dynamic-private
{{% /private %}}

何をするか？
----

Svelte (+SvelteKit) を使ったアプリケーションから、API キーが必要な外部サービスを呼び出す方法を紹介します。
例えば、サードパーティ API や、データベースサーバーにアクセスするときには、API キーが必要になることがあります。
API キーはクライアントサイドからは見えてはいけないので、プライベートな環境変数として定義し、サーバー側で実行されるコードからのみ参照するようにします。


Private 環境変数
----

Svelte アプリでは、**`PUBLIC_`** というプレフィックスが付いている環境変数は Public な環境変数、それ以外は Private な環境変数として扱われます。
つまり、環境変数名に `PUBLIC_` を付けなければ、サーバーサイドで実行されるコード（`+page.server.ts` や `+server.ts`）からのみ参照できるようになります。

本番環境用の環境変数は、デプロイ先のサービス上（Cloudflare Pages や Vercel）で設定する必要がありますが、開発サーバーで使用する環境変数は、**`.env`** ファイルで定義しておくことができます。

{{< code lang="ini" title=".env" >}}
MY_API_KEY="c3ytex9bsyed9zjgnbxhk2hninmd"
{{< /code >}}

`.env` ファイルには、API キーなどの機密情報を記述することになるため、Git などのバージョン管理ツールにコミットしてはいけません（おそらく `.gitignore` に `.env` ファイルが登録されているはずです）。
Git にコミットするのは、次のような参考ファイルだけにしておきます。

{{< code lang="ini" title=".env.example" >}}
MY_API_KEY="XXXXX"
{{< /code >}}

アプリ実行時に Private な環境変数を参照するには、SvelteKit が提供する **`$env/dynamic/private`** モジュールを使います。

{{< code lang="ts" title="src/routes/lib/server/sample.ts" >}}
import { env } from '$env/dynamic/private';
// ...
console.log(env.MY_API_KEY);
{{< /code >}}

これに似たモジュールに **`$env/static/private`** がありますが、こちらは名前の通り、ビルド時に（static に）環境変数の値を参照することになります。
多くのケースでは `dynamic` の方を使うことになると思いますが、環境に応じて使い分けてください。

| モジュール | 環境変数の参照タイミング |
| ---- | ---- |
| `$env/static/private` | **ビルド時に** 環境変数を参照する。 |
| `$env/dynamic/private` | **実行時に** 環境変数を参照する。 |

例えば、GitHub Actions でビルドして Cloudflare Pages にデプロイするようなケースでは、`static` の方を使った場合は GitHub Actions の環境変数が参照され、`dynamic` の方を使った場合は Cloudflare Pages の環境変数が参照されることになります。

ちなみに、クライアントサイドで実行される可能性のある `+page.svelte` や `+layout.svelte` から Private な環境変数を参照しようとすると、次のようなエラーが発生します。

```
Cannot import $env/dynamic/private into client-side code.
This could leak sensitive information.
```

`+page.svelte` の中では、`PUBLIC_` プレフィックスの付いた環境変数のみ参照できます（`$env/dynamic/public` や `$env/static/public` モジュールを使用します）。


準備： News API の API キーを取得する
----

以下のサンプルコードでは、API キーが必要な REST API の題材として [News API](https://newsapi.org/) を使用しています（1 日 100 リクエストまでは無料です）。
News API の API キーを取得して、プロジェクトのルートの `.env` ファイルに次のように記述しておきます。

{{< code lang="ini" title=".env" >}}
NEWS_API_KEY="XXXXXXXXXXXXXXXXXXXXXXXX"
{{< /code >}}


実装例： load() 関数からの参照
----

サーバーサイドで何らかのデータを取得して、そのデータをページコンポーネントに渡したいときは、**`+page.server.ts`** というファイルの `load()` 関数でデータのロード処理を実装します。

- 参考: [SvelteKit の load 関数でデータのロード処理を定義する](/p/wqt76qw/)

サーバーサイドで実行するロジックは、`+page.server.ts` に直接書くのではなく、**`src/lib/server/*.ts`** ファイルに切り出しておくとコードを再利用しやすくなります。
下記は、News API から最新のニュース (Top Headlines) を取得して JSON 形式で返す関数の例です。

{{< code lang="ts" title="src/lib/server/news.ts" >}}
import { env } from '$env/dynamic/private';

export async function fetchNews(): Promise<any> {
  const url = 'https://newsapi.org/v2/top-headlines?country=us';
  const res = await fetch(url, { headers: { 'x-api-key': env.NEWS_API_KEY } });
  return res.json();
}
{{< /code >}}

次に、ページコンポーネント用のデータ取得関数 `load()` を **`+page.server.ts`** で実装します。
上で定義した `fetchNews()` 関数を呼び出しています。

{{< code lang="ts" title="src/routes/news/+page.server.ts" >}}
import type { PageServerLoad } from './$types';
import { getNews } from '$lib/server/news';

export const load: PageServerLoad = async () => {
  return await getNews();
};
{{< /code >}}

あとは、同じ階層に置いたページコンポーネント **`+page.svelte`** で、取得したデータを表示するだけです。

{{< code lang="svelte" title="src/routes/news/+page.svelte" >}}
<script lang="ts">
  import type { PageProps } from './$types';

  let { data }: PageProps = $props();
</script>

<h1>Top headlines in the US</h1>
<pre>{JSON.stringify(data, undefined, 2)}</pre>
{{< /code >}}

開発サーバーを起動 (`pnpm dev --open`) して、`http://localhost:5173/news` にアクセスすると、News API から取得した JSON データが次のように表示されます。

```js
{
  "status": "ok",
  "totalResults": 37,
  "articles": [
    {
      "source": {
        "id": "the-washington-post",
        "name": "The Washington Post"
      },
      "author": "Natalie Allison",
      "title": "Trump’s Gaza proposal strikes fear in his new Arab American supporters - The Washington Post",
      "description": "Whether Trump can keep these new supporters could have significant implications on key upcoming statewide races, but also on the fragile coalition the party assembled to gain new ground.",
      "url": "https://www.washingtonpost.com/politics/2025/02/08/trump-gaza-arab-american-supporters/",
      "urlToImage": "https://www.washingtonpost.com/wp-apps/imrs.php?src=https://arc-anglerfish-washpost-prod-washpost.s3.amazonaws.com/public/BIR3GYEV3OE46QNNQQFXVXBJWY_size-normalized.jpg&w=1440",
      "publishedAt": "2025-02-08T14:32:41Z",
      "content": "DEARBORN, Mich. The half-dozen Lebanese American retirees sat socializing in the same spot they do every afternoon: the mall sofas on the ground floor of the Fairlane Town Center.\r\nAli Hammoud, 77, w… [+10811 chars]"
    },
    {
      "source": {
        "id": null,
        "name": "Financial Times"
      },
      "author": "Andres Schipani, Jyotsna Singh",
      "title": "Narendra Modi’s BJP wins in Delhi polls for first time in almost 3 decades - Financial Times",
      "description": "Indian prime minister’s party regains control of capital’s legislative assembly ahead of Trump meeting",
      "url": "https://www.ft.com/content/33f4e738-b336-4835-8426-f203ef06f002",
      "urlToImage": "https://www.ft.com/__origami/service/image/v2/images/raw/https%3A%2F%2Fd1e00ek4ebabms.cloudfront.net%2Fproduction%2F5c77b945-0663-480b-9232-0cea02bce06c.jpg?source=next-barrier-page",
      "publishedAt": "2025-02-08T14:21:37Z",
      "content": "FT newspaper delivered Monday-Saturday, plus FT Digital Edition delivered to your device Monday-Saturday.\r\n<ul><li></li>Weekday Print Edition<li></li>FT Weekend<li></li>FT Digital Edition<li>Global n… [+105 chars]"
    },
    {
      "source": {
        "id": null,
        "name": "BBC News"
      },
      "author": null,
      "title": "Hamas frees three Israeli hostages as Palestinian prisoners released - BBC.com",
      "description": "The three civilians were handed over on Saturday before Israel began releasing 183 Palestinians from prison.",
      "url": "https://www.bbc.com/news/articles/c4g9vyz747eo",
      "urlToImage": "https://ichef.bbci.co.uk/news/1024/branded_news/150b/live/5e742c10-e579-11ef-a819-277e390a7a08.jpg",
      "publishedAt": "2025-02-08T14:10:57Z",
      "content": "Raffi Berg &amp; Mallory Moench\r\n(L-R): Or Levy, Ohad Ben Ami and Eli Sharabi\r\nHamas has freed three Israeli hostages in Gaza while Israel released 183 Palestinian prisoners in the latest exchange as… [+6231 chars]"
    },
    //...
  ]
}
```

できたー ٩(๑❛ᴗ❛๑)۶ わーぃ


応用： Form アクション
----

ここまでの例では、`+page.server.ts` の `load()` 関数で、ページコンポーネントを表示するためのデータを取得していました。
この仕組みは、ページの読み込み時に、その URL に対応するデータを取得するのに適しています。
一方で、**ユーザーが画面上で何らかの操作をしたときにサーバーサイド処理** を実行したいこともあるでしょう。
例えば、フォームに入力した値に応じて情報を取得したい場合や、押したボタンに応じた処理を実行したい場合などです。
そのようなケースでは、SvelteKit の **Form アクション** という仕組みを使います。

- 参考: [Form actions • Docs • Svelte](https://svelte.jp/docs/kit/form-actions)
- 参考: [Forms / The form element • Svelte Tutorial](https://svelte.dev/tutorial/kit/the-form-element)

下記のコードは Svelte Tutorial からの抜粋ですが、このように `+page.server.ts` から **`actions`** オブジェクトを export することで、Form アクションを定義できます。
この Form アクションも、サーバーサイドでのみ実行されるため、API キーを必要とするデータベース処理などを行うことができます。
ここでは、ページ読み込み時に実行される `load()` 関数も一緒に定義しています。

{{< code lang="ts" title="src/routes/todos/+page.server.ts" hl_lines="13-20" >}}
import * as db from '$lib/server/database';  // コードは省略
import type { PageServerLoad, Actions } from './$types';

export const load: PageServerLoad = ({ cookies }) => {
  let id = cookies.get('userid');
  if (!id) {
    id = crypto.randomUUID();
    cookies.set('userid', id, { path: '/' });
  }
  return { todos: db.getTodos(id) };
}

export const actions = {
  default: async ({ cookies, request }) => {
    const data = await request.formData();
    const id = cookies.get('userid');
    const desc = data.get('description');
    db.createTodo(id, desc);
  }
} satisfies Actions;
{{< /code >}}

ページコンポーネントから Form アクションを呼び出すには、次のように `<form>` 要素で POST するだけです。

{{< code lang="svelte" title="src/routes/todos/+page.svelte" hl_lines="9-13" >}}
<script lang="ts">
  import type { PageProps } from './$types';

  let { data }: PageProps = $props();
</script>

<div class="centered">
  <h1>todos</h1>
  <form method="POST">
    <label>
      add a todo: <input name="description" autocomplete="off" />
    </label>
  </form>
  <ul class="todos">
    {#each data.todos as todo (todo.id)}
      <li>{todo.description}</li>
    {/each}
  </ul>
</div>

<style>
  <!-- ... -->
</style>
{{< /code >}}

このように実装すると、ユーザーが `input` 要素に何か入力して Enter キーを押したときに、Form アクションの `default` 関数が呼び出されるようになります。
`default` 関数には、サーバーサイドでのデータ更新処理しか記述されていないことに注目してください。
フォームの送信処理が行われると、ブラウザーの振る舞いとして同じページがリロードされるため、再度 `load()` 関数が実行されて最新のデータが表示されます。
なるほどねー。

