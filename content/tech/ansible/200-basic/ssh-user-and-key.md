---
title: "Ansible の SSH 接続で使用するユーザーと秘密鍵を指定する"
url: "p/n3jygwd/"
date: "2022-07-18"
tags: ["Ansible"]
---

Ansible コマンド（`ansible` や `ansible-playbook`）でマネージドノードを制御するとき、デフォルトでは SSH ユーザーとしてコントロールノードのカレントユーザーが使用されます。
異なる SSH ユーザー（と秘密鍵）で接続したい場合は、コマンドライン引数やインベントリファイルで指定することができます。

- [コマンドライン引数で指定する方法](#cmd-line)
- [インベントリファイルで指定する方法](#inventory)
- `ansible.cfg` でデフォルトユーザーを指定する方法
- [`~/.ssh/config` で接続先ごとにユーザーを指定する方法](#ssh-config)


コマンドライン引数で SSH ユーザーを指定する方法 {#cmd-line}
----

`ansible-playbook` コマンドの、__`-u (--user)`__ オプションと __`--private-key`__ オプションで、SSH 接続に使用するユーザー名と秘密鍵を指定することができます。

```console
$ ansible-playbook -i hosts.ini site.yml -u maku --private-key ~/.ssh/maku/id_rsa
```


インベントリファイルで SSH ユーザーを指定する方法 {#inventory}
----

インベントリファイルで指定する場合は、__`ansible_user`__ 変数と __`ansible_ssh_private_key`__ 変数を使います。

{{< code lang="ini" title="hosts.ini（ホストごとに指定する場合）" >}}
[servers]
www1.example.com ansible_user=maku ansible_ssh_private_key_file=~/.ssh/maku/id_rsa
www2.example.com ansible_user=maku ansible_ssh_private_key_file=~/.ssh/maku/id_rsa
{{< /code >}}

{{< code lang="ini" title="hosts.ini（グループ単位で指定する場合）" >}}
[servers]
www1.example.com
www2.example.com

[servers:vars]
ansible_user=maku
ansible_ssh_private_key_file=~/.ssh/maku/id_rsa
{{< /code >}}

YAML 形式でインベントリファイルを記述する場合も、同様の変数で設定できます。

{{< code lang="yaml" title="hosts.yml" >}}
all:
  hosts:
    www1.example.com:
    www2.example.com:
  vars:
    ansible_user: maku
    ansible_ssh_private_key_file: ~/.ssh/maku/id_rsa
{{< /code >}}


~/.ssh/config で接続先ごとにユーザーを指定する方法 {#ssh-config}
----

{{% private %}}
`~/.ssh/config` のヘルプは `man ssh_config` で確認可能。
{{% /private %}}

インベントリファイルに SSH のユーザー名や秘密鍵のパスを記述するのが煩わしいときは、SSH クライアントの設定ファイル (`~/.ssh/config`) で、接続先ごとに使用する SSH ユーザーや秘密鍵を定義してしまう方法があります。
次の例では、`maku-example.com` という名前で、SSH 接続設定を定義しています。

{{< code lang="ini" title="~/.ssh/config" >}}
Host maku-example.com
    Hostname example.com
    User maku
    Port 22
    IdentityFile ~/ssh/maku/id_rsa
{{< /code >}}

これで、次のようにするだけで、自動的にユーザー `maku`、およびその秘密鍵を使って SSH 接続されるようになります。

```console
$ ssh maku-example.com
```

この設定は、内部で ssh を使用しているプロダクトにも有効なので、Ansible のインベントリファイルは次のようにシンプルに記述できるようになります。

{{< code lang="ini" title="hosts.ini" >}}
maku-example.com
{{< /code >}}

