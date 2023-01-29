---
title: "Next.js のコンポーネント内でクエリ文字列を取得する (next/router, useRouter)"
url: "/p/r7fou3a"
date: "2021-06-27"
tags: ["Next.js"]
---

クエリ文字列とは
----

{{< code >}}
https://example.com/todos?sortby=title&order=asc
{{< /code >}}

URL のクエリ文字列（クエリパラメーター）というのは、上記のような URL の末尾の `?` 以降の、`sortby=title&order=asc` の部分のことを指します。
この部分を参照する方法としては、主に次の 2 種類の方法があります。

- クライアントサイド JS ... `useRouter` フックを使う
- サーバーサイド JS ... `getServerSideProps` に渡されるパラメーターを使う


クライアントサイド JS からクエリパラメーターを参照する (router.query)
----

Next.js の [useRouter フック](https://nextjs.org/docs/api-reference/next/router) を使うと、上記のようなクエリパラメーター部分を簡単に抽出することができます。
次の例では、クエリパラメーターとして渡された `sortby` と `order` の値を取得しています。
値が省略された場合は、それぞれの値は `undefined` になります。

{{< code lang="tsx" title="src/pages/todos.tsx" >}}
import { NextPage } from 'next'
import { useRouter } from 'next/router'

const TodosPage: NextPage = () => {
  const router = useRouter()
  const { sortby, order } = router.query

  console.log(sortby)  // 指定されていなければ undefined
  console.log(order)   // 指定されていなければ undefined

  return (
    <p>{ sortby } で { order === 'asc' ? '昇順' : '降順' } ソートします</p>
  )
}

export default TodosPage
{{< /code >}}

{{% note title="プリレンダリングと hydrate 処理の考慮" %}}
Next.js にはページコンポーネントのプリレンダリングの仕組み（あらかじめ JS 部分まで実行して静的な HTML を生成しておく仕組み）があり、その時点では URL のクエリ文字列は参照できないので、`router.query` オブジェクトも空っぽになっています。
上記の例で言えば、プリレンダリング時の `sortby` や `order` の値は `undefined` になっているということです。
もちろん、実際にクエリパラメーター付きの URL で Web サイトにアクセスするタイミングでは、クライアントサイドで実行される JavaScript で `router.query` の値を参照できるようになっていますが、ページコンポーネントの実装では、クエリ文字列が指定されていない場合のことを考えておくべきです。

このように、Next.js アプリにおいて、ページコンポーネントに記述した JS コードが、プリレンダリング時とアクセス時に行われる仕組みは [hydration (hydrate) と呼ばれています](https://nextjs.org/docs/advanced-features/automatic-static-optimization)。
{{% /note %}}


サーバーサイド JS からクエリパラメーターを参照する (context.query)
----

Web サイトへのアクセス時に必ずサーバーサイドで呼び出される `getServerSideProps` 関数では、関数のパラメーターとしてクエリパラメーターを受け取ることができます。

{{< code lang="tsx" title="serverSideProps 内でのクエリパラメーターの参照" >}}
import { GetServerSideProps, NextPage } from 'next'

/** ページの描画に使用するデータ（NextPage コンポーネントへの入力） */
type PageProps = {
  todos: Todo[]
}

/** サーバーサイドでのデータ取得処理 */
export const getServerSideProps: GetServerSideProps<PageProps> =
  async (context) => {
    const { sortby, order } = context.query  // URL の ? 以降のクエリパラメーター

    console.log(sortby)  // 指定されていなければ undefined
    console.log(order)   // 指定されていなければ undefined

    // 指定されたクエリパラメーターに基づいてデータ取得
    const props: PageProps = {
      todos: db.getTodos(sortBy, order)
    }

    return { props }
  }

/** ページコンポーネント */
const TodoPage: NextPage<PageProps> = ({ todos }) => {
  // ...
}

export default TodoPage
{{< /code >}}

ちなみに、ビルド時に呼び出される `getStaticProps` 関数の中では、クエリパラメーターを取得することはできません。
クエリパラメーターは、あくまで実際にユーザーが Web サイトにアクセスするときに指定されるものという扱いであり、ビルド時にはクエリパラメーターまで含んだページを出力できないからです。
もっと具体的にいうと、`001?sortby=title&order=asc.html` のようなクエリまで含んだファイルを事前生成することはできないということです。


クエリ文字列を変更する
----

コンポーネントの中から URL のクエリ文字列部分を変更するには、`NextRouter` オブジェクトの `replace` メソッドの引数に渡すオブジェクトの __`query`__ プロパティを指定します。
元の URL をブラウザの履歴に積みたいときは、`replace` の代わりに `push` メソッドを使用します。

```ts
// import NextRouter from 'next/router'

const updateQuery = (value: string) => {
  NextRouter.replace({
    // 既存のクエリパラメーターとマージする形で keyName の値を書き換える
    query: { ...NextRouter.query, keyName: value },
  })
}
```

上記の例では、`useRouter` フックの代わりに、`next/router` モジュールがデフォルト export するオブジェクトを参照しています。
ユーザー操作によるトリガーでしか呼ばれない関数など、コンポーネントのライフサイクルから切り離された場所であれば、このデフォルトオブジェクトを使用できます。

複数のクエリパラメーターを一度に変更したい場合は、次のような `updateQuery` 関数を作っておくと便利かもしれません。
この関数は、複数のキー＆バリューを受け取ることができます。

```ts
// import { ParsedUrlQueryInput } from 'querystring'
// import NextRouter from 'next/router'

/**
 * URL のクエリパラメーター部分を更新します。
 *
 * 使用例: updateQuery({ key1: "value1", key2: "value2" })
 */
function updateQuery(query: { [key: string]: string | number }) {
  // 既存のクエリパラメーターとマージ
  const newQuery: ParsedUrlQueryInput = { ...NextRouter.query, ...query }
  NextRouter.replace({ query: newQuery })
}
```

