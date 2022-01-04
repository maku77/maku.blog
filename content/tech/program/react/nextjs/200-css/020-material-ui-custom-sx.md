---
title: "MUI (Material-UI) でカスタムコンポーネントに sx プロパティを渡せるようにする"
url: "/p/5e4cikn"
date: "2021-12-15"
tags: ["Material-UI"]
weight: 20
---

何をするか？
----

MUI (Material-UI) ver.5 以降では、各コンポーネントのスタイル設定に便利な __`sx`__ props を使うことができます。

- [The sx prop - MUI](https://mui.com/system/the-sx-prop/)

MUI コンポーネントをラップするようなカスタムコンポーネントを作成するときは、カスタムコンポーネント経由で `sx` props を渡せるようにしておくと、利用するときにマージン設定などを簡単に行えて便利です。


実装例
----

下記は、`sx` props を渡せるようにした `CustomButton` コンポーネントの実装例です。
`sx` props の値は、MUI の `Button` コンポーネントにそのまま渡しています。
型情報として __`SxProps<Theme>`__ を使うところがポイントです。

{{< code lang="tsx" title="src/components/CustomButton.tsx" >}}
import { FC, ReactNode } from 'react'
import Button from '@mui/material/Button'
import { SxProps, Theme } from '@mui/material/styles'

type Props = {
  children: ReactNode
  sx?: SxProps<Theme>
}

export const CustomButton: FC<Props> = ({ children, sx }) => {
  return (
    <Button variant="contained" sx={sx}>
      {children}
    </Button>
  )
}
{{< /code >}}

これで、次のように `sx` prop 流のマージン設定などを行えるようになります。

{{< code lang="tsx" title="src/pages/index.tsx（抜粋）" >}}
<CustomButton sx={{ m: 2 }}>Hello</CustomButton>
{{< /code >}}

上記のカスタムコンポーネントの実装例では、`sx` props の値を `Button` コンポーネントに渡していますが、複数要素から構成されるカスタムコンポーネントの場合は、次のように最上のコンテナ系コンポーネント（`Box` や `Stack` など）の `sx` props に渡した方がよいでしょう。

{{< code lang="tsx" title="src/components/CustomButton.tsx" >}}
import { FC, ReactNode } from 'react'
import Button from '@mui/material/Button'
import Stack from '@mui/material/Stack'
import Typography from '@mui/material/Typography'
import { SxProps, Theme } from '@mui/material/styles'

type Props = {
  children: ReactNode
  sx?: SxProps<Theme>
}

export const CustomButton: FC<Props> = ({ children, sx }) => {
  return (
    <Stack direction="row" spacing={1} alignItems="center" sx={sx}>
      <Button variant="contained">{children}</Button>
      <Typography variant="subtitle2">Amazing!</Typography>
    </Stack>
  )
}
{{< /code >}}

