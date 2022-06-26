---
title: "Linux ネットワーク: ip コマンドの使い方"
url: "p/p7q7n4i/"
date: "2022-06-16"
tags: ["ネットワーク"]
---

{{% private %}}
- [ip コマンドチートシート](https://access.redhat.com/sites/default/files/attachments/rh_ip_command_cheatsheet_1214_jcs_print_ja4.pdf)
- [Wikipedia - iproute2](https://ja.wikipedia.org/wiki/Iproute2)
{{% /private %}}

apt で ip コマンドをインストールする
----

__`ip`__ コマンドは APT の __`iproute2`__ パッケージに含まれています。

```console
$ apt update  # パッケージリストの更新
$ apt install -y iproute2
```

iproute2 パッケージには、`ip` コマンド以外にも いろいろなコマンドが含まれています（例: ip・ss・bridge・rtacct・rtmon・tc・ctstat・lnstat・nstat・routef・routel・rtstat・tipc・arpd・devlink）。

- 参考: iproute2 に含まれるファイルのリスト ─ [Debian](https://packages.debian.org/sid/amd64/iproute2/filelist) / [archlinux](https://archlinux.org/packages/core/x86_64/iproute2/files/)


サブコマンドのヘルプを表示する
----

__`ip <サブコマンド名> help`__ で各サブコマンドのヘルプを表示できます。

{{< code lang="console" title="例: ip address コマンドのヘルプを表示する" >}}
$ ip address help
{{< /code >}}

{{< code lang="console" title="例: ip link コマンドのヘルプを表示する" >}}
$ ip link help
{{< /code >}}


ip address コマンド（IP アドレスの確認・設定）
----

`ip address` コマンドは、ホストの（ネットワークインタフェース）に割り当てられている IP アドレスの情報を表示します。
サブコマンドの `address` は 1 文字まで省略できるので、`ip a` で実行することができます。

{{< code lang="console" title="すべてのアドレスを表示" >}}
$ ip addr
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
2: tunl0@NONE: <NOARP> mtu 1480 qdisc noop state DOWN group default qlen 1000
    link/ipip 0.0.0.0 brd 0.0.0.0
3: ip6tnl0@NONE: <NOARP> mtu 1452 qdisc noop state DOWN group default qlen 1000
    link/tunnel6 :: brd :: permaddr e28e:402f:dcf2::
81: eth0@if82: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default 
    link/ether 02:42:ac:11:00:03 brd ff:ff:ff:ff:ff:ff link-netnsid 0
    inet 172.17.0.3/16 brd 172.17.255.255 scope global eth0
       valid_lft forever preferred_lft forever
{{< /code >}}

{{< code lang="console" title="指定したデバイス (eth0) のアドレスを表示" >}}
$ ip addr show dev eth0
81: eth0@if82: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default 
    link/ether 02:42:ac:11:00:03 brd ff:ff:ff:ff:ff:ff link-netnsid 0
    inet 172.17.0.3/16 brd 172.17.255.255 scope global eth0
       valid_lft forever preferred_lft forever
{{< /code >}}

