---
title: "gnuplot: gnuplot の基本設定"
url: "/p/je8stcd"
date: "2002-08-28"
tags: ["gnuplot"]
weight: 101
---

初期化ファイル (~/.gnuplot or gnuplot.ini)
----

初期化ファイルは gnuplot を起動した時に、自動的に読み込まれるファイルです。
Unix の場合は **`~/.gnuplot`**、Windows の場合は **`gnuplot.ini`** が読み込まれます。
`gnuplot.ini` は次に説明する「作業フォルダ」内に置きます。
例えば、作業フォルダを **`D:\home\gnuplot`** とした場合は、よく使う関数を `D:\home\gnuplot\lib\func.gp` などに書いておいて、`gnuplot.ini` で次のように起動時に読み込むようにしておくと便利です。

{{< code title="gnuplot.ini" >}}
load 'lib/func.gp'
{{< /code >}}


gnuplot のホームディレクトリの設定
----

gnuplot を起動した時にカレントとなるディレクトリを指定しておくと便利です。
デフォルトでは、gnuplot の実行ファイルのあるディレクトリがカレントディレクトリになっているので、出力したファイルがそのディレクトリにできてしまいます。
起動時のカレントディレクトリを変更するには次のようにします。

{{< code title="gnuplot.ini (~/.gnuplot)" >}}
cd 'D:\home\gnuplot'
{{< /code >}}

初期化ファイルの中で、ディレクトリを移動しているだけです。

あるいは、Windows では `wgnuplot.exe` のショートカットを作成して、そのプロパティの作業フォルダで指定する方法もあります。

1. `wgnuplot.exe` のショートカット右クリック → <samp>プロパティ</samp>
2. <samp>ショートカット</samp> タブの <samp>作業フォルダ</samp> に `D:\home\gnuplot` などを設定

上のように設定してショートカットをダブルクリックすると、好きなディレクトリで作業を始められます。
この方法を使うと、ショートカットごとに作業ディレクトリを変更することができます。

