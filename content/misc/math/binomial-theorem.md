---
title: "数学メモ: 公式 - 二項定理"
url: "/p/draiajt"
date: "2012-11-10"
tags: ["数学"]
use_math: true
---

二項定理の公式
----

$$(a+b)^n=\sum_{r=0}^{n} {}_n \mathrm{C}_r a^{n-r}b^{n}$$

### 例: 2乗の場合

$$
\begin{aligned}
(a+b)^2 &= \sum_{r=0}^{2} {}_2 \mathrm {C}_r  a^{2-r}b^r \\\\
        &= {}_2 \mathrm{C}_0 a^2b^0 + {}_2 \mathrm{C}_1 a^1b^1 + {}_2 \mathrm{C}_2 a^0b^2 \\\\
        &= a^2 + 2ab + b^2
\end{aligned}
$$

###  例: 3乗の場合

$$
\begin{aligned}
(a+b)^3 &= \sum_{r=0}^{3} {}_3 \mathrm{C}_r a^{3-r}b^r \\\\
        &= {}_3 \mathrm{C}_0 a^3b^0 + {}_3 \mathrm{C}_1 a^2b^1 + {}_3 \mathrm{C}_2 a^1b^2 + {}_3 \mathrm{C}_3 a^0b^3 \\\\
        &= a^3 + 3a^2b + 3ab^2 + b^3
\end{aligned}
$$
