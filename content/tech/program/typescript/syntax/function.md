---
title: "TypeScriptの型: 関数を定義する (function)"
linkTitle: "関数を定義する (function)"
url: "/p/tiwamzb"
date: "2020-06-22"
tags: ["TypeScript"]
weight: 30
---

TypeScript の関数定義は JavaScript とほぼ同じですが、各パラメータと戻り値に型アノテーション付けることができます。

簡単な関数
----

次の `indent` 関数は、指定したレベルのインデント文字列（レベル数x2 の半角スペース）を返します。
TypeScript の型アノテーションで、`number` 型のパラメータと、`string` 型の戻り値を持つことを示しています。

{{< code lang="typescript" >}}
function indent(level: number): string {
  return '  '.repeat(level);
}

console.log(indent(0) + 'AAA');  //=> 'AAA'
console.log(indent(1) + 'BBB');  //=> '  BBB'
console.log(indent(2) + 'CCC');  //=> '    CCC'
{{< /code >}}


関数オブジェクト
----

名前なしの関数オブジェクトやアロー関数を使用するときも同様に、パラメータや戻り値の型をアノテートできます。

{{< code lang="typescript" >}}
const add = function(a: number, b: number) { return a + b; };
const sub = (a: number, b: number) => { return a - b; };

console.log(add(1, 2));  //=> 3
console.log(sub(1, 2));  //=> -1
{{< /code >}}

関数の実装から戻り値の型を推測できる場合は、上記のように戻り値の型アノテーションを省略できます（この場合は `: number` が省略されています）。


戻り値を返さないを示す void
----

関数を抜ける時に、何も値を返さない場合は、戻り値の型を __`void`__ にします。

{{< code lang="typescript" >}}
function greet(name: string): void {
  console.log(`Hello, ${name}`);
}

greet('Maku');  //=> 'Hello, Maku'
{{< /code >}}

戻り値を持たない関数はよくあるので、`void` 型のアノテーションは __省略してもよい__ ことになっています。

{{< code lang="typescript" >}}
function greet(name: string) {
  console.log(`Hello, ${name}`);
}
{{< /code >}}


関数が終わらないことを示す never
----

戻り値の型として __`never`__ を指定すると、その関数は終了することがないことを示します。
無限ループはその典型的な例です。

{{< code lang="typescript" >}}
function neverEnding(): never {
  while (true) {
    console.log('Hello');
  }
}

neverEnding();
{{< /code >}}

必ず例外をスローする関数も `never` でアノテートできます。

{{< code lang="typescript" >}}
function error(msg: string): never {
  throw Error(`My error - ${msg}`);
}

error('Hoge');
{{< /code >}}


オプショナルパラメータとデフォルト引数
----

### オプショナルパラメータ

パラメータ名の末尾に __`?`__ を付けると、そのパラメータはオプショナル扱いとなり、関数の呼び出し時に引数を省略することができるようになります。

{{< code lang="typescript" >}}
function greet(name?: string) {
  if (name) {
    console.log(`Hello, ${name}`);
  } else {
    console.log('名を名乗れっ！');
  }
}

greet('まく');  //=> 'Hello, まく'
greet();        //=> '名を名乗れっ！'
greet(undefined);  //=> （同上）
greet('');         //=> （同上）
{{< /code >}}

引数を省略された場合、関数内でその値を参照すると __`undefined`__ になります。
JavaScript では、`undefined` を真偽値として評価すると `false` 扱いとなるので、上記のように `if` 文で条件分岐させることができます。

ただし、空文字列 (`''`) も `false` として評価されることに注意してください。
変数の値が `undefined` であることを確実にチェックしたいときは、次のように `typeof` を使って判定してください。

{{< code lang="typescript" >}}
function greet(name?: string) {
  if (typeof name === 'undefined') {
    console.log('名を名乗れっ！');
  } else {
    console.log(`Hello, ${name}`);
  }
}

greet('まく');  //=> 'Hello, まく'
greet();        //=> '名を名乗れっ！'
greet(undefined);  //=> （同上）
greet('');         //=> 'Hello, '
{{< /code >}}

### デフォルト引数

引数が省略された場合のデフォルト値を指定したい場合は、次のように __デフォルト引数__ の仕組みを使用すると簡単です。

{{< code lang="typescript" >}}
function greet(name: string = '名無しさん') {
  console.log(`Hello, ${name}`);
}

greet('まく');  //=> 'Hello, まく'
greet();  //=> 'Hello, 名無しさん'
{{< /code >}}

