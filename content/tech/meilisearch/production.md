---
title: "Meilisearch を本番環境で使用する（マスターキーと API キーを理解する）"
url: "p/iuvuyzd/"
date: "2024-02-15"
lastmod: "2024-03-04"
tags: ["Meilisearch"]
changes:
  - 2024-03-04: ボリュームマウントを使う方法を追記。
---

{{% private %}}
- [Running Meilisearch in production — Meilisearch documentation](https://www.meilisearch.com/docs/learn/cookbooks/running_production)
{{% /private %}}

爆速で使いやすいインクリメンタル・サーチエンジンの __Meilisearch__ ですが、本番環境で運用するときは、アクセス制御用のマスターキーや API キーの使い方を理解する必要があります。


Meilisearch のデフォルト動作
----

Meilisearch サーバーを次のように特に何もオプションを指定せずに起動すると、開発用の `development` 環境として立ち上がります。
アクセス制御用のマスターキーも設定されません。

{{< code title="お試し用の Meilisearch サーバー起動" >}}
$ docker run -it --rm -p 7700:7700 \
    -v $(pwd)/meili_data:/meili_data \
    getmeili/meilisearch:v1.6
{{< /code >}}

動作環境やマスターキーは、Meilisearch サーバーの起動時に、下記のような環境変数（あるいはコマンドライン引数）で指定することができます。

| 環境変数 | CLI オプション | デフォルト値 | 設定可能な値 |
| ---- | ---- | ---- | ---- |
| [`MEILI_ENV`](https://www.meilisearch.com/docs/learn/configuration/instance_options#environment) | `--env` | `development` | `development` or `production` |
| [`MEILI_MASTER_KEY`](https://www.meilisearch.com/docs/learn/configuration/instance_options#master-key) | `--master-key` | なし | 16 バイト以上の UTF-8 文字列 |

デフォルトではアクセス制御を行うためのマスターキーが設定されず、Meilisearch サーバーはどのような API 呼び出しも受け付けてしまいます（インデックスデータの更新を含めて）。
マスターキーを設定して Meilisearch サーバーを起動すると、API への匿名アクセスができなくなります（ヘルスチェック用の API エンドポイント `/health` を除く）。


マスターキーと API キー
----

Meilisearch のキーには、__マスターキー (master key)__ と __API キー (API keys)__ の 2 種類があります。
どちらのキーも、API 呼び出し時の `Authorization` ヘッダーに指定するものですが、次のような用途の違いがあります。

### マスターキー (master key)

Meilisearch サーバーを起動するときに、__`MEILI_MASTER_KEY`__ 環境変数、あるいは __`--master-key`__ オプションで設定するキーです。
本番環境 (`--env=production`) で起動する場合、必ずマスターキーを指定する必要があります。

API 呼び出し時にマスターキーを使うと、どんな処理でも実行できてしまうので、マスターキーは厳重に管理する必要があります。
後述の「API キー」を管理するための `/keys` エンドポイントには、マスターキーでしかアクセスできません。
逆に、マスターキーはこの `/keys` エンドポイントへのアクセスのみに使用することが推奨されています。

### API キー (API keys)

各種クライアントからの API 呼び出し時に使用するキーです。
API キーは、マスターキーを使って __`/keys`__ エンドポイントにアクセスすることで、生成・削除・リスト表示できます。
API キーの生成時に、その権限を次のようなプロパティで設定できるようになっています。

- __`indexes`__ プロパティ ... どのインデックスにアクセス可能か (例: `"indexes": ["*"]`)
- __`actions`__ プロパティ ... どのような操作が可能か（例: `"actions": ["search"]`）
- __`expiresAt`__ プロパティ ... いつまで使用できるか（例: `"expiresAt": null`）

マスターキーを指定して Meilisearch サーバーを起動すると、次のような 2 つの デフォルト API キーが自動生成されます。

Default Admin API Key
: すべてのインデックスに対して、すべての API 呼び出しを行うことができる API キーです。
ただし、API キー自体を管理するための `/keys` エンドポイントにはアクセスできません（マスターキーが必要）。
主にインデックスを更新するときに使用します。
キーの使用期限はありません。

Default Search API Key
: すべてのインデックスの読み取りアクセス (`search`) が可能な API キーです。
クライアントアプリケーションから検索処理を行うときに使用します。
キーの使用期限はありません。

自動生成されるデフォルト API キーをそのまま使ってもよいですし、アプリケーションごとに新しく API キーを生成するのもよいです。
デフォルト API キーは削除することが可能です。


マスターキーを生成する
----

マスターキーには、UTF-8 で 16 バイト以上の任意の文字列を設定できます。
英数字であれば通常は 16 文字以上に相当します。
次のような Linux コマンドを使用して、ランダムなマスターキー文字列を生成できます。

{{< code lang="console" title="ランダムなマスターキーの生成方法" >}}
$ openssl rand -hex 16
8801d3d091409c4fedd56bd425dc1a0d

$ uuidgen
FA41233F-B85E-4749-8E95-A90118171FE8
{{< /code >}}

あるいは、[RandomKeygen のサイト](https://randomkeygen.com/) で表示されたランダム文字列を使ってもよいですし、マスターキーを指定せずに Meilisearch サーバーを起動したときに表示される、次のようなメッセージを利用するのでも OK です。

```
We generated a new secure master key for you (you can safely use this token):

>> --master-key xaAEQv0e4wVMJ4QgaRlLpK2TZGUBply4Gliix8hk43E <<
```


本番環境で Meilisearch サーバーを起動する
----

Meilisearch サーバーを本番環境 (`--env=production`) として起動するときは、マスターキーも合わせて指定する必要があります。
例えば、次のように環境変数で設定しておくことができます。

{{< code lang="sh" title="環境変数によるマスターキーの設定" >}}
export MEILI_MASTER_KEY=8801d3d091409c4fedd56bd425dc1a0d
{{< /code >}}

ここでは、Docker Compose ファイルを使って Meilisearch コンテナーを起動することにします。
ポイントは、ハイライトされた環境変数の部分 (`environment:`) です。

{{< code lang="yaml" title="docker-compose.yml" hl_lines="9-12" >}}
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
      - MEILI_MASTER_KEY  # マスターキーはシェルの環境変数をそのまま渡す
    volumes:
      - type: bind
        source: ./meili_data
        target: /meili_data
        bind:
          create_host_path: true
{{< /code >}}

{{< accordion title="bind ではなく volume マウントを使う場合" >}}
{{< code lang="yaml" hl_lines="9-12" >}}
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
      - MEILI_MASTER_KEY  # マスターキーはシェルの環境変数をそのまま渡す
    volumes:
      - type: volume
        source: meili_data
        target: /meili_data
        volume:
          nocopy: true

# 作成するボリュームの定義
volumes:
  meili_data:
    # name: meili_data  # プロジェクト名のプレフィックスを付けたくないとき
{{< /code >}}
{{< /accordion >}}

このファイルがあるディレクトリで、次のように Meilisearch コンテナーを起動・停止できます。

```console
$ docker compose up -d  # Meilisearch コンテナーの起動
$ docker compose down   # Meilisearch コンテナーの停止
```


本番環境で Meilisearch の API を呼び出してみる
----

マスターキーを指定して Meilisearch を起動すると、各 API エンドポイントへのアクセス時に API キー（あるいはマスターキー）が必要になります。
ただし、ヘルスチェック用の __`/health`__ エンドポイントだけはそのままアクセスできます。

```console
$ curl 127.0.0.1:7700/health  # アクセスできる
{"status":"available"}

$ curl 127.0.0.1:7700/keys  # アクセスできない
{"message":"The Authorization header is missing. ...省略...
```

### API キーの確認

HTTP の GET メソッドで [__`/keys`__ エンドポイント](https://www.meilisearch.com/docs/reference/api/keys) にアクセスすると、API キーの一覧を取得することができます。
`Authorization` ヘッダーで __マスターキー__ を指定する必要があります。

```
$ curl -H "Authorization: Bearer 8801d3d091409c4fedd56bd425dc1a0d" \
    http://localhost:7700/keys | jq
```

{{< accordion title="出力例" >}}
{{< code lang="json" >}}
{
  "results": [
    {
      "name": "Default Search API Key",
      "description": "Use it to search from the frontend",
      "key": "d044f32b03a0b7b2b38e96a5fe3ca44dbb3bca443ec8458d184a8f89d34eaf78",
      "uid": "e6d62591-4123-45e4-a68f-5f19f9961def",
      "actions": [
        "search"
      ],
      "indexes": [
        "*"
      ],
      "expiresAt": null,
      "createdAt": "2024-02-15T15:20:06.785410298Z",
      "updatedAt": "2024-02-15T15:20:06.785410298Z"
    },
    {
      "name": "Default Admin API Key",
      "description": "Use it for anything that is not a search operation. Caution! Do not expose it on a public frontend",
      "key": "29847d8297d17657ac98555cb7059b2a59288509f84fd28e87f0430d15f04ae9",
      "uid": "7b372564-d644-4aac-a783-64b72bbc69e5",
      "actions": [
        "*"
      ],
      "indexes": [
        "*"
      ],
      "expiresAt": null,
      "createdAt": "2024-02-15T15:20:06.783015465Z",
      "updatedAt": "2024-02-15T15:20:06.783015465Z"
    }
  ],
  "offset": 0,
  "limit": 20,
  "total": 2
}
{{< /code >}}
{{< /accordion >}}

{{% note title="jq コマンド" %}}
上記で使用している __`jq`__ コマンドは、JSON 出力の整形ツールです。
インストール方法は、[公式の download ページ](https://jqlang.github.io/jq/download/) に記述されています。
下記はインストール方法の一例です。

- macOS の場合: `brew install jq` / `port install jq`
- Windows の場合: `winget install jqlang.jq` / `choco install jq`
- Debian / Ubuntu の場合: `sudo apt install -y jq`
{{% /note %}}

`/keys` エンドポイントからの出力結果を抜粋して見ると、前述したように 2 つのデフォルト API キーが生成されていることを確認できます。

```
"name": "Default Search API Key",
"key": "d044f32b03a0b7b2b38e96a5fe3ca44dbb3bca443ec8458d184a8f89d34eaf78",
...
"name": "Default Admin API Key",
"key": "29847d8297d17657ac98555cb7059b2a59288509f84fd28e87f0430d15f04ae9",
```

主に、`Default Search API Key` の方はフロントエンドアプリからの検索に使用し、`Default Admin API Key` の方はインデックスの更新などに使用します。

### 新しい API キーを作成する

特定のインデックスへのアクセスだけを許可する API キーを作りたい場合は、`/keys` エンドポイントに __`POST`__ メソッドでアクセスします。
次の例では、`books` インデックスの検索のみを許可する API キーを作成しています。

{{< code title="books インデックス検索用の API キーを作成" hl_lines="7" >}}
$ curl \
    -X POST 'http://localhost:7700/keys' \
    -H 'Content-Type: application/json' \
    -H 'Authorization: Bearer 8801d3d091409c4fedd56bd425dc1a0d' \
    --data-binary '{
      "name": "Search key for books index",
      "actions": ["search"],
      "indexes": ["books"],
      "expiresAt": "2025-01-01T00:00:00Z"
    }'
{{< /code >}}

`"actions": ["search"]` というアクション指定は、`/indexes/{index_uid}/search` エンドポイントへの `GET` および `POST` アクセスのみを許可することを示しています。
API キーの作成に成功すると、次のように出力されます。
`key` プロパティに表示されている長い文字列が、検索時に使用する API キーです。

{{< code lang="json" title="生成された API キー" hl_lines="4" >}}
{
  "name": "Search key for books index",
  "description": null,
  "key": "e4dca0e0e82f6af276d3090a6e9294e61288bdbd9ee9097a91cd3ad393f57cc0",
  "uid": "800f8446-5813-429b-8508-7fcd97370b01",
  "actions": [
    "search"
  ],
  "indexes": [
    "books"
  ],
  "expiresAt": "2025-01-01T00:00:00Z",
  "createdAt": "2024-02-15T16:05:03.075883962Z",
  "updatedAt": "2024-02-15T16:05:03.075883962Z"
}
{{< /code >}}

インデックスの更新（ドキュメントの追加）などを行うための API キーには、[`actions` プロパティ](https://www.meilisearch.com/docs/reference/api/keys#actions)として、`documents` や `indexes` などを指定する必要があります。
ワイルドカード (__`*`__) を指定すると、すべてのアクションが許可されます。
ワイルドカードの使用は推奨されないようですが、ちゃんとキー管理しておけば、まぁ大丈夫かと思います。

{{< code title="管理用の API キーを作成" hl_lines="7" >}}
$ curl \
    -X POST 'http://localhost:7700/keys' \
    -H 'Content-Type: application/json' \
    -H 'Authorization: Bearer 8801d3d091409c4fedd56bd425dc1a0d' \
    --data-binary '{
      "name": "Admin key for books index",
      "actions": ["*"],
      "indexes": ["books"],
      "expiresAt": "2025-01-01T00:00:00Z"
    }'
{{< /code >}}

これで、`books` インデックス専用の「検索用 API キー」と「管理用 API キー」を作成できました。

### API キーの削除

不要な API キーを削除するには、__`DELETE`__ メソッドで `/keys/{uid}` エンドポイントにアクセスします。

{{< code title="API キーの削除" >}}
$ curl \
  -X DELETE 'http://localhost:7700/keys/800f8446-5813-429b-8508-7fcd97370b01' \
  -H 'Authorization: Bearer 8801d3d091409c4fedd56bd425dc1a0d'
{{< /code >}}

URL のパスの末尾には、削除したい API キーの uid を指定しますが、uid がわからない場合は、`GET` メソッドで API キーの一覧を確認しましょう。

{{< code title="API キーの uid を確認" >}}
$ curl -X GET 'http://localhost:7700/keys' \
    -H 'Authorization: Bearer 8801d3d091409c4fedd56bd425dc1a0d' \
    | jq '.results[] | {name, uid}'

{
  "name": "Admin key for books index",
  "uid": "fd5efe58-cd71-428a-b6d4-34a04f5f1ff4"
}
{
  "name": "Search key for books index",
  "uid": "800f8446-5813-429b-8508-7fcd97370b01"
}
{{< /code >}}


### インデックスの作成と検索

適切な API キーを準備できたら、あとはそれらを使ってインデックスを作成し、検索するだけです。
ここでは、下記のようなテスト用ドキュメントを投入することにします。

{{< accordion title="books.json" >}}
{{< code lang="json" >}}
[
  {
    "id": "1",
    "title": "The Great Gatsby",
    "author": "F. Scott Fitzgerald",
    "genre": "Fiction",
    "year": 1925
  },
  {
    "id": "2",
    "title": "To Kill a Mockingbird",
    "author": "Harper Lee",
    "genre": "Fiction",
    "year": 1960
  },
  {
    "id": "3",
    "title": "1984",
    "author": "George Orwell",
    "genre": "Science Fiction",
    "year": 1949
  },
  {
    "id": "4",
    "title": "Pride and Prejudice",
    "author": "Jane Austen",
    "genre": "Romance",
    "year": 1813
  },
  {
    "id": "5",
    "title": "Moby-Dick",
    "author": "Herman Melville",
    "genre": "Adventure",
    "year": 1851
  }
]
{{< /code >}}
{{< /accordion >}}

まず、管理用の API キーを使って、`books` インデックスを作成します。

{{< code title="books インデックスの作成" >}}
$ curl -X POST 'http://localhost:7700/indexes/books/documents' \
    -H 'Content-Type: application/json' \
    -H 'Authorization: Bearer 275e10e7234602...省略...' \
    --data-binary @books.json
{{< /code >}}

次に、検索用の API キーを使って、`books` 内のドキュメントを検索します。
ペイロード部分 (`--data-binary`) でクエリ文字列を送るため、`GET` ではなく `POST` メソッドを使うことに注意してください（参考: [Search API](https://www.meilisearch.com/docs/reference/api/search)）。

{{< code title="books インデックスの検索" >}}
$ curl -X POST 'http://localhost:7700/indexes/books/search' \
    -H 'Content-Type: application/json' \
    -H 'Authorization: Bearer ef43c9a2cac822...省略...' \
    --data-binary '{ "q": "fiction" }' | jq
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code lang="json" >}}
{
  "hits": [
    {
      "id": "1",
      "title": "The Great Gatsby",
      "author": "F. Scott Fitzgerald",
      "genre": "Fiction",
      "year": 1925
    },
    {
      "id": "2",
      "title": "To Kill a Mockingbird",
      "author": "Harper Lee",
      "genre": "Fiction",
      "year": 1960
    },
    {
      "id": "3",
      "title": "1984",
      "author": "George Orwell",
      "genre": "Science Fiction",
      "year": 1949
    }
  ],
  "query": "fiction",
  "processingTimeMs": 0,
  "limit": 20,
  "offset": 0,
  "estimatedTotalHits": 3
}
{{< /code >}}
{{< /accordion >}}

その他の API も、上記のような感じで API キーを使って呼び出せば OK です。

