---
title: "TypeScriptの型: クラス定義の基本 (class)"
linkTitle: "クラス定義の基本 (class)"
url: "/p/qhxet9n"
date: "2020-03-31"
tags: ["TypeScript"]
weight: 100
---

TypeScript のクラスは、Java や Kotlin に似た文法で定義します。
インタフェースの定義に似ていますが、コンストラクタやメソッドを持つことができるという大きな違いがあります。

簡単なクラス
----

次の `Greeter` クラスは、1 つのプロパティ、1 つのコンストラクタ、1 つのメソッドを持っています。

{{< code lang="ts" >}}
class Greeter {
  // プロパティの定義（デフォルトで public）
  private name: string;

  // コンストラクタの定義
  constructor(name: string) {
    this.name = name;
  }

  // メソッドの定義（デフォルトで public）
  greet() {
    console.log(`Hello, ${this.name}`);
  }
}
{{< /code >}}

コンストラクタやメソッドの前に `function` というキーワードは必要ないことに注意してください。
プロパティやメソッドの可視性は __デフォルトで `public`__ になるため、外部からアクセスできないようにするには、明示的に `private` と指定する必要があります。

クラスのインスタンスを生成するには、次のように __`new`__ キーワードを使用します。

{{< code lang="ts" >}}
const greeter = new Greeter('Maku');
greeter.greet();  //=> 'Hello, Maku'
{{< /code >}}


プロパティ定義の省略
----

コンストラクタ (`constructor()`) のパラメータ名の前に、`private` や `public` といった可視性を指定すると、プロパティ定義とその値を設定するコードを省略することができます。

次のようなコードは、

{{< code lang="ts" >}}
class Book {
  private name: string;

  constructor(name: string) {
    this.name = name;
  }
}
{{< /code >}}

次のように省略して記述することができます。

{{< code lang="ts" >}}
class Book {
  constructor(private name: string) {}
}
{{< /code >}}

`public` でリードオンリーなプロパティとして定義する場合は、こんな感じ。

{{< code lang="ts" >}}
class Book {
  constructor(public readonly name: string) {}
}
{{< /code >}}


コンストラクタのデフォルト引数
----

関数のパラメータと同様に、コンストラクタのパラメータでもデフォルト引数の仕組みを使用できます。

{{< code lang="ts" >}}
class Indentor {
  constructor(private indentStr: string = '  ') {}

  indent(level: number): string {
    return this.indentStr.repeat(level);
  }
}

// コンストラクタのパラメータは省略可能
const indentor = new Indentor();
console.log(indentor.indent(0) + 'AAA');  //=> 'AAA'
console.log(indentor.indent(1) + 'BBB');  //=> '  BBB'
console.log(indentor.indent(2) + 'CCC');  //=> '    CCC'
{{< /code >}}


プロパティの setter/getter を定義する
----

メソッド名の前に __`set`__ キーワード、 __`get`__ キーワードを付けて定義することで、プロパティの setter/getter を定義することができます。
setter はプロパティへの代入時 (`obj.prop = value`) に呼び出され、getter はプロパティの参照時 (`value = obj.prop`) に呼び出されます。
setter/getter には可視性を指定することはできず、必ず `public` として扱われます。

次の例では、`title` という名前の setter/getter を定義しています。
実際の値は、`_title` というプライベートプロパティに保存しています。

{{< code lang="ts" >}}
class Book {
  private _title: string = '';

  set title(value: string) {
    console.log('setter が呼ばれたよ');
    this._title = value;
  }

  get title(): string {
    console.log('getter が呼ばれたよ');
    return this._title;
  }
}

const b = new Book();
b.title = 'タイトル';  // setter が呼ばれたよ
console.log(b.title);  // getter が呼ばれたよ → タイトル
{{< /code >}}

次のように getter だけを定義しておけば、外からは値を変更できないリードオンリーなプロパティを定義することができます。
下記の `Counter` クラスの `count` プロパティは参照専用で、外から値を代入できないようになっています。

{{< code lang="ts" >}}
class Counter {
  constructor(private _count: number = 0) {}
  tick() { this._count += 1; }
  get count(): number { return this._count; }
}

const counter = new Counter();
counter.tick();
counter.tick();
counter.tick();
console.log(counter.count);  //=> 3
{{< /code >}}

上記のように getter のみを提供することは、プロパティ自体に `readonly` キーワードを付けるのとは異なることに注意してください。
プロパティに `readonly` を付けると、クラスの中からもその値を変更できなくなります。
次のようにクラス定数を定義するときに使えます。

{{< code lang="ts" >}}
class TaxCalculator {
  private static readonly RATE = 0.10;
  static calc(price: number) : number {
    return Math.floor(price * (1 + TaxCalculator.RATE));
  }
}

console.log(TaxCalculator.calc(100));  //=> 110
{{< /code >}}


（応用）プライベートコンストラクタ
----

コンストラクタ (`constructor`) を `private` 定義することで、外部からの `new` 呼び出しを禁止することができます。
代わりに、`static` なファクトリメソッドを用意してインスタンス生成できるようにします。

{{< code lang="ts" >}}
export class MyData {
  static of(value: string): MyData {
    return new this(value)
  }

  // 外部からのコンストラクタ (new) の直接呼出しは禁止
  private constructor(public readonly value: string) {}
}
{{< /code >}}

`static` メソッドから、__`this()`__ でコンストラクタを呼び出せるところがポイントです。

