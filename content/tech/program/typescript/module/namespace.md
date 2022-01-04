---
title: "TypeScriptで名前空間を定義する (namespace)"
linkTitle: "名前空間を定義する (namespace)"
url: "/p/a3eh9w2"
date: "2019-11-21"
tags: ["JavaScript", "TypeScript"]
weight: 210
---

namespace ではなくモジュールの仕組みを使うべし
----

TypeScript では、**`namespace`** キーワードを使って名前空間を定義することができますが、通常はより柔軟性の高い [モジュールの仕組み](/p/fbu8k8j) を使うようにしてください（といっても `.ts` ファイルを分けるだけですが）。

`namespace` を使うと、同じファイル内で階層化された名前空間を作ることができますが、あくまでその階層構造はグローバルに共有されています。
一方、モジュールの仕組みを使うと、ファイル単位で名前空間のコンテキストを分けることができます。
大きなプロジェクトであっても、適切な単位でモジュール（ファイル）を分割している限り、名前の衝突は本質的には発生しません。

とはいえ、これは `namespace` の記事なので、ここからは `namespace` の使い方の説明をします。


namespace による名前空間の定義
----

`namespace` による名前空間の定義は簡単で、`namespace Xxx { ... }` というブロックで囲むだけです。
次の例では、`First` と `Second` という名前空間を作成し、それぞれに同じ名前の `Person` というクラスを定義しています。

{{< code lang="typescript" >}}
namespace First {
  export class Person {
    greet() { console.log('First.Person'); }
  }
}

namespace Second {
  export class Person {
    greet() { console.log('Second.Person'); }
  }
}

const p1 = new First.Person();
const p2 = new Second.Person();

p1.greet();  //=> First.Person
p2.greet();  //=> Second.Person
{{< /code >}}

`namespace` の中で定義したクラスなどは、デフォルトではその外の名前空間からは見えないようになっています。
外からアクセスしたい場合は、上記のようにクラス定義の前に **`export`** を付けて公開しておく必要があります。

名前空間は、ドットで区切って入れ子で定義することもできます。

{{< code lang="typescript" >}}
namespace First.Second.Third {
  export class Person {
    greet() { console.log('Hello'); }
  }
}

const p = new First.Second.Third.Person();
{{< /code >}}

上記は、下記のように定義するのと同等です。

{{< code lang="typescript" >}}
namespace First {
  export namespace Second {
    export namespace Third {
      export class Person {
        greet() { console.log('Hello'); }
      }
    }
  }
}
{{< /code >}}

内部の `namespace` は、`export` で公開しなければ外からアクセスできないため、上記のように `export` の連続になります。

`import` を使って、名前空間内のメンバーに別名（エイリアス）をつけてアクセスすることもできます。

{{< code lang="typescript" >}}
namespace First {
  export class Person {
    greet() { console.log('First.Person'); }
  }
}

import Person = First.Person;
const p = new Person();
p.greet();
{{< /code >}}

同じ階層にあるクラスと名前空間名が同じ名前の場合は、クラス定義が先にある限り TypeScript トランスパイラがうまくマージしてくれます。

{{< code lang="typescript" >}}
class Foo {
  greet() {
    console.log('greet1');
  }
}

namespace Foo {
  export function greet() {
    console.log('greet2');
  }
}

new Foo().greet();
Foo.greet();
{{< /code >}}

もちろん、このような名前の重複は発生しないように定義すべきですし、そもそも `namespace` は使わずに [モジュールの仕組み](/p/fbu8k8j) だけで名前空間の管理をすれば、このような混乱が発生することはないでしょう。

