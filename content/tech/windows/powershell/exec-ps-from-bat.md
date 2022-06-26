---
title: "PowerShell: バッチファイルから PowerShell を呼び出して結果を変数に格納する"
linkTitle: "バッチファイルから PowerShell を呼び出して結果を変数に格納する"
url: "/p/372dx46"
date: "2020-03-18"
tags: ["PowerShell", "バッチファイル"]
---

Linux や mac では、外部コマンドをバッククォート (`) で囲んでやるだけで実行結果を取得できますが、バッチファイルで同じようなことをするには `FOR /F` コマンドを使用します。
本来は、コマンドの出力結果を一行ずつ処理するためのコマンドですが、次のようにすれば、コマンドの実行結果を一行だけ変数に格納できます。

{{< code lang="batch" >}}
FOR /F "usebackq delims=" %%A IN (`外部コマンド`) DO set 変数名=%%A
{{< /code >}}

- FOR ループのパラメータの意味
    - `/F`: コマンドの出力結果をループ処理する
    - `usebackq`: バッククォートで囲まれた文字列全体を外部コマンドとみなす
    - `delims=`: コマンド実行結果にスペースが含まれていても分割せずに取得（ここではデリミタ文字をなくしている）

上記の `外部コマンド` のところで、**`powershell`** コマンドを実行すれば、PowerShell で実行した結果をバッチファイル内の変数で受け取ることができます。

例えば、次のバッチファイル (`next-month.bat`) では、1か月後の日時を PowerShell で求めて、その結果をバッチファイル内の `next_month` 変数に取得しています。

{{< code lang="batch" title="next-month.bat" >}}
@echo off
setlocal
FOR /F "usebackq delims=" %%A IN (`powershell "(Get-Date).AddMonths(1).ToString('yyyy-MM-dd')"`) DO set next_month=%%A
echo %next_month%
{{< /code >}}

次のように PowerShell で実行するコマンドを分離しておくと見やすいかもしれません。
`ps_command` 変数の内容を置き換えるだけで、別のコマンドに対応できます。

{{< code lang="batch" >}}
set ps_command=`powershell "(Get-Date).AddMonths(1).ToString('yyyy-MM-dd')"`
FOR /F "usebackq delims=" %%A IN (%ps_command%) DO set result=%%A
echo %result%
{{< /code >}}

{{< code title="実行結果" >}}
C:\> next-month
2020-04-19
{{< /code >}}

