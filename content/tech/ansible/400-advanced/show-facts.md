---
title: "Ansible で facts 情報を表示する"
url: "p/zw7emu3/"
date: "2022-08-29"
tags: ["Ansible"]
---

Ansible の Playbook を実行すると、最初にリモートホストの情報 ([Ansible facts](https://docs.ansible.com/ansible/latest/user_guide/playbooks_vars_facts.html#vars-and-facts)) の収集が行われます。
ここには、OS の情報や IP アドレスの情報などが含まれており、タスクの中でこれらの情報を参照することができます。
実際に Ansible facts にどのような情報が含まれているかは、次のような Playbook で確認することができます。

{{< code lang="yaml" title="show-facts.yml" >}}
- hosts: all
  gather_facts: true
  tasks:
    - name: Show facts
      ansible.builtin.debug:
        var: ansible_facts
{{< /code >}}

{{< code title="実行結果（抜粋）" >}}
TASK [Show facts] *********************************
ok: [example.com] => {
    "ansible_facts": {
        "all_ipv4_addresses": [
            "172.xxx.xx.xx",
            "160.xxx.xx.xx"
        ],
        "all_ipv6_addresses": [
            "xxxx:xxxx:xxxx:xxx:xxx:xxx:xx:xx",
            "xxxx::x:xxxx:xxxx:xxxx"
        ],
        ...
{{< /code >}}

`ansible_facts` 変数の値がからっぽになってしまう場合は、`gather_facts: false` と指定していないか確認してください。

`ansible_facts` オブジェクトの個々のプロパティを参照するには次のようにします。

```yaml
- name: Show architecture
  ansible.builtin.debug:
    msg: "Architecture: {{ ansible_architecture }}"
```

