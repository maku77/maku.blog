---
title: "Ansible タスク例: when による Playbook の条件分岐"
linkTitle: "when による Playbook の条件分岐"
url: "p/o4o7o6m/"
date: "2022-07-19"
tags: ["Ansible"]
---

コマンドが既に存在するかどうかチェックする
----

次の例では、`docker` コマンドが存在しない場合のみ、`get-docker.sh` スクリプトを実行して Docker をインストールしています。

```yaml
- name: Check if docker command exists
  ansible.builtin.shell:
    cmd: command -v docker
  register: docker_exists
  ignore_errors: yes

- name: Install Docker
  when: docker_exists is failed
  ansible.builtin.shell:
    cmd: sh ~/get-docker.sh
```

`command -v <コマンド名>` とすると、指定したコマンドがインストールされているときのみリターンコードが 0（成功）になることを利用しています。
この振る舞いは、次のように `$?`（リターンコード）の値を参照することで確かめられます。

```console
$ command -v pwd
pwd
$ echo $?
0  （pwd コマンドは存在するのでリターンコードは 0）

$ command -v hoge
$ echo $?
1  （hoge コマンドは存在しないのでリターンコードは 1）
```

