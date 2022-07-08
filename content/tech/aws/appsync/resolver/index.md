---
title: "（ドラフト）AppSync のリゾルバーまわりのメモ"
url: "p/neydnx9/"
date: "2022-07-08"
tags: ["AWS/AppSync"]
draft: true
---

用語
----

__Automatic Provision__ ... スキーマを定義するだけで AppSync が自動で DynamoDB のテーブルやリゾルバを定義してくれる仕組み。


AppSync のリクエスト／レスポンスの基本構造
----

{{< image src="img-001.drawio.svg" >}}

開発の流れとしては、まず何らかの Data Source を AppSync に登録しておいてから、リゾルバーを各タイプのフィールドにアタッチするという手順になります。
そして、リゾルバー実装の中でその Data Source からデータを取り出して適切なレスポンスフォーマットで GraphQL クライアントに返します。


AppSync リゾルバー
----

レスポンステンプレート (VTL) の中では、DynamoDB クエリで取得したリストは、`$ctx.result.items` で参照できます。
単一の項目は、`$ctx.result` で参照できます。
そして、それらを GraphQL クエリの結果として返すためには、`$util.toJson($ctx.result.items)` のように記述します。

### レスポンステンプレートで未承認処理

```css
#if ($ctx.identity.username == "maku")
    $util.toJson($ctx.result)
#else
    $util.unauthorized()
#end
```

### パイプラインリゾルバーの構成

1. Before マッピングテンプレート（パイプラインリゾルバーにおける Request マッピングテンプレートのこと？）
   1. 関数（Request/Response マッピングテンプレート）
   1. 関数（Request/Response マッピングテンプレート）
   1. ...
1. After マッピングテンプレート（パイプラインリゾルバーにおける Response マッピングテンプレートのこと？）

前の関数の結果は、`$ctx.prev.result` で受け取ることができます。
（After マッピングテンプレートに限り？）`$ctx.result` でも参照できます。

関数間でデータ共有したいときは、`$ctx.stash`（実体は Java の Map）を使えます。

```css
$util.qr($ctx.stash.put("email", "test@example.com"))
```

