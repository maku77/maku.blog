---
title: "Next.js でコンポーネント単位の CSS を作成する (CSS Modules)"
linkTitle: "コンポーネント単位の CSS を作成する (CSS Modules)"
url: "/p/seq2cmw"
date: "2021-05-06"
tags: ["Next.js"]
weight: 2
---

CSS Modules とは
----

__CSS Modules__ は、コンポーネント（ファイル）単位で CSS ファイルを分けて管理する仕組みです（これ自体は Next.js の仕組みではありません）。
CSS の名前空間がコンポーネントごとに分離されるため、シンプルな CSS クラス名を付けても名前がコンフリクトする心配がありません。

Next.js は標準で CSS Modules の仕組みをサポートしており、CSS ファイルの拡張子を __`.module.css`__ にするだけで、各コンポーネントの実装ファイルから簡単にインポートすることができます。


Next.js で CSS Modules 機能を使う
----

Next.js で特定のコンポーネント用に CSS ファイルを用意するときは、そのコンポーネントと同じディレクトリに __`.module.css`__ という拡張子を持つファイルを作成します。
この拡張子は、Next.js で CSS Modules の仕組みを使うときのルールです。

ここでは、警告メッセージを表示する `Alert` コンポーネント用の `Alert.module.css` を作成してみます。
といっても、普通に CSS ファイルを記述するだけです。
グローバルに適用する CSS ファイルと異なるのは、`Alert` コンポーネント用のスタイル定義しか含まれていないことです。

{{< code lang="css" title="components/Alert.module.css" >}}
.box {
  margin: 1em;
  padding: 0.5em;
  background: red;
  color: white;
  font-weight: bolder;
  border-radius: 0.5em;
}
{{< /code >}}

この CSS ファイルを `Alert` コンポーネントから使用するには、次のようにインポートします。

{{< code lang="tsx" title="components/Alert.tsx" >}}
import React from 'react'
import styles from './Alert.module.css'

type Props = {
  message: string;
}

export const Alert: React.FC<Props> = ({ message }) => {
  return (
    <div className={styles.box}>{message}</div>
  )
}
{{< /code >}}

上記の例では `.css` ファイルを使いましたが、Next.js は [Sass を標準サポート](/p/2n2doyb) しているので、`.scss` ファイルも同様にインポートすることができます。


TypeScript を使っているときに警告が出る場合
----

TypeScript を使ってコンポーネントを実装している場合、`css` ファイルをインポートしている部分で次のような警告が出ることがあります。

{{< code >}}
Cannot find module './Alert.module.css' or its corresponding type declarations.
{{< /code >}}

これは、`.css` ファイルの内容を `styles` オブジェクト経由で参照するときに、型情報が存在しないよという警告です。
この警告を抑制するには、プロジェクトのルートにある型情報ファイル (__`next-env.d.ts`__) に次のように追記します。

{{< code lang="ts" title="next-env.d.ts" >}}
/// <reference types="next" />
/// <reference types="next/types/global" />

declare module '*.css' {
  const styles: { [className: string]: string };
  export default styles;
}

declare module '*.scss' {
  const styles: { [className: string]: string };
  export default styles;
}
{{< /code >}}

この記述により、TypeScript コンパイラに対して、「`styles` オブジェクトは文字列型のパラメーターを持っているよ」ということを知らせることができます。
ここでは、将来的に [Sass を導入](/p/2n2doyb) したときのために、`.scss` ファイルにも同様の型情報を追加しています。

{{< reference >}}
- [既存の JavaScript ライブラリに型情報を追加する（.d.ts ファイル）](/p/s7wk5k3)
{{< /reference >}}

