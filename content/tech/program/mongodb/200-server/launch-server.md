---
title: "MongoDB サーバー (mongod) を起動する"
date: "2015-04-13"
lastmod: "2021-05-09"
url: "/p/3ucs8n3"
tags: ["MongoDB"]
weight: 1
---

MongoDB クライアント（`mongo` コマンドや Web アプリケーション）から MongoDB のデータベースにアクセスするには、あらかじめ MongoDB サーバー（`mongod`）を起動しておく必要があります。


MongoDB サーバーを起動する (mongod)
----

MongoDB サーバーは、単純にコマンドラインから __`mongod`__ と実行するだけで起動できます（[Windows のサービスとして起動](/p/tm4jzet) しておくことも可能です）。
`mongod` コマンドを実行するときに、__`--dbpath`__ オプションを使ってデータの格納先ディレクトリを指定することができます（デフォルトは __`/data/db`__）。
下記の例では、`mydata` ディレクトリをデータ格納先に指定しています。

{{< code lang="console" title="MongoDB サーバーの起動" >}}
$ mkdir mydata
$ mongod --dbpath mydata
{{< /code >}}


ログファイル名を指定する (--logpath, --logappped)
----

MongoDB サーバーのログはデフォルトで標準出力に出力されますが、__`--logpath`__ オプションを使って、出力先のログファイル名を指定することもできます。
さらに、__`--logappend`__ オプションを指定すると、これまでのログに追記される形で書き込まれます（このオプションを付けないと、ログファイルが上書きされてしまいます）。

{{< code lang="console" title="ログを log.txt に保存する" >}}
$ mongod --dbpath mydata --logpath log.txt --logappend
{{< /code >}}

`--dbpath` にはディレクトリ名を指定しますが、`--logpath` にはファイル名を指定することに注意してください。


ポート番号を指定する (--port)
----

`mongod` が使用するデフォルトのポート番号は __`27017`__ ですが、__`--port`__ オプションを使用して、任意のポート番号で起動することができます。
ポート番号を変更しておけば、1 台のホスト上で複数の `mongod` を立ち上げることができます。

{{< code lang="console" title="ポート番号 40001 で起動する" >}}
$ mongod --dbpath mydata --port 40001
{{< /code >}}

サーバー側のポート番号を変更した場合は、`mongo` クライアントを起動するときにも `mongo localhost:40001/dbname` や `--port 40001` のようにポート番号の指定が必要になることに注意してください。


MongoDB サーバー用の設定ファイル
----

### 設定を YAML ファイルに記述する

MongoDB サーバー用の起動オプションは、YAML 形式の設定ファイルに記述しておくことができます（YAML 形式は ver 2.6 から対応）。

{{< code lang="yaml" title="mongod.yml（mongod 用の設定ファイル）" >}}
storage:
   dbPath: data/db
systemLog:
   destination: file
   path: data/log/mongod.log
   logAppend: true
net:
   port: 40001
{{< /code >}}

{{< code title="設定ファイルを指定して起動する" >}}
$ mongod --config mongod.yml
{{< /code >}}

設定ファイルの詳細については下記の公式ドキュメントを参照してください。

{{< reference >}}
- [Configuration File Options — MongoDB Manual](https://docs.mongodb.com/manual/reference/configuration-options/)
{{< /reference >}}

### 起動用のスクリプトを作成する

いずれにしても、データ格納先のディレクトリ（上記の例では `data`）はあらかじめ作成しておく必要があるので、簡単な設定だけであれば、シェルスクリプトやバッチファイルで起動するようにしておいた方が楽かもしれません。

{{< code lang="shell" title="mongod-start.sh（Linux 用）" >}}
#!/bin/bash
PORT=40001
mkdir -p data/db
mkdir -p data/log
echo Start mongod on port $PORT ...
mongod --dbpath data/db --logpath data/log/mongod.log --logappend --port $PORT &
{{< /code >}}

{{< code lang="shell" title="mongod-stop.sh（Linux 用）" >}}
#!/bin/bash
killall mongod
{{< /code >}}

{{< code lang="bat" title="mongod-start.bat（Windows 用）" >}}
@echo off
md data\db > NUL 2>&1
md data\log > NUL 2>&1
echo Start mongod ...
mongod --dbpath data\db --logpath data\log\mongod.log --logappend --port 40001
{{< /code >}}

