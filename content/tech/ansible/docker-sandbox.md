---
title: "Docker で Ansible の設定テストに使える Sandbox コンテナを作成する"
url: "p/f3eox8h/"
date: "2022-07-02"
tags: ["Ansible", "Docker"]
draft: true
---

{{% private %}}
この記事は削除して OK。
{{% /private %}}

SSH 接続できる Docker コンテナを作成する
----

`ansible-playbook` を使って何らかの Linux 環境のセットアップを自動化するとき、Playbook のテスト用に使い捨ての Docker コンテナ（Sandbox コンテナ）があると便利です。
Ansible によるコントロールを可能にするには、少なくとも次のようなイメージを作成する必要があります。

- __SSH 接続__ できること（パスワード認証でも OK）
- __Python3__ がインストールされていること

例えば、次の `Dockerfile` は SSH 接続可能な Ubuntu 22.02 イメージを構成します。
Python3 の実行環境は最初から入っているので、sshd (openssh-server) の環境だけセットアップすればよさそうです。

{{< code lang="docker" title="Dockerfile" >}}
FROM ubuntu:22.04

RUN apt update && apt install -y openssh-server
RUN mkdir /var/run/sshd
RUN echo 'root:mypass' | chpasswd
RUN sed -i -E 's/^#(PermitRootLogin) .+/\1 yes/' /etc/ssh/sshd_config

EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]
{{< /code >}}

`root` のパスワードを適当に設定し、SSH による直接接続を許可しています。
`Dockerfile` を用意したら、次のようにして `ansible-sandbox` という名前のイメージを作成します。

{{< code lang="console" title="イメージの作成" >}}
$ docker image build -t ansible-sandbox .
{{< /code >}}

Docker イメージを作成したら、次のようにしてコンテナを起動できます。
ここでは、ホスト側の 10022 ポートにコンテナ側の SSH ポートをマッピングしてます。

{{< code lang="console" title="sandbox コンテナを起動する" >}}
$ docker container run --rm -d -p 10022:22 --name sandbox ansible-sandbox
{{< /code >}}

あとは、ホスト側から次のように SSH 接続できます。

```console
ssh -p 10022 root@localhost
```

コンテナのシェルを使いたくなったときは、次のように `bash` プロセスを起動します。

```console
$ docker container exec -it sandbox /bin/bash
root@408dea24e473:/#
```

テストが終わって、コンテナが必要無くなったら次のように停止します。
`run` するときに `--rm` オプションを指定しておいたので、コンテナは停止と同時に削除されます。

```console
$ docker container stop sandbox
```

