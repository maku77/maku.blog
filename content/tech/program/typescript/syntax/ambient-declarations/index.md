---
title: "TypeScriptの型: 既存の JavaScript ライブラリに型情報を追加する（.d.ts ファイル）"
linkTitle: "既存の JavaScript ライブラリに型情報を追加する（.d.ts ファイル）"
url: "/p/s7wk5k3"
date: "2020-02-27"
lastmod: "2020-10-11"
tags: ["JavaScript", "TypeScript"]
weight: 300
---

{{% private %}}
- [型定義ファイルを作成する | Revised Revised TypeScript in Definitelyland](http://typescript.ninja/typescript-in-definitelyland/definition-file.html)
- [Best practices | DefinitelyTyped](http://definitelytyped.org/guides/best-practices.html)
- [Publishing · TypeScript](http://www.typescriptlang.org/docs/handbook/declaration-files/publishing.html)
- [Consumption · TypeScript](http://www.typescriptlang.org/docs/handbook/declaration-files/consumption.html)
- [どのようにパスが解決されるか - TypeScript Deep Dive 日本語版](https://typescript-jp.gitbook.io/deep-dive/project/modules/external-modules#donoyounipasugasareruka)
{{% /private %}}

アンビエント宣言とは
----

TypeScript の **アンビエント宣言 (Ambient Declarations)** を行うと、既存の JavaScript ライブラリに型情報を付加することができます。
この仕組みを利用すると、

* サードパーティ製の JavaScript ライブラリ（npm パッケージ）や、自作の JavaScript ライブラリ（ただし TypeScript 化はしたくないもの）を TypeScript コードから使用する
* jQuery などのブラウザ上でロードされるライブラリを TypeScript コードから使用する

といったことが可能になります。
ようするに、TypeScript トランスパイラに対して、このオブジェクトはこういう型のものとして外から提供されているので、型チェックエラーを出さないでね、と知らせることができます。

さらに、VisualStudio Code などのエディタを使用している場合は、アンビエント型宣言があることにより、エディタ上での補完入力ができるようになります。


アンビエント宣言 (declare)
----

- 参考: [TypeScript - Declaration Reference](https://www.typescriptlang.org/docs/handbook/declaration-files/by-example.html)

どこか別の場所でロードされる予定の JavaScript モジュールに対して、自力で型情報を付けたい場合は __`declare`__ キーワードを使用します。
例えば、jQuery はもともと JavaScript 用のライブラリなので、TypeScript 用の型情報は提供していませんが、次のように自力で型情報を付けることで、TypeScript コードから利用できるようになります（実際には、DefinitelyTyped プロジェクトが提供する `@types/jquery` を使用するのが簡単です）。

{{< code lang="ts" title="index.ts" >}}
// $ という変数を参照できるようにする（実体は実行時に後付けで定義される予定）
declare var $: any;

// これで、$ という未定義の変数を参照してもエラーにならない
$('#id').html('Hello!');
{{< /code >}}

この仕組みを使わずに、いきなり `$` を参照してしまうと、そのような変数は定義されていないというエラーになってしまいます。
TypeScript トランスパイラは、`declare` によって付けられた型情報を正しいものと判断するため、この型情報は間違えないように指定する必要があります。
また、実行時にはそのオブジェクトの実体がどこかで生成されていなければいけません。

もっと明確な型付けを行うなら次のような感じで、クラスや関数などの型を定義していきます。

{{< code lang="ts" title="index.ts" >}}
declare class jQuery {
  html(html: string): void;
}
declare function $(query: string): jQuery;

$('#id').html('Hello!');
{{< /code >}}

### 型宣言ファイル (.d.ts)

プロジェクト全体でアンビエント宣言を共有したい場合は、次のような型宣言ファイル __`.d.ts`__ をソースツリーのルートに配置します。

{{< code lang="ts" title="globals.d.ts" >}}
declare var $: any;
{{< /code >}}

`.d.ts` という拡張子は、オブジェクトの実体ではなく、型情報のみが含まれていることを示しています。
このファイルは TypeScript ビルド時のみに参照されるものであり、`.js` ファイルの生成は行われません。


アンビエントモジュール (declare module)
----

`declare` キーワードは、単一の変数や関数、クラスに型情報を与えるものですが、__`declare module 'モジュール名'`__ のように使用すると、モジュール単位で型情報を付加することができます。
この仕組みを __アンビエントモジュール (Ambient Modules)__ と呼びます。
これで型情報を定義しておくと、指定したモジュールをインポートしたときに、自動的に型情報が付加されるようになります。

- 参考: [TypeScript - Ambient Modules](https://www.typescriptlang.org/docs/handbook/modules.html#ambient-modules)

次の例では、`foo` モジュール用の型情報を定義しています。

{{< code lang="ts" title="globals.d.ts" >}}
// foo モジュール用の型宣言
declare module 'foo' {
  export val bar: number;
}
{{< /code >}}

次のように `foo` モジュールをインポートすると、`bar` 変数が export されているモジュールとして扱うことができます。

{{< code lang="ts" title="型情報のインポート" >}}
import * as foo from 'foo';
console.log(foo.bar);
{{< /code >}}

典型的なのは、CSS Modules を使用できるようにするための次のような定義です。
TSDoc/JSDoc 形式でドキュメンテーションコメントを記述しておけば、VS Code などのエディタ上でドキュメント表示してくれるようになります。

{{< code lang="ts" title="globals.d.ts" >}}
declare module '*.css' {
  /** CSS クラスを参照するためのオブジェクトです。 */
  const styles: { [className: string]: string };
  export default styles;
}
{{< /code >}}

対象モジュールを `any` としてだけ扱えればよいのであれば、次のように型定義部分を省略可能です。

{{< code lang="ts" title="globals.d.ts" >}}
declare module '*.css';
{{< /code >}}

これで、次のように `.css` ファイルをインポートしたときに、TypeScript の型エラーが発生しなくなります。

{{< code lang="ts" >}}
import * as styles from './App.css';
{{< /code >}}


存在する JavaScript ファイルに型宣言ファイルを提供する
----

プロジェクト内にすで存在する JavaScript モジュール（`.js` ファイル）に対して、後付けで型宣言ファイル (`.d.ts`) を提供したい場合があります。
例えば、社内の別の人が作成した JavaScript ライブラリを、TypeScript コードから参照したい場合などです。
そのような場合は、対象の `.js` ファイルと同じ階層に、同じモジュール名で `モジュール名.d.ts` というファイルを作成します。
例えば、次のような JavaScript ライブラリがあったとします。

{{< code lang="js" title="lib/util.js" >}}
exports.greet = function(name) {
  console.log(`Hello, ${name}`);
}
{{< /code >}}

この `lib/util.js` は、`greet` 関数を公開していますが、パラメータと戻り値の型が定義されておらず、そのままでは TypeScript コードから使用できません（`noImplictAny` 設定時）。
この `lib/util.js` ファイルの内容を変更せずに、型情報付きの TypeScript モジュールとして利用したいときは、__同じディレクトリに `lib/util.d.ts` ファイルを作成__ します。

{{< code lang="ts" title="lib/util.d.ts" >}}
export function greet(name: string): void;
{{< /code >}}

すると、JavaScript で作成されたライブラリを、型情報付きのモジュール `lib/util` として参照できるようになります。
VisualStudio Code などで次のように型情報の補完が効くようになります。

{{< image border="true" src="img-001.png" title="main.ts" >}}

TypeScript のビルド時には、`import` で指定されたモジュール名に対して `.ts`、`.tsx`、`.d.ts` という順でファイル検索が行われます。
そのため、ここでは `lib/util.d.ts` という型宣言ファイルが読み込まれることになります。
一方、実行時に使用されるのは `lib/util.js` というライブラリ本体側のファイルであることに注意してください。
`.js` ファイルと `.d.ts` ファイルを同じディレクトリに格納して、インポート時のパスが一致するようにしているのは、このあたりに理由があります。
`.d.ts` ファイルが一種の緩衝材のようになり、TypeScript と JavaScript の世界をうまくつないでくれています。


既存ライブラリの例
----

### 各 NPM パッケージに埋め込まれた型宣言ファイルを使う

NPM パッケージとして公開されている JavaScript ライブラリには、自分自身の NPM パッケージ内に TypeScript 用の型宣言ファイル (`.d.ts`) を含んでいるものもあります。
型宣言ファイルのパスは、次のように `package.json` ファイルの `types` プロパティで指定されています。

{{< code lang="json" title="package.json" >}}
{
  "name": "awesome",
  "author": "Vandelay Industries",
  "version": "1.0.0",
  "main": "./lib/main.js",
  "types": "./lib/main.d.ts"
}
{{< /code >}}

このような NPM パッケージは、そのまま TypeScript ライブラリとしてインポートできます。


### DefinitelyTyped で公開される @types/xxx を使う

jQuery ライブラリなどの有名な JavaScript ライブラリの型宣言ファイルは、[DefinitelyTyped](https://definitelytyped.org/) プロジェクトでまとめて公開されています。
パッケージ名は、 **`@types/XXX`** という名前になっており、次のようにインストールすることができます。

{{< code title="jQuery 用の TypeScript 型宣言をインストール" >}}
$ npm install --save-dev @types/jquery
{{< /code >}}

これでインストールされるのは型宣言のみなので、jQuery ライブラリ本体は何らかの方法で参照できるようにしておく必要があります。
最終的に生成された JavaScript を Web ブラウザから実行するのであれば、HTML ファイルの `script` タグなどで jQuery 本体をロードすることになるでしょう。
ここでは、テストのため、`npm` コマンドでインストールしてしまうことにします。

{{< code title="jQuery 本体をインストール（JavaScript 用の NPM パッケージ）" >}}
$ npm install --save jquery
{{< /code >}}

{{< note >}}
`@types/jquery` はビルド時のみ必要なので `--save-dev` フラグでインストールしていますが、`jquery` モジュールは実行時に必要になるので `--save` フラグでインストールしていることに注意してください。
{{< /note >}}

このようにインストールした型宣言ファイルは `node_modules/@types` 以下に格納されており、TypeScript はデフォルトでこのディレクトリ以下のモジュールをインポートするパスを通しています（`tsconfig.json` の `compilerOptions.typeRoots` プロパティでカスタマイズ可能）。
つまり、`@types/` というプレフィックスを付けずに、次のようにインポートすることができます

{{< code lang="ts" title="main.ts" >}}
import * as $ from 'jquery';
const footerElem = $('#footer');
{{< /code >}}

このように、あたかも最初から TypeScript 用のライブラリとして提供されているかのように JavaScript ライブラリを参照できるようになります（実際には `.d.ts` の型情報だけ見てトランスパイルしてるだけですが）。

ただこれだと、生成される JavaScript ファイル内に `import` 文が残ってしまうので、jQuery の場合は、次のようにコメント形式で `.d.ts` の定義を参照する方法がよいかもしれません。

{{< code lang="ts" >}}
/// <reference path="../node_modules/@types/jquery/index.d.ts" />
const footerElem = $('#footer');
{{< /code >}}


その他
----

### String ではなく string を使う

型宣言ファイル (`.d.ts`) の中では、大文字で始まる `String` ではなく、TypeScript プリミティブな `string` 型を使い、効率的なコードを生成するようにします。
`Number`、`Boolean`、`Symbol`、`Object` なども同様に、すべて小文字の型の方を使います。

- 参考: [Do's and Don'ts](http://www.typescriptlang.org/docs/handbook/declaration-files/do-s-and-don-ts.html)）。

### .ts ファイルから .d.ts ファイルを生成する

TypeScript ファイル (`.ts`) をトランスパイルするときに、`.js` ファイルに加えて `.d.ts` ファイルを生成したいときは、ビルドオプションで __`--declaration`__ を指定します。
TypeScript で作成したライブラリを NPM パッケージとして公開するときは、このように作成した `.js` ファイルと `.d.ts` ファイルをパッケージングします（逆に `.ts` ファイルはパッケージに入れてはいけません）。

