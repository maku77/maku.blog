---
title: "Ansible タスク例: UFW でファイアウォールを設定する (community.general.ufw)"
linkTitle: "UFW でファイアウォールを設定する (community.general.ufw)"
url: "p/evdubr7/"
date: "2022-07-19"
tags: ["Ansible"]
---

Ubuntu のファイアウォール（パケットフィルタ）設定には、UFW というツールが使用されています。

- 参考: [Linux コマンド: ufw による Ubuntu のファイアウォール設定](/p/drar8o4/)

Ansible の [community.general.ufw](https://docs.ansible.com/ansible/latest/collections/community/general/ufw_module.html) モジュールを使用して [UFW (Uncomplicated Firewall)](https://help.ubuntu.com/community/UFW) の設定を行うことができます。
`community.general` コレクションは、Ansible のコミュニティパッケージをインストールした場合は標準でインストールされています。


UFW をインストールする
----

UFW の設定は `community.general.ufw` モジュールで行うのですが、UFW の本体（`ufw` コマンド）はあらかじめ APT でインストールしておく必要があります。

```yaml
- name: Install UFW
  ansible.builtin.apt:
    name: ufw
    state: present
    update_cache: yes
```


UFW を有効化する
----

UFW をインストールしたら、有効化する必要があります。
次の例では、デフォルトポリシーを接続拒否 (deny) にして UFW を有効化しています。

```yaml
- name: Enable UFW
  community.general.ufw:
    state: enabled
    policy: deny
```


特定ポートへのアクセスを許可する
----

次の例では、22 番ポート (SSH) と、80 番ポート (HTTP)、443 番ポート (HTTPS) へのアクセスを許可しています。

```yaml
- name: UFW - Allow all access to tcp port 22 (SSH)
  community.general.ufw:
    rule: allow
    port: '22'
    proto: tcp

- name: UFW - Allow all access to tcp port 80 (HTTP)
  community.general.ufw:
    rule: allow
    port: '80'
    proto: tcp

- name: UFW - Allow all access to tcp port 443 (HTTPS)
  community.general.ufw:
    rule: allow
    port: '443'
    proto: tcp
```


特定ポートへの連続アクセスを制限する
----

特定ポートに対して、30 秒以内に 6 回以上のアクセスがあった場合に、一定時間接続を拒否します。
次のようにすると、SSH ポートへのブルートフォースアタックを検出したときに、アクセスを遮断します。

```yaml
- name: UFW - Protecting against brute-force login attacks
  community.general.ufw:
    rule: limit
    port: ssh
    proto: tcp
```

`rule: limit` を設定した場合は、そのポートに対して `rule: allow` でアクセス許可する必要はありません。
`rule: limit` は制限付きの `rule: allow` を意味します。

