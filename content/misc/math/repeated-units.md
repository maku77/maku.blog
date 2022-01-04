---
title: "数学メモ: 任意の桁のレプ・ユニット (repeated units) を求める方法"
url: "/p/29awadj"
date: "2013-05-26"
tags: ["数学", "Python"]
use_math: true
---

**レプ・ユニット (repeated units)** というのは、1 の連続した数字を表します（例: 1, 11, 111, 1111）。

n 桁のレプ・ユニットは、以下のような計算で求められます。

$$R_{n} = \frac{10^n - 1}{9}$$

下記は Python でこの計算をテストする例です。

{{< code lang="python" title="repeated_units.py" >}}
def repeated_units(n):
  return (10 ** n - 1) // 9

for i in range(1, 10):
  print(repeated_units(i))
{{< /code >}}

{{< code title="実行結果" >}}
1
11
111
1111
11111
111111
1111111
11111111
111111111
{{< /code >}}

もちろん、Python のプログラムとしては、単純に繰り返し文字列を作成してから、それを数値に変換することもできます。

{{< code lang="python" >}}
def repeated_units(n):
    return int('1' * n)
{{< /code >}}

