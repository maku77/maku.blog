---
title: "Go アプリを実行する軽量な Docker コンテナイメージを作成する"
url: "p/wbtbr8o/"
date: "2022-10-02"
tags: ["Docker", "Golang"]
---

{{% private %}}
- [Build your Go image | Docker ドキュメント](https://matsuand.github.io/docs.docker.jp.onthefly/language/golang/build-images/)
{{% /private %}}

何をするか？
----

Go 言語 (Golang) で簡単な Web サーバーを作成して、それを動かす軽量な Docker コンテナイメージを作成します。
`Dockerfile` には __マルチステージのビルド構成__ を適用し、Go 言語アプリのビルドと、実行イメージのビルドのステージを分けます。
実行用のコンテナイメージとしては、__Alpine Linux__ ベースと、__scratch__ ベースの 2 種類のイメージを作成してみます。

Golang は軽量なシングルバイナリを生成するのに適した言語で、Docker イメージの生成にも向いています。
Node.js などでイメージを作ろうとすると、Hello World でも 100MB 超えになってしまいますが、Golang を使えば、その 1/10 程度のサイズのイメージを生成できます。
軽量のイメージを作れるようになると、頻繁なビルドとデプロイを気兼ねなく行えるようになります。

Golang アプリを準備する
----

Golang で作るアプリは何でもよいのですが、ここでは Golang 標準の `net/http` パッケージを使って、`Hello World` というレスポンスを返すだけの簡単な Web サーバーアプリを用意します。

まずは、お馴染みの `go.mod` の作成から。

```console
$ mkdir hello && cd hello
$ go mod init hello
```

あとは、次のような `main.go` ファイルを作成すれば完成です。

{{< code lang="go" title="main.go" >}}
package main

import (
	"log"
	"net/http"
)

func main() {
	http.HandleFunc("/", func(w http.ResponseWriter, _ *http.Request) {
		w.Write([]byte("Hello World"))
	})
	log.Fatal(http.ListenAndServe(":8080", nil))
}
{{< /code >}}

__`go run .`__ でサーバーを起動して、Web ブラウザーなどで `http://localhost:8080` にアクセスすれば、`Hello World` というレスポンスが返ってくることを確認できます。

{{% maku-common/reference %}}
- [Golang で HTTP サーバーを作成する (net/http, rs/cors) - まくまく Golang ノート](https://maku77.github.io/p/goruwy4/)
{{% /maku-common/reference %}}


Alpine Linux ベースの実行イメージを作成する
----

{{< code lang="docker" title="Dockerfile" >}}
# syntax=docker/dockerfile:1

###
### Build stage （アプリをビルドするための 1st ステージ）
###
FROM golang:1.19-alpine AS build
WORKDIR /work
# 外部モジュールを使い始めたら下記を追加
# COPY go.mod go.sum ./
# RUN go mod download
COPY . .
RUN GOOS=linux GOARCH=amd64 go build -ldflags '-w -s' -o hello

###
### Deploy stage （実行用のイメージをビルドするための 2nd ステージ）
###
FROM alpine:3.16
COPY --from=build /work/hello /app/hello
CMD ["/app/hello"]
{{< /code >}}

ここでは上記のような `Dockerfile` を作成してイメージをビルドします。
Go アプリをビルドするための build ステージと、実行用の Docker イメージをビルドするための deploy ステージに分かれています。
このようなマルチステージのビルド構成にすると、最終的にできる実行用のイメージに Go アプリのビルド環境を入れなくてすむので、イメージサイズを小さくすることができます。
最終的に生成される実行イメージはコンパクトな Alpine Linux をベースとしており、これもイメージサイズの削減につながっています。
以下、2 つのステージを順に見ていきます。

### アプリをビルドするための 1st ステージ

```docker
FROM golang:1.19-alpine AS build
WORKDIR /work
COPY . .
RUN GOOS=linux GOARCH=amd64 go build -ldflags '-w -s' -o hello
```

1st ステージでは、Golang のビルド環境で `main.go` ファイルをビルドします。
ここでは親イメージに Alpine Linux ベースの Golang ビルド環境である __`golang:1.19-alpine`__ を指定していますが、最新のタグは、[Docker Hub の golang イメージ](https://hub.docker.com/_/golang) のページで確認してください。
処理内容は単純で、ホスト側のファイルをコンテナ側へコピー (`COPY . .`) して、そこに含まれる `main.go` ファイルをビルドしているだけです。
ワーキングディレクトリを `WORKDIR /work` で移動しているので、ビルド後の実行ファイルのパスは `/work/hello` になります。

{{% note title="go build の -ldflags について" %}}
`go build` 時に __`-ldflags '-s -w'`__ オプションを指定すると、デバッグ用のシンボル情報を除いて実行ファイルのサイズを小さくできます（20％程度？）。
このフラグは、内部的には [go tool link](https://pkg.go.dev/cmd/link) に渡されます。

```console
# 通常ビルドした場合
$ go build -o hello
$ du -h hello
5.9M	hello

# デバッグシンボル情報を削った場合
$ go build -o hello -ldflags '-s -w'
$ du -h hello
4.4M	hello
```
{{% /note %}}

この 1st ステージには、`FROM` 命令の __`AS build`__ オプションで `build` というエイリアス名を付けています。
以下の 2nd ステージからは、このエイリアス名を使って 1st ステージで生成したファイルを参照できます。

### 実行用のイメージをビルドするための 2nd ステージ

```docker
FROM alpine:3.16
COPY --from=build /work/hello /app/hello
CMD ["/app/hello"]
```

2nd ステージでは、1st ステージで生成した Golang 製のアプリ（`/work/hello`) を実行するための Docker イメージをビルドします。
このイメージには、もう Golang のビルド環境は必要ないので、軽量 Linux である Alpine Linux を親イメージとして指定します（5MB くらい！）。
ここでは、__`alpine:3.16`__ を指定していますが、最新のタグは [Docker Hub の alpine イメージ](https://hub.docker.com/_/alpine) のページで確認してください。
`COPY` 命令では、__`--from=build`__ 指定により、1st ステージ (`build`) の `/work/hello` ファイルを、2nd ステージの `/app/hello` へコピーしています。
最後の `CMD` 命令で、コンテナ起動時に `/app/hello` を起動するように指定しています。

### イメージのビルド

`Dockerfile` の作成が終わったら、__`docker image build`__ コマンドでビルドして Docker イメージを作成します。
ここではイメージ名を `hello` としています。

```console
$ docker image build -t hello .
```

イメージのビルドが完了したら、ちゃんとできているか確認します。

```console
$ docker image ls
REPOSITORY     TAG       IMAGE ID       CREATED          SIZE
hello          latest    b8e4afa8a2a3   3 seconds ago    9.75MB
```

Docker イメージの完成です！
イメージサイズは 10MB 弱なので、まぁまぁコンパクトなイメージになっています。
これは、Go アプリと Alpine Linux の合計サイズです。
次のようにすれば、このイメージからコンテナを起動できます。

{{< code lang="console" title="コンテナの起動" >}}
$ docker container run --rm -p 8080:8080 hello
{{< /code >}}

__`-p 8080:8080`__ オプションで、ホスト PC の 8080 ポートを、コンテナ内の 8080 ポートに転送しているので、ホスト PC 上の Web ブラウザーで `http://localhost:8080` を開けばアクセスできます。
コンテナは <kbd>Ctrl + C</kbd> で停止できます。
起動時に `--rm` オプションを指定しておいたので、コンテナは停止と同時に削除されます。


Scratch ベースの実行イメージを作成する
----

上記では、Alpine Linux + Go アプリという構成のイメージを作成しましたが、__scratch__ イメージというのを親イメージとして指定すると、OS を含まない Go アプリだけのイメージを作成できます。
正確には、カーネル自体は Docker ホスト側のものが使われるので、その機能だけで動作させられる実行ファイルであれば、scratch ベースのイメージにすることができます。
Linux の各種機能（ライブラリやシェル）が使えなくなるので、トラブル発生時の調査などが難しくなりますが、__非常に軽量なイメージ__ を作成することができます。

{{< code lang="docker" title="Dockerfile" >}}
# syntax=docker/dockerfile:1

### Build stage
FROM golang:1.19-alpine AS build
WORKDIR /work
# 外部モジュールを使い始めたら下記を追加
# COPY go.mod go.sum ./
# RUN go mod download
COPY . .
RUN GOOS=linux GOARCH=amd64 go build -ldflags '-w -s -extldflags "-static"' -o hello

### Deploy stage
FROM scratch
EXPOSE 8080
COPY --from=build /work/hello /hello
CMD ["/hello"]
{{< /code >}}

scratch イメージを使用するときに注意しなければいけないのは、Go アプリをビルドするときに __外部ライブラリを静的リンク__ しておく必要があるということです（ライブラリがまったく入っていないので）。
この設定は、上記のように `-ldflags` オプションの引数に __`-extldflags "-static"`__ を追加することで行えます（オプション指定が入れ子になってるので分かりにくいですね ^^;）。
多くの場合はこの指定をしなくても静的リンクでビルドされるようですが、cgo パッケージ（C ライブラリ連携）などを使っていると自動的に動的リンクになるなどの振る舞いをするので、明示的なオプション指定をしておいた方がよさそうです。
ライブラリの動的リンクが残っていると、コンテナ起動時に `exec /hello: no such file or directory` といったエラーが発生します。

上記の `Dockerfile` をビルドすると、小さな Docker イメージができあがります。

```console
$ docker image build -t hello .
$ docker image ls
REPOSITORY     TAG       IMAGE ID       CREATED             SIZE
hello          latest    757922277d94   4 seconds ago       4.46MB
```

10MB 弱だったのが、5MB 弱まで小さくなりました！
ちょうど Alpine Linux のサイズ分だけ小さくなっています。

{{% note title="file コマンドで実行ファイルの種類を調べる" %}}
`go build` コマンドで指定するオプションによって生成される実行ファイルの形式が変わってきます。
実行ファイルのフォーマットは、__`file`__ コマンドで確認できます。

```console
# macOS で何もオプション指定せずにビルドした場合
$ go build -o hello
$ file hello
hello: Mach-O 64-bit executable x86_64

# OS やアーキテクチャを指定してクロスコンパイルした場合
$ GOOS=linux GOARCH=amd64 go build -o hello
$ file hello
hello: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), statically linked, Go BuildID=MjRBk0jRAbyfXWsLUypa/ZDyt22XXA4ZiyzbfYl8i/5chB8hOVmP2fY1XwpT95/R7p5BmDr9QZtUfrx80RH, not stripped
```
{{% /note %}}


おまけ
----

### Docker Compose でビルド＆実行をワンステップで行う

[Docker Compose を使う](https://maku77.github.io/p/qm5k2hx/) と、Docker イメージのビルドからコンテナ起動までをワンステップで実行できるようになります。
`Dockerfile` と同じディレクトリに、次のようなファイルを作成すれば準備完了です。

{{< code lang="yaml" title="docker-compose.yml" >}}
version: "3"

services:
  web:
    build: .
    ports:
      - 8080:8080
{{< /code >}}

あとは、`docker image` や `docker container` コマンドの代わりに次のような __`docker compose`__ コマンドを使います。
イメージ名やコンテナ名は、ディレクトリ名と `docker-compose.yml` に記述したサービス名から自動生成されるので、コマンド実行時に指定する必要はありません。
今回の例の場合は、イメージ名は `hello_web`、コンテナ名は `hello-web-1` という感じになります。

```console
$ docker compose up     # イメージビルド＆コンテナ起動（-d でバックグラウンド起動）
$ docker compose ps     # コンテナの一覧を表示
$ docker compose stop   # コンテナを停止
$ docker compose start  # 停止されたコンテナを起動
$ docker compose rm     # 停止されたコンテナを削除
$ docker compose down   # コンテナの停止＆削除
```

とりあえず、`docker compose up -d` で起動、`docker compose down` で停止＆削除を覚えておけばなんとかなります。

### コンテナから HTTPS 通信するとき

コンテナ内のアプリから HTTPS 通信するときは、Root CA 証明書や、タイムゾーン情報が必要になります。
親イメージにこれらのファイルがない場合は、何らかのイメージからファイルをコピーすることで対応できます。

```docker
FROM scratch
COPY --from=golang:1.19 /etc/ssl/certs/ca-certificates.crt /etc/ssl/certs/ca-certificates.crt
COPY --from=golang:1.19 /usr/share/zoneinfo /usr/share/zoneinfo
```

