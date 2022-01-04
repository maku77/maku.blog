---
title: "Next.js のプリレンダリング機能を使用する (getStaticProps)"
linkTitle: ""
url: "/p/iv4agnt"
date: "2021-05-04"
tags: ["Next.js"]
weight: 1
---

Pre-rendering とは
----

ブログの記事一覧ページなどを生成する場合、なんらかの API で取得した値をもとにページのコンテンツを生成する必要があります。
例えば、次のように取得した値を使ってページを生成することになります。

- Web API で取得した値
- データベースのクエリ結果
- ローカルファイルの内容や、ファイルの一覧情報

Next.js には、Web サイトのビルド時や、Web サーバーへのアクセス時にこういった API を呼び出して、HTML コンテンツを生成する __Pre-rendering__ 機能が備わっています。
Pre-rendering 機能は次の 2 種類があり、どちらか一方を使うこともできますし、両方を組み合わせて使うこともできます。

__SSG: Static Generation（静的サイトジェネレーション）__
: Web サイトのビルド時に HTML ファイルを生成します。Web サーバーは静的な HTML ファイルを返すだけでよいので、パフォーマンスが非常に高くなります。すべてのページを事前に列挙できるのであれば、できるだけこの SSG を使って静的に HTML 生成してしまうことが推奨されています。静的な HTML ファイルをホスト可能なサーバー（GitHub Pages など）があれば、Web サイトを公開できます。

__SSR: Server-side Rendering（サーバーサイドレンダリング）__
: クライアントが Web サーバーにアクセスしたときに、サーバーサイドで動的に HTML を生成します。この仕組みを使うと、日々増減するデータを扱いやすくなりますが、Web サーバーとして Next.js サーバーを稼働させておく必要があります。感覚的には、PHP サーバーなどが動作しているイメージに近いです。

ちなみに、純粋に React.js のみを使用した場合とはどう違うのでしょうか？
React.js 自体には Pre-rendering 機能は備わっておらず、主に SPA (Single Page Application) を作成するライブラリとして使用されています。
React.js のみを使って上記の例のような記事一覧ページを生成する場合、Web ブラウザ上で JavaScript を実行してコンテンツを動的に生成する必要があります。
これを SSG や SSR と区別するために、__CSR: Client-side rendering（クライアントサイドレンダリング）__ と呼びます。

- CSR ... Web ブラウザ上の JavaScript API のみ呼び出せる（React.js のみを使った場合）
- SSG/SSR ... Node.js 環境の JavaScript API を呼び出せる（Next.js を使った場合）

つまり、Next.js を使うと、より柔軟な方法でコンテンツを事前生成することが可能になります。

{{% note title="静的サイトジェネレーターとの違いは？" %}}
Hugo などの Static Site Generator（静的サイトジェネレーター）は、そのビルドコマンド自体が、上記の SSG (Static Generation) に対応する Pre-rendering 機能を提供しています。
Next.js を使うと、Hugo に組み込まれている Static Generation 部分（`.md` から `.html` への変換部分）を自由に実装できるのだと考えるとわかりやすいかもしれません。
{{% /note %}}


Pre-rendering 機能を使用する
----

### Pre-rendering 用の関数

Next.js が提供する Pre-rendering 機能のひとつである静的サイトジェネレーション (SSG: Static Generation) を使用すると、Web サイトのビルド時（`next build` 実行時）に、外部 API などを利用して HTML を事前生成することができます。
SSG 機能を利用するには、各ページコンポーネントの実装ファイルで、次のような名前の `async` 関数を実装して `export` します。

__getStaticProps 関数__ (SSG)
: Web サイトのビルド時にこの関数が呼び出されるので、そのページのレンダリングに使用するデータを返すように実装します。戻り値として返したオブジェクトの `props` プロパティの値が、ページコンポーネントの引数（通称 `props`）として渡されます。

__getStaticPaths 関数__ (SSG)
: ダイナミックルーティングを使用するとき、事前生成するページのリストを返すよう実装します。詳しくは、[ダイナミックルーティングの記事](/p/rdq3ep2) で説明します。

一方、完全にサーバーサイドで HTML を生成するサーバーサイドレンダリング (SSR: Server-side Rendering) の仕組みを使いたい場合は、上記の代わりに `getServerSideProps` 関数を実装します。

__getServerSideProps 関数__ (SSR)
: Web サイトへのアクセス時にこの関数が呼び出されるので、そのページのレンダリングに使用するデータを返すように実装します。詳しくは、[ダイナミックルーティングの記事](/p/rdq3ep2) で説明します。

ちょっとややこしいですが、Next.js では、SSG も SSR も Pre-rendering の一種として分類されています。
Pre-rendering というのは、あくまでサーバーサイド（ビルド時含む）でページの内容を構築することを示しています。
Pre-rendering じゃないのは、CSR: Client-side Rendering のみです。

### Pre-rendering の実装例

例として、ローカルファイルや、データベースなどに格納された「本の一覧」を表示する `books` インデックスページを考えてみます。
こういった __インデックスページ__ を Web サイトのビルド時に生成するには、__`getStaticProps`__ 関数を実装して、戻り値の __`props`__ プロパティで一覧情報を返すように実装します。
`props` プロパティの値はページコンポーネントの引数として渡されるため、その値を参照して「本の一覧」を表示することができます。
下記のサンプルコードでは、`props` プロパティの型を `Props` として定義しています。

{{< code lang="tsx" title="pages/books/index.tsx" >}}
import Link from 'next/link'
import React from 'react'
import { GetStaticProps } from 'next'

type Book = {
  id: string;
  title: string;
}

// ページコンポーネントに渡されるデータ
type Props = {
  books: Book[];
}

// この関数がビルド時に呼び出され、戻り値の props の値がページコンポーネントに渡される
export const getStaticProps: GetStaticProps<Props> = async context => {
  // 本来は、ここで外部 API などを呼び出してデータを取得する
  const books = [
    { id: '001', title: 'Title-1' },
    { id: '002', title: 'Title-2' },
    { id: '003', title: 'Title-3' }
  ]

  // この props プロパティの値がページコンポーネントに渡される
  return { props: { books } }
}

// ページコンポーネントの実装
const BooksPage: React.FC<Props> = ({ books }) => (
  <>
    <h2>Book list</h2>
    <ul>
      {books.map(book => (
        <li key={book.id}>
          <Link href={`/books/${book.id}`}><a>{book.title}</a></Link>
        </li>
      ))}
    </ul>
  </>
)
export default BooksPage
{{< /code >}}

`getStaticProps` 関数は Node.js 環境で実行されるため、ローカルファイルへのアクセスや、外部 API の呼び出しなどを自由に行うことができます。
つまり、データソースとして、ローカルの YAML ファイルや CSV ファイル、Web API で取得したデータなど、どのようなデータでも扱うことができます。

`getStaticProps` は、ページコンポーネントの実装ファイル (`pages/**.tsx`) の中でしか定義できないことに注意してください。

### 個別ページの Pre-rendering

上記の実装例では、「本の一覧」を表示するインデックスページを Pre-rendering する方法を示しました。
そこに表示された各アイテムには次のような URL のリンクが張られており、個々の本の詳細情報にジャンプできるようになっています。

- `/books/001`
- `/books/002`
- `/books/003`

これらのページを、`pages/books/001.tsx` や `pages/books/002.tsx` のようなファイルで、ひとつずつページコンポーネントとして実装していくのは大変です。
Next.js は、上記のような URL によるアクセスを 1 つのページコンポーネント (`pages/books/[id].tsx`) でハンドルする __ダイナミックルーティング__ という仕組みを提供しています。

{{< reference >}}
- [Next.js のダイナミックルーティング機能を利用する (getStaticPaths, getStaticProps, getServerSideProps)](/p/rdq3ep2)
{{< /reference >}}

