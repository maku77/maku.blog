---
title: "Docker コンテナで MySQL サーバーを起動する"
url: "p/48hqx6d/"
date: "2022-07-01"
tags: ["Docker", "MySQL"]
draft: true
---

{{% private %}}
- [MySQL :: MySQL 8.0 リファレンスマニュアル :: Docker での MySQL Server のデプロイに関するその他のトピック](https://dev.mysql.com/doc/refman/8.0/ja/docker-mysql-more-topics.html#docker-optimized-installation)
{{% /private %}}


Docker コンテナで MySQL サーバーを起動する
----

Docker 環境がインストールされていれば、Docker Hub の [mysql オフィシャルイメージ](https://hub.docker.com/_/mysql) を使って、簡単に MySQL サーバーを立ち上げることができます。

{{< code lang="console" title="MySQL ver 8 を起動する" >}}
$ docker run --rm --name test-mysql -d -e MYSQL_ROOT_PASSWORD=my-secret-pw mysql:8
{{< /code >}}

外部の PC から接続するなら、`-p` オプションでホストマシンのポートと接続します。

```console
$ docker run --rm --name test-mysql -d -p 3306:3306 -e MYSQL_ROOT_PASSWORD=my-secret-pw mysql:8
```

オプションの説明:

- `--rm` ... コンテナを停止したらコンテナを自動で削除します。
- `--name test-mysql` ... コンテナに `test-mysql` という名前を付けます。
- `-d (--detach)` ... バックグラウンドで MySQL デーモンを動作させます（参考: [Docker コンテナをデーモンとして動作させる](https://maku77.github.io/p/dmpsvz3/)）。
- `-p 3306:3306` ... ホストマシンの 3306 ポートへのアクセスを、コンテナの 3306 ポートにフォワードします。
- `-e MYSQL_ROOT_PASSWORD=my-secret-pw` ... 環境変数 `MYSQL_ROOT_PASSWORD` にパスワードを設定しています。

{{% note %}}
Apple M1 環境 で `docker: no matching manifest for linux/arm64/v8` といったエラーが出る場合は、`--platform linux/x86_64` オプションを付けて実行します。
{{% /note %}}

次のようにしてちゃんと MySQL サーバーが起動しているか確認できます。

```console
% docker container ls
CONTAINER ID   IMAGE     COMMAND                  CREATED          STATUS          PORTS                 NAMES
47b6ff66465d   mysql:8   "docker-entrypoint.s…"   13 seconds ago   Up 12 seconds   3306/tcp, 33060/tcp   test-mysql
```

