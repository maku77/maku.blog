---
title: "読書メモ『Javaの鉄則』ピーター・ハガー"
linkTitle: "『Javaの鉄則』ピーター・ハガー"
url: "/p/kojyx2x"
date: "2010-11-22"
tags: ["読書", "Java"]
working: true
---

{{% amazon
  itemId="489471258X"
  title="Javaの鉄則"
  author="ピーター・ハガー"
  publisher="ピアソンエデュケーション"
  imageUrl="https://images-na.ssl-images-amazon.com/images/I/517ZSR63HHL.jpg"
%}}

鉄則1 パラメータは参照渡しではなく値で渡される
----

パラメータを参照渡しするという誤解がある。Java ではパラメータはすべて値渡しされる。オブジェクトをパラメータで渡す場合は、オブジェクトへの参照が値渡しされる。つまり、以下のようにしても呼び出し元のオブジェクトは変更されない。

{{< code lang="java" >}}
public void doSomething(MyClass obj) {
    obj = new MyClass();
}
{{< /code >}}


鉄則2 定数データおよび定数オブジェクトへの参照に final を使う
----

`final` で定数を定義すると、別の場所での **代入** がエラーになる。

{{< code lang="java" >}}
public class MyClass {
    private static final int mInt = 100;
    private static final Circle mCircle = new Circle(3.0);
    ...
{{< /code >}}

例えば上記のように宣言されている場合、以下のような代入処理がエラーになる。

{{< code lang="java" >}}
mInt = 200;  // Error!
mCircle = new Circle(5.0);  // Error!
{{< /code >}}

オブジェクトの参照先の値は変更できることに注意。

{{< code lang="java" >}}
mCircle.setRadius(6.0);  // OK
{{< /code >}}


鉄則3 非 static メソッドはデフォルトでオーバーライドできることを理解する
----

- メソッドを `final` で宣言すると、そのメソッドはサブクラスでオーバーライドできなくなる。
- クラスを `final` で宣言すると、そのクラスはサブクラス化できなくなる。すべてのメソッドを暗黙のうちに `final` で宣言するのと同じ。

▽メモ -- ユニットテストの書きやすさを考慮すると、`final` 宣言は実はあまり使うべきではない。参考：『レガシーコード改善ガイド』。


鉄則4 配列と Vector の選択に注意
----

- 配列の個々のエントリーはその型に基づくデフォルト値に設定される。数値は 0、boolean 値は `false`、オブジェクト型は `null` がデフォルト値。
- Vector はオブジェクトへの参照しか含められないが、配列はオブジェクトへの参照とプリミティブ型のどちらでも含めることができる。プリミティブ型の場合は配列を使った方が遥かに効率的。


鉄則5 instanceof よりも多態を
----

- 多態を用いることでほとんどの `instanceof` の誤用を避けることができる。`instanceof` を使用しているコードを見たら、必ずそれを除去できないか考えるべき。
- `instanceof` は効率的でないし、簡素でなく、拡張性がない。`instanceof` のような処理は本来 Java のランタイムシステムがやるべきこと。


鉄則6 絶対必要なときにしか instanceof を使わない
----

- Effective C++ で Scott Meyer は、「オブジェクトが T1 型だったらこうして、T2 型だったら他の処理をする、そのような形態のコードを書いていることに気が付いたら、自分の頬を叩きなさい」と述べている。これはまったく正しい。
- クラスライブラリの設計が不適切なために `instanceof` を使わざるを得ない状況はある。例えば、Vector のようなコレクションの集合体を自分で操作できない場合は、`instanceof` を使う必要がある。


鉄則7 必要がなくなったらオブジェクト参照に null を設定する
----

- ある参照が必要なくなったときに `null` をセットすれば、ガーベジコレクションがメモリを開放する手出すけをすることができる。
- 多くのガーベジコレクションでは、他のスレッドをすべて保留してからガーベジコレクションを実行する。


鉄則8 参照型とプリミティブ型の違いを理解する
----

- オブジェクトのスタックエントリは、オブジェクト自身ではなくオブジェクトへの参照である。オブジェクトへの参照は、ヒープのある領域を指すポインタである。
- `MyClass obj2 = obj1;` で、`obj1`、`obj2` は同じオブジェクトを参照することになる。


鉄則9 == と equals を区別する
----

オブジェクト同士の `==` での比較は、2つの参照が同じオブジェクトを指しているかを調べるだけ。つまり、`obj1`、`obj2` が保持しているオブジェクトの実体へのアドレスが等しいかを見ているだけ。

意味的に2つのオブジェクトの内容が等しいことを調べるには、`equals()` メソッドを使用する。`==` が真なら `equals` も必ず真になるが、`equals` が真のときに `==` が真になるとを限らない。

{{< code lang="java" >}}
Integer obj1 = new Integer(1);
Integer obj2 = new Integer(1);
System.out.println(obj1 == obj2);      // false !!
System.out.println(obj1.equals(obj2);  // true
{{< /code >}}

通常は異なるクラスのオブジェクトを `equals` で比較しても `true` になることはない。

{{< code lang="java" >}}
Integer iVal = new Integer(100);
Float fVal = new Float(100.0f);
System.out.println(iVal == fVal);  // false
{{< /code >}}


鉄則10 equals のデフォルト実装をあてにしない
----

`equals` を実装していないクラスでは、デフォルトでは以下のような `java.lang.Object` の `equals` 実装が使用される。

{{< code lang="java" >}}
public boolean equals(Object obj) {
    return (this == obj);
}
{{< /code >}}

つまり、`equals` を実装していないクラスでは、以下の2つは同じ意味になる。

{{< code lang="java" >}}
if (obj1 == obj2) { ... }
if (obj1.equals(obj2) { ... }
{{< /code >}}

`equals` は大体次のように実装する。メンバフィールドの比較も、オブジェクト同士の比較の場合は `==` ではなくて `equals` を使用すること。

{{< code lang="java" >}}
@Override
public boolean equals(Object obj) {
    if (this == obj) {
        return true;
    }

    if (obj == null || getClass() != obj.getClass() || !super.equals(obj)) {
        return false;
    }

    Person p = (Person) obj;
    return mName.equals(p.getName()) &&
           mAddress.equals(p.getAddress()) &&
           mAge == p.getAge();
}
{{< /code >}}

※メモ

- `float` は `Float.floatToIntBits` で `int` に変換してから `==` で比較する
- `double` は `Double.doubleToLongBits` で `long` に変換して `==` で比較する


鉄則11 よく考えてから equals メソッドを実装する
----

2つのオブジェクトがメモリ上で同じ空間を占めていなくても、**意味的に同じだと判断できることがある場合は `equals` メソッドを実装する**。


鉄則12 equals メソッドの実装には getClass を使うのがよい
----

`getClass()` を使って、2つのオブジェクトの型が同じでないときは `false` を返すようにするのが一番安全な方法。


鉄則13 基底クラスの super.equals を呼び出す
----

サブクラスで `equals` メソッドを実装する場合は、親クラスのフィールドの同一性を `super.equals` を使ってチェックする。


鉄則14 equals メソッド実装での instanceof 使用はよく考える
----

派生クラスのオブジェクトと、基底クラスのオブジェクトが等しいと判断する `equals` を実装するには、`getClass` の代わりに `instanceof` を使用する必要がある。

{{< code lang="java" >}}
public boolean equals(Object obj) {
    if (this == obj) {
        return true;
    }

    if (obj == null || !(obj instanceof Base) || !super.equals(obj)) {
        return false;
    }
    ...
}
{{< /code >}}

このような、基底オブジェクトと派生オブジェクトの比較は受け入れがたい問題を引き起こすことがあるので、できれば `equals` メソッドの実装では、`instanceof` ではなく `getClass` でのクラス同一性を調べるようにした方がよい。

例えば、`instanceof` を使用して `equals` メソッドを実装してしまうと、対称性の問題が出ててくる。つまり、以下のような比較が同一の結果を返さなくなる。

{{< code lang="java" >}}
if (base.equals(derived)) { ... }
if (derived.equals(base)) { ... }
{{< /code >}}

基底クラスで `instanceof` を使用した `equals` メソッドが実装されている場合でも、その派生クラスを作成する場合は、`equals` の実装で `getClass()` を使用するのが最良の方法。ただし、この場合も対称性の問題は避けられない。

Java ライブラリでは、`equals` メソッドの実装で `getClass` を使用したり、`instanceof` を使用したり一貫性がない。


鉄則15 equals メソッドを実装する際はこのルールに従うこと
----

- `this` との比較を行う。
- `Object` 以外の基底クラスが `equals` を提供する場合は、`super.equals` を呼び出す。
- 基本的に `getClass` を使用する。
- `instanceof` で基底クラスのオブジェクトとの一致を許す場合は、`instanceof` の問題を理解してから実装すること。


鉄則16 例外制御フローのメカニズムを理解する
----

例外が発生すると、制御は即座に以下の３つのうちの１つに移る。

- `catch` ブロック
- `finally` ブロック
- それを呼び出したメソッド


鉄則17 例外を無視しない
----

Java では、例外が発生して捕えられないと、例外が起きたスレッドが終了する。


鉄則18 例外を隠さない
----

例外を処理している間に、`catch` あるいは `finally` ブロックから別の例外がスローされると、もとの例外の情報が見えなくなってしまう。ひとつの解決策としては、発生したすべての例外をリストに保存する方法がある。


鉄則19 throws 文節の欠点を知る
----

あるメソッドの `throws` 文節に例外を追加すると、それを間接的に呼び出す全てのメソッドに影響する。エラー処理に関しては開発の初期の段階から計画的に構築し、後から例外処理を追加するのはなるべく避けるようにする。


