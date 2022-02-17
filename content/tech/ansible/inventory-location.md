---
title: "Ansible でインベントリーファイルの場所を指定する"
linkTitle: "インベントリーファイルの場所を指定する"
url: "/p/eycnx9i"
date: "2016-10-22"
tags: ["Ansible"]
weight: 202
---

Ansible のコマンド（`ansible` や `ansible-playbook`）を実行すると、下記の順でインベントリーファイルが検索されます。

1. コマンドラインオプション __`-i`__ で指定したファイル
2. コンフィグファイル __`ansible.cfg`__ 内の __`hostfile`__ で指定したファイル（参考: [ansible.cfg ファイルの検索パス](/p/pamv6gq)）
3. __`/etc/ansible/inventry`__

以下の例は、いずれもカレントディレクトリ内の `hosts` というファイルをインベントリファイルとして使用するように指定しています。

{{< code lang="console" title="例: コマンドラインオプション (-i) で指定する方法" >}}
$ ansible myserver -i hosts -m ping
{{< /code >}}

{{< code lang="ini" title="例: コンフィグファイル (ansible.cfg) で指定する方法" >}}
[defaults]
hostfile = hosts
{{< /code >}}

