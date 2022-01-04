---
title: "Azure Table Strage を使ってみる (2) 接続情報（キー）を確認する"
url: "/p/dofzeua"
date: "2020-01-31"
tags: ["Azure Table Storage"]
weight: 102
---

Table Storage サービスの [ストレージアカウントを作成](/p/xyzwod2) すると、各種プログラムから Table Storage にアクセスするための接続情報（キー）を取得することができます。

Table Storage の接続情報は、[Azure ポータル](https://portal.azure.com/) からストレージアカウントのリソースを開き、**`設定`** → **`アクセスキー`** と辿ると確認することができます。

{{< image border="true" src="img-key-info.png" >}}

Python や Node.js などのプログラムから Azure Storage に接続するには、上の図の中の、

- **ストレージアカウント名** と **キー** のペア
- **接続文字列**

のいずれかの情報が必要です。

