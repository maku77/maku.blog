---
title: "Linuxメモ: firewalld による Linux のファイアウォール設定"
url: "p/ij6kxeq/"
date: "2022-07-06"
tags: ["Linux", "セキュリティ"]
---

Red Hat 系 Linux のネットワークのアクセス制御には、従来 `iptables` が用いられていましたが、現在は __`firewalld`__ に置き換えられています。
Fedora 18 や CentOS 7、および CentOS の後継的な位置付けの Rocky Linux 8、AlmaLinux 8 には `firewalld` が標準搭載されています（`nftables` というのもあります）。

CentOS 6 の頃は `iptables` が使われていました。
`iptables` のルールは `/etc/sysconfig/iptables` というファイルに保存され、システム再起動時にはこのファイルから設定を復帰させるという動作をしていました。
`iptables` はコマンド体系が煩雑すぎるという問題があり、より直感的な設定が可能な `firewalld` が作成されました。
`firewalld` では「一時的な」ルール設定などにも対応しています。

ファイアウォールの設定ツールは変化してきていますが、下回りとして Linux カーネルの Netfilter (netfilter firewall) が使用されていることに変わりはありません。


firewalld のインストール
----

{{< code lang="code" title="apt でのインストール" >}}
$ sudo apt install firewalld
{{< /code >}}


firewalld の使い方
----

### firewalld の起動・停止と自動起動設定

`firewalld` は、`systemd` のサービスとして自動起動できます。

```console
$ systemctl status firewalld   # 現在の状態を確認
$ systemctl start firewalld    # 直ちに firewalld を起動
$ systemctl stop firewalld     # 直ちに firewalld を停止
$ systemctl enable firewalld   # 自動起動する
$ systemctl disable firewalld  # 自動起動しない
```

### firewalld の設定を確認する

```console
$ firewall-cmd --list-all
```

### firewalld の設定を変更する

firewalld で HTTP 通信を許可するには次のようにします。

```console
$ firewall-cmd --add-service=http --permanent
$ firewall-cmd --reload
```

TCP のポート 9200、9300 へのアクセスを許可するには次のように実行します。

{{< code lang="console" title="例: firewall-cmd コマンド" >}}
$ sudo firewall-cmd --add-port={9200/tcp, 9300/tcp} --permanent
$ sudo firewall-cmd --reload
{{< /code >}}

