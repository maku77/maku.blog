---
title: "Docker コンテナで Ansible のテストベッド環境を用意する"
url: "p/csctaq7/"
date: "2022-07-18"
tags: ["Ansible", "Docker"]
---

何をするか
----

`ansible-playbook` を使って何らかの Linux 環境のセットアップを自動化するとき、Playbook のテスト用に使い捨ての Docker コンテナ（テストベッド環境）があると便利です。

{{< image src="img-001.drawio.svg" >}}

Ansible には、Playbook 実行後の冪等性を保つという性質がありますが、一度 Playbook を実行してしまうと元の状態に戻すことはできません。
一方通行の冪等性はあっても、可逆性はないということですね。
Playbook を試行錯誤して作っている段階では、何度も `ansible-playbook` を実行することになるので、本当にその Playbook がまっさらな状態の OS に対して適用できるのか不安になってきます（冪等性があるので理論的には動作するはずですが）。
異なるディストリビューションに対して実行できるのか試したくなることもあります。

ここでは、Ansible のテストベッド環境として Docker コンテナを作成し、各種 Ansible コマンド (`ansible` / `ansible-playbook`) で制御してみます。


テストベッド用のコンテナを起動する
----

Ansible は一般的にはマネージドノードに SSH 接続して Playbook を実行しますが、実は Docker コンテナに直接接続することもできます。
つまり、テストベッド用の Docker コンテナには、sshd (OpenSSH) などのサービスをわざわざインストールする必要はありません。

ただし、Ansible マネージドノードの要件として、Python3 だけはインストールしておく必要があります。
次の `Dockerfile` では、Ubuntu を親イメージとして、Python3 だけ追加しています。

{{< code lang="docker" title="Dockerfile" >}}
FROM ubuntu:22.04

# Python3 のインストール
RUN apt update && apt install --no-install-recommends -y python3
{{< /code >}}

`Dockerfile` をビルドして、`testbed` イメージを作成します。

{{< code lang="console" title="testbed イメージをビルド" >}}
$ docker image build . -t testbed
{{< /code >}}

このイメージを使って、コンテナを起動します。
ここでは、コンテナ名も `testbed` にしています。

{{< code lang="console" title="testbed コンテナを起動" >}}
$ docker container run --rm -td --name testbed testbed bash
{{< /code >}}

コンテナがすぐに終了してしまわないように、`-t (--tty)` オプションを付けて `bash` に仮想端末を割り当て、さらにバックグラウンド起動 (`-d`) しています（参考: [Docker コンテナをバックグラウンド実行する](https://maku77.github.io/p/dmpsvz3/)）。

{{% note title="コンテナを特権モードで動かす" %}}
Docker コンテナ内でのパケットフィルタ設定は通常必要ありませんが、今回の例のように Ansible の制御対象となるテストコンテナでは、`iptables` や `ufw` といったファイアウォールツールを実行する必要があるかもしれません。
このような場合は、コンテナ起動時に __`--cap-add=NET_ADMIN`__ オプションを指定することで、ネットワーク制御の権限を与える必要があります。
さらに強い特権モードでコンテナを起動するには __`--priviledged`__ オプションを使用できますが、こちらはよほどのことがない限り使用しません（Docker コンテナ内で Docker コンテナを動かすなど）。
{{% /note %}}

次のようにして、`testbed` コンテナが立ち上がっていることを確認します。

```console
$ docker container ls
CONTAINER ID  IMAGE    COMMAND  CREATED        STATUS        PORTS  NAMES
20cc261f5352  testbed  "bash"   3 seconds ago  Up 2 seconds         testbed
```


Ansible のインベントリファイルを作成する
----

操作対象とする Docker コンテナの名前（ここでは `testbed` のみ）を列挙したインベントリファイルを作成します。
__`ansible_connection=docker`__ オプションを指定することで、接続先が Docker コンテナであることを Ansible に知らせます。
この指定がないと、Ansible は SSH 接続しようとしてしまいます。

{{< code lang="ini" title="hosts.ini（INI 形式の場合）" >}}
testbed ansible_connection=docker
{{< /code >}}

{{< code lang="ini" title="hosts.yml（YAML 形式の場合）" >}}
all:
  hosts:
    testbed:
      ansible_connection: docker
{{< /code >}}


Ansible コマンドを実行する
----

インベントリファイルができたので、Ansible コマンドを実行して `testbed` コンテナを制御してみます。

### ansible コマンドの実行

まずは、`ansible` コマンドで単独のモジュール (`ping`) を実行してみます。

```console
$ ansible -i hosts.ini all -m ping
testbed | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python3"
    },
    "changed": false,
    "ping": "pong"
}
```

うまく制御できました！

### ansible-playbook コマンドの実行

次に、`ansible-playbook` を実行してみます。
テスト用に次のような簡単な Playbook を作成します。
Ansible 組み込みの `user` モジュールを使って、`ansible` というユーザーを作成する Playbook です。

{{< code lang="yaml" title="main.yml (Playbook)" >}}
- hosts: all
  tasks:
  - name: Create ansible user
    ansible.builtin.user:
      name: ansible
      shell: /bin/bash
{{< /code >}}

Playbook を実行します。

```console
$ ansible-playbook -i hosts.ini main.yml

PLAY [all] **************************...

TASK [Gathering Facts] **************...
ok: [testbed]

TASK [Create ansible user] **********...
changed: [testbed]

PLAY RECAP **************************...
testbed  : ok=2 changed=1 unreachable=0 failed=0 skipped=0 rescued=0 ignored=0
```

こちらもうまくいきました！

### コンテナの状態を見てみる

最後に、`testbed` コンテナ上で `bash` を起動して、どんな状態になっているかを確認しておきます。
ちゃんと `ansible` ユーザーが作成されていれば成功です。

```console
$ docker container exec -it testbed bash
root@f7c7a2d8f325:/# ls /home
ansible
```

次のようにコンテナを作り直せば、再びまっさらな状態からやり直すことができます。
コンテナ起動時に `--rm` オプションを指定しておいたので、コンテナは停止と同時に削除されます。

```console
# コンテナの停止＆削除
$ docker container stop testbed
testbed

# コンテナの再生成＆起動
$ docker container run --rm -td --name testbed testbed bash
fc17f036b0a5c2dafbaa7e55d936fb6a052cbe0fd05465c25c0e680baebf2316
```

