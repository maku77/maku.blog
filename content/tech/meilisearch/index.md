---
title: "Meilisearch サーチエンジンを導入する"
url: "p/bo8q8p7/"
date: "2022-10-16"
tags: ["Meilisearch"]
draft: true
---

Meilisearch とは？
----

{{< image border="true" src="https://docs.meilisearch.com/search_preview/no_documents.png" title="Meilisearch ダッシュボード" >}}

[Meilisearch](https://www.meilisearch.com/) は、簡単に使えることを目標にして作られた OSS のサーチエンジンで、フロントエンドからの検索をターゲットにしています。
個人や中小企業でも手軽に導入でき、柔軟な設定により様々な用途に対応できます。
Meilisearch には次のような特徴があります。

- __高速な応答__（検索結果は 50 ミリ秒以内に得られる。ただし、1,000 万件以上の大規模なデータは想定していない）
- __インクリメンタルサーチ__（入力するたびにプレフィックスで絞り込んで結果を返す）
- __自動 typo 補正__（タイポ入力してもそれっぽい単語で検索してくれる）
- __プレースホルダー検索__（キーワードを入力せずにカテゴリーなどで結果を絞り込む）
- __カスタムランキング処理__（ranking rules による検索結果のソート）
- __検索結果のハイライト表示__（入力キーワードに一致した位置がわかる）
- __日本語を含む各種言語に対応__（形態素解析エンジンに Lindera を採用）
- __同義語の定義__

Meilisearch は __簡単であること__ を哲学としているので、類似の検索フレームワークに比べて全体的にシンプルです。
Elasticsearch などでは大袈裟すぎるというニーズにマッチするかもしれません。
JSON 形式のドキュメントを REST API で登録してインデックスを作成するだけで、REST API による検索が可能になります。
最終的にはフロントエンドから直接 REST API を呼び出すように UI を作り込んでいく必要がありますが、プレビュー用の Web サーバー（ダッシュボード）が内蔵されているので、コーディングなしでインクリメンタルサーチを試すことができます。

Meilisearch は Algolia にインスパイアされて作られているので、アルゴリズム等は似ていますが、オープンソースであるという大きな違いがあります（Algolia は個人で使うには高すぎますし...）。


Meilisearch サーバーを起動する
----

Docker Hub で [getmeili/meilisearch](https://hub.docker.com/r/getmeili/meilisearch/) というイメージが公開されているので、これを使えば簡単に Meilisearch のサーバーを立ち上げることができます。

{{< code title="meilisearch サーバーの起動" >}}
$ docker run -it --rm -p 7700:7700 \
      -v $(pwd)/meili_data:/meili_data \
      getmeili/meilisearch:v0.30
{{< /code >}}

- `docker run` のオプションの意味:
  - `-it` ... ターミナルとコンテナの標準入出力を連動させます。
  - `--rm` ... コンテナを停止したときに、コンテナを自動で削除します。
  - `-p 7700:7700` ... `localhost:7700` へのアクセスをコンテナ内の Meilisearch サーバーへ転送します。
  - `-v $(pwd)/meili_data:/meili_data` ... カレントディレクトリの `meili_data` ディレクトリを、Meilisearch のデータ格納先として使用します。ディレクトリが存在しない場合は、自動で作成されます。

Meilisearch はデフォルトで development モード (`--env=development`) で起動し、プレビュー用の UI を提供します。
コンテナが起動したら、ブラウザで __`http://localhost:7700/`__ にアクセスすると、Meilisearch のダッシュボードを表示できます。


プライマリーフィールド
----

{{< image w="300" src="img-001.drawio.svg" >}}



(MEMO) インデックス情報を登録する
----

`books` というインデックスを作成し、そのドキュメントとして JSON ファイルで示されるドキュメント群を登録する、という意味になります。

```
curl \
  -X POST 'http://localhost:7700/indexes/books/documents?primaryKey=reference_number' \
  -H 'Content-Type: application/json' \
  --data-binary '[
    {
      "reference_number": 287947,
      "title": "Diary of a Wimpy Kid",
      "author": "Jeff Kinney",
      "genres": [
        "comedy",
        "humor"
      ],
      "price": 5.00
    }
  ]'
```

{{< code lang="console" title="meilisearch サーバーの起動" >}}
docker run -it --rm \
    -p 7700:7700 \
    -e MEILI_MASTER_KEY='MASTER_KEY'\
    -v $(pwd)/meili_data:/meili_data \
    getmeili/meilisearch:v0.30 \
    meilisearch --env="development"
{{< /code >}}

マスターキーを指定しないと、Meilisearch サーバーはデフォルトでどのような要求も受け付けます（インデックスデータの更新を含めて）。

```
export MEILI_MASTER_KEY=xxx
```

