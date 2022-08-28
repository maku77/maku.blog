---
title: "Ansible タスク例: ユーザーを特定のグループに参加させる (ansible.builtin.user)"
linkTitle: "ユーザーを特定のグループに参加させる (ansible.builtin.user)"
url: "p/s2fs5gs/"
date: "2022-08-29"
tags: ["Ansible"]
---

Ansible の [ansible.builtin.user モジュール](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/user_module.html) を使用すると、ターゲットノード上に指定したユーザーを作成することができます。
このモジュールは、既存のユーザーの情報を変更する場合にも使えるので、例えば、あるユーザーを指定したグループに参加させる、といったことが可能です。

次の Playbook では、ユーザー `maku` をグループ `docker` に参加させています。
ユーザーが存在しない場合は、ユーザー自体の作成も行います。

```yaml
- hosts: all
  become: true

  tasks:
    - name: Append the group 'docker' to the user's groups
      ansible.builtin.user:
        name: maku
        groups: [docker]
        append: yes
```

注意点としては、__`groups`__ でグループ名を列挙すると同時に、__`append: yes`__ と指定するところです。
`append` 指定がないと、現在参加しているグループの情報が消えて、`groups` で指定したグループにしか参加していない状態になります。

ユーザー名をハードコードするのではなく、SSH 接続しているカレントユーザーを対象にしてグループ参加させたいときは、ユーザー名として __`"{{ ansible_facts.env.SUDO_USER }}"`__ を指定します。
これにより、`sudo` 実行前のユーザー名を取得できます。

```yaml
- hosts: all
  gather_facts: true
  become: true

  tasks:
    - name: Append the group 'docker' to the current user's groups
      ansible.builtin.user:
        name: "{{ ansible_facts.env.SUDO_USER }}"
        groups: [docker]
        append: yes
```

`{{ ansible_user_id }}` とすると、`sudo` した結果のユーザー名である `root` になってしまうので注意してください。


