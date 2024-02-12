---
title: "Certbot で Let's Encrypt の SSL 証明書を取得する（Web サーバーの HTTPS 対応）"
url: "p/io4gs6h/"
date: "2022-12-03"
tags: ["Let's Encrypt", "Certbot", "ssl"]
weight: 1
---

{{% private %}}
- [Certbot commands](https://eff-certbot.readthedocs.io/en/stable/using.html#certbot-commands)
- [Configuring HTTPS servers - nginx](https://nginx.org/en/docs/http/configuring_https_servers.html)
{{% /private %}}

何をするか？
----

数年前に Google が Web サイトの常時 SSL 化を推奨し始めて、今では多くの Web サイトが HTTPS によるアクセスに対応しています。
Web サーバーを HTTPS (SSL) 対応するには、第三者機関となる認証局 (CA: Certificate Authority) から SSL 証明書を発行してもらう必要があるのですが、__Let's Encrypt という認証局を使うと、SSL 証明書を無料で発行してもらえます__（感謝！）。

{{< image src="img-001.png" w="600" title="Let's Encrypt + Certbot による HTTPS 対応" >}}

レンタルサーバー側で提供されている WordPress 環境などを使用している人は、知らないうちに Let's Encrypt を使った HTTPS 化の恩恵を受けているかもしれません。
ここでは、VPS などで自力で Web サーバーを立ち上げている人が、HTTPS (SSL) 対応する方法を説明します。

__Let's Encrypt からの SSL 証明書の発行には、Certbot というツールを使うのが一般的です__。
Certbot による証明書取得や、Web サーバー (nginx) のヴァーチャルホスト設定をまとめて行ってしまう Docker イメージなども存在しますが、ここでは、Certbot の基本的な振る舞いを理解するために、`certbot` コマンドを直接実行する前提で説明していきます。


Certbot とは
----

[Certbot](https://certbot.eff.org/) は、Let's Encrypt 認証局から SSL 証明書を発行してもらうためのクライアントツールで、以下のような作業を自動で行ってくれます。

1. Web サーバー用の公開鍵ファイル（CA に送られる）と秘密鍵ファイルの生成
2. Let's Encrypt という認証局で SSL 証明書（CRT ファイル）を発行
3. 自動生成した秘密鍵ファイルや、発行された SSL 証明書を Web サーバー（nginx や Apache）に設定

認証局となる Let's Encrypt は、無料で SSL 証明書を発行してくれれます。
つまり、Certbot を使えば、__Web サーバーの HTTPS 対応を無料で簡単に行うことができます__。
証明書の取得までを Certbot で行い、Web サーバーの設定は別の仕組み（手動など）で行う、という使い方も一般的です。


事前準備（HTTP でのアクセス確認）
----

Certbot は、基本的に HTTPS (SSL) 対応したい Web サーバー上で実行する必要があり、その Web サーバーがすでに HTTP でアクセスできる状態になっている必要があります。
また、Let's Encrypt によって発行される SSL 証明書は、ドメイン検証 (Domain Validation; DV) 型の証明書であり、Web サーバーにドメイン名でアクセスできるようになっている必要があります。

例えば、__`example.com`__ というドメインで運用する Web サーバーを HTTPS 対応する場合は、次のような前提条件を満たしておく必要があります（DDNS サービスなどを使えば、必ずしも固定の IP アドレスは必要なかったりしますが、ここではより一般的なグローバル IP アドレスの使用を前提とします）。

前提条件:

1. Web サーバーにグローバル IP アドレスが割り当てられていること
   - 参考: 固定アドレスを使える VPS → {{< ad/conoha-vps-text >}} / {{< ad/onamae-vps-text >}} / {{< ad/sakura-vps-text >}}
1. 独自ドメインを取得済みであること
   - 参考: 独自ドメインの取得 → {{< ad/onamae-domain-text >}} / {{< ad/valuedomain-domain-text >}}
1. DNS の A レコードで、ドメイン名から IP アドレスを正引きできるよう設定されていること
1. `http://example.com/` といったアドレス（80 番ポート）で Web サーバーにアクセスできること

[Let's Debug](https://letsdebug.net/) というサイトに対象のドメイン名を入力すると、証明書発行の準備ができているかを確認することができます。
あるいは、対象のホスト上で次のようにコマンド実行して確認するのでも OK です。

```console
$ curl ifconfig.me     # 自ホストのグローバル IP アドレスを確認
$ dig example.com      # 独自ドメイン (example.com) の正引き結果が上記と同じか確認
$ curl -I example.com  # Web サーバーに HTTP アクセスできるか確認 (HTTP/1.1 200 OK)
```

ここでは、Web サーバーが立ち上げられていることまで前提条件に含めましたが、後述の Certbot の `standalone` モードを使用すれば、Certbot 内部で起動する Web サーバーを利用して証明書の発行だけを行うことができます。


certbot コマンドのインストール
----

`certbot` コマンドのインストールは、[Certbot Instructions](https://certbot.eff.org/instructions) に従って進めれば OK です。
ほとんどの Linux 系ディストリビューションでは、`snap` でインストールできます（`snap` はディストリビューションに依存しないパッケージ管理ツールです）。

{{< code lang="console" title="certbot コマンドのインストール" >}}
$ sudo snap install --classic certbot
{{< /code >}}

次のように実行できればインストールはできています。

```console
$ certbot --version
certbot 1.32.0
```


Let's Encrypt のステージング環境について
----

Let's Encrypt による SSL 証明書の発行処理にはさまざまなレート制限が設けられており、例えば、Web サーバーの準備ができていない状態で証明書のリクエストを繰り返していると、すぐにリクエストが拒否されるようになってしまいます。
Web サーバーの接続検証は 1 時間に 5 回までしか実行できません（参考: [Failed Validation Limit](https://letsencrypt.org/docs/failed-validation-limit/)）。
また、同一ホスト用の証明書発行は、1 週間に 5 回までに制限されています（参考: [Duplicate Certificate Limit](https://letsencrypt.org/docs/duplicate-certificate-limit/)）。

{{< code title="リクエスト上限に達したときのエラー" >}}
too many failed authorizations recently: see https://letsencrypt.org/docs/failed-validation-limit/
{{< /code >}}

Let's Encrypt が用意してくれている __ステージング環境__ を使用すると、このレート制限が緩和されるので、Web サーバーの接続テストはステージング環境を使用することが推奨されています（1 時間あたりの接続検証回数が 60 回に緩和されています）。
`certbot` コマンドを使用する場合は、__`--test-cert`__ オプションを指定することで、ステージング環境による検証（証明書の取得テスト）を行うことができます。


Certbot のプラグイン
----

`certbot` コマンドは、実行時にプラグインを指定するようになっており、環境や用途によって使い分けます。
大きく分けて 2 種類のプラグインがあり、SSL 証明書取得だけを行うプラグイン (__`standalone`__, __`webroot`__) と、SSL 証明書取得に加えて Web サーバーへの設定まで行うプラグイン (__`nginx`__, __`apache`__) があります。
後者は、特定の Web サーバー用に作り込まれたプラグインです。
どのプラグインを使用するかは、`certbot` コマンド実行時にオプションで指定します。

`--standalone`
: Certbot 内部で接続確認用の Web サーバー（80 番ポート）を立ち上げ、証明書の取得を行います。
コマンドライン引数でドメイン名（`-d` オプション）を指定するだけで、そのドメイン用の証明書を取得できます。
一番シンプルな仕組みですが、実行時に内部的な Web サーバーを立ち上げるため、運用中の Web サーバー（80 番ポート）を停止してから実行しなければいけません。

`--webroot`
: 証明書の取得時に、既存の Web サーバーの webroot ディレクトリを、接続テスト用のファイル置き場として使用するプラグインです。
Let's Encrypt はサーバーの存在確認のために、Web サーバーの `http://<DOMAIN>/.well-known/acme-challenge/xxxx` というパスにアクセスしてきます。
`webroot` プラグインでは、`-w` オプションで指定したディレクトリ (webroot) に `.well-known/` ディレクトリを自動生成することで、このチャレンジリクエストに対応します。

`--nginx`
: 証明書の取得時に、既存の nginx サーバーの webroot ディレクトリを、接続テスト用のファイル置き場として使用するプラグインです。
このプラグインでは、nginx の設定ファイル (`.conf`) を自動で書き換えて、証明書や秘密鍵ファイルの設定 (`ssl_certificate`, `ssl_certificate_key`) まで行ってくれます。

`--apache`
: 証明書の取得時に、既存の Apache サーバーの webroot ディレクトリを、接続テスト用のファイル置き場として使用するプラグインです。
このプラグインでは、Apache の設定ファイル (`.conf`) を自動で書き換えて、証明書や秘密鍵ファイルの設定 (`SSLCertificateFile`, `SSLCertificateKeyFile`) まで行ってくれます。

表にまとめると次のような感じでしょうか。

| | 秘密鍵<br>の生成 | 証明書<br>の取得 | Web サーバーの<br>webroot を利用 | Web サーバーの<br>設定を書き換え |
| ---- | :--: | :--: | :--: | :--: |
| `standalone` | ✅ | ✅ | ─ | ─ |
| `webroot` | ✅ | ✅ | ✅ | ─ |
| `nginx` | ✅ | ✅ | ✅ | ✅ |
| `apache` | ✅ | ✅ | ✅ | ✅ |

`standalone` プラグインは、もっともシンプルに証明書を取得できますが、本番環境用の Web サーバーを停止して実行しなければいけないので、実運用では使いにくいでしょう。
`webroot` プラグインは、特定の Web サーバー実装に依存しないので汎用的ですが、秘密鍵や証明書の情報を Web サーバーの設定ファイルに自力で設定する必要があります。
`nginx` や `apache` などの各種 Web サーバー用のプラグインを使用すると、Web サーバーの設定まで自動で行ってくれますが、何が自動的に設定されるのかをちゃんと把握した上で使用する必要があります。


証明書の発行（standalone プラグイン）
----

まずは、`standalone` プラグインで、秘密鍵の作成と、証明書の発行のみを行ってみます。
`certbot` コマンドが内部的な Web サーバーを立ち上げるので、既存の Web サーバー（80 番ポート）が稼働している場合は、先に停止してください。
`certbot` コマンドは、下記のようなディレクトリにアクセスするため、__`sudo`__ を付けて root 権限で実行する必要があります。

- `/etc/letsencrypt/` ... 生成した秘密鍵、取得した証明書、ドメインごとの certbot コマンド設定など
- `/var/lib/letsencrypt/` ... バックアップなど
- `/var/log/letsencrypt/` ... ログファイルなど

`standalone` プラグインで証明書を発行するには、次のように __`certbot certonly --standalone`__ コマンドを使用します。
ここでは、Let's Encrypt のステージング環境を利用するため、__`--test-cert`__ オプションを付けていますが、本番環境用の証明書を発行する場合は、このオプションを外してください（これで取得した証明書を使うと、Web ブラウザでアクセスしたときに ERR_CERT_AUTHORITY_INVALID といったエラーになります）。

{{< code lang="console" title="standalone モードで example.com 用の証明書を発行" >}}
$ sudo certbot certonly --standalone -d example.com --test-cert
{{< /code >}}

{{% accordion title="出力例" %}}
```
Saving debug log to /var/log/letsencrypt/letsencrypt.log
Plugins selected: Authenticator standalone, Installer None
Requesting a certificate for example.com
Performing the following challenges:
http-01 challenge for example.com
Waiting for verification...
Cleaning up challenges

Successfully received certificate.
Certificate is saved at: /etc/letsencrypt/live/example.com/fullchain.pem
Key is saved at:         /etc/letsencrypt/live/example.com/privkey.pem
This certificate expires on 2023-03-04.
These files will be updated when the certificate renews.
Certbot has set up a scheduled task to automatically renew this certificate in the background.
```
{{% /accordion %}}

実行に成功すると、__`/etc/letsencrypt/live/<DOMAIN>/`__ ディレクトリ以下に各種キーファイルや証明書が格納されます。
この下のファイル群は、実際にはシンボリックリンクですが、通常はこの下のファイルを Web サーバーに設定すれば大丈夫です。

- `cert.pem` ... SSL 証明書
- `chain.pem` ... 中間 CA 証明書
- `fullchain.pem` ... SSL 証明書 ＋ 中間 CA 証明書（`cert.pem` と `chain.pem` をマージしたもの）
- `privkey.pem` ... 秘密鍵 (private key)

Web サーバーによって使用するファイルが異なりますが、最近の Web サーバーでは、__`fullchain.pem`__ と __`privkey.pem`__ の 2 つを使用します。


証明書の発行 (webroot プラグイン）
----

`webroot` プラグインで証明書を発行する場合は、既存の Web サーバーの webroot 領域を間借りして `.well-known/acme-challenge/xxxx` ファイルを配置することで、Let's Encrypt からの接続テストに対応します。
何らかの Web サーバーが必要なので、ここでは Docker で nginx サーバーを起動しておくことにします。

{{< code lang="yaml" title="/home/maku/docker-compose.yml" >}}
version: "3"
services:
  web:
    image: nginx:alpine
    ports:
      - "80:80"
    volumes:
      - ./public:/usr/share/nginx/html
{{< /code >}}

{{< code lang="console" title="nginx サーバーを起動しておく" >}}
$ cd /home/maku
$ mkdir public
$ docker compose up -d
{{< /code >}}

上記のように nginx を起動すると、ホスト上では `/home/maku/public/` ディレクトリが webroot となります。
Certbot の `webroot` プラグインで証明書を取得するには、__`certbot certonly --webroot`__ コマンドを使用します。
このとき、webroot ディレクトリを __`-w`__ オプションで指定します。

{{< code lang="console" title="webroot モードで example.com 用の証明書を発行" >}}
$ sudo certbot certonly --webroot -w /home/maku/public -d example.com --test-cert
{{< /code >}}

{{% accordion title="出力例" %}}
```
Saving debug log to /var/log/letsencrypt/letsencrypt.log
Requesting a certificate for example.com

Successfully received certificate.
Certificate is saved at: /etc/letsencrypt/live/example.com/fullchain.pem
Key is saved at:         /etc/letsencrypt/live/example.com/privkey.pem
This certificate expires on 2023-03-04.
These files will be updated when the certificate renews.
Certbot has set up a scheduled task to automatically renew this certificate in the background.
```
{{% /accordion %}}

`standalone` プラグインのときと同様ですが、うまくいくと、次のようなファイルが生成されるので、これを nginx サーバーの設定ファイルなどに設定することになります。

- `/etc/letsencrypt/live/example.com/fullchain.pem` （証明書）
- `/etc/letsencrypt/live/example.com/privkey.pem` （秘密鍵）


nginx サーバーに証明書を設定する
----

秘密鍵の生成と SSL 証明書の発行に成功したら、あとは nginx サーバーなどの設定ファイルでそれらのファイルパスを指定することで、HTTPS (SSL) 対応が完了します。
ここでは、Docker で nginx サーバーを立ち上げて確認してみます。

{{< code lang="html" title="~/public/index.html（サンプル HTML）" >}}
<html>Hello</html>
{{< /code >}}

`docker-compose.yml` ファイルの変更点は、SSL ポート 443 を公開しているところと、サーバーの設定ファイルと証明書ファイル用のディレクトリをマウントしている部分です。

{{< code lang="yaml" title="~/docker-compose.yml" hl_lines="7 10 11" >}}
version: "3"
services:
  web:
    image: nginx:alpine
    ports:
      - "80:80"
      - "443:443"
    volumes:
      - ./public:/usr/share/nginx/html
      - ./conf.d:/etc/nginx/conf.d:ro
      - /etc/letsencrypt:/etc/letsencrypt:ro
{{< /code >}}

{{% note title="バインドマウントの起点に注意" %}}
証明書のディレクトリとして、`/etc/letsencrypt/live` 以下をマウントしたくなりますが、このディレクトリにはシンボリックリンクしか含まれていないので、実体となるファイルも含めるように `/etc/letsencrypt` ディレクトリごとマウントしなければいけません。
`live` ディレクトリ以下だけをマウントすると、Docker で nginx サーバーを起動しようとしたときに、次のようにファイルを見つけられないエラーが発生します。

```
nginx: [emerg] cannot load certificate "/etc/letsencrypt/live/example.com/fullchain.pem": BIO_new_file() failed
```
{{% /note %}}

nginx の設定ファイルでは、証明書ファイルと、秘密鍵ファイルのパスを設定します。

{{< code lang="nginx" title="~/conf.d/example.com.conf" hl_lines="16 17" >}}
server {
    server_name example.com;
    listen 80;       # IPv4
    listen [::]:80;  # IPv6

    location / {
        return 301 https://$host$request_uri;
    }
}

server {
    server_name example.com;
    listen 443 ssl http2;       # IPv4
    listen [::]:443 ssl http2;  # IPv6

    ssl_certificate     /etc/letsencrypt/live/example.com/fullchain.pem;
    ssl_certificate_key /etc/letsencrypt/live/example.com/privkey.pem;

    location / {
        root  /usr/share/nginx/html;
        index index.html;
    }
}
{{< /code >}}

これで、Web サーバーの HTTPS 対応は完了です。
次のように、Web ブラウザから `https://` でアクセスできるようになっていれば成功です。

{{< image src="img-001.png" w="600" title="Let's Encrypt + Certbot による HTTPS 対応" >}}

できたー ٩(๑❛ᴗ❛๑)۶ わーぃ

