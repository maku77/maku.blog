---
title: "Ansible で Hello World"
url: "p/uhu7hs4/"
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
`localhost` 以外のホストは、SSH で接続できる状態になっている必要があります（参考: [SSH 関連記事](/p/gwnatcs/)）。


ansible コマンドで ping モジュールを実行してみる
----

最初のステップとして、`ansible` コマンドで __`ping`__ モジュールを実行してみます。
`ping` モジュールは Ansible の組み込みモジュールとして提供されており、ターゲットホストへの接続確認のために使われます。
いわゆる Linux の `ping` コマンド (ICMP ping) ではないことに注意してください。

準備として、次のようなインベントリーファイル (`hosts.ini`) をカレントディレクトリに作成しておきます。

{{< code lang="ini" title="hosts.ini（インベントリーファイル）" >}}
localhost
192.168.1.20
{{< /code >}}

### ローカルホストを制御する

まずは、`localhost` に対して（自分自身を制御対象として）、`ping` モジュールを実行してみます。
`ansible` コマンドを実行するときは、第 1 パラメータで制御対象とするホスト名を指定します。
前述の通り、指定するホスト名はインベントリーファイル内に列挙されているものの中から選びます。
次のように実行して SUCCESS 表示が出れば成功です（`python` コマンドのパスに関する警告が出るかもしれませんが、ひとまず無視しておいて大丈夫です）。

{{< code lang="console" title="例: localhost に対して ping を実行" >}}
$ ansible localhost -i hosts.ini -m ping -c local
localhost | SUCCESS => {
    "changed": false,
    "ping": "pong"
}
{{< /code >}}

`ansible` コマンドの各オプションは次のような意味を持っています。

- __`-i hosts.ini`__ ... インベントリーファイルとして、カレントディレクトリの `hosts.ini` を使用します。
- __`-m ping`__ ... 組み込みの `ping` モジュールを使用してタスクを実行します。
- __`-c local`__ ... ローカルホストを制御対象とするときはこのオプションを指定します。Ansible はデフォルトで SSH 接続しようとするので、その振る舞いを抑制するためのオプションです。

{{% note title="-c local オプションを省略したいとき" %}}
イベントリーファイル内で、ホスト名 (`localhost`) の後ろに __`ansible_connection=local`__ と記述しておくと、`ansible` コマンド実行時の `-c local` オプションの指定を省略できます。

{{< code title="hosts.ini（接続方式の指定）" >}}
localhost  ansible_connection=local
192.168.1.20
{{< /code >}}

ちなみに、このようにホスト名の後ろに指定したもの「ホスト変数」と呼びます。
{{% /note %}}

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

{{< code lang="ini" title="hosts.ini" >}}
localhost  ansible_connection=local
192.168.1.20  ansible_ssh_user=maku
{{< /code >}}

これらのホスト変数設定により、`ansible` コマンド実行時にはほとんどのオプションを省略できます。

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

{{< code lang="ini" title="/etc/ansible/hosts" >}}
localhost  ansible_connection=local
192.168.1.20  ansible_ssh_user=maku
{{< /code >}}


トラブルシューティング: Python interpreter の警告が出る場合
----

Ansible のバージョンによっては、`ansible` コマンドの実行時に次のような警告が出ることがあります。

> [WARNING]: Platform darwin on host localhost is using the discovered Python interpreter at /Library/Frameworks/Python.framework/Versions/3.10/bin/python3.10, but future installation of another Python interpreter could change the meaning of that path. See https://docs.ansible.com/ansible-core/2.12/reference_appendices/interpreter_discovery.html for more information.

これは、ターゲットホスト上の Python 実行環境としてどれを使えばよいか判別できないということを示しています。
[Ansible の設定ファイル](/p/pamv6gq) に次のような感じで Python の実行ファイルパスを指定すれば警告は消えます。

{{< code lang="ini" title="ansible.cfg" >}}
[defaults]
interpreter_python = /usr/bin/python3

# 次のようにパスを自動解決させることもできます
# interpreter_python = auto_silent
{{< /code >}}

