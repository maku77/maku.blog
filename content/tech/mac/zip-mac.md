---
title: "macOS のコマンドラインで ZIP ファイルを作成する"
url: "/p/2it3bjs"
date: "2021-04-06"
tags: ["mac", "zip", "圧縮・展開"]
---

ZIP ファイルを作成する（zip コマンド）
----

macOS に標準で付属している `zip` コマンドを使って、ファイル群を ZIP ファイルとしてアーカイブ（圧縮）できます。

### ファイルを 1 つずつ指定する

```console
$ zip sample.zip 1.txt 2.txt 3.txt
```

{{< code title="作成される ZIP ファイル" >}}
sample.zip
  +-- 1.txt
  +-- 2.txt
  +-- 3.txt
{{< /code >}}

### ディレクトリごとまとめて ZIP 化する (-r)

```console
$ zip -r sample.zip dir1 dir2
```

{{< code title="作成される ZIP ファイル" >}}
sample.zip
  +-- dir1/
  |     +-- 1.txt
  |     +-- 2.txt
  |     +-- 3.txt
  +-- dir2/
        +-- 4.txt
        +-- 5.txt
        +-- 6.txt
{{< /code >}}

### トップディレクトリを含めずに ZIP 化する

{{< code lang="console" title="例: src ディレクトリ以下のファイルだけ圧縮" >}}
$ (cd src && zip -r ../src.zip ./*)
{{< /code >}}

コマンド全体を括弧 `()` で囲んでいるのは、コマンド実行後にもとのディレクトリに戻るためです。

{{< code title="作成される ZIP ファイル" >}}
src.zip
  +-- 1.txt
  +-- 2.txt
  +-- 3.txt
{{< /code >}}

### 除外するファイルを指定する (-x)

{{< code lang="console" title="例: ドットで始まるファイルやディレクトリを含めない" >}}
$ zip -r src.zip src -x "*/.*"
{{< /code >}}

`-x` オプションは複数指定することができます。

```console
$ zip -r src.zip src -x "*/.*" -x "*/__pycache__/*"
```

