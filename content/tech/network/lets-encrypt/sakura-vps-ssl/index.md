---
title: "Sakura VPS レンタルサーバーを Let's Encrypt で SSL 対応"
url: "/p/xkpjfcf"
date: "2019-01-10"
tags: ["Let's Encrypt", "nginx"]
---

今更ですが、さくらインターネットでレンタルしている Sakura VPS の Web サーバーを SSL (https) に対応しました。
とりあえず今回は無料の Let's Encrypt の証明書を、nginx サーバに設定します。

使っている OS が CentOS6 だったので、ついでに CentOS7 をクリーンインストールすることにしました。
といっても、さくらインターネットの VPS の設定画面でポチポチとやっていくだけ。
ここで、**[public] CentOS_LetsEncrypt** というスタートアップスクリプトを選んだら、自動的に SSL の証明書などの対応が完了します。
入力するパラメータはドメイン（`example.com` など）と連絡用のメールアドレスだけ。

{{< image w="400" border="true" src="sakura-vps-ssl.png" >}}

Web サーバの nginx も自動的にインストールされて、Web サーバにアクセスできる状態で起動するのですが、nginx の設定は少しだけ調整する必要があります（ドキュメントルートなど）。

数分で OS の再インストールが完了し、下記のような nginx の設定ファイルが自動生成されていました（ドメイン名は `example.com` に置換してあります）。
SSL の設定も完了した状態になっています。

{{< code title="/etc/nginx/conf.d/https.conf" >}}
map $http_upgrade $connection_upgrade {
    default upgrade;
    ''      close;
}
server {
    listen 443 ssl http2;
    server_name example.com;

    location / {
        root   /usr/share/nginx/html;
        index  index.html index.htm;
    }

    ssl_protocols TLSv1.2;
    ssl_ciphers EECDH+AESGCM:EECDH+AES;
    ssl_ecdh_curve prime256v1;
    ssl_prefer_server_ciphers on;
    ssl_session_cache shared:SSL:10m;

    ssl_certificate /etc/letsencrypt/live/example.com/fullchain.pem;
    ssl_certificate_key /etc/letsencrypt/live/example.com/privkey.pem;

    error_page   500 502 503 504  /50x.html;
    location = /50x.html {
        root   /usr/share/nginx/html;
    }
}
{{< /code >}}

HTML ファイルの格納ディレクトリ（ドキュメントルート）が、デフォルトでは上記のように `/usr/share/nginx/html` になっているので、ここを適切なディレクトリパス（`/home/maku/website` など）に置き換えてやれば完了です。

```
# 設定ファイルを編集
$ sudo vim /etc/nginx/conf.d/https.conf

# 設定を反映
$ sudo service nginx reload
```

https://example.com/ など https でアクセスできれば OK です。

「403 Forbidden」が出るときは、[こちら](/p/ew5zmmw)をチェック。

