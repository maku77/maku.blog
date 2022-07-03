---
title: "Linuxメモ: apt (apt-get) コマンドの使い方メモ"
url: "/p/rdq2cnx"
date: "2012-02-21"
lastmod: "2022-01-30"
tags: ["Linux"]
---

apt と dpkg の違い
---

Debian/Ubuntu 系 Linux のパッケージ管理には `apt` (`apt-get`) コマンドを使いますが、 Low-Level なパッケージ管理コマンドとして `dpkg` コマンドも備えています。

- 参考: [DebianPackageManagement - Debian Wiki](https://wiki.debian.org/DebianPackageManagement)
- 参考: [AptCLI - Debian Wiki](https://wiki.debian.org/AptCLI)

`dpkg` コマンドは純粋にシステムにパッケージをインストールする部分のみを担うため、インターネットからパッケージをダウンロードしたり、依存関係を自動解決したりする仕組みは備えていません。
そこで、`apt` は `dpkg` をラップする形でこれらの機能を提供します。

- __dpkg__ ... Debian/Ubuntu にパッケージをインストールする仕組み
- __apt__ ... パッケージの依存関係を解決してダウンロードし、パッケージをインストールする仕組み（ただし、インストールには内部的に dpkg が使われる）


apt と apt-get の違い
----

{{% private %}}
[Difference Between apt and apt-get Explained - It's FOSS](https://itsfoss.com/apt-vs-apt-get-difference/)
{{% /private %}}

2016 年にリリースされた Ubuntu 16.04 の頃から、`apt-get` コマンドの代わりに `apt` コマンドが使われるようになりました。
2020 年にリリースされた Ubuntu 20.04 でも `apt-get`、`apt-cache` コマンドは使用可能ですが、今後は主に `apt` コマンドを使っていくことになりそうです。

`apt` はもともと Linux Mint ユーザーによって作られたコマンドで、これまで `apt-get` や `apt-cache` などに散らばっていたコマンド群を整理して使いやすくしています。

- `apt-get install` → `apt install`
- `apt-get remove` → `apt remove`
- `apt-cache search` → `apt search`
- `apt-cache show` → `apt show`

`apt` によるパッケージのインストール中にはプログレスバーが表示されるなど、表示上の見やすさも考慮されています。
`apt` が進化している段階では、`apt-get` が廃止されてしまうことはないでしょうが、まずはパッケージ管理は `apt` でできるだけ済ませ、どうしても `apt-get`、`apt-cache` が必要な場合にはそちらを使う、という考え方でよいと思います。


よく使うコマンドの一覧 (apt --help)
----

| コマンド | 説明 |
| ---- | ---- |
| [apt update](#update) | パッケージリストを更新する / update list of available packages |
| [apt upgrade](#upgrade) | インストール済みのパッケージをまとめて更新する / upgrade the system by installing/upgrading packages |
| [apt list](#list) | パッケージの一覧を表示する / list packages based on package names |
| [apt search](#search) | パッケージを検索する / search in package descriptions |
| [apt show](#show) | パッケージの詳細情報を表示する / show package details |
| `apt install` | パッケージをインストールする / install packages |
| `apt reinstall` | パッケージを再インストールする / reinstall packages |
| `apt remove` | パッケージを削除する / remove packages |
| `apt autoremove` | 使用していないパッケージを削除する / remove automatically all unused packages |
| `apt full-upgrade` | upgrade the system by removing/installing/upgrading packages |
| `apt edit-sources` | edit the source information file |
| `apt satisfy` | satisfy dependency strings |


パッケージリストを更新する (apt update) {#update}
----

__`apt update`__ コマンドを実行すると、ローカルのパッケージリスト情報を更新します。

```console
$ apt update
Get:1 http://ports.ubuntu.com/ubuntu-ports jammy InRelease [270 kB]
Get:2 http://ports.ubuntu.com/ubuntu-ports jammy-updates InRelease [109 kB]
Get:3 http://ports.ubuntu.com/ubuntu-ports jammy-backports InRelease [99.8 kB]
Get:4 http://ports.ubuntu.com/ubuntu-ports jammy-security InRelease [110 kB]
...（省略）...
8 packages can be upgraded. Run 'apt list --upgradable' to see them.
```

[apt search](#search) などでパッケージを検索したときに何の情報も表示されないときは、このコマンドを先に実行すると見つかるようになります。
出力の最後には、現在インストールされているパッケージのうち、更新可能なパッケージの数が表示されます。
表示に従って [apt list --upgradable](#upgradable) コマンドを実行すると、それらのパッケージ情報を確認できます。

{{% note title="エラー以外の出力を抑制する (--qq)" %}}
`apt update` などで APT のパッケージキャッシュを更新する場合、デフォルトでは大量のメッセージが出力されてしまいます。
スクリプトなどで `apt` コマンドの実行を自動化する場合は、このようなメッセージは邪魔になるかもしれません。
`apt` コマンドの実行時に、エラーメッセージ以外の表示を抑制するには、次のように __`-qq`__ オプションを指定します。

```console
$ apt -qq update
8 packages can be upgraded. Run 'apt list --upgradable' to see them.
```
{{% /note %}}


インストール済みのパッケージを更新する (apt upgrade)
----

システムにインストールされているパッケージのうち更新できるものをまとめて更新するには __`apt upgrade`__ コマンドを実行します。
パッケージの最新情報が必要になるため、通常は `apt update` に続けて実行することになります。

```console
$ apt update   # パッケージリストの更新
$ apt upgrade  # 更新可能なパッケージをまとめて更新
```

次のように実行すると、どのように更新されるかをシミュレーションすることができます。

```console
$ apt --simulate upgrade  # 実際には更新されない
```


パッケージの一覧を表示する (apt list) {#list}
----

### パッケージリストに登録されているパッケージ名の一覧を表示する (apt list)

何もオプションを指定せずに __`apt list`__ コマンドを実行すると、[apt update](#update) で取得済みのパッケージ情報の一覧を表示します。
何万件ものパッケージ情報がズラーッと表示されます。

```console
$ apt list
Listing...
0ad-data-common/jammy 0.0.25b-1 all
0ad-data/jammy 0.0.25b-1 all
0ad/jammy 0.0.25b-2 arm64
...（省略）...
```

### インストールされているパッケージの一覧を表示する (apt list --installed)

システムにインストールされているパッケージの一覧を表示するには、__`--installed`__ オプションを指定します。

```console
$ apt list --installed
Listing... Done
adduser/jammy,now 3.118ubuntu5 all [installed]
apt-file/jammy,now 3.2.2 all [installed]
apt/jammy,now 2.4.5 arm64 [installed]
...（省略）...
```

### アップデート可能なパッケージの一覧を表示する (apt list --upgradable) {#upgradable}

[apt update](#update) コマンドでパッケージリストを更新したときに、更新可能なパッケージの数が表示されることがあります。
そのような場合は、__`apt list --upgradable`__ コマンドで更新可能なパッケージの一覧を確認できます。

```console
$ apt list --upgradable
Listing... Done
e2fsprogs/jammy-updates,jammy-security 1.46.5-2ubuntu1.1 arm64 [upgradable from: 1.46.5-2ubuntu1]
libcom-err2/jammy-updates,jammy-security 1.46.5-2ubuntu1.1 arm64 [upgradable from: 1.46.5-2ubuntu1]
libext2fs2/jammy-updates,jammy-security 1.46.5-2ubuntu1.1 arm64 [upgradable from: 1.46.5-2ubuntu1]
...（省略）...
```


パッケージの詳細情報を表示する (apt show / apt-cache show) {#show}
----

```command
$ apt show <パッケージ名>
```

__`apt show`__ コマンドを使うと、指定したパッケージの詳細情報を表示することができます。
この情報は [apt update](#update) で取得したパッケージリスト情報をもとに表示するため、インストールされていないパッケージの情報も確認することができます。

{{< code lang="console" title="例: gnuplot パッケージの詳細情報を表示" >}}
$ apt show gnuplot
{{< /code >}}

{{% accordion title="実行結果" %}}
```
Package: gnuplot
Version: 5.4.2+dfsg2-2
Priority: optional
Section: universe/math
Origin: Ubuntu
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Original-Maintainer: Debian Science Team <debian-science-maintainers@lists.alioth.debian.org>
Bugs: https://bugs.launchpad.net/ubuntu/+filebug
Installed-Size: 32.8 kB
Depends: gnuplot-qt | gnuplot-x11 | gnuplot-nox
Suggests: gnuplot-doc
Homepage: http://gnuplot.sourceforge.net/
Download-Size: 3576 B
APT-Sources: http://ports.ubuntu.com/ubuntu-ports jammy/universe arm64 Packages
Description: Command-line driven interactive plotting program.
```
{{% /code %}}

{{< code lang="console" title="例: ansible パッケージの詳細情報を表示" >}}
$ apt show ansible
{{< /code >}}

{{% accordion title="実行結果" %}}
```
Package: ansible
Version: 2.10.7+merged+base+2.10.8+dfsg-1
Priority: optional
Section: universe/admin
Origin: Ubuntu
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Original-Maintainer: Lee Garrett <debian@rocketjump.eu>
Bugs: https://bugs.launchpad.net/ubuntu/+filebug
Installed-Size: 204 MB
Depends: python3-cryptography, python3-jinja2, python3-packaging, python3-yaml, python3:any, openssh-client | python3-paramiko (>= 2.6.0), python3-pycryptodome, python3-distutils, python3-dnspython, python3-httplib2, python3-netaddr
Recommends: python3-argcomplete, python3-jmespath, python3-kerberos, python3-libcloud, python3-selinux, python3-winrm, python3-xmltodict
Suggests: cowsay, sshpass
Breaks: ansible-base (<= 2.10.5+dfsg-2)
Replaces: ansible-base (<= 2.10.5+dfsg-2)
Homepage: https://www.ansible.com
Download-Size: 17.5 MB
APT-Sources: http://ports.ubuntu.com/ubuntu-ports jammy/universe arm64 Packages
Description: Configuration management, deployment, and task execution system
```
{{% /accordion %}}


パッケージを検索する (apt search / apt-cache search) {#search}
----

### キーワードでパッケージを検索する

```console
$ apt search <キーワード>
$ apt-cache search <キーワード>
```

上記のコマンドを使うと、パッケージ情報の中に指定したキーワードが含まれているパッケージを検索することができます（`apt` と `apt-cache` で出力形式が異なります）。
あるソフトウェアをビルド中に何らかのライブラリが足りないというエラーが出た場合、このコマンドを使って、インストールしなければいけないパッケージを見つけることができます。
例えば、`configure` コマンドを実行して、`No package 'pthread-stubs' found` というエラーが出たら、以下のように `pthread-stubs` というキーワードで検索してみます。

```console
$ apt-cache search pthread-stubs
libpthread-stubs0 - pthread stubs not provided by native libc
libpthread-stubs0-dev - pthread stubs not provided by native libc, development files
```

これで、`libpthread-stubs0-dev` をインストールする必要があることがわかります。

```console
$ apt install libpthread-stubs0-dev
```

一行説明だけでなく、パッケージの詳細情報も表示したい場合は、__`--full`__ オプションを付けて検索します。

```console
$ apt-cache --full search pthread-stubs
```

### パッケージ名でパッケージを検索する (apt-cache pkgnames)

```console
$ apt-cache pkgnames <キーワード>
```

上記のコマンドを使うと、指定したキーワードで始まるパッケージ名を列挙することができます。
`apt-cache` コマンドは、APT のパッケージキャッシュから検索を行うため、まだインストールされていないパッケージを探すために使用することができます。
ただし、`apt-cache pkgnames` は、__前方一致でしかキーワードを検索しない__ ことに注意してください。

{{< code lang="console" title="例: gnuplot でパッケージ名が始まるものを検索（前方一致）" >}}
$ apt-cache pkgnames gnuplot
gnuplot-doc
gnuplot-nox
gnuplot-mode
...（省略）...
{{< /code >}}

これは、次のように [apt list](#list) の結果を絞り込んだのとほぼ同様の結果になります。

```console
$ apt list | grep -E '^gnuplot'
gnuplot-data/jammy 5.4.2+dfsg2-2 all
gnuplot-doc/jammy 5.4.2+dfsg2-2 all
gnuplot-mode/jammy 1:0.7.0-2014-12-31-2 all
...（省略）...
```

`apt-cache pkgnames` コマンドでキーワードを省略すると、APT が現在のパッケージキャッシュから認識できるすべてのパッケージ名を表示します（インストールしていないものも含めて）。
以下のように `grep` と組み合わせて使用すれば、キーワードの部分一致でパッケージ名を検索することができます。

{{< code lang="console" title="例: gnuplot をパッケージ名に含むものを検索（部分一致）" >}}
$ apt-cache pkgnames | grep gnuplot | sort
gnuplot
gnuplot-doc
gnuplot-mode
gnuplot-nox
gnuplot-x11
libchart-gnuplot-perl
libgnuplot-ruby
libgnuplot-ruby1.8
libgraphics-gnuplotif-perl
python-gnuplot
{{< /code >}}


パッケージに含まれているファイルを検索する (apt-file)
----

__`apt-file`__ コマンドを使うと、パッケージに含まれているファイルを検索することができます。
`apt-file` は以下のようにして導入します。

```console
$ apt install apt-file  # apt-file コマンドをインストール
$ apt-file update       # パッケージ情報を更新
```

{{< code lang="console" title="例: libssl.so がどのパッケージに含まれているか検索" >}}
$ apt-file search libssl.so
android-libboringssl: /usr/lib/aarch64-linux-gnu/android/libssl.so.0
android-libboringssl-dev: /usr/lib/aarch64-linux-gnu/android/libssl.so
libssl-dev: /usr/lib/aarch64-linux-gnu/libssl.so
libssl3: /usr/lib/aarch64-linux-gnu/libssl.so.3
{{< /code >}}


パッケージがどんなファイルで構成されているか調べる (dpkg -L)
----

（Ubuntu 12.04 〜 20.04 で動作確認済み）

Debian/Ubuntu 系の Linux では、deb パッケージ用の管理コマンド __`dpkg`__ を使うことができます（APT は内部で `dpkg` を使用しています）。
あるパッケージが所有しているファイル・ディレクトリの一覧を表示するには次のコマンドを使用します。

```console
$ dpkg -L ＜パッケージ名＞
```

例えば、`libdbus-glib-1-doc` が `apt install` されているとして、実際にこのパッケージがどのようなファイルで構成されているかは次のように調べることができます。

```console
$ dpkg -L libdbus-glib-1-doc
/.
/usr
/usr/share
/usr/share/gtk-doc
/usr/share/gtk-doc/html
/usr/share/gtk-doc/html/dbus-glib
/usr/share/gtk-doc/html/dbus-glib/ch01.html
/usr/share/gtk-doc/html/dbus-glib/ch02.html
/usr/share/gtk-doc/html/dbus-glib/ch03.html
...
```

