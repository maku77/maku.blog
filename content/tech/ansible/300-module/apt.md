---
title: "Ansible タスク例: APT パッケージをインストールする (ansible.builtin.apt)"
linkTitle: "APT パッケージをインストールする (ansible.builtin.apt)"
url: "p/efqyair/"
date: "2022-07-19"
tags: ["Ansible"]
---

Debian 系の Linux ディストリビューション（Ubuntu など）では、パッケージ管理に APT を使用します。
Ansible 組み込みの [ansible.builtin.apt](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/apt_module.html) モジュールを使用して、APT パッケージのインストールを行うことができます。


APK パッケージを 1 つインストールする
----

```yaml
- name: Install Apache
  ansible.builtin.apt:
    name: apache2
    state: present
    update_cache: yes
```

`state: present` の指定はデフォルトなので省略できます。
__`update_cache: yes`__ を指定しておくと、事前に `apt update` を実行してパッケージリスト情報を更新してくれます。
さらに、`cache_valid_time: 3600` のように、キャッシュの有効期間（秒）を指定しておくこともできます。


複数のパッケージをインストールする
----

```yaml
- name: Install a list of packages
  ansible.builtin.apt:
    pkg:
    - git
    - iproute2
    - ...
```

