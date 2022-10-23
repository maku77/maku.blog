---
title: "Linuxコマンド: curl コマンドの使い方メモ（GET/POST リクエスト、ファイルのダウンロード）"
url: "/p/phdp2do"
date: "2015-04-03"
tags: ["Linux"]
---

`curl` コマンドは URL を指定してサーバー上のリソースにアクセスするためのコマンドで、Web API のテストや、ファイルのダウンロードに使用できます。
昔は `curl` コマンドは Linux 用のコマンドという認識でしたが、Windows 10 には標準搭載されるようになりました。
`curl` は様々なプロトコルで通信できますが、主に HTTP/HTTPS が使用されます。


curl コマンドでファイルをダウンロードする
----

curl コマンドはデフォルトではダウンロードしたファイルを標準出力に出力します。
__`-o`__（小文字のオー）オプションや、__`-O`__（大文字のオー）オプションを指定することで、__ファイルに保存する__ ことができます。

### 指定したファイル名で保存 (-o)

{{< code lang="console" >}}
# カレントディレクトリに sample.zip という名前で保存（この場合は後述の -O を使うと楽）
$ curl -L http://example.com/sample.zip -o sample.zip

# 保存先を絶対パスで指定
$ curl -L http://example.com/sample.zip -o /tmp/sample.zip
{{< /code >}}


### ダウンロード元と同じファイル名で保存 (-O)

{{< code lang="console" title="sample.zip という名前で保存" >}}
$ curl -L -O http://example.com/sample.zip
{{< /code >}}


curl コマンドで GET/POST リクエストを送信する
----

REST API などのテストに `curl` コマンドを利用できます。

### GET リクエスト

特にオプションを指定しなければ、HTTP の GET リクエストが送信されます。

{{< code lang="console" title="PokeAPI を実行してみる" >}}
$ curl https://pokeapi.co/api/v2/pokemon/ditto
...（JSON データが返ってくる）...
{{< /code >}}

### POST リクエスト

HTTP POST リクエストを送るには、__`--request POST`__ オプションでメソッドを明示するか、__`-d (--data)`__ オプションで Body 部分で送るデータを指定します（両方指定しても OK です）。

{{< code lang="console" title="POST リクエストの例" >}}
$ curl http://localhost:3000/books -d {}
{{< /code >}}

下記はより複雑なオプションを指定したリクエストの例です。
HTTP リクエストヘッダー (__`-H / --header`__) で API キー（アクセストークン）を指定することはよくあります。

{{< code title="GitHub API の例" >}}
$ curl --request POST \
     --url "https://api.github.com/repos/octocat/Spoon-Knife/issues" \
     --header "Accept: application/vnd.github+json" \
     --header "Authorization: Bearer YOUR-TOKEN" \
     --data '{
        "title": "Created with the REST API",
        "body": "This is a test issue created by the REST API"
     }'
{{< /code >}}


（おまけ）wget で POST リクエスト
----

`wget` コマンドの場合は次のように POST リクエストを送信できます。

```console
$ wget http://localhost:3000/books --post-data='{}' -O -
```

