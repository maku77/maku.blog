---
title: "XAMPP/Apache の DocumentRoot を設定する"
url: "/p/6h47cno"
date: "2012-04-29"
tags: ["Apache"]
---

httpd.conf を編集
----

例えば、Mac OSX にインストールした XAMPP の Apache の場合は、`/Applications/XAMPP/xamppfiles/etc/httpd.conf` を編集します。

{{< code title="(1) DocumentRoot の変更" >}}
DocumentRoot "/Applications/XAMPP/xamppfiles/htdocs"
↓
DocumentRoot "/Users/maku/website/public_html"
{{< /code >}}

{{< code title="(2) 対応する Directory ディレクトリの変更" >}}
<Directory "/Applications/XAMPP/xamppfiles/htdocs">
↓
<Directory "/Users/maku/website/public_html">
{{< /code >}}


トラブルシューティング
----

http://localhost/ にアクセスしたときに以下のようなエラーが出た場合は、DocumentRoot として設定した、ディレクトリのパーミッションを確認します。

```
Access forbidden!

You don't have permission to access the requested directory.
There is either no index document or the directory is read-protected.
```

例えば、DocumentRoot として、`/aaa/bbb/ccc` を設定した場合、`/aaa`、`/aaa/bbb`、`/aaa/bbb/ccc` 全てに read パーミッションがついている必要があります。

```
$ chmod 0755 /aaa
$ chmod 0755 /aaa/bbb
$ chmod 0755 /aaa/bbb/ccc
```

