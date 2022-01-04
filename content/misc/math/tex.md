---
title: "数学メモ: TeX の記法のメモ"
url: "/p/oau2e7h"
date: "2013-04-29"
tags: ["数学", "TeX"]
use_math: true
---

演算子、等号（不等号）
----

### 演算子

* $a+b$ `a+b` (plus)
* $a-b$ `a-b` (minus)
* $a \times b$ `a \times b` (multiplied by, times)
* $a \pm b$ `a \pm b` (plus or minus)

### 等号・不等号

* $x+y = z$ `x+y = z` (equal to)
* $x+y \equiv z$ `x+y \equiv z` (identical to) （定義・合同）
* $x+y \neq z$ `x+y \neq z` (not equal to)
* $x+y \lt z$ `x+y \lt z` (less than)（そのまま `<` でも OK）
* $x+y \gt z$ `x+y \gt z` (greater than)（そのまま `>` でも OK）
* $x+y \leq z$ `x+y \leq z` (less than or equal to)
* $x+y \geq z$ `x+y \geq z` (greater than or equal to)
* $x+y \ll z$ `x+y \ll z` (much less than)
* $x+y \gg z$ `x+y \gg z` (much greater than)

### 近似

* $x+y \sim z$ `x+y \sim z` (similar)
* $x+y \simeq z$ `x+y \simeq z` (similar equal)
* $x+y \approx z$ `x+y \approx z` (approximately equal)


対数 (logarithm)
----

* $\log x$ `\log x`
* $\log_2 x$ `\log_2 x` 対数の底が 2 場合
* $\log_{10} x$ `\log_{10} x` 常用対数 (common logarithm)
* $\ln x$ `\ln x` 自然対数 (natural logarithm)

10 を底とする対数を常用対数 (common logarithm) と呼びます。
また、ネイピア数 $\mathrm{e}$ を底とする対数を、自然対数 (natural logarithm) と呼びます。
自然対数はラテン語で logarithmus naturalis なので、ln と略します。


分数、連分数
----

* $y = \frac{a}{b}$ `y = \frac{a}{b}` 分数 (fraction)
* $y = \frac{a-1}{\frac{b+1}{c+2}}$ `y = \frac{a-1}{\frac{b+1}{c+2}}` 連分数 (continued fraction)


論理演算（論理積∩、論理和∪、含意⇒）
----

* $\cup$ `\cup` 論理和 (logical add, logical sum, OR)
* $\cap$ `\cap` 論理積 (logical AND, logical multiply)
* $\oplus$ `\oplus` 排他的論理和 (exclusive OR)
* $A && B$ `A && B` 条件付き論理積 (conditional AND, short-circuit logical AND)
* $A || B$ `A || B` 条件付き論理和 (conditional OR, short-circuit logical OR)
* $\Rightarrow$ `\Rightarrow` 含意 (conditional implication)


順列、組み合わせ
----

* ${}_n \mathrm{P} _r$ `{}_n \mathrm{P} _r` 順列 (permutation)
* ${}_n \Pi _r$ `{}_n \Pi _r` 重複順列 (repeated permutation)
* ${}_n \mathrm{C} _r$ `{}_n \mathrm{C} _r` 組み合わせ1 (combination)
* ${n}\choose{r}$ `{n}\choose{r}` 組み合わせ2 (combination) ※海外ではこう書くことがあります
* ${}_n \mathrm{H} _r$ `{}_n \mathrm{H} _r` 重複組み合わせ (repeated combination)


Sum 型記号
----

* $\sum$ `\sum`
* $\sum_{k=1}^{n}k$ `\sum_{k=1}^{n}k`


等号 align
----

$$
\begin{align}
(a + b)^2 &= (a + b)(a + b) \\\\
          &= a^2 + 2ab + b^2
\end{align}
$$

{{< code >}}
\begin{align}
(a + b)^2 &= (a + b)(a + b) \\\\
          &= a^2 + 2ab + b^2
\end{align}
{{< /code >}}

