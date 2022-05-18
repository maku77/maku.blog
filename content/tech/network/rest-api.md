---
title: "REST API 設計に関するメモ"
url: "p/8cpnayf/"
date: "2022-05-18"
tags: ["REST"]
draft: true
---

Swagger
----

[Swagger](https://swagger.io/) は、REST API を構築する際に使用するフレームワークです。
Swagger は [OpenAPI Initiative](https://www.openapis.org/) が定める OpenAPI Specification (OAS) に基づいた一連のツールを提供します。

- [Swagger Editor](https://editor.swagger.io/) ... OAS に従う REST API 仕様を記述するためのエディタ。Web ブラウザ上で動作します。
- Swagger UI .. API 仕様から HTML ドキュメントを生成するツール
- Swagger Codegen ... API 仕様からコードを生成するツール


JSON Schema
----

REST API のリソースの定義には [JSON Schema](https://json-schema.org/) を使うと分かりやすく記述できます。

- JSON Schema の利点
  - 人間にも機械に読めるドキュメントとなる。
  - データに対する制約（10 文字以内など）を記述できるので、アプリケーション内でのバリデーションに利用できる。
  - JSON Schema 自体が JSON ファイルになるので、プログラムから利用しやすい。

下記は [公式サイトの例](https://json-schema.org/learn/miscellaneous-examples.html) からの抜粋です。

{{< code lang="json" title="JSON Schema" >}}
{
  "$id": "https://example.com/person.schema.json",
  "$schema": "https://json-schema.org/draft/2020-12/schema",
  "title": "Person",
  "type": "object",
  "properties": {
    "firstName": {
      "type": "string",
      "description": "The person's first name."
    },
    "lastName": {
      "type": "string",
      "description": "The person's last name."
    },
    "age": {
      "description": "Age in years which must be equal to or greater than zero.",
      "type": "integer",
      "minimum": 0
    }
  }
}
{{< /code >}}

{{< code lang="json" title="Data" >}}
{
  "firstName": "John",
  "lastName": "Doe",
  "age": 21
}
{{< /code >}}


エンドポイント
----


