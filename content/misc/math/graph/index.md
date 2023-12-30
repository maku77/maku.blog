---
title: "いろいろなグラフ"
url: "p/nvop8mx/"
date: "2023-12-30"
tags: ["Matplotlib"]
draft: true
useMath: true
---

log(x)
----

{{< image src="img-log.svg" >}}

\\( y = \log (x) \\) のグラフは、\\( x = 1 \\) のとき \\( y = 0 \\) になる。

{{< accordion title="matplotlib のコード" >}}
{{< code lang="python" >}}
import matplotlib.pyplot as plt
import numpy as np

x = np.linspace(0.2, 5.0, 100)  # [0.2, 5.0] の範囲の等間隔の NumPy 配列
y = np.log(x)

fig = plt.figure(layout="tight", dpi=72, figsize=(4, 3))
ax = fig.subplots()

ax.plot(x, y, label=r'$y = \log(x)$', color='blue')
ax.axhline(0, color='black', linewidth=0.5)
ax.axvline(0, color='black', linewidth=0.5)
ax.grid(True)
ax.legend()
#fig.show()

fig.savefig("img-001.svg")
{{< /code >}}
{{< /accordion >}}

