---
title: "TypeScriptの型: リテラル型を定義する (Literal types)"
linkTitle: "リテラル型を定義する (Literal types)"
url: "p/6iuo8aj/"
date: "2020-02-04"
tags: ["TypeScript"]
weight: 200
---

TypeScript のリテラル型は enum 型（列挙型）の制約を緩めたようなものです。
enum 型は **整数値** で値を保持しますが、リテラル型は **任意の型** で格納可能な値を定義します。

例えば、次のように定義した `Answer` 型は、その値として `yes` あるいは `no` という文字列のみ格納できるようになります。

{{< code lang="ts" >}}
// Answer というリテラル型を定義する
type Answer = `yes` | `no`;

// Answer 型の変数には 'yes' か 'no' のみ格納可能
let answer: Answer
answer = 'yes';    // OK
answer = 'no';     // OK
answer = 'maybe';  // Error
answer = 100;      // Error
{{< /code >}}

{{% maku-common/reference %}}
- [TypeScript: 列挙型を定義する (enum)](/p/fdjk4hh)
- [TypeScript: タイプエイリアスを定義する (type)](/p/5zoqtmi)
{{% /maku-common/reference %}}
