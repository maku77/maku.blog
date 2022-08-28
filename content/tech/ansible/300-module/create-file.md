---
title: "Ansible タスク例: 空のファイルを作成する (ansible.builtin.file)"
linkTitle: "空のファイルを作成する (ansible.builtin.file)"
url: "p/uas9p6m/"
date: "2022-08-28"
tags: ["Ansible"]
---

Ansible で空のファイルを作成するには、[ansible.builtin.file モジュール](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/file_module.html) の __`state`__ パラメーターに __`touch`__ を指定します。
Linux の `touch` コマンドに相当する動きをするので、ファイルがなければ空のファイルを作成し、ファイルが存在する場合はタイムスタンプを更新する、という振る舞いになります。


空のファイルを作成する
----

```yaml
tasks:
  - name: Create file
    ansible.builtin.file:
      path: ~/sample.txt
      state: touch
```

上記のようにすると、`path` で指定したファイルを作成します。
ファイルが既に存在する場合は、ファイルの中身は変更せずに、タイムスタンプを更新します。
つまり、このタスクはすでにファイルが存在する場合も実行されます（`changed` になる）。


タイムスタンプを更新しない
----

```yaml
- name: Create empty file
  ansible.builtin.file:
    path: ~/sample.txt
    state: touch
    access_time: preserve
    modification_time: preserve
```

ファイルがすでに存在する場合に、タイムスタンプを更新しないようにするには、__`access_time`__ および __`modification_time`__ パラメーターに __`preserve`__ を指定します（両方とも指定する必要があります）。
この指定により、ファイルがすでに存在している場合は、このタスクはスキップされるようになります。

