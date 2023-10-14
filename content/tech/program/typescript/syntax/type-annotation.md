---
title: "TypeScriptの型: 変数の型指定（タイプアノテーション）の基本"
linkTitle: "変数の型指定（タイプアノテーション）の基本"
url: "p/ugyw5ee/"
date: "2019-09-26"
tags: ["TypeScript"]
weight: 2
---

TypeScript の特徴は、変数の型を明示的に指定できることです。
この型指定のことを **タイプアノテーション (Type annotation)** と呼びます。


単純な変数のタイプアノテーション
----

下記は、文字列、数値、真偽値、配列のタイプアノテーションの例です。

{{< code lang="ts" >}}
let user: string = 'まく';
let age: number = 14;
let isActive: boolean = true;
let titles: string[] = ['名前1', '名前2', '名前3'];
{{< /code >}}

指定した型と異なる型の値を代入しようとするとエラーになります。

{{< code lang="ts" >}}
let age: number = 14;
age = '100歳';
{{< /code >}}

{{< code title="エラーメッセージ" >}}
Cannot assign to 'age' because it is a constant.
{{< /code >}}

配列や辞書、クラスなどの使い方は下記のページを参考にしてください。

- [配列を定義する (Array)](/p/ttwoetd/)
- [辞書型を定義する (Dictionary)](/p/x3ocp9a/)
- [クラス定義の基本 (class)](/p/qhxet9n/)


関数のタイプアノテーション
----

関数の型もアノテーションで示すことができます。

{{< code lang="ts" >}}
// 文字列を受け取り、何も返さない関数
let logger: (name: string) => void;

// 2つの数値を受け取り、真偽値を返す関数
let equal: (a: number, b: number) => boolean;
{{< /code >}}

これらの変数には、パラメータと戻り値の型が等しく定義された関数のみ代入することができます。

{{< code lang="ts" >}}
function func1(name: string) {
  console.log("Hello, " + name);
}

function func2(a: number, b: number): boolean {
  return a == b;
}

logger = func1;
equal = func2;

logger('Maku');  // Hello, Maku
console.log(hikaku(10, 20));  // false
{{< /code >}}

