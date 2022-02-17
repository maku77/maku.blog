---
title: "Ansible で Hello World"
url: "/p/uhu7hs4"
date: "2022-02-17"
weight: 102
tags: ["Ansible"]
---

インベントリーファイルを作る
----

Ansible で制御したいホストは、__インベントリーファイル (inventory file)__ に列挙しておく必要があります。
これは、想定外のホストを操作してしまうのを防ぐための安全策です。
デフォルトでは、インベントーリファイルとして __`/etc/ansible/hosts`__ というファイルが読み込まれます。
コマンドラインオプション (`-i`) などで、[読み込むファイルを指定する](/p/eycnx9i) こともできます。

{{< code lang="ini" title="/etc/ansible/hosts（記述例）" >}}
localhost
192.168.1.20
host.example.com
{{< /code >}}

インベントリーファイル内では、上記のように「ホスト名」や「IP アドレス」で制御対象のホスト (managed node) を列挙します。
ここでは、3 つのホストを Ansible のコマンド（`ansible` や `ansible-playbook`）で制御できるようにしています。
`localhost` 以外のホストは、SSH で接続できる状態になっている必要があります。


ansible コマンドで ping モジュールを実行してみる
----

インベントリーファイルを用意したら、まずは制御対象のホストに `ping` を実行してみます。
`ping` は Ansible の組み込みモジュールとして提供されており、ターゲットホストへの接続確認のために使われます。
いわゆる Linux の `ping` コマンド (ICMP ping) ではないことに注意してください。

### ローカルホストを制御する

まずは、`localhost` に対して（自分自身を制御対象として）、`ping` を実行してみます。
制御対象とするホスト名は、`ansible` コマンドの第 1 パラメータで指定します。
次のように SUCCESS 表示が出れば成功です。

{{< code lang="console" title="例: localhost に対して ping を実行" >}}
$ ansible localhost -c local -m ping
localhost | SUCCESS => {
    "changed": false,
    "ping": "pong"
}
{{< /code >}}

Ansible はデフォルトで SSH 接続しようとするので、ローカルホストを制御対象とするときは __`-c local`__ オプションを指定します。
__`-m ping`__ オプションは、ping モジュールを使用してタスクを実行することを示しています。

イベントリーファイルで、接続方法オプション __`ansible_connection=local`__ を指定しておくと、`ansible` コマンド実行時の `-c local` の指定を省略できます。

{{< code title="/etc/ansible/hosts（接続オプションを追加）" >}}
localhost  ansible_connection=local
192.168.1.20
{{< /code >}}

インベントリーファイルに定義されていないホスト名を指定すると、`ansible` コマンドは次のような警告メッセージを出力して終了します。

{{< code lang="console" title="例: インベントリーに登録されていないホストを指定した場合" >}}
$ ansible unknown-host -m ping
[WARNING]: Could not match supplied host pattern, ignoring: unknown-host
[WARNING]: No hosts matched, nothing to do
{{< /code >}}


### SSH 経由で制御する

次に、SSH で接続可能なターゲットホストに対して `ping` モジュールを実行してみます。
あらかじめターゲットホストには SSH 鍵による接続ができることを確認しておいてください。

- 参考: [ssh-id-copy で SSH の公開鍵をリモートホストに登録する](/p/2mzbmw8)

パスワード認証でも Ansible による制御は可能ですが、追加で `sshpass` パッケージをインストールするなどの対応が必要になります。
SSH 鍵による公開鍵認証方式で接続できるようになっていれば、次のように `ansible` コマンドで制御できるはずです。
SSH での接続ユーザー名は `maku` だと仮定しています。

{{< code lang="console" title="例: SSH 接続で ping を実行" >}}
$ ansible 192.168.1.20 -u maku -m ping
Enter passphrase for key '/Users/maku/.ssh/id_rsa': （SSH鍵のパスワードを入力）
192.168.1.20 | SUCCESS => {
    "changed": false,
    "ping": "pong"
}
{{< /code >}}

上の例では、接続ユーザー名を `-u` オプションで指定していますが、インベントリーファイルの中で次のようにユーザー名を設定しておくこともできます。

{{< code lang="inf" title="/etc/ansible/hosts" >}}
localhost  ansible_connection=local
192.168.1.20  ansible_ssh_user=maku
{{< /code >}}

この設定により、`ansible` コマンド実行時にはほとんどのオプションを省略できます。

```console
$ ansible localhost -m ping
$ ansible 192.168.1.20 -m ping
```


すべてのターゲットホストをまとめて制御する
----

`ansible` コマンドの第 1 引数（ターゲット名）として __`all`__ を指定すると、インベントリーファイルに記述したすべてのターゲットホストを一度に制御することができます。

```console
$ ansible all -m ping
localhost | SUCCESS => {
    "changed": false,
    "ping": "pong"
}
Enter passphrase for key '/Users/maku/.ssh/id_rsa': （SSH鍵のパスワードを入力）
192.168.1.20 | SUCCESS => {
    "changed": false,
    "ping": "pong"
}
```

接続方法 (local or ssh) や接続ユーザー名は、インベントリーファイルなどで指定しておく必要があります。

{{< code lang="inf" title="/etc/ansible/hosts" >}}
localhost  ansible_connection=local
192.168.1.20  ansible_ssh_user=maku
{{< /code >}}


おまけ: ping モジュールのヘルプを確認する
----

__`ansible-doc`__ コマンドを使うと、Ansible モジュールのドキュメントを表示することができます。

```console
$ ansible-doc ping
> ANSIBLE.BUILTIN.PING    (/Users/maku/Library/Python/3.10/lib/python/site-packages/ansible/modules/ping.py)

        A trivial test module, this module always returns `pong' on successful contact. It does not make
        sense in playbooks, but it is useful from `/usr/bin/ansible' to verify the ability to login and
        that a usable Python is configured. This is NOT ICMP ping, this is just a trivial test module that
        requires Python on the remote-node. For Windows targets, use the [ansible.windows.win_ping] module
        instead. For Network targets, use the [ansible.netcommon.net_ping] module instead.
...（省略）...
```

例えば、上記のように ping モジュールの説明を読むと、`data` というオプションを指定できることがわかります。
次のように `data` オプションを付けて ping モジュールを実行すると、指定した値がターゲットホストからそのまま返ってきます。
モジュールのオプションは `-a` に続けて入力します。

```console
$ ansible localhost -m ping -a data=Hello
localhost | SUCCESS => {
    "changed": false,
    "ping": "Hello"
}
```


トラブルシューティング: Python interpreter の警告が出る場合
----

Ansible のバージョンによっては、`ansible` コマンドの実行時に次のような警告が出ることがあります。

> [WARNING]: Platform darwin on host localhost is using the discovered Python interpreter at /Library/Frameworks/Python.framework/Versions/3.10/bin/python3.10, but future installation of another Python interpreter could change the meaning of that path. See https://docs.ansible.com/ansible-core/2.12/reference_appendices/interpreter_discovery.html for more information.

これは、ターゲットホスト上の Python 実行環境としてどれを使えばよいか判別できないということを示しています。
[Ansible の設定ファイル](/p/pamv6gq) に次のような感じで Python の実行ファイルパスを指定すれば警告は消えます。

{{< code lang="inf" title="ansible.cfg" >}}
[defaults]
interpreter_python = /usr/bin/python3

# 次のようにパスを自動解決させることもできます
# interpreter_python = auto_silent
{{< /code >}}

