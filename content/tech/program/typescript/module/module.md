---
title: "TypeScriptでモジュールを作成する／インポートする (export, import)"
linkTitle: "モジュールを作成する／インポートする (export, import)"
url: "p/fbu8k8j/"
date: "2019-11-21"
tags: ["JavaScript", "TypeScript"]
weight: 201
---

モジュールとは
----

TypeScript で大きなプログラムを作成するときは、モジュールの仕組みを使って **複数のファイルに分割** していきます。
ファイルを分割することでコードを管理しやすくなるだけでなく、名前空間のコンテキストが分けられることになるので、 **名前の衝突の問題も解決** することができます。

モジュールを作るのは簡単で、**`export`** キーワードを含む `.ts` ファイルを作ればそれがモジュールになります。
`export` キーワードでは、クラスやインタフェースをまるごと公開することもできるし、関数や変数の単位で公開することもできます。

`export` の使い方のポイントは、次のように、 **クラスや変数を定義するときにプレフィックスとして付加する** というところです。
基本的には、定義済みのオブジェクトを後から `export` するという使い方はしません。

* export class ...
* export interface ...
* export const ...
* export let ...

ここからは、具体的な `export` の使い方を見ていきます。


クラスやインタフェースの定義を export する
----

下記の `lib/mylib.ts` ファイルでは、`MyInterface` インタフェースと、`MyClass` クラスを公開しています。
それぞれの定義の前に `export` キーワードを付けるだけでよいので簡単ですね。

{{< code lang="typescript" title="lib/mylib.ts" >}}
export interface MyInterface {
  name: string;
}

export class MyClass implements MyInterface {
  constructor(public name: string) {}
}
{{< /code >}}

次の `index.ts` ファイルからは、上記のクラスモジュールを **`import`** して読み込んでいます。

{{< code lang="typescript" title="index.ts（個別に import）" >}}
import { MyInterface, MyClass } from './lib/mylib';

const obj: MyInterface = new MyClass('Maku');
console.log(obj.name);  //=> Maku
{{< /code >}}

`from` の後ろに指定するファイルパスには、 **拡張子の `.ts` を記述しない** ことに注意してください（実際に読み込むファイルは `.js` なので、`.ts` の指定は意味的にも間違っています）。
上記の例では、インタフェースやクラスの名前を直接指定して、個別の変数に読み込んでいますが、次のようにワイルドカードを使って `export` されているものを 1 つの変数にすべて読み込むこともできます。

{{< code lang="typescript" title="index.js（まとめて import）" >}}
import * as mylib from './lib/mylib';

const obj: mylib.MyInterface = new mylib.MyClass('Maku');
console.log(obj.name);  //=> Maku
{{< /code >}}

ここでは、**`as` キーワード** を使って `mylib` という変数のプロパティ経由で公開されているインタフェースにアクセスできるようにしています。
**ワイルドカードを使用する場合、この変数の指定は必須** であることに注意してください（それにより、不注意による名前の衝突を防げるようになっています）。

個別の変数に読み込む場合でも、`as` キーワードを使って別名を付けることができます。
使用頻度は高くないかもしれませんが、名前の衝突が起こってしまう場合や、長いインタフェース名を短い名前で参照したいときなどに使えるテクニックです。

{{< code lang="typescript" title="index.js（別名を付ける）" >}}
import { MyInterface as MI, MyClass as MC } from './lib/mylib';

const obj: MI = new MC('Maku');
console.log(obj.name);  //=> Maku
{{< /code >}}


単一の変数や定数、関数などを export する
----

`const` や `let` を使って変数を定義するときに、`export` キーワードを付けておくと、その変数を単独で公開できます。

次の `config.ts` モジュールは、`config` 変数（`Config` クラスのインスタンス）を公開しています。

{{< code lang="typescript" title="config.ts" >}}
class Config {
  debug: boolean = true;
  animSpeed: number = 10;
}

export const config = new Config();
{{< /code >}}

参照側では次のようにして `config` 変数を扱うことができます。

{{< code lang="typescript" title="index.ts" >}}
import { config } from './config';

console.log(config.debug);  //=> true
{{< /code >}}

ちなみに、このような設定情報であれば、次のように各プロパティを static なクラス変数として定義し、クラスごと `export` した方がシンプルでよいかもしれません。

{{< code lang="typescript" title="config.ts" >}}
export class Config {
  static readonly DEBUG: boolean = true;
}
{{< /code >}}

{{% maku-common/reference %}}
- [TypeScript: クラス定数を定義する (static readonly)](/p/nky6cbb/)
{{% /maku-common/reference %}}

{{< code lang="typescript" title="index.ts" >}}
import { Config } from './config';

console.log(Config.DEBUG);  //=> true
{{< /code >}}


定数や関数を単独で公開することもできます。

{{< code lang="typescript" title="mylib.ts" >}}
export const MAX_CHANNELS = 100;
export function greet() {
  console.log('Hello');
}
{{< /code >}}

{{< code lang="typescript" title="index.ts" >}}
import { MAX_CHANNELS, greet } from './config';

console.log(MAX_CHANNELS);  //=> 100
greet();  //=> Hello
{{< /code >}}

次のように、`export` するときに別名を付けることもできます。

{{< code lang="typescript" title="mylib.ts" >}}
function greet() {
  console.log('Hello');
}

export { greet as myGreet }
{{< /code >}}


再 export (Re-export)
----

次のように `export` を使用すると、別のモジュールから `export` されているインタフェースを、あたかも自分が `export` しているかのように見せることができます。

{{< code lang="typescript" >}}
export { Foo, Bar } from './submodule';
{{< /code >}}

次のように、別名を付けて再 `export` したり、すべてのインタフェースを再 `export` することもできます。

{{< code lang="typescript" >}}
export { Foo as MyFoo } from './submodule';
export * from './submodule';
{{< /code >}}

このようにワイルドカードを使用する場合は、`import` の場合と違い、`as` による別名を付けなくてよいことに注意してください。

この再 export の仕組みを使用すれば、複数のサブモジュールで構成されたライブラリを、 1 つの大きなモジュールとして見せることができます。


デフォルト export
----

（この仕組みはオススメしません）

**`export default`** を指定すると、モジュールの中の 1 つのメンバーをデフォルト export に設定することができ、シンプルな記述で `import` できるようになります。

次の例では、`Book` クラスの定義をデフォルト export しています。

{{< code lang="typescript" title="book.ts" >}}
export default class Book {
  constructor(public title: string) {}
}
{{< /code >}}

このクラスは下記のようにシンプルに `import` できます（変数名を `{}` で囲んだりする必要がありません）。

{{< code lang="typescript" title="index.ts" >}}
import Book from './book';

const book = new Book('Title');
{{< /code >}}

デフォルト export された `Book` クラスは、次のように別の名前の変数にも代入できてしまいます（それでも実体は `Book` クラスです）。

{{< code lang="typescript" title="index.ts" >}}
import Hoge from './book';

const book = new Hoge('Title');
{{< /code >}}

このように名前の変更ができてしまうので、デフォルト export を使ったコードは分かりにくくなってしまう可能性があります。
そのため、**デフォルト export の仕組みはあまりオススメできません**。

`export default` は、次のように、単一のインスタンス（ここでは `Config` オブジェクト）を公開するのにも使用できます。

{{< code lang="typescript" title="config.ts" >}}
class Config {
  debug: boolean = true;
  animSpeed: number = 10;
}

export default new Config();
{{< /code >}}

次のように `import` すれば、モジュール側で生成された `Config` オブジェクトを直接参照できます。

{{< code lang="typescript" title="index.ts" >}}
import config from './config';

console.log(config.debug);  //=> true
{{< /code >}}

とはいえ、通常の `export` の使い方でも同様のことを実現できるため、やはり `export default` の使用はオススメできません。


オブジェクト export (Object export)
----

（この仕組みもオススメしません）

単一のオブジェクトだけを公開したい場合は、上で説明した `export default` の仕組みを使用する方法以外にも、

{{< code >}}
export = オブジェクト;
{{< /code >}}

という構文を使用する方法があります。
CommonJS の exports & requre のようなものだというと伝わる人には伝わるかもしれません。

次の `config.ts` モジュールでは、唯一 `Config` クラスのインスタンスだけを `export` しています。

{{< code lang="typescript" lang="config.ts" >}}
class Config {
  debug: boolean = true;
  animSpeed: number = 10;
}

export = new Config();
{{< /code >}}

このモジュールを読み込む側では、`import/from` ではなく、**`import/require`** を使って読み込む必要があります。

{{< code lang="typescript" lang="index.ts" >}}
import config = require('./config');

console.log(config.debug);  //=> true
{{< /code >}}

この方法でモジュールを読み込む場合、公開されているオブジェクトの名前と合わせた変数に代入する必要はありません（上記の例では意図的に `config` と合わせているだけです）。

{{< code lang="typescript" >}}
import hogehoge = require('./config');
{{< /code >}}

このように使用側で名前を勝手に変更できてしまう点は、`export default` との共通点であり、オススメできない理由でもあります。

