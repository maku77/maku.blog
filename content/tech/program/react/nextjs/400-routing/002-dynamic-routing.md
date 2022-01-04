---
title: "Next.js のダイナミックルーティング機能を利用する (getStaticPaths, getStaticProps, getServerSideProps)"
url: "/p/rdq3ep2"
date: "2021-05-05"
tags: ["Next.js"]
weight: 2
---

ダイナミックルーティングとは
----

Next.js では、__`pages/books/[id].tsx`__ のようなファイル名でページを作成すると、1 つのファイルで、

- `/books/001`
- `/books/002`
- `/books/003`

のようなパス (URL) によるアクセスをハンドルできます。
これを __ダイナミックルーティング (Dynamic Routes)__ 機能と呼びます。

Next.js のページコンポーネント (`/pages/xxx.tsx`) は、そのページのエントリポイント（ルートコンポーネント）となるため、通常の React コンポーネントとは違って、上位のコンポーネントから props 情報を渡すことができません。
そこで Next.js では、ページコンポーネントの実装ファイル内で __`getStaticProps`__ という関数を定義することで、ページコンポーネントに渡す props 情報を生成できるようにしています。
`getStaticProps` 内では、上記のような URL パラメータ情報（`/books/[id]` の `id` の部分の値）を取り出して、それを元に props 情報を生成できます。
この仕組みによって、Next.js のページコンポーネントは、1 つの `.tsx` ファイルで、複数のページ (.html) を生成できるようになっています。


ダイナミックルーティングの実装（SSG の場合）
----

静的ジェネレーション (SSG: Static Generation)、つまり Web サイトのビルド時にすべての HTML ファイルを生成してしまうには、あらかじめどのようなパラメーター（上記の例では `id`）でのアクセスが行われるかを把握した上で、各ページの内容を生成する必要があります。
これを実現するには、ページコンポーネントの実装ファイル (`pages/*.tsx`) で、次のような `async` 関数を実装して `export` します。

__getStaticPaths 関数__
: URL のパラメーター部分（上記の例では `id`）で指定可能な値を返すように実装します。言い換えると、プリビルドすべきページの一覧情報を Next.js に教えてあげるための実装です。この関数は通常、Web サイトのビルド時にだけ実行されますが、開発サーバー (`next dev`) 使用時はアクセス毎に呼び出されることに注意してください。

__getStaticProps 関数__
: 指定されたパラメーターに対応するデータを返すように実装します。この値がページコンポーネントに引数として渡されます。この関数は通常、Web サイトのビルド時にだけ実行されますが、`getStaticPath` の戻り値のプロパティ `fallback` を `true`、あるいは `'blocking'` に設定した場合は、アクセス時に呼び出される可能性があります（後述のフォールバックの説明を参照）。

次の例では、`/books/001`、`/books/002`、`/books/003` といった URL でアクセス可能な `books/[id]` ページを定義しています。

{{< code lang="tsx" title="pages/books/[id].tsx" >}}
import Head from 'next/head'
import React from 'react'
import { GetStaticPaths, GetStaticProps } from 'next'

// パスの構成要素を表す型 (/books/[id].tsx の id 部分など)
// (Note) Params という名前にすると曖昧なので、アクセス時のパスから抽出する
// 情報だということを示すために PathParams という名前にしています。
type PathParams = {
  id: string;
}

// ページコンポーネントに渡される props の型
// (Note) ページコンポーネント用の props であることを意識するために、
// 一般的な Props ではなく、PageProps という名前にしています。
type PageProps = {
  title: string;
  author: string;
}

// 事前生成するページのパス（URL のパラメータ部分）のリストを返します。
// eslint-disable-next-line @typescript-eslint/require-await
export const getStaticPaths: GetStaticPaths<PathParams> = async () => {
  // /books/001、/books/002、/books/003 のページを事前生成するには、
  // 次のように paths プロパティの値を設定して返します。
  // 本来は id のリストを外部 API（getBookList など）で取得します。
  return {
    paths: [
      { params: { id: '001' } },
      { params: { id: '002' } },
      { params: { id: '003' } }
    ],
    fallback: false  // 上記以外のパスでアクセスした場合は 404 ページにする
  }
}

// URL のパラメータ情報（プレースホルダー部分に指定された値）をもとに、
// ページコンポーネントに渡す props データを生成します。
// context.params プロパティでこれらのパラメータを参照できるので、
// その値に応じて props データを生成して返すように実装します。
// eslint-disable-next-line @typescript-eslint/require-await
export const getStaticProps: GetStaticProps<PageProps> = async context => {
  // ファイル名が [id].tsx なので id パラメーターを取り出すことができる
  const { id } = context.params as PathParams

  // 本来はここで getBook(id) のように API を呼び出してデータを取得する
  const props: PageProps = {
    title: `Title-${id}`,
    author: `Author-${id}`
  }

  // ページコンポーネントに渡す PageProps オブジェクトを
  // props プロパティに設定したオブジェクトを返す
  return { props }
}

// ページコンポーネントの実装
const BookPage: React.FC<PageProps> = ({ title, author }: PageProps) => {
  return <>
    <Head>
      <title>{title} の詳細ページ</title>
    </Head>
    <ul>
      <li>タイトル: {title}</li>
      <li>著者: {author}</li>
    </ul>
  </>
}
export default BookPage
{{< /code >}}


フォールバック制御
----

### fallback プロパティ

上記の例では、`getStaticPaths` 関数の戻り値の __`fallback`__ プロパティに `false` をセットしていますが、この値は、__想定外のパラメーターを指定された場合のフォールバック方法__ を設定するために使用します。

`fallback: false`
: paths プロパティで指定したパラメーター以外でのアクセス時に、__404 ページ (`pages/404.tsx`)__ を返します。

`fallback: true`
: paths プロパティで指定したパラメーター以外でのアクセス時に、サーバーサイドで `getStaticProps` を呼び出して動的にページを生成します。それ以降のアクセスは、そのページを返します。この機能を利用する場合は、Next.js サーバーで Web サイトをホスティングしている必要があります（`next export` で HTML エクスポートした場合は動作しません）。サーバーサイドでのページ生成中は、Web ブラウザ側には瞬間的に「フォールバックページ」が表示されることになります。フォールバック表示中かどうかを判断するには、`useRouter` フックを利用して `isFallback` 情報を参照します（後述）。

`fallback: 'blocking'`
: `true` を指定した場合とほぼ同じですが、サーバーサイドでの HTML 生成が終わってから初めて Web ブラウザにレスポンスが返されるところが異なります（なのでブロッキングという名前になっています）。`true` を指定した場合は、フォールバック表示（Loading 表示など）を行うことができますが、`'blocking'` を指定した場合は、フォールバック表示は行えません（実装は楽ですがユーザーへのレスポンスが遅くなります）。

### getStaticProps でデータを生成できないときの処理

このフォールバック機能を有効にしているとき（`fallback: true` あるいは `fallback: 'blocking'` のとき）、`getStaticProps` は Web サイトへのアクセス時に、未知のパラメーター（上記の例の場合は `id` の値）を伴って呼び出される可能性があります。
それに対応する props オブジェクト (`PageProps`) を生成できない場合は、次のいずれかの対応を行う必要があります。

- データが空であることを示すオブジェクト（空オブジェクトとか）を返して、ページコンポーネント内で適切にハンドルする
- 404 コード（と 404 ページ）を返す（`{ notFound: true }` を返す）
- 別のパスにリダイレクトする（`{ redirect: ... }` を返す）

例えば、次の例では、対応する props オブジェクトを生成できない場合に、空のオブジェクト `{}` を返すように実装しています。

{{< code lang="ts" >}}
export const getStaticProps: GetStaticProps<PageProps | {}> = async context => {
  // ... context.params.id に対応する props を提供できない場合
  return { props: {} }
}

const BookPage: React.FC<PageProps> = ({ title, author }: PageProps) => {
  if (title == undefined) {
    return <div>NOT FOUND</div>
  }
  // ...
}
{{< /code >}}

潔く、404 ページを表示するのであれば、`getStaticProps` 関数の戻り値として `{ notFound: true }` を返すだけで OK です。

{{< code lang="ts" >}}
export const getStaticProps: GetStaticProps<PageProps> = async context => {
  // ... props を提供できない場合
  return { notFound: true }
}
{{< /code >}}

{{< note >}}
`getStaticPaths` の戻り値で `fallback: false` と設定している場合は、上記のような分岐処理は必要ありません。
パラメーターとして未知の値が指定された場合に、自動的に 404 が返されます（`getStaticProps` が呼び出されることがありません）。
{{< /note >}}

不正なパラメーターを指定された時に別の URL へリダイレクトしてしまいたい場合は、戻り値で `redirect` プロパティを指定します。

{{< code lang="ts" >}}
return {
  redirect: { destination: '/', permanent: false }
}
{{< /code >}}

### フォールバック時のキャッシュの有効時間

フォールバックを有効にしているとき (`fallback: true`)、未知のパラメーターでページアクセスがあると、Next.js サーバー側で `getStaticProps` 関数が呼び出されて、動的にページ生成が行われます。
このとき、次回以降のアクセスのために、生成されたページはキャッシュされるわけですが、このキャッシュの有効期間は `getStaticProps` の戻り値の __`revalidate`__ プロパティで秒単位で設定することができます。

{{< code lang="ts" >}}
export async function getStaticProps() {
  const res = await fetch('https://.../posts')
  const posts = await res.json()

  return {
    props: { posts },
    revalidate: 60 * 10  // 10 分間キャッシュ
  }
}
{{< /code >}}

指定された秒数を経過後に、同じ URL にアクセスがあったときは、再度 `getStaticProps` が呼び出されて、最新データでページが再構築されます。
逆に、`revalidate` プロパティを設定しなかった場合は、最初に生成されたページがずーっと使われることになります。
`revalidate` プロパティを指定してサーバーサイドで定期的にページを再構築することを、Next.js では __ISR: Incremental Static Regeneration__ と呼んでいます。


### フォールバック時の一時表示（フォールバックページ）

フォールバック有効時（`fallback: true` のとき）、サーバーサイドで動的なページ生成が行われている最中（`getStaticProps` 実行中）は、クライアント側には「フォールバックページ」が表示されることになります。
フォールバックページでは、パラメーターとして渡される `props` は空っぽになるので、代わりに適切な Loading 表示などを行う必要があります。
現在のページがフォールバック中であるかどうかを調べるには、__`useRouter`__ フックを使って、__`isFallback`__ の値をチェックします。

{{< code lang="tsx" >}}
import { useRouter } from 'next/router'

// ページコンポーネントの実装
const BooksPage: React.FC<PageProps> = ({ title, author }: PageProps) => {
  const router = useRouter()
  if (router.isFallback) {
    return <div>Loading...</div>
  }

  // ...
}
{{< /code >}}

- 参考: [Fallback pages - Next.js](https://nextjs.org/docs/basic-features/data-fetching#fallback-pages)


ダイナミックルーティングの実装（SSR の場合）
----

すべてのページを Web サーバーへのアクセス時に動的に生成することを、サーバーサイドレンダリング (SSR: Server-side Rendering) と呼びます。
ダイナミックルーティング機能を SSR で使用するには、`getStaticPaths` 関数や `getStaticProps` 関数の代わりに次の関数を実装します。

__getServerSideProps 関数__
: Web サーバー（Next.js サーバー）へのアクセス時に呼び出されます。`context.params` を参照すると URL で指定されたパラメーター (`id`) を取得できるので、それに対応するデータを返すように実装します。このデータはページコンポーネントの引数として渡されます。

実装方法は `getStaticProps` と同様です。

{{< code lang="tsx" title="pages/books/[id].tsx（抜粋）" >}}
export const getServerSideProps: GetServerSideProps<PageProps> = async context => {
  const { id } = context.params as PathParams

  // 本来はここで getBook(id) のように API を呼び出してデータを取得する
  const props: PageProps = {
    title: `Title-${id}`,
    author: `Author-${id}`
  }

  return { props }
}
{{< /code >}}

この関数は Web サーバーへのアクセス時に呼び出されるため、必ず Web サイトを Next.js サーバーでホスティングする必要があります（`next export` で静的な HTML ファイルとしてエクスポートした場合は動作しません）。
Next.js アプリのホスティングサービスとしては、[Vercel](https://vercel.com/) が有名です。

SSR の仕組み (`getServerSideProps`) を使うと、Web ページへのアクセス時に毎回ページ生成が行われるため、クライアントへのレスポンスはどうしても遅くなります。
常に最新の情報を使ってページ生成を行う必要がないのであれば、できるだけ SSG: Static Generation によるレンダリングを行うべきです（`getServerSideProps` ではなく `getStaticProps` を実装する）。
`getStaticProps` でも、戻り値の `revalidate` プロパティを設定すれば、定期的にページを再構築できます (ISR)。

