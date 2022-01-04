---
title: "HTTP ヘッダに関するメモ"
url: "/p/3kkchub"
date: "2012-12-15"
tags: ["技術"]
---


プロキシ経由時の「リクエスト行」の URI について
----

プロキシサーバを介する場合は、リクエスト行に絶対アドレス指定で URI を指定する必要があります。
これは、リクエストを受け取ったプロキシサーバが、リクエストの転送先サーバを判断するためです。

```
Client
  ｜
  ｜ GET http://target.example.com/index.html HTTP/1.1
  ▽
Proxy server
  ｜
  ｜ GET /index.html HTTP/1.1
  ▽
target.example.com
```


HTTP1.1 の Host 指定
----

HTTP1.1 では、メッセージヘッダに `Host:` の指定が必須です。これはバーチャルホストの機能を実現するためです。
HTTP1.0 を使う場合は、`Host:` の指定は必要ありません。


HTTP のメッセージヘッダの種類
----

HTTP のメッセージヘッダには、大きく分けて、

* End-to-End
* Hop-by-Hop

の 2 種類があります。

