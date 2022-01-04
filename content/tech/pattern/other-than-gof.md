---
title: "DRAFT: GoF 以外のデザインパターン"
url: "/p/4q4fs4f"
date: "2014-06-27"
tags: ["デザインパターン"]
draft: true
---

Work Queue Processor（ワークキュープロセッサー）
----

タスクを保持するためのキューを 1 つ用意し、1 つのワーカースレッドで順次タスクを処理していきます。
Android の `IntentService` はこのパターンで実装されています（シングルスレッドで `Intent` を処理する `Service` です）。


J2EE Design Pattern <!-- 2014-06-12 -->
----

- Presetntation Layer
- Business Layer
    - データベースパターン
        - DataTransfer Pattern
        - DataTransferObject (DTO) Pattern
        - DataTransferHash Pattern

