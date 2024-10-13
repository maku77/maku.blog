---
title: "Nginx の設定: Nginx でバーチャルホストを設定する（リバースプロキシ）"
url: "p/q8tw6c4/"
date: "2013-12-03"
tags: ["nginx"]
---

- （参考）[nginx-proxy コンテナで簡単にリバースプロキシを立ち上げる](/p/kos367z/)

アクセス時のドメイン名により別の Web サーバに処理をフォワードする
----

例えば、`http://test.example.com/` というアドレス（80番ポート）で nginx サーバにアクセスしてきたときに、`localhost:7000` で動作している Web サーバに処理を委譲するには、下記のように設定します。

{{< code lang="nginx" title="/etc/nginx/conf.d/test.example.com.conf" >}}
server {
    listen 80;
    server_name test.example.com;
    location / {
        proxy_pass http://localhost:7000/;
    }
}
{{< /code >}}

この機能を利用することで、一つの PC 内に複数の Web サーバを（異なるポート）で立ち上げておき、アクセスしてきた URL（のドメイン名）によって処理を振り分けることができます。
例えば、次のように Web サーバへのアクセスをフォワードできます。

- http://**aaa**.example.com/ でアクセスされた場合 → localhost:**7000** で処理
- http://**bbb**.example.com/ でアクセスされた場合 → localhost:**7100** で処理
- http://**ccc**.example.com/ でアクセスされた場合 → localhost:**7200** で処理

それぞれのドメインは同じ IP アドレスにマッピングされており、物理的には 1 つのマシンに対してアクセスが発生するのですが、クライアント（Web ブラウザのユーザ）から見ると、あたかも異なるホストにアクセスしているかのように見えます（ドメイン名が異なるので）。
バーチャルなホストが存在しているような動作をするということで、このような Web サーバの機能を**バーチャルホスト**と呼びます。

また、このように、サーバ環境側（ここでは 80 ポートのサーバ）でいったんアクセスを受けておいて、アドレス等を見て別の内部サーバ（ここでは 7000 ポートで待ち受けているサーバなど）に処理をフォーワードする仕組みのことを、**リバースプロキシ**と呼びます。

リバースプロキシというのは Web サーバのためだけの仕組みではなく、より一般的なサーバの仕組みを指す名前です。
nginx や Apache などの Web サーバのバーチャルホスト機能は、リバースプロキシの仕組み（概念）を利用して実現されているということです。


アクセス時のドメイン名によりドキュメントルートを切り替える
----

上記の例では、クライアントからのアクセスを、別々の Web サーバ（例えば同じ PC 内の別のポートで待ち受けている Web サーバ）に振り分ける例でしたが、Web サーバは 1 つだけで、使用するコンテンツファイルのルートだけを切り替えるという方法もあります。

下記の設定例では、`aaa.example.com` でアクセスされた場合と、`bbb.example.com` でアクセスされた場合に使用するドキュメント（html ファイル）のルートディレクトリを切り替えています。

{{< code lang="nginx" title="/etc/nginx/conf.d/example.com.conf" >}}
server {
    listen 80;
    server_name aaa.example.com;
    location / {
        root /home/maku/website/aaa/public;
        index index.html index.htm;
    }
}

server {
    listen 80;
    server_name bbb.example.com;
    location / {
        root /home/maku/website/bbb/public;
        index index.html index.htm;
    }
}
{{< /code >}}

ここでは設定を 1 つの `example.com.conf` ファイルにまとめて記述していますが、`aaa.example.com.conf` と `bbb.example.com.conf` の 2 つに分けても大丈夫です（参考: [Nginx の設定ファイル (.conf) の場所](/p/pms426x/)）。
この設定により、クライアントからのアクセス時に指定された　URL によって、下記のように参照するファイルが切り替わります。

- http://**aaa**.example.com/ でアクセスされた場合 → /home/maku/**website/aaa**/public/index.html
- http://**bbb**.example.com/ でアクセスされた場合 → /home/maku/**website/bbb**/public/index.html

