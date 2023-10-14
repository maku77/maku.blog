---
title: "TypeScript プロジェクトに ESLint を導入する"
linkTitle: "ESLint (1) ESLint を導入する"
url: "p/xz9iry9/"
date: "2021-05-20"
tags: ["ESLint"]
weight: 301
---

ESLint とは
----

[ESLint](https://eslint.org/) は JavaScript/TypeScript 向けの Lint ツール（静的解析ツール）です。
プロジェクト内のソースコードに対して __`eslint`__ コマンドを実行することで、コーディングルールに従っていない部分や、不具合の原因になりそうな部分を検出してくれます。

{{% note title="eslint vs tslint" %}}
もともと TypeScript 用には `tslint` という Lint ツールが提供されていましたが、Microsoft の TypeScript チームが `eslint` への統合を [公式に発表](https://eslint.org/blog/2019/01/future-typescript-eslint) してから（2019年以降）は、TypeScript においても `eslint` が事実上の標準 Lint ツールとなっています。参考: [TypeScript の ESLint 対応プロジェクト](https://typescript-eslint.io/)
{{% /note %}}


ESLint のインストール
----

`eslint` コマンドは `npm` でインストールできます。
TypeScript プロジェクトのディレクトリ内で、次のように開発用にインストールすることが推奨されています（`package.json` は作成済みだと想定します）。

{{< code title="ESLint のインストール" >}}
$ npm install eslint --save-dev
{{< /code >}}


ESLint の初期設定
----

`eslint` コマンドを実行するためには、設定ファイル __`.eslintrc.{js,yml,json}`__ を作成しておく必要があります。

{{< code title="設定ファイルの生成" >}}
$ npx eslint --init
{{< /code >}}

上記のように実行すると、ウィザード形式でどのような用途に使用するかを質問されるので、順番に答えていくだけで設定ファイルを生成できます。
最後にその構成に必要なモジュールをまとめてインストールできます。
ここでは、eslint v7.26.0 における表示例を示しています。

{{< code title="質問1" >}}
? How would you like to use ESLint? …
  To check syntax only
  To check syntax and find problems
❯ To check syntax, find problems, and enforce code style
{{< /code >}}

ここでは「構文チェック」「問題の発見」「スタイル強制」をすべて行いたいので、カーソルキーで一番下の項目を選んで `Enter` キーを押します。

{{< code title="質問2" >}}
? What type of modules does your project use? …
❯ JavaScript modules (import/export)
  CommonJS (require/exports)
  None of these
{{< /code >}}

モジュールのインポート形式には `import/export` を選択します。

{{< code title="質問3" >}}
? Which framework does your project use? …
❯ React
  Vue.js
  None of these
{{< /code >}}

React.js や Vue.js を使うのであれば、ここで選択します。

{{< code title="質問4" >}}
? Does your project use TypeScript?  No / Yes
{{< /code >}}

TypeScript を使うかどうかを聞かれるので、`Yes` を選択します。

{{< code title="質問5" >}}
? Where does your code run? …
✔ Browser
  Node
{{< /code >}}

実行環境を選択します（スペースキーで選択をトグルします）。
コードの中で、どのようなグローバルオブジェクトを参照するかに応じて選択します。
Web ブラウザ上で動作させる JavaScript コードであれば `Browser` を選択し、Node.js 環境で動作させるコードであれば `Node` を選択します。

{{< code title="質問6" >}}
? How would you like to define a style for your project? …
❯ Use a popular style guide
  Answer questions about your style
  Inspect your JavaScript file(s)
{{< /code >}}

最初の質問で一番下の `enforce code style` を選んでいると、ここでどのようなコーディングスタイルを使うかを質問されます。
ここでは、ポピュラーなスタイルをそのまま使うことにします。

{{< code title="質問7" >}}
? Which style guide do you want to follow? …
  Airbnb: https://github.com/airbnb/javascript
❯ Standard: https://github.com/standard/standard
  Google: https://github.com/google/eslint-config-google
  XO: https://github.com/xojs/eslint-config-xo
{{< /code >}}

実際にどのスタイルを提供するかを指定します。
有名どころの Airbnb スタイルや JavaScript Standard スタイルなどを選択できます。
ここでは、行末のセミコロンを省略するスタイルである `Standard` を選択することにします。
参考: [TypeScript のコーディング規約（ルール／ガイドライン）](/p/avssq37)

{{< code title="質問8" >}}
? What format do you want your config file to be in? …
  JavaScript
❯ YAML
  JSON
{{< /code >}}

コンフィグファイルの形式を選択します。
基本的には一番簡潔に記述できる `YAML` 形式を選び、何らかの動的処理が必要な場合のみ `JavaScript` 形式を選べばよいと思います。
`JSON` 形式は記述が冗長になるし、コメントも記述できないので、選択しないようにしましょう。

{{< code title="質問9" >}}
The config that you've selected requires the following dependencies:

eslint-plugin-react@latest @typescript-eslint/eslint-plugin@latest eslint-config-standard@latest eslint@^7.12.1 eslint-plugin-import@^2.22.1 eslint-plugin-node@^11.1.0 eslint-plugin-promise@^4.2.1 @typescript-eslint/parser@latest
? Would you like to install them now with npm?  No / Yes
{{< /code >}}

必要なパッケージの一覧が表示されるので、`Yes` を選択してインストールします。
これらの依存情報は、`package.json` の `devDependencies` に自動的に追加されます。

これで、ESLint の初期設定は完了です。
作成された `.eslintrc.yml` の内容は次のような感じになっています。

{{< code lang="yaml" title=".eslintrc.yml" >}}
env:
  browser: true
  es2021: true
extends:
  - 'plugin:react/recommended'
  - standard
parser: '@typescript-eslint/parser'
parserOptions:
  ecmaFeatures:
    jsx: true
  ecmaVersion: 12
  sourceType: module
plugins:
  - react
  - '@typescript-eslint'
rules: {}
{{< /code >}}


ESLint の実行
----

カレントディレクトリ以下の `.ts` および `.tsx` ファイルに対して、ESLint をかけるには次のように実行します。

{{< code >}}
$ npx eslint . --ext ts,tsx
{{< /code >}}

標準出力に警告がずらずらと表示されるはずです。
上記コマンドは、NPM スクリプトとして `package.json` に登録しておくと楽です。

{{< code lang="js" title="package.json（抜粋）" >}}
{
  //...
  "scripts": {
    "lint:eslint": "eslint . --ext ts,tsx",
    "fix:eslint": "eslint . --ext ts,tsx --fix",
{{< /code >}}

ここでは、単純にチェックだけかける `lint:eslint` コマンドと、自動修正までしてしまう `fix:eslint` コマンドを定義してみました。
これで、次のように簡単に ESLint を実行できるようになります。

{{< code >}}
$ npm run lint:eslint  # ESLint の実行
$ npm run fix:eslint   # 自動修正
{{< /code >}}

npm スクリプト名を単純に `lint`、`fix` にしていないのは、追加で Prettier（自動整形ツール）を導入することを想定したものです。
Prettier の方にも同様の機能があるため、`lint:prettier`、`fix:prettier` という名前を付ける余地を残しています。

{{% maku-common/reference %}}
- [TypeScript/JavaScript コードを Prettier で自動整形する](/p/au8iu6u)
{{% /maku-common/reference %}}


（おまけ）VS Code への ESLint プラグインのインストール
----

エディターとして VS Code を使用している場合は、VS Code 用の ESLint プラグインもインストールしておきましょう。
次のような機能が有効になります。

- 編集中にエディター上にエラー表示
- エラーの一覧を Output パネル (`Ctrl + Shift + M`) に表示
- 上記それぞれから Quick Fix 可能

### ESLint プラグインのインストールと有効化

{{< image border="true" src="img-001.png" >}}

- [ESLint - Visual Studio Marketplace](https://marketplace.visualstudio.com/items?itemName=dbaeumer.vscode-eslint)

ESLint プラグインは上記のサイトあるいは、VS Code の `Extensions` メニューからワンクリックでインストールできます。
ESLint プラグインをインストールした直後は、次のような警告が出て ESLint が有効になっていない可能性があります。

{{< image border="true" src="img-002.png" >}}

その場合は、VS Code 右下の `ESLint` という部分をクリックして有効化できます。

{{< image border="true" src="img-003.png" >}}

### VS Code のフォーマッター設定

VS Code には、JavaScript/TypeScript のフォーマッターがデフォルトで搭載されています。
これらのフォーマットスタイルは ESLint で設定したスタイルに合わないことがあるので、ESLint のスタイル設定を優先するように設定しておきます。
__`Ctrl + ,`__ で設定画面を開いて、__`Formatter`__ などで検索すると設定項目を見つけられます。

{{< image border="true" src="img-004.png" >}}

これで、__`Shift + Alt + F`__（あるいは `Ctrl + Shift + P` → `Format Document`）で編集中ファイルのフォーマットを実行できます。


（おまけ）React 用の設定
----

最新の React では、TypeScript コード内に JSX が含まれていても `React` モジュールをインポートする必要がなくなっています。
ESLint がこれを認識できずに、次のようなエラーを出力することがあります。

{{< code >}}
4:10  error  'React' must be in scope when using JSX  react/react-in-jsx-scope
{{< /code >}}

このチェックを無効にするには、設定ファイルの __`rules`__ プロパティで次のように指定します。

{{< code lang="yaml" title=".eslintrc.yml（抜粋）" >}}
rules:
  react/react-in-jsx-scope: off
{{< /code >}}


（おまけ）Airbnb スタイルで TypeScript のインポートエラーが出る場合
----

`eslint --init` で導入される Airbnb スタイルは、TypeScript のインポートまわりの扱いが不十分で、多少のマニュアル設定が必要になることがあります（2021-05 現在）。
具体的には、Airbnb スタイルの [import 設定](https://github.com/airbnb/javascript/blob/master/packages/eslint-config-airbnb-base/rules/imports.js) が `.js` ファイルを前提としており、拡張子を省略して `.ts` ファイルをインポートできません（`import/no-unresolved`、`import/extensions` エラーが出ます）。
このエラーは、次のように設定を上書きすることで回避することができます。

{{< code lang="yaml" title=".eslintrc.yml（抜粋）" >}}
rules:
  import/extensions:
    - error
    - ts: never
      tsx: never
      js: never
      jsx: never
settings:
  import/resolver:
    node:
      extensions: [.ts, .tsx, .js, .jsx]
{{< /code >}}

あるいは、`airbnb` (`eslint-config-airbnb`) 共有設定を使うのではなく、[airbnb-typescript (eslint-config-airbnb-typescript)](https://www.npmjs.com/package/eslint-config-airbnb-typescript) という共有設定を使う方法もあります。
こちらであれば、最初から TypeScript ファイルのインポートに対応しています（React にも対応しています）。
ただし、`eslint --init` でインストールできる共有設定は `airbnb` の方なので、`airbnb-typescript` を使いたい場合は、マニュアルで `npm install` する必要があります。

{{< code lang="yaml" >}}
extends:
  - airbnb-typescript
parserOptions:
  ecmaVersion: 2021
  project: ./tsconfig.json
{{< /code >}}

他にも、Standard スタイルで TypeScript を使う場合の [eslint-config-standard-typescript](https://www.npmjs.com/package/eslint-config-standard-typescript) などがあります。

{{< code lang="yaml" >}}
extends:
  - standard-typescript
parserOptions:
  ecmaVersion: 2021
  project: ./tsconfig.json
{{< /code >}}

インストール方法や、`.eslintrc.*` ファイルの記述方法は、各パッケージの GitHub サイトを参照してください。

{{% maku-common/reference %}}
- [GitHub - iamturns/eslint-config-airbnb-typescript](https://github.com/iamturns/eslint-config-airbnb-typescript)
- [GitHub - standard/eslint-config-standard-with-typescript](https://github.com/standard/eslint-config-standard-with-typescript)
{{% /maku-common/reference %}}


（おまけ）Standard スタイルか Airbnb スタイルか
----

JavaScript/TypeScript の世界で一般的に採用されているスタイルは [こちら](/p/avssq37) で簡単にまとめていますが、個人的には、Standard スタイルのセミコロン省略はかなり楽でいいなと感じています。
単純にコードの見た目がスッキリするし、`}` の後ろにセミコロンを入れるか入れないかの一瞬の無駄な思考時間をゼロにできます。
一方で、もっと厳密にスタイルを定義したいのであれば、Airbnb スタイルを使うのがよいかもしれません。
Airbnb スタイルを適用した上で、セミコロンだけ省略するスタイルにするのもありですね。

{{< code lang="yaml" title=".eslintrc.yml" >}}
env:
  es2021: true
extends:
  - airbnb-typescript
  # - airbnb-typescript/base # React を使わない場合
parser: "@typescript-eslint/parser"
parserOptions:
  ecmaVersion: 12
  ecmaFeatures:
    jsx: true
  project: ./tsconfig.json
plugins:
  - react
  - "@typescript-eslint"
rules:
  semi: [error, never]  # セミコロンを省略するスタイルにする
  react/react-in-jsx-scope: off  # JSX 用の React インポートエラー抑制
{{< /code >}}


参考
----

- [ESLint の設定ファイル (.eslintrc) の各プロパティの意味を理解する](/p/j6iu7it)
- [ESLint + Prettier の設定方法まとめ](/p/yfow6dk)

