---
title: "Nginx の基本: Nginx をインストールする"
url: "/p/wyeboxi"
date: "2013-12-01"
tags: ["nginx"]
---

yum で nginx をインストールする
----

ここでは、CentOS などの Linux 環境を想定し、nginx を yum でインストールします。
下記のような yum の設定ファイルを作成しておくことで、最新に近い nginx をインストールできるようになります。

{{< code title="/etc/yum.repos.d/nginx.repo" >}}
[nginx]
name=nginx repo
baseurl=http://nginx.org/packages/centos/$releasever/$basearch/
gpgcheck=0
enabled=1
{{< /code >}}

下記のコマンドで nginx をインストールします（**バージョンアップも同じコマンドで実行できます**）。

```
$ sudo yum install nginx
```

インストールできたか確認するために、バージョンを表示してみましょう。

```
$ nginx -v
nginx version: nginx/1.4.4
```


nginx の起動
----

下記のようにして nginx サーバを起動できます。

```
$ sudo service nginx start
```

動作しているかを確認。

```
$ sudo service nginx status
nginx (pid  19996) is running...
```


PC 起動時に nginx が自動起動するようにする
----

nginx が無事に起動するようになったら、**`chkconfig`** でマシンの立ち上げ時に nginx が自動起動するようにしておきましょう。

{{< code title="nginx を自動起動するように設定" >}}
$ sudo chkconfig nginx on
{{< /code >}}

{{< code title="設定内容の確認" >}}
$ chkconfig --list nginx
nginx               0:off     1:off     2:on     3:on     4:on     5:on     6:off
{{< /code >}}

