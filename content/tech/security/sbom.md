---
title: "SBOM（ソフトウェア部品表）とは"
url: "p/bw9qjeu/"
date: "2022-01-23"
lastmod: "2023-03-28"
changes:
  - GitHub の SBOM 出力について
tags: ["セキュリティ"]
---

ソフトウェア開発の世界で、__SBOM__ という用語がよく出てくるようになったので簡単にまとめておきます。

SBOM とは
----

SBOM とは、ソフトウェア製品の部品表（Software Bill of Materials）の略称で、ソフトウェア製品が構成されている部品のリストを指します。
SBOM は、ソフトウェア製品がどのように構成されているかを明確にするために使用されます。
__2021 年 5 月の米国大統領令に SBOM の作成と脆弱性対策を義務付ける条項__ が盛り込まれました。


SBOM の目的
----

SBOM を作成することにより、以下のような恩恵を得ることができます。

- セキュリティ向上（セキュリティ・マネージメント）: SBOM を利用することで、ソフトウェア製品がどのような部品から構成されているかを正確に把握することができ、悪意のある部品が含まれているかどうかを特定することができます。
- 規制遵守（コンプライアンス・マネージメント）: SBOM は、製品のライセンス情報、サードパーティー製品の使用状況、およびその他の規制要件に関する情報を提供するため、規制遵守に役立ちます。
- 品質向上: SBOM は、ソフトウェア製品がどのように構成されているかを明確にするため、品質管理プロセスの改善に役立ちます。


SBOM の代表的なフォーマット
----

- __SPDX__: Software Package Data Exchange
  - 標準: ISO/IEC 5962:2021
  - サポート団体: The Linux Foundation SPDX Group
- __CycloneDX__: CycloneDX specification
  - サポート団体: OWASP Foundation
- __SWID__: Software Identification Tags
  - 標準: ISO/IEC 19770-2:2015


GitHub から SBOM を出力する
----

GitHub で管理しているプロジェクトであれば、GitHub の Web サイト上で SBOM ファイル（SPDX 形式）を出力することができます。

- [リポジトリのソフトウェア部品表のエクスポート - GitHub Docs](https://docs.github.com/ja/code-security/supply-chain-security/understanding-your-software-supply-chain/exporting-a-software-bill-of-materials-for-your-repository)
- [セルフサービスの SBOM が登場 (2023-03-28) - GitHubブログ](https://github.blog/jp/2023-04-06-introducing-self-service-sboms/)

