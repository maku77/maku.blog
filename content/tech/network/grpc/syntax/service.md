---
title: ".proto の文法: サービス型 (service)"
url: "p/napwb4e/"
date: "2022-12-13"
tags: ["Protocol Buffers"]
draft: true
---

{{% private %}}
- [サービスの定義](https://developers.google.com/protocol-buffers/docs/proto3#services)
{{% /private %}}

Protoco Buffers の __サービス型__ は、クライアントとサーバー間の通信方法 (RPC メソッド群) を定義するための型で、`.proto` ファイルの中で __`service`__ キーワードを使って定義します。
gRPC プロトコルを使ったアプリケーションが使用する、スタブコードなどの生成に使用します。

