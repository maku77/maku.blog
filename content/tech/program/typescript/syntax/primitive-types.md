---
title: "TypeScriptの型: 基本型（プリミティブタイプ）の一覧"
linkTitle: "基本型（プリミティブタイプ）の一覧"
url: "p/2uds9o4/"
date: "2020-04-28"
tags: ["TypeScript"]
weight: 1
---

{{< private >}}
- [Basic Types · TypeScript](https://www.typescriptlang.org/docs/handbook/basic-types.html)
{{< /private >}}


プリミティブ型 (primitive types)
----

下記のプリミティブ型は、JavaScript が内部的な型情報として扱うものであり、TypeScript のタイプアノテーションでも同様に使用することができます。
型名がすべて小文字になっているのが特徴的です。

- boolean（真偽値）
    - `let isDone: boolean = false;` （`true` または `false`）
- number（数値）
    - `let decimal: number = 6;` （10進数リテラル）
    - `let hex: number = 0xf00d;` （16進数リテラル）
    - `let octal: number = 0o744;` （8進数リテラル）
    - `let binary: number = 0b1010;` （2進数リテラル）
- string（文字列）
    - `let name: string = 'Maku';` （シングルクォートとダブルクォートは同様）
    - ```let msg: string = `Hello, ${name}`;```（変数展開する場合はバッククォート）
- function（関数）
    - `function foo(): number { return 100; }`
    - `let bar: () => number = foo`
- bigint（巨大整数）
    - `let x: bigint = 1n;` （ES2020 移行）
- symbol（ユニークなシンボル）
    - `let sym: symbol = Symbol('key');` （ES2015 移行）
- undefined（変数が初期化されていないことを示す）


オブジェクト型 (object)
----

上記の型以外は、JavaScript の世界ではすべて __`object`__ 型とみなされます。
TypeScript のタイプアノテーションでは、変数に格納できる値の型をより明確に示すことができます。

- Array（配列）
    - `let arr: number[] = [1, 2, 3];` （数値の配列）
    - `let arr: Array<number> = [1, 2, 3];` （〃）
    - `let arr: string[] = ['AAA', 'BBB'];` （文字列の配列）
    - `let arr: Array<string> = ['AAA', 'BBB'];` （〃）
    - 参考: [配列を定義する (Array)](/p/ttwoetd)
- Tuple（タプル）扱いは配列とほぼ同じだが、各位置の要素の型が決まっているもの
    - `let x: [string, number] = ['Hello', 100];`
    - 参考: [タプルを定義する (Tuple types)](/p/6uzqf4q)
- Null（値がない）
    - `let x: string | null = null;`

その他
----

### Enum

{{< code lang="ts" >}}
enum Color {Red, Green, Blue}
let c: Color = Color.Red;
{{< /code >}}

列挙型の値（上記の例では `Color.Red` など）は、内部的にはただの数値 (`number`) 扱いです。

### Void / Never

`void` や `never` は、関数の戻り値の性質を表すもので、通常は変数の型情報としては使われません。

{{< code lang="ts" >}}
// 戻り値 void 型は、return 時に値を返さないことを示す
function foo(name: string): void {
  console.log(`Hello, ${name}`);
}

// 戻り値 never 型は、return することがない関数を示す
function throwError(msg: string): never {
  throw new Error(msg);
}
{{< /code >}}


{{% maku-common/reference %}}
- [typeof で型情報を調べる](/p/ods6iv8)
{{% /maku-common/reference %}}

