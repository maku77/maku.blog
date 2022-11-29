---
title: "Nginx の設定: Nginx の設定ファイルの書き方が正しいか確認する (configtest, nginx -t)"
url: "/p/3q8uzoo"
date: "2015-09-21"
tags: ["nginx"]
---

nginx の __`configtest`__ コマンドを使用すると、nginx のコンフィグファイルの記述方法が正しいかを確認することができます。
このコマンドは管理者権限で実行する必要があります。

```console
$ sudo /etc/init.d/nginx configtest
nginx: the configuration file /etc/nginx/nginx.conf syntax is ok
nginx: configuration file /etc/nginx/nginx.conf test is successful
```

`nginx` コマンド (`/usr/sbin/nginx`) を認識している環境であれば、このコマンドの __`-t`__ オプションで確認することもできます。

```console
$ nginx -t
nginx: the configuration file /etc/nginx/nginx.conf syntax is ok
nginx: configuration file /etc/nginx/nginx.conf test is successful
```

