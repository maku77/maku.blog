---
title: "JUnit のテストケースを一時的に無効にする（@Ignore/@Disabledアノテーション）"
url: "/p/ow3zskd"
date: "2020-09-30"
tags: ["ユニットテスト"]
---

JUnit で特定のテストケース（クラスや関数）を一時的に無効にしておきたい場合は、次のようなアノテーションをクラスや関数に付けます。

- JUnit4 の場合: [@Ignore アノテーション (org.junit.Ignore)](https://junit.org/junit4/javadoc/latest/org/junit/Ignore.html)
- JUnit5 の場合: [@Disabled アノテーション (org.junit.jupiter.api.Disabled)](https://junit.org/junit5/docs/current/api/org.junit.jupiter.api/org/junit/jupiter/api/Disabled.html)

{{< code lang="java" title="Java の場合" >}}
// import org.junit.Ignore;
// import org.junit.Test;

public class MyClassTest {
    @Test @Ignore public void testSomething() {
        // ...
    }
}
{{< /code >}}

{{< code lang="kotlin" title="Kotlin の場合" >}}
// import org.junit.Ignore
// import org.junit.Test

class MyClassTest {
    @Test @Ignore fun testSomething() {
        // ...
    }
}
{{< /code >}}

`@Ignore` (`@Disabled`) に文字列パラメータを渡すと、なぜそのテストを無効にしているのかを示すことができます。

{{< code lang="kotlin" >}}
@Ignore("HogeHogeのパラメータを整理中")
@Test
fun testSomething() {
    // ...
}
{{< /code >}}

このメッセージは JUnit でテストを実行したときに表示されます。
下記は、Android Studio 上で JUnit によるテストを実行したときの表示例です。
16 個のテスト関数のうち 1 つが無視され、その理由が表示されています。

{{< image src="img-001.png" >}}

