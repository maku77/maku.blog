---
title: "Let's Encrypt certificate expiration notice が来たら"
url: "/p/cniq5vr"
date: "2019-03-21"
tags: ["nginx"]
---

Let's Encrypt Expiry Bot からの証明書期限切れメール
----

2 ヶ月ほど前に、[Sakura VPS レンタルサーバーを Let's Encrypt で SSL 対応](/p/xkpjfcf)していたら、こんなメールが来ました。

> Your certificate (or certificates) for the names listed below will expire in 20 days (on 10 Apr 19 07:02 +0000). Please make sure to renew your certificate before then, or visitors to your website will encounter errors.
>
> We recommend renewing certificates automatically when they have a third of their
total lifetime left. For Let's Encrypt's current 90-day certificates, that means
renewing 30 days before expiration. See
https://letsencrypt.org/docs/integration-guide/ for details.

**90 日ごとに証明書 (certificate) が期限切れになってしまう**ので更新しなきゃヤバイよってメールでした。
更新しないでいると、Web サイトに https でアクセスできなくなってしまいます。
見逃すところでした。


certbot コマンドによる証明書の更新
----

証明書の更新は、**`certbot`** コマンドを使って簡単に実行できます。

まず、`certbot` コマンドがインストールされていない場合はインストールします。

{{< code title="certbot のインストール" >}}
$ sudo yum install certbot
{{< /code >}}

証明書を更新するには、下記のように **`certbot renew`** を実行します。
証明書の有効期限まで 30 日以上残っている状態で更新する場合は、**`--force-renew`** オプションを付けて実行する必要があります。
更新したら、Web サーバをリロードすれば完了です。

{{< code title="SSL 証明書の更新" >}}
$ sudo certbot renew --force-renew
$ sudo systemctl reload nginx
{{< /code >}}


トラブルシューティング: 404 Not Found が出る場合
----

`certbot renew` コマンドを実行したときに、下記のようなエラーが出て失敗することがあります（下記の例では、Web サイトのドメイン名は `example.com` に変えています）。

{{< code title="certbot renew のエラー（抜粋）" >}}
Attempting to renew cert (example.com) from /etc/letsencrypt/renewal/example.com.conf produced an unexpected error: Failed authorization procedure. example.com (http-01): urn:ietf:params:acme:error:unauthorized :: The client lacks sufficient authorization :: Invalid response from https://example.com/.well-known/acme-challenge/Y1WeUoSwipdo5aZHsUNevZl_ppylAAWJh9bfSv1Xqu0 [225.100.100.100]: "<html>\r\n<head><title>404 Not Found</title></head>\r\n<body bgcolor=\"white\">\r\n<center><h1>404 Not Found</h1></center>\r\n<hr><center>". Skipping.

...

All renewal attempts failed. The following certs could not be renewed:
  /etc/letsencrypt/live/example.com/fullchain.pem (failure)
All renewal attempts failed. The following certs could not be renewed:
  /etc/letsencrypt/live/example.com/fullchain.pem (failure)
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
1 renew failure(s), 0 parse failure(s)
{{< /code >}}

このエラーが出る場合、多くの場合は nginx の Web サイトのドキュメントルートを `/usr/share/nginx/html` から変更していることが原因のようです。
下記の設定ファイルの、`webroot_map` のパスを、Web サイトの正しいドキュメントルートのディレクトリに変更します。

{{< code lang="ini" title="/etc/letsencrypt/renewal/example.com.conf" >}}
[[webroot_map]]
example.net = /usr/share/nginx/html
{{< /code >}}

例えば、`/home/maku/website` ディレクトリに設定しているなら、下記のように変更して保存します。

{{< code lang="ini" >}}
example.net = /home/maku/website
{{< /code >}}

これで、`certbot renew` コマンドの実行は成功するはずです。


cron で自動更新するように設定しておく
----

3 ヶ月おきに上記のコマンドを実行するのは面倒なので、cron で自動実行されるように設定しておきます。
設定ファイルは、`/etc/crontab` をテンプレートにして、`/etc/cron.d/lets_encrypt` を作成します。

{{< code title="/etc/cron.d/lets_encrypt を作成" >}}
$ sudo cp /etc/crontab /etc/cron.d/lets_encrypt
$ sudo vim /etc/cron.d/lets_encrypt
{{< /code >}}

ファイルの中身は下記のようにして、毎週日曜日に、証明書の更新コマンドを実行するようにします。
実際には、証明書の有効期限が 30 日を切っている場合にのみ更新が行われます。

{{< code title="/etc/cron.d/lets_encrypt" >}}
SHELL=/bin/bash
PATH=/sbin:/bin:/usr/sbin:/usr/bin
LANG=ja_JP.UTF-8
LC_ALL=ja_JP.UTF-8
CONTENT_TYPE=text/plain; charset=UTF-8
MAILTO="xxxxx@gmail.com"

# Let's Encrypt の証明書を更新（毎週日曜日2時30分）
30 2 * * sun root /usr/bin/certbot renew && /usr/bin/systemctl reload nginx
{{< /code >}}

ここでは、`MAILTO` を設定することで、cron のログをメールで送るように設定しています。
例えば、下記のようなメールが届くことになります（まだ証明書の期限は切れないから更新要求はスキップされたよ、というログですね）。

{{< code title="cron からのメールの例" >}}
Saving debug log to /var/log/letsencrypt/letsencrypt.log

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Processing /etc/letsencrypt/renewal/example.com.conf
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Cert not yet due for renewal

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

The following certs are not due for renewal yet:
  /etc/letsencrypt/live/example.com/fullchain.pem expires on 2019-06-19 (skipped)
No renewals were attempted.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
{{< /code >}}

ちなみに、cron の日時指定の方法は下記のようになっています（`/etc/crontab` より抜粋）。

{{< code >}}
# Example of job definition:
# .---------------- minute (0 - 59)
# |  .------------- hour (0 - 23)
# |  |  .---------- day of month (1 - 31)
# |  |  |  .------- month (1 - 12) OR jan,feb,mar,apr ...
# |  |  |  |  .---- day of week (0 - 6) (Sunday=0 or 7) OR sun,mon,tue,wed,thu,fri,sat
# |  |  |  |  |
# *  *  *  *  * user-name  command to be executed
{{< /code >}}


Sakura VPS レンタルサーバで Let's Encrypt 設定した場合
----

Sakura のレンタルサーバの設定で Let's Encrypt を自動設定した場合、下記のような cron 設定ファイルが自動的に生成されているかもしれません。
毎週木曜日の2時14分に証明書の更新をしようとしています。
こういったファイルが自動生成されているのであれば、それを使うのもよいでしょう。

{{< code title="/etc/cron.d/certbot-auto" >}}
14 2 * * 4 root /usr/local/certbot/certbot-auto renew --webroot -w /usr/share/nginx/html --post-hook 'systemctl reload nginx'
{{< /code >}}

ただし、ここでも Web サイトのドキュメントルートとして `/usr/share/nginx/html` が指定されているので、もしパスを変更しているのであれば、上記のファイルを修正しておく必要があります。

自分で作成した `/etc/cron.d/lets_encrypt` があるのであれば、上記のファイルは処理が重複しているので削除しちゃいましょう。

