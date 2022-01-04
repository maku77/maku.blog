---
title: "Nginx の設定: http でアクセスされた場合に https にリダイレクトする"
url: "/p/jowgstm"
date: "2019-01-10"
tags: ["nginx"]
---

nginx サーバの SSL 対応（https 有効化）が終わった後は、http プロトコルによるアクセスは避けてもらいたいですね。
そのような場合は、下記のように設定しておけば、http でアクセスされた場合に、https で再度アクセスしてもらうようにクライアントに応答することができます（301 という値は、Moved Permanently を示す HTTP レスポンスコードです）。

{{< code lang="apache" title="/etc/nginx/conf.d/xxx.conf" >}}
server {
    listen 80;
    server_name example.com;
    return 301 https://$host$request_uri;
}

server {
    listen 443 ssl http2;
    server_name example.com;
    # ...
}
{{< /code >}}

結果として、Web ブラウザから `http://example.com/` にアクセスすると、Web ブラウザは自動的に `https://example.com/` という URL でアクセスし直すことになります。

