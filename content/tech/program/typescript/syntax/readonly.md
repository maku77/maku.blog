---
title: "TypeScriptの型: インタフェースのプロパティを読み取り専用にする (readonly)"
linkTitle: "インタフェースのプロパティを読み取り専用にする (readonly)"
url: "/p/p8ir2ai"
date: "2020-06-22"
tags: ["TypeScript"]
weight: 52
---

インタフェースのプロパティの前に、 __`readonly`__ キーワードを付けると、そのプロパティは読み取り専用になり、オブジェクト生成時に格納した値を変更できなくなります。

次の `Book` インタフェースは 3 つのプロパティを持ちますが、そのうち `id` プロパティだけがリードオンリーとして定義されています。

{{< code lang="typescript" >}}
interface Book {
  readonly id: string;  // id プロパティはリードオンリー
  title: string;
  price: number;
}
{{< /code >}}

次の例では `Book` オブジェクトを作成した後で各プロパティの値を変更しようとしていますが、`id` プロパティを変更しようとしている部分でトランスパイルエラーになります。

{{< code lang="typescript" >}}
const book: Book = {
  id: '001', title: 'TypeScript is Awesome', price: 2300
};

book.id = '002';  // Error: リードオンリープロパティ
book.title = 'TypeScript is Horrible';
book.price = 800;
{{< /code >}}

