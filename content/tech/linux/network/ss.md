---
title: "Linuxコマンド: TCP/TDP ポートの通信状態を確認する (ss)"
url: "p/sg2m6wm/"
date: "2022-07-05"
tags: ["Linux", "ネットワーク"]
---

関連コマンド: [ip コマンド](/p/p7q7n4i/)

ss コマンドとは
----

Linux の __`ss`__ コマンド (socket statistics) は、TCP ポートや UDP ポートの通信状態を確認するためのコマンドで、過去に使われていた `net-tools` パッケージの `netstat` コマンドの後継です。
Cent OS 7 ではデフォルトで `ss` コマンドが採用されています。

APT の `iproute2` パッケージをインストールすると、`ss` コマンドを使用できるようになります。

{{< code lang="console" title="ss コマンドのインストール" >}}
$ apt update  # パッケージリストの更新
$ apt install -y iproute2
{{< /code >}}

`iptable2` パッケージをインストールすると、一緒に [ip コマンド](/p/p7q7n4i/) などもインストールされます。


ss サブコマンド
----

| コマンド | 説明 | 対応する旧コマンド |
| ---- | ---- | ---- |
| __`ss -nat`__ | TCP ポートの通信状態を確認 | `netstat -nat` |
| __`ss -nlt`__ | LISTEN（待ち受け）状態の TCP ポートを確認 | `netsta -nlt` |
| __`ss -nau`__ | UDP ポートの通信状態を確認<br/>UDP ソケットの `State` カラムは `UNCONN` と表示される | `netstat -nau` |

