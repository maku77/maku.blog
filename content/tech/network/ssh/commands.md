---
title: "SSH サーバー (sshd) の起動・停止・再起動方法まとめ"
url: "p/9gs3cmu/"
date: "2022-07-17"
tags: ["SSH"]
---

各 OS（ディストリビューション）で SSH サーバーを起動・停止するときのコマンドのまとめです。

Debian 系 (Ubuntu/Debian)
----

{{< code lang="console" title="Ubuntu 15.04 以降" >}}
$ sudo systemctl start ssh   （起動）
$ sudo systemctl stop ssh    （停止）
$ sudo systemctl restart ssh （再起動）
$ sudo systemctl reload ssh  （設定リロード）
{{< /code >}}

{{< code lang="console" title="Ubuntu 14.10 以前" >}}
$ sudo service ssh start   （起動）
$ sudo service ssh stop    （停止）
$ sudo service ssh restart （再起動）
$ sudo service ssh reload  （設定リロード）
{{< /code >}}

{{% note title="ssh でも sshd でも OK" %}}
`/lib/systemd/system/ssh.service` ファイルに、エイリアスとして `sshd.service` が定義されています。
なので、上記コマンドの `ssh` の部分は `sshd` としても動作します。
サービス名のサフィックス (`.service`) はほとんどのケースで省略できます。
{{% /note %}}


Red Hat 系 (RHEL/Fedora/CentOS/Rocky Linux)
----

{{< code title="CentOS 7.x/8.x" >}}
# systemctl start sshd.service   （起動）
# systemctl stop sshd.service    （停止）
# systemctl restart sshd.service （再起動）
# systemctl reload sshd.service  （設定リロード）
{{< /code >}}

{{< code title="CentOS 4.x/5.x/6.x" >}}
# service sshd start   （起動）
# service sshd stop    （停止）
# service sshd restart （再起動）
# service sshd reload  （設定リロード）

OR

# /etc/init.d/sshd start   （起動）
# /etc/init.d/sshd stop    （停止）
# /etc/init.d/sshd restart （再起動）
# /etc/init.d/sshd reload  （設定リロード）
{{< /code >}}


FreeBSD / OpenBSD
----

```
# /etc/rc.d/sshd start   （起動）
# /etc/rc.d/sshd stop    （停止）
# /etc/rc.d/sshd restart （再起動）
# /etc/rc.d/sshd reload  （設定リロード）
```

