---
title: "Linuxコマンド: テンポラリディレクトリを作成する (mktemp)"
linkTitle: "テンポラリディレクトリを作成する (mktemp)"
url: "/p/oo32y2x"
date: "2011-06-30"
tags: ["Linux"]
---

`mktemp` コマンドを使用して、一時ディレクトリを作成することができます。

{{< code >}}
$ mktemp -d
/tmp/tmp.LE04SMiEzq
{{< /code >}}

`mktemp` コマンドは、作成したファイルやディレクトリのパスを表示するので、シェルスクリプト内で以下のようにすれば、そのパスを変数に格納することができます。

{{< code lang="bash" >}}
tempdir=`mktemp -d`
{{< /code >}}

