---
title: "Docker で MongoDB サーバーを立ち上げる"
url: "p/y6zgrg2/"
date: "2023-01-18"
tags: ["Docker", "MongoDB"]
---

Docker のオフィシャルイメージとして、__`mongo`__ イメージが公開されています。

- [mongo - Official Image | Docker Hub](https://hub.docker.com/_/mongo)

このイメージには、MongoDB サーバー (`mongod`) や、MongoDB クライアント (`mongosh`) が含まれており、コマンドを省略してコンテナを起動すると、デフォルトで MongoDB サーバーが立ち上がります。


MongoDB サーバーのコンテナを起動する
----

次のように `mongo` コンテナを起動すると、ローカルホスト上で MongoDB サーバーを立ち上げたのと同様に振舞います。

```console
$ docker container run --rm -d -p 27017:27017 --name mongo mongo
```

オプションの意味:

- __`--rm`__ ... コンテナを停止したときにコンテナを削除します。コンテナを削除したくなければ、このオプションは外してください。
- __`-d`__ ... デーモンをバックグラウンドで動作させます。
- __`-p 27107:27017`__ ... ローカルホストの `27107` ポートへのアクセスを、コンテナ内の MongoDB サーバーの `27107` ポートへ転送します。
- __`--name mongo`__ ... 起動するコンテナに `mongo` という名前を付けます。
- __`mongo`__ ... Docker イメージとして `mongo:latest` を使用します（デフォルトで `:latest` が使われます)。

次のようにして、`mongo` コンテナが起動していることを確認できます。

```console
$ docker container ls
CONTAINER ID   IMAGE     COMMAND                  CREATED          STATUS          PORTS                                           NAMES
e5a2aeba6bf4   mongo     "docker-entrypoint.s…"   16 seconds ago   Up 14 seconds   0.0.0.0:27017->27017/tcp, :::27017->27017/tcp   mongo
```

コンテナを停止したいときは次のようにします。

```console
$ docker container stop mongo
```

次のような `docker-compose.yml` を作成しておけば、__`docker compose up -d`__ とするだけで起動できます（停止は __`docker compose down`__ で）。

{{< code lang="yaml" title="docker-compose.yml" >}}
version: "3"
services:
  mongo:
    image: mongo
    ports:
      - "27017:27017"
    restart: always
{{< /code >}}


MongoDB サーバーに接続する
----

ローカルホスト上の MongoDB クライアント (__`mongosh`__) を使って、上記のように起動した Docker コンテナの MongoDB サーバーに接続できます。
デフォルトで、`localhost:27017` に接続しに行くので、接続先アドレスを指定する必要はありません。

```console
$ mongosh mydb
...
mydb>
```

接続できました。
٩(๑❛ᴗ❛๑)۶ わーぃ


（おまけ）コンテナ間で通信させる
----

コンテナ内の MongoDB サーバーに、別のコンテナから接続したい場合は、それぞれのコンテナを同一のネットワークに接続しておくことで、コンテナ名でアクセスできるようになります。
ここでは、`mongo-net` というブリッジネットワークを作って、そこに各コンテナを接続してみます。

### docker コマンドを使う場合

__`docker network create`__ コマンドで、接続先のネットワーク `mongo-net` を作成しておきます。

{{< code lang="console" title="ネットワークの作成" >}}
$ docker network create mongo-net
{{< /code >}}

MongoDB サーバー用のコンテナを起動するときに、__`--net mongo-net`__ オプションを付けて、上記で作成したネットワークに接続します。
このとき、`-p` オプションによるポートフォワードの指定は必要ありません（ローカルホストへのアクセスをフォワードするわけではないので）。

{{< code lang="console" title="MongoDB サーバー用のコンテナを起動" >}}
$ docker container run --rm -d --net mongo-net --name mongo mongo
{{< /code >}}

次に、別のコンテナから MongoDB クライアント (`mongosh`) で接続します。
接続先コンテナの IP アドレスの代わりに、コンテナ名を使って `mongo:27017` というアドレスで接続します。
Docker イメージとしては、サーバーと同じ `mongo` イメージを使用できます。

{{< code lang="console" title="別のコンテナの mongosh で接続" >}}
$ docker container run --rm -it --net mongo-net mongo mongosh mongo:27017/mydb
...
mydb>
{{< /code >}}

### docker compose コマンドを使う場合

Docker Compose (`docker compose up -d`) を使う場合は、次のような Compose ファイルを使用します。
前述の通り、`ports` プロパティの指定は必要ありません。
Docker Compose はネットワークの作成まで同時にやってくれるので便利です。

{{< code lang="yaml" title="docker-compose.yml" >}}
version: "3"

services:
  mongo:
    image: mongo
    restart: always
    networks:
      - mongo-net

networks:
  mongo-net:
{{< /code >}}

{{< code lang="console" title="MongoDB サーバー用コンテナの起動" >}}
$ docker compose up -d
{{< /code >}}

MongoDB クライアントからの接続方法はほぼ同様ですが、ネットワーク名には、Docker Compose のプロジェクト名（親ディレクトリ名）がプレフィックスとして付けられているので注意してください。
例えば、親ディレクトリ名が `mongo` であれば、実際に生成されるネットワーク名は `mongo_mongo-net` になるので、次のように接続することになります。

{{< code lang="console" title="別コンテナの mongosh コマンドで接続" >}}
$ docker container run --rm -it --net mongo_mongo-net mongo mongosh mongo:27017/mydb
...
mydb>
{{< /code >}}

