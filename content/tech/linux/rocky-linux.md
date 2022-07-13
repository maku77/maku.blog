---
title: "Rocky Linux メモ (Draft)"
url: "p/m7q2cnw/"
date: "2022-07-13"
tags: ["Linux"]
draft: true
---

- Rocky Linux は CentOS の後継的な位置付け。
- Rocky Linux では、パケットフィルタの仕組みとして `iptables` ではなく、__`nftables`__ を使用する。
- Rocky Linux では、コンテナ管理ツールとして `Docker` ではなく、__`Podman`__ を使用する。
- Rocky Linux では、NIC のデバイス名として「一貫性のあるデバイス命名規則」と呼ばれる `enoX`、`ensX`、`enpXXX` などの名前が使われる。CentOS 6 の頃に使われていた `eth0` や `eth1` などの旧式の命名規則を使うオプションもあるが非推奨。

