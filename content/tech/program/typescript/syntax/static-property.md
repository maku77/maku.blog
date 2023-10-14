---
title: "TypeScriptの型: クラスに static プロパティを定義する"
linkTitle: "クラスに static プロパティを定義する"
url: "p/2snuk25/"
date: "2020-02-04"
tags: ["TypeScript"]
weight: 101
---

TypeScript のクラス内に、static（静的）なプロパティを定義するには、その名の通り **`static`** キーワードを付けるだけで済みます。

下記の例では、`MyClass` クラスの中に `DEBUG` という static プロパティを定義しています。
このプロパティを参照するときは、クラス名を前に付けて、`MyClass.DEBUG` と記述します。

{{< code lang="ts" >}}
class MyClass {
  static DEBUG: boolean = true;

  hello() {
    if (MyClass.DEBUG) {
      console.log('hello!');
    }
  }
}

const obj = new MyClass();
obj.hello();
{{< /code >}}

static プロパティに、さらに **`readonly`** キーワードを付加することで、読み取り専用にすることができます（TypeScript 2.0 移行）。

{{< code lang="ts" >}}
static readonly DEBUG: boolean = true;
{{< /code >}}

{{% maku-common/reference %}}
- [TypeScript: クラス定数を定義する (static readonly)](/p/nky6cbb)
{{% /maku-common/reference %}}

