---
title: "Nginx の設定: Nginx の設定ファイルの変更を反映する（再読み込み）"
url: "/p/6aazmy3"
date: "2015-09-20"
tags: ["nginx"]
---

nginx の設定ファイル (`/etc/nginx/nginx.conf` や `/etc/nginx/conf.d/*.conf`) の内容を変更した場合は、下記のようにして反映することができます。

```
$ sudo service nginx reload
$ sudo /etc/init.d/nginx reload
```

nginx の起動はとても速いので、単純な Web コンテンツだけを提供しているの Web サーバであれば、nginx サービス自体を再起動してしまってもよいでしょう。

```
$ sudo service nginx restart
$ sudo /etc/init.d/nginx restart
```

