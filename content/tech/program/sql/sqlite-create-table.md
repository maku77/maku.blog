---
title: "SQLite のテーブル作成に関するメモ"
url: "/p/2iccv8a"
date: "2010-08-01"
tags: ["SQL", "SQLite"]
---

SQLite 3 のフィールド型
----

テーブル作成時 (`CREATE TABLE`) に指定したフィールド定義に含まれているテキストにより、以下のようにフィールド型が決定されます。

- <b>INTEGER 型</b>: `INT` という文字列を含む場合。
- <b>TEXT 型</b>: `CHAR`、`CLOB`、`TEXT` を含む場合。
- <b>NONE 型</b>: `BLOB` を含む型、あるいはデータ型が特定されない場合。
- <b>NUMERIC 型</b>: それ以外の場合。


テーブルが存在しない場合だけテーブルを作成する (IF NOT EXISTS)
----

{{< code lang="sql" >}}
CREATE TABLE IF NOT EXISTS tbl(id INTEGER PRIMARY KEY, name TEXT, bin BLOB);
{{< /code >}}

逆に、テーブルが存在する場合だけテーブルを削除するということもできます。
この場合は、`IF NOT EXISTS` の代わりに、**`IF EXISTS`** を使用します。

{{< code lang="sql" >}}
DROP TABLE IF EXISTS tbl;
{{< /code >}}


NULL の追加を禁止する (NOT NULL)
----

フィールドの定義時に、**`NOT NULL`** というフィールド制約を加えておくと、データ追加時にそのフィールドの値を省略できないようになります。

{{< code lang="sql" >}}
CREATE TABLE tbl(f1 NOT NULL, f2, f3);
{{< /code >}}

{{< code title="動作確認" >}}
sqlite> INSERT INTO tbl(f1, f2) VALUES(100, 200);
sqlite> INSERT INTO tbl(f2, f3) VALUES(200, 300);
SQL error: tbl.f1 may not be NULL
{{< /code >}}


同じ値の入力を禁止する (UNIQUE)
----

フィールドの定義時に、`UNIQUE` というフィールド制約を加えることで、別のレコードに入っている値と同じ値を追加できないようになります。

{{< code lang="sql" >}}
CREATE TABLE tbl(name UNIQUE);
{{< /code >}}

{{< code title="動作確認" >}}
sqlite> INSERT INTO tbl(name) VALUES('Jack');
sqlite> INSERT INTO tbl(name) VALUES('Jack');
SQL error: column name is not unique
{{< /code >}}


自動的に連番を振る (INTEGER PRIMARY KEY)
----

フィールドの定義で、`INTEGER PRIMARY KEY` を指定しておくと、データ追加時にそのフィールドの値を指定しなかった場合に、自動的に連番を振ってくれます。

{{< code lang="sql" >}}
CREATE TABLE tbl(id INTEGER PRIMARY KEY);
{{< /code >}}

{{< code title="動作確認" >}}
sqlite> INSERT INTO tbl(id) VALUES(NULL);
sqlite> INSERT INTO tbl(id) VALUES(NULL);
sqlite> INSERT INTO tbl(id) VALUES(NULL);
sqlite> SELECT * FROM tbl;
1
2
3
{{< /code >}}


自動的に日付・時刻を格納する (DEFAULT CURRENT_TIMESTAMP)
----

フィールドの定義に日時関連の `DEFAULT` 制約を設定しておくと、データの追加時にそのフィールドに何も指定しない場合に現在の日時をセットしてくれます。

- <b>DEFAULT CURRENT_DATE</b>: 現在の日付 (2010-08-21)
- <b>DEFAULT CURRENT_TIME</b>: 現在の時刻 (14:30:56)
- <b>DEFAULT CURRENT_TIMESTAMP</b>: 現在の日時 (2010-08-21 14:30:56)

上記の日時は、UTC で格納されるので、実際にこの値を取得してローカルタイムで表示する場合は、時差などを考慮して時刻を表示する必要があります。

{{< code lang="sql" title="テーブルの作成例" >}}
CREATE TABLE tbl(
    id INTEGER PRIMARY KEY,
    date DEFAULT CURRENT_TIMESTAMP
);
{{< /code >}}

{{< code title="動作確認" >}}
sqlite> INSERT INTO tbl(id) VALUES(1);
sqlite> INSERT INTO tbl(id) VALUES(2);
sqlite> INSERT INTO tbl(id) VALUES(3);
sqlite> SELECT * FROM tbl;
1||2010-07-31 19:15:27
2||2010-07-31 19:15:40
3||2010-07-31 19:15:44
{{< /code >}}

