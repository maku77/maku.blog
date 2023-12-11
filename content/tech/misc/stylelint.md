---
title: "Stylelint で CSS の記述ミスを防ぐ（CSS の静的解析ツールを導入する）"
url: "p/y2fru7o/"
date: "2023-11-17"
tags: ["Stylelint"]
---

Stylelint とは
----

- [https://stylelint.io/](https://stylelint.io/)

__Stylelint__ は CSS の記述が正しいかをチェックしてくれる lint ツールです。
正しい CSS コードを記述するのはとても難しいので、Stylelint にいろいろ教えてもらいましょう。
Stylelint は、`.css` ファイルだけでなく、`.scss`  などの CSS プリプロセッサ用のファイルや、HTML に埋め込まれた `style` 要素なども解析できます。

既存のプロジェクトに Stylelint をかけると、おそらく大量の指摘事項が表示されます。
まずは、Stylelint の導入だけしておいて、指摘事項は徐々に直していくというやり方で進めるとよいです。


Stylelint のインストール
----

Stylelint は Node.js の NPM モジュールとして配布されています。
既存のプロジェクトに `package.json` が存在しない場合は、先に作成しておいてください。

{{< code lang="console" title="（必要があれば）package.json の作成" >}}
$ npm init -y
{{< /code >}}

あるいは、下記のような最小限の `package.json` ファイルを手動で作成するのでもよいです。

{{< code lang="json" title="package.json" >}}
{
  "name": "example.com",
  "version": "0.0.1",
  "license": "UNLICENSED"
}
{{< /code >}}

`package.json` があるディレクトリで、次のように実行すると Stylelint がインストールされ、初期設定ファイル (`.stylelintrc.json`) が作成されます。

{{< code lang="console" title="Stylelint のインストールと初期設定ファイルの生成" >}}
$ npm init stylelint
{{< /code >}}

`package.json` の内容は次のように更新されます。
__`stylelint`__ コマンド本体と、__`stylelint-config-standard`__ という基本的なコンフィグ（後述）がインストールされていることがわかります。

{{< code lang="json" title="package.json" hl_lines="6 7" >}}
{
  "name": "example.com",
  "version": "0.0.1",
  "license": "UNLICENSED",
  "devDependencies": {
    "stylelint": "^15.11.0",
    "stylelint-config-standard": "^34.0.0"
  }
}
{{< /code >}}

初期設定ファイルは次のような内容になっており、上記でインストールされたコンフィグを読み込むように設定されています。

{{< code lang="json" title=".stylelintrc.json" >}}
{ "extends": ["stylelint-config-standard"] }
{{< /code >}}

これで、CSS ファイルを解析する準備が整いました。


Stylelint を実行する
----

コマンドラインから Stylelint を実行するには次のようにします。
ファイルのパスは適宜変更してください。

```console
$ npx stylelint "assets/**/*.css"

assets/css/main.css
 2:20  ✖  Expected "#ffccdd" to be "#fcd"  color-hex-length

1 problem (1 error, 0 warnings)
```

解析対象のファイルを毎回指定するのは面倒なので、次のような感じで `package.json` に NPM スクリプトを追加しておくと便利です。

{{< code lang="js" title="package.json" >}}
{
  // ...
  "scripts": {
    "lint": "stylelint \"assets/**/*.css\""
  },
  // ...
}
{{< /code >}}

これで、次のように統一された方法で Stylelint を実行できるようになります。

```console
$ npm run lint
```

ここまでできれば、基本的な Stylelint の導入は完了なので、Git にコミットしておきましょう。
あとは、プロジェクトごとに試行錯誤しながらルール設定していくことになります。


Stylelint の設定
----

### 設定ファイルの基本構成

Stylelint の設定ファイルの基本的な構成は次のようになります（ESLint とそっくりです）。

{{< code lang="js" title=".stylelintrc.json" >}}
{
  "extends": [
    "コンフィグ",
    "コンフィグ"
  ],
  "plugins": [
    "プラグイン",
    "プラグイン"
  ],
  "rules": [
    "ルール名": "設定値",
    "ルール名": "設定値"
  ]
}
{{< /code >}}

各プロパティは次のような意味を持ちます。

`extends`
: コンフィグ (shared config) を読み込みます。コンフィグはルール設定をまとめたもので、標準的な公式コンフィグとして `stylelint-config-standard` が提供されています。コンフィグは、プラグイン（後述）や他のコンフィグを内包していることがあります。

`plugins`
: 独自ルールを定義したプラグインを読み込みます。CSS ファイル用の組み込みルールでは足りないときにプラグインを使用します。例えば、HTML に埋め込まれた `style` 要素や、SCSS ファイルなどを解析したいときにプラグインを使用します。プラグインはコンフィグに内包されていることがあるので、実際にはプラグインを明示的に指定する必要はなかったりします。

`rules`
: Stylelint 組み込みのルール、あるいはプラグインが提供する独自ルールに対する設定を行います。プラグインが提供するルールの名前には、`scss/` のような名前空間を示すプレフィックスが付いています。一般的には、`extends` プロパティのコンフィグで複数のルールを一括で適用しておいて、`rules` プロパティでルールを追加・上書きして細かく調整する、という使い方になります。


### rules でルールを適用する

Stylelint は、__ルールを設定しない限り、デフォルトでは何も検出してくれません__。
Stylelint は [100 種類以上の組み込みルール](https://stylelint.io/user-guide/rules/) を用意しているので、設定ファイルの __`rules`__ プロパティで、それらのルールをひとつずつ設定していきます。
例えば、重複するセレクター (`no-duplicate-selectors`) と、空っぽのブロック (`block-no-empty`) の検出を有効化したいときは次のようにルール設定します。

{{< code lang="json" title=".stylelintrc.json" >}}
{
  "rules": {
    "no-duplicate-selectors": true,
    "block-no-empty": true
  }
}
{{< /code >}}

### extends でコンフィグを読み込む

上記のようにルールをひとつずつ適用していくのは大変なので、ルール設定をまとめて適用するためのコンフィグ (config) という仕組みが提供されています。
shared config や shareable config と呼ばれたりしますが、ここでは簡単に「コンフィグ」と呼ぶことにしています。

Stylelint 用の各種コンフィグは、__`stylelint-config-`__ というプレフィックスの付いた NPM モジュールとして提供されています。
CSS ファイル用の標準的なコンフィグは、公式の [`stylelint-config-standard`](https://github.com/stylelint/stylelint-config-standard) として提供されており、`npm install -D stylelint-config-standard` でインストールできます（`npm init stylelint` でプロジェクトに Stylelint を導入した場合もインストールされます）。

インストール済みのコンフィグを使用するには、設定ファイルの __`extends`__ プロパティで次のように指定します。
複数のコンフィグを指定することも可能です。
複数のコンフィグでルール設定が競合している場合は、後から指定したコンフィグのルール設定で上書きされます。

{{< code lang="js" title=".stylelintrc.json" hl_lines="2" >}}
{
  "extends": ["stylelint-config-standard"],
  "rules": {
    "color-hex-length": null
  }
}
{{< /code >}}

コンフィグが提供するルール設定をそのまま適用できることは少ないので、上記のように `rules` プロパティを使って各ルールを上書きすることで、プロジェクトごとのカスタマイズを行います。
上記の例では、`color-hex-length` ルールを無効化 (`null`) することで、カラー値を `#ff0099` のような 6 桁指定できるようにしています（デフォルトだと `#f09` のような 3 桁指定が強制されます）。

`stylelint-config-standard` コンフィグによりどのようなルールが有効化されるかは、公式サイトの [Rules のページ](https://stylelint.io/user-guide/rules) で確認できます（✅マークの付いたルールが有効化されます）。

### YAML で書くとスッキリする

Stylelint の設定ファイルは、JSON 形式だけでなく、YAML 形式でも記述することができます。
YAML 形式だとコメント (`#`) を記述することができ、JSON 形式よりも簡潔に記述できるので、（周囲の人の抵抗がなければ）YAML 形式を採用するのがよさそうです。

{{< code lang="yaml" title=".stylelintrc.yaml" >}}
extends:
  - stylelint-config-standard
rules:
  # 16 進数のカラーコードは 6 桁での記述を許可する
  color-hex-length: null
{{< /code >}}


SCSS ファイルを解析できるようにする
----

Stylelint で `.scss` ファイルを解析できるようにするには、Stylelint 本体 (`stylelint`) に加え、SCSS 用のコンフィグ [stylelint-config-standard-scss](https://www.npmjs.com/package/stylelint-config-standard-scss) をインストールします。

```console
$ npm install --save-dev stylelint-config-standard-scss
```

設定ファイルの `extends` プロパティで、このコンフィグを読み込めば基本的な設定は完了です。

{{< code lang="json" title=".stylelintrc.json" >}}
{ "extends": ["stylelint-config-standard-scss"] }
{{< /code >}}

`.scss` ファイルを解析するには次のように実行します。

```console
$ npx stylelint "assets/**/*.scss"
```

`stylelint-config-standard-scss` コンフィグは、標準的なコンフィグである `stylelint-config-standard` を内包しているので、`.css` と `.scss` の両方を標準的なルールで解析できます。
`stylelint` コマンドで `.css` と `.scss` の両方を解析するには、次のいずれかの方法で指定します。

```console
$ npx stylelint "assets/**/*.{css,scss}"
$ npx stylelint "assets/**/*.css" "assets/**/*.scss"
```

Stylelint の導入時に `npm init stylelint` コマンドを使用した場合は、`stylelint-config-standard` モジュールが単独でインストールされている可能性があります（`package.json` の `devDependencies` を確認してください）。
`stylelint-config-standard-scss` コンフィグを導入する場合は、単独の `stylelint-config-standard` は必要ないので削除しておきましょう。

```console
$ npm uninstall stylelint-config-standard
```


HTML / Svelte / Astro ファイル内の style 要素を解析できるようにする
----

HTML の `style` 要素内に記述した CSS コードも解析対象にしたいときは、[`stylelint-config-html`](https://www.npmjs.com/package/stylelint-config-html) というコンフィグを導入するのが簡単です。
内部的に `postcss-html` モジュールを使った HTML のパースが行われるようになり、正しく `style` 要素を解析できるようになります。
Svelte や Astro のコンポーネント内の `style` 要素も扱えます。

{{< code lang="console" title="stylelint-config-html コンフィグのインストール" >}}
$ npm install --save-dev stylelint-config-html
{{< /code >}}

あとは、設定ファイルの `extends` プロパティで読み込むだけです。
下記は YAML 形式での設定例です。

{{< code lang="yaml" title=".stylelintrc.yaml" hl_lines="3" >}}
extends:
  - stylelint-config-standard
  - stylelint-config-html
rules:
  # 16 進数のカラーコードは 6 桁での記述を許可する
  color-hex-length: null
  # ...
{{< /code >}}

`.css`、`.html`、`.svelte` ファイルをまとめて解析するには、次のように実行します。

```console
$ npx stylelint "src/**/*.{css,html,svelte}"
```


自動修正 (fix) 機能を使用する
----

Stylelint のルールの中には、コードの自動修正に対応したものがあります。
`stylelint` コマンドを実行するときに、__`--fix`__ オプションを付けると、コードが自動修正されます。

{{< code lang="console" title="Stylelint で CSS コードを自動修正する" >}}
$ npx stylelint --fix "**/*.css"
{{< /code >}}

`stylelint-config-standard` コンフィグが設定するルールのうち、どのルールが自動修正に対応しているかは、公式サイトの [Rules ページ](https://stylelint.io/user-guide/rules) で確認できます（🔧アイコンが付いたものが自動修正に対応したルールです）。
例えば、`rule-empty-line-before` というルールは自動修正に対応しており、各セレクターの前に空行を入れてくれます。

自動修正用のコマンドも、NPM スクリプトとして定義しておくと便利です。

{{< code lang="js" title="package.json" hl_lines="5 7" >}}
{
  // ...
  "scripts": {
    "lint": "run-p --print-label --continue-on-error lint:*",
    "lint:stylelint": "stylelint \"**/*.css\"",
    "fix": "run-s --print-label fix:*",
    "fix:stylelint": "stylelint --fix \"**/*.css\""
  },
  "devDependencies": {
    "npm-run-all": "^4.1.5",
    "stylelint": "^15.11.0",
    "stylelint-config-standard": "^34.0.0"
  },
  // ...
}
{{< /code >}}

上記の例では、`npm-run-all` モジュールの `run-p`/`run-s` コマンドを使って、複数の解析ツールをまとめて実行できるようにしています（今回の例では Stylelint しか導入していませんが）。

```console
$ npm run lint:stylelint  # Stylelint による解析
$ npm run fix:stylelint   # Stylelint による自動修正

$ npm run lint   # すべての静的解析ツールによる解析
$ npm run fix    # すべての静的解析ツールによる自動修正
```


（おまけ）設定例
----

{{< code lang="yaml" title=".stylelintrc.yaml" >}}
extends:
  - stylelint-config-standard-scss

rules:
  color-function-notation: [modern, { disableFix: true }]
  media-feature-range-notation: [context, { disableFix: true }]
{{< /code >}}

- [color-function-notation](https://stylelint.io/user-guide/rules/color-function-notation)
  - `modern` にすると、`rgb(12 122 231 / 0.2)` のようなカンマなしの記述を強制します。
  - `legacy` にすると、`rgba(12, 122, 231, 0.2)` のようなカンマありの記述を強制します。
  - 上記の例では、モダンな書き方の `modern` を設定しつつ、自動 fix だけを無効化しています。

- [media-feature-range-notation](https://stylelint.io/user-guide/rules/media-feature-range-notation/)
  - `context` にすると、`@media (1px <= width <= 2px) {}` という形式での記述を強制します
  - `prefix` にすると、`@media (min-width: 1px) and (max-width: 2px) {}` という形式での記述を強制します
  - 上記の例では、モダンな書き方の `context` を設定しつつ、自動 fix だけを無効化しています。

