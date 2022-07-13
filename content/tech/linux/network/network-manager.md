---
title: "Linuxメモ: NetworkManager による Linux のネットワーク設定"
url: "p/mzovvgq/"
date: "2022-07-13"
tags: ["Linux"]
draft: true
---

NetworkManager を使うと、`/etc/sysconfig/network-scripts/ifcfg-*` ファイルを直接編集せずに、コマンドラインツール (`nmcli`) や GUI ツールにファイルの更新を任せることができます。
これらのファイルを直接編集した場合は、`nmcli connection reload` コマンドを実行して、その内容を NetworkManager に反映する必要があります。


NetworkManager 自体を操作するコマンド
----

{{< code lang="console" title="NetworkManager を再起動する" >}}
$ sudo systemctl restart NetworkManager
{{< /code >}}

