---
title: "Ansible の ansible.cfg ファイルの検索順序"
linkTitle: "ansible.cfg ファイルの検索順序"
url: "p/pamv6gq/"
date: "2016-10-23"
tags: ["Ansible"]
weight: 301
---

Ansible の実行環境が参照する設定ファイル (__`ansible.cfg`__) は、下記のようなパスから検索されます。

1. 環境変数 `ANSIBLE_CONFIG` で指定されたファイル
2. `ansible.cfg`（カレントディレクトリ以下のファイル）
3. `~/.ansible.cfg`（ホームディレクトリ以下のファイル）
4. `/etc/ansible/ansible.cfg`

Git などで管理する場合は、Playbook の近くに一緒に入れておくとわかりやすいです（上記の 2 番目の方法）。

