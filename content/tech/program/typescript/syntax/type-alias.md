---
title: "TypeScriptの型: タイプエイリアスを定義する (type)"
linkTitle: "タイプエイリアスを定義する (type)"
url: "/p/5zoqtmi"
date: "2019-09-26"
tags: ["TypeScript"]
weight: 200
---

TypeScript の **タイプエイリアス (Type alias)** を使用すると、既存の型を組み合わせて新たな型を定義することができます。

下記の例では、string 型と number 型の 2 つの値を保持する `Person` という型を定義しています。
タイプエイリアスの定義では、代入 (`=`) の構文を使用すること注意してください。

{{< code lang="typescript" >}}
type Person = {
  name: string;
  age: number;
};

let p: Person = { name: 'Maku', age: 14 };
console.log(p.name);
console.log(p.age);
{{< /code >}}

`Person` 型の変数を初期化するときは、上記のように各プロパティの値を漏れなく明示する必要があります。
次のように、プロパティの指定に過不足があるとコンパイルエラーが発生します。

{{< code lang="typescript" title="間違った例" >}}
let p1: Person = { name: 'Maku' };  // プロパティが不足
let p2: Person = { name: 'Maku', age: 14, xxx: 20 };  // 未知のプロパティ
{{< /code >}}

{{< code title="エラーメッセージ" >}}
sample.ts:6:5 - error TS2741: Property 'age' is missing in type '{ name: string; }' but required in type 'Person'.
sample.ts:7:43 - error TS2322: Type '{ name: string; age: number; xxx: number; }' is not assignable to type 'Person'.
  Object literal may only specify known properties, and 'xxx' does not exist in type 'Person'.
{{< /code >}}

{{< reference >}}
- [TypeScript: リテラル型を定義する (Literal types)](/p/6iuo8aj)
{{< /reference >}}

