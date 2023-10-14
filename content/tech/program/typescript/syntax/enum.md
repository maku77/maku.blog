---
title: "TypeScriptの型: 列挙型を定義する (enum)"
linkTitle: "列挙型を定義する (enum)"
url: "p/fdjk4hh/"
date: "2019-09-26"
tags: ["TypeScript"]
weight: 200
---

列挙型 (enum) の基本
----

TypeScript で列挙型を定義するには、**`enum`** キーワードを使用します。

{{< code lang="typescript" >}}
enum Fruits {
  Apple,   //= 0
  Banana,  //= 1
  Orange   //= 2
}

console.log(Fruits.Apple);  //=> 0
console.log(Fruits[Fruits.Apple]);  //=> Apple
{{< /code >}}

デフォルトでは、各要素の値として先頭から順番に 0、1、2 という連番の **整数値** が内部的に割り当てられます（C 言語や Java と同様です）。
この値は任意の数値に変更することができます。

次の例は、ビットフラグとして使用することを想定した enum の定義例です。
各値が 2 のべき乗の値になっていることに注意してください。

{{< code lang="typescript" >}}
enum OpenModes {
  Read = 1,
  Write = 2,
  Append = 4
}

const mode = OpenModes.Read | OpenModes.Write;

if ((mode & OpenModes.Write) == OpenModes.Write) {
  console.log('Write フラグが指定されています');
}
{{< /code >}}


enum 値をインライン展開する (Constant enumeration)
----

enum 定義を行うときに **`const`** キーワードを付加すると、その enum 値を使用した場所に値がハードコードされる形で展開されます（JavaScript のコードに変換するときに、0 や 1 といった値をインライン展開する）。

{{< code lang="typescript" title="TypeScript のコード" >}}
enum NormalEnum { A, B, C }
const enum ConstEnum { A, B, C }

console.log(NormalEnum.A)
console.log(ConstEnum.A)
{{< /code >}}

上記の TypeScript が JavaScript にトランスパイルされると、enum 値の参照箇所はそれぞれ下記のように出力されます。

{{< code lang="javascript" title="JavaScript に変換されたコード" >}}
// ...省略...

console.log(NormalEnum.A);
console.log(0 /* A */);
{{< /code >}}

`ConstEnum.A` という部分は、0 という値でハードコードされているのが分かります。

{{% maku-common/reference %}}
- [TypeScript: リテラル型を定義する (Literal types)](/p/6iuo8aj/)
{{% /maku-common/reference %}}

