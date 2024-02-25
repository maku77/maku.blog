---
title: "電子証明書（SSL 証明書）申請用の CSR ファイルを作成する"
url: "p/vntjjaw/"
date: "2024-01-22"
tags: ["security", "ssl"]
---

CSR ファイルとは
----

Web サーバーで HTTPS 通信を可能にするには、SSL 証明書を Web サーバーに設定する必要があります。
SSL 証明書は具体的には、Web サーバーの公開鍵に CA 認証局の電子署名を付けたものです。
SSL 証明書を CA 認証局に発行してもらうときに必要になるのが __CSR: Certificate Signing Request（証明書署名要求）__ ファイルです。

- 参考: [電子署名と電子証明書の基礎](/p/fzwyf7z/)

CA 認証局にもよりますが、__DNS 経由でのドメインへのアクセス__ が可能であれば、SSL 証明書を申請することができます。
Web サーバーの立ち上げは後回しでよいということです。


CSR ファイル作成のポイント
----

CSR ファイルには、Web サーバー用の公開鍵と、申請者の情報（ディスティングイッシュネーム）を含める必要があります。

- CSR ファイル (`.csr`) を作成するには、対象となる Web サーバーで使用する秘密鍵が必要です（ただし、生成される CSR に含まれるのは公開鍵の方です）。秘密鍵と CSR ファイルの作成には __`openssl`__ コマンドを使用します。
- CSR に含める公開鍵は、__そのサーバー専用__ のものである必要があります。別のサーバー用に発行されている証明書で使われている公開鍵を使い回すことはできません。つまり、秘密鍵もそのサーバー専用のものを作るということです。ワイルドカード証明書でない場合は、サブドメインごとに秘密鍵＆証明書を作ることになります。
- 電子証明書の更新時には、__毎回新しい秘密鍵を作成__ してしまえば OK です（Web サーバーに設定し直す必要はありますが）。特に推奨されるわけではありませんが、CSR のディスティングイッシュネーム（Common Name や Organization）が変化しないケースでは、秘密鍵を使い回すことが可能です。


CSR ファイルを作成する
----

### 秘密鍵ファイルを作成する

CSR ファイルを作成するには、サーバー用の秘密鍵が必要になるので、まずは `openssl` コマンドで秘密鍵を作成します。

{{< code lang="console" title="秘密鍵の作成" >}}
$ openssl genrsa -out example.com-2024.key
{{< /code >}}

ここでは、秘密鍵のファイル名は `example.com-2024.key` としていますが、このように __ホスト名と作成した年の情報__ を含めておくと管理しやすくなります。
出力される秘密鍵は、デフォルトでは次のような [PEM 形式](https://en.wikipedia.org/wiki/Privacy-Enhanced_Mail) のテキストファイルになっています。

{{< code lang="pem" title="example.com-2024.key（秘密鍵ファイルの例）" >}}
-----BEGIN PRIVATE KEY-----
MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQCqr4tAxXZTZ88Y
voHA484nIlpEvCfS3Wf89m3VJ9d0PkxrRm6Al5HBlOBQOe4acRXF9DWJjiGSL2Nv
wdCJnaCwy/yzjjipua8rB8xZtQUYwlX9ldaZuYCq58R7OW729Zebm4y3bDPedNYc
...（省略）...
qmYWUKR0BLX/F+6bNj8GUJjEWxeYa5Jsv8DDGwqKnXqDzkvVI0H1cEdMMhbFukfJ
aalhG6BA3LOLbKqgXspnJnfQ5i32BB0w9AsahSTv5GINt2InQcfi8jJ1eRSsV4Co
ZZGjdw5E0kbzPRScvfkSvsA=
-----END PRIVATE KEY-----
{{< /code >}}

この秘密鍵は、Web サーバーに設定するもので、次に説明する CSR ファイルの生成にも使うものなので大切に保管してください。

### CSR ファイルを作成する

秘密鍵から CSR ファイルを作成するときにも `openssl` コマンドを使用します。
次のようにコマンドを起動し、対話形式で申請者の情報を入力していきます。
この情報のことを __ディスティングイッシュネーム (DN: Distinguished Name)__ と呼びます。
CA 認証局はこの情報をもとに証明書発行の可否を判断します。

{{< code lang="console" title="CSR ファイルの作成" >}}
$ openssl req -new -key example.com-2024.key -out examle.com-2024.csr

You are about to be asked to enter information that will be incorporated
into your certificate request.
What you are about to enter is what is called a Distinguished Name or a DN.
There are quite a few fields but you can leave some blank
For some fields there will be a default value,
If you enter '.', the field will be left blank.
-----
Country Name (2 letter code) [AU]: JP
State or Province Name (full name) [Some-State]: Tokyo
Locality Name (eg, city) []: Ota-ku
Organization Name (eg, company) [Internet Widgits Pty Ltd]: Rei Ayanami
Organizational Unit Name (eg, section) []:
Common Name (e.g. server FQDN or YOUR name) []: example.com
Email Address []:

Please enter the following 'extra' attributes
to be sent with your certificate request
A challenge password []:
An optional company name []:
{{< /code >}}

各項目には、以下のような内容を入力していきます。

| 項目 | 内容 | 入力例 |
| ---- | ---- | ---- |
| Country Name (C) | 国を表す 2 文字のコード | `JP` （日本の場合） |
| State or Province Name (ST) | 組織の置かれた都道府県 | `Tokyo` （東京都の場合） |
| Locality Name (L) | 市区町村名 | `Ota-ku` （大田区の場合） |
| Organization Name (O) | 組織名 or 個人名 | `Rei Ayanami` （個人名の場合） |
| Organizational Unit Name (OU) | 部署名 | （省略可） |
| Common Name (CN) | サーバーの FQDN | `example.com` |
| Email Address (E) | 申請者のメールアドレス | （省略可） |
| A challenge password | チャレンジパスワード | （省略可） |
| An optional company name | オプショナルな企業名 | （省略可） |

{{% note %}}
対話形式でディスティングイッシュネームを入力するのではなく、コマンドラインで指定したい場合は、__`-subj`__ オプションを使用します。

```
$ openssl req -new -key example.com-2024.key -out examle.com-2024.csr \
          -subj "/C=JP/ST=Tokyo/L=Ota-ku/O=Rei Ayanami/CN=example.com"
```
{{% /note %}}

入力が終わると、次のような CSR ファイルが出力されます（これも PEM 形式になっています）。

{{< code lang="pem" title="examle.com-2024.csr（CSR ファイルの例）" >}}
-----BEGIN CERTIFICATE REQUEST-----
MIICnzCCAYcCAQAwWjELMAkGA1UEBhMCSlAxDjAMBgNVBAgMBVRva3lvMQ8wDQYD
VQQHDAZPdGEta3UxFDASBgNVBAoMC1JlaSBBeWFuYW1pMRQwEgYDVQQDDAtleGFt
cGxlLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKqvi0DFdlNn
...（省略）...
haAymhkI4HNoh+k/JBgnGjASQjjLKkfFBSboEVIn0NiZN6Jx7ghw7zpnDDJmOI3T
qHaqlQ3Lc/vF90NpFlhul6UxjJUQlqsSPYgessTzrPOvqo9kcOo6s9bG9yN09V3K
oY0+
-----END CERTIFICATE REQUEST-----
{{< /code >}}

CSR ファイルに記録されたディスティングイッシュネームなどは、次のように確認できます。

{{< code lang="console" title="CSR ファイルの解析" >}}
$ openssl req -text -noout -in example.com-2024.csr
Certificate Request:
    Data:
        Version: 1 (0x0)
        Subject: C=JP, ST=Tokyo, L=Ota-ku, O=Rei Ayanami, CN=example.com
        Subject Public Key Info:
        ...（省略）...
{{< /code >}}


（おまけ）SSL 証明書の申請と Web サーバーへの設定
----

### SSL 証明書の申請

CSR ファイルの作成ができたら、任意の CA 認証局から電子証明書（SSL 証明書）を発行してもらいます。
例えば、{{< mm/ad/conoha-vps-text "ConoHa の VPS" >}} を使っている場合は、アルファ SSL の電子証明書を無料で発行してもらえたりします。
[Web サイト上の手順](https://support.conoha.jp/v/vps-alphassl/) に従って CSR ファイルの内容を貼り付けるだけですぐに証明書が発行されます。

発行された SSL 証明書は `example.com-2024.crt` などの名前で保存して、Web サーバーに設定します。

{{% note title="電子証明書の拡張子" %}}
電子証明書の拡張子は何でもよいのですが、`.crt` や `.pem` がよく使われます。
`.crt` は Cirtificate の略で、`.pem` は PEM フォーマットで記述されていることを示しています。
{{% /note %}}

ここまでの作業で、次のような 3 種類のファイルを作成したことになります。

- example.com-2024.key（秘密鍵）
- example.com-2024.csr（CSR ファイル）
- example.com-2024.crt（電子証明書）

### Web サーバーへの設定

秘密鍵と SSL 証明書（電子証明書）を Web サーバーに設定することで、SSL (HTTPS) による通信が可能になります。
例えば、Web サーバーとして nginx を使用している場合は、次のような感じで設定します。

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

    ssl_certificate     /etc/nginx/ssl/example.com-2024.crt;
    ssl_certificate_key /etc/nginx/ssl/example.com-2024.key;

    location / {
        root  /usr/share/nginx/html;
        index index.html;
    }
}
{{< /code >}}
