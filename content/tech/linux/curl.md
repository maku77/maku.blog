---
title: "Linuxコマンド: curl コマンドの使い方メモ"
url: "/p/phdp2do"
date: "2015-04-03"
tags: ["Linux"]
---

昔は `curl` コマンドは Linux 用のコマンドという認識でしたが、Windows 10 には標準搭載されるようになりました。


curl コマンドでファイルをダウンロードする
----

curl コマンドはデフォルトではダウンロードしたファイルを標準出力に出力します。
__`-o`__（小文字のオー）オプションや、__`-O`__（大文字のオー）オプションを指定することで、ファイルに保存することができます。

### 指定したファイル名で保存 (-o)

{{< code >}}
$ curl -L http://example.com/sample.zip -o sample.zip （相対パスで指定）（この場合は -O のが早い）
$ curl -L http://example.com/sample.zip -o /tmp/sample.zip （絶対パスで指定）
{{< /code >}}


### ダウンロード元と同じファイル名で保存 (-O)

{{< code >}}
$ curl -L -O http://example.com/sample.zip
{{< /code >}}


curl コマンドで POST リクエストを送信する
----

`curl` コマンドは、ファイルのダウンロードだけではなく、POST コマンドの送信にも使用できます。
REST API を提供する Web アプリのテストなどに利用できます。

{{< code >}}
$ curl http://localhost:3000/books -d {}
{{< /code >}}

ちなみに、`wget` コマンドの場合は次のように POST リクエストを送信できます。。

{{< code >}}
$ wget http://localhost:3000/books --post-data='{}' -O -
{{< /code >}}

