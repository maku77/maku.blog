---
title: "プロコンメモ: Dynamic Programming (1) メモ化による高速化 (memoization)"
url: "p/pxw2pe2/"
date: "2013-04-17"
tags: ["python", "プロコン"]
---

メモ化 (Memoization)
----

ある関数が同じ引数で 2 回以上呼ばれる場合は、最初に返した値を記憶しておくことで、高速に結果を返すことができるようになります。
この手法を **メモ化 (memoization)** といいます。

例えば、以下のようなフィボナッチ関数 (fibonacci) を考えます。

{{< code lang="python" title="Python" >}}
def fib(n):
    if n <= 2: return 1
    else: return fib(n - 2) + fib(n - 1)
{{< /code >}}

このような再帰による実装は、同じ引数での自分自身の呼び出しを何度も発生させてしまいます。
フィボナッチ数列の第 7 項を求めようとすると、以下のように指数関数的に呼び出し回数が増えていきます。

```
                  f(7)
                   |
         +---------+---------+
         |                   |
        f(5)       +        f(6)
         |                   |
    +----+----+         +----+----+
    |         |         |         |
   f(3)  +   f(4)      f(4)  +   f(5)
    |         |         |         |
  +-+-+     +-+-+     +-+-+     +-+-+
  |   |     |   |     |   |     |   |
f(1)+f(2) f(2)+f(3) f(2)+f(3) f(3)+f(4)
```

* `fib(1)` ... 呼び出し回数 1 回
* `fib(5)` ... 呼び出し回数 9 回
* `fib(10)` ... 呼び出し回数 109 回
* `fib(20)` ... 呼び出し回数 13529 回
* `fib(30)` ... 呼び出し回数 1664079 回

`fib(35)` を求めようとすると、6 秒以上かかります。

{{< code lang="python" title="sample.py" >}}
import time

# def fib(n): ...

if __name__ == '__main__':
    start = time.clock()
    print(fib(35))
    print('{:.4} seconds'.format(time.clock() - start))
{{< /code >}}

{{< code lang="console" title="実行結果" >}}
$ ./sample.py
9227465
6.215 seconds
{{< /code >}}

`fib` 関数をメモ化 (memoization) すると、実行時間を 1ms 以下に改善できます。

{{< code lang="python" title="sample.py" >}}
memo = {}

def fib(n):
    if n in memo: return memo[n]
    if n <= 2: memo[n] = 1
    else: memo[n] = fib(n - 2) + fib(n - 1)
    return memo[n]

# ...snipped...
{{< /code >}}

{{< code lang="console" title="実行結果" >}}
$ ./sample.py
9227465
0.000111 seconds
{{< /code >}}

グローバルに `memo` 変数を定義したくない場合は、Python の場合は以下のようにも書けます。

```python
def fibonacci(n):
    memo = {}

    def f(n):
        if n in memo: return memo[n]
        if n <= 2: memo[n] = 1
        else: memo[n] = f(n - 2) + f(n - 1)
        return memo[n]

    return f(n)
```


Memoizer クラス
----

以下のような汎用メモ化クラスを用意しておくのもよいでしょう。
この Memoizer クラスは、コンストラクタに渡された関数をメモ化された関数として呼べるようにします。

```python
class Memoizer:
    def __init__(self, func):
        self.func = func
        self.memo = {}
    def __call__(self, *args):
        if not args in self.memo:
            self.memo[args] = self.func(*args)
        return self.memo[args]

def fib(n):
    if n <= 2: return 1
    else: return fib(n - 2) + fib(n - 1)

fib = Memoizer(fib)
print(fib(35))
```


memoize デコレータ
----

関数をラッパするだけなら、`Memoizer` クラスではなく下記のような `memoize` 関数として作った方が手っ取り早いかもしれません。

```python
def memoize(func):
    memo = {}
    def f(*args):
        if not args in memo:
            memo[args] = func(*args)
        return memo[args]
    return f

def fib(n):
    if n <= 2: return 1
    else: return fib(n - 2) + fib(n - 1)

fib = memoize(fib)
print(fib(35))
```

Python では実は、上記のような `Memoizer` クラスや `memoize` 関数は、次のようにデコレータとして関数定義時に適用してしまうことができます。キター！

```python
# def memoize(func): ...

@memoize
def fib(n):
    if n <= 2: return 1
    else: return fib(n - 2) + fib(n - 1)
```

