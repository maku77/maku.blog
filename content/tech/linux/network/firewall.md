---
title: "Linuxメモ: Linux のファイアウォール設定 (ufw, firewalld)"
url: "p/ij6kxeq/"
date: "2022-07-06"
tags: ["Linux", "ネットワーク"]
draft: true
---

- Cent OS 7 ではデフォルトで __`firewalld`__ が導入される
- Ubuntu 16.04 では __`ufw`__ というファイアウォール設定ツールが導入されている

firewalld のインストール
----

{{< code lang="code" title="apt でのインストール" >}}
$ sudo apt install firewalld
{{< /code >}}


firewalld の使い方
----

Linux のネットワークのアクセス制御には、従来 `iptables` が用いられていましたが、現在は __`firewalld`__ に置き換えられています。
Red Hat 系の Cent OS 7 では、`firewalld` がデフォルトになっています。

### firewalld の起動・停止と自動起動設定

`firewalld` は、`systemd` のサービスとして自動起動できます。

```console
$ systemctl status firewalld     # 現在の状態を確認

$ systemctl enable firewalld     # 自動起動するように設定
$ systemctl disable firewalld    # 自動起動しないように設定

$ systemctl startstop firewalld  # 直ちに firewalld を起動する
$ systemctl stop firewalld       # 直ちに firewalld を停止する
```

### firewalld の設定を確認する

```console
$ firewall-cmd --list-all
```

### firewalld の設定を変更する

firewalld で HTTP 通信を許可するには次のようにします。

```console
$ firewall-cmd --permanent --add-service=http
$ firewall-cmd --reload
```

TCP のポート 9200、9300 へのアクセスを許可するには次のように実行します。

{{< code lang="console" title="例: firewall-cmd コマンド" >}}
$ sudo firewall-cmd --add-port={9200/tcp, 9300/tcp} --permanent
$ sudo firewall-cmd --reload
{{< /code >}}

{{< code lang="console" title="例: ufw コマンド" >}}
$ sudo ufw allow 9200
$ sudo ufw allow 9300
{{< /code >}}

