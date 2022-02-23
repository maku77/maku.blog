---
title: "Elasticsearch を Docker コンテナで動かしてみる"
url: "/p/panx9it"
date: "2022-01-30"
tags: ["Docker", "Elasticsearch"]
draft: true
---

{{% private %}}
- [Install Elasticsearch with Docker | Elasticsearch Guide](https://www.elastic.co/guide/en/elasticsearch/reference/current/docker.html)
{{% /private %}}


はじめに
----

Elasticsearch はオープンソースとして提供されている検索・分析エンジンです。
Docker イメージとしても公開されているため、Docker 環境があれば、簡単に試してみることができます。

- [Elasticsearch の Docker イメージ一覧](https://www.docker.elastic.co/r/elasticsearch)


Docker イメージをダウンロードする
----

Elasticsearch の Docker イメージは、`docker pull` コマンドで簡単に取得できます。

```console
$ docker pull docker.elastic.co/elasticsearch/elasticsearch:7.16.3
```

ちゃんとダウンロードできているか確認します。

```console
$ docker image ls
REPOSITORY                                      TAG       IMAGE ID       CREATED       SIZE
docker.elastic.co/elasticsearch/elasticsearch   7.16.3    3a5e93284781   3 weeks ago   611MB
```


Docker コンテナを起動する
----

Elasticsearch の Docker コンテナを起動するには、次のように `docker run` を実行します。
ここではコンテナ名を `elasticsearch` にしていますが、名前は何でも構いません。

```console
$ docker run --name elasticsearch \
    -p 9200:9200 -p 9300:9300 -e "discovery.type=single-node" \
    docker.elastic.co/elasticsearch/elasticsearch:7.16.3
```

Elasticsearch では、ポート 9200 番を REST API 用、ポート 9300 番をクラスタ内のノード通信用に使用するので、これらをホストマシンの同じポートと関連付けています。
1 ノード構成のシンプルな構成で起動するには、上記のように `discovery.type` 環境変数に `single-node` を設定します。

上記コマンドでコンテナを起動すると、Elasticsearch のログがだーーっと流れ始めます。
別のターミナルから、次のように REST API にアクセスできればうまく動作しています。
単なる HTTP GET アクセスなので、Web ブラウザで [http://localhost:9200](http://localhost:9200) を開くことでも確認できます。

```console
$ curl -X GET http://localhost:9200
{
  "name" : "6a1892fc367a",
  "cluster_name" : "docker-cluster",
  "cluster_uuid" : "pT2ciOcAQz-NSRYTQ_imRw",
  "version" : {
    "number" : "7.16.3",
    "build_flavor" : "default",
    "build_type" : "docker",
    "build_hash" : "4e6e4eab2297e949ec994e688dad46290d018022",
    "build_date" : "2022-01-06T23:43:02.825887787Z",
    "build_snapshot" : false,
    "lucene_version" : "8.10.1",
    "minimum_wire_compatibility_version" : "6.8.0",
    "minimum_index_compatibility_version" : "6.0.0-beta1"
  },
  "tagline" : "You Know, for Search"
}
```

