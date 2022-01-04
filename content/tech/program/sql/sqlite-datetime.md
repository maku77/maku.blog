---
title: "SQLite の日時関連の処理"
url: "/p/qwtoj94"
date: "2007-02-17"
tags: ["SQL", "SQLite"]
---


現在の日時を取得する（テキスト形式）
----

{{< code title="日付＋時刻" >}}
sqlite> select datetime('now')
2007-02-17 07:53:30
{{< /code >}}

{{< code title="日付のみ" >}}
sqlite> select date('now')
2007-02-17
{{< /code >}}

{{< code title="時刻のみ" >}}
sqlite> select time('now')
07:53:30
{{< /code >}}

これらのデータ型は TEXT 型です。

{{< code >}}
sqlite> select typeof(datetime('now'));
text
{{< /code >}}


現在の日時を現地時間で取得する（テキスト形式）
----

{{< code >}}
sqlite> select datetime('now', 'localtime')
2007-02-17 16:53:30

sqlite> select date('now', 'localtime')
2007-02-17

sqlite> select time('now', 'localtime')
16:53:30
{{< /code >}}


ある標準時を現地時刻へ変換する
----

{{< code >}}
sqlite> select datetime('2007-02-17 21:30:00', 'localtime');
2007-02-18 06:30:00

sqlite> select time('21:30:00', 'localtime');
06:30:00
{{< /code >}}

