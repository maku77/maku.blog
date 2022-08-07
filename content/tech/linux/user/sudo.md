---
title: "Linuxメモ: sudo で管理者権限でコマンド実行できるようにする (/etc/sudoers, visudo)"
linkTitle: "sudo で管理者権限でコマンド実行できるようにする (/etc/sudoers, visudo)"
url: "p/h5v6gqz/"
date: "2022-07-10"
tags: ["Linux"]
---

sudo とは？
----

Linux の __`sudo`__ コマンドは、一時的に `root` ユーザー（あるいは任意のユーザー）としてコマンドを実行できるようにするためのコマンドです。
システムの管理者的な立場の人であっても、常に `root` ユーザーとして作業するのはリスクがあるため、必要に応じて `sudo` でコマンド実行することが推奨されています。

`sudo` コマンドは、次のような形で実行します。

```console
$ sudo ＜管理者権限が必要なコマンド＞
```

例えば、`/etc/shadow` ファイルの閲覧は `root` ユーザーにしか許可されていませんが、カレントユーザーが `sudoers`（後述）に登録されていれば、次のようにして読み込むことができます。

```console
$ sudo cat /etc/shadow
[sudo] password for maku: （カレントユーザーのパスワードを入力）
```

APT パッケージのインストールでも `sudo` はよく使われますね。

```console
$ sudo apt install ansible
[sudo] password for maku: （カレントユーザーのパスワードを入力）
```

`sudo` 経由でコマンドを実行するときにパスワードの入力を求められますが、このとき入力するパスワードは `root` のものではなく、カレントユーザーのパスワードであることに注意してください（OpenSUSE の場合は例外的に `root` のパスワード）。
つまり、`sudo` は一般ユーザーが自分のパスワードで実行権限を切り替えてコマンドを実行する仕組みであり、`sudoers` の設定は慎重に行わなければいけません。


/etc/sudoers ファイルと visudo コマンド
----

### /etc/sudoers ファイル

どのユーザーが、どのコマンドを、どのユーザーとして実行できるかを管理しているのが __`/etc/sudoers`__ ファイルです。
ただし、このファイルは次のように編集ができないよう権限設定されており、直接エディタで開いて編集することはできません。

```console
$ ls -l /etc/sudoers
-r--r----- 1 root root 1671 Feb  8 17:41 /etc/sudoers
```

### visudo コマンド

`/etc/sudoers` ファイルを編集するには、特別に用意された __`visudo`__ コマンドを使用する必要があります。
次のように実行すれば、デフォルトエディタで `/etc/sudoers` ファイルを開くことができます。

{{< code title="/etc/sudoers を編集する" >}}
# visudo       （root ユーザーの場合）
$ sudo visudo  （一般ユーザーの場合）
{{< /code >}}

デフォルトエディタは例えば Ubuntu/Debian の場合は `nano` ですが、__EDITOR__ 環境変数で任意のエディタで開くよう指定できます。

{{< code title="/etc/sudoers を vim で開く" >}}
$ sudo EDITOR=vim visudo
{{< /code >}}


/etc/sudoers ファイルを記述する
----

### /etc/sudoers のフォーマット

`/etc/sudoers` ファイルの各行は次のようなフォーマットで記述されています（他にもエイリアスという変数のようなものを定義できますが、ここでは省略します）。

{{< code title="フォーマット" >}}
ユーザー ホスト=(RunAs) コマンド [, (RunAs) コマンド, ...]
{{< /code >}}

{{< code title="記述例" >}}
maku ALL=(root) /sbin/hogehoge
{{< /code >}}

大まかには、`=` の前の部分で「どのユーザー」が、`=` の後ろの部分で「どのユーザーとしてどのコマンド」を実行できるかを表現しています。

- __`ユーザー`__ ... `sudo` を実行可能にするユーザー名を指定します。`%グループ名` とするとグループに対しての設定になります。例えば、`%admin` と指定すると、`admin` グループに所属するユーザーに対して `sudo` 権限を割り当てることになります。
- __`ホスト`__ ... `sudo` コマンドを実行するホスト（ホスト名あるいは IP アドレス）を指定します。通常は `ALL` と指定しておけば問題ありません。
- __`(RunAs)`__ ... 後ろに指定するコマンドを、どのユーザーとして実行可能かを指定します。`ALL` を指定すると、どのユーザーとしてでもコマンドを実行できるようになります（これは `root` として実行できることを意味します）。`sudo` はデフォルトで `root` ユーザーとしてコマンドを実行しようとしますが、`-u` オプションで、ユーザーを明示して実行できます。`(RunAs)` 部分に `(hoge)` と設定した場合は、`sudo` 実行時に必ず `-u hoge` オプションを指定しなければいけません。
- __`コマンド`__ ... `sudo` コマンド経由で、どんなコマンドを実行できるかを指定します。`ALL` を指定すると、あらゆるコマンドを実行できます。

`=` の後ろの `(RunAs) コマンド` は、カンマ区切りで複数列挙することができます。
例えば、`(user1) cmd1, (user2) cmd2` と記述すると、ユーザー `user1` でコマンド `cmd1` を、ユーザー `user2` でコマンド `cmd2` を実行できるようになります。
また、2 番目以降の `(RunAs)` 部分を省略すると、前に指定したものが再利用されます。
例えば、`(root) cmd1, cmd2, cmd3` と記述すると、`root` ユーザーで `cmd1`、`cmd2`、`cmd3` を実行できるようになります。

### /etc/sudoers の設定例

```
ALL ALL=(ALL) ALL
```

__上記のように設定してはいけません__。
これは、すべてのユーザーが、管理者権限ですべてのコマンドを実行できることを意味します。

```
maku ALL=(ALL) ALL
```

ユーザー `maku` は、管理者権限であらゆるコマンドを実行できます。
デフォルトでは `root` ユーザーとしてコマンドを実行しますが、`-u` オプションを使えば、任意のユーザーとしてコマンドを実行することができます（その場合は、指定したユーザーの権限でしかコマンドを実行できなくなります）。

```
%admin ALL=(ALL) ALL
```

`admin` グループに所属するユーザーは、すべてのコマンドを管理者権限で実行できます。
例えば、ユーザー `maku` を `admin` グループに所属させれば、`maku` は管理者権限であらゆるコマンドを `sudo` 実行できるようになります（参考: [Linux ユーザーをグループに登録する](/p/uexfvcs/)）。
ちなみに、管理者グループを示すグループ名として、Debian 系 (Ubuntu) では `admin` という名前が使われ、Red Hat 系 (RHEL, Fedora, CentOS, Rocky, AlmaLinux) では `wheel` という名前が使われます。

```
maku ALL=(puni) ALL
```

ユーザー `maku` は、ユーザー `puni` としてあらゆるコマンドを実行できます。
ただし、`puni` が実行権限を持っているコマンドしか実行できません。
`sudo` コマンドはデフォルトで `root` ユーザーとしてコマンド実行しようとするので、`-u` オプションでユーザーを指定して実行する必要があります（例: `sudo -u puni command`）。


/etc/sudoers.d ディレクトリを使う方法
----

多くの Linux ディストリビューションでは、`/etc/sudoers` ファイルの末尾で、次のように __`/etc/sudoers.d`__ ディレクトリ以下のファイルをインクルードするようになっています。

{{< code title="/etc/sudoers（抜粋）" >}}
@includedir /etc/sudoers.d
{{< /code >}}

つまり、このディレクトリに追加の設定ファイルを配置することでも、`sudoers` の設定を行うことができます。
`/etc/sudoers` を直接変更するよりも、カスタマイズした内容を管理しやすくなります。
`/etc/sudoers.d` ディレクトリ以下に設定ファイルを作成（あるいは既存のファイルを編集）する場合も、__`visudo`__ コマンドを使用します。

{{< code lang="console" title="/etc/sudoers.d 以下にファイルを作成／編集" >}}
$ sudo visudo /etc/sudoers.d/xxxx
{{< /code >}}

エディタでファイルが開かれるので、次のような感じで必要な設定を記述します。

{{< code title="/etc/sudoers.d/xxxx" >}}
xxxx ALL=(ALL) NOPASSWD:ALL
{{< /code >}}

`/etc/sudoers.d` ディレクトリ内のファイルのモードは `0400` に設定されている必要がありますが、`visudo` コマンドでファイルを作成した場合は、自動的にこのモードに設定してくれます。


管理者グループ (admin、wheel) を使う方法
----

上記では、個々のユーザーを `/etc/sudoers` に登録する方法を説明しましたが、実はユーザーを特定の管理者グループに所属させるだけで、sudo でコマンド実行できるようになります。

- Debian/Ubuntu の場合 ... __admin__、__sudo__ グループ
- CentOS/RockyLinux の場合 ... __wheel__ グループ

なぜなら、`/etc/sudoers` ファイルに次のようにグループが登録されているからです。

{{< code title="/etc/sudoers 抜粋（Debian の場合）" >}}
# Members of the admin group may gain root privileges
%admin ALL=(ALL) ALL

# Allow members of group sudo to execute any command
%sudo	ALL=(ALL:ALL) ALL
{{< /code >}}

{{< code title="/etc/sudoers 抜粋（CentOS の場合）" >}}
## Allows people in group wheel to run all commands
%wheel	ALL=(ALL)	ALL
{{< /code >}}

よって、管理者権限で sudo 実行したいユーザーを、次のようにグループ登録するだけで済みます。

```console
# Debian/Ubuntu の場合
$ sudo gpasswd -a maku admin

# CentOS/RockyLinux の場合
$ sudo gpasswd -a maku wheel
```

ユーザーを登録するときに、同時にこれらのグループに所属させてしまうこともできます。

```console
$ sudo useradd -m -G admin maku
```

- 参考: [Linux のユーザーを管理する (useradd, userdel, passwd)](/p/7m5k3hx/)
- 参考: [Linux のグループを管理する (groupadd, groupdel, gpasswd)](/p/uexfvcs/)


（おまけ）sudo によるコマンド出力をリダイレクトするときの注意
----

次のように、`sudo` コマンドの出力をリダイレクトして、`root` ユーザーのホームディレクトリ `/root` にファイル保存しようとしても失敗します。

{{< code lang="console" title="うまくいかない例" >}}
$ sudo echo Hello > /root/hello.txt
-sh: 15: cannot create /root/hello.txt: Permission denied
{{< /code >}}

これは、リダイレクト後のファイル書き込みまでは `sudo` の効果が及ばないからです。
このようなケースでは、次のように `sudo` コマンドで `sh` を実行するようにして、リダイレクト処理まで `root` ユーザーで実行すればうまくいきます。

```console
$ sudo sh -c "echo Hello > /root/hello.txt"
```

