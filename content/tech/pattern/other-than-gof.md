---
title: "DRAFT: GoF 以外のデザインパターン"
url: "/p/4q4fs4f"
date: "2014-06-27"
tags: ["デザインパターン"]
draft: true
---

{{% private %}}
- https://www.corej2eepatterns.com/index.htm
{{% /private %}}

Work Queue Processor（ワークキュープロセッサー）
----

タスクを保持するためのキューを 1 つ用意し、1 つのワーカースレッドで順次タスクを処理していきます。
Android の `IntentService` はこのパターンで実装されています（シングルスレッドで `Intent` を処理する `Service` です）。


J2EE Design Pattern <!-- 2014-06-12 -->
----

- プレゼンテーション層 (Presetntation Layer)
- ビジネス層 (Business Layer)
  - Application Service
  - Business Object
  - データベースパターン
    - DataTransfer Pattern
    - DataTransferHash Pattern
    - DataTransferObject (DTO) Pattern
- ユーティリティ
  - Business Delegate
  - Service Locator
  - Value List Handler
  - Web Service Broker
- Web アプリ
  - Application Controller
  - Composite View
  - Context Object
  - Dispatcher View
  - Front Controller
  - Intercepting Filter
  - Service To Worker
  - View Helper
- 永続化
  - Data Access Object
  - Domain Store
- 分散ビジネスコンポーネント
  - Composite Entity
  - Service Activator
  - Session Facade
  - TO Assembler
  - Transfer Object

