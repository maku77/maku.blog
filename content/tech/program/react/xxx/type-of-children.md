---
title: "React の props.children の型定義には ReactNode を使う"
linkTitle: ""
url: "/p/xenv4bh"
date: "2021-07-18"
tags: ["React", "TypeScript"]
---

children の型定義
----

TypeScript で React の関数コンポーネントを定義するときには、下記のような [React.FC (React.FunctionComponent)](https://github.com/DefinitelyTyped/DefinitelyTyped/blob/33988371025ae2e58bdaa4391d6661d4ef273242/types/react/index.d.ts#L543) を使用します。

{{< code lang="js" >}}
type FC<P = {}> = FunctionComponent<P>;

interface FunctionComponent<P = {}> {
    (props: PropsWithChildren<P>, context?: any): ReactElement<any, any> | null;
    propTypes?: WeakValidationMap<P> | undefined;
    contextTypes?: ValidationMap<any> | undefined;
    defaultProps?: Partial<P> | undefined;
    displayName?: string | undefined;
}

// ...
type PropsWithChildren<P> = P & { children?: ReactNode | undefined };
{{< /code >}}

`FC` の型パラメータ `P` は、上記のような `PropsWithChildren` 型にラップされるので、`props` の型定義をするときに明示的に `children` を含める必要はありません。
下記の `ColorBox` コンポーネントは、指定した背景色で子要素 (`children`) を表示します。

{{< code lang="tsx" title="ColorBox.tsx" >}}
import { FC } from 'react'

type Props = {
  color: string
}

export const ColorBox: FC<Props> = ({ color, children }) => {
  return <div style={{ background: color }}>{children}</div>
}
{{< /code >}}

ただ、これだと、`ColorBox` コンポーネントの型情報を見ただけでは、子要素 (`children`) が必要なのかどうかを判別することができません。
`props` の型定義で、明示的に `children` の型情報を指定するには、次のように __`React.ReactNode`__ を使います。

{{< code lang="tsx" hl_lines="1 5" >}}
import { FC, ReactNode } from 'react'

type Props = {
  color: string
  children: ReactNode
}

export const ColorBox: FC<Props> = ({ color, children }) => {
  return <div style={{ background: color }}>{children}</div>
}
{{< /code >}}

これで、子要素を指定せずに `ColorBox` コンポーネントを使おうとしたときにエラー表示してくれるようになります。

{{< code lang="tsx" >}}
<ColorBox color="red" />  // 子要素がないのでエラー！
{{< /code >}}

ちなみに、子要素の指定がオプショナルであることを明示するには、次のように `children?` と定義すれば OK です。
これであれば何も定義しないのと同じでは？と思うかもしれませんが、この定義があることで、子要素を指定したときに確実に何らかの描画が行われるということを示すことができます。

{{< code lang="tsx" hl_lines="3" >}}
type Props = {
  color: string
  children?: ReactNode
}
{{< /code >}}


（おまけ） React.ReactElement や JSX.Element との違い
----

上記で `children` の型定義に使った `React.ReactNode` 以外にも、次のような似たような型があるのでまとめて起きます。

### ReactElement / JSX.Element

React コンポーネントが生成する描画要素（`<MyComponent>` など）を表す型です。
関数コンポーネント (`FC`) の戻り値は、この `ReactElement` です。

（参考: [DefinitelyTyped の定義](https://github.com/DefinitelyTyped/DefinitelyTyped/blob/33988371025ae2e58bdaa4391d6661d4ef273242/types/react/index.d.ts#L146)）

{{< code lang="ts" >}}
interface ReactElement<
  P = any,
  T extends string | JSXElementConstructor<any>
    = string | JSXElementConstructor<any>
> {
  type: T
  props: P
  key: Key | null
}

type Key = string | number;
{{< /code >}}

`React.FC` インタフェースの定義を見ると、関数の戻り値として `ReactElement` が使われていることを確認できます。
TypeScript の型定義に慣れないと最初は理解しにくいかもしれませんが、下記の `interface` 定義の 1 つ目のプロパティが、関数コンポーネントのコールシグネチャを表しています。

{{< code lang="js" hl_lines="4" >}}
type FC<P = {}> = FunctionComponent<P>

interface FunctionComponent<P = {}> {
  (props: PropsWithChildren<P>, context?: any): ReactElement<any, any> | null
  propTypes?: WeakValidationMap<P>
  contextTypes?: ValidationMap<any>
  defaultProps?: Partial<P>
  displayName?: string
}
{{< /code >}}

`JSX.Element` もほぼ同じもので、次のようなエイリアスなので、関数コンポーネントの戻り値の型として使用できます。
コンポーネントの実装において、部分的な要素を生成するユーティリティ関数の戻り値としても使用できます。

{{< code lang="js" >}}
namespace JSX {
  interface Element extends React.ReactElement<any, any> { }
  // ...
}
{{< /code >}}

### ReactChild

`ReactChild` は、`ReactElement`、文字列、数値のいずれかを表現する型です。
（参考: [DefinitelyTyped の定義](https://github.com/DefinitelyTyped/DefinitelyTyped/blob/33988371025ae2e58bdaa4391d6661d4ef273242/types/react/index.d.ts#L233)）

{{< code lang="ts" >}}
type ReactChild = ReactElement | ReactText
type ReactText = string | number
{{< /code >}}

一見これを `children` の型として使用できそうですが、`ReactChild` はあくまで 1 つの要素を表すものなので、複数の要素を指定できる `children` の型としては使用できません。

### ReactNode

`ReactNode` は次のように定義されており、1 つ以上のコンポーネントやプリミティブ値を表します。
簡単にいうと、JSX コードの中の一部分はこの `ReactNode` で表現できます。
つまり、`children` の型としてはこの `ReactNode` を使うことができます。

{{< code lang="ts" >}}
type ReactNode =
  | ReactChild
  | ReactFragment
  | ReactPortal
  | boolean
  | null
  | undefined

type ReactFragment = {} | ReactNodeArray
interface ReactNodeArray extends Array<ReactNode> {}
interface ReactPortal extends ReactElement {
  key: Key | null
  children: ReactNode
}
{{< /code >}}

### まとめ

各タイプを粒度の観点で整理すると次のような感じです。

- `ReactNode` （1 つ以上の React コンポーネント or 文字列 or 数値）
    - `ReactChild` （1 つの React コンポーネント or 文字列 or 数値）
        - `ReactElement` / `JSX.Element` （1 つの React コンポーネント）
        - `ReactText` （文字列 or 数値）

