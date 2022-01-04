---
title: "Azure のストレージアカウントを作成する"
url: "/p/7axgzfu"
date: "2020-03-17"
tags: ["Azure", "Azure Storage"]
description: "Azure の Table Storage や BLOB Storage、Queue Storage などは、1 つの Azure Storage アカウントでまとめて管理されます。ここでは、Azure Storage アカウントの作成方法を説明します。"
weight: 1
---

Table Storage などのストレージ系サービスを使用するには、[Azure ポータル](https://portal.azure.com/) から **ストレージアカウント** を作成しておく必要があります。
このトレージアカウントは Table Storage 専用ではなく、様々なストレージサービス（BLOB、ファイル、キュー、テーブル）をまとめて扱うものです。

`ストレージアカウント` → `作成` のような感じで進めば作成用の画面が開くので、ストレージアカウント名などを入力します。
ストレージアカウント名は、Azure 内で一意（要するに世界中で一意）な名前を付ける必要があります。

{{< image w="450" border="true" src="img-create-account.png" >}}

選択項目によっては、料金が変わってくるものがあるので、情報アイコンの説明を見ながら、安いプランを選択していきます。
例えば、Table Storage のレプリケーションの種類別価格は次のようになっています（[こちら](https://azure.microsoft.com/ja-jp/pricing/details/storage/tables/)から抜粋）。

{{< image src="img-price.png" title="Table Storage の月額" >}}

入力が終わったら、`作成` のボタンを押して、しばらく待てばストレージアカウントが作成されます（1分くらいかかります）。

