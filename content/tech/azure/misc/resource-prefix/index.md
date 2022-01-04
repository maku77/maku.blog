---
title: "Azure リソースのプレフィックス名（省略名）"
url: "/p/rcoz9o9"
date: "2021-06-12"
tags: ["Azure"]
---

Azure で何らかのリソースを作成するときは、__リソース名のプレフィックス__ として、リソースの種類に応じた略称（`cosmos-` など）をつけることが多いのですが、この略称の指針が Microsoft Docs のサイトに書かれています。
リソース名をどうするかは意外と迷うところなので、こういった情報は地味に助かります。

- [Azure リソースの種類に推奨される省略形 - Cloud Adoption Framework | Microsoft Docs](https://docs.microsoft.com/ja-jp/azure/cloud-adoption-framework/ready/azure-best-practices/resource-abbreviations)

例えば次のような感じで定義されています。

| リソースの種類 | プレフィックス |
| ---- | ---- |
| リソースグループ | `rg-` |
| Cosmos DB アカウント | `cosmos-` |
| ストレージアカウント | `st-` |
| 静的 Web アプリ | `stapp-` |
| 関数アプリ | `func-` |

また、[リソース名全体の構成](https://docs.microsoft.com/ja-jp/azure/cloud-adoption-framework/ready/azure-best-practices/resource-naming)も次のような例で示されています。

{{< image w="500" src="img-001.png" title="Azure リソース名の構成（Microsoft Docs より）" >}}

この辺りは開発チームによってルールが決められているかもしれませんが、まずはこの構成で間に合うか考えてみるとよいと思います。

