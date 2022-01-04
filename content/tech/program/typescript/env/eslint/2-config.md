---
title: "ESLint の設定ファイル (.eslintrc) の各プロパティの意味を理解する"
linkTitle: "ESLint (2) 設定ファイル (.eslintrc) の各プロパティの意味を理解する"
url: "/p/j6iu7it"
date: "2021-05-22"
tags: ["ESLint"]
weight: 302
---

{{% private %}}
- [Configuring ESLint](https://eslint.org/docs/user-guide/configuring/)
- [Configuring ESLint - Configuration Files](https://eslint.org/docs/user-guide/configuring/configuration-files)
{{% /private %}}

何を説明するか
----

下記は、TypeScript を使った React アプリ実装用に用意した ESLint 設定ファイル (`.eslint.yml`) の例です。
トップレベルのプロパティとして、`env` や `extends` などがありますが、これらが何を意味しているかをざっと説明します。

{{< code lang="yaml" title=".eslint.yml（例）" >}}
root: true
env:
  browser: true
  es2021: true
parser: '@typescript-eslint/parser'
parserOptions:
  ecmaVersion: 2021
  project: ./tsconfig.json
plugins:
  - react
  - react-hooks
  - '@typescript-eslint'
extends:
  - eslint:recommended
  - plugin:react/recommended
  - plugin:react-hooks/recommended
  - plugin:@typescript-eslint/recommended
  - plugin:@typescript-eslint/recommended-requiring-type-checking
rules:
  react/react-in-jsx-scope: off
{{< /code >}}


各プロパティの説明
----

### `root: true`

ESLint は、実行時のカレントディレクトリを起点にして、上位のディレクトリの設定ファイル (`.eslintrc.*`) を探していきます。
`root: true` の指定があると、この検索の振る舞いをそこで停止できます。
プロジェクトのトップディレクトリに置く `.eslintrc.*` には、この指定をしておくとよいです。


### `env`: 実行環境の指示

どのようなグローバルオブジェクトを宣言なしで参照可能にするかを ESLint に知らせるための設定です。
例えば、Web ブラウザ上で動作させる JavaScript コードであれば `browser`、Node.js 環境で動作させるコードであれば `node` を `true` に設定します。
内部的には、選択した環境ごとに `globals` オプションの設定が行われます。

{{< code lang="yaml" title="設定例" >}}
env:
  browser: true
  node: true
  es2021: true
{{< /code >}}

{{% reference %}}
- [ESLint - Specifying Environments](https://eslint.org/docs/user-guide/configuring/language-options#specifying-environments)
{{% /reference %}}


### `parser`: 使用するパーサー

ESLint は標準で JavaScript コードのパースに対応していますが、TypeScript コード (`.ts`、`.tsx`) を扱えるようにするには、TypeScript 用のパーサー (`@typescript-eslint/parser`) をインストールして指定する必要があります。

{{< code lang="yaml" title="設定例" >}}
parser: '@typescript-eslint/parser'
{{< /code >}}


### `parserOption`: パーサーの設定

ESLint のデフォルトパーサーは ECMAScript 5 の構文で記述されたコードを想定します。
別のバージョンの ECMAScript 構文で記述したいときは、`ecmaVersion` でバージョンを設定します。
他にも、React アプリ内で JSX コードを使用する場合の指定などもここで行えます。

{{< code lang="yaml" title="設定例" >}}
parserOptions:
  ecmaFeatures:
    jsx: true
  ecmaVersion: 2021   # same as 12
  sourceType: module  # use import/export
{{< /code >}}

`parser` に `@typescript-eslint/parser` を指定したのであれば、[TypeScript 用の parserOptions 設定](https://github.com/typescript-eslint/typescript-eslint/blob/master/packages/parser/README.md#configuration) を行います。
パーサーを変更すると、`parserOptions` で指定すべき項目や、それぞれのデフォルト値が変化することに注意してください。
例えば、`jsx` オプションは通常必要ありません（ファイルの拡張子が `.tsx` の場合は、デフォルトで JSX コードを認識します）。

{{< code lang="yaml" title="設定例" >}}
parser: '@typescript-eslint/parser'
parserOptions:
  ecmaVersion: 2021         # デフォルト値は 2018
  project: ./tsconfig.json  # プロジェクト内の型認識に使用
{{< /code >}}

{{% reference %}}
- [ESLint - Specifying Parser Options](https://eslint.org/docs/user-guide/configuring/language-options#specifying-parser-options)
- [typescript-eslint/README.md - Configuration](https://github.com/typescript-eslint/typescript-eslint/blob/master/packages/parser/README.md#configuration)
{{% /reference %}}


### `plugins`: 使用するプラグインの指定

ESLint プラグインを使用するには、あらかじめ `npm` でインストールした上で、ここに列挙しておく必要があります。さらに、実際にルールを有効化するには、`extends` なども指定する必要があります。
次の例では、`npm` でインストールした `@typescript-eslint/eslint-plugin` プラグイン、および `eslint-plugin-react` をロードするように指示しています。

{{< code lang="yaml" title="設定例" >}}
plugins:
  - react                 # means eslint-plugin-react
  - '@typescript-eslint'  # means @typescript-eslint/eslint-plugin
{{< /code >}}

上記の例からも分かるように、__`eslint-plugin`__ というプレフィックスは省略できるようになっています（参考: [Naming convention](https://eslint.org/docs/user-guide/configuring/plugins#naming-convention)）。

{{% reference %}}
- [ESLint - Configuring Plugins](https://eslint.org/docs/user-guide/configuring/plugins#configuring-plugins)
{{% /reference %}}


### `extends`: 共有設定の適用 (Sharable configuration)

複数のルールをまとめたコンフィギュレーション名を適用します。
ここで指定可能ものを sharable configuration オブジェクトと呼びます。
ESLint 標準のもの（`eslint:recommeded` など）以外は、npm パッケージをインストールすることで指定できるようになります。

{{< code lang="yaml" title="設定例" >}}
extends:
  - 'eslint:recommended'
  - 'plugin:react/recommended'
  - 'plugin:@typescript-eslint/recommended'
{{< /code >}}

内部のルール設定が重複する場合は、後から指定したものが優先されることに注意してください。
例えば、`plugin:@typescript-eslint/recommended` は `eslint:recommended` より後に指定する必要があります（TypeScript 用の設定で上書きする必要があるため）。

sharable configuration のみを提供している npm パッケージには、__`eslint-config-`__ というプレフィックスが付けられており、`extends` プロパティで指定するときは、このプレフィックスを省略できます。

- `eslint-config-airbnb` パッケージ → `airbnb`
- `eslint-config-standard` パッケージ → `standard`

npm パッケージのうち、ESLint プラグインと呼ばれているものは、その中のひとつの機能として sharable configuration を提供しています。
これを `extends` で指定するときは、次のようなフォーマットでコンフィグ名を指定します。

{{< code >}}
plugin:プラグインの省略名/コンフィグ名
{{< /code >}}

例えば、`eslint-plugin-react` プラグインが提供する `recommended` コンフィグを使う場合は、`plugin:react/recommended` と指定します。
`@typescript-eslint/eslint-plugin` プラグインが提供する `recommended` コンフィグを使う場合は、`plugin:@typescript-eslint/recommended` と指定します。


### `rules`: 各ルールの設定

個々のルール単位で有効／無効にする設定を行います。
多くのケースでは、`extends` による共有設定で大まかなルール設定を行い、ここで個別ルールを細かく調整します。

{{% reference %}}
- [ESLint - Rules](https://eslint.org/docs/rules/)
{{% /reference %}}

