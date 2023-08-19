---
title: "Linuxコマンド: ポートを開いているプロセスを調べる (lsof)"
url: "p/i7zokcv/"
date: "2023-08-19"
tags: ["Linux", "lsof", "ネットワーク"]
---

迷ったらとりあえずコレ
----

次のように実行すると、TCP でポートを開いて LISTEN 状態になっているプロセスのコマンド名や PID の一覧を表示できます。

```console
$ lsof -i tcp -s tcp:listen -P
COMMAND     PID USER   FD   TYPE             DEVICE SIZE/OFF NODE NAME
ControlCe   604 maku   24u  IPv4 0xbf4647db74c55935      0t0  TCP *:7000 (LISTEN)
ControlCe   604 maku   25u  IPv6 0x27a96e6374c5592c      0t0  TCP *:7000 (LISTEN)
ControlCe   604 maku   26u  IPv4 0xbf46527374c55935      0t0  TCP *:5000 (LISTEN)
ControlCe   604 maku   27u  IPv6 0x27a9756374c5592c      0t0  TCP *:5000 (LISTEN)
deno       8572 maku   14u  IPv4 0xbc6a127374c55935      0t0  TCP *:8000 (LISTEN)
hugo      10971 maku    6u  IPv4 0xbcba127374c55935      0t0  TCP localhost:50001 (LISTEN)
hugo      10975 maku    6u  IPv4 0xbd35a81374c55935      0t0  TCP localhost:50002 (LISTEN)
ruby      52070 maku    9u  IPv4 0xbf461d7b74c55935      0t0  TCP localhost:4000 (LISTEN)
ruby      52070 maku   12u  IPv4 0xbd38681374c55935      0t0  TCP localhost:35729 (LISTEN)
node      65924 maku   38u  IPv6 0x27a9356374c5592c      0t0  TCP localhost:5173 (LISTEN)
Dropbox   68519 maku  114u  IPv4 0xbc67e81374c55935      0t0  TCP *:17500 (LISTEN)
Dropbox   68519 maku  115u  IPv6 0x27a8836374c5592c      0t0  TCP *:17500 (LISTEN)
```


lsof -i の使い方
----

[lsof](https://lsof.readthedocs.io/en/latest/) (list open files) コマンドは、Linux や macOS で、現在開いているファイルやソケットの一覧を表示するコマンドです。
TCP/UDP ソケット（つまりネットワーク）に関する情報を表示するには、__`-i`__ オプションを指定します。

```console
$ lsof -i
$ lsof -i tcp -s tcp:listen       # TCP で LISTEN 状態になっているものに絞る
$ lsof -i tcp:3000 -s tcp:listen  # 指定したポート番号を開いているプロセスを調べる
```

デフォルトでは、ポート番号がサービス名（IANA 管理）に変換して表示されるので、この挙動を抑制して、数値のポート番号で表示したいときは、__`-P`__ オプションを指定します。

```console
$ lsof -i tcp:8000
COMMAND   PID USER   FD   TYPE             DEVICE SIZE/OFF NODE NAME
Python  71362 maku    5u  IPv6 0x2825a06374c5592c      0t0  TCP *:irdmi (LISTEN)

$ lsof -i tcp:8000 -P
COMMAND   PID USER   FD   TYPE             DEVICE SIZE/OFF NODE NAME
Python  71362 maku    5u  IPv6 0x2825a06374c5592c      0t0  TCP *:8000 (LISTEN)
```

上記のように実行して PID を調べれば、`kill` コマンドで対象プロセスを終了することができます。

```console
$ kill 71362
```

