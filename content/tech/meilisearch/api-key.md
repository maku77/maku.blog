---
title: "Meilisearch のアクセスを API キーで制限する"
url: "p/iuvuyzd/"
date: "2023-01-24"
tags: ["Meilisearch"]
draft: true
---

マスターキーの指定
----

Meilisearch サーバーを起動するときにマスターキーを指定しないと、Meilisearch サーバーはデフォルトでどのような要求も受け付けます（インデックスデータの更新を含めて）。
マスターキーを __`MEILI_MASTER_KEY`__ 環境変数で指定することで、インデックスを不特定多数のクライアントから更新されてしまうのを防ぐことができます。

{{< code title="マスターキーを指定して meilisearch を起動する" >}}
$ docker run --rm -it -p 7700:7700 \
    -e MEILI_MASTER_KEY="MASTER_KEY" \
    -v $(pwd)/meili_data:/meili_data \
    getmeili/meilisearch:v1.6 \
    meilisearch --env="development"
{{< /code >}}

本番 (production) 環境では、マスターキーの指定は必須とされています。

