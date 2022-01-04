---
title: "数値計算系のプログラムメモ"
url: "/p/m3p8hh6"
date: "2001-10-20"
tags: ["プログラム"]
---

n が 2 のべき乗か判定する
----

{{< code lang="cpp" >}}
long powerof2(long n) {
    return (n > 0) && ((n & (n - 1)) == 0);
}
{{< /code >}}

