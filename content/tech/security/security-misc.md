---
title: "セキュリティ雑多メモ"
url: "p/m8688y4/"
date: "2023-03-28"
tags: ["セキュリティ"]
draft: true
---

セキュリティマネジメントには「既知」と「未知」のものがある
----

既知の脆弱性 (known vulnerabilities)
: 開発時、出荷時などにすでに発見されている脆弱性。管理は容易。

未知の脆弱性 (unknown vulnerabilities)
: __定期的なチェックの仕組み__ を導入して発見する必要がある。


脆弱性データベース
----

[NVD: National Vulnerability Database](https://nvd.nist.gov/)
: NVD で管理されている脆弱性には、一意の __CVE: Common Vulnerabilities and Exposures__ という ID が割り当てられています。
  これにより、脆弱性の報告、追跡、共有が簡単になります。
  また、脆弱性の深刻度を評価するための、__CVSS: Common Vulnerability Scoring System__ 枠組みが定義されており、脆弱性の影響、機密性、可用性、完全性、およびその他の要因を評価して、それぞれの脆弱性に 0 ～ 10 の CVSS スコアが割り当てられています。
  CVSS スコアにより、組織は脆弱性を優先順位付けして対処することができます。

[GitHub Advisory Database](https://github.com/advisories/)
: GitHub が管理している脆弱性データベースです。
  いろいろなデータベースを横断的に検索できるので便利です。

  - 参考: [GitHub Advisory Database について](https://docs.github.com/ja/code-security/security-advisories/global-security-advisories/about-the-github-advisory-database)

[JVN: Japan Vulnerability Notes](https://jvn.jp/)


SBOM とは
----

SBOM とは、ソフトウェア製品の部品表（Software Bill of Materials）の略称で、ソフトウェア製品が構成されている部品のリストを指します。
SBOM は、ソフトウェア製品がどのように構成されているかを明確にするために使用されます。
__2021 年 5 月の米国大統領令に SBOM の作成と脆弱性対策を義務付ける条項__ が盛り込まれました。

### SBOM の目的

- セキュリティ向上（セキュリティ・マネージメント）: SBOM を利用することで、ソフトウェア製品がどのような部品から構成されているかを正確に把握することができ、悪意のある部品が含まれているかどうかを特定することができます。
- 規制遵守（コンプライアンス・マネージメント）: SBOM は、製品のライセンス情報、サードパーティー製品の使用状況、およびその他の規制要件に関する情報を提供するため、規制遵守に役立ちます。
- 品質向上: SBOM は、ソフトウェア製品がどのように構成されているかを明確にするため、品質管理プロセスの改善に役立ちます。

### SBOM の代表的なフォーマット

- __SPDX__: Software Package Data Exchange
  - 標準: ISO/IEC 5962:2021
  - サポート団体: The Linux Foundation SPDX Group
- __CycloneDX__: CycloneDX specification
  - サポート団体: OWASP Foundation
- __SWID__: Software Identification Tags
  - 標準: ISO/IEC 19770-2:2015

