---
title: "MUI コンポーネント (v5) に独自のスタイルを設定する (sx prop、styled)"
url: "p/87p5o2d/"
date: "2022-07-11"
tags: ["MUI"]
---

{{% private %}}
- https://mui.com/system/the-sx-prop/
- https://mui.com/system/styled/
{{% /private %}}

MUI v5 以降のカスタムスタイル設定
----

MUI v5 以降でコンポーネントにカスタムスタイル (CSS) を割り当てる方法には、大きく下記の 2 つのやり方があります。

- __sx prop__ を使う方法
  - MUI のコンポーネントには __sx prop__ が定義されていて、ここにスタイルオブジェクトを渡すことで、個別にスタイルを調整できます。つまり、使い捨てのスタイル設定を行う方法です。sx prop は HTML 要素本来の style プロパティと比べて簡潔な構文で記述できます。例えば、margin や padding の設定用に 1 文字 (`m`, `p`) のプロパティ名が定義されていたりします。
- __styled API__ を使う方法
  - 既存のコンポーネントをラップする形で、スタイル拡張したコンポーネントを生成します。sx prop を使った方法に比べて再利用性が高い方法で、複数個所で使用するコンポーネントにスタイル設定したいときに便利です。`styled()` 関数は内部的には [Emotion](https://emotion.sh/) というライブラリが提供する関数につながっていますが、MUI では `@mui/material/styles` パッケージをインポートすることで使えるようになっています。

いずれの方法でも、テーマ設定（theme オブジェクト）に基づいたカスタマイズが可能で、この仕組みにより、アプリ全体のデザインに一貫性を持たせやすくなっています。

{{% note title="Emotion と JSS" %}}
MUI v4 以前は、スタイルエンジンとして [JSS](https://cssinjs.org/) ライブラリベースの `withStyles()` や `createStyles()` といった関数を使用していましたが、MUI v5 以降は、動的スタイルにおけるパフォーマンスがよい Emotion の `styled()` 関数を使うように変更されました。
MUI v5 を使用するアプリケーションでも `@mui/styles` パッケージをインポートすれば `withStyles()` や `createStyles()` 関数を使用できますが、`@mui/styles` パッケージは deprecated（非推奨）扱いとなっています。
代わりに、`@mui/material/styles` パッケージの `styled()` 関数を使いましょう。
{{% /note %}}

以下、2 つのやり方を詳しく見ていきます。


sx prop によるスタイル設定
----

MUI コンポーネントに実装されている __sx prop__ を使うと、[様々な省略記法](https://mui.com/system/properties/) を使って、テーマ (theme) に基づいたスタイル設定を行うことができます。
このプロパティに渡すオブジェクトは、具体的には `SxProps<Theme>` 型ですが、TypeScript で型情報を扱うケース以外は意識する必要はありません。
次のサンプルコードでは、MUI の `div` 要素的な位置付けの `Box` コンポーネントに対して sx prop でスタイル指定しています。
sx prop にはオブジェクトを渡すため、コードは `sx={{ ... }}` のように中括弧が 2 つ続く形になることに注意してください。

```tsx
// import { Box } from '@mui/material'

<Box
  sx={{
    boxShadow: 1,                   // boxShadow: theme.shadows[1]
    color: 'primary.contrastText',  // color: theme.palette.primary.contrastText
    bgcolor: 'primary.main',        // background: theme.palette.primary.main
    m: 1,                           // margin: theme.spacing(1)
    p: {
      xs: 1,  // [theme.breakpoints.up('xs')]: { padding: theme.spacing(1) }
    },
    zIndex: 'tooltip',              // zIndex: theme.zIndex.tooltip
  }}
>
  This is a box
</Box>
```

{{< image src="img-001.png" title="表示結果" >}}

各プロパティの末尾のコメントは、省略記法を使わなかった場合にどのような記述になるかを示しています。
テーマ設定を利用した値の記述がシンプルになるだけでなく、プロパティ名自体もシンプルなものが定義されています。
特に、マージンの指定方法は `m: 1` というように、ものすごく短く書けるようになっています。
sx prop が独自で定義している `m` のようなプロパティを、__カスタムシステムプロパティ__ と呼びます。

- 参考: [sx prop のカスタムシステムプロパティの一覧](https://mui.com/system/properties/)

よく使いそうなカスタムシステムプロパティをいくつか紹介していきます。

### マージン (m)、パディング (p)

マージンやパディングの設定は頻繁に行われるので、カスタムプロパティには 1 文字や 2 文字の非常に短い名前が付けられています。

- マージン
  - __`m`__ ... 上下左右のマージン
  - __`mx`__ ... 左右のマージン
  - __`my`__ ... 上下のマージン
  - __`mt`__, __`mb`__, __`ml`__, __`mr`__ ... それぞれ上下左右のマージン
- パディング
  - __`p`__ ... 上下左右のパディング
  - __`px`__ ... 左右のパディング
  - __`py`__ ... 上下のパディング
  - __`pt`__, __`pb`__, __`pl`__, __`pr`__ ... それぞれ上下左右のパディング

マージンやパディングの値として数値だけを指定すると、単位はテーマの `spacing` に設定したピクセル数となります（デフォルトは 8px）。
具体的なピクセル数を `24px` のように指定することも可能ですが、テーマで設定されたスペース単位で配置することで、全体のレイアウトに統一感が出ます。
例えば、次のように指定すると、上下左右のマージンが 16px (= 8px x 2) に設定されます。

```tsx
<Box sx={{ m: 2 }}>...</Box>
```

さらに、これらのカスタムシステムプロパティは、sx prop オブジェクト経由ではなく、次のように MUI コンポーネント自体の prop としても指定できるようになっています。
ちょこちょこっとスタイル指定する場合は、こちらの方が簡潔に書けます。

```tsx
<Box m={2}>...</Box>
```

### 色 (color, bgcolor)

__`color`__ でテキスト色、__`bgcolor`__ で背景色を指定します。
値には、次のようにドット区切りでテーマのパレットキー名を指定することができます。

```tsx
<Box sx={{ color: 'primary.contrastText', bgcolor: 'primary.main' }}>Primary</Box>
<Box sx={{ color: 'secondary.contrastText', bgcolor: 'secondary.main' }}>Secondary</Box>
<Box sx={{ color: 'success.contrastText', bgcolor: 'success.main' }}>Success</Box>
<Box sx={{ color: 'error.contrastText', bgcolor: 'error.main' }}>Error</Box>
<Box sx={{ color: 'info.contrastText', bgcolor: 'info.main' }}>Info</Box>
```

例えば、`'primary.main'` というパレットキー名は、`theme.palette.primary.main` に設定されたプライマリ色に置き換えられます。
指定可能なパレットキー名の一覧は、[Default Theme - MUI](https://mui.com/material-ui/customization/default-theme/?expand-path=$.palette) のページで確認できます。

これらのカスタムシステムプロパティも、MUI コンポーネント自身の prop として直指定できます。

```tsx
<Box color="error.contrastText" bgcolor="error.main">
```

カラー値として、ドットを含まない文字列を指定すれば、従来のカラー名や HEX 値で色指定できます。

```tsx
<Box sx={{ color: 'white', bgcolor: '#f00' }}>
```

### レスポンシブデザイン

次の例では、ブラウザの横幅によって Box コンポーネントの色を切り替えています。

```tsx
<Box sx={{
  bgcolor: {
    xs: 'red',      // [theme.breakpoints.up('lg')]: 'red'
    sm: 'blue',     // [theme.breakpoints.up('sm')]: 'blue'
    md: 'yellow',   // [theme.breakpoints.up('md')]: 'yellow'
    lg: '#00ff00',  // [theme.breakpoints.up('lg')]: '#00ff00'
    xl: '#000000',  // [theme.breakpoints.up('xl')]: '#000000'
  },
}}>
```

[テーマで設定されたブレイクポイント](https://mui.com/material-ui/customization/breakpoints/) の値よりも、ブラウザの横幅が大きくなったときに採用する色を指定しています。
ブレイクポイントの各キーに設定されたデフォルト値は次のようになっています。

- __`xs`__ ... 0px (extra-small)
- __`sm`__ ... 600px (small)
- __`md`__ ... 900px (medium)
- __`lg`__ ... 1200px (large)
- __`xl`__ ... 1536px (extra-large)

ブレイクポイントは、コンポーネントの表示・非表示を切り替えるときにも使えます。

{{< code lang="tsx" title="横幅が狭いときは非表示にする" >}}
<Box sx={{ display: { xs: 'none', sm: 'block' } }}>
{{< /code >}}

### 影は 25 段階 (boxShadow)

__`boxShadow`__ にはデフォルトで 0～24 の値を設定することができ、値が大きいほど大きい影が付きます（浮いているように見えます）。
0 を指定すると影は表示されません。

```tsx
<Button sx={{ m: 1, p: 1, boxShadow: 0 }}>shadow = 0</Button>
<Button sx={{ m: 1, p: 1, boxShadow: 1 }}>shadow = 1</Button>
<Button sx={{ m: 1, p: 1, boxShadow: 2 }}>shadow = 2</Button>
<Button sx={{ m: 1, p: 1, boxShadow: 3 }}>shadow = 3</Button>
<Button sx={{ m: 1, p: 1, boxShadow: 4 }}>shadow = 4</Button>
<Button sx={{ m: 1, p: 1, boxShadow: 24 }}>shadow = 24</Button>
```

### 印刷時に非表示にする (displayPrint)

印刷時に `display` プロパティに反映する値を指定したいときは、__`displayPrint`__ カスタムプロパティを使うとシンプルに記述できます。

{{< code lang="tsx" title="この要素は印刷時には出力しない" >}}
<Box sx={{
  display: 'block',
  displayPrint: 'none'  // '@media print': { display: 'none' }
}}>
{{< /code >}}

### 疑似セレクタ、メディアクエリ、入れ子セレクタ

{{< code lang="tsx" title="疑似セレクタ" >}}
sx={{
  ':hover': {
    boxShadow: 6
  }
}}
{{< /code >}}

{{< code lang="tsx" title="メディアクエリ" >}}
sx={{
  '@media print': {
    width: 300
  }
}}
{{< /code >}}

{{< code lang="tsx" title="入れ子セレクタ" >}}
sx={{
  '& .child': {
    bgcolor: 'primary.main'
  }
}}
{{< /code >}}

### SxProps 変数を定義して sx prop に渡す

sx prop に渡している値は、`SxProps<Theme>` 型のオブジェクトなので、次のように変数として定義してから渡すこともできます。

```tsx
import { Box, SxProps, Theme } from '@mui/material'

const style: SxProps<Theme> = {
  m: 1,
  bgcolor: 'background.paper',
  width: '5rem',
  height: '5rem',
}

<Box sx={style}>Hello</Box>
```

次のようにすれば、スタイル定義を使いまわしつつ、一部のプロパティだけをオーバーライドできます。

```tsx
<Box sx={{ ...style, border: 1 }}>Box-1</Box>
<Box sx={{ ...style, borderTop: 1 }}>Box-2</Box>
<Box sx={{ ...style, borderLeft: 1 }}>Box-3</Box>
```

styled API によるスタイル設定
----

__`@mui/material/styles`__ パッケージが提供する __`styled`__ 関数を使用すると、既存のコンポーネントをラップする形でスタイルを付加することができます。
また、スタイル定義時にテーマオブジェクト (theme) を受け取ることができるので、テーマ設定に基づいたスタイル指定が可能です（というか、`theme` オブジェクトを使わないとほぼ設定できません）。
次の例では、MUI の `Button` コンポーネントにカスタムスタイルを付加した新しい `StyledButton` コンポーネントを定義しています。

```tsx
import { Button } from '@mui/material'
import { styled } from '@mui/material/styles'

const StyledButton = styled(Button)(({ theme }) => ({
  margin: theme.spacing(1),
  boxShadow: theme.shadows[3],
  color: theme.palette.error.main,
}))

<StyledButton>OK</StyledButton>
```

styled API によるスタイル定義の中では、sx prop のようなショートカット記法は使えないことに注意してください。
例えば、`m: 1` という記述はできず、`margin: theme.spacing(1)` と記述する必要があります。

HTML の標準的なコンポーネント、例えば `div` などを `styled` API でスタイル拡張することもできます。

```tsx
const StyledDiv = styled('div')(({ theme }) => ({
  color: theme.palette.primary.contrastText,
  background: theme.palette.primary.main,
  margin: theme.spacing(1),
  padding: theme.spacing(2),
  borderRadius: theme.shape.borderRadius,
  fontSize: '20pt',
}))

<StyledDiv>Hello</StyledDiv>
```

この副次的な効果として、生成されたコンポーネントで __`sx prop`__ が使えるようになります。

```tsx
<StyledDiv sx={{ boxShadow: 6 }}>Hello</StyledDiv>
```

[Next.js での MUI 利用サンプル](https://github.com/mui/material-ui/blob/master/examples/nextjs-with-typescript/src/Link.tsx#L9) では、次のように sx prop を有効化するためだけに styled API を使っている箇所があったりします。

```tsx
// Add support for the sx prop for consistency with the other branches.
const Anchor = styled('a')({});
```

（おまけ）styled API によるスタイル定義サンプル
----

### 例: テーブルの行を 1 行おきに縞々表示

```tsx
const StyledTableRow = styled(TableRow)(({ theme }) => ({
  '&:nth-of-type(odd)': {
    backgroundColor: theme.palette.action.hover,
  },
  // hide last border
  '&:last-child td, &:last-child th': {
    border: 0,
  },
}))
```

### 例: Dark / Light モードによるスタイル切り替え

```tsx
const Item = styled(Paper)(({ theme }) => ({
  backgroundColor: theme.palette.mode === 'dark' ? '#1A2027' : '#fff',
  ...theme.typography.body2,
  padding: theme.spacing(1),
  textAlign: 'center',
  color: theme.palette.text.secondary,
}))
```

### 例: プロパティ値の一部で theme を使う

```tsx
border: `1px solid ${theme.palette.divider}`,
```

