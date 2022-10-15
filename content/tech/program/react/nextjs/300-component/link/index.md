---
title: "Next.js アプリでのリンク方法まとめ（Material-UI との連携なども） (next/link, next/router)"
url: "/p/vgs4dnw"
date: "2021-08-10"
tags: ["Material-UI", "Next.js"]
changes:
  - 2022-10-11: クエリパラメーター部分の変更方法について
---

{{% private %}}
- https://next.material-ui.com/guides/routing/
{{% /private %}}

アプリ内ページへのリンク（基本）
----

{{< code lang="html" >}}
// import Link from 'next/link'

<Link href="/about">
  <a>About us</a>
</Link>
{{< /code >}}

`pages/about.tsx` ページコンポーネントが生成するページへのリンクになります。
リンクを `<a>` コンポーネントとして出力するために、上記のように子要素として明示的に `<a>` タグの記載が必要です。
`<a>` を省略しても、リンククリック時は JavaScript でハンドルされるので動作しますが、HTML 的には正しく `<a>` 要素を出力しておくべきです（SEO 的にも）。

### replace オプション

{{< code lang="html" >}}
<Link href="/about" replace>
{{< /code >}}

上記のように __`replace`__ オプションを指定すると、ページ遷移前の URL がブラウザの履歴に残りません（戻るキーで戻らなくなります）。


外部リンク
----

`https://` で始まる外部リンクを出力したい場合は、`<a>` コンポーネントをそのまま使用します。
`next/link`（や `react-router`) が提供する `Link` コンポーネントは、アプリ内のルーティング用なので使えません。
外部リンクを開く場合は、安全性のために一律で `rel="noopener noreferrer"` を付けましょう。
リンククリック時に必ず別タブで開きたいときは、`target="_blank"` を指定してください。

{{< code lang="html" >}}
<a href="https://example.com/" target="_blank" rel="noopener noreferrer">
  サイト名
</a>
{{< /code >}}

Material-UI を採用したサイトの場合は、`a` の代わりに `@material-ui/core/Link` コンポーネントを使用することで、サイト内のデザインを統一することができます。
`Link` という名前のコンポーネントはいろいろなライブラリが提供しているので、下記のように別名 (`MuiLink`) を付けて使用すると混乱を防ぐことができます。

{{< code lang="html" >}}
// import { Link as MuiLink } from '@material-ui/core'

<MuiLink href="https://example.com/" target="_blank" rel="noopener noreferrer">
  サイト名
</MuiLink>
{{< /code >}}

外部リンクを出力するときに、毎回上記のような長ったらしいコードを書くのは大変なので、次のような独自コンポーネント (`ExternalLink`) などを定義しておくと便利です。
ここでは、外部リンクを示すアイコン (`@material-ui/icons/Launch`) を末尾に表示するようにしています。

{{< code lang="tsx" title="src/components/ExternalLink.tsx" >}}
import { FC } from 'react'
import LaunchIcon from '@material-ui/icons/Launch'
import { Link as MuiLink } from '@material-ui/core'

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

`MuiLink` コンポーネントの `style` プロパティは、`LaunchIcon` の上下位置をリンク名のテキストに合わせるためのものです。

{{< code lang="tsx" title="使用例" >}}
<ExternalLink url="https://example.com/" title="サイト名" />
{{< /code >}}

{{< image src="img-001.png" w="150" border="true" title="表示結果" >}}


Next.js の Link コンポーネント以下に Material-UI の UI コンポーネントを配置する
----

Next.js の Link コンポーネント (`next/link`) で独自のコンポーネントをリンクとして動作させるためには、ちょっとしたコツがあります。

Material-UI は独自のコンポーネントとして、[Link](https://next.material-ui.com/components/links/) や [Button](https://next.material-ui.com/components/buttons/) を持っており、これらに `href` プロパティを指定することによって、`a` 要素としてレンダリングするようになっています（Material-UI の `Link` と Next.js の `Link` は別物なので注意）。
ダイレクトに `a` 要素を使わないのは、Material-UI が提供する UI 表現を使用するためですね。

{{< code lang="tsx" title="Material-UI のリンク系コンポーネント" >}}
import Link from '@material-ui/core/Link'
import Button from '@material-ui/core/Button'

<Link href="/about" underline="none">About us</Link>
<Button href="/about" variant="contained">About us</Button>
{{< /code >}}

これはこれで便利なのですが、Next.js アプリ内での遷移には `next/link` を使ったルーティングを行う必要があるため、上記のようには記述できません（クライアントサイド JS でのルーティングが動作しません）。
`next/link` モジュールの `Link` 要素を使って、Material-UI の UI コンポーネントをリンクとして機能させるには、次のように記述します。
`Link` という名前がコンフリクトするため、ここでは、それぞれ `NextLink`、`MuiLink` という名前でインポートしています（片方だけリネームすれば十分ですが分かりやすくするため両方リネームしています）。

{{< code lang="tsx" title="next/link と Material-UI を組み合わせて使う" >}}
import NextLink from 'next/link'
import { Button, Link as MuiLink } from '@material-ui/core'

<NextLink href="/about" passHref>
  <MuiLink underline="none">About us</MuiLink>
</NextLink>
<NextLink href="/about" passHref>
  <Button variant="contained">About us</Button>
</NextLink>
{{< /code >}}

Next.js の `Link` コンポーネント (`next/link`) の `href` プロパティで指定した値を子要素に伝搬させるために、__`passHref`__ プロパティを指定するのがポイントです。
`passHref` を指定しなくても、Next.js のクライアントサイド JS でリンク機能が動作するため、一見正しく動いているかのように見えますが、`a` 要素の `href` 属性がされないため、SEO 的に不利な Web サイトになってしまいます（さらに、Material-UI の `Button` コンポーネントの場合は、`href` が渡されないと、`a` 要素ではなく `button` 要素として出力されてしまいます）。


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
// import Link from 'next/link'

<Link href={{
  query: {
    key1: 'value1',
    key2: 'value2',
  },
}}>...</Link>
```

上記のようにすると、ページのパス自体は変更されず、クエリパラメーター部分だけが変更されます。
例えば、`https://example.com/hello` ページから上記リンクをクリックすると、URL は `https://example.com/hello?key1=value1&key2=value2` に変化します。

リンククリック時にパス部分も変更したい場合（ページ遷移したい場合）は、次のように __`href`__ プロパティを一緒に指定します。

```tsx
<Link href={{
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
import Link from 'next/link'
import { useRouter } from 'next/router'

export const MyComponent: FC = () => {
  const router = useRouter()
  const query = router.query  // カレント URL に含まれるクエリパラメーター

  return <>
    <Link
      href={{
        query: {
          ...query,
          key2: 'newValue',
        },
      }}
    >...</Link>
  </>
}
```

上記のリンクをクリックすると、URL のクエリパラメーターの `key2=newValue` という部分だけが書き変わります（他のパラメーターは維持されます）。
`Link` コンポーネントではなく、関数呼び出しでクエリパラメーターを変更したいときは、__`router.push`__ や __`router.replace`__ に同様のオブジェクトを渡してやれば OK です。

