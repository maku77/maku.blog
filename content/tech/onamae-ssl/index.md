---
title: "お名前.com の共用サーバーを Let's Encrypt で SSL 対応"
url: "/p/s46j9sq"
date: "2019-01-12"
tags: ["Apache", "お名前.com"]
---

Chrome で https に対応していない Web サイトにアクセスすると、アドレスバーに「**保護されていない通信**」という警告が出るようになりました。
Web サーバーの管理者は、https (SSL) への対応がほぼ必須になっています。
ここでは、お名前.com の共用サーバー（SD-11、SD-12 プランなど）で、SSL を有効にし、http によるアクセスがあったときに https なアドレスにリダイレクトする設定方法を説明します。


Let's Encrypt による SSL 対応
----

お名前.com の共用サーバーで Web サイトを運用している場合は、下記のコントロールパネルからポチポチやるだけで、簡単に SSL (https) によるアクセスを有効にすることができます。

- [共用サーバーSD コントロールパネル https://cp.rentalserver.jp/](https://cp.rentalserver.jp/)

{{< image w="500" border="true" src="onamae-ssl.png" >}}

自前の Web サーバで SSL 設定を行おうとすると、証明書の発行手続きや設定などが結構面倒ですが、このコンパネからポチポチやれば 1 分で設定完了です。
無料証明書で有名な Let's Encrypt のサービスに対応しているので、無料で SSL 対応することができます。


http アクセスを https にリダイレクトする
----

数時間待って、無事 SSL の設定が反映されると、`https://` によるウェブサイトへのアクセスが可能になります。
ただし、そのままだと古い `http://` のサイトも残ったままです。

ここでは、Web サーバー (Apache) の設定ファイルである `.htaccess` を修正（なければ新規作成）することで http から https へのリダイレクト設定を行います。
ここでは、下記のような手順で修正することにしましょう。

1. `sftp` コマンドでサーバ上の `.htaccess` を取得（存在する場合のみ）
2. ローカルで `.htaccess` の内容を修正
3. `sftp` コマンドでサーバ上の `.htaccess` を置き換え

まずは、コントロールパネルの「**FTP・SSHアカウント**」のページで、接続用のアカウント名などを確認しておきましょう。
例えば下記のような感じで設定されているはずです。

- アカウント : `sd0000000@gmoserver.jp`
- パスワード : `mypassword`
- SSHサーバー名 : `ssh10.gmoserver.jp`

### (1) sftp コマンドで既存の .htaccess を取得

まず、Web サーバ上のコンテンツ用ディレクトリに置かれている `.htaccess` ファイルを取得します。
存在しなければ無視して大丈夫です。

```
$ sftp sd0000000@gmoserver.jp@ssh10.gmoserver.jp
Password: xxxxxxxx
sftp> cd example.com
sftp> get .htaccess
sftp> quit
```

### (2) ローカルで .htaccess を編集

サーバから取得した `.htaccess` ファイルを編集して、http アクセスを https へリダイレクトする設定を追加します。
`.htaccess` ファイルが存在しなかった場合は、新しくテキストファイルとして作成し、下記のような設定を追加してください。

~~~
# HTTPS redirect settings
RewriteEngine on
RewriteCond %{HTTPS} off
RewriteRule ^(.*)$ https://%{HTTP_HOST}%{REQUEST_URI} [L,R=301]
~~~

### (3) サーバ上の .htaccess を上書き

`.htaccess` ファイルの修正が終わったら、同じディレクトリから再び `sftp` でサーバに接続し、下記のように `.htaccess` ファイルをアップロード（上書き）します。

~~~
sftp> cd examle.com
sftp> put .htaccess
sftp> quit
~~~

これで、`http://example.com/` というアクセスが、自動的に `https://example.com/` にリダイレクトされるようになります。

