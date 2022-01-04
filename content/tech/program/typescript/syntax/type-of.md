---
title: "TypeScriptの型: 値の型を typeof で調べる"
linkTitle: "値の型を typeof で調べる"
url: "/p/ods6iv8"
date: "2020-04-28"
tags: ["TypeScript"]
weight: 3
---

次のサンプルコードでは、それぞれの変数値の型情報を、JavaScript の `typeof` で調べています（ついでに、`Object.prototype.toString.call` でもう少し詳しい情報も表示）。

{{< code lang="ts" >}}
// ヘルパ関数
function printType(x: any) {
  console.log(`${typeof(x)} ${Object.prototype.toString.call(x)}`);
}

let booleanValue: boolean = true;
printType(booleanValue);  //=> boolean [object Boolean]

let numberValue: number = 100;
printType(numberValue);  //=> number [object Number]

let stringValue: string = 'Hello';
printType(stringValue);  //=> string [object String]

function func(): number { return 100; }
printType(func);  //=> function [object Function]

let undefinedValue: undefined = undefined;
printType(undefinedValue);  //=> undefined [object Undefined]

let tupleValue: [string, number] = ['Hello', 100];
printType(tupleValue);  //=> object [object Array]

let arrayValue: number[] = [1, 2, 3];
printType(arrayValue);  //=> object [object Array]

let nullValue: null = null;
printType(nullValue);  //=> object [object Null]

let general: any = { name: 'Mofu' };
printType(general);  //=> object [object Object]

let sym: symbol = Symbol('key');  // ES2015
printType(sym);  //=> symbol [object Symbol]

let x: bigint = 1n;  // ES2020
printType(x);  //=> bigint [object BigInt]

enum Color { Red, Green, Yellow };  // これは数値と同じ扱い
printType(Color.Red);  // number [object Number]
{{< /code >}}

注意しなければいけないのは、JavaScript の `typeof` は、TypeScript のタイプアノテーションで指定した型を返してくれるわけではないということです。
`typeof` が返す型情報は、JavaScript コードとして実行したときに、実際にその値がどのような型で扱われているかを示すものです。
なので、TypeScript 独自のタイプアノテーション情報（Tuple 型など）が調べられるわけではありません。
TypeScript のタイプアノテーションは、JavaScript へのトランスパイル時に使われる参考情報でしかありません。

これは、次のように「何でも型」の `any` で変数を定義し、具体的な値として数値、文字列、オブジェクトを格納してみるとよくわかります。

{{< code lang="ts" >}}
let x: any = 100;
printType(x);  //=> number [object Number]

x = 'Hello';
printType(x);  //=> string [object String]

x = {'a': 100};
printType(x);  //=> object [object Object]
{{< /code >}}

共用体 (Union) 型を使った場合も同様。

{{< code lang="ts" >}}
let x: string | null;
x = 'Hello';
printType(x);  //=> string [object String]
x = null;
printType(x);  //=> object [object Null]
{{< /code >}}

`typeof` は、あくまで __実行時の値__ の型を調べるものだということですね。

