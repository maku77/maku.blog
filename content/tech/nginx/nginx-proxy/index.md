---
title: "nginx-proxy コンテナで簡単にリバースプロキシを立ち上げる"
url: "p/kos367z/"
date: "2024-02-12"
lastmod: "2024-03-04"
tags: ["Docker", "nginx"]
changes:
  - 2024-03-04: Meilisearch コンテナに繋ぐ例を追加。
---

nginx-proxy は Docker コンテナ用のリバースプロキシ
----

Docker コンテナイメージの [nginx-proxy](https://github.com/nginx-proxy/nginx-proxy) を使うと、複数の Docker コンテナ用のリバースプロキシを簡単に立ち上げることができます。

例えば、1 つの VPS サーバー上で複数の Docker コンテナー（例えば Web サーバー）を立ち上げるような場合は、次のような感じで入り口にリバースプロキシを配置し、そこから各 Web サーバーにアクセスを振り分ける構成にします。
ここでは、`aaa.example.com` と `bbb.example.com` というサブドメインにより、プロキシ先を振り分けるようにしていますが、`example.com` というベースドメインを使うこともできます。

{{< image w="750" src="img-001.drawio.svg" title="nginx-proxy によるリバースプロキシでコンテナーを繋ぐ" >}}

もともと Nginx でリバースプロキシを設定するのはそれほど難しくはないのですが、どうしてもその背後にあるコンテナーの設定がリバースプロキシ側の設定に染み出してしまいます。
また、リバースプロキシ用のコンテナーを最後に起動するという手順の複雑さもあります。

- 参考: [Nginx でバーチャルホストを設定する（リバースプロキシ）](/p/q8tw6c4/)
- 参考: [Docker Compose でリバースプロキシを立てて別の Docker Compose 内のコンテナに接続する - まくまく Docker ノート](https://maku77.github.io/p/5f2j2hz/)

`nginx-proxy` によるリバースプロキシを起動しておくと、背後でコンテナーを起動するだけで、自動的にリバースプロキシのエントリに追加してくれます。
`nginx-proxy` には次のような利点があります。

- 背後の Docker コンテナの存在を意識しなくてよい（同一の Docker ネットーワーク内のコンテナ起動を検出して、自動的にリバースプロキシ設定に反映してくれます）
- 各サブドメイン用の SSL 設定（HTTPS 対応）を自動で行ってくれる（[acme-companion](https://github.com/nginx-proxy/acme-companion) を組み合わせて使用することで、Let's Encrypt による SSL 証明書の取得を自動化できます）

リバースプロキシの設定更新が自動化されているので、Web アプリ用の Docker コンテナーを簡単に追加・削除できます。


事前準備
----

`nginx-proxy` の設定を行う前に、サブドメイン用の DNS 設定や、80/443 ポートの解放などを行っておく必要があります。
ネットワーク的に繋がらないとどうしようもないですからね。

### サブドメインの DNS 設定

独自ドメインの DNS 設定で、サブドメイン用の A レコードを追加して IP アドレスに関連づけておきます。
下記は {{< mm/ad/conoha-vps-text >}} での DNS 設定例ですが、だいたい同じような感じで設定できるはずです。

{{< image w="600" border="true" src="img-002.png" title="A レコードの追加（IP アドレスは偽物です）" >}}

反映されるまで数分待ってから、DNS の A レコードを正しく引けるか確認しておきます。
例えば、__`dig`__ コマンドの出力の `status:` が `NOERROR` になっていれば正しく設定されています（もっと簡単に `nslookup` や `ping` で正引きできるかを確認するのでも OK です）。

```console
## A レコードが正しく設定されている場合
$ dig A aaa.example.com | grep status
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 55073

## A レコードが設定されていない場合
$ dig A aaa.example.com | grep status
;; ->>HEADER<<- opcode: QUERY, status: NXDOMAIN, id: 55073
```

### http/https 用のポート解放

対象のサーバー（VPS など）のファイアウォール設定で、外部からの __80__ 番ポート (http) および __443__ 番ポート (https) へのアクセスを許可しておきます。


まずは http 接続で試してみる
----

まずは `nginx-proxy` の基本的な動かし方を理解しましょう。
SSL を使わない http ベースの接続であれば簡単に試せます。

下記は、`nginx-proxy` コンテナーを起動するための Docker Compose ファイルです。
`volumes:` プロパティで `docker.sock` ファイルをマウントしているのは、`nginx-proxy` が他のコンテナーの起動を監視して情報を取得するためです。

{{< code lang="yaml" title="nginx-proxy/docker-compose.yml" >}}
version: "3.8"

# nginx-proxy コンテナの基本的な設定
services:
  nginx-proxy:
    image: nginxproxy/nginx-proxy:1.5
    container_name: nginx-proxy
    ports:
      - "80:80"
    volumes:
      - /var/run/docker.sock:/tmp/docker.sock:ro
    restart: always

# 接続先の各コンテナーはこれと同じ Docker ネットワークに参加する必要がある
networks:
  default:
    name: nginx-proxy
{{< /code >}}

次のようにしてリバースプロキシを起動できます。

{{< code lang="console" title="リバースプロキシの起動" >}}
$ cd nginx-proxy
$ docker compose up -d
{{< /code >}}

あとは、リバースプロキシの中継先となる Docker コンテナーを起動していくだけです。
このとき、バーチャルホスト名をコンテナー側の __VIRTUAL_HOST__ 環境変数で設定することで、`nginx-proxy` が自動的にその情報をリバースプロキシ設定に反映してくれます。

例えば、簡単な Web サーバーを起動する場合は次のようにします。
__リバースプロキシと同じ `nginx-proxy` ネットワークに接続する__ ことに注意してください（これを忘れると、アクセス時に 502 Bad Gateway エラーになります）。
ここでは接続確認だけしたいので、`Ctrl-C` で停止できる形でコンテナを起動します（`-d` オプションをつけません）。

{{< code title="テスト用に Web サーバーを起動" >}}
$ docker container run --rm --net nginx-proxy \
    --env VIRTUAL_HOST=aaa.example.com nginx:alpine
{{< /code >}}

{{< accordion title="docker-compose.yml で定義する場合" >}}
{{< code lang="yaml" hl_lines="8" >}}
version: "3.8"

services:
  webapp:
    image: "nginx:alpine"
    container_name: webapp
    environment:
      VIRTUAL_HOST: aaa.example.com
    networks:
      - nginx-proxy

networks:
  nginx-proxy:
    external: true  # 既存の nginx-proxy ネットワークを参照する
{{< /code >}}
{{< /accordion >}}

Web サーバーを起動したら、Web ブラウザから `http://aaa.example.com` という URL でアクセスして、Welcome to nginx! のページが表示されれば成功です。

次のステップ（SSL 証明書の自動設定）に進む前に、リバースプロキシを停止しておきましょう。

```console
$ cd nginx-proxy
$ docker compose down
```


本番用の設定（SSL/TSL 対応）
----

`nginx-proxy` と合わせて [`acme-companion`](https://github.com/nginx-proxy/acme-companion) という Docker コンテナイメージを使うと、__SSL 証明書の設定まで自動化できます（https によるアクセスが可能になります）__。
Let's Encrypt による SSL 証明書発行処理のために、`nginx-proxy` コンテナーの方にもいくつかボリューム設定を追加する必要があります（本家サイトの説明通りに設定しています）。

{{< code lang="yaml" title="nginx-proxy/docker-compose.yml" >}}
version: "3.8"

services:
  # リバースプロキシ
  nginx-proxy:
    image: nginxproxy/nginx-proxy:1.5
    container_name: nginx-proxy
    ports:
      - "80:80"
      - "443:443"
    volumes:
      - /var/run/docker.sock:/tmp/docker.sock:ro
      - certs:/etc/nginx/certs
      - html:/usr/share/nginx/html
      - vhost:/etc/nginx/vhost.d
    restart: always

  # SSL/TLS 証明書の自動更新
  nginx-proxy-acme:
    image: nginxproxy/acme-companion:2.2
    container_name: nginx-proxy-acme
    volumes_from:
      - nginx-proxy
    volumes:
      - /var/run/docker.sock:/var/run/docker.sock:ro
      - acme:/etc/acme.sh
    restart: always

# 接続先の各コンテナーはこれと同じ Docker ネットワークに参加する必要がある
networks:
  default:
    name: nginx-proxy

# ボリュームの定義
volumes:
  acme:
  certs:
  html:
  vhost:
{{< /code >}}

ボリューム設定が若干ややこしいですが、接続先のコンテナーの情報がまったく出てこないのが素晴らしいです。
つまり、この `docker-compose.yml` ファイルは、みなさんの環境でもそのまま使い回すことができます。

先ほどと同様に、`nginx-proxy` コンテナーを起動します。
今回は https 対応版になっています。

{{< code lang="console" title="nginx-proxy コンテナを起動" >}}
$ cd nginx-proxy
$ docker compose up -d
{{< /code >}}


中継先のコンテナーを立ち上げる
----

### 例: Web サーバー (nginx)

中継先のコンテナー（ここでは Web サーバー）を立ち上げるときは、Let's Encrypt 用の設定値として __`LETSENCRYPT_HOST`__ と __`LETSENCRYPT_EMAIL`__ を指定します。
これらは、SSL 証明書の発行時に使用されます。

{{< code lang="yaml" title="webapp/docker-compose.yml" hl_lines="7-10" >}}
version: "3.8"

services:
  webapp:
    image: nginx:alpine
    container_name: webapp
    environment:
      VIRTUAL_HOST: aaa.example.com
      LETSENCRYPT_HOST: aaa.example.com
      LETSENCRYPT_EMAIL: yourname@example.com
    networks:
      - nginx-proxy

# 使用するネットワークの一覧
networks:
  nginx-proxy:
    external: true  # 既存のネットワークに乗っかる
{{< /code >}}

{{< code lang="console" title="webapp コンテナを起動" >}}
$ cd webapp
$ docker compose up -d
{{< /code >}}

このコンテナを起動すると、自動的に Let's Encrypt から SSL 証明書が発行されてリバースプロキシサーバーに設定されます。
これで、Web ブラウザから `https://aaa.example.com` というアドレスでアクセスできるようになります。

### 例: Meilisearch サーバー

もうひとつ例として、インクリメンタルサーチエンジンの Milisearch サーバーを立ち上げる例を示しておきます。
こちらは、ドメイン名（バーチャルホスト）として `bbb.example.com` を設定しています。
下記の例では `environment` プロパティをリスト形式で設定していますが、前述のマップ形式での指定方法と意味は同じです。

{{< code lang="yaml" itle="meilisearch/docker-compose.yml" hl_lines="13-15" >}}
version: "3.9"

services:
  meilisearch:
    image: "getmeili/meilisearch:v1.6"
    container_name: meilisearch
    ports:
      - "7700:7700"
    environment:
      - MEILI_ENV=production  # 本番環境として起動する
      - MEILI_NO_ANALYTICS=true  # テレメトリデーターは送らない
      - MEILI_MASTER_KEY  # マスターキーはコントローラー側の環境変数をそのまま渡す
      - VIRTUAL_HOST=bbb.example.com
      - LETSENCRYPT_HOST=bbb.example.com
      - LETSENCRYPT_EMAIL=yourname@example.com
    volumes:
      - type: volume  # ボリュームマウントを使う
        source: meili_data
        target: /meili_data
        volume:
          nocopy: true  # ボリューム生成時にコンテナから内容をコピーしない
    networks:
      - nginx-proxy

# 使用するボリュームの一覧
volumes:
  meili_data:
    # name: meili_data  # プロジェクト名のプレフィックスを付けたくないとき

# 使用するネットワークの一覧
networks:
  nginx-proxy:
    external: true  # 既存のネットワークに乗っかる
{{< /code >}}

ポートフォワードの設定 (`ports`) で `7700:7700` と指定していますが、バーチャルホストを使った URL アクセス時には、80 番ポートの URL でアクセスすることができます。
つまり、`https://bbb.example.com/` というアドレスで、コンテナー内の Meilisearch サーバーにアクセスすることができます。
このあたりのデフォルトのポートフォワード処理は `nginx-proxy` がやってくれるので、実は `ports` プロパティの設定は省略できたりします（Docker ホスト上で `localhost:7700` のようにアクセスしたい場合は必要）。


（おまけ）nginx の設定を変更する
----

nginx 自体の設定を変更したいときは、Docker ホスト上に設定ファイルを作成しておいて Docker コンテナにバインドマウントします（参考: [nginx-proxy/docs](https://github.com/nginx-proxy/nginx-proxy/tree/main/docs#proxy-wide)）。
ファイルの配置方法はいくつかありますが、__`/etc/nginx/conf.d`__ ディレクトリ以下に __`.conf`__ ファイルを配置するのが簡単です。

下記は、nginx サーバーの `client_max_body_size` 値をカスタマイズする設定ファイルの例です。
この値のデフォルト値は 1M になっているため、POST リクエストで大きなデータを送りたい場合はこのように大きめのサイズを指定しておく必要があります。

{{< code lang="inf" title="nginx-proxy.conf" >}}
# POST で大きいデータを送れるようにしておく
client_max_body_size 20M;
{{< /code >}}

設定ファイルを用意したら、あとは `docker-compose.yml` の `volumes` セクションにバインドマウントの設定を追加するだけです。

{{< code lang="yaml" title="nginx-proxy/docker-compose.yml" hl_lines="16" >}}
version: "3.8"

services:
  # リバースプロキシ
  nginx-proxy:
    image: nginxproxy/nginx-proxy:1.5
    container_name: nginx-proxy
    ports:
      - "80:80"
      - "443:443"
    volumes:
      - /var/run/docker.sock:/tmp/docker.sock:ro
      - certs:/etc/nginx/certs
      - html:/usr/share/nginx/html
      - vhost:/etc/nginx/vhost.d
      - ./nginx-proxy.conf:/etc/nginx/conf.d/nginx-proxy.conf:ro
    restart: always

  # ...省略...
{{< /code >}}

__`docker compose up -d`__ を再実行すれば、`nginx-proxy` コンテナーを新しい設定で起動できます。


まとめ
----

長々と説明してきましたが、結局のところ __`nginx-proxy` を一度導入してしまえば、あとは中継先のコンテナーを起動するときに、`VIRTUAL_HOST` や `LETSENCRYPT_HOST` を指定するだけでインターネットから https (SSL) でアクセスできるようになる__ ということですね。
これで、Docker コンテナーを使ったサービスをいくつでも簡単に公開できるようになります。

٩(๑❛ᴗ❛๑)۶ わーぃ

みんなも VPS を借りて遊んでみよー（これは宣伝です(￣∇￣) → {{< mm/ad/conoha-vps-text "ConoHa の VPS" >}}）

<center>
{{< mm/ad/conoha-vps-banner >}}
</center>

