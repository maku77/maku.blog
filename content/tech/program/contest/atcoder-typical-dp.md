---
title: "プロコンメモ: At Coder - Typical DP Contest"
url: "p/o7ghox9/"
date: "2013-08-31"
tags: ["プロコン"]
---

問題: https://tdpc.contest.atcoder.jp/assignments


Typical DP Contest: 問題 (A) コンテスト - 解答メモ
----

問題: https://tdpc.contest.atcoder.jp/tasks/tdpc_contest

取り得る得点の配列を **`dp[]`** として、0 問解くごとに、,どの得点を取り得るかの情報をアップデートしていけば OK です。
初期状態は、0 問解いたときに取り得る得点は 0 点だけなので、`dp[0] = True` として、あとは False に初期化しておきます。

{{< code lang="python" title="a.py" >}}
#!/usr/bin/python2.7
import sys
def read_line(): return sys.stdin.readline().strip()
def read_int(): return int(sys.stdin.readline())
def read_ints(): return [int(x) for x in sys.stdin.readline().split()]

def solve():
    # Read a problem
    N = read_int()
    P = read_ints()

    # Solve a problem
    dp = [False] * (sum(P) + 1)
    dp[0] = True
    for p in P:
        for i in reversed(range(len(dp))):
            if dp[i]:
                dp[i + p] = True

    return dp.count(True)

print(solve())
{{< /code >}}


Typical DP Contest: 問題 (B) ゲーム - 解答メモ
----

問題: https://tdpc.contest.atcoder.jp/tasks/tdpc_game

`dp[i][j]` を、A から i 個目、B から j 個目を取り終わったときに、残りの山から獲得できる **合計得点差** の最善値とします。
**合計得点差** というのは、

「自分が獲得できる合計点数 - 相手が獲得できる合計点数」

を表します。**`dp[A][B]`** は、山から全て取り終わった後を示していて、そこから得点できる点数は 0 です。

```python
dp[A][B] = 0
```

**`dp[A-1][B]`** は、A の山から最後の 1 個を自分が獲得したときの、相手との合計得点差なので、

```python
dp[A-1][B] = A 山の最後の札の得点
```

と初期化できます。

**`dp[A-2][B]`** は、A の山の最後から 2 個目を自分が獲得したときの合計得点差なので、

```python
dp[A-2][B] = A 山の最後から 2 枚目の得点 - dp[A-1][B]
```

となります。相手との合計得点差を格納していくので、そこで取得できる得点から `dp[A-1][B]` を引いているところがポイントです。

**`dp[A-1][B-1]`** は、A の山にも、B の山にもそれぞれ 1 個ずつ残っている状態からの合計得点差です。
この状態からは A と B どちらを取るかを選べるのですが、「すぬけ君」も「すめけ君」も最善手を選ぶので、どちらを取った方がより多く得点を獲得できるかを判断基準にして計算していきます。

```python
dp[A-1][B-1] = max(A山を選んだときの点 - dp[A][B-1], B山を選んだときの点 - dp[A-1][B])
```

この計算を最後まで続けていき、**`dp[0][0]`** の値が求まると、それが「先手の最善の合計得点差」になります。
求めたいのは、先手の「得点差」ではなく「得点」なので、以下のように計算すると答えが出ます。

```
dp[0][0] = 先手合計点 - 後手合計点
山の合計点 = 先手合計点 + 後手合計点
つまり、
先手合計点 = (dp[0][0] + 山の合計点) / 2
```

{{< code lang="python" title="b.py" >}}
#!/usr/bin/python2.7
import sys
def read_line(): return sys.stdin.readline().strip()
def read_int(): return int(sys.stdin.readline())
def read_ints(): return [int(x) for x in sys.stdin.readline().split()]

def solve():
    # Read a problem
    A, B = read_ints()
    a = read_ints()
    b = read_ints()

    # Solve a problem
    dp = [[0] * (B+1) for _ in range(A+1)]  # dp[A+1][B+1]

    dp[A][B] = 0
    for i in reversed(range(A)):
        dp[i][B] = a[i] - dp[i+1][B]
    for j in reversed(range(B)):
        dp[A][j] = b[j] - dp[A][j+1]
        for i in reversed(range(A)):
            dp[i][j] = max(a[i] - dp[i+1][j], b[j] - dp[i][j+1])

    return (dp[0][0] + sum(a) + sum(b)) / 2

print(solve())
{{< /code >}}


Typical DP Contest: 問題 (C) トーナメント - 解答メモ
----

問題: https://tdpc.contest.atcoder.jp/tasks/tdpc_tournament

4 人 (A, B, C, D) でトーナメントをするとします。

```
A --+
    +--+
B --+  |
       +-- Winner
C --+  |
    +--+
D --+
```

A が N 回戦後に残っている可能性を求める問題だと考えれば OK です。
上記のケースでは N=2

```
Aが0回戦後に残る確率
    = 100% (1.0)
Aが1回戦後に残る確率
    = AがBに勝つ確率
Aが2回戦後に残る確率
    = Aが1回戦後に残る確率 x Aが2回戦に勝つ確率
    = Aが1回戦後に残る確率 x (Cが1回戦に残ってAが勝つ確率 + Dが1回戦に残ってAが勝つ確率)
```

上記のように考えると、A が N 回戦後に残る確率は、N-1 回戦後に残っている確率を用いて求められるので、DP で解くことができます。

