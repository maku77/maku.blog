---
title: "Linuxコマンド: パッチファイルを作成・適用する (diff, patch)"
url: "/p/nyehhfd"
date: "2007-05-11"
tags: ["Linux"]
---

パッチの基本 (diff/patch)
----

### パッチファイルの作成

{{< code lang="cpp" title="sample.cpp" >}}
#include <iostraem>

int main() {
    std::cout << "AAA" << std::endl;
    return 0;
}


ew/sample.cpp
#include <iostraem>

int main() {
    std::cout << "BBB" << std::endl;
    return 0;
}
{{< /code >}}

例えば、上記のような `sample.cpp` の内容を `new/sample.cpp` に変更するためのパッチファイルを作成するには、次のように `diff` コマンドを実行します。

{{< code >}}
$ diff -u sample.cpp new/sample.cpp > sample.patch
{{< /code >}}

`-u` オプションは unified diff 形式で差分を出力することを示しています（他の形式でも出力可能ですが、`patch` 用のパッチファイルを作成するときは、ほとんどの場合 unified diff 形式が使用されるようです）。

これで、次のようなパッチファイルが生成されます。

{{< code lang="patch" title="sample.patch" >}}
--- sample.cpp  2007-05-11 20:36:13.265625000 +0900
+++ new/sample.cpp      2007-05-11 20:26:31.875000000 +0900
@@ -1,6 +1,6 @@
 #include <iostraem>

 int main() {
-    std::cout << "AAA" << std::endl;
+    std::cout << "BBB" << std::endl;
     return 0;
 }
{{< /code >}}

差分を表示している各行の 1 文字目は次のような意味を持っています。

- `+` ... 追加される行
- `-` ... 削除される行
- `space` ... 変化しない行

変化しない行の情報を含めることにより、パッチを当てるときに、前後関係を考慮しながら処理できるようになります。
FreeBSD の `diff` の `man` ページにも、パッチ作業をうまく働かせるために、この変化しない行が必要であることが明記されています。

### パッチファイルの適用

作成したパッチファイルを適用するには、対象のファイルがあるディレクトリへ移動し、以下のように実行します。

{{< code >}}
$ patch < sample.patch
patching file sample.cpp
{{< /code >}}

パッチを適用する前のファイルをバックアップしておきたい場合は、`-b` オプションをつけて patch コマンドを実行します。

{{< code >}}
$ patch -b < sample.patch
patching file sample.cpp
{{< /code >}}

`sample.cpp` が修正されると同時に、バックアップファイルとして `sample.cpp.orig` が生成されます。


ディレクトリ単位での diff/patch
----

### ディレクトリ内のファイルをまとめての diff を取る

{{< code >}}
$ diff -urN old_dir new_dir > hoge.patch
{{< /code >}}

`-r` オプションでディレクトリを再帰的に検索、`-N` オプションで削除、追加されたファイルについても `diff` を取るようになります。

### ディレクトリ内のファイルにまとめて patch を当てる

変更対象のファイルのあるディレクトリに `hoge.patch` を置いて、そこで次のように実行します。

{{< code >}}
$ patch < hoge.patch
{{< /code >}}

