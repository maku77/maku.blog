---
title: "TypeScriptの型: インタフェースを結合する (交差型: Intersection types)"
linkTitle: "インタフェースを結合する (交差型: Intersection types)"
url: "/p/b4i46ah"
date: "2020-02-04"
tags: ["TypeScript"]
weight: 53
---

2 つのインタフェースを **`&`** で結ぶと、両方のインタフェースを備えていることを示す新しい型 (**交差型: Intersection type**) を定義することができます。

{{< code lang="ts" >}}
interface Foo {
  foo(): void;
}

interface Bar {
  bar(): void;
}

// インタフェースの結合
type FooAndBar = Foo & Bar;
{{< /code >}}

上記のように定義された `FooAndBar` インタフェースを実装するクラスは、`Foo` インタフェースと `Bar` インタフェースの両方を実装しなければいけません。

{{< code lang="ts" >}}
class Hoge implements FooAndBar {
  foo(): void {
    console.log('foo!!');
  }
  bar(): void {
    console.log('bar!!');
  }
}

const h = new Hoge();
h.foo();
h.bar();
{{< /code >}}

{{< note title="共用体は OR、交差型は AND" >}}
どちらかの型になることができること示す共用体 (union) が `|` （OR記号）で定義されるのに対し、両方の性質を持つ交差型 (intersection type) は `&`（AND記号）を使って定義されます。
意味が直感的に分かりやすいですね。
{{< /note >}}

