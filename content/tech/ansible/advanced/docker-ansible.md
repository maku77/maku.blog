---
title: "Docker で Ansible の実行環境用のコンテナを作成する"
url: "p/euevcs8/"
date: "2022-07-02"
tags: ["Ansible", "Docker"]
---

何をするか？
----

Ansible の実行環境は Python がインストールされている環境であれば比較的簡単にインストールできますが、Docker の実行環境があれば、ホスト環境に何もインストールせずに Ansible の実行環境を手に入れることができます（もちろんコンテナは作る必要はありますが）。

Docker Hub を見ると、Alpine Linux ベースの Ansible 実行環境用イメージ [alpine/ansible](https://hub.docker.com/r/alpine/ansible) があるようですが、ここでは `Dockerfile` を使って自分でイメージを作成することにします。


Ansible 実行環境用イメージを作成する
----

`Dockerfile` で Alpine Linux ベースの Ansible 実行環境を定義します。

{{< code lang="docker" title="Dockerfile" >}}
FROM alpine:3

WORKDIR /app

# --no-cache を付けることで /var/cache/apk 以下にキャッシュが残るのを防ぐ
# --update-cache を付けることで先に apk update するのと同じ効果になる
RUN apk --no-cache --update-cache add ansible openssh sshpass
{{< /code >}}

APK (Alpine Package Keeper) で次のようなパッケージをインストールしています。

- `ansible` ... Ansible Community パッケージ（`ansible` コマンドや `ansible-playbook` コマンドなど）
- `openssh` ... `ssh` コマンドのため
- `sshpass` ... ターゲットホストにパスワード認証 (`--ask-pass`) で接続するときのため

Ansible Community パッケージ (`ansible`) ではなく、Ansible Core (`ansible-core`) を使うようにすれば、イメージサイズは 500MB 弱から 80MB 程度に削減できますが、サイズを気にしなくてよいなら Ansible Community パッケージを使っておいた方が楽です。

`Dockerfile` を記述したら、ビルドしてイメージを作成します。
ここではイメージ名はシンプルに `ansible` としています。

{{< code lang="console" title="ansible という名前のイメージを作成" >}}
$ docker image build -t ansible .
{{< /code >}}

イメージが作成できているか確認します。

```console
$ docker image ls
REPOSITORY   TAG       IMAGE ID       CREATED          SIZE
ansible      latest    224bbc474bf8   3 minutes ago   472MB
```


コンテナの ansible コマンドを実行する
----

### ansible コンテナ内のコマンドを実行する

Ansible の各種コマンドをインストールした `ansible` イメージを作成したので、次のようにして `ansible` コマンドや `ansible-playbook` コマンドを実行できます。

```console
## ansible コマンドの実行
$ docker container run --rm ansible ansible

## ansible-playbook コマンドの実行
$ docker container run --rm ansible ansible-playbook
```

`--rm` オプションを指定して実行しているので、コマンド実行後にコンテナはすぐに破棄されます。
でもよく考えたら `ansible-galaxy` による Ansible コレクションのインストールは適宜必要になるので、`--rm` オプションは外して使うことになるかもしれません。

{{% note title="コンテナのシェルを使う" %}}
必要に応じて、`ansible` コンテナの `ash` シェルを使って作業することもできます。

```console
$ docker container run --rm -it ansible ash
/app # ansible --help
/app # exit
```
{{% /note %}}

### ansible エイリアス、ansible-playbook エイリアスを作成する

毎回 `docker container run` コマンドを入力するのは面倒ですし、このままだと、ホスト側のディレクトリにあるインベントリファイル (`inventory`) や Playbook ファイルを参照できないので、次のようなエイリアスを定義しておくと便利です。

{{< code lang="bash" title="alias-ansible.bashrc（source での読み込み用）" >}}
alias ansible='docker container run --rm -it --mount type=bind,src="$(pwd)",dst=/app ansible ansible'
alias ansible-playbook='docker container run --rm -it --mount type=bind,src="$(pwd)",dst=/app ansible ansible-playbook'
{{< /code >}}

`--mount` オプションによって、ホスト側のカレントディレクトリを、コンテナの作業ディレクトリ (`/app`) にバインドマウントしています。
SSH キー用に `--mount type=bind,src="$HOME/.ssh",dst=/root/.ssh` も追加しておいた方がいいかもしれません。
これで、あたかもホストマシンに `ansible` コマンドや `ansible-playbook` コマンドがインストールされているかのように実行できます。

```console
$ ansible localhost -m ping
[WARNING]: No inventory was parsed, only implicit localhost is available
localhost | SUCCESS => {
    "changed": false,
    "ping": "pong"
}
```

### もう少し実践的な使用例

`example.com` というホストに SSH 接続（パスワード認証）して `ping` モジュールを実行してみます。
まず、Ansible によるコントロール対象を記述したインベントリファイルを用意します。

{{< code title="inventory" >}}
example.com  ansible_ssh_common_args='-o StrictHostKeyChecking=no'
{{< /code >}}

インベントリファイルでは、初回 SSH 接続時の known_hosts 未登録エラーを防ぐための `StrictHostKeyChecking` オプションを指定しています。

{{% accordion title="参考: known_hosts 未登録エラーの例" %}}
```
example.com | FAILED! => {
    "msg": "Using a SSH password instead of a key is not possible because Host Key checking is enabled and sshpass does not support this.  Please add this host's fingerprint to your known_hosts file to manage this host."
}
```
{{% /accordion %}}

`root` ユーザーでパスワード認証して `ping` モジュールを実行してみます。

```console
$ ansible -i inventory example.com -u root --ask-pass -m ping
SSH password: （root ユーザーのパスワードを入力）
example.com | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python3"
    },
    "changed": false,
    "ping": "pong"
}
```

うまくいきました！

`ansible-playbook` コマンドも同様に実行できます。

