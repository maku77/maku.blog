---
title: "SQL の検索結果をソートする (ORDER BY)"
url: "/p/6ebo58o"
date: "2011-02-20"
tags: ["SQL"]
---

`SELECT` によるレコードの検索結果の並び順は不定なので、特定のカラムでソートしたい場合は、明示的に `ORDER BY` 指定を行う必要があります。

{{< code lang="sql" title="書式" >}}
SELECT * FROM tbl ORDER BY col
{{< /code >}}

複数のカラムでソートしたい場合は、カンマでチェック順にカラム名を並べます。

{{< code lang="sql" >}}
SELECT * FROM tbl ORDER BY a, b, c
{{< /code >}}

昇順 (`ASC`)、降順 (`DESC`) を指定するには以下のように末尾に追加するだけです。

{{< code lang="sql" >}}
SELECT * FROM tbl ORDER BY col ASC   -- 昇順 (default)
SELECT * FROM tbl ORDER BY col DESC  -- 降順
{{< /code >}}

`NULL` がどのように扱われるかはデータベースによって異なります。

