---
title: "Linuxコマンド: ip コマンドの使い方"
url: "/p/8awcq4h"
date: "2022-02-18"
tags: ["Linux"]
draft: true
---

ip addr - IP アドレスの確認
----


ip コマンドのインストール
----

Docker の Ubuntu コンテナなどで `ip` コマンドが見つからない場合は、次のように __`iproute2`__ パッケージをインストールします。

```console
$ apt install iproute2
```

`iprout2` パッケージをインストールすると、次のようなコマンドを使用できるようになります。

- `arpd` - userspace arp daemon
- `bridge` - show / manipulate bridge addresses and devices
- `devlink` - Devlink tool
- __`ip`__ - show / manipulate routing, devices, policy routing and tunnels
- `lnstat` - unified linux network statistics
- `nstat` - network statistics tools
- `routef` - flush routes
- `routel` - list routes with pretty output format
- `rtacct` - network statistics tools
- `rtmon` - listens to and monitors RTnetlink
- `rtstat` - unified linux network statistics
- `ss` - another utility to investigate sockets
- `tc` - show / manipulate traffic control settings
- `tipc` - a TIPC configuration and management tool

