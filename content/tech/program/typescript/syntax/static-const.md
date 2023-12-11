---
title: "TypeScriptの型: クラス定数を定義する (static readonly)"
linkTitle: "クラス定数を定義する (static readonly)"
url: "/p/nky6cbb"
date: "2020-02-12"
tags: ["TypeScript"]
weight: 102
---

static readonly プロパティを定義する
----

TypeScript 2.0 以降では、クラス内の `static` プロパティに **`readonly`** キーワードを付けて読み取り専用であることを示すことができます。

{{< code lang="ts" >}}
class SiteInfo {
  // クラス定数の定義
  static readonly FAQ_URL = 'https://example.com/faq/';
}
{{< /code >}}

### クラス定数を別ファイルから参照する

このようなクラスをモジュールとして公開 (`export`) すれば、アプリ全体でそのクラス定数を共有できます。

{{< code lang="ts" title="（export側）siteInfo.ts" >}}
export class SiteInfo {
  static readonly FAQ_URL = 'https://example.com/faq/';
}
{{< /code >}}

{{< code lang="ts" title="（import側）index.ts" >}}
import { SiteInfo } from 'siteInfo';

console.log(SiteInfo.FAQ_URL);
{{< /code >}}

### 配列をクラス定数にする (ReadonlyArray)

`readonly` キーワードは、その変数への代入を禁止するだけなので、オブジェクト自体のセット系メソッドは呼び出せてしまいます。
配列に関しても各要素の値が変更できてしまうのは同様ですが、TypeScript にはそれを禁止するための **`ReadonlyArray<T>`** というジェネリッククラスが用意されています。

{{< code lang="ts" title="クラス定数として読み取り専用配列を定義" >}}
class Coordinates {
  static readonly ORIGIN: ReadonlyArray<number> = [0, 0];
}

Coordinates.ORIGIN = [1, 1];  // Error (readonly により防止)
Coordinates.ORIGIN[0] = 100;  // Error (ReadonlyArray により防止)
{{< /code >}}

get アクセサを定義する方法
----

ECMAScript5 で導入された **`get`** アクセサを使う方法でも、クラス定数のような表現が可能です。

{{< code lang="ts" >}}
class SiteInfo {
  static get FAQ_URL(): string {
    return 'https://example.com/faq/';
  }
}

console.log(SiteInfo.FAQ_URL);
{{< /code >}}

ただし、`get` アクセサは関数定義になるので、若干記述が冗長になってしまうのと、呼び出しごとに処理が走るという違いがあります。


（コラム）const キーワードは使えない
----

TypeScript や、そのベースになっている JavaScript では、次のようにクラス内の `static` プロパティに `const` を付けようとするとエラーになります。

{{< code lang="ts" title="間違った方法" >}}
export class SiteInfo {
  static const FAQ_URL = 'https://example.com/faq/';  // Error!
}
{{< /code >}}

{{< code title="エラーメッセ―ジ" >}}
A class member cannot have the `const` keyword.
{{< /code >}}

なので、`const` の代わりに、TypeScript 2.0 で導入された `readonly` キーワードを使用しましょう。
TypeScript 2.0 より前のバージョンしか使えない場合は、次のように `namespace` を使えば、一応クラス定数っぽいものを定義することができます。


（コラム）namespace を使ったクラス定数もどき
----

名前空間 (`namespace`) 内で定数を定義すると、あたかもクラス定数のように使用することができます。
この `namespace` を使った定義方法であれば、TypeScript 2.0 より前のバージョンでも使用できます。

{{< code lang="ts" title="siteInfo.ts" >}}
export namespace SiteInfo {
  // 名前空間内に定数を定義して公開する
  export const FAQ_URL = 'https://example.com/faq/';
}
{{< /code >}}

`export` は名前空間と定数の両方に付けなければいけないことに注意してください。
最初の `export` は、このモジュール（ファイル）が `SiteInfo` 名前空間を公開することを示し、2 つ目の `export` は、`SiteInfo` 名前空間が `FAQ_URL` を公開することを示しています。

上記のように定義した定数は、次のように `import` して参照することができます。
参照方法は、クラス定数 (`static readonly`) を扱う場合と同様です。

{{< code lang="ts" title="index.ts" >}}
import { SiteInfo } from 'siteInfo';

console.log(SiteInfo.FAQ_URL);
{{< /code >}}

ただし、このように定義した `SiteInfo` はクラスではなく名前空間なので、そこにインスタンスメソッドを追加するといったことはできません。
名前空間は、あくまで定数を階層化して管理したい場合のみに使用できます。


