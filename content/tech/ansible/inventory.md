---
title: "Ansible のインベントリファイルとは"
linkTitle: "インベントリファイルとは"
url: "/p/h4fq2bm"
date: "2016-10-20"
tags: ["Ansible"]
weight: 201
---

Ansible で制御したいホストは、__インベントリファイル (inventory file)__ に列挙しておく必要があります。
これは、想定外のホストを操作してしまうのを防ぐための安全策です。
デフォルトでは、インベントリファイルとして __`/etc/ansible/hosts`__ というファイルが読み込まれます。

{{< code lang="ini" title="/etc/ansible/hosts（記述例）" >}}
localhost
192.168.100.1
red.example.com
blue.example.com

[webservers]
192.168.100.2
red.example.com
green.example.com

[dbservers]
db1.example.com
db2.example.com
{{< /code >}}

上記のように、ホスト名や IP アドレスで制御対象のホストを列挙します。
`[webservers]` や `[dbservers]` という部分では __グループ名__ を定義しており、複数のホストに対して同時に命令する場合に、ホスト名の代わりにこのグループ名を使用することができるようになります。

インベントリファイルに記述されたホスト名やグループ名を、`ansible` コマンドや `ansible-playbook` コマンドの第一パラメータとして渡すことで、制御対象のホストを指定することになります。
上記では、`localhost` を明示的に指定していますが、Ansible 2.0 以降は `localhost` というホスト名は暗黙的に使用できるようになっています。

{{< code lang="console" title="例: localhost に対して ping を実行（ping モジュールを起動）" >}}
$ ansible localhost -c local -m ping
localhost | success >> {
    "changed": false,
    "ping": "pong"
}
{{< /code >}}

`-m ping` オプションで、ping モジュールを使用してタスクを実行することを示しています。
インベントリファイルに定義されていないホスト名やグループ名を指定すると、`ansible` コマンドや `ansible-playbook` コマンドは下記のような警告メッセージを出力します。

{{< code lang="console" title="例: インベントリに登録されていないホストを指定した場合" >}}
$ ansible unknown-host -m ping
[WARNING]: No inventory was parsed, only implicit localhost is available
[WARNING]: provided hosts list is empty, only localhost is available.
           Note that the implicit localhost does not match 'all'
[WARNING]: Could not match supplied host pattern, ignoring: unknown-host
{{< /code >}}

