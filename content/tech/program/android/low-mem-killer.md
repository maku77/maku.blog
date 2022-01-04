---
title: "Android開発: LowMemoryKiller の動きのメモ"
linkTitle: "LowMemoryKiller の動きのメモ"
url: "/p/7s4vnax"
date: "2011-05-11"
tags: ["Android"]
---

Android の lowmemorykiller では、最もメモリを使っていて、__oom_adj__ の値が大きいものが殺されます。

oom_adj の値は、`/proc/<Process ID>/oom_adj` に書かれた値です。
本来は Linux の oom (Out of memory) killer が使うファイルですが、Android の lowmemorykiller もこのファイルを使っています。

oom_adj の値は -16～15 をとり、小さいほど殺されにくくなります。デフォルトは 0 です。
ただし、oom_adj に `OOM_DISABLE (-17)` を設定すると、lowmemorykiller の対象外になるようです。

oom_adj の値を確認したり、強引に変更したい場合はこんな感じ。

{{< code >}}
# cat /proc/1234/oom_adj
0

# echo -16 > /proc/1234/oom_adj
{{< /code >}}

