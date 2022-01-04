---
title: "PowerShell: 環境変数を扱う"
url: "/p/jodtbr8"
date: "2020-03-19"
tags: ["PowerShell"]
---

環境変数の値を参照する
----

PowerShell スクリプトの中から環境変数の値を取得するには、**`$env:変数名`** を参照します。

{{< code lang="powershell" title="sample.ps1" >}}
echo $env:USERPROFILE
{{< /code >}}


環境変数が設定されているかどうかを調べる
----

環境変数が定義されているかどうかを調べるには、以下のような条件分岐を使用します。

{{< code lang="powershell" title="sample.ps1" >}}
if ($env:HOME -eq $null) {
    Write-Host 'HOME is not set'
}
if ($env:HOME -ne $null) {
    Write-Host 'HOME is set'
}
{{< /code >}}

値が空である場合も「定義されていない」とみなしたいのであれば、`IsNullOrEmpty()` でチェックします。

{{< code lang="powershell" >}}
if ([string]::IsNullOrEmpty($env:HOME)) {
    Write-Host 'HOME is not set'
    exit
}
{{< /code >}}

