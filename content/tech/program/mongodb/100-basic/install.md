---
title: "MongoDB (mongo, mongod) をインストールする"
url: "/p/jyn7v9y"
date: "2013-12-28"
lastmod: "2021-05-08"
tags: ["MongoDB"]
weight: 3
---

各種 OS 用のパッケージでインストールする
----

MongoDB のクライアント (__`mongo`__) やサーバー (__`mongod`__)、その他の各種コマンド（`mongoexport` や `mongostat` など） は、下記のサイトから各種 OS 用のバイナリをダウンロードしてインストールすることができます。

- [MongoDB Community Download | MongoDB](https://www.mongodb.com/try/download/community)

Windows 用のインストーラを使うと、簡単に `mongod` を Windows サービスとして登録したりすることができます（[手動でサービス登録する](/p/tm4jzet)こともできます）。
インストールして、`bin` ディレクトリへのパスが通ったら、`mongo` コマンドを実行できるか確認しておきましょう。

{{< code lang="console" title="mongo コマンドを実行できるか確認" >}}
$ mongo --version

MongoDB shell version v4.0.9
git version: fc525e2d9b0e4bceff5c2201457e564362909765
allocator: tcmalloc
modules: none
build environment:
    distmod: 2008plus-ssl
    distarch: x86_64
    target_arch: x86_64
{{< /code >}}


macOS なら Homebrew でのインストールが楽
----

macOS の場合、[Homebrew](https://brew.sh/index_ja) を使うと簡単に MongoDB のコマンド群（`mongo` や `mongod`）をインストールできます。

{{< code lang="console" title="mongodb-community パッケージのインストール" >}}
$ brew tap mongodb/brew  # MongoDB 用のリポジトリ情報を追加
$ brew install mongodb-community  # MongoDB 関連コマンドをインストール
$ brew upgrade mongodb-community  # （バージョンアップしたいとき）
{{< /code >}}

これで、`/usr/local/bin` に以下のようなコマンド群がインストールされます（正確にはこれらはシンボリックリンクであり、実体は `/usr/local/Cellar/mongodb-*` 以下にインストールされています）。

```
/usr/local/bin/mongo
/usr/local/bin/mongod
/usr/local/bin/mongodump
/usr/local/bin/mongoexport
/usr/local/bin/mongofiles
/usr/local/bin/mongoimport
/usr/local/bin/mongorestore
/usr/local/bin/mongos
/usr/local/bin/mongostat
/usr/local/bin/mongotop
```

{{% note title="古い mongodb パッケージを使用している場合" %}}
2019 年に Homebrew の Core リポジトリから `mongodb` パッケージが削除されました。
代わりに 3rd party リポジトリ (`mongodb/brew`) から、`mongodb-community` パッケージが配信されています。
以前の `mongodb` パッケージがインストールされている場合は、__`brew uninstall mongodb`__ でアンインストールして、`mongodb-community` の方を使うようにしてください。
{{% /note %}}

{{< code lang="console" title="動作確認" >}}
$ mongo --version
MongoDB shell version v4.4.5
...

$ mongod --version
db version v4.4.5
...
{{< /code >}}

