---
title: "Nginx の設定: Web サイトのドメインを移転したときの 301 リダイレクト設定"
url: "p/mjs29go/"
date: "2023-05-15"
tags: ["nginx"]
---

nginx で URL リダイレクト用のレスポンスを返す
----

Web サイトのドメイン（URL プレフィックス）を次のような感じで移転することを考えます。

- 旧アドレス: `https://old.example.com`
- 新アドレス: `https://new.example.com`

多くのユーザーが旧アドレスでブックマークしている可能性があるので、しばらくは旧アドレスにアクセスしてきたユーザー（ブラウザ）に対して、リダイレクト用のレスポンスを返すよう nginx を設定しておく必要があります。
下記は、旧アドレスでのアクセス時に 301 (Moved Permanently) リダイレクトレスポンスを返す設定例です。

{{< code lang="nginx" title="/etc/nginx/conf.d/old.example.com.conf" hl_lines="11 17">}}
# これは http から https へリダイレクトする設定（オプション）
server {
    server_name old.example.com;
    listen 80;
    return 301 https://$host$request_uri;
}

# https://old.example.com から
# https://new.example.com へリダイレクトする設定
server {
    server_name old.example.com;
    listen 443 ssl http2;

    ssl_certificate /etc/letsencrypt/live/old.example.com/fullchain.pem;
    ssl_certificate_key /etc/letsencrypt/live/old.example.com/privkey.pem;

    return 301 $scheme://new.example.com$request_uri;
}
{{< /code >}}

ポイントは最後の __`return`__ ディレクティブです。
1 つ目の引数に __`301`__ を指定することで、301 (Moved Permanently) の HTTP レスポンスコードを返すことができます。
このコードを省略した場合は、302 (Moved Temporarily) になります。

上記のように設定を変更したら、nginx サービスに [設定ファイルをリロード](/p/6aazmy3/) させて完了です。

```console
$ sudo service nginx reload
```

これで、例えば、`https://old.example.com/hoge` へのアクセスが、`https://new.example.com/hoge` にリダイレクトされ、ブラウザの URL 表示も自動的に新しい URL に変わるようになります。

{{% reference %}}
- [Nginx の設定: http でアクセスされた場合に https にリダイレクトする](/p/jowgstm/)
{{% /reference %}}


（おまけ）Google Search Console のお引越し設定
----

[Google Search Console](https://search.google.com/search-console/) でウェブサイトのインデックス管理をしている場合は、そちらのアドレス変更をしておきましょう。
Search Console には、アドレス変更用のメニューが用意されています。

1. 新アドレスのウェブサイト（プロパティ）を追加して __サイトマップを登録__ し、ページインデックスが生成されるまで 1 日ほど待つ
2. 旧アドレスのウェブサイト（プロパティ）の __設定__ タブから、__アドレス変更__ を選択し、新しいサイトを指定

もし、最後の確認で次のようなエラーが出るようであれば、新アドレスのインデックスがまだ生成されていない可能性があります。
その場合は、数日待ってからもう一度試すとうまくいくことがあります。

```
ホームページからの 301 リダイレクト
ページを取得できませんでした
「新アドレス」
```


（おまけ）URL が変わったことをサイト上で知らせる
----

Web サイトへのアクセスが自動でリダイレクトされるようなったのはよいのですが、これだけでは、古い URL にブックマークしているユーザーが、永遠に古い URL を使い続けてしまう可能性があります（URL が変わったことに気付かない）。
それを防ぐために、すべてのページの先頭に、次のような移転のお知らせメッセージを表示するという方法が考えられます。

```
Web サイトの URL が変わりました。ブックマークの変更をお願いします。
変更前: https://new.example.com
変更後: https://old.example.com
```

できれば、このメッセージは __古いアドレスでアクセスしてきたユーザーにのみ表示したい__ ところです。
ひとつの方法として、Web サーバーからの 301 リダイレクト時に、__`?redirected`__ のようなクエリ文字列を付加し、そのクエリ文字列が付いている場合のみクライアントサイド JavaScript でメッセージ表示を行う、という方法があります。

次の nginx 設定では、`old.example.com` へのアクセスを `new.example.com` へ 301 リダイレクト (`permanent`) しつつ、クエリ文字列として `redirected` を付加します。
ここでは、`return 301` ではなく、`rewrite` ディレクティブを使用していることに注意してください。

{{< code lang="nginx" title="/etc/nginx/conf.d/old.example.com.conf" hl_lines="8">}}
server {
    server_name old.example.com;
    listen 443 ssl http2;

    ssl_certificate /etc/letsencrypt/live/old.example.com/fullchain.pem;
    ssl_certificate_key /etc/letsencrypt/live/old.example.com/privkey.pem;

    rewrite ^ https://new.example.com$uri?redirected permanent;
}
{{< /code >}}

`rewrite` ディレクティブは賢い動きをするようになっており、もともとの URL がクエリ文字列を含んでいる場合は、それを保ったまま `redirected` クエリ文字列を付加してくれます。
例えば、リダイレクト時に次のような URL 変換が行われます。

- 転送元: `https://old.example.com/?foo=100#bar`
- 転送先: <code>https://<b>new</b>.example.com/?<b>redirected</b>&foo=100#bar</code>

あとは、HTML ファイルの中に、次のような移転お知らせメッセージを埋め込んでおきます。

```html
<div id="redirect-notification" style="display: none;">
  Web サイトの URL が変わりました。ブックマークの変更をお願いします。<br>
  変更前: https://new.example.com<br>
  変更後: https://old.example.com<br>
</div>
<script>
  // URL のクエリ文字列に redirected が含まれていたらサイト移転のお知らせを表示し、
  // URL 表示からクエリ文字列 (redirected) を削除する。
  const url = new URL(document.location);
  if (url.searchParams.has('redirected')) {
    document.getElementById('redirect-notification').style.display = 'block';
    url.searchParams.delete('redirected');
    history.replaceState({}, '', url);
  }
</script>
```

`div` 要素の初期スタイルは `display: none` となっており、お知らせメッセージは非表示になっていますが、URL のクエリ文字列に `redirected` が含まれている場合は、`display: block` に変更してメッセージを表示するようにしています。
ついでに、`history.replaceState` を実行することにより、ブラウザ上の URL 表示から `redirected` というクエリ文字列を削除しています。

