---
title: "Ansible タスク例: 空のディレクトリを作成する (ansible.builtin.file)"
linkTitle: "空のディレクトリを作成する (ansible.builtin.file)"
url: "p/25gqyai/"
date: "2022-08-28"
tags: ["Ansible"]
---

Ansible で空のディレクトリ作成するには、[ansible.builtin.file モジュール](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/file_module.html) の __`state`__ パラメーターに __`directory`__ を指定します。


空のディレクトリを作成する
----

```yaml
- hosts: all
  gather_facts: false
  become: true

  tasks:
    - name: Create directory
      ansible.builtin.file:
        path: /var/cache/myapp
        state: directory
```

`path` パラメーターで指定したディレクトリ `/var/cache/myapp` を作成します。
すでにディレクトリが存在する場合は、このタスクはスキップされます。
ディレクトリは再帰的に作成されるので、深い階層のパスを指定しても大丈夫です（`mkdir` の `-p` オプションに相当）。


ディレクトリのパーミッションや所有者を指定する
----

```yaml
- hosts: all
  gather_facts: false
  become: true

  tasks:
    - name: Create directory
      ansible.builtin.file:
        path: /var/cache/myapp
        state: directory
        mode: "0755"
        owner: myapp-user
        group: myapp-group
```

ディレクトリ作成時のパーミッションは `mode`、所有ユーザーは `owner`、グループは `group` で指定することができます。
パーミッションは、`0755` のような 8 進数指定の代わりに、`u=rw,g=r,o=r` のようなシンボリックモードでも指定可能です。

