---
title: "HTTP/1.1 の Chunked Transfer Encoding を理解する"
url: "p/ugkqy8z/"
date: "2024-03-08"
tags: ["Deno", "HTTP"]
---

Chunked Transfer Encoding (Coding) とは
----

HTTP/1.1 では [`Transfer-Encoding: chunked`](https://datatracker.ietf.org/doc/html/rfc9112#section-7.1) というレスポンスヘッダーを返すことにより、本文（ペイロード）部分を少しずつ返すことができるようになっています。
この仕組みを使うと、クライアントが最初のデータを受信するまでの時間 (TTFB: Time To First Byte) を短くすることができます。

例えば、次の HTTP レスポンスの例では、`Hello` と `World` というテキストを分けて送っています（これらをチャンクと呼んでいます）。
各行末は `\r\n` で終わっていると考えてください。

{{< code title="Transfer-Encoding: chunked のレスポンス例" >}}
HTTP/1.1 200 OK
Content-Type: text/plain
Transfer-Encoding: chunked

5
Hello
5
World
0
　
{{< /code >}}

各チャンクは、サイズ（16進数文字列）とデータが __`<サイズ>\r\n<データ>\r\n`__ というペアの形で送られます。
そして、最後はサイズ 0 のチャンク (`0\r\n\r\n`) を送ることで、すべてのデータの送信が完了したことを示します。

HTTP/1.1 のレスポンスには、この `Transfer-Encoding: chunked` ヘッダーか、[`Content-Length`](https://datatracker.ietf.org/doc/html/rfc9112#name-content-length) ヘッダーのいずれか一方を含む必要があります。
本文全体を一度に送る場合は、`Content-Length` ヘッダーで本文全体のサイズ（オクテット数）を示します。

{{% note title="なぜサイズ情報が必要なのか？" %}}
`Transfer-Encoding: chunked` と `Content-Length` のどちらを使う場合も、本文のデータサイズを送ることが前提となっています。
これは、クライアント側で一連のデータの受信を完了したことを判断できるようにするためです。

HTTP 接続では半二重通信が行われるため、1 つのリクエストのやりとりがすべて完了するまで、次のリクエストを送ることができません。
クライアントでデータの受信完了タイミングを判断できれば、次のリクエスト用にその接続を使いまわすことができます (`Connection: keep-alive`)。

もちろん、サーバー側から接続を切ることでもデータの送受信を止めることができますが、次のリクエストのために TCP 接続を使いまわすことができなくなってしまいます。
新しく TCP 接続を確立するには、3-way ハンドシェイクという 3 回の通信を行わなければいけないため、できれば TCP 接続は使いまわすような仕組みの方が効率がよいのです。
{{% /code %}}


Chunked Transfer Encoding なレスポンスを返すサーバー
----

`Transfer-Encoding: chunked` なレスポンスを返すサーバーの Deno での実装例が公開されています。

- (Code) https://examples.deno.land/http-server-streaming
- (Playground) https://dash.deno.com/playground/example-streaming

このサーバーは Deno Deploy 上で公開されているので、Web ブラウザーか `curl` コマンドで次のようにアクセスすると、1 秒おきに文字列を受信できます（現在時刻を表す文字列になっています）。
`curl` の __`-i`__ オプションを付けてレスポンスヘッダーを見ると、`transfer-encoding: chunked` が含まれていることを確認できます。

{{< code lang="console" title="curl コマンドでの HTTP stream 受信" hl_lines="6" >}}
$ curl -i https://example-streaming.deno.dev
HTTP/1.1 200 OK
content-type: text/plain
x-content-type-options: nosniff
vary: Accept-Encoding
transfer-encoding: chunked
date: Fri, 08 Mar 2024 07:28:13 GMT
via: http/1.1 edgeproxy-h
server: deno/gcp-asia-northeast1

It is 2024-03-08T07:28:14.180Z
It is 2024-03-08T07:28:15.181Z
It is 2024-03-08T07:28:16.182Z
...
{{< /code >}}

デフォルトでは、上記のようにチャンクのデータ部分しか表示されませんが、`curl` の __`--raw`__ オプションを付けると、チャンクサイズの部分も表示してくれます。

```console
$ curl --raw https://example-streaming.deno.dev
1F
It is 2024-03-08T07:28:14.180Z

1F
It is 2024-03-08T07:28:15.181Z

1F
It is 2024-03-08T07:28:16.182Z
```

`1F` というサイズは 16 進数表記なので、10 進数だと 31 オクテット（≒ 31bytes）ということですね。
上記テキストの場合は「30 文字＋改行(`\n`)」でちょうど 31 オクテットになっています。

理解したっ ٩(๑❛ᴗ❛๑)۶ わーぃ
