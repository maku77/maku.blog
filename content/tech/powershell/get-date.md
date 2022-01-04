---
title: "PowerShell: 現在の日時を ISO 8601 (YYYY-MM-DD) フォーマットで取得する (Get-Date)"
linkTitle: "現在の日時を ISO 8601 (YYYY-MM-DD) フォーマットで取得する (Get-Date)"
url: "/p/9a9n6se"
date: "2020-03-19"
tags: ["PowerShell"]
---

PowerShell の [Get-Date](https://docs.microsoft.com/en-us/powershell/module/microsoft.powershell.utility/get-date) コマンドレットを使用すると、現在の日時を表す文字列を取得することができます。

Format、UFormat パラメータで日時の書式を指定する
----

### Format

`Get-Date` の `Format` パラメータを指定することで、任意の書式で日時文字列を取得することができます。

{{< code >}}
PS> Get-Date -Format "yyyy-MM-dd HH:mm:ss"
2020-03-19 15:23:09

PS> Get-Date -Format "yyyy-MM-ddTHH:mm:ssK"  # ISO 8601
2020-03-19T15:23:09+09:00
{{< /code >}}

### UFormat

`UFormat` パラメータを使うと、もう少しシンプルに記述できたりします（[指定できる記号の一覧はこちら](https://docs.microsoft.com/en-us/powershell/module/microsoft.powershell.utility/get-date?view=powershell-7#notes)）。

{{< code >}}
PS> Get-Date -UFormat "%F %T"
2020-03-19 15:26:42

PS> Get-Date -UFormat "%Y-%m-%d %H:%M:%S"  # 同上
2020-03-19 15:26:42

PS> Get-Date -UFormat "%FT%T%Z"  # ISO 8601
2020-03-19T15:26:42+09
{{< /code >}}


DateTime オブジェクトの ToString メソッドを使用する
----

`Get-Date` で先に `DateTime` オブジェクトを取得しておいて、`ToString` 関数で日時を表す文字列に変換するという方法もあります。

{{< code >}}
PS> (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
2020-03-19 15:11:37
{{< /code >}}

この方法を使うと、`DateTime` オブジェクトで日時データを編集してから文字列に変換することができます。
例えば、次の例では、現在から 10 日後の日時を取得しています。

{{< code >}}
PS> (Get-Date).AddDays(10).ToString("yyyy-MM-dd HH:mm:ss")
2020-03-29 15:11:37
{{< /code >}}

他にも次のような感じで日時の演算を行えます。

- `(Get-Date).AddYears(10)`: 10 年後
- `(Get-Date).AddYears(-5)`: 5 年前
- `(Get-Date).AddMonths(10)`: 10 ヵ月後
- `(Get-Date).AddMonths(-5)`: 5 ヵ月前
- `(Get-Date).AddDays(10)`: 10 日後
- `(Get-Date).AddDays(-5)`: 5 日前
- `(Get-Date).AddHours(10)`: 10 時間後
- `(Get-Date).AddHours(-5)`: 5 時間前
- `(Get-Date).AddMinutes(10)`: 10 分後
- `(Get-Date).AddMinutes(-5)`: 5 分後
- `(Get-Date).AddMinutes(10)`: 10 分後
- `(Get-Date).AddMinutes(-5)`: 5 分後

### UTC 時刻で表示する

世界協定時 (UTC) で日時文字列を取得したい場合は、`ToString` の前に `ToUniversalTime()` を挟みます。

{{< code >}}
PS> (Get-Date).AddDays(10).ToUniversalTime().ToString("yyyy-MM-ddTHH:mm:ssZ")
2020-03-29T06:11:37Z
{{< /code >}}

ローカルタイムではなく、UTC 時刻であることを明確にするために、末尾に `Z` を付けておきましょう（詳細は ISO 8601 で検索）。

