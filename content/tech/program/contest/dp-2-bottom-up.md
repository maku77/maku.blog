---
title: "プロコンメモ: Dynamic Programming (2) Top-down DP から Bottom-up DP へ"
url: "p/gycxxxm/"
date: "2013-04-21"
tags: ["python", "プロコン"]
---

DP では、問題を解くアプローチとして、大きく分けて

* Top-down approach
* Bottom-up approach

の 2 つのアプローチがあります。


Top-down DP の考え方
----

Top-down DP の方は、n 番目の値を求めたいときに、それに必要なより小さな値を再帰で求めます。
例えば、10 番目のフィボナッチ数 `fib(10)` を Top-down で求める場合、

* fib(10) を求めるには fib(9) と fib(8) が必要だ（再帰呼び出し）
* fib(9) を求めるには fib(8) と fib(7) が必要だ（再帰呼び出し）
* ... 繰り返し ...
* fib(3) を求めるには fib(2) と fib(1) が必要だ（再帰呼び出し）
* fib(2) の答えは 1 である
* fib(1) の答えは 1 である

と考えていきます。

{{< code lang="python" title="Top-down DP によるフィボナッチ関数" >}}
#!/usr/bin/env python
memo = {}

def fib(n):
    if n in memo: return memo[n]
    if n <= 2:
        memo[n] = 1
    else:
        memo[n] = fib(n-2) + fib(n-1)
    return memo[n]

print(fib(2000))  # RuntimeError: maximum recursion depth exceeded in comparison
{{< /code >}}

Top-down DP で問題の答えを求める場合、直感的な記述ができる反面、明らかな欠点があります。
それは、関数呼び出しにかかるコストです。また、再帰関数の呼び出し数には限界があるため、あまり大きな値を求めることができないという欠点もあります。


Bottom-up DP の考え方
----

一方で、Bottom-up DP の方は、小さな値から答えを積み上げていき、最終的に求めたい答えに辿り着くという考え方です。
例えば、10 番目のフィボナッチ数 fib(10) を求めるときは、

* fib(1) を求める（簡単に求まる）
* fib(2) を求める（簡単に求まる）
* fib(3) は既に求めた fib(1) と fib(2) から求められる
* fib(4) は既に求めた fib(2) と fib(3) から求められる　
* ... 繰り返し ...
* fib(10) は既に求めた fib(8) と fib(9) から求められる

のように、小さな問題の答えをあらかじめ求めておくことで、より大きな問題の答えが自ずと見つかるという手順を踏みます。

{{< code lang="python" title="Bottom-up DP によるフィボナッチ関数" >}}
#!/usr/bin/env python
def fib(n):
    memo = {}
    for i in range(1, n+1):
        if i <= 2: memo[i] = 1
        else: memo[i] = memo[i-2] + memo[i-1]
    return memo[n]

print(fib(2000))
{{< /code >}}

上記のサンプルコードでは、再帰呼び出しを行っていないところに注目です。
fib 関数を一度しか呼び出さないのであれば、上記のようにメモ化用の変数 memo をローカル変数にしてしまうこともできます。
これも、再帰呼び出しをしていないからこそできることです。

Bottom-up DP を適用するときは、

* f(n) を求めるためには、もっと小さな f(n-?) が必要だ。再帰かな？
* いや、f(1) から順番に求めてみよう

という発想の転換が必要です。小さい母集団から考えるのがコツです。

