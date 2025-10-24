---
title: "プロコンメモ: f(x) <= a を満たす最大の整数 x を求める"
url: "p/kdxkan4/"
date: "2013-04-27"
tags: ["プロコン"]
useMath: true
---

例えば、\\( f(x) = x^3 + 2x^2 + 500000 \le 10^7 \\) を満たす最大の整数 \\( x \\) を求めるには、以下のように二分探索で探すと高速に求められます。
二分探索は、大小関係をもとに絞り込みを行うため、\\( f(x) \\) は単調増加でなければいけません。

```python
#!/usr/bin/env python
def max_x(left, right, func, expect):
    while right - left > 1:
        mid = (left + right) // 2
        if func(mid) <= expect:
            left = mid
        else:
            right = mid
    return left

def f(x):
    return x**3 + 2 * x**2 + 500000

EXPECTATION = 10**7

x = max_x(0, 100000, f, EXPECTATION)

print('f({}) = {} < {} < {}'.format(x, f(x), EXPECTATION, f(x+1)))
```

{{< code title="実行結果" >}}
f(211) = 9982973 < 10000000 < 10118016
{{< /code >}}

上記の例は、求められる `x` が少なくとも **`[left, right)`** の範囲にあることを前提としています。
つまり、あらかじめ `x` が高々 `right` より小さいと分かっていることが前提です。
多くの場合は、`right` の初期値を十分に大きな値に設定すれば大丈夫です。

`left`、`right` の初期値を自動で設定したい場合は、以下のように適当に範囲を設定してしまう方法があります。
ここでは、\\( f(2^k) \le EXPECTATION \lt f(2^{k+1}) \\) となるような `k` を求めて、`left`、`right` の初期値を \\( 2^k \\) と \\( 2^{k+1} \\) に設定しています。

```python
#!/usr/bin/env python
def init_left_right(func, expect):
    right = 2
    while func(right) < expect:
        right *= 2
    return (right // 2, right)

def max_x(func, expect):
    left, right = init_left_right(func, expect)
    while right - left > 1:
        mid = (left + right) // 2
        if func(mid) <= expect:
            left = mid
        else:
            right = mid
    return left

def f(x):
    return x**3 + 2 * x**2 + 500000

EXPECTATION = 10**7

x = max_x(f, EXPECTATION)

print('f({}) = {} < {} < {}'.format(x, f(x), EXPECTATION, f(x+1)))
```

