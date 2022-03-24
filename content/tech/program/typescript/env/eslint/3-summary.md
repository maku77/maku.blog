---
title: "ESLint + Prettier の設定方法まとめ"
linkTitle: "ESLint (3) ESLint + Prettier の設定方法まとめ"
url: "/p/yfow6dk"
date: "2021-05-23"
tags: ["ESLint", "Prettier"]
weight: 303
---

ここでは、とにかく混乱しがちな __ESLint + Prettier__ の設定方法をまとめておきます。
まず、基本的な構成として、__TypeScript (with React)__ の Lint をかけられるようにして、__必要に応じて Airbnb スタイル__ を追加で設定する、という流れにしたいと思います。


TypeScript プロジェクトの準備
----

前提条件として、TypeScript (+ React) のプロジェクト自体は作成済みとします。
何も準備できていなければ、Next.js の `create-next-app` コマンドを使うと簡単に作成できます（`typescript` および `@types/node`、`@types/react`、`@types/react-dom` などの型情報が導入された状態のプロジェクトが作成されます）。

{{< code title="2 つやり方あるけど、大体中身は同じ" >}}
# テンプレートを使う方法
$ create-next-app myapp --example with-typescript

# typescript オプションを使う方法
$ create-next-app myapp --typescript
{{< /code >}}

{{% reference %}}
- [Next.js で HelloWorld（プロジェクト作成からサーバー起動まで）](/p/ubmu3bj)
- [Next.js のプロジェクトを TypeScript 化する](/p/ny9fmty)
{{% /reference %}}


Prettier + ESLint の基本設定 (for TypeScript with React)
----

{{< code title="必要なパッケージのインストール" >}}
# Prettier 本体
$ yarn add --dev --exact prettier

# ESLint 本体
$ yarn add --dev eslint

# ESLint 用のプラグインおよび共有設定
$ yarn add --dev @typescript-eslint/eslint-plugin
$ yarn add --dev @typescript-eslint/parser
$ yarn add --dev eslint-plugin-react
$ yarn add --dev eslint-plugin-react-hooks
$ yarn add --dev eslint-config-prettier

# npm スクリプトの並列実行用
$ yarn add --dev npm-run-all
{{< /code >}}

{{< accordion title="npm の場合" >}}
{{< code >}}
# Prettier 本体
$ npm install prettier --save-dev --save-exact

# ESLint 本体
$ npm install eslint --save-dev

# ESLint 用のプラグインおよび共有設定
$ npm install @typescript-eslint/eslint-plugin --save-dev
$ npm install @typescript-eslint/parser --save-dev
$ npm install eslint-plugin-react --save-dev
$ npm install eslint-plugin-react-hooks --save-dev
$ npm install eslint-config-prettier --save-dev

# npm スクリプトの並列実行用
$ npm install npm-run-all --save-dev
{{< /code >}}
{{< /accordion >}}

{{< code lang="yaml" title=".prettierrc（必要に応じて Prettier フォーマット設定）" >}}
semi: false # 行末のセミコロンは省略する
singleQuote: true # 引用符にはシングルクォートを使う
{{< /code >}}

{{< code title=".prettierignore（Prettier の無視設定）" >}}
.next/
build/
*.html
{{< /code >}}

{{< code lang="yaml" title=".eslintrc.yml（ESLint 設定）" >}}
root: true
env:
  browser: true
  es2021: true
extends:
  - eslint:recommended
  - plugin:react/recommended
  - plugin:react-hooks/recommended
  - plugin:@typescript-eslint/recommended
  - plugin:@typescript-eslint/recommended-requiring-type-checking
  - prettier
parser: "@typescript-eslint/parser"
parserOptions:
  ecmaVersion: 12
  ecmaFeatures:
    jsx: true
  project: ./tsconfig.json
plugins:
  - react
  - react-hooks
  - "@typescript-eslint"
rules:
  react/react-in-jsx-scope: off
{{< /code >}}

{{< code lang="js" title="package.json（npm スクリプト設定）" >}}
// ...
  scripts: {
    "lint": "run-p -c -l lint:**",
    "lint:prettier": "prettier --check .",
    "lint:eslint": "eslint . --ext .ts,.tsx",
    "lint:tsc": "tsc",
    "fix": "run-s -l fix:**",
    "fix:prettier": "prettier --write .",
    "fix:eslint": "eslint . --ext .ts,.tsx --fix",
{{< /code >}}

これで、次のようにして Prettier + ESLint による Lint チェックと自動修正をかけられるようになります。

{{< code title="Prettier と ESLint の実行" >}}
# yarn の場合
$ yarn lint  # Lint チェック
$ yarn fix   # 自動修正

# npm の場合
$ npm run lint  # Lint チェック
$ npm run fix   # 自動修正
{{< /code >}}

`fix` を実行すると多数のファイルが自動修正されることになるので、`lint` が実行できた段階で、一度 Git へ設定をコミットしておくことをオススメします。


ts/tsx ファイルを src ディレクトリにまとめる
----

[Next.js 9.1](https://nextjs.org/blog/next-9-1) からは、`pages` ディレクトリを __`src/pages`__ というパスで作成できるようになりました。
`next` コマンドは、プロジェクトのルートに `pages` が見つからないとき、`src/pages` を参照してくれます。

この仕組みを利用すると、`.ts`、`.tsx` ファイルは全て `src` ディレクトリ以下にまとめられるので、プロジェクトのルートディレクトリがスッキリします。
ただし、`tsconfig.json` や `public` ディレクトリは、これまで通りルートディレクトリに配置する必要があります。

{{< code title=".ts/.tsx ファイルの入ったディレクトリを src 以下にまとめる" >}}
$ mkdir src
$ git mv components src
$ git mv interfaces src
$ git mv pages src
$ git mv utils src
{{< /code >}}

あとは、各種コマンドの対象パスなどを変更すれば OK です。

{{< code lang="diff" title="package.json" >}}
   "scripts": {
     "lint": "run-p -c -l lint:**",
-    "lint:prettier": "prettier --check .",
-    "lint:eslint": "eslint . --ext .ts,.tsx",
+    "lint:prettier": "prettier --check src",
+    "lint:eslint": "eslint src --ext .ts,.tsx",
     "lint:tsc": "tsc",
     "fix": "run-s -l fix:**",
-    "fix:prettier": "prettier --write .",
-    "fix:eslint": "eslint . --ext .ts,.tsx --fix",
+    "fix:prettier": "prettier --write src",
+    "fix:eslint": "eslint src --ext .ts,.tsx --fix",
{{< /code >}}

{{< code lang="diff" title="tsconfig.json" >}}
     "target": "esnext"
   },
   "exclude": ["node_modules"],
-  "include": ["**/*.ts", "**/*.tsx"]
+  "include": ["src/**/*.ts", "src/**/*.tsx"]
 }
{{< /code >}}


ここまでの結果をテンプレートにする
----

ここで作成したプロジェクトを GitHub にプッシュしておけば、`create-next-app` コマンドのテンプレートとして使用できるようになります。
次のように、__`--example`__ オプションで GitHub リポジトリの URL を指定するだけで OK です。

{{< code title="例: TypeScript、ESLint、Prettier 設定済みのプロジェクトを生成" >}}
$ create-next-app myapp --example \
  https://github.com/maku77/template-nextjs-ts-eslint-prettier
{{< /code >}}


Airbnb スタイルを追加導入する
----

JavaScript のコーディングスタイルとしては、Airbnb のスタイルが有名です。
ここでは、前述の基本的な ESLint + Prettier 設定が終わっている想定で、追加で Airbnb スタイルを導入してみます。

Airbnb は ESLint 用の [eslint-config-airbnb](https://www.npmjs.com/package/eslint-config-airbnb) 共有設定を用意しているのですが、これをそのまま使うと TypeScript 用のカスタマイズが厄介なので、最初から TypeScript 用の対応が入った [eslint-config-airbnb-typescript](https://www.npmjs.com/package/eslint-config-airbnb-typescript) を使うことにします。

公式サイトを見ると、`eslint-config-airbnb-typescript` の依存パッケージとして下記が必要ということが分かるので、まだインストールしていないものを追加でインストールします。

- eslint-plugin-import
- eslint-plugin-jsx-a11y
- eslint-plugin-react （インストール済み）
- eslint-plugin-react-hooks （インストール済み）
- @typescript-eslint/eslint-plugin （インストール済み）

{{< code title="eslint-config-airbnb-typescript のインストール" >}}
$ yarn add --dev eslint-config-airbnb-typescript
$ yarn add --dev eslint-plugin-import
$ yarn add --dev eslint-plugin-jsx-a11y
{{< /code >}}

{{< accordion title="npm の場合" >}}
{{< code >}}
$ npm install eslint-config-airbnb-typescript --save-dev
$ npm install eslint-plugin-import --save-dev
$ npm install eslint-plugin-jsx-a11y --save-dev
{{< /code >}}
{{< /accordion >}}

{{< code lang="yaml" title=".eslintrc.yml（airbnb-typescript を使うよう修正）" >}}
root: true
env:
  browser: true
  es2021: true
extends:
  - airbnb-typescript
  - prettier
parser: "@typescript-eslint/parser"
parserOptions:
  ecmaFeatures:
    jsx: true
  ecmaVersion: 12
  project: ./tsconfig.json
plugins:
  - react
  - "@typescript-eslint"
rules:
  react/react-in-jsx-scope: off  # JSX 用の React インポートエラー抑制
{{< /code >}}

実行方法はこれまで通りで変化なしです（`npm run lint` と `npm run fix`）。


ESLint と Prettier の設定で気を付けること
----

ESLint および Prettier の設定のポイントや、ハマりがちなことをまとめておきます。

### TypeScript 型情報のチェックを ESLint で行う

`@typescript-eslint/eslint-plugin` プラグインが提供する __`recommended-requiring-type-checking`__ という共有設定を有効にすると、TypeScript の型情報を利用した Lint チェックを行うことができます。
このとき、内部では TypeScript コンパイラが使用されるため、__`parserOptions.project`__ で `tsconfig.json` ファイルの位置を知らせてやる必要があります（指定しないとエラーになります）。

{{< code lang="yaml" title=".eslintrc.yml" >}}
root: true
parser: '@typescript-eslint/parser'
parserOptions:
  ecmaVersion: 2021
  project: ./tsconfig.json  # ★型チェックに必要
plugins:
  - '@typescript-eslint'
extends:
  - eslint:recommended
  - plugin:@typescript-eslint/recommended
  - plugin:@typescript-eslint/recommended-requiring-type-checking  # ★型チェック
{{< /code >}}

TypeScript の型チェックを行うよう設定すると、追加で TypeScript コンパイラが動作するようになるため、ESLint の実行に時間がかかるようになります。
`tsconfig.json` の情報をそのまま使うのではなく、ESLint 用に `tsconfig.eslint.json` を作成してパフォーマンス向上させる方法が提案されています。
いずれにしても、型チェックの仕組みはあまり洗練されているとは言えないので、型チェックに関しては TypeScript コードの `tsc` ビルド時にだけ行うと割り切ってもいいかもしれません（VS Code デフォルトの検出機能もあります）。

{{% reference %}}
- [typescript-eslint - Getting Started - Linting with Type Information](https://github.com/typescript-eslint/typescript-eslint/blob/master/docs/getting-started/linting/TYPED_LINTING.md)
{{% /reference %}}


### TypeScript / Prettier の設定は後ろへ

{{< code lang="yaml" title=".eslintrc.yml" >}}
plugin:
  - react
  - @typescript-eslint
extends:
  - eslint:recommended
  - plugin:react/recommended
  - plugin:@typescript-eslint/recommended
  - plugin:@typescript-eslint/recommended-requiring-type-checking
{{< /code >}}

TypeScript 用の共有設定 `plugin:@typescript-eslint/recommended` は、`eslint:recommended` などにより定義されるルールのうち、TypeScript と競合するものを解決してくれます。
なので、TypeScript に関する設定は `extends` の中で最後の方に指定して設定を上書きする必要があります。
具体的には、`plugin:@typescript-eslint/eslint-recommended` がこの競合を解決するものであり、`plugin:@typescript-eslint/recommended` から利用されています。

同様に、コードフォーマッターの [Prettier](https://prettier.io/) を使用する場合は、Prettier が自動修正してくれる部分を ESLint が指摘しないように、一番最後に [eslint-config-prettier](https://www.npmjs.com/package/eslint-config-prettier) 関連の共有設定を指定する必要があります。

{{< code lang="yaml" >}}
extends:
  - eslint:recommended
  - plugin:react/recommended
  - plugin:@typescript-eslint/recommended
  - plugin:@typescript-eslint/recommended-requiring-type-checking
  - prettier
{{< /code >}}

{{% reference %}}
- [typescript-eslint/typescript-eslint - Usage with prettier](https://github.com/typescript-eslint/typescript-eslint/blob/master/docs/getting-started/linting/README.md#usage-with-prettier)
{{% /reference %}}


### Airbnb スタイルは React 込み

{{< code lang="diff" title=".eslintrc.yml" >}}
  extends:
-   - eslint:recommended
-   - plugin:react/recommended
+   - airbnb
+   - airbnb/hooks
{{< /code >}}

`extends` プロパティで `airbnb` ([eslint-config-airbnb](https://www.npmjs.com/package/eslint-config-airbnb)) を指定する場合、`eslint:recommended` や `plugin:react/recommeded` の設定は `airbnb` 側でほぼカバーされるので、指定する必要はありません。

逆に、React を使わない場合は、`eslint-config-airbnb` ではなく [eslint-config-airbnb-base](https://www.npmjs.com/package/eslint-config-airbnb-base) の方を使います。


### eslint-config-airbnb が依存するパッケージを調べる

`.eslintrc.*` ファイルの `extends` プロパティで __`airbnb`__ を指定するには、あらかじめ [eslint-config-airbnb](https://www.npmjs.com/package/eslint-config-airbnb) パッケージをインストールするだけでなく、その依存パッケージ (peerDependencies) もインストールしておく必要があります。
依存パッケージは次のように確認することができます。

{{< code >}}
$ npm info "eslint-config-airbnb@latest" peerDependencies
{
  eslint: '^5.16.0 || ^6.8.0 || ^7.2.0',
  'eslint-plugin-import': '^2.22.1',
  'eslint-plugin-jsx-a11y': '^6.4.1',
  'eslint-plugin-react': '^7.21.5',
  'eslint-plugin-react-hooks': '^4 || ^3 || ^2.3.0 || ^1.7.0'
}
{{< /code >}}

