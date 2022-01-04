---
title: "TypeScript コードを Prettier で自動整形する"
url: "/p/au8iu6u"
date: "2021-05-26"
tags: ["Prettier", "TypeScript"]
---

Prettier とは？
----

{{< image src="img-001.png" >}}

[Prettier](https://prettier.io/) は、TypeScript や JavaScript などのソースコードを自動フォーマットするためのツールです。
HTML や CSS など様々なファイルに対応していますが、主に TypeScript や JavaScript のフォーマッターとして使用されています。
実際にどのようにフォーマットされるかは、[Playground のページ](https://prettier.io/playground/) で試すことができます。

### 特徴と思想

Prettier の一番の特徴は、__Opinionated（独断的な）__ コードフォーマッターであることを標榜していることです。
これは、ユーザーに自由なカスタマイズを許さず、「Prettier 自身が定義しているスタイルに強制的にフォーマットするよ」ということです（セミコロンの有無など最低限の設定はできます）。
これにより、コーディングスタイルに関する __不毛な議論を避ける__ ことができ、プロジェクト内のコーディングスタイルを簡単に統一することができます。

もちろん、自分がベストだと思っているスタイルでフォーマットすることはできなくなるかもしれませんが、そんな些細なことよりも、アプリケーション（成果物）を作り上げることに集中すべきだという考え方です。

### Prettier がやらないこと

Prettier はあくまでコードのフォーマットのみを行います。
コードの意味を解析して危険な部分（潜在的バグ）を検出してくれたりはしないので、そういったことを行いたい場合は、他の静的解析ツール（ESLint など）を使う必要があります。
ESLint にもコードフォーマット機能がありますが、フォーマッターとしては Prettier が優れており、「Prettier による整形 ＋ ESLint による静的解析」という形で組み合わせて使うのが一般的です。

あと、import 文のソートなど、一見やってくれてもよさそうなフォーマットもしてくれなかったりします。
これは、その import 順序がロジック的に意味を持っていたりする場合に、Prettier が判断できないからです。
このように、いろいろと想像と異なるフォーマット結果になることがありますが、そこにはちゃんと理由があります（参考: [Rationale](https://prettier.io/docs/en/rationale.html)）。
あまり気にせずにそんなものだと考えるのがよいです。
ちなみに、ESLint の方には import 文のソートを行うプラグイン (`eslint-plugin-import`) があります。


Prettier をインストールする
----

Prettier は `npm` で簡単にインストールすることができます。
[TypeScript のプロジェクトは作成済み](/p/ak7u3h3) で、`package.json` がすでに存在すると想定します。

{{< code >}}
### yarn の場合
$ yarn add prettier --dev --exact

### npm の場合
$ npm install prettier --save-dev --save-exact
{{< /code >}}

Prettier は、パッチバージョンが上がるだけで、出力結果に微妙な差分が生まれる可能性があります。
そのため、インストール時のオプションとして __`--save-exact`__ を指定することで、明確なバージョンを `package.json` の依存情報として保存することが推奨されています。
これにより、チームメンバー全員のフォーマット結果を確実に一致させることができます。

{{< code lang="js" title="package.json（抜粋）" >}}
"devDependencies": {
  "prettier": "^2.3.0"  // こうではなく、
  "prettier": "2.3.0"   // こう書き込まれます。
}
{{< /code >}}

あと、他のツール（VS Code など）に、このプロジェクトが Prettier を使用していることを知らせるために、設定ファイル（__`.prettierrc.json`__ や __`.prettierrc.yml`__）を作成しておきます。
拡張子を省略して __`.prettierrc`__ というファイル名にすると、JSON 形式と YAML 形式のどちらでも記述できますが、エディタの補完機能などを有効にするために、`.prettierrc.yml` のように拡張子は明示しておいた方がよいでしょう。
特別な設定をしないのであれば、設定内容は空っぽで構いません（JSON 形式であれば `{}`、YAML 形式であれば本当に何も書かないでで OK）。

{{< code title=".prettierrc（JSON形式）の作成" >}}
$ echo {}> .prettierrc
{{< /code >}}

行末セミコロンの省略や、引用符をシングルクォートにする、といった設定はしたいかもしれません。
カスタマイズに関しては後述しますが、こんな感じで設定できます（コメントを入れるために YAML 形式で記述しています）。

{{< code lang="yaml" title=".prettierrc.yml（YAML形式）" >}}
semi: false # 行末のセミコロンは省略する
singleQuote: true # 引用符にはシングルクォートを使う
{{< /code >}}

次に、Prettier に無視させたいファイル群を __`.prettierignore`__ ファイルで指定しておきます。
フォーマットは、[.gitignore](https://git-scm.com/docs/gitignore#_pattern_format) と同様です。
Markdown ファイル (`md`) なども自動整形したいところですけど、Prettier は日本語の処理が弱いようなので無視無視。

{{< code title=".prettierignore" >}}
.next/
build/
*.html
*.md
{{< /code >}}

正しく `.prettierignore` ファイルを記述できていれば、`prettier --write .` コマンド（後述）で、プロジェクト内のファイルをすべて整形できます。
作成した `.prettierrc` と `.prettierignore` は、忘れずに Git リポジトリにコミットしてください。


Prettier を実行する
----

### prettier コマンドの使い方

`prettier` コマンドにソースコードを食わせると、Perttier のスタイルに従ってフォーマットし直した結果を標準出力に出力してくれます。

{{< code >}}
$ npx prettier main.ts
{{< /code >}}

__`-w (--write)`__ オプションを付けて実行すると、フォーマット結果をそのファイルに書き戻します。
ディレクトリ名や [グロブパターン](https://github.com/mrmlnc/fast-glob#pattern-syntax) で複数ファイルをまとめて処理できます。

{{< code >}}
# カレントディレクトリ以下のファイルを整形
$ npx prettier --write .

# src ディレクトリ以下のファイルを整形
$ npx prettier --write src

# src ディレクトリ以下の .ts、.tsx ファイルを整形
$ npx prettier --write src/**/*.{ts,tsx}
{{< /code >}}

カレントディレクトリ以下のファイルをまとめて整形する場合は、先にどのファイルが整形対象になっているかを調べておくと安心です。
__`-l (--list-different)`__ オプション、あるいは __`-c (--check)`__ オプションを付けて実行すると、Prettier のフォーマットに従っていないファイルを一覧表示できます。

{{< code >}}
# カレントディレクトリ以下のファイルで整形対象になるものを確認
$ npx prettier -l .
{{< /code >}}

### npm スクリプト化

毎回 `npx prettier ...` と入力するのは面倒なので、npm スクリプト化しておきます。

{{< code lang="js" title="package.json（抜粋）" >}}
{
  // ...
  "scripts": {
    "lint:prettier": "prettier --check src",
    "fix:prettier": "prettier --write src",
    // ...
{{< /code >}}

これで、次のように簡単に Prettier によるフォーマットをかけられるようになります。

{{< code >}}
$ npm run lint:prettier  # 違反のチェックだけしたいとき
$ npm run fix:prettier   # 自動で fix 実行
{{< /code >}}

npm スクリプト名は `lint`、`fix` のようなシンプルな名前にしてもよいのですが、ここでは ESLint も導入することを考えて、詳細な名前を付けるようにしています（ESLint 側のスクリプトには `lint:eslint`、`fix:eslint` という名前を付ける想定）。

{{% reference %}}
- [TypeScript プロジェクトに ESLint を導入する](/p/xz9iry9)
- [ESLint + Prettier の設定方法まとめ](/p/yfow6dk)
{{% /reference %}}


Prettier と一緒に ESLint を使う場合
----

プロジェクトに [ESLint を導入](/p/xz9iry9) している場合、Prettier によるフォーマットと ESLint による解析ルールが競合するので、その部分を無効にしておく必要があります。
これを簡単に行ってくれるのが、[eslint-config-prettier](https://github.com/prettier/eslint-config-prettier) パッケージ（ESLint プラグイン）です。
このパッケージは __Prettier チームが正式に提供している__ ものなので、誰かが適当に作ったパッケージではなく、これを使うことをお勧めします。

{{< code >}}
### yarn の場合
$ yarn add eslint-config-prettier --dev

### npm の場合
$ npm install eslint-config-prettier --save-dev
{{< /code >}}

あとは、ESLint の設定ファイルの `extends` プロパティの最後に __`prettier`__ というコンフィグを追加するだけで OK です。

{{< code lang="yaml" title=".eslintrc.yml（抜粋）" >}}
extends:
  - eslint:recommended
  - plugin:react/recommended
  - plugin:react-hooks/recommended
  - plugin:@typescript-eslint/recommended
  - plugin:@typescript-eslint/recommended-requiring-type-checking
  - prettier
{{< /code >}}

{{% note %}}
古い情報では、`prettier/react` コンフィグも必要と記載されてものがありますが、現在のバージョン（v8.x.x 以降）では `prettier` の指定だけで大丈夫です。
{{% /note %}}


Prettier のカスタマイズ
----

Prettier のフォーマット方法は、`.prettier.{json,yml,js}` ファイルである程度カスタマイズできますが、Prettier はフォーマット関連のオプションをほとんど用意しておらず、可能な限りデフォルト設定のまま使うべきとされています。
なぜなら、スタイル設定による不毛な議論を防ぐことを一番の目的としているからです（参考: [Prettier のオプションに対する思想](https://prettier.io/docs/en/option-philosophy.html)）。
過去の経緯で追加されてしまったオプションは、今さら消すことができないという理由で残っているものがあり、これらも通常使うべきではありません。

- 使わない方がよいオプションの例
    - `arrowParens`
    - `jsxSingleQuote`
    - `jsxBracketSameLine`
    - `noBracketSpacing`

そして、Prettier のデフォルトスタイルは十分に成熟したとし、[今後はフォーマット関連のオプションは追加しない](https://prettier.io/docs/en/option-philosophy.html) としています。
そんな中でもいくつかのオプションは使われることがあるので、下記にいくつか紹介しておきます。

[printWidth](https://prettier.io/docs/en/options.html#print-width) (default: 80)
: 一行あたりの文字数がどれくらいであるべきかを指定します。ESLint の `max-len`（最大文字数）とは若干概念が異なり、実際にはこの値よりも長い行が出力されることはあります。各種フォーマッターで、最大文字数の設定を 100 や 120 にしているプロジェクトは多くありますが、Prettier は `printWidth` はあくまでデフォルトの 80 を使うことを推奨しています。80 文字を超える文字列リテラルや、深いネストによって 80 文字を超える場合は、Prettier はそのまま出力します。

[semi](https://prettier.io/docs/en/options.html#semicolons) (default: true)
: 行末のセミコロンの有無を指定します。デフォルトではセミコロンが付加されますが、[JavaScript Standard Style](https://standardjs.com/readme-ja.html) ではセミコロンを省略する提案がされていますし、Next.js の `create-next-app` コマンドで出力される雛形コードもセミコロンなしのコードになっていたりします。将来的にセミコロンなしが主流になる可能性は十分にあります。

[singleQuote](https://prettier.io/docs/en/options.html#quotes) (default: false)
: 文字列リテラルはデフォルトでダブルクォートで囲まれます。シングルクォートで囲みたいときは `singleQuote` オプションを true に設定します。多くのスタイルではシングルクォートが優先されているので、この設定だけは true にして使う人が多いかもしれません。ちなみに、この設定を true にしても、JSX コード内の文字列リテラルはダブルクォートで囲まれます（`jsxSingleQuote` という別オプションになっています）。

[tralingComma](https://prettier.io/docs/en/options.html#trailing-commas) (default: "es5")
: デフォルトでは、配列などの末尾要素の後にカンマ (`,`) を付加されます。`"none"` にするとカンマが付かなくなります。

{{< code title="yaml" title=".prettierrc の記述例（YAML形式）" >}}
printWidth: 100
semi: false
singleQuote: true
trailingComma: none
{{< /code >}}


Prettier フォーマットを無効にするコメント
----

ソースコードの中で、部分的に Prettier フォーマットを無効にしたいときは、対象オブジェクトの前に __`prettier-ignore`__ コメントを入れます。

{{< code lang="js" >}}
// prettier-ignore
matrix(
  1, 0, 0,
  0, 1, 0,
  0, 0, 1
)
{{< /code >}}


参考
----

- [ESLint の設定方法のまとめと注意点](/p/yfow6dk)
- [VS Code のフォーマッターで自動整形する (formatOnSave)](/p/qcoz9ju)

