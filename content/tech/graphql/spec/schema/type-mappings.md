---
title: "GraphQL スキーマ仕様: GraphQL スキーマの型と各言語の型の対応付け"
url: "p/qhv8jv8/"
date: "2022-09-11"
tags: ["GraphQL"]
draft: true
---

| GraphQL の型 | Golang | 意味 |
| ---- | ---- | ---- |
| `ID!` | `string` | ユニークな ID |
| `ID` | `*string` | ユニークな ID (nullable) |
| `String!` | `string` | 文字列 |
| `String` | `*string` | 文字列 (nullable) |
| `Boolean!` | `bool` | 真偽値 |
| `Boolean` | `*bool` | 真偽値 (nullable) |
| `Int!` | `int` | 整数 |
| `Int` | `*int` | 整数 (nullable) |
| `Float!` | `float64` | 浮動小数点数 |
| `Float` | `*float64` | 浮動小数点数 (nullable) |
| `[String!]!` | `[]string` | 文字列の配列 |
| `[String!]` | `[]string` | 文字列の配列 (nullable) |
| `[String]!` | `[]*string` | 文字列 (nullable) の配列 |
| `[String]` | `[]*string` | 文字列 (nullable) の配列 (nullable) |

