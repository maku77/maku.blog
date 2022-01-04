---
title: "C/C++サンプル: エンディアンの判定"
url: "/p/g9q6kyf"
date: "2001-05-11"
tags: ["C/C++"]
---

使用している PC のホストバイトオーダーが、ビッグエンディアンかリトルエンディアンかを調べるプログラムです。

{{< code lang="cpp" title="endian.cpp" >}}
#include <iostream>
using namespace std;

int main()
{
    union {
        char c[2];
        short s;
    } u;
    u.s = 0x0102;

    if (u.c[0] == 0x01 && u.c[1] == 0x02)
        cout << "Big-endian" << endl;
    else if (u.c[0] == 0x02 && u.c[1] == 0x01)
        cout << "Little-endian" << endl;
    else
        cout << "Unknown" << endl;

    return 0;
}
{{< /code >}}
