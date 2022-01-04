---
title: "TypeScriptの型: インタフェースを定義する (interface)"
linkTitle: "インタフェースを定義する (interface)"
url: "/p/5q5gs4f"
date: "2020-06-22"
tags: ["TypeScript"]
weight: 50
description: "TypeScript のインタフェースは、あるインスタンスがどのようなプロパティ、メソッドを持っているかの仕様を定義します。"
---

プロパティを定義する
----

次の例では、2 つのプロパティ `x`、`y` を持つインタフェース `Point` を定義しています。

{{< code lang="typescript" >}}
// Point インタフェースの定義
interface Point {
  x: number;
  y: number;
}
{{< /code >}}

このインタフェースを使って、次のようにオブジェクトを生成することができます。
オブジェクトを生成するときには、インタフェースで定義されているすべてのプロパティに値を設定してやる必要があります。

{{< code lang="typescript" >}}
// Point 型のオブジェクトを生成
const p: Point = { x: 10, y: 20 };

// 内容を出力してみる
console.log(`x=${p.x}, y=${p.y}`);  //=> x=10, y=20
{{< /code >}}

Java などと異なり、TypeScript ではインタフェース定義さえあれば、上記のようにその型のオブジェクトを生成することができます（Java では厳密にはクラス定義がないとオブジェクトを生成できません）。
そのため、TypeScript では、`interface` キーワードを使ったインタフェース定義の頻度が高くなります。

TypeScript のインタフェースは、拡張に対してオープンであり、次のように後付けでプロパティを追加することができます。

{{< code lang="typescript" >}}
interface Point {
  x: number;
  y: number;
}

// Point インタフェースにプロパティを追加
interface Point {
  z: number;
}

const p: Point = { x: 1, y: 2, z: 3 };
{{< /code >}}


メソッドを定義する
----

インタフェースでは、オブジェクトが持つべきメソッドを定義することができます。
次の `Product` インタフェースは、2 つのプロパティ `name`、`price` の他に、1 つのメソッド `calcTotal()` を持つ型を示しています。

{{< code lang="typescript" >}}
// Product インタフェースを定義
interface Product {
  name: string;
  price: number;
  calcTotal(quantity: number): number;
}
{{< /code >}}

この `Product` 型のオブジェクトを生成するには次のようにします。
プロパティの値だけではなく、メソッドの実装もこのタイミングで渡す必要があります。

{{< code lang="typescript" >}}
// Product 型のオブジェクトを生成
const prod: Product = {
  name: 'Brown Shelf',
  price: 250,
  calcTotal(quantity: number): number {
    return this.price * quantity;
  }
};

console.log(prod.calcTotal(4));  //=> 1000
{{< /code >}}

もっとも、オブジェクト生成時に上記のように毎回メソッドの実装を渡していては大変なので、このような場合は、通常はクラス定義を使用します。

- 参考: [クラス定義の基本 (class)](/p/qhxet9n)

