---
title: "Next.js でブラウザ履歴で戻るボタンを表示する (router.back)"
url: "/p/m4dmu4c"
date: "2022-02-22"
tags: ["Next.js"]
---

Next.js の `next/router` モジュールが提供する `NextRouter` オブジェクトの [back メソッド](https://nextjs.org/docs/api-reference/next/router#routerback) を呼び出すと、ブラウザの履歴に従って 1 つ前のページに遷移することができます。
つまり、ブラウザの「戻る」ボタンを押した場合と同じ振る舞いをします。
`NextRouter` オブジェクトは、`useRouter` フックで取得することができます。

次の `BackButton` コンポーネントはシンプルな「戻る」ボタンを表示し、クリック時に `NextRouter#back()` を呼び出します。

{{< image src="img-001.png" title="戻るボタン（標準の button 版）" >}}

{{< code lang="tsx" title="components/BackButton.tsx" >}}
import { FC } from 'react'
import { useRouter } from 'next/router'

export const BackButton: FC = () => {
  const router = useRouter()
  return (
    <button alia-label="戻る" type="button" onClick={() => router.back()}>
      戻る
    </button>
  )
}
{{< /code >}}

上の例では、素の `button` コンポーネントを使っていますが、mui (Material-UI) などの UI ライブラリを使えばリッチなボタンを表示できます。

{{< image src="img-002.png" title="戻るボタン（mui 版）" >}}

{{< code lang="tsx" title="components/BackButton.tsx" >}}
import { FC } from 'react'
import { useRouter } from 'next/router'
import BackIcon from '@mui/icons-material/ArrowBackIosNew'
import Button from '@mui/material/Button'

export const BackButton: FC = () => {
  const router = useRouter()
  return (
    <Button aria-label="戻る" variant="contained" onClick={() => router.back()}>
      <BackIcon />
    </Button>
  )
}
{{< /code >}}

