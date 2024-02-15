---
title: "Meilisearch サーチエンジンを使ってみる"
url: "p/bo8q8p7/"
date: "2023-01-24"
lastmod: "2024-01-21"
changes:
  - 2024-01-21: Meilisearch のバージョンを更新
tags: ["Meilisearch"]
weight: 1
---

Meilisearch とは？
----

[Meilisearch](https://www.meilisearch.com/) は、簡単に使えることをポリシーとして作られた OSS のサーチエンジンで、これを利用すると、アプリの __フロントエンドに高速な検索機能__ を組み込むことができます。
個人や中小企業でも手軽に導入でき、柔軟な設定により様々な用途に対応できます。
Meilisearch には次のような特徴があります。

- __高速な応答__（検索結果は 50 ミリ秒以内に得られる。ただし、1,000 万件以上の大規模なデータは想定していない）
- __インクリメンタルサーチ__（入力するたびにプレフィックスで絞り込んで結果を返す）
- __タイポ入力補正__（検索語句を多少間違えて入力してもそれっぽい単語で検索してくれる）
- __プレースホルダー検索__（キーワードを入力せずにカテゴリーなどで結果を絞り込む）
- __カスタムランキング処理__（ranking rules による検索結果のソート）
- __検索結果のハイライト表示__（入力キーワードに一致した位置がわかる）
- __日本語を含む各種言語に対応__（形態素解析エンジンに Lindera を採用）
- __同義語の定義__

Meilisearch は __簡単であること__ を哲学としているので、類似の検索フレームワークに比べて全体的にシンプルな構成になっています。
Elasticsearch などでは大袈裟すぎるというニーズにマッチするかもしれません。
JSON 形式のドキュメントを REST API で登録してインデックスを作成するだけで、REST API による検索が可能になります。
最終的にはフロントエンドから直接 REST API を呼び出すように UI を作り込んでいく必要がありますが、プレビュー用の Web サーバー（ダッシュボード UI）が内蔵されているので、コーディングなしでインクリメンタルサーチを試すことができます。

{{< image border="true" src="img-003.png" title="Meilisearch ダッシュボード" >}}

似たような検索エンジンとしては、Algolia という SaaS があります。
Meilisearch は Algolia にインスパイアされて作られているので、アルゴリズム等は似ていますが、__オープンソース__ であるという大きな違いがあります（Algolia は個人で使うには高すぎるというのもあります...）。


Meilisearch サーバーを起動する
----

Docker Hub に [getmeili/meilisearch](https://hub.docker.com/r/getmeili/meilisearch/) というイメージが公開されているので、これを使えば簡単に Meilisearch のサーバーを立ち上げることができます。

{{< code title="meilisearch サーバーの起動" >}}
docker run --rm -it -p 7700:7700 \
    -v $(pwd)/meili_data:/meili_data \
    getmeili/meilisearch:v1.6
{{< /code >}}

- `docker run` のオプションの意味:
  - __`--rm`__ ... コンテナを停止したときに、コンテナを自動で削除します。
  - __`-it`__ ... ターミナルとコンテナの標準入出力を連動させます。
  - __`-p 7700:7700`__ ... `localhost:7700` へのアクセスをコンテナの 7700 番ポート（Meilisearch サービス）へ転送します。
  - __`-v $(pwd)/meili_data:/meili_data`__ ... カレントディレクトリの `meili_data` ディレクトリを、Meilisearch のデータ格納先として使用します。ディレクトリが存在しない場合は、自動で作成されます。
  - __`getmeili/meilisearch:v1.6`__ ... 使用する Meilisearch イメージを指定しています。最新バージョンのタグは [Docker Hub](https://hub.docker.com/r/getmeili/meilisearch/tags) 上で確認してください。

Meilisearch はデフォルトで development モード (`--env=development`) で起動し、プレビュー用の UI を提供します。
コンテナが起動したら、ブラウザで __`http://localhost:7700/`__ にアクセスすると、Meilisearch のダッシュボードを表示できます。


プライマリーフィールド
----

Meilisearch を使い始めるときは、最初に __プライマリーフィールド__ の概念を理解しておく必要があります。

Meilisearch に登録するデータのことを __ドキュメント__ と呼びますが、各ドキュメントには必ず、プライマリーフィールドと呼ばれる一意の ID フィールドが必要です。
逆に言うと、これ以外のフィールドの値は、どれだけ重複していても大丈夫です。
同一のドキュメント ID を持つドキュメントを登録しようとすると、古いドキュメントは上書きされます。

{{< image w="300" src="img-001.drawio.svg" title="Meilisearch のプライマリフィールド関連用語" >}}

多くの場合、プライマリーフィールドのキー名（プライマリーキー）は、`id` といった名前ですが、インデックスを作成するときに任意のキー名を指定することができます。
キー名を省略すると、1 つ目のドキュメントから `id` を含む名前のキーが自動的に選択されます（例: `id`、`bookId`、`todo-id`）。

インデックスにドキュメントを登録するとき、そのフィールドにプライマリーキーが見つからない場合は登録に失敗します。
ドキュメント ID が不正な形式の場合も登録に失敗します。
ドキュメント ID は、整数値、あるいは、英数字 (`a-zA-Z0-9`)、ハイフン (`-`)、アンダースコア (`_`) のみで構成された文字列である必要があります。

{{< code title="ドキュメント ID の形式" >}}
"id": 12345      // Good
"id": "a39NfE2"  // Good
"id": "x-12_34"  // Good

"id": 0.12345    // NG（浮動小数点数はダメ）
"id": "df/3 #@"  // NG（不正な記号やスペース）
{{< /code >}}


インデックスの作成とドキュメントの追加
----

Meilisearch で検索を行えるようにするには、インデックスを作成して、そこに JSON 形式のドキュメントを投入していく必要があります。
それぞれの作業を独立して行うこともできますし、ドキュメントを追加するときに同時にインデックスを作成してしまうこともできます。
Meilisearch の操作方法としては、各種言語用のライブラリが提供する API を使う方法と、REST API を直接呼び出す方法があります。
ここでは `curl` コマンドで直接 REST API を呼び出して、インデックスの作成とドキュメントの追加をしてみます。

あるインデックスに対してドキュメントを追加するには、__`/indexes/<インデックス名>/documents`__ というエンドポイントに POST メソッドで JSON 形式のデータを送ります。
次の例では、`books` という名前のインデックスを作成し、そこに 3 つの書籍情報（ドキュメント）を登録しています。

{{< code title="ドキュメントの登録（同時に books インデックスを生成）" >}}
curl -X POST 'http://localhost:7700/indexes/books/documents?primaryKey=id' \
    -H 'Content-Type: application/json' \
    --data-binary '[
        { "id": 1, "title": "みんなの Meili", "author": "名無し" },
        { "id": 2, "title": "Meili かわいい", "author": "John Doe" },
        { "id": 3, "title": "Best of Meili", "author": "Maku" }
    ]'
{{< /code >}}

前述の通り、`id` というキーを自動的にプライマリーキーとして採用してくれますが、上記のように `primaryKey=id` でキー名を明示することもできます。
次のように処理内容が出力されれば成功です。

{{< code lang="json" title="実行結果（の出力）" >}}
{"taskUid":3,"indexUid":"books","status":"enqueued","type":"documentAdditionOrUpdate","enqueuedAt":"2023-01-24T07:02:22.4346468Z"}
{{< /code >}}

多数のドキュメントを登録したいときは、次のような JSON ファイルとしてデータを用意しておき、

{{< code lang="json" title="books.json" >}}
[
  { "id": 1, "title": "みんなの Meili", "author": "名無し" },
  { "id": 2, "title": "Meili かわいい", "author": "John Doe" },
  { "id": 3, "title": "Best of Meili", "author": "Maku" }
]
{{< /code >}}

次のように Meilisearch のインデックスに登録します（ファイル名の前に __`@`__ プレフィックスを付けます）。

{{< code title="books インデックスに JSON ファイルのドキュメントを登録" >}}
curl -X POST 'http://localhost:7700/indexes/books/documents?primaryKey=id' \
    -H 'Content-Type: application/json' --data-binary @books.json
{{< /code >}}

Meilisearch サービスに現在登録されているインデックスの一覧を確認するには、__`/indexes`__ というエンドポイントに GET リクエストを投げます。

{{< code lang="console" title="インデックスの一覧を取得（出力は整形してます）" >}}
$ curl -X GET http://localhost:7700/indexes?limit=3
{
  "results": [
    {
      "uid": "books",
      "createdAt": "2023-01-24T06:47:13.0555429Z",
      "updatedAt": "2023-01-24T07:10:13.2373666Z",
      "primaryKey": "id"
    }
  ],
  "offset": 0,
  "limit": 3,
  "total": 1
}
{{< /code >}}

正しく、`books` というインデックスが作成されているようです。
まだインデックスは 1 つしか作成していないので、`total` フィールドの値は 1 になっています。


ドキュメントを取得する
-----

### すべてのドキュメントを取得する

__`/indexes/{index_uid}/documents`__ に GET 要求を送ると、すべてのドキュメントを取得できます。
ただし、`limit` パラメーターのデフォルト値は `20` です。

{{< code lang="console" title="最大 3 つのドキュメントを取得" >}}
$ curl http://localhost:7700/indexes/books/documents?limit=3
{{< /code >}}

__`/indexes/{index_uid}/documents/{document_id}`__ に GET 要求を送ると、指定したドキュメント ID に一致するドキュメントを取得できます。

{{< code lang="console" title="ドキュメント ID = 1 のドキュメントを取得" >}}
$ curl http://localhost:7700/indexes/books/documents/1
{{< /code >}}

### ドキュメントを検索する

__`/indexes/{index_uid}/search`__ に GET あるいは POST 要求を送ると、任意の検索文字列（`q` パラメーター）でドキュメントを検索できます。
この API が Meilisearch による検索のキモとなる API です。

{{< code lang="console" title="Meili で検索（GET リクエスト）" >}}
$ curl http://localhost:7700/indexes/books/search?q=Meili
{{< /code >}}

実運用上は、検索速度の向上のため GET リクエストではなく、__POST リクエストを使用する__ 必要があります（HTTP のプリフライト・リクエストのキャッシュが有効になる）。

{{< code title="Meili で検索（POST リクエスト）" >}}
$ curl -X POST 'http://localhost:7700/indexes/books/search' \
    -H 'Content-Type: application/json' \
    --data-binary '{ "q": "Meili" }'
{{< /code >}}

ちょっと綴りを間違えて、「Meilli」と入力してもうまく検索してくれます。
(*>ω<)bグッ

日本語で検索する場合は、検索文字列を URL エンコードしておく必要があるかもしれません。

{{< code lang="console" title="「かわいい」で検索" >}}
$ curl http://localhost:7700/indexes/books/search?q=%E3%81%8B%E3%82%8F%E3%81%84%E3%81%84
{{< /code >}}

他にも様々なクエリパラメーターが用意されていて、ページングやソート方法などを指定できるようになっています。
詳細は [Search API のドキュメント](https://docs.meilisearch.com/reference/api/search.html) を参照してください。


ダッシュボードで確認する
----

インデックスに登録されたドキュメントは、`http://localhost:7700` で表示されるダッシュボード (Mini Dashboard) からも検索できるようになっています。
ちなみにこれはテスト用の UI で、開発モード (`--env=development`) として起動した場合だけ有効になっています（デフォルトは開発モードです）。

{{< image border="true" src="img-002.png" title="Mini Dashboard で books インデックスを検索" >}}

うまくいきました！
٩(๑❛ᴗ❛๑)۶ わーぃ


（おまけ）テレメトリデータを送らない設定
----

Meilisearch はプロダクト改善のために、デフォルトで __テレメトリデータ__ を収集しています。
データベース内の情報が送信されるようなことはありませんが、社内プロジェクトなどで、いかなるデータも送りたくない場合は、`meilisearch` サーバー起動時に __`--no-analytics`__ オプションを指定します。

