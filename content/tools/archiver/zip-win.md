---
title: "Windows のコマンドラインで ZIP ファイルを作成する (Compress-Archive)"
url: "/p/vin35f7"
date: "2021-03-03"
tags: ["Windows", "zip", "圧縮・展開"]
---

Windows のコマンドプロンプト（あるいは PowerShell）から zip ファイルを作成するには、PowerShell の [Compress-Archive コマンド](https://docs.microsoft.com/ja-jp/powershell/module/Microsoft.PowerShell.Archive/Compress-Archive) を使用します。
特に何もインストールしなくても実行できます。


指定したディレクトリを zip 圧縮する
----

{{< code title="src ディレクトリを dst.zip にアーカイブ" >}}
C:\> powershell compress-archive src dst
{{< /code >}}

{{< code title="作成される zip ファイル" >}}
dst.zip
  +-- src/
       +-- file1
       +-- file2
       +-- file3
{{< /code >}}

出力ファイル名 `dst.zip` の `.zip` は省略することができます。
`dst.zip` を展開すると、ルートに `src` ディレクトリが現れる状態になります。


ディレクトリ内のファイルのみを zip 圧縮する
----

ルートディレクトリ（この例では `src`）を zip ファイルに含めずに、そのディレクトリ内のファイル群だけを zip ファイルにまとめたいときは、次のように __`src/*`__ と指定します。

{{< code title="src ディレクトリの中身だけを dst.zip にアーカイブ" >}}
C:\> powershell compress-archive src/* dst
{{< /code >}}

{{< code title="作成される zip ファイル" >}}
dst.zip
  +-- file1
  +-- file2
  +-- file3
{{< /code >}}


複数のディレクトリやファイルを zip 圧縮する
----

圧縮対象のディレクトリやファイルを複数指定するには、次のようにカンマ (`,`) で列挙します。
カンマの前後にはスペースを入れないことに注意してください。

{{< code title="build 以下のファイルと libs ディレクトリをアーカイブ" >}}
C:\> powershell compress-archive build/*,libs dst
{{< /code >}}


zip ファイルを上書きする
----

すでに同じ名前の zip ファイルが存在するときに、強制的に上書きするときは __`-Force`__ オプションを指定します。

{{< code title="すでに dst.zip があっても上書き" >}}
C:\> powershell compress-archive -Force src/* dst
{{< /code >}}

