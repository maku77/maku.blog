---
title: "CppUnit の使い方メモ"
url: "p/nyj2hxe/"
date: "2008-08-19"
tags: ["C/C++", "ユニットテスト"]
---

CppUnit-x のダウンロード＆インストール
----

- Version:
  - CppUnit-x 20020331
  - Red Hat Linux 9
  - SourceForge.JP: Project Info - CppUnit-x
  - http://sourceforge.jp/projects/cppunit-x/

CppUnit-x は数多くある CppUnit の派生物の中でも移植性が高いといわれています。
2008-08-19 現在のバージョンは 20020331 です。
次のように CppUnit-x のアーカイブをダウンロードしてインストールします。

```console
$ wget http://iij.dl.sourceforge.jp/cppunit-x/8452/cppunit-x-20020331.tar.gz
$ tar xzvf cppunit-x-20020331.tar.gz
$ cd cppunit-x-20020331
$ ./configure
$ make
$ su
# make install
```

これにより、以下のようなライブラリやヘッダファイルがインストールされます。

- `/usr/local/lib/libcppunit.a`
- `/usr/local/include/cppunit/*.h`


はじめての CppUnit-x
----

テストが 1 つだけの場合は、`TestCase` のサブクラスで `runTest()` メソッドをオーバーライドしてテストコードを実装します。
`TestRunner` オブジェクトに `TestCase` オブジェクトを追加して `TestRunner::run()` でテストを実行できます。
`TestRunner::run()` を呼び出すと、`TestRunner` に登録された `TestCase` の `runTest()` が順に呼び出されます。
次の例のように、名前空間の使用宣言に、`USING_NAMESPACE_CPPUNIT` というマクロを使用すると、名前空間をサポートしていないコンパイラを使った場合にもそのままコンパイルできるようになります。

{{< code lang="cpp" title="main.cpp" >}}
#include <cppunit/TestCase.h>
#include <cppunit/TestRunner.h>

USING_NAMESPACE_CPPUNIT

class MyTest : public TestCase {
public:
    MyTest() : TestCase("MyTest") {}

    virtual void runTest() {
        TEST_ASSERT_EQUALS(2, 1 + 1);
        TEST_ASSERT(0 < 1);
    }
};

int main(int argc, char **argv) {
    TestRunner runner;
    runner.addTest(new MyTest());
    return runner.run(argc, argv);
}
{{< /code >}}

{{< code lang="console" title="ビルド＆実行" >}}
$ g++ -o main main.cpp -lcppunit
$ ./main
.

OK (1 tests)
{{< /code >}}


任意のテストメソッドを実行するようにする (TEST_CALLER、TestCaller)
----

オーバーライドした `TestCase::runTest()` ではなく、指定したテストメソッドを実行するには、`TEST_CALLER` マクロを使用します。

```cpp
class MathTest : public TestCase {
public:
    MathTest() : TestCase("MathTest") {}
    // runTest は実装せずに独自のテストメソッドを複数実装する
    void testAdd() { ... }
    void testSubtract() { ... }
    void testMultiply() { ... }
};

int main(int argc, char **argv) {
    TestRunner runner;
    runner.addTest(new TEST_CALLER(MathTest, testAdd));
    runner.addTest(new TEST_CALLER(MathTest, testSubtract));
    runner.addTest(new TEST_CALLER(MathTest, testMultiply));
    return runner.run(argc, argv);
}
```

上記のようにすると、`MathTest::runTest()` ではなく、独自に定義した `MathTest::testAdd()`、`MathTest::testSubtract()`、`MathTest::testMultiply()` が実行されるようになります。

### 解説

`TestRunner::run()` を実行すると、あらかじめ `TestRunner::addTest()` で登録しておいた `Test` オブジェクトの `run()` メソッドが順番に実行されていきます。
例えば、`Test` のサブクラスである `TestCase` クラスのインスタンスを登録しておくと、`TestRunner::run()` は `TestCase::run()` をひとつずつ呼び出していきます。

`TestCase::run()` は最終的に純粋仮想関数である `TestCase::runTest()` を呼び出す実装になっているので、ユーザはこの `TestCase::runTest()` をオーバーライドすることによってテストコードを記述することができます。
`TestSuite::run()` も、内部で `TestCase::run()` を呼び出しているだけなので、最終的に `TestCase::runTest()` が呼び出されることに違いはありません。

```
TestRunner::run() ==> TestCase::run() ==> TestCase::runTest() ※仮想関数
```

このように最終的に呼び出されるテストメソッドは `TestCase::runTest()` という名前に固定されているのですが、`TEST_CALLER` マクロ（`TestCaller` クラス）を使用することで間接的に任意のテストメソッドを呼び出せるようになっています。
`TEST_CALLER` マクロは `TestCaller.h` に次のように定義されています。

```cpp
#define TEST_CALLER(Fixture, method) \
    CPPUNIT::TestCaller<Fixture>(#Fixture "::" #method, &Fixture::method)
```

例えば、

```cpp
TEST_CALLER(MathTest, testAdd)
```

とすると、

```cpp
CPPUNIT::TestCaller<MathTest>("MathTest::testAdd", &MathTest::testAdd);
```

のように展開されます。つまり、ひとつのテストメソッドを実行するために、それ専用の `TestCaller` オブジェクトをひとつ作ることになります。

`TestCaller` クラスの定義では、テンプレートによって `Fixture` の位置に指定したクラスのサブクラス `CallerImpl` が自動的に定義されるようになっています。
上記の例の場合は、`MathTest` クラスのサブクラスが自動的に定義されます。
そして、`TestCaller` オブジェクトは、その `CallerImpl` オブジェクトをひとつ持ちます。

{{< code lang="cpp" title="TestCaller.h（一部抜粋）" >}}
template <class Fixture> class TestCaller : public Test {
    ...
    // コンストラクタ
    TestCaller(const char* name, TestMethod method) : m_impl(name, method) {}

    // これはつまり TestCase::run() を呼び出す
    virtual void run(TestResult* result) { m_impl.run(result); }

    // Fixture に指定したクラスのサブクラスが自動で定義される
    class CallerImpl : public Fixture {
        ...
    } m_impl;
};
{{< /code >}}

`CallerImpl::runTest()` の実装は、`TestCaller` で指定したメンバ関数を呼び出すようになっています。
つまり、ユーザが `TEST_CALLER` マクロで指定したテストメソッドが呼び出されます。

{{< code lang="cpp" title="TestCaller.h（一部抜粋）" >}}
class CallerImpl : public Fixture {
public:
    CallerImpl(const char* name, TestMethod method) : Fixture(name), m_method(method) {}
    virtual void runTest() { (this->*m_method)(); }
private:
    TestMethod m_method;
} m_impl;
{{< /code >}}

下記のようにテストメソッドが呼び出されます。

```
TestRunner::run()
    => TestCaller<MathTest>::run()
    => MathTest::run()
    => CallerImpl::runTest()
    => TEST_CALLER マクロに指定したメソッド（MathTest::testAdd など）
```


複数のテストメソッドを一度に実行する (TestSuite)
----

複数のテストメソッドを一度に実行したい場合は、`TestSuite` オブジェクトに複数の `TestCase` を追加し、これを `TestRunner` に追加して実行します。

以下の例では、テストクラスに `TestSuite` オブジェクトを作成するための static メソッド `suite()` を作成しています。
また、独自のテストメソッド（`testAdd` など）を実行するために、`TEST_CALLER` マクロでテストメソッドを指定しています。

{{< code lang="cpp" title="main.cpp" >}}
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TestSuite.h>

USING_NAMESPACE_CPPUNIT

class MathTest : public TestCase {
public:
    MathTest() : TestCase("MathTest") {}

    static TestSuite *suite() {
        TestSuite *suite = new TestSuite("MathTest");
        suite->addTest(new TEST_CALLER(MathTest, testAdd));
        suite->addTest(new TEST_CALLER(MathTest, testSubtract));
        suite->addTest(new TEST_CALLER(MathTest, testMultiply));
        return suite;
    }

    void testAdd() {
        TEST_ASSERT_EQUALS(3, 1 + 2);
        TEST_ASSERT_EQUALS(0, -1 + 1);
    }

    void testSubtract() {
        TEST_ASSERT_EQUALS(1, 2 - 1);
        TEST_ASSERT_EQUALS(-1, 0 - 1);
    }

    void testMultiply() {
        TEST_ASSERT_EQUALS(0, 10 * 0);
        TEST_ASSERT_EQUALS(-1, 1 * -1);
    }
};

int main(int argc, char **argv) {
    TestRunner runner;
    runner.addTest(MathTest::suite());
    return runner.run(argc, argv);
}
{{< /code >}}

{{< code lang="console" title="ビルド＆実行" >}}
$ g++ -o main main.cpp -lcppunit
$ ./main
.

OK (3 tests)
{{< /code >}}

