---
title: "Jadeメモ: jade コマンドの使い方"
url: "/p/onvso3a"
date: "2013-12-30"
tags: ["Jade"]
weight: 102
---

jade コマンドで HTML を生成する
----

コマンドラインから `jade` コマンドを実行するには下記のようにします。
いずれの場合も `index.jade` ファイルの内容から HTML を生成します。

```console
$ jade index.jade             # index.html を作成
$ jade < index.jade           # 標準出力へ HTML を出力
$ jade --pretty < index.jade  # 改行を追加して見やすく出力
```


テスト
----

実際に `index.jade` というファイルから、HTML を生成してみます。

{{< code lang="css" title="index.jade" >}}
doctype html
html
  body
    h1 Hello World
{{< /code >}}

{{< code lang="console" title="jade コマンドの実行" >}}
$ jade < index.jade
<!DOCTYPE html><html><body><h1>Hello World</h1></body>
{{< /code >}}

