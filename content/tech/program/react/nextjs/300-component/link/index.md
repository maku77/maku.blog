---
title: "Next.js アプリでのリンク方法まとめ（mui/Material-UI との連携なども） (next/link, next/router)"
url: "/p/vgs4dnw"
date: "2021-08-10"
lastmod: "2022-11-19"
tags: ["Material-UI", "mui", "Next.js"]
changes:
  - 2022-11-19: mui バージョンのコード、Next.js 13 バージョンのコードに対応
  - 2022-10-11: クエリパラメーター部分の変更方法について
---

{{% private %}}
- （新） https://mui.com/material-ui/guides/routing/
- （旧） https://next.material-ui.com/guides/routing/
{{% /private %}}

アプリ内ページへのリンク（基本）
----

{{< code lang="tsx" >}}
// import Link from 'next/link'
<Link href="/about">About us</Link>
{{< /code >}}

Next.js でアプリ内部で閉じるページリンクを張るには、[next/link](https://nextjs.org/docs/api-reference/next/link) モジュールを使用します。
上記のようにすると、`pages/about.tsx` ページコンポーネントが生成するページへのリンクになります。

{{% note title="a 要素は必要ない" %}}
Next.js 12 以前は `Link` 要素の下に明示的に `a` 要素を配置する必要がありました。
これは、HTML 的に正しく `a` 要素を配置するためでしたが、Next.js 13 では自動的に `a` 要素を挿入するようになったので、`a` 要素の記述は必要ありません。
{{% /note %}}

### 遷移前の履歴を置き換える replace オプション

{{< code lang="tsx" >}}
<Link href="/about" replace>
{{< /code >}}

上記のように __`replace`__ オプションを指定すると、ページ遷移前の URL がブラウザの履歴に残りません（戻るキーで戻らなくなります）。


外部リンク
----

`https://` で始まる外部リンクを出力したい場合は、`<a>` コンポーネントをそのまま使用します。
`next/link`（や `react-router`) が提供する `Link` コンポーネントは、アプリ内のルーティング用なので使えません。
外部リンクを開く場合は、安全性のために一律で `rel="noopener noreferrer"` を付けましょう。
リンククリック時に必ず別タブで開きたいときは、`target="_blank"` を指定してください。

{{< code lang="tsx" >}}
<a href="https://example.com/" target="_blank" rel="noopener noreferrer">
  Website title
</a>
{{< /code >}}

mui (Material-UI) を採用したサイトの場合は、`a` の代わりに mui が提供する `Link` コンポーネントを使用することで、サイト内のデザインを統一することができます。
`next/link` が提供する `Link` コンポーネントと混同しないように、次のように別名 (`MuiLink`) を付けて使用すると分かりやすくなります。

{{< code lang="tsx" >}}
// import { Link as MuiLink } from '@mui/material'     // mui 版
// import { Link as MuiLink } from '@material-ui/core' // material-ui 版

<MuiLink href="https://example.com/" target="_blank" rel="noopener noreferrer">
  Website title
</MuiLink>
{{< /code >}}

外部リンクを出力するときに、毎回上記のような長ったらしいコードを書くのは大変なので、次のような独自コンポーネント (`ExternalLink`) などを定義しておくと便利です。
ここでは、ついでに、外部リンクを示す `Launch` アイコンを末尾に表示するようにしています。

{{< code lang="tsx" title="src/components/ExternalLink.tsx" >}}
import { FC } from 'react'
import LaunchIcon from '@mui/icons-material/Launch'
import { Link as MuiLink } from '@mui/material'
// 古い material-ui バージョンの場合
// import LaunchIcon from '@material-ui/icons/Launch'
// import { Link as MuiLink } from '@material-ui/core'

type Props = {
  url: string
  title: string
}

export const ExternalLink: FC<Props> = ({ url, title }: Props) => {
  return (
    <MuiLink
      href={url} target="_blank" rel="noopener noreferrer"
      style={{ display: 'inline-flex', alignItems: 'center' }}
    >
      {title}
      <LaunchIcon fontSize="inherit" />
    </MuiLink>
  )
}
{{< /code >}}

`MuiLink` コンポーネントの `style` プロパティは、`LaunchIcon` の上下位置をリンク名のテキストに合わせるために設定しています。

{{< code lang="tsx" title="使用例" >}}
<ExternalLink url="https://example.com/" title="サイト名" />
{{< /code >}}

{{< image src="img-001.png" w="150" border="true" title="表示結果" >}}


Next.js の Link コンポーネント以下に mui (Material-UI) の UI コンポーネントを配置する
----

Next.js の Link コンポーネント (`next/link`) で独自のコンポーネントをリンクとして動作させるためには、ちょっとしたコツがあります。

mui (Material-UI) は独自のコンポーネントとして、[Link](https://mui.com/material-ui/react-link/) や [Button](https://mui.com/material-ui/react-button/) を持っており、これらに `href` プロパティを指定することによって、`a` 要素としてレンダリングするようになっています（mui の `Link` と Next.js の `Link` は別物なので注意）。
ダイレクトに `a` 要素を使わないのは、mui (Material-UI) が提供する UI 表現を使用するためですね。

{{< code lang="tsx" title="Material-UI のリンク系コンポーネント" >}}
// import { Button, Link as MuiLink } from '@mui/material' // mui の場合
// import { Button, Link as MuiLink } from '@material-ui/core' // material-ui の場合

<MuiLink href="/about" underline="none">About us</Link>
<Button href="/about" variant="contained">About us</Button>
{{< /code >}}

これはこれで便利なのですが、Next.js アプリ内での遷移には `next/link` を使ったルーティングを行う必要があるため、上記のようには記述できません（クライアントサイド JS でのルーティングが動作しません）。
`next/link` モジュールの `Link` 要素を使って、mui (Material-UI) の UI コンポーネントをリンクとして機能させるには、次のように記述します。
`Link` という名前がコンフリクトするため、ここでは、それぞれ `NextLink`、`MuiLink` という名前でインポートしています（片方だけリネームすれば十分ですが分かりやすくするため両方リネームしています）。

{{< code lang="tsx" title="next/link と mui (Material-UI) を組み合わせて使う" >}}
// import NextLink from 'next/link'
// import { Button, Link as MuiLink } from '@mui/material' // mui の場合
// import { Button, Link as MuiLink } from '@material-ui/core' // material-ui の場合

<NextLink href="/about" passHref>
  <MuiLink underline="none">About us</MuiLink>
</NextLink>
<NextLink href="/about" passHref>
  <Button variant="contained">About us</Button>
</NextLink>
{{< /code >}}

Next.js の `Link` コンポーネント (`next/link`) の `href` プロパティで指定した値を子要素に伝搬させるために、__`passHref`__ プロパティを指定するのがポイントです。
`passHref` を指定しなくても、Next.js のクライアントサイド JS でリンク機能が動作するため、一見正しく動いているかのように見えますが、`a` 要素の `href` 属性がされないため、SEO 的に不利な Web サイトになってしまいます（さらに、mui の `Button` コンポーネントの場合は、`href` が渡されないと、`a` 要素ではなく `button` 要素として出力されてしまいます）。


プログラム内部でリダイレクト (Router.replace, window.location)
----

{{< code lang="tsx" >}}
// import Router from 'next/router'
Router.replace('/list/users')
{{< /code >}}

__`Router.replace`__ を使うと、リダイレクト前の URL がブラウザの履歴に残りません（最初からこの URL でアクセスしたかのように振る舞います）。

アプリ内のルーティングではなく、外部 URL へ移動する場合は、`next/router` ではなく __`window.location`__ を使用します。
これは、外部リンク用のコンポーネントとして `next/link` ではなく `<a>` を使用するのと同じ理由です。


クエリパラメーターを変更するリンク
----

リンクをクリックしたときに、URL 末尾のクエリパラメーター部分（`?key=value` みたいなところ）を変更したいときは、Next.js の `Link` コンポーネントの `href` 属性に、URL を表す文字列の代わりに `UrlObject` オブジェクトを指定します。
このオブジェクトの __`query`__ プロパティで指定した値が、遷移先の URL の末尾にクエリパラメーターとして付加されます。

```tsx
// import NextLink from 'next/link'

<NextLink href={{
  query: {
    key1: 'value1',
    key2: 'value2',
  },
}}>...</NextLink>
```

上記のようにすると、ページのパス自体は変更されず、クエリパラメーター部分だけが変更されます。
例えば、`https://example.com/hello` ページから上記リンクをクリックすると、URL は `https://example.com/hello?key1=value1&key2=value2` に変化します。

リンククリック時にパス部分も変更したい場合（ページ遷移したい場合）は、次のように __`href`__ プロパティを一緒に指定します。

```tsx
<NextLink href={{
  href: './other-page',
  query: {
    key1: 'value1',
    key2: 'value2',
  },
}}>
```

カレント URL に含まれているクエリパラメーターの一部だけを変更したい場合は、`useRouter` フックで現在のクエリパラメーターを取得した上で、変更する値を組み合わせます。

```tsx
import { RC } from 'react'
import NextLink from 'next/link'
import { useRouter } from 'next/router'

export const MyComponent: FC = () => {
  const router = useRouter()
  const query = router.query  // カレント URL に含まれるクエリパラメーター

  return <>
    <NextLink
      href={{
        query: {
          ...query,
          key2: 'newValue',
        },
      }}
    >...</NextLink>
  </>
}
```

上記のリンクをクリックすると、URL のクエリパラメーターの `key2=newValue` という部分だけが書き変わります（他のパラメーターは維持されます）。
`Link` コンポーネントではなく、関数呼び出しでクエリパラメーターを変更したいときは、__`router.push`__ や __`router.replace`__ に同様のオブジェクトを渡してやれば OK です。

