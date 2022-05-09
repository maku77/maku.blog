---
title: "ウェブサイトのリンク切れを自動でチェックする (muffet)"
url: "/p/s7q8o5k/"
date: "2022-04-23"
tags: ["ツール"]
---

muffet のインストール
----

__`muffet`__ はウェブサイトのリンク切れをチェックしてくれるコマンドラインツールです。
Go 言語で実装されており、いろいろな OS (Linux、macOS、Windows) で利用することができます。

- [raviqqe/muffet: Fast website link checker in Go](https://github.com/raviqqe/muffet)

インストール方法は、上記サイトに記述されていますが、例えば macOS であれば、homebrew を使って次のように簡単にインストールできます。

{{< code lang="console" title="macOS の場合" >}}
% brew install muffet
{{< /code >}}

各 OS 用の実行バイナリが欲しければ、[Release ページ](https://github.com/raviqqe/muffet/releases) からダウンロードできます。


muffet の使い方
----

基本的な使い方は、次のように調べたいウェブページのアドレスを入力するだけです。

```console
% muffet http://localhost:51000
```

リンク元のサイトアドレスと、アクセスできなかったサイトアドレスの組み合わせが一覧表示されます。

```
http://localhost:51000/p/eu7djpv/
	404	http://localhost:51000/assets/favicon/180x180.png
	404	http://localhost:51000/assets/favicon/192x192.png
http://localhost:51000/p/ckahx6k/
	404	http://localhost:51000/assets/favicon/180x180.png
	404	http://localhost:51000/assets/favicon/192x192.png
	404	http://video.google.com/videoplay?docid=973149761529535925
	404 (following redirect https://www.example.com/p/44327.html)
	   	http://www.example.com/44327/2005/04/tipsinbox.html
```

muffet は複数スレッドで高速にリンクチェックを行いますが、ひとつのドメインに対する同時接続数を制限したいときは、__`--max-connections-per-host`__ オプションを指定します。
Web サーバーへの負荷を考慮したいときに使えます。

{{< code lang="console" title="同時接続数を制限する" >}}
% muffet --max-connections-per-host=5 http://localhost:51000
{{< /code >}}

__`--exclude`__ オプションを使うと、チェック対象外とするアドレスを正規表現で指定できます。
例えば、次のようにすれば、`https://` で始まるリンクをチェック対象外にできます。
ローカル Web サーバーで開発している場合、このオプション指定によって外部サイトのリンクだけをチェック対象外にできます。

{{< code lang="console" title="外部リンクを対象外にする" >}}
% muffet --exclude="https://.*" http://localhost:51000
{{< /code >}}


その他のオプションは、__`muffet --help`__ で確認できます。

{{< code lang="console" title="muffet のヘルプ (v2.4.9)" >}}
% muffet --help
Usage:
  muffet [options] <url>

Application Options:
  -b, --buffer-size=<size>                  HTTP response buffer size in bytes (default: 4096)
  -c, --max-connections=<count>             Maximum number of HTTP connections (default: 512)
      --max-connections-per-host=<count>    Maximum number of HTTP connections per host (default: 512)
  -e, --exclude=<pattern>...                Exclude URLs matched with given regular expressions
      --follow-robots-txt                   Follow robots.txt when scraping pages
      --follow-sitemap-xml                  Scrape only pages listed in sitemap.xml
      --header=<header>...                  Custom headers
  -f, --ignore-fragments                    Ignore URL fragments
      --json                                Output results in JSON
  -r, --max-redirections=<count>            Maximum number of redirections (default: 64)
      --rate-limit=<rate>                   Max requests per second
  -t, --timeout=<seconds>                   Timeout for HTTP requests in seconds (default: 10)
  -v, --verbose                             Show successful results too
      --proxy=<host>                        HTTP proxy host
      --skip-tls-verification               Skip TLS certificate verification
      --one-page-only                       Only check links found in the given URL
      --color=[auto|always|never]           Color output (default: auto)
  -h, --help                                Show this help
      --version                             Show version
{{< /code >}}

現状そこまで凝ったことはできないようですね。
オプションの使い方もよくわからない点が多いので、公式サイトのドキュメントが拡充されることを期待します。

