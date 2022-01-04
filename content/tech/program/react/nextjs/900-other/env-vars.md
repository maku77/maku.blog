---
title: "Next.js で環境変数を扱う (.env, NEXT_PUBLIC, NODE_ENV)"
url: "/p/gbpeyov"
date: "2021-06-28"
tags: ["Next.js"]
---

Next.js アプリ内での環境変数の振る舞い
----

### process.env の振る舞い

Node.js の __`process.env`__ による環境変数の参照が有効なのは、基本的には次のようなサーバーサイドで実行されるコード内のみです。

- ビルド時あるいはアクセス時に呼び出される `getStaticPaths` や `getStaticProps`
- 必ずアクセス時に呼び出される `getServerSideProps`
- 必ずアクセス時に呼び出される API ルートのハンドラ関数 (`handler`)

{{< code lang="tsx" title="src/pages/sample.tsx" >}}
export const getStaticProps: GetStaticProps<PageProps> = async context => {
  // このコードはビルド時に実行されるので環境変数を参照できる
  console.log(process.env.VAR_NAME)
  return { props: {} }
}
{{< /code >}}

### NEXT_PUBLIC プレフィックス

ただし、例外として、__`NEXT_PUBLIC_`__ で始まる環境変数を `process.env.NEXT_PUBLIC_XXX` のように参照すると、`next build` によるビルド時に変数値がインライン展開されるので、クライアントサイドで実行されるコード（コンポーネントの実装内）から参照できます。

{{< code lang="tsx" title="src/pages/sample.tsx" >}}
const SamplePage: FC = () => {
  return <>
    <p>Public env: {process.env.NEXT_PUBLIC_ANALYTICS_ID}</p>
    <p>Private env: {process.env.API_SECRET_KEY} （必ず空っぽ）</p>
  </>
}
{{< /code >}}

上記のようにすると、`process.env.NEXT_PUBLIC_ANALYTICS_ID` の部分には、ビルド時の環境変数 `NEXT_PUBLIC_ANALYTICS_ID` の値がそこに埋め込まれ、`process.env.API_SECRET_KEY` の方は必ず `undefined` になります（何も表示されない）。

サーバーサイドでしか参照しない環境変数（シークレットキーなど）には、`NEXT_PUBLIC_` プレフィックスを付けないように注意してください。


.env (.env.local) ファイル
----

Next.js サーバーは、デフォルトでプロジェクトルートに配置した __`.env`__ や __`.env.local`__ といった名前のファイルを読み込んで、`process.env.XXX` で参照可能な状態にしてくれます。
ローカル開発中に、一時的にテスト用のリソースサーバーに繋ぎたいときなどに便利です。

{{< code title=".env.local" >}}
DB_HOST=localhost
DB_USER=myuser
DB_PASS=mypassword
{{< /code >}}

これらの設定ファイルの中では、次のように `$VAR` という形の変数展開を行えるようになっています。

{{< code >}}
HOSTNAME=localhost
PORT=8080
HOST=http://$HOSTNAME:$PORT
{{< /code >}}

`.local` サフィックスが付いている方のファイルは、「Git などにコミットしないファイルですよ」という Next.js での取り決めですね。
Next.js は他にも実行環境に応じて色々な名前の環境設定ファイルを読み込むようになっています。
下記は、どのファイルがどの環境で読み込まれるかの一覧です。

<table>
  <thead>
    <tr>
      <th>ファイル</th>
      <th>本番環境<br>(next start)</th>
      <th>開発環境<br>(next dev)</th>
      <th>テスト環境<br>（jest など）</th>
      <th>Git コミット<br>するか？</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th style="text-align: left"><code>.env</code></th>
      <td style="text-align: center">&#x2714;(4)</td>
      <td style="text-align: center">&#x2714;(4)</td>
      <td style="text-align: center">&#x2714;(4)</td>
      <td style="text-align: center">&#x2714;する</td>
    </tr>
    <tr>
      <th style="text-align: left"><code>.env.local</code></th>
      <td style="text-align: center">&#x2714;(2)</td>
      <td style="text-align: center">&#x2714;(2)</td>
      <td style="text-align: center">&#x2714;(2)</td>
      <td style="text-align: center">しない</td>
    </tr>
    <tr>
      <th style="text-align: left"><code>.env.production</code></th>
      <td style="text-align: center">&#x2714;(3)</td>
      <td></td>
      <td></td>
      <td style="text-align: center">&#x2714;する</td>
    </tr>
    <tr>
      <th style="text-align: left"><code>.env.production.local</code></th>
      <td style="text-align: center">&#x2714;(1)</td>
      <td></td>
      <td></td>
      <td style="text-align: center">しない</td>
    </tr>
    <tr>
      <th style="text-align: left"><code>.env.development</code></th>
      <td></td>
      <td style="text-align: center">&#x2714;(3)</td>
      <td></td>
      <td style="text-align: center">&#x2714;する</td>
    </tr>
    <tr>
      <th style="text-align: left"><code>.env.development.local</code></th>
      <td></td>
      <td style="text-align: center">&#x2714;(1)</td>
      <td></td>
      <td style="text-align: center">しない</td>
    </tr>
    <tr>
      <th style="text-align: left"><code>.env.test</code></th>
      <td></td>
      <td></td>
      <td style="text-align: center">&#x2714;(3)</td>
      <td style="text-align: center">&#x2714;する</td>
    </tr>
    <tr>
      <th style="text-align: left"><code>.env.test.local</code></th>
      <td></td>
      <td></td>
      <td style="text-align: center">&#x2714;(1)</td>
      <td style="text-align: center">しない</td>
    </tr>
  </tbody>
</table>

括弧の中の数値は、優先度を示しています（1が最大）。
複数のファイルに同じ環境変数が定義されている場合は、`.local` が付いたものが優先的に使われます。
例えば、本番環境 (`next start`) では、`.env.production.local` ＞ `.env.local` ＞ `.env.production` ＞ `.env` という優先度になります。

末尾に `.local` が付いたファイルには秘密鍵などの情報を記述することを想定しているので、間違えて Git にコミットしないように __`.gitignore`__ ファイルに登録しておきましょう。

{{< code title=".gitignore" >}}
# local env files
.env.local
.env.production.local
.env.development.local
.env.test.local
{{< /code >}}


config.ts に環境変数の値を反映させる
----

アプリ全体のコンフィグ用に `config.ts` のようなファイルを作成しているケースは多いと思います。
この中で環境変数を参照するようにしておくと、環境 (production or development) に応じてリソースを使い分けるといったことが簡単にできます。
これは、Next.js が前述のように複数のファイルを読み分けてくれるおかげです。

{{< code lang="ts" title="src/libs/config.ts" >}}
export const config = {
  userPoolId: process.env.NEXT_PUBLIC_USER_POOL_ID ?? 'default-user-pool',
  identityPoolId: process.env.NEXT_PUBLIC_IDENTITY_POOL_ID ?? 'default-id-pool',
}
{{< /code >}}

ちなみに、`??` という演算子は Nullish coalescing という仕組みで、左側が `undefined` だったときに右側の値が使われるようになります。
つまり、上記のように記述しておくと、環境変数が設定されていればその値を使い、設定されていなければ後ろに書いたデフォルト値を使う、という振る舞いになります。

- 参考: [TypeScript で undefined/null をうまく扱う (nullish coalescing (??), optional chaining (?.))](/p/5oyaju5)

例えば、本番環境で接続先の情報を変えたいときは、次のような環境変数ファイルを作成するだけで済みます。
`next start` で Next.js サーバーを起動したときや、Vercel でホスティングするときはこの設定が使われるようになります。

{{< code lang="ts" title=".env.production" >}}
NEXT_PUBLIC_USER_POOL_ID=xxxxxxxxxxxxx
NEXT_PUBLIC_IDENTITY_POOL_ID=yyyyyyyyyyyyy
{{< /code >}}


NODE_ENV 環境変数で開発環境と本番環境の動作を切り替える
----

Next.js サーバー実行時の __NODE_ENV__ 環境変数の値は、サーバーの起動方法によって次のように設定されるようになっています。

| Next.js サーバーの起動方法 | process.env.NODE_ENV の値 |
| ---- | ---- |
| `next dev` | __`development`__ |
| `next start` | __`production`__ |

これを利用すると、現在の実行環境に応じて振る舞いを変更できます。
次の例では、開発環境 (`next dev`) ではローカルのリソースファイルを使い、本番環境 (`next start`) ではインターネット上のリソースファイルを使うようにしています。

{{< code lang="ts" >}}
function getImageUrl(filename: string) {
  if (process.env.NODE_ENV === 'production') {
    // 本番環境ではインターネット上のファイルを参照
    return `https://example.com/img/${filename}`
  } else {
    // 開発環境では public ディレクトリ以下のファイルを参照
    return `/img/${filename}`
  }
}
{{< /code >}}

ほとんどのユースケースは上記のような切り替えでカバーできるのですが、時々 `next build` によるビルド後のサイト確認 (`next start`) でも、ローカルリソースを参照したいことがあります（ファイル転送による従量課金が気になる場合など）。
このようなケースで、`next start` コマンド実行時に強引に `NODE_ENV` 環境変数の値を `development` に変えようとしても、内部で `production` に上書きされてしまってうまくいきません。

{{< code title="これは無意味 (>_<)" >}}
$ NODE_ENV=development next start
{{< /code >}}

こういったケースでは、[next.config.js の env コンフィグ](https://nextjs.org/docs/api-reference/next.config.js/environment-variables) 機能を使って、ビルド時に独自の環境変数を設定してやるとうまくいきます。

{{< code lang="js" title="next.config.js" >}}
const isProd = process.env.NODE_ENV === 'production'

module.exports = {
  env: {
    isProd: isProd,
  }
}
{{< /code >}}

`next.config.js` ファイルはビルド時に処理されるので、上記のように記述しておくと、__`process.env.isProd`__ の値が、`next build` 実行時の `NODE_ENV` 変数の値によって確定します（正確には、コード内部の `process.env.isProd` という部分が具体的な値に置換されます）。
プログラム内で `process.env.NODE_ENV` を参照していた部分は、次のように `process.env.isProd` を参照するように書き換えます。

{{< code lang="ts" >}}
function getImageUrl(filename: string) {
  if (!!process.env.isProd) {
    return `https://example.com/img/${filename}`
  } else {
    return `/img/${filename}`
  }
}
{{< /code >}}

あとは、ビルド方法や実行方法により、`process.env.NODE_ENV` や `process.env.isProd` の値を次のように変化させることができます（コンポーネント内のコードで参照したときの値を示していることに注意してください）。

| ビルド／実行方法 | NODE_ENV の値 | isProd の値 |
| ---- | ---- | ---- |
| `$ next build`<br>`$ next start` | `'production'` | `true` |
| `$ NODE_ENV=development next build`<br>`$ next start` | `'production'` | `false` |
| `$ next dev` | `'development'` | `false` |

実際には、`NODE_ENV` 環境変数を `isProd` の値の制御のために使うのは混乱を招くので避けた方がいいかもしれません。
上記の結果を見るとわかるように、ビルド時に `NODE_ENV=development` と指定しても、ランタイムでは `NODE_ENV` の値は `production` になってしまいます。
リソースの参照先を柔軟に切り替えたいときは、次のように独自の環境変数 (`IS_LOCAL`) を用意するようにした方がよいでしょう。

{{< code lang="js" title="next.config.js" >}}
const isProd = process.env.NODE_ENV === 'production'
const isLocal = process.env.IS_LOCAL != undefined || !isProd

module.exports = {
  env: {
    isLocal,
    imageUrlPrefix: isLocal ? '/img/' : 'https://example.com/img/'
  },
}
{{< /code >}}

{{< code lang="ts" title="ローカルファイルを参照したいときのビルド方法" >}}
$ IS_LOCAL=1 next build
$ next start
{{< /code >}}

上記の `next.config.js` の例では、環境によってリソースファイルのプレフィックス情報 (`process.env.imageUrlPrefix`) を切り替えているので、`getImageUrl` 関数は次のように簡潔に記述できます。

{{< code lang="ts" title="使用例" >}}
function getImageUrl(filename: string) {
  return (process.env.imageUrlPrefix ?? '') + filename
}
{{< /code >}}

