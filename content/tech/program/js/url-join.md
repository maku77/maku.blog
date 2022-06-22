---
title: "Node.js で URL のパスを結合する (url-join)"
url: "p/oj9nzgt/"
date: "2022-06-22"
tags: ["Node.js", "URL"]
---

José F. Romaniello 氏 (jfromaniello) が公開している NPM パッケージの [url-join](https://www.npmjs.com/package/url-join) を使うと、バラバラになった URL のパスをうまいこと結合してくれます。

{{< code lang="console" title="url-join のインストール" >}}
$ npm install url-join
{{< /code >}}

{{< code lang="ts" title="使用例" >}}
import urlJoin from 'url-join'

urlJoin('https://example.com', 'a', '/b/c'))        //=> https://example.com/a/b/c
urlJoin('https://example.com/', '/a', '/b/c/'))     //=> https://example.com/a/b/c/
urlJoin('https://example.com', '/foo', '?q=123'))   //=> https://example.com/foo?q=123
urlJoin('https://example.com', 'foo/', '/?q=123'))  //=> https://example.com/foo?q=123
{{< /code >}}

URL の末尾にクエリ文字列 (`?q=123`) があるときは、パス部分の末尾の `/` は消されちゃうみたいですね。

{{% note title="path.join は URL 結合には使えない" %}}
`path` モジュールの `path.join` は、ローカルファイルシステム用のパス結合関数なので、URL の結合には使ってはいけません。
例えば、Windows 環境ではバックスラッシュが使われてしまったりします。
{{% /note %}}

