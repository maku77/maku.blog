---
title: "Docker コンテナで Nginx サーバーを立ち上げる"
url: "p/99qo8zf/"
date: "2022-07-13"
tags: ["Docker", "nginx"]
---

何をするか？
----

VPS などで Web アプリをホスティングする場合、各種サーバーを Docker コンテナとして立ち上げるようにすると OS 環境をクリーンに保てます。
特に、1 つのホスト（VPS サーバー）で複数の Web アプリを提供するような場合は、各アプリをコンテナで構成することで、関係ないアプリの設定が混ざってしまうのを防げます。
もちろん、Azure Container Instances や AWS Fargate といったコンテナ実行用のクラウドサービスを使えば、より独立した環境を構築できるのですが、これらのサービスは個人が趣味で使うにはまだまだ高価なので、VPS などの環境で Docker コンテナを立ち上げることには価値があります。

ここでは、Nginx サーバーを Docker コンテナとして立ち上げる方法を示します。
Docker Hub で公開されている [Nginx イメージ](https://hub.docker.com/_/nginx) は、デフォルトでコンテナ内の __`/usr/share/nginx/html`__ ディレクトリに配置されたコンテンツを公開するようになっています。
大きく分けて、次の 2 つのいずれかの方法で簡単にコンテンツを公開できます。

- bind マウントで Docker ホスト側のコンテンツを参照する方法
- コンテンツを含んだコンテナイメージを作成する方法

以下、それぞれの方法を順番に見ていきます。


bind マウントで Docker ホスト側のコンテンツを参照する方法
----

Nginx のコンテナを起動するときに、Docker ホスト側のコンテンツディレクトリを bind マウントして、コンテナの `/usr/share/nginx/html` ディレクトリとして参照できるようにする方法です。

まず、簡単なコンテンツファイルとして次のような HTML ファイルを用意しておきます。

{{< code lang="html" title="public/index.html" >}}
<html>Hello</html>
{{< /code >}}

あとは、`docker container run` コマンドで nginx コンテナを起動するだけです。

```console
$ docker container run --rm -d -p 8000:80 -v "$(pwd)/public":/usr/share/nginx/html --name web nginx
```

各引数は次のような意味を持っています。

- __`--rm`__ ... コンテナ停止時にコンテナを削除します。
- __`-d (--detach)`__ ... コンテナをバックグラウンドで動作させることで、現在の端末を引き続き利用できるようにします。
- __`-p 8000:80`__ ... ホスト側のポート 8000 へのアクセスをコンテナのポート 80 へ転送します。これにより、`http://localhost:8000` で Web サイトにアクセスできます。
- __`-v ...`__ ... ホスト側のカレントディレクトリにある `public` ディレクトリを、コンテナ側から `/usr/share/nginx/html` として見えるようにバインドマウントします。バインド時のパスは絶対パスで記述しないといけないので、Linux シェルの機能の `$(pwd)` を使って、カレントディレクトリの絶対パスを取得しています。ちなみに `-v` オプションは旧式のやり方で、Docker 公式で推奨されている `--mount` オプションを使うと次のようになります（バインドマウントであることが明確です）。
  ```
  --mount type=bind,src="$(pwd)/public",dst=/usr/share/nginx/html
  ```
- __`--name web`__ ... 起動するコンテナに `web` という名前を付けます。必須ではありませんが、名前が付いていると `docker container stop web` でコンテナ停止できたりして便利です。
- __`nginx`__ ... Docker Hub から `nginx:latest` イメージを取得するよう指定しています。本番環境では、`nginx:1.23` のようにバージョンタグまで指定した方がよいでしょう。

無事コンテナが起動したら、次のように動作中であることを確認できます。

```console
$ docker container ls
CONTAINER ID  IMAGE  COMMAND                  CREATED         STATUS        PORTS                                  NAMES
dd4b471044b1  nginx  "/docker-entrypoint.…"   2 minutes ago  Up 2 minutes  0.0.0.0:8000->80/tcp, :::8000->80/tcp  web
```

あとは、Web ブラウザや `curl` コマンドで `http://localhost:8000` にアクセスできれば成功です！

```console
$ curl localhost:8000
<html>Hello</html>
```

最後にコンテナを停止して後片付けします。

```console
$ docker container stop web
```


コンテンツを含んだイメージを作成する方法
----

コンテンツディレクトリの内容を埋め込んだコンテナイメージを作成する方法です。
ここでも、上記と同様に `public/index.html` というコンテンツファイルを用意します。
イメージを作成するための `Dockerfile` を次のような感じで作成します。

{{< code lang="docker" title="Dockerfile" >}}
FROM nginx:1.23
COPY public /usr/share/nginx/html
{{< /code >}}

`Dockerfile` をビルドして、`web` という名前のイメージを作成します。

```console
$ docker image build -t web .
```

イメージが作成できているか確認します。

```console
$ docker image ls
REPOSITORY   TAG       IMAGE ID       CREATED         SIZE
web          latest    1e2497edf7e0   3 minutes ago   142MB
nginx        1.23      41b0e86104ba   30 hours ago    142MB
```

あとは、このイメージでコンテナを起動すれば OK です。
今回は、`nginx` イメージではなく、独自に作成した `web` イメージを使って起動することに注意してください（最後の部分が `nginx` から `web` に変わっています）。

```console
$ docker container run --rm -d -p 8000:80 --name web web
```

先ほどと同様に、`http://localhost:8000` にアクセスできれば成功です。
このやり方は、作成したイメージをそのまま別の Docker 環境で実行できるというポータビリティがありますが、コンテンツを変更したときはイメージの再ビルドが必要になります。

最後にコンテナを停止してお片付けしておきます。

```console
$ docker container stop web
```


（おまけ）Docker Compose を使う
----

[Docker Compose を使う](https://maku77.github.io/p/qm5k2hx/) と、`docker container run ...` の長いコマンドを入力せずに `docker compose up` という短いコマンドでコンテナを起動できるようになります。
また、Docker Compose はイメージのビルド機能も備えており、`Dockerfile` を用いたイメージのビルドからコンテナの起動までをワンステップで実行できます。

バインドマウントを使う場合の Compose ファイル (`docker-compose.yml`) は次のような感じで作成します。
内容は `docker container run` で指定したオプションとほぼ同様なので、簡単に理解できると思います。

{{< code lang="yaml" title="docker-compose.yml（nginx イメージをそのまま使う場合）" >}}
version: "3"

services:
  web:
    image: nginx:1.23
    volumes:
      - ./public:/usr/share/nginx/html
    ports:
      - "8000:80"
{{< /code >}}

あとは、この `docker-compose.yml` ファイルがあるディレクトリで、次のように実行すればコンテナを起動できます（`-d` オプションを付けるとバックグラウンド実行になります）。

```console
$ docker compose up -d
```

コンテナを停止＆削除するときは、次のようにします（`down` の代わりに `stop` を使うと、コンテナの停止だけで削除されません）。

```console
$ docker compose down web
```

`Dockerfile` からイメージをビルドして、そのイメージでコンテナ起動する場合は、Compose ファイルを次のように記述します。
イメージ名を指定する `image: nginx:1.23` の代わりに __`build: .`__ を指定して、カレントディレクトリの `Dockerfile` をビルドするように指示しています。

{{< code lang="yaml" title="docker-compose.yml（Dockerfile からイメージをビルドする場合）" >}}
version: "3"

services:
  web:
    build: .
    ports:
      - "8000:80"
{{< /code >}}

あとは、同様に次のように実行すれば、イメージのビルドからコンテナの起動まで一気に終わります。ステキ！

```console
$ docker compose up -d
```

{{% reference %}}
- [nginx-proxy コンテナで簡単にリバースプロキシを立ち上げる](/p/kos367z/)
{{% /reference %}}

