---
title: "TypeScriptの型: 共用体を定義する (Union types)"
linkTitle: "共用体を定義する (Union types)"
url: "/p/cdazjwv"
date: "2019-09-26"
tags: ["TypeScript"]
weight: 200
---

共用体 (union) の基本
----

変数の型をタイプアノテーションで指定するときに、複数の型を `|` (or) でつなげて指定すると、それらのいずれの型の値でも格納できる **共用体 (union)** となります。

{{< code lang="typescript" title="例: 文字列と数値を格納できる変数" >}}
let a: string | number;

a = 'Hello';  // OK
a = 123;      // OK
a = true;     // NG
{{< /code >}}

{{< code lang="typescript" title="例: 文字列あるいは数値のパラメータをとる関数" >}}
function foo(value: string | number) {
  // ...
}
{{< /code >}}


共用体のタイプエイリアスを定義する
----

ある種類の共用体を繰り返し使用する場合、タイプエイリアスを定義しておくと、何度も同じ記述をしなくて済みます。

{{< code lang="typescript" >}}
// interface Context {};

type ContextOrString = Context | string;
{{< /code >}}

定義したタイプエイリアスは、通常の型と同様に使用することができます。

{{< code lang="typescript" >}}
function onMessage(context: ContextOrString) {
  // ...
}
{{< /code >}}

