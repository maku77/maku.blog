---
title: "TypeScriptサンプル: ジェネリッククラスの定義例（MyStackクラス）"
linkTitle: "ジェネリッククラスの定義例（MyStackクラス）"
url: "/p/tkwj6bw"
date: "2020-02-27"
tags: ["TypeScript"]
---

TypeScript で型パラメータを使用したジェネリッククラスの定義サンプルとして、簡単なスタッククラスの実装例です。
TypeScript の配列がスタックの代わりになるので、スタッククラスなど作る必要はありませんが、Generics の使用例ということで。

{{< code lang="ts" title="myStack.ts" >}}
export class MyStack<T> {
  private items: T[] = [];

  push(item: T): void {
    this.items.push(item);
  }

  pop(): T | undefined {
    return this.items.pop();
  }
}
{{< /code >}}

{{< code lang="ts" title="使用例 (main.ts)" >}}
import { MyStack } from './myStack';

const stack = new MyStack<number>();
stack.push(1);
stack.push(2);
stack.push(3);
console.log(stack.pop());  //=> 3
console.log(stack.pop());  //=> 2
console.log(stack.pop());  //=> 1
console.log(stack.pop());  //=> undefined
{{< /code >}}

