---
title: "Nginx の設定: http でアクセスされた場合に https にリダイレクトする"
url: "/p/jowgstm"
date: "2019-01-10"
tags: ["nginx", "ssl"]
---

nginx サーバの SSL 対応（https 有効化）が終わった後は、http プロトコルによるアクセスは避けてもらいたいですね。
そのような場合は、下記のように設定しておけば、http でアクセスされた場合に、https で再度アクセスしてもらうようにクライアントに応答することができます（301 という値は、Moved Permanently を示す HTTP レスポンスコードです）。

{{< code lang="nginx" title="/etc/nginx/conf.d/xxx.conf" hl_lines="2-6" >}}
# http でアクセスされた場合
server {
    listen 80;
    server_name example.com;
    return 301 https://$host$request_uri;
}

# https でアクセスされた場合
server {
    server_name example.com;
    listen 443 ssl http2;       # IPv4
    listen [::]:443 ssl http2;  # IPv6

    ssl_certificate     /etc/nginx/ssl/example.com-2024.crt;
    ssl_certificate_key /etc/nginx/ssl/example.com-2024.key;
    ssl_prefer_server_ciphers on;  # サーバー側の暗号スイートを優先

    location / {
        root /usr/share/nginx/html;
        index index.html;
    }
}
{{< /code >}}

結果として、Web ブラウザから `http://example.com/` にアクセスすると、Web ブラウザは自動的に `https://example.com/` という URL でアクセスし直すことになります。

