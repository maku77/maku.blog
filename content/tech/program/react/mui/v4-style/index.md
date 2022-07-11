---
title: "MUI コンポーネント (v4) に独自のスタイルを設定する (makeStyles)"
url: "/p/cw9ju6f"
date: "2021-06-06"
tags: ["Material-UI", "Next.js"]
---

この記事では、MUI v4 以前の `makeStyles()` 関数を使ったカスタムスタイル設定方法を説明しています。
MUI v5 以降のカスタムスタイル設定では `sx prop` や `styled()` 関数を使用しますので、下記の記事を参照してください。

- 参考: [MUI コンポーネントに独自のスタイルを設定する（v5 以降の sx prop、styled 関数を使う方法）](/p/87p5o2d/)


Material-UI コンポーネントのスタイル設定
----

Material-UI が提供する各種コンポーネントには、表示スタイルを切り替えるためのプロパティが用意されています。
例えば、[Button コンポーネント](https://material-ui.com/components/buttons/) は `variant` や `color` プロパティで見た目を切り替えることができます。

{{< code lang="tsx" >}}
<Button>Default</Button>
<Button variant="contained" color="primary">Primary</Button>
<Button variant="contained" color="secondary">Secondary</Button>
<Button variant="outlined" disabled>Disabled</Button>
{{< /code >}}

多くのケースでは、この仕組みで十分にスタイル設定できるのですが、デフォルトのスタイルから外れた表示をしたり、`div` 要素など Material-UI 以外のコンポーネントに対して独自の CSS を適用したいことがあります。
このような場合、コンポーネントの実装ファイル内に直接 CSS コードを記述してスタイルをカスタマイズできます（JavaScript 内に記述するので __CSS-in-JS__ と呼びます）。
React の世界では色々な CSS 参照方法がありますが、Material-UI は次のような理由で CSS-in-JS な記述方法を採用しています。

- 現在のテーマ設定に基づいたスタイル設定を行える（例: 基準スペースの2倍のマージンを設定する）
- コンポーネントの `props` の値を使って動的にスタイル設定できる（例: `<MyButton color="vivid">` で派手な色のスタイルを設定する）


フックによるスタイル設定 (makeStyle)
----

Material-UI で、コンポーネントに独自スタイルを設定する方法としては、主に次の 3 種類の方法が用意されています。

- [Hook API](https://material-ui.com/styles/basics/#hook-api)
    - `makeStyle` 関数で生成したフック関数をコンポーネント内で呼び出す方法。一番よく使われてる。
- [Styled components API](https://material-ui.com/styles/basics/#styled-components-api)
    - 既存のコンポーネント (`Button` など）をラップする形で、スタイルを適用したコンポーネント（`MyButton` など）を作成する方法。
- [Higher-order component API](https://material-ui.com/styles/basics/#higher-order-component-api)
    - Styled components に似てるけど、HoC の仕組みでスタイル設定したコンポーネントを作成する方法。ちょっとわかりにくい。

ここでは、一番メジャーで分かりやすい、フックを利用したスタイル設定方法を紹介します。
次の例では、Material-UI の `Button` コンポーネントに、独自の CSS スタイル（`customButton` クラス）を適用しています。

{{< image border="true" w="200" src="img-001.png" >}}

{{< code lang="tsx" title="components/MyButton.tsx" >}}
import { Button } from '@material-ui/core'
import { makeStyles } from '@material-ui/core/styles'

const useStyles = makeStyles({
  customButton: {
    color: 'white',
    background: '#229966',
    padding: '1em 3em',
  },
})

const MyButton: React.FC = () => {
  const classes = useStyles()
  return <Button className={classes.customButton}>Button</Button>
}

export default MyButton
{{< /code >}}

ポイントは、[makeStyles 関数](https://material-ui.com/styles/api/#makestyles-styles-options-hook) で作成したフック関数を呼び出して、その戻り値のオブジェクトを使って各コンポーネントの __`className`__ プロパティを指定するところです。
基本はこれだけなので、このやり方に慣れてしまえば OK です。


Props によるスタイル設定の分岐
----

CSS-in-JS なスタイル記述方法の利点の一つとして、次のような動的なスタイル設定があります。
この例では、スタイル設定用のフック関数 (`useStyles`) に、`StyleProps` 型の引数を渡すことでテキストの色を切り替えています。

{{< code lang="tsx" title="components/MyLabel.tsx" >}}
import { Typography } from '@material-ui/core'
import { makeStyles, Theme } from '@material-ui/core/styles'

// スタイル定義用のフック関数が受け取るプロパティの型
type StyleProps = {
  textStyle: 'normal' | 'vivid'
}

const useStyles = makeStyles<Theme, StyleProps>({
  customText: {
    // 引数で渡されたオブジェクトの値で分岐処理できる
    color: (props) => (props.textStyle === 'normal' ? 'black' : '#ff6633'),
  },
})

const MyLabel: React.FC = () => {
  const classes = useStyles({ textStyle: 'vivid' })
  return <Typography className={classes.customText}>Hello</Typography>
}

export default MyLabel
{{< /code >}}

上記の例では、コンポーネントの実装内で `StyleProps` オブジェクトを作成して `useStyles` に渡していますが、コンポーネント自身の `Props` オブジェクトをそのまま渡してしまう方が一般的かもしれません。

{{< code lang="tsx" title="components/MyLabel.tsx" >}}
import { Typography } from '@material-ui/core'
import { makeStyles, Theme } from '@material-ui/core/styles'

type Props = {
  textStyle?: 'normal' | 'vivid'
  children: React.ReactNode
}

const useStyles = makeStyles<Theme, Props>({
  customText: {
    color: (props) => (props.textStyle === 'normal' ? 'black' : '#ff6633'),
  },
})

const MyLabel: React.FC<Props> = (props: Props) => {
  const classes = useStyles(props)
  return (
    <Typography className={classes.customText}>{props.children}</Typography>
  )
}

MyLabel.defaultProps = {
  textStyle: 'normal',
}

export default MyLabel
{{< /code >}}

こうすると、そのコンポーネントを使う側でスタイルを制御できるようになります。

{{< image border="true" w="200" src="img-002.png" >}}

{{< code lang="tsx" title="pages/index.tsx" >}}
import MyLabel from '../components/MyLabel'

const IndexPage: React.FC = () => {
  return (
    <div>
      <MyLabel>Default Label</MyLabel>
      <MyLabel textStyle="normal">Normal Label</MyLabel>
      <MyLabel textStyle="vivid">Vivid Label</MyLabel>
    </div>
  )
}

export default IndexPage
{{< /code >}}


オブジェクト間のマージン用にスタイル設定する
----

`makeStyles` で作成したスタイル設定は、Material-UI のコンポーネント以外にも適用できます。
次の例では、`div` 要素に独自スタイルを適用し、子要素のマージンを現在のテーマのスペース 1 つ分 (`theme.spacing(1)`) に設定しています。
スタイルオブジェクトを入れ子の形で定義すると、プロパティ名の部分で __`&`__ を使ってカレント要素を参照できます。

{{< image border="true" w="400" src="img-003.png" >}}

{{< code lang="tsx" >}}
import { Button } from '@material-ui/core'
import { makeStyles, Theme } from '@material-ui/core/styles'

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    '& > *': {
      margin: theme.spacing(1),
    },
  },
}))

const IndexPage: React.FC = () => {
  const classes = useStyles()

  return (
    <div className={classes.root}>
      <Button variant="contained">Button 1</Button>
      <Button variant="contained">Button 2</Button>
      <Button variant="contained">Button 3</Button>
    </div>
  )
}

export default IndexPage
{{< /code >}}

