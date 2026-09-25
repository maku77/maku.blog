---
title: "読書メモ『Javaの鉄則』ピーター・ハガー"
linkTitle: "『Javaの鉄則』ピーター・ハガー"
url: "/p/kojyx2x"
date: "2010-11-22"
lastmodSage: "2026-09-25"
tags: ["読書", "Java"]
---

{{< amazon
  itemId="489471258X"
  title="Javaの鉄則"
  author="ピーター・ハガー"
  publisher="ピアソンエデュケーション"
  imageUrl="https://images-na.ssl-images-amazon.com/images/I/517ZSR63HHL.jpg"
>}}

第1章 一般的なテクニック
----

### 鉄則1 パラメータは参照渡しではなく値で渡される

パラメータを参照渡しするという誤解がある。Java ではパラメータはすべて値渡しされる。オブジェクトをパラメータで渡す場合は、オブジェクトへの参照が値渡しされる。つまり、以下のようにしても呼び出し元のオブジェクトは変更されない。

```java
public void doSomething(MyClass obj) {
    obj = new MyClass();
}
```

### 鉄則2 定数データおよび定数オブジェクトへの参照に final を使う

`final` で定数を定義すると、別の場所での **代入** がエラーになる。

```java
public class MyClass {
    private static final int mInt = 100;
    private static final Circle mCircle = new Circle(3.0);
    ...
```

例えば上記のように宣言されている場合、以下のような代入処理がエラーになる。

```java
mInt = 200;  // Error!
mCircle = new Circle(5.0);  // Error!
```

オブジェクトの参照先の値は変更できることに注意。

```java
mCircle.setRadius(6.0);  // OK
```

### 鉄則3 非 static メソッドはデフォルトでオーバーライドできることを理解する

- メソッドを `final` で宣言すると、そのメソッドはサブクラスでオーバーライドできなくなる。
- クラスを `final` で宣言すると、そのクラスはサブクラス化できなくなる。すべてのメソッドを暗黙のうちに `final` で宣言するのと同じ。

▽メモ -- ユニットテストの書きやすさを考慮すると、`final` 宣言は実はあまり使うべきではない。参考: [『レガシーコード改善ガイド』](/p/p6awy3z/)。

### 鉄則4 配列と Vector の選択に注意

配列の個々のエントリーはその型に基づくデフォルト値に設定される。
数値は 0、boolean 値は `false`、オブジェクト型は `null` がデフォルト値。

`Vector` はオブジェクトへの参照しか含められないが、配列はオブジェクトへの参照とプリミティブ型のどちらでも含めることができる。
プリミティブ型の場合は配列を使った方が遥かに効率的である。

### 鉄則5 instanceof よりも多態を

多態を用いることでほとんどの `instanceof` の誤用を避けることができる。
**`instanceof` を使用しているコードを見たら、それを除去できないか考えるべき**。
`instanceof` は効率的でなく、簡素でもなく、拡張性もない。
`instanceof` のような処理は本来 Java のランタイムシステムがやるべきである。

### 鉄則6 絶対必要なときにしか instanceof を使わない

Effective C++ で Scott Meyer は、「オブジェクトが T1 型だったらこうして、T2 型だったら他の処理をする、そのような形態のコードを書いていることに気が付いたら、自分の頬を叩きなさい」と述べている。これはまったく正しい。

クラスライブラリの設計が不適切なために `instanceof` を使わざるを得ない状況はある。
例えば、`Vector` のようなコレクションの集合体を自分で操作できない場合は、`instanceof` を使う必要がある。

### 鉄則7 必要がなくなったらオブジェクト参照に null を設定する

ある参照が必要なくなったときに `null` をセットすれば、ガーベジコレクションがメモリを解放する手助けをすることができる。
多くのガーベジコレクションでは、他のスレッドをすべて保留してからガーベジコレクションを実行する。


第2章 オブジェクトと等値
----

### 鉄則8 参照型とプリミティブ型の違いを理解する

オブジェクトへの参照は、ヒープのある領域を指すポインタである。
オブジェクトのスタックエントリは、オブジェクト自身ではなくオブジェクトへの参照である。
`MyClass obj2 = obj1;` とすると、`obj1` と `obj2` は同じオブジェクトを参照することになる。

### 鉄則9 == と equals を区別する

オブジェクト同士の `==` での比較は、2つの参照が同じオブジェクトを指しているかを調べるだけである。
つまり、`obj1`、`obj2` が保持しているオブジェクトの実体へのアドレスが等しいかを見ているだけ。

意味的に2つのオブジェクトの内容が等しいことを調べるには、`equals()` メソッドを使用する。
**`==` が真なら `equals` も必ず真になるが、`equals` が真のときに `==` が真になるとは限らない**。

```java
Integer obj1 = new Integer(1);
Integer obj2 = new Integer(1);
System.out.println(obj1 == obj2);      // false
System.out.println(obj1.equals(obj2);  // true
```

通常、異なるクラスのオブジェクトを `equals` で比較した場合に `true` になることはない。

```java
Integer iVal = new Integer(100);
Float fVal = new Float(100.0f);
System.out.println(iVal == fVal);  // false
```

### 鉄則10 equals のデフォルト実装をあてにしない

`equals` を実装していないクラスでは、以下のような `java.lang.Object` のデフォルトの `equals` 実装が使用される。

```java
public boolean equals(Object obj) {
    return (this == obj);
}
```

これは参照を比較しているだけなので、`equals` を実装していないクラスでは以下の2つは同じ意味になる。

```java
if (obj1 == obj2) { ... }
if (obj1.equals(obj2) { ... }
```

`equals` は大体次のように実装する。
`equals` の実装コードにおいてメンバフィールドを比較する場合は、それがオブジェクト同士の比較であれば `==` ではなくて `equals` を使用すること。

```java
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
        mAddress.equals(p.getAddress()) && mAge == p.getAge();
}
```

- `float` 値は `Float.floatToIntBits` で `int` に変換してから `==` で比較する。
- `double` 値は `Double.doubleToLongBits` で `long` に変換して `==` で比較する。

### 鉄則11 よく考えてから equals メソッドを実装する

2つのオブジェクトがメモリ上で同じ空間を占めていなくても、**意味的に同じだと判断できる場合は `equals` メソッドを実装する**。

### 鉄則12 equals メソッドの実装には getClass を使うのがよい

`getClass()` を使って、2つのオブジェクトの型が同じでないときは `false` を返すようにするのが一番安全。

### 鉄則13 基底クラスの super.equals を呼び出す

サブクラスで `equals` メソッドを実装する場合は、**親クラスのフィールドの同一性を `super.equals` を使ってチェックする**。

### 鉄則14 equals メソッド実装での instanceof 使用はよく考える

派生クラスのオブジェクトと、基底クラスのオブジェクトが等しいと判断する `equals` を実装するには、`getClass` の代わりに `instanceof` を使用する必要がある。

```java
public boolean equals(Object obj) {
    if (this == obj) {
        return true;
    }

    if (obj == null || !(obj instanceof Base) || !super.equals(obj)) {
        return false;
    }
    ...
}
```

このような、基底オブジェクトと派生オブジェクトの比較は受け入れがたい問題を引き起こすことがあるので、できれば `equals` メソッドの実装では、`instanceof` ではなく `getClass` でのクラス同一性を調べるようにした方がよい。

例えば、`instanceof` を使用して `equals` メソッドを実装してしまうと、対称性の問題が出てくる。
つまり、以下のような比較が同一の結果を返さなくなる。

```java
if (base.equals(derived)) { ... }
if (derived.equals(base)) { ... }
```

基底クラスで `instanceof` を使用した `equals` メソッドが実装されている場合でも、その派生クラスを作成する場合は、`equals` の実装で `getClass()` を使用するのが最良の方法。
ただし、この場合も対称性の問題は避けられない。

Java の標準ライブラリの `equals` メソッドの実装では、`getClass` を使用したり `instanceof` を使用したり一貫性がない。

### 鉄則15 equals メソッドを実装する際はこのルールに従うこと

- `this` との比較を行う。
- `Object` 以外の基底クラスが `equals` を提供する場合は、`super.equals` を呼び出す。
- 基本的に `getClass` を使用する。
- `instanceof` で基底クラスのオブジェクトとの一致を許す場合は、`instanceof` の問題を理解してから実装すること。


第3章 例外処理
----

### 鉄則16 例外制御フローのメカニズムを理解する

例外が発生すると、制御は即座に以下の3つのうちの1つに移る。

- `catch` ブロック
- `finally` ブロック
- それを呼び出したメソッド

### 鉄則17 例外を無視しない

Java では、例外が発生して捕えられないと、**例外が起きたスレッドが終了する**。

### 鉄則18 例外を隠さない

例外を処理している間に、`catch` あるいは `finally` ブロックから別の例外がスローされると、もとの例外の情報が見えなくなってしまう。
ひとつの解決策としては、発生したすべての例外をリストに保存する方法がある。

### 鉄則19 throws 文節の欠点を知る

あるメソッドの `throws` 文節に例外を追加すると、それを間接的に呼び出す全てのメソッドに影響する。
**エラー処理に関しては開発の初期の段階から計画的に構築し、後から例外処理を追加するのはなるべく避けるようにする**。

### 鉄則20 throws 文節にはすべての例外を明示する

Exception1 が Exception2 や Exception3 を継承していて、あるメソッドがこれら3種類の例外を投げる可能性がある場合は、`throws` 文節にはすべての例外を明示すること。

```java
// よくない例
public void foo(int i) throws Exception1 {
}

// よい例
public void foo(int i) throws Exception1, Exception2, Exception3 {
}
```

例外クラスの派生元が同じであっても、エラーの種類がまったく異なることもある。

### 鉄則21 finally を使ってリソースのリークを回避する

`try` ブロックの先頭でリソースをオープンし、`finally` ブロックでクローズする。

### 鉄則22 try ブロックから return しない

`try` ブロックから `return`、`break`、`continue` を呼んでブロックを抜けようとしても `finally` ブロックは必ず実行される。
不具合のもとになるのでこのようなコードは書かないようにする。

```java
public int hoge() {
    try {
        return 1;  // ここで return しても finally ブロックが呼び出される
    } finally {
        return 2;  // 結果的にこのメソッドは必ず 2 を返す
    }
}
```

### 鉄則23 try/catch ブロックをループの外側に配置する

for ループの内側に `try/catch` ブロックを配置すると、ループのたびに分岐コストがかかってパフォーマンスが低下する。
`try/catch` ブロックはループの外側に配置すること。
JIT が最適化してくれる場合もあるが、それには頼らない。

### 鉄則24 制御フローに例外を使用しない

例外のスローにはコストがかかるので、制御フローには使わない。
例えば、`for` ループから抜けるために `throw` を使ったりしない（非効率かつメンテナンスが困難）。

### 鉄則25 例外を用いるべきエラー状況をよく考えること

例外は、通常の動作の一部ではない、予期せぬ状況に対して使用すること。
例えば、ファイル読み出しにおいてストリームの終端に達したことを表すには、例外を投げるのではなく、ただ 0 を返すのが望ましい。

### 鉄則26 コンストラクタから例外をスローすること

オブジェクトの生成に失敗したことを示すときは、コンストラクタから例外をスローすればよい。
コンストラクタは戻り値を持たないが、例外をスローすることはできる。

### 鉄則27 オブジェクトを有効な状態に戻してから例外をスローすること

例外をスローする前に、オブジェクトのプロパティを整合性の取れた状態にまで戻すこと。
そうしないと、せっかくエラーからの復帰処理を実装しても再度オブジェクトを参照することができなくなってしまう。
例外から回復可能な強固なクラスにするためには、トランザクションにおける「コミットとロールバック」のようなプランが必要だということ。
こういった仕組みは、コーディングの初期段階から留意しておかないと、後から追加するのは難しい。


第4章 性能
----

### 鉄則28 まずは設計、データ構造、アルゴリズムを吟味する

高速でコンパクトなコードを書くことに力を注ぐ前に、良質でしっかりとした設計を行うこと。
最大の性能改善は、言語に依存しないところにある。

### 鉄則29 コンパイル時のコード最適化に頼らない

多くの Java コンパイラはほとんど最適化を行わない。
コーディングフェイズでちゃんと効率の良いコードを書いておくことが重要。

### 鉄則30 実行時コード最適化を理解する

JIT は実行時に最適化のためのコストがかかることを理解すること。
JIT による実行時最適化よりも、コンパイルの時点で最適化されていることが望ましい。

### 鉄則31 連結には String よりも StringBuffer を使う

文字列を動的に連結するときは、immutable な `String` を使うのではなく、効率の良い `StringBuilder` や `StringBuffer` を使う。

- `StringBuilder` ... 動的な文字列の連結に使用する（スレッドセーフではないが `StringBuffer` より高速）
- `StringBuffer` ... 動的な文字列の連結に使用する（スレッドセーフ）

### 鉄則32 オブジェクト作成のコストを最少にする

下記のような実装をしているとオブジェクトの作成に時間がかかるので、性能改善の余地がある。

- コンストラクタに大量のコードがある場合
- コンストラクタで初期化されるオブジェクトが大量、あるいは巨大な場合
- 継承の階層が深い場合

### 鉄則33 未使用オブジェクトを作成しない

インスタンスの生成コストは大きいので、インスタンスは必要になった時点で作る。
条件分岐などの結果、作成したインスタンスがまったく参照されなかったという状況は防ぐこと。

### 鉄則34 同期化は必要最小限にとどめる

下記のコードは等価ではなく、`top1()` の方が高速。
`top2()` の方は、メソッド本文で `synchronized` を使っているので、内部的には例外を処理するコードが追加されるだけでなく、monitorenter 命令や monitorexit 命令が生成されてしまう。

```java
// 速い
public synchronized int top1() {
    return intArr[0];
}

// 遅い
public synchronized int top2() {
    synchronized (this) {
        return intArr[0];
    }
}
```

同期化の必要のないコードで、同期化されたライブラリを使用しない。
同等の機能を持つ非同期化クラスを使用する（`Vector` の代わりに `ArrayList` を使うなど）か、非同期化バージョンのサブクラスを作成する（Java 2 SDK 以前は `Vector` を継承して非同期化バージョンを作ったりしていたが、この方法はナンセンス）。

### 鉄則35 可能なときはスタック変数を使う

JVM はスタックベースのマシンであり、スタックデータへのアクセスが効率的になるよう設計されている。
局所変数はすべて局所変数テーブルに格納され、Java オペランドスタック上で操作されるため、効率的にアクセスできる。
ループ内でインスタンス変数に繰り返しアクセスするようなコードは、一旦スタック変数上にコピーしてから操作すると高速に動作するようになる。

### 鉄則36 static、final、private 型のメソッドを使ってインライン化を可能にする

インライン化の候補にしたいメソッドは、`static`、`final`、`private` のいずれかでなければならない（かつ単純なコードであること）。
典型的なのは次のような getter で、JIT コンパイラはこれらのメソッドをインライン化してくれる可能性が高い。

```java
public final int length() {
    return len;
}
```

インライン化のチャンスは、コンパイル時（コンパイラ）と実行時（JIT コンパイラ）の両方にある。
コンパイラで行う場合は、元のメソッドが変更されたときに参照する側のクラスで再度インライン化しなければいけない（再コンパイル）。
一方で、JIT コンパイラは実行時にメソッドをインライン化するので、元のメソッドが変更されても再コンパイルの必要はない。

### 鉄則37 インスタンス変数は一度だけ初期化する

下記のようにインスタンス変数を初期化するコードを書くと、インスタンス変数のデフォルト値が割り当てられた後に、同じ値で初期化されることになるので非効率。

```java
class Foo {
    private int count = 0;
    private boolean done = false;
    // ...
}
```

よって、次のように初期化用の代入 (`=`) を省略するのがよい。

```java
class Foo {
    private int count;  // デフォルト値 0
    private boolean done;  // デフォルト値 false
    // ...
}
```

ただし、局所変数はデフォルト値が割り当てられないので、必ず初期化する必要がある。
デフォルト値を持つのはインスタンス変数と static 変数だけである。

### 鉄則38 高速で小さいコードを作るにはプリミティブ型を使う

`Integer` よりも `int` を使う。
ただしコレクションで扱う場合はプリミティブラッパーが必要。

### 鉄則39 Vector を探索する際に Enumeration や Iterator を使わない

`hasNext()` を使ったループよりも、`int` のカウンタを使ったループの方が速い。
ループ処理ごとに `hasNext()` のメソッド呼び出しのオーバーヘッドがあるから。

### 鉄則40 配列のコピーには System.arraycopy を使う

ネイティブ実装されている `System.arraycopy` を使うと、配列のコピーが高速に行える。

### 鉄則41 Vector や ArrayList よりも配列を使うこと

`Vector`（同期化されたコレクション）も、`ArrayList`（同期化されていないコレクション）も、性能が重視されるコードでは使わない。
データ数が明確ではない場合でも、最大サイズを見込んだ配列を使った方がよいことが多々ある。

### 鉄則42 可能な時は必ずオブジェクトを再利用する

for ループの中で `new` しているコードがあったら、外に出して同じインスタンスを使いまわせないか考えてみる。
場合によっては、再初期化用のメソッドを用意してでも使い回せるようにする価値がある。

### 鉄則43 遅延評価を利用する

フィールドが多いオブジェクトの作成時間が問題になっているときは、コンストラクタですべてのフィールドを初期化せずに `null` のままにしておく。
そのフィールドが必要になったタイミングで初期化すればよい。

### 鉄則44 手動でソースコードを最適化する

コンパイラの最適化に頼らなくても、簡単にコードレベルで最適化できるものがある。
そのようなものはコードレベルで修正しておくこと。

- 空メソッド除去
- 不要コードの除去
- 強度の削減（`a[i] = a[i] + x` は複合代入演算子を使って `a[i] += x` と書く）
- 定数の折りたたみ（定数は `static final` で）
- 共通部分式の除去（`a + b` という同じ計算を何度もやっているなど）
- ループの展開
- 代数処理の単純化（計算式のレベルで最適化するということ）
- ループ内の不変コードの移動

### 鉄則45 ネイティブコードにコンパイルする

Java コードをネイティブコードにコンパイルすると実行速度は向上するが、プラットフォーム可搬性が失われてしまう。
両者の利点を生かす方法として、特定の Java メソッドだけを選択してネイティブコードにコンパイルするという手法がある。
ネイティブ化された部分には JNI を通じてアクセスできる。


第5章 マルチスレッド
----

### 鉄則46 インスタンスメソッドの場合 synchronized はメソッドやコードではなくオブジェクトをロックすることを理解する

下記の `foo` メソッドと `bar` メソッドは共に `this` に関して同期化される。

```java
class Test {
    public syncronized void foo() {
        // ...
    }

    public void bar() {
        synchronized (this) {
            // ...
        }
    }
}
```

つまり、同じインスタンスを使って、`foo()` と `bar()` を複数スレッドから同時に呼び出すことはできない。
ただし、別のインスタンスであれば、複数スレッドから同時に呼び出すことができる。

### 鉄則47 同期化 static メソッドと同期化インスタンスメソッドをはっきり区別する

下記の `foo` メソッドのように、`static` メソッドに `synchronized` 修飾子を付けると、そのクラスの `Class` オブジェクトに関連づけられたロックが取得される（クラスロック）。
`bar` メソッドのように、同期化ブロックでリテラルクラス (`Test.class`) を指定した場合も同様である。

```java
class Test {
    public synchronized static void foo() {
        // ...
    }

    public void bar() {
        synchronized (Test.class) {
            // ...
        }
    }
}
```

鉄則46 の `this` に関連づけられたロックとは異なり、クラス全体に対するロックを取得していることを理解すること。

ちなみに、`synchronized (lock)` のパラメーター部分 (`lock`) に渡すのは、オブジェクトよりも、サイズ 0 のバイト配列を使うのが効率がよい。

```java
Object lock = new Object();   // コスト大
byte[] lock = new byte[0];    // コスト小
```

### 鉄則48 public データや protected データの代わりに、private データをアクセサメソッドと共に使用する

フィールドへのアクセスを排他してスレッドセーフにするには、そのデータを間接的にも変更できないように注意する必要がある。
例えば、次の `data` フィールドは `private` であるが、`public` なアクセサメソッドを通じて変更できてしまうので、このクラスはスレッドセーフではない。

```java
class Test {
    private int[] data = new int[10];

    public int[] getData() {
        return data;  // 参照を返すのでスレッドセーフではない
    }

    // ...
}
```

どうしても外部から `data` を参照したいのであれば、次のようにコピーを返すようにする。

```java
class Test {
    private int[] data = new int[10];

    public int[] getData() {
        return data.clone();  // クローンを返せばスレッドセーフになる
    }

    // ...
}
```

### 鉄則49 不要な同期化を避ける

互いに排他しなくて良いメソッドに `synchronized` を付けたりしない。
排他すべきコード単位でロック取得用のオブジェクトを用意する。

```java
private byte[] lock1 = new byte[0];
private byte[] lock2 = new byte[0];

public void foo() {
    synchronized (lock1) {
        // ...
    }
}

public void bar() {
    synchronized (lock1) {
        // ...
    }
}

public void baz() {
    synchronized (lock2) {
        // ...
    }
}
```

### 鉄則50 共有変数へのアクセスには synchronized または volatile を使う

アトミック演算は自動的にスレッドセーフな演算になるわけではない。

```java
class RealTimeClock {
    private long time;

    public void setTime(long time) {
        this.time = time;  // スレッドセーフではない
    }

    public long getTime() {
        return time;  // スレッドセーフではない
    }
}
```

**それぞれのスレッドは独自の CPU キャッシュを持って動く可能性があり**、あるスレッドが `setTime()` メソッドで `time` フィールドを書き換えても、別のスレッドが `getTime()` メソッドで読み出したときに、最新の値が返されるとは限らない。
さらに、`long` 型や `double` 型のフィールドは、JVM の仕様上、アトミックに読み書きされるとは限らない。

この問題を解決するには、`synchronized` を使って `time` フィールドへのアクセスを排他するか、**`volatile`** 修飾子を付けて `time` フィールドを宣言する。

```java
private volatile long time;
```

- 並行処理を重視し、メモリ更新が少ない場合は `volatile` の方が速いことが多い。
- メモリ更新が多い場合は、`synchronized` の方が速いことが多い。

### 鉄則51 1つの演算に関わるすべてのオブジェクトをロックする

下記のメソッドには `synchronnized` が付けられているが、操作される配列オブジェクト (`a1`, `a2`) が同期化されていないのでスレッドセーフではない。

```java
public synchronized int sumArrays(int[] a1, int[] a2) {
    // ...
}
```

このコードを正しく動作させるには、次のように、操作されるすべてのオブジェクトをロックする必要がある。

```java
public int sumArrays(int[] a1, int[] a2) {
    synchronized (a1) {
        synchronized (a2) {
            // ...
        }
    }
}
```

ただし、このコードはデッドロックの危険性があるので、鉄則 52 に従ってロックを取得する順序を決めておくこと。

### 鉄則52 デッドロックを回避するために、ロックを取得する際の広域的な固定順序を決める

鉄則 51 のコードは、次のような 2 つのスレッドが同時に実行されるとデッドロックになる可能性がある。

```java
// スレッド 1
sumArrays(a1, a2);  // a1 ロック → a2 ロック

// スレッド 2
sumArrays(a2, a1);  // a2 ロック → a1 ロック
```

このようなデッドロックを回避するには、ロックを取得する順序を決めておくこと。
あるいは、オブジェクトごとにロック順序を決めるロジックを追加する。
ロック順序を自動化するロジックを入れるとコードが複雑になるが、実際にデッドロックが発生していたのであれば導入する価値がある。

（まく補足）書籍には、オブジェクトごとに `static` な `lock_order` 値を割り振っていくコードが紹介されているが、オブジェクトのハッシュコードを使う方法もある。

### 鉄則53 notify よりも notifyAll を使う

Java ではスレッド間のイベント通知に `wait()`、`notify()`、`notifyAll()` メソッドを使う。

- `wait()` ... スレッドを待機状態にする。
- `notify()` ... 待機状態のスレッドのうち 1 つを起こす。
- `notifyAll()` ... 待機状態のスレッドをすべて起こす。

**`notify()` はどのスレッドを起こすかを指定できない** ので、これが問題になる場合は必ず `notifyAll()` を使うこと。
`notifyAll()` は待機中のスレッドをすべて目覚めさせるが、**通知を受けるスレッドの順序は制御できない** ことに注意。
通知順序を制御したいときは「**特定通知パターン**」という手法を使う。

（まく補足）上記の 3 メソッドは、`synchronized` ブロックまたは `synchronized` メソッドの中からしか呼び出せない。
それ以外の場所で呼び出すと `IllegalMonitorStateException` がスローされる。

### 鉄則54 wait と notifyAll にはスピンロックを使う

スレッドの動作をある条件で待機させる場合は、**スピンロック** という手法を使う。
コードとしては、次のように `synchronized` ブロックの中に条件チェックのための `while` ループを配置し、その中で `wait()` を呼び出すようにする。

{{< code lang="java" hl_lines="7-11" >}}
// private static byte[] commands;

public void run() {
    // データを延々と処理するループ
    while (true) {
        synchronized (lock) {
            while (commands == null) {
                try {
                    lock.wait();
                } catch (InterruptedException e) { /* 意図的な例外無視 */ }
            }

            // ロックを取得した時点で commands が null でないことが保証される
            int len = commands.length;
            for (int i = 0; i < len; i++) {
                processCommand(commands[i]);
            }
            commands = null;
        }
    }
}
{{< /code >}}

この例では、他のスレッドから `commands` フィールドに値が格納され、`notifyAll()` により `wait()` の待機状態から抜けることを期待している。
この `wait()` 呼び出し部分を `while (commands == null)` のループで囲んでおかないと、ロックを取得した直後の `commands.length` の部分でぬるぽになる可能性がある（別のスレッドが先に通知を受けて、`commands = null;` を実行しているかもしれない）。

### 鉄則55 ポーリングループではなく wait と notifyAll を使う

スレッド間の通信を行うためにポーリング・ループでデータの有無をチェック（`while` ループの中で `sleep()` とデータチェックの繰り返し）していると、プロセッササイクルを消費するので効率が悪い。
鉄則 54 で述べたようにスピンロックを使えばよい。

```java
class ReadFromPipe extends Thread {
    private Pipe pipe;

    public void run() {
        while (true) {
            synchronized (pipe) {
                while (!pipe.hasData()) {
                    try {
                        pipe.wait();
                    } catch (InterruptedException e) { /* 意図的な例外無視 */ }
                }
                // ここでデータを処理
                // int data = pipe.readData();
            }
        }
    }
}
```

同期化にはコストがかかるが、ポーリング・ループと比べたらはるかに効率的である。

### 鉄則56 ロックされているオブジェクトの参照を再割り当てしない

```java
// private int[] intArr = new int[stackSize];

public void push(int val) {
    synchronized (intArr) {
        // ... intArr を使う ...
    }
}

public int pop() {
    synchronized (intArr) {
        // ... intArr を使う ...
    }
}
```

このコードは、`intArr` 配列オブジェクトを同期化するために `intArr` 自体のロックを取得している。
この同期化コードの中で `intArr` を再割り当て (`new int[]`) してしまうと、他のスレッドがすでに `synchronized (intArr)` でブロックしていた場合に、別の `intArr` オブジェクトでロックしようとしてしまう。
結果的に、複数の `synchronized` ブロックが同時に実行されてしまうことになり、スレッドセーフではなくなる。
`intArr` を再割り当てする必要がある場合は、メソッド自体を `synchronized` で同期化するか、別のロック用オブジェクトを使えばよい。

### 鉄則57 stop や suspend メソッドを呼び出さない

スレッドを停止するために `stop()` や `suspend()` を使ってはいけない。
`stop()` はスレッドを急に止めるためデータの整合性が取れなくなる可能性があり、`suspend()` はロックを保持したままスレッドを停止するのでデッドロックの原因になる。
代替として、スレッド自身が終了条件を見て処理を終わらせるようにする（→ 鉄則58）。

### 鉄則58 スレッドは協調動作を通じて終了させる

下記は外からスレッドを安全に終了させるための仕組みを導入したクラスの実装例。

```java
class Example extends Thread {
    private volatile boolean stop;
    public void stopThread() { stop = true; }
    public void run() {
        while (!stop) {
            // スレッドのメイン処理
        }
        // クリーンアップ処理
    }
}
```

通常のフィールドへのアクセス方法だけでは、あるスレッドによる書き込みが別のスレッドからいつ見えるかは保証されない。
そのため、スレッド間で共有する `stop` フィールドには **`volatile` 修飾子** を付けて、スレッド間で値の変更が即座に反映されるようにする。
あるいは、`synchronized` ブロックの中で `stop` フィールドの値を参照するようにしてもよい。


第6章 クラスとインタフェース
----

### 鉄則59 多重継承をサポートするにはインタフェースを使う

Java はクラスの多重継承をサポートしていないが、インタフェースの多重継承は可能。

### 鉄則60 インタフェースにおけるメソッドの衝突を回避する

インタフェースの多重継承時には、メソッド名の衝突という潜在的な問題が存在する。
これは、サードパーティ制のインタフェースを実装するときに起こりやすい。

### 鉄則61 部分実装を提供するのが適切な場合は抽象クラスを使う

クラスの実装の一部だけを提供したいときは抽象クラスを使う。
実装を持たないメソッドはすべて抽象メソッドであり、派生クラスで実装する必要がある。

### 鉄則62 インタフェース、抽象クラス、具象クラスを区別する

インタフェースは実装を伴わない「契約の表現」であり、抽象クラスは実装を伴う「契約の表現」である。

### 鉄則63 不変クラスの定義と実装は慎重に

不変オブジェクトは状態が変化しないので初めからスレッドセーフが保証されており、同期化の必要がない。
Java には不変性を指定するキーワードがないため、次のように明示的にコーディングする必要がある。

- クラスを `final` 宣言する
- クラス内のデータをすべて `private` 宣言する
- セッターメソッドを提供しない
- クラスに含まれるデータはすべてコンストラクタで設定する
- ゲッターメソッドで内部の可変オブジェクトを返すときは、クローンしてから返す（→鉄則64）

### 鉄則64 不変オブジェクトが可変オブジェクトへのオブジェクト参照をやりとりするときは、クローンを使用する

不変オブジェクトのメソッドで可変オブジェクトの参照を返す場合は、クローンしてから返すようにする。
そのオブジェクトの `clone()` メソッドで正しく複製できるようであればそれを使えばよい。
そうでなければ、ヘルパーメソッドなどを作って深いクローニングを行うようにする。

### 鉄則65 不変クラスの定義には継承または委譲を使用する

可変クラス (mutable class) と不変クラス (immutable class) の 2 種類を提供したい場合は、共通インタフェースを使う方法と、不変委譲クラスを作る方法がある。

#### 共通インタフェースを使う方法

```java
/** 不変メソッドを定義する共通インターフェース */
public interface User {
    /**
     * ユーザー名を取得する。
     */
    String getName();
}

/** 不変クラス */
public final class ImmutableUser implements User {
    private final String name;

    public ImmutableUser(String name) {
        this.name = name;
    }

    @Override
    public String getName() {
        return name;
    }
}

/** 可変クラス */
public class MutableUser implements User {
    private String name;

    public MutableUser(String name) {
        this.name = name;
    }

    @Override
    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}
```

共通インタフェースを使う方法は、2 つのクラスで同様の実装を行う必要があるため実装量が増える傾向がある。

#### 不変委譲クラスを作る方法

```java
/** 可変クラス */
public class MutableUser {
    private String name;

    public MutableUser(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}

/** 不変クラス */
public final class ImmutableUser {
    private final MutableUser delegate;

    public ImmutableUser(String name) {
        this.delegate = new MutableUser(name);
    }

    public String getName() {
        return delegate.getName();
    }
}
```

不変委譲クラスを作る方法は、既存の可変クラスのソースを変更できない場合にも使用できる。
ただし、常に内部で委譲処理が発生するため、性能上のペナルティがある。

### 鉄則66 clone メソッドを実装する時は super.clone を呼び出す

あるクラスで `clone()` をサポートしていることを示すには `implements Cloneable` を宣言する。
`Cloneable` はマーカーインタフェースであり、`clone()` メソッドを宣言していないので、`Object` に定義された protected な `clone()` メソッドを public メソッドとしてオーバーライドする。
`clone()` の実装では、**`super.clone()`** を呼び出すことで、`java.lang.Object.clone()` を間接的に呼び出し、オブジェクトのフィールドが正しくコピーされるようにする。

{{< code lang="java" hl_lines="6" >}}
public class House implements Cloneable {
    // ...
    @Override
    public Object clone() {
        try {
            return super.clone();
        } catch (CloneNotSupportedException e) {
            // Cloneable を実装しているのでこの例外は発生しない
            throw new InternalError();
        }
    }
}
{{< /code >}}

ただし、`Object.clone()` は浅いコピーを行うので、参照型フィールドを持つクラスでは、自分で深いコピーを実装する必要がある。

{{< code lang="java" title="深いコピーの例" >}}
public class House implements Cloneable {
    private Room room;

    @Override
    public Object clone() {
        try {
            House cloned = (House) super.clone();
            // Room クラスも Cloneable を実装している必要がある
            cloned.room = (Room) room.clone();
            return cloned;
        } catch (CloneNotSupportedException e) {
            throw new InternalError();
        }
    }
}
{{< /code >}}


### 鉄則67 非メモリリソースのクリーンアップを finalize メソッドに頼らないこと

非メモリリソース（ファイルハンドル、ソケット、データベース接続など）のクリーンナップを必要とするクラスでは、必ずリソースを解放するための public メソッドを提供し、そのクラスの `finalize` メソッドからも呼び出されるようにしておく。
プログラム自体が終了する前に `finalize` メソッドが実行されるということは保証されていない。

```java
public class Communication {
    private ServerSocket socket;

    public synchronized void cleanup() throws IOException {
        if (socket != null) {
            socket.close();
            socket = null;
        }
    }

    @Override
    protected void finalize() throws Throwable {
        try {
            cleanup();
        } finally {
            super.finalize();
        }
    }
}
```

注記: 現在は **`AutoCloseable`** を使って次のように try-with-resources の構文を使うことが推奨されている。

```java
import java.io.IOException;
import java.net.ServerSocket;

public final class Communication implements AutoCloseable {
    private ServerSocket socket;

    public Communication(int port) throws IOException {
        this.socket = new ServerSocket(port);
    }

    @Override
    public synchronized void close() throws IOException {
        if (socket != null) {
            socket.close();
            socket = null;
        }
    }
}


public class Main {
    public static void main(String[] args) throws Exception {
        try (Communication communication = new Communication(8080)) {
            // ...
        }
        // ここで close() が自動的に呼び出される
    }
}
```

### 鉄則68 非 final メソッドをコンストラクタから呼び出すときは注意を怠らない

コンストラクタ内で非 final メソッドを呼び出すと、サブクラスでのオーバーライドによって不可解な動作を引き起こす可能性がある。

```java
public class Base {
    public Base() {
        // サブクラスでオーバーライドされたメソッドが呼び出される
        doSomething();
    }

    public void doSomething() {
        System.out.println("Base");
    }
}

public class Derived extends Base {
    private String name;

    public Derived(String name) {
        this.name = name;
    }

    @Override
    public void doSomething() {
        // コンストラクタがまだ実行されていないので、name は null
        System.out.println("Derived: " + name);
    }
}

public class Main {
    public static void main(String[] args) {
        new Derived("Alice");
    }
}
```

{{< code title="実行結果" >}}
Derived: null
{{< /code >}}

`Derived` のインスタンスを生成すると次のような順序で処理が行われる。

1. `Derived` のインスタンス領域が確保され、各フィールドにデフォルト値が設定される（`String` 型の `name` には `null` が設定される）。
1. `Derived` のコンストラクタから暗黙的に `super()` が呼び出され、`Base` のコンストラクタが実行される。
1. `Base` のコンストラクタ内で `doSomething()` が呼び出され、`Derived.doSomething()` が実行される。
1. この時点では `Derived` のコンストラクタ本体がまだ実行されていないため、**`name` は `null` のままである**。
1. `Base` のコンストラクタが終了した後、`Derived` のコンストラクタ本体が実行され、`name` に `"Alice"` が代入される。

このように、コンストラクタからオーバーライド可能なインスタンスメソッドを呼び出すと、サブクラスの初期化が完了する前にサブクラスのメソッドが実行される可能性がある。
そのため、コンストラクタからは、原則として private または final のメソッドだけを呼び出し、サブクラスでオーバーライド可能なメソッドの呼び出しは避けるべきである。

