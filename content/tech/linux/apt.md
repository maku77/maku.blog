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


アップデート可能なパッケージの一覧を表示する (apt list --upgradable)
----

更新可能なパッケージの一覧を確認できます。

```console
$ apt list --upgradable
Listing... Done
libsystemd0/focal-updates,focal-security 245.4-4ubuntu3.15 amd64 [upgradable from: 245.4-4ubuntu3.14]
libudev1/focal-updates,focal-security 245.4-4ubuntu3.15 amd64 [upgradable from: 245.4-4ubuntu3.14]
```


インストールされているパッケージの一覧を調べる (apt list --installed)
----

```console
$ apt list --installed
```


エラー以外の出力を抑制する (--qq)
----

`apt update` などで APT のパッケージキャッシュを更新する場合、デフォルトでは大量のメッセージが出力されてしまいます。
スクリプトなどで `apt` コマンドの実行を自動化する場合は、このようなメッセージは邪魔なことがあります。
`apt` コマンドの実行時に、エラーメッセージ以外の表示を抑制するには、下記のように __`-qq`__ オプションを指定します。

```console
$ sudo apt -qq update
```


パッケージの詳細情報を表示する (apt show / apt-cache show)
----

```command
$ apt show <パッケージ名>
```

上記のコマンドを使うと、指定したパッケージの詳細情報を表示することができます。
インストールされていないパッケージの情報も見ることができます。

{{< code lang="console" title="例: gnuplot パッケージの詳細情報を表示する" >}}
$ apt show gnuplot
Package: gnuplot
Architecture: all
Version: 5.2.8+dfsg1-2
Priority: optional
Section: universe/math
...
{{< /code >}}


パッケージを検索する (apt search / apt-cache search)
----

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
$ sudo apt install libpthread-stubs0-dev
```

一行説明だけでなく、パッケージの詳細情報も表示したい場合は、__`--full`__ オプションを付けて検索します。

```console
$ apt-cache --full search pthread-stubs
```


パッケージ名からパッケージを検索する (apt-cache pkgnames)
----

```console
$ apt-cache pkgnames <キーワード>
```

上記のコマンドを使うと、指定したキーワードで始まるパッケージ名を列挙することができます。
`apt-cache` コマンドは、APT のパッケージキャッシュから検索を行うため、まだインストールされていないパッケージを探すために使用することができます。
ただし、`apt-cache pkgnames` は、__前方一致でしかキーワードを検索しない__ ことに注意してください。

{{< code lang="console" title="例: gnuplot でパッケージ名が始まるものを検索（前方一致）" >}}
$ apt-cache pkgnames gnuplot
gnuplot
gnuplot-doc
gnuplot-x11
gnuplot-nox
gnuplot-mode
{{< /code >}}

キーワードを省略すると、APT が現在のパッケージキャッシュから認識できるすべてのパッケージ名を表示します（インストールしていないものも含めて）。
以下のように `grep` と組み合わせて使用すれば、キーワードの部分一致でパッケージ名を検索することができます。

{{< code lang="console" title="例: gnuplot をパッケージ名に含むものを検索（部分一致）" >}}
$ apt-cache pkgnames | grep gnuplot
gnuplot
python-gnuplot
gnuplot-doc
libgnuplot-ruby
gnuplot-x11
gnuplot-nox
libgnuplot-ruby1.8
gnuplot-mode
libchart-gnuplot-perl
libgraphics-gnuplotif-perl
{{< /code >}}



パッケージに含まれているファイルを検索する (apt-file)
----

__`apt-file`__ コマンドを使うと、パッケージに含まれているファイルを検索することができます。
`apt-file` は以下のようにして導入します。

```console
$ sudo apt-get install apt-file
$ sudo apt-file update
```

{{< code lang="console" title="例: libGL.so がどのパッケージに含まれているか検索" >}}
$ apt-file search libGL.so
{{< /code >}}


パッケージがどんなファイルで構成されているか調べる (dpkg -L)
----

（Ubuntu 12.04 〜 20.04 で動作確認済み）

Debian/Ubuntu 系の Linux では、deb パッケージ用の管理コマンド __`dpkg`__ を使うことができます（APT は内部で `dpkg` を使用しています）。
あるパッケージが所有しているファイル・ディレクトリの一覧を表示するには次のコマンドを使用します。

```console
$ dpkg -L ＜パッケージ名＞
```

例えば、`libdbus-glib-1-doc` が `apt-get install` されているとして、実際にこのパッケージがどのようなファイルで構成されているかは次のように調べることができます。

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

