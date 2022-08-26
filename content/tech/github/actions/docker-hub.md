---
title: "GitHub Actions でコンテナイメージをビルドして Docker Hub へ push する"
url: "p/v9r8o5k/"
permalink: "p/v9r8o5k/"
date: "2022-08-26"
tags: ["Docker", "GitHub", "GitHub Actions"]
---

何をするか？
----

Docker イメージを作成するための `Dockerfile` を GitHub のリポジトリで管理しているとします。
ここでは、GitHub Actions のワークフローを作成して、GitHub リポジトリにコードを push したときに次のような処理が自動で実行されるようにします。

1. `Dockerfile` を使ってイメージをビルド
2. Docker Hub にイメージを push

前提条件として、Docker Hub のアカウントは作成済みとします。


Dockerfile の準備
----

GitHub のリポジトリを作成して、ルートディレクトリに次のような `Dockerfile` をコミット＆プッシュしておきます。

{{< code lang="dockerfile" title="Dockerfile" >}}
FROM alpine:3
CMD ["echo", "Hello World"]
{{< /code >}}

上記のサンプルコードは、軽量の Alpine Linux で Hello World と出力するだけの `Dockerfile` ですが、もちろん好きなように記述していただいて構いません。


GitHub Actions の Secrets 情報を登録
----

GitHub Actions から Docker Hub にイメージをプッシュできるようにするため、GitHub リポジトリの Secrets 情報として、Docker Hub のユーザー名およびアクセストークンを登録しておきます。

Docker Hub のアクセストークンは次のように発行できます。

1. [Docker Hub](https://hub.docker.com) にサインイン
1. 右上のユーザー名をクリックして、`Account Settings` を選択
1. `Security` → `New Access Token` と選択して、トークンの説明文を入力
   - `Access Token description`: Deploy hello world など適当に入力
   - `Access permissions`: 少なくとも Read, Write を付ける
1. `Generate` ボタンを押して生成

これで、次のような感じのアクセストークンが発行されるので、どこかにコピーしておきます（後から確認することはできません）。

```
dckr_pat_Wy1rW3PCchFgwW2dtES8x8C4AGo
```

アクセストークンを用意できたら、GitHub Actions の Secrets に登録します。

1. 対象の GitHub リポジトリのページを開く
1. `Settings` タブ → `Secrets` → `Actions` とたどる
1. `New repository secret` ボタンを押して、次のように Secrets を追加する
   - __`DOCKERHUB_USERNAME`__ : Docker Hub のユーザー名を設定
   - __`DOCKERHUB_TOKEN`__ : Docker Hub のアクセストークンを設定

{{< note title="パスワードではなくアクセストークンを使う" >}}
Docker Hub へのイメージの push には、アクセストークンの代わりに、ユーザーのパスワードを使うこともできます。
ただし、万が一の漏洩時の被害を最小限にするため、デプロイ専用のアクセストークンを生成して使うことが推奨されています。
アクセストークンの生成は 1 分もかからずにできるので、面倒がらずに作るようにしましょう。
{{< /note >}}


ワークフローの作成
----

Docker イメージをビルドして、Docker Hub に push するためのワークフローを作成します。

{{<code lang="yaml" title=".github/workflows/build-and-push.yml" >}}
name: Build Docker image and push to Docker Hub

on:
  push:
    branches: main

jobs:
  build-and-push:
    runs-on: ubuntu-latest
    steps:
      - name: Check out the repo
        uses: actions/checkout@v3

      - name: Log in to Docker Hub
        uses: docker/login-action@v2.0.0
        with:
          username: ${{ secrets.DOCKERHUB_USERNAME }}
          password: ${{ secrets.DOCKERHUB_TOKEN }}

      - name: Build and push
        uses: docker/build-push-action@v3
        with:
          context: .
          push: true
          tags: ${{ secrets.DOCKERHUB_USERNAME }}/hello:latest
{{< /code >}}

### 各アクションの説明

```yaml
- name: Log in to Docker Hub
  uses: docker/login-action@v2.0.0
  with:
    username: ${{ secrets.DOCKERHUB_USERNAME }}
    password: ${{ secrets.DOCKERHUB_TOKEN }}
```

[docker/login-action アクション](https://github.com/marketplace/actions/docker-login) を使って、Docker Hub へサインインします。
ちなみに、このアクションで GitHub Container Registory へのサインインもできます。

```yaml
- name: Build and push
  uses: docker/build-push-action@v3
  with:
    context: .
    push: true
    tags: ${{ secrets.DOCKERHUB_USERNAME }}/hello:latest
```

[docker/build-push-action アクション](https://github.com/marketplace/actions/build-and-push-docker-images) を使って、Docker イメージのビルドと、Docker Hub への push を行います。
`context: .` というプロパティは、カレントディレクトリの `Dockerfile` を使ってイメージをビルドすることを示しています。
`tags` プロパティの値はイメージの識別子（タグ）になるものなので、適切な名前を付けるようにしてください。

上記のようなワークフロー・ファイルを作成したら、GitHub にコミット＆プッシュします。
すると、自動的に GitHub Actions が起動して、Docker イメージのビルドと、Docker Hub への push が実行されます。
簡単なイメージなので、わずか数秒でデプロイまで完了します。
GitHub Actions の実行が完了したら、[Docker Hub](https://hub.docker.com/) にサインインして、イメージがデプロイされていることを確認してください。


イメージを使ってみる
----

Docker Hub へのイメージのデプロイが終わったら、__`docker container run`__ でイメージを取得＆実行できるかを確認します。
イメージ名は適切なものに変更して実行してください。

```console
$ docker container run --rm maku77/hello
...（イメージがダンロードされる）...
Hello World
```

うまくいきました！


（応用）イメージのタグを自動で付加する
----

上記の例では、イメージのタグを次のように指定していました。

```yaml
tags: ${{ secrets.DOCKERHUB_USERNAME }}/hello:latest
```

[docker/metadata-action アクション](https://github.com/marketplace/actions/docker-metadata-action) を使うと、GitHub リポジトリで付けたタグ名などを利用して、Docker イメージのタグをそれっぽく自動で付加してくれます。

例えば、`main` ブランチへプッシュしたときにはイメージのタグにも `main` を付けるとか、GitHub で `v1.0.0` タグを付けた場合はイメージのタグにも `v1.0.0` を付けるといったことが可能です。
ワークフローの具体的な記述方法は、上記公式サイトを参照してください。

