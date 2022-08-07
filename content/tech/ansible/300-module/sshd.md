---
title: "Ansible で SSH サーバーをセキュアにする (ansible.builtin.lineinfile, ansible.builtin.service)"
url: "p/hufvdta/"
date: "2022-08-06"
tags: ["Ansible", "SSH"]
---

SSH サーバーを安全に運用するには、[いくつかのベストプラクティス](/p/42cmu5d/)があります。
ここでは、root ユーザーでの SSH 接続を拒否 (`PermitRootLogin no`) する設定例を紹介します。

{{< code lang="yaml" title="secure-sshd.yaml (Playbook)" >}}
---
- hosts: all
  gather_facts: false
  become: true

  tasks:
    - name: prohibit root login
      ansible.builtin.lineinfile:
        path: /etc/ssh/sshd_config
        state: present
        regexp: '^PermitRootLogin '
        line: 'PermitRootLogin no'
      notify:
        - reload ssh daemon

  handlers:
    - name: reload ssh daemon
      ansible.builtin.service:
        name: sshd
        state: reloaded
{{< /code >}}

この Ansible Playbook では、次のようなことを行っています。

1. [`lineinfile` モジュール](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/lineinfile_module.html)で `/etc/ssh/sshd_config` の内容を修正する
   - `PermitRootLogin` で始まる行が見つからない場合は、ファイルの末尾に `PermitRootLogin no` を追加する
   - `PermitRootLogin` で始まる行が見つかった場合は、`PermitRootLogin no` に置換する（ただし、最初からその設定が記述されていたら何もしない）
2. [`service` モジュール](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/service_module.html) で SSH デーモンに設定をリロードさせる

この設定を行うと、root ユーザーでの SSH 接続ができなくなるので注意してください。
必ず、root ユーザー以外で SSH 接続できることを確認してから実行してください。

