---
title: "数学メモ: 最大公約数 (GCD) と最小公倍数 (LCM)"
url: "p/fvvzur8/"
date: "2012-03-02"
tags: ["数学"]
---

最小公倍数を求める
----

__ユークリッドの互除法__ により、最大公約数 (GCD: Greatest Common Divisor) を求めます。
正の整数 a, b (a &gt; b) の最大公約数 `GCD(a, b)` を求めたいとします。

まず、a が b で割り切れれば、明らかに b が最大公約数になりますので、答えは b です。
問題は割り切れない場合です。
a, b が最大公約数 d を持つとすると、

```
a = md
b = nd
```

と書けます。すると、

```
a-b = (m-n)d
```

となるので、a-b も最大公約数 d を持つことが分かります。
言い換えると、d の倍数（この場合は a）から d の倍数（この場合は b）を引いても、残りは d の倍数であるということです。

```
GCD(a, b) = GCD(a-b, b)  ただし、a > b
```

依然として a-b &gt; b のときは、同様に左側の値から b を引き続けて、

```
GCD(a, b) = GCD(a-b, b)
          = GCD(a-2b, b)
          = GCD(a-3b, b)
          = ...
          = GCD(a%b, b)   // a%b は a を b で割った時の剰余
```

結局は、最大公約数は b 以上になることはないので、最後の GCD(a%b, b) を求めるのが一番楽というところに落ち着きます。

簡潔に書くために、a%b = r とおくと、

```
GCD(a, b) = GCD(r, b)
          = GCD(b, r)   (b > r なので分かりやすく左に大きい数値をも持ってきた)
```

この時点で、最初の問題 GCD(a, b) は、より小さな問題 GCD(b, r) に変換できました！！

あとはこの変換を繰り返すだけです。

b が r で割り切れるのであれば、r が最大公約数になります。
まだ割り切れないのであれば、再び GCD(a, b)=GCD(b, a%b) の変換を適用して問題を小さくしていきます。

2 つの数が互いに素（最大公約数が 1）である場合は、最終的に剰余が 1 になった時点で割り切れることになります。1 で割れば余りは必ず 0 になるので、最大公約数が 1 であるという答えが出ます。


最小公倍数 (LCM) を求める
----

最大公約数 (GCD) を d とすると、a, b の最小公倍数 (LCM) は以下のように求められます。

```
LCM(a, b) = d * (a/d) * (b/d)
```

これは、以下のように説明できます。

例えば、30 と 105 の GCD, LCM を求めるとして、それぞれを因数分解すれば以下のように表現できます。

```
30  = 2 * 3 * 5
105 =     3 * 5 * 7
----------------------------
          3 * 5     が最大公約数 (d)
      2 * 3 * 5 * 7 が最小公倍数
```

__最大公約数は因数の AND を取った部分__ なので、3*5 となり、__最小公倍数は因数の OR を取った部分__ なので、2*3*5*7 となります。
最小公倍数を構成している因数のうち、共通部分は d、30 の因数にしか含まれていないものは 30/d、105 の因数にしか含まれていないものは 105/d で求められます。結局、最小公倍数 (LCM) は以下のように求められます。

```
LCM(30, 105) = d * 30/d * 105/d
```


Python と Ruby で実装してみる
----

### Python で最大公約数 (gcd) を求める

{{< code lang="python" title="再帰版" >}}
def gcd(a, b):
    r = a % b
    if r == 0: return b
    return gcd(b, r)
{{< /code >}}

{{< code lang="python" title="非再帰版" >}}
def gcd(a, b):
    while b:
        a, b = b, a % b
    return a
{{< /code >}}

{{< code lang="python" title="標準ライブラリ (fractions.gcd) を使用する場合" >}}
from fractions import gcd
{{< /code >}}

{{< code lang="python" title="テスト" >}}
print(gcd(3, 7))     #=> 1
print(gcd(12, 20))   #=> 4
print(gcd(30, 105))  #=> 15
{{< /code >}}

### Python で最小公倍数 (lcm) を求める

{{< code lang="python" >}}
def lcm(a, b):
    return a * b // gcd(a, b)
{{< /code >}}

### Python で 3 値以上の gcd、lcm を求める

Python の `functools.reduce` を使って以下のように求められます。

{{< code lang="python" >}}
from functools import reduce
d = reduce(gcd, (12, 20, 32))    #=> 4
{{< /code >}}

内部的には次のような処理が行われます。

{{< code lang="python" >}}
data = (12, 20, 32)
d = data[0]
for i in data[1:]:
    d = gcd(d, i)
{{< /code >}}


### Ruby で最大公約数 (gcd) を求める

{{< code lang="ruby" title="再帰版" >}}
def gcd(a, b)
  a,b = b,a if a < b  # これはなくても一回再帰が増えるだけ
  r = a % b
  return b if r == 0
  gcd(b, r)
end
{{< /code >}}

{{< code lang="ruby" title="非再帰版" >}}
def gcd(a, b)
  while not b == 0
    a, b = b, a % b
  end
  return a
end
{{< /code >}}

Ruby では 0 が真 (true) になるので、`while not b == 0` の部分は、`while b` とは書けません。

{{< code lang="ruby" title="テスト" >}}
puts gcd(3, 7)    #=> 1
puts gcd(18, 60)  #=> 6
{{< /code >}}

### Ruby で最小公倍数 (lcm) を求める

{{< code lang="ruby" >}}
def lcm(a, b)
  return a * b / gcd(a, b)
end

puts lcm(3, 7)    #=> 21
puts lcm(18, 60)  #=> 180
{{< /code >}}

