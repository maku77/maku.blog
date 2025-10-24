---
title: "プロコンメモ: 2 つの数値を連結した数値を作る (123 + 456 → 123456)"
linkTitle: ""
url: "p/3bhhjxu/"
date: "2012-03-16"
tags: ["c/c++", "プロコン"]
---

たとえば、123 と 456 という数字を連結して、123456 という数字を作りたい場合、以下のように数値計算だけで求めるのが効率的です。
引数 left の値を、「right の桁数+1」分だけ 10 倍してやって、right と足し合わせているだけです。

{{< code lang="cpp" title="c++" >}}
/*
 * Concatnate numbers as they are strings.
 * Both two numbers must not be negative (i.e. 0 or larger).
 *
 * e.g.) concat_num(100, 200) => 100200
 */
int concat_num(int left, int right) {
    int r = 10;
    while (r <= right) r *= 10;
    return (left * r) + right;
}
{{< /code >}}

以下のように、連結文字列を生成して再度数値に戻す方法は、数値だけで計算するよりも数十倍時間がかかってしまいます。

```cpp
// NOT good (slower)
int concat_num_BAD(int a, int b) {
    char buf[128];
    snprintf(buf, sizeof(buf), "%d%d", a, b);
    return atoi(buf);
}
```

{{< code lang="cpp" title="テストコード" >}}
int main() {
    cout << concat_num(0, 0) << " = 0" << endl;
    cout << concat_num(1, 1) << " = 11" << endl;
    cout << concat_num(123, 0) << " = 1230" << endl;
    cout << concat_num(0, 123) << " = 123" << endl;
    cout << concat_num(300, 100) << " = 300100" << endl;
    cout << concat_num(777, 999) << " = 777999" << endl;
}
{{< /code >}}

