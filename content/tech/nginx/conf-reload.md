---
title: "Nginx の設定: Nginx の設定ファイルの変更を反映する (nginx reload)"
url: "/p/6aazmy3"
date: "2015-09-20"
tags: ["nginx"]
---

nginx の設定ファイル (`/etc/nginx/nginx.conf` や `/etc/nginx/conf.d/*.conf`) の内容を変更した場合は、次のように __`nginx reload`__ で再読み込みします。

```console
$ sudo service nginx reload      # 方法 1
$ sudo /etc/init.d/nginx reload  # 方法 2
```

nginx の起動はとても速いので、単純な Web コンテンツだけを提供しているの Web サーバであれば、__`nginx restart`__ でサービス自体を再起動してしまってもよいでしょう。

```console
$ sudo service nginx restart      # 方法 1
$ sudo /etc/init.d/nginx restart  # 方法 2
```

