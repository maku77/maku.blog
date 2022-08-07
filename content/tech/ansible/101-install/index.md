---
title: "Ansible とは？ Ansible をインストールする"
url: "/p/m7ju6fq"
date: "2016-10-22"
lastmod: "2022-02-15"
tags: ["Ansible"]
weight: 101
---

Ansible とは？
----

Ansible は、2012 年に Michael DeHaan 氏によって公開されたコンフィギュレーションツールです。
Ansible を実行するホスト自身の構成を行うこともできるし、複数のホストに対して一括して設定することもできます。
Chef や Puppet に比べて、導入や設定が容易という特徴があります。

### 類似ツールの比較

| ツール名 | 公開日 | 作者 | 作成言語 | 構成管理ファイル |
| ---- | ---- | ---- | ---- | ---- |
| __Ansible__ | 2012 年 | Michael DeHaan 氏 | Python 製 | Playbook |
| Chef | 2009 年 | Adam Jacob 氏 | Ruby + Erlang 製 | クックブック |
| Puppet | 2005 年 | Luke Kanies 氏 | Ruby 製 | マニフェスト |


### Ansible の特徴

* ツール自体は Python で記述されています。
* コントロールされる側のホスト（マネージドノード）には、__Python と SSH さえ入っていればよく__、導入が非常に容易です。コントロールする側のホスト（コントロールノード）から、SSH で Python スクリプトを流し込んで実行するという手法です。
* 複数のホストをプッシュ型でコントロールするので、大量のホスト（数千）の制御も問題なく行えます（複数のホストで並列にコンフィギュレーションが実行される）。ansible-pull というツールを導入すれば、プル型で動作させることも可能です（リモートホストがプロキシ環境内にある場合など）。
* __設定・構成情報は YAML 形式のテキストファイル (Playbook)__ で記述します。
* Playbook で定義する各種処理（タスク）はモジュールによって提供されており、__モジュール自身は様々な言語で実装することが可能__ です（200 を超える組み込みモジュールは Python で記述されています）。
* 実行後の状態に関して冪等性（べきとうせい）が保証されており、何度実行しても同じ状態になるようになっています。Playbook には、「期待する状態」を「宣言的」に記載します。処理手順ではなく、目指すべき姿を定義するということです。
* どのような環境でも実行可能な汎用的な Playbook を記述するというよりは、自分たちの組織用にカスタマイズされた Playbook を作成するという用途に向いています。たとえば、apt と yum のどっちのパッケージ管理ツールが使えるのかなどは意識して記述する必要があります。


Ansible の要件
----

{{< image w="250" src="img-001.drawio.svg" >}}

VPS などのリモートサーバーを Ansible で制御できるようにするには、少なくとも対象のサーバー（マネージドノード）に SSH 接続できるようになっていて、Python がインストールされている必要があります。
これは、Ansible が SSH で Python スクリプトを流し込んで、ターゲット上で実行するという仕組みになっているからです。

現状 Ansible のコントロールノードになり得るのは、Linux 系 OS や macOS のみです。
Windows はサポートされていません（参考: [Control node requirements](https://docs.ansible.com/ansible/latest/installation_guide/intro_installation.html#control-node-requirements)）。


Ansible のインストール
----

Ansible のコントロールノードとなるマシンには、`ansible` コマンドをインストールする必要があります。
必要条件は、Linux 系の OS（macOS でも OK）であり、Python 3.8 以上がインストールされていることです。
内部で `fork` などを使用する都合上、Windows への `ansible` コマンドのインストールはサポートされていません（WSL を使って動かすことはできます）。

Ansible 2.10 からは、以下の 2 種類のパッケージが提供されており、いずれも Python の __`pip`__ コマンドや、OS ごとのパッケージ管理コマンドでインストールできます。

ansible（コミュニティパッケージ）
: 従来の ansible に相当するもので、いくつかの基本的なモジュールがバンドルされています。

ansible-core
: 最小限の構成のパッケージで、必要なモジュールは適宜 [Ansible Galaxy](https://galaxy.ansible.com) などからインストールする必要があります。

慣れないうちは、前者のコミュニティパッケージの方を使った方がトラブルが少ないと思います。
詳細なインストール方法は [公式マニュアル](https://docs.ansible.com/ansible/latest/installation_guide/intro_installation.html) に記載がありますが、基本的には `pip` コマンドでインストールしてしまえば OK です。

{{% private %}}
- Ansible 2.9 までは 3000 以上のモジュールが本体といっしょにリリースされていた。
- 現在は `ansible-core`（本体）と各機能（コレクション）が別々に配布されている。これにより、バージョンアップのタイミングが柔軟になりました。
- 現在でも Ansible Community Package というのを使えば、コミュニティが選択したコレクションをまとめてインストールできます。どんなコレクションが同梱されているかは、GitHub の [ansible-community/ansible-build-data](https://github.com/ansible-community/ansible-build-data/) で確認できます。
- `ansible --version` とすると、`ansible-core` のバージョンを確認できます。
{{% /private %}}

### pip でインストールする方法（全 OS 共通）

Ansible は Python 製のツールなので、Python のパッケージマネージャである `pip` でインストールできます。

```console
$ python3 -m pip install --user ansible
```

グローバルにインストールしたい場合は、上記のコマンドから `--user` オプションを外して、`sudo` を付けて実行します。
ただし、システム全体に影響してしまうので、`--user` オプションでのインストールが推奨されています。

最後に、インストールがうまくいったか確認しましょう。

```console
$ ansible --version
ansible [core 2.12.2]
  config file = None
  configured module search path = ['...']
  ...
```

もし、`ansible: command not found` というエラーになったら、次のようにして `ansible` がインストールされたディレクトリを検索して、`PATH` 環境変数に設定します（参考: [pip - Installing to the User Site](https://packaging.python.org/en/latest/tutorials/installing-packages/#installing-to-the-user-site)）。

```console
$ python3 -m site --user-base
/Users/maku/Library/Python/3.10 ←たぶんこの下の bin ディレクトリにある

$ ~/Library/Python/3.10/bin/ansible --version
ansible [core 2.12.2]
...
```

{{< code lang="bash" title="~/.bash_profile（下記を追加）" >}}
export PATH=$PATH:$HOME/Library/Python/3.10/bin
{{< /code >}}

### 各種 Linux 系パッケージマネージャでインストールする方法

{{< code lang="console" title="Fedora の場合" >}}
$ sudo dnf install ansible
{{< /code >}}

{{< code lang="console" title="RHEL の場合" >}}
$ sudo yum install ansible
{{< /code >}}

{{< code lang="console" title="CentOS の場合" >}}
$ sudo yum install epel-release
$ sudo yum install ansible
{{< /code >}}

yum の拡張リポジトリ (EPEL: Extra Package for Enterprise Linux) から Ansible をインストールするために、最初に `epel-release` をインストールする必要があります。

{{< code lang="console" title="Ubuntu の場合" >}}
$ sudo apt update
$ sudo apt install software-properties-common
$ sudo add-apt-repository --yes --update ppa:ansible/ansible
$ sudo apt install ansible
{{< /code >}}

拡張リポジトリ (PPA: Personal Package Archive) を登録して情報更新してから `install` を実行する必要があります。

