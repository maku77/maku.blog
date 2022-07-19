---
title: "Ansible の SSH 接続で使用するユーザーと秘密鍵を指定する"
url: "p/n3jygwd/"
date: "2022-07-18"
tags: ["Ansible"]
---

Ansible でマネージドノードに SSH 接続するときに使用するユーザー名と秘密鍵は、__`-u (--user)`__ オプションと __`--private-key`__ オプションで指定することができます。

```console
$ ansible-playbook -i hosts.ini site.yml -u maku --private-key ~/.ssh/test/id_rsa
```

インベントリファイルで指定する場合は、__`ansible_user`__ 変数と __`ansible_ssh_private_key`__ 変数を使います。

{{< code lang="ini" title="hosts.ini（ホストごとに指定する場合）" >}}
[servers]
www1.example.com ansible_user=maku ansible_ssh_private_key_file=~/.ssh/test/id_rsa
www2.example.com ansible_user=maku ansible_ssh_private_key_file=~/.ssh/test/id_rsa
{{< /code >}}

{{< code lang="ini" title="hosts.ini（グループ単位で指定する場合）" >}}
[servers]
www1.example.com
www2.example.com

[servers:vars]
ansible_user=maku
ansible_ssh_private_key_file=~/.ssh/test/id_rsa
{{< /code >}}

YAML 形式でインベントリファイルを記述する場合も、同様の変数で設定できます。

{{< code lang="yaml" title="hosts.yml" >}}
all:
  hosts:
    www1.example.com:
    www2.example.com:
  vars:
    ansible_user: maku
    ansible_ssh_private_key_file: ~/.ssh/test/id_rsa
{{< /code >}}

