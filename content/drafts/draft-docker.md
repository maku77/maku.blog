---
title: "DRAFT: Docker"
url: "p/bqar8n4/"
date: "2022-06-13"
tags: ["Docker"]
draft: true
---

docker コマンドの動作確認
----

```console
$ docker container run --rm hello-world

（Hello from Docker! のようなメッセージを確認できればうまく動いています）
```

その他メモ
----

- Docker は __Go 言語__ で実装されている。
- Docker のクライアントとデーモンは __REST API__ で通信している。なので、それぞれは別の PC で動いていても問題ない（クラ・サバモデル）。
- docker コマンドを root 権限なしで実行できるようにする。
  ```console
  $ sudo usermod -aG docker $(whoami)
  $ reboot  # システムを再起動して反映
  ```
- Docker レジストリ (Docker Hub) への push
  ```console
  $ docker tag myimg username/myimg
  $ docker push username/myimg
  ```


Docker 関連用語
----

ビルド・コンテキスト (build context)
: docker build コマンド実行時の作業ディレクトリ。ビルド・コンテキストから不要なファイルを除外するには、`.dockerignore` ファイルを使います。


Docker コンテナ内のファイルをホスト側に取り出す 2022-06-15
----

まず、`alpine/git` イメージからコンテナ (`repo`) を作って、その中で git clone します。

```console
$ docker run --name repo alpine/git clone https://github.com/docker/getting-started.git
```

__`docker cp`__ コマンドで、コンテナの中の `/git/getting-started` ディレクトリを Docker ホスト側にコピーします。

```console
$ docker cp repo:/git/getting-started .
```

これで、ホスト側のカレントディレクトリに `getting-started` ディレクトリがコピーされます。


コマンドメモ
----

| コマンド | 内容 |
| -------- | ---- |
| `docker build --no-cache ...` | ビルド時にキャッシュを使わない |
| `docker ps -l`   | 最後に作成されたコンテナの情報を確認 |
| `docker ps -lq`  | 最後に作成されたコンテナの ID だけ確認 |
| `docker commit -m "<Comment>" -a "<Author>" <ContainerID> <user>/<repo>:<tag>` | コンテナからイメージを作成 |
| `docker run -it ubuntu:12.04 /bin/bash` | シェルを起動して TTY 接続 |
| `docker run --rm -it ubuntu:12.04 /bin/bash` | 終了時に自動的にコンテナを破棄 |

