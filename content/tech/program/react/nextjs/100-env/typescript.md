---
title: "Next.js のプロジェクトを TypeScript 化する"
url: "/p/ny9fmty"
date: "2021-04-26"
tags: ["Next.js", "React"]
weight: 102
---

何をするか？
----

`create-next-app` コマンドで Next.js プロジェクトを生成するときに __`--typescript (--ts)`__ オプションをつけて実行すると、TypeScript 対応したプロジェクトを生成することができます。

{{< code >}}
$ npx create-next-app myapp --typescript

# ただ、こっちのテンプレートを使う方法の方が若干洗練されてる気はする
$ npx create-next-app myapp --example with-typescript
{{< /code >}}

また、Next.js は既存の JavaScript プロジェクトを TypeScript 化する機能も備えています。
ここから先は、その方法を説明します。

{{% reference %}}
- [Next.js で HelloWorld](/p/ubmu3bj)
{{% /reference %}}


TypeScript 環境の導入
----

Next.js は、プロジェクトのルートに __`tsconfig.json`__ ファイルがあると、自動的に TypeScript モードで動作するようになります。
次のようにして、空の `tsconfig.json` ファイルを作成し、TypeScript 用のパッケージをインストールします。

{{< code >}}
$ touch tsconfig.json
$ npm install typescript @types/node @types/react --save-dev
{{< /code >}}

なんと、これだけで Next.js プロジェクトへの TypeScript 導入は完了です。
お手軽〜


自動生成される設定ファイルを見ておく
----

上記のステップで、TypeScript の設定ファイル (`tsconfig.json`) に何も記述していないことに気づいたかもしれません。
実は、Next.js のビルド（`next dev` や `next build`）を実行すると、自動的に設定ファイルの内容を更新してくれるようになっています。

{{< code title="Next.js 開発サーバーの起動 (next dev)" >}}
$ npm run dev
{{< /code >}}

例えば上記のように開発サーバーを起動すると、空っぽだった `tsconfig.json` が次のような内容に初期化されます。
まさにゼロコンフィグです。

{{< accordion title="tsconfig.json（自動更新された内容）" >}}
{{< code lang="json" >}}
{
  "compilerOptions": {
    "target": "es5",
    "lib": [
      "dom",
      "dom.iterable",
      "esnext"
    ],
    "allowJs": true,
    "skipLibCheck": true,
    "strict": false,
    "forceConsistentCasingInFileNames": true,
    "noEmit": true,
    "esModuleInterop": true,
    "module": "esnext",
    "moduleResolution": "node",
    "resolveJsonModule": true,
    "isolatedModules": true,
    "jsx": "preserve"
  },
  "include": [
    "next-env.d.ts",
    "**/*.ts",
    "**/*.tsx"
  ],
  "exclude": [
    "node_modules"
  ]
}
{{< /code >}}
{{< /accordion >}}

さらに、Next.js はプロジェクトのルートに __`next-env.d.ts`__ というファイルを生成します。
このファイルには、Next.js が提供するコンポーネントなどの型情報が含まれています。

{{< code type="ts" title="next-env.d.ts" >}}
/// <reference types="next" />
/// <reference types="next/types/global" />
{{< /code >}}

デフォルトでは上記のような内容になっていて、`node_modules/next` 下にインストールされた型情報ファイルを参照するようになっています。
このファイルは Next.js が勝手に更新するので、マニュアルで更新する必要はありません。


既存の JavaScript コードを TypeScript コードに置き換える
----

### 拡張子を .js から .tsx に置換する

TypeScript 環境の導入をサクッと完了したら、既存の JavaScript ファイル (`.js`) を TypeScript ファイル（__`.ts`__ および __`.tsx`__）に置き換えていきます。
JavaScript ファイルは `pages` ディレクトリ以下に格納されているので、まずは、それらのファイルの拡張子を変更します。

{{< code >}}
pages/
  +-- _app.js  (→ _app.tsx)
  +-- api/
  |     +-- hello.js  (→ hello.ts)
  +-- index.js  (→ index.tsx)
{{< /code >}}

`api` ディレクトリの中の `.js` ファイルには JSX が含まれていないので、それだけは `.ts` にして、他のファイルは `.tsx` にしてしまえばよいでしょう。

### 厳格な型チェック（strict モード）を ON にする

ファイルの拡張子を `.ts` と `.tsx` にしたので、ビルド時に TypeScript の型チェックが働くようになるはずが、`npm run dev` や `npm run build` してもビルドエラーにはなりません。
なぜなら、`tsconfig.json` の __`strict`__ 設定が false になっており、暗黙的な `any` 型の使用が許されているからです。
次のように `tsconfig.json` を修正して、型チェックを強制するようにしましょう。
ついでに、`allowJs` はもう必要ないので false にしておきます。

{{< code lang="js" title="tsconfig.json（修正箇所抜粋）" >}}
{
  ...
  "allowJs": false,  // ← js ファイルは扱わない
  "skipLibCheck": true,
  "strict": true,  // ← 型チェックなどを厳密に行う
  ...
}
{{< /code >}}

これで、ビルド時にちゃんと型情報に関するエラーを出してくれるようになります。

{{< code >}}
$ npm run build
...
Failed to compile.

./pages/_app.tsx:3:18
Type error: Binding element 'Component' implicitly has an 'any' type.
...
{{< /code >}}

### TypeScript コードの修正

ビルドエラーにならないように、TypeScript コードに型情報を追加していきます。

{{< code lang="tsx" title="pages/_app.tsx" >}}
import type { AppProps } from 'next/app'

function MyApp({ Component, pageProps }: AppProps): JSX.Element {
  return <Component {...pageProps} />
}

export default MyApp
{{< /code >}}

{{< code lang="ts" title="pages/api/hello.ts" >}}
import type { NextApiRequest, NextApiResponse } from 'next'

export default (req: NextApiRequest, res: NextApiResponse) => {
  res.status(200).json({ name: 'John Doe' })
}
{{< /code >}}

これで、正しくビルドが通るようになります。
Next.js が提供する他の型情報に関しては、下記のサイトを参考にしてください。

{{% reference %}}
- [Basic Features: TypeScript | Next.js](https://nextjs.org/docs/basic-features/typescript)
{{% /reference %}}


おまけ（example オプションでさまざまな環境が整ったプロジェクトを作成する）
----

`create-next-app` コマンドで Next.js プロジェクトを作成するときに、__`--example`__ オプションを使用すると、GitHub 上の任意のテンプレートを使ってプロジェクトを作成できます。
Next.js (Vercel) 公式のテンプレートが、下記のリポジトリにたくさん用意されています。

- https://github.com/vercel/next.js/tree/master/examples

例えば、TypeScript + ESLint + Jest 導入済みのテンプレートである __`with-typescript-eslint-jest`__ を使って Next.js プロジェクトを作成したいときは、次のように `create-next-app` コマンドを実行します。

{{< code >}}
$ npx create-next-app myapp --example with-typescript-eslint-jest
{{< /code >}}

こうすれば、最初から色々な環境が整った状態でプロジェクトが作成されるので、すぐに本質的な開発作業に取りかかることができます。
とはいえ、 ひとつずつ手作業で導入していった方が理解しやすいという側面もあるので、好きな導入方法を選べばよいと思います。

