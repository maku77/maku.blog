---
title: "Ansible モジュールのヘルプを表示する (ansible-doc)"
url: "p/xx7enu3/"
date: "2022-02-18"
tags: ["Ansible"]
---

__`ansible-doc`__ コマンドを使うと、Ansible モジュールのドキュメントを表示することができます。

{{< code lang="console" title="ping モジュールのドキュメントを表示" >}}
$ ansible-doc ping
> ANSIBLE.BUILTIN.PING    (/Users/maku/Library/Python/3.10/lib/python/site-packages/ansible/modules/ping.py)

        A trivial test module, this module always returns `pong' on successful contact. It does not make sense in
        playbooks, but it is useful from `/usr/bin/ansible' to verify the ability to login and that a usable Python is
        configured. This is NOT ICMP ping, this is just a trivial test module that requires Python on the remote-node.
        For Windows targets, use the [ansible.windows.win_ping] module instead. For Network targets, use the
        [ansible.netcommon.net_ping] module instead.

ADDED IN: historical

OPTIONS (= is mandatory):

- data
        Data to return for the `ping' return value.
        If this parameter is set to `crash', the module will cause an exception.
        [Default: pong]
        type: str

...（省略）...
{{< /code >}}

例えば、上記のように ping モジュールのドキュメントを確認すると、`data` というオプションを指定できることがわかります。
次のように `data` オプションを付けて ping モジュールを実行すると、指定した値がターゲットホストからそのまま返ってきます。
モジュールのオプションは `-a` に続けて入力します。

```console
$ ansible localhost -m ping -a data=Hello
localhost | SUCCESS => {
    "changed": false,
    "ping": "Hello"
}
```

