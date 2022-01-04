---
title: "Linuxメモ: デバイスドライバのライセンスについて"
linkTitle: "デバイスドライバのライセンスについて"
url: "/p/89tt5ie"
date: "2011-06-19"
tags: ["Linux"]
---

デバイスドライバを Linux カーネルに直接取り込む（静的リンクする）場合は GPL になります。
Linux カーネル自体が GPL なので、ドライバも GPL になるということです。
このようなドライバはランレベル 1 でロードされます。

カーネルモジュール（ローダブルモジュール）としてドライバを動的にロードする場合は、ライセンスは開発者が決めることができます。
通常はランレベル 3 になった時点で、`insmod` や `modprobe` コマンドでロードします。

カーネルモジュールとして作成するドライバのライセンスは、
コードの中で以下のように `MODULE_LICENSE` マクロを使用して指定します。

{{< code lang="c" >}}
MODULE_LICENSE("Dual BSD/GPL");
{{< /code >}}

ライセンスは以下のものから選択することができます。

* GPL
* GPL v2
* GPL and additional rights
* Dual BSD/GPL
* Dual MIT/GPL
* Dual MPL/GPL
* Proprietary  # default

