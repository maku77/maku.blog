---
title: "Nginx の設定: Nginx の設定ファイル (nginx.conf) の場所"
url: "/p/pms426x"
date: "2013-12-02"
tags: ["nginx"]
---

nginx の設定ファイルは、__`/etc/nginx/nginx.conf`__ です。
ソースからビルドする場合は、`./configure` の実行時にパスを指定できます。

この設定ファイルの中で、以下のように `include` ディレクティブが指定されているため、**`/etc/nginx/conf.d/`** ディレクトリ内にある個別の設定ファイル (`*.conf`) も読み込まれるようになっています。

{{< code >}}
http {
    ...
    include /etc/nginx/conf.d/*.conf;
}
{{< /code >}}

バーチャルホスト機能を利用して複数ドメインのサーバを運用するようなケースでは、ドメインごとに設定ファイルを分けておくと管理しやすいでしょう。
例えば、`example.com` ドメインのサーバ用には `/etc/nginx/conf.d/example.com.conf` を作成します。

