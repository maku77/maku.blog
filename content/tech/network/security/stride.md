---
title: "セキュリティ雑多メモ: STRIDE による脅威分類"
url: "p/p7ngweu/"
date: "2023-10-08"
tags: ["security"]
---

__STRIDE__ は Microsoft が提唱した脅威分類で、情報セキュリティにおける主要な攻撃を示すための頭字語です。

S: Spoofing of Identity（アイデンティティの偽装）
: 他のユーザーや何らかのエンティティになりすましてシステムにアクセスすること。

T: Tampering with Data（データの改ざん）
: データやコードを不正に変更することによって、そのシステムやデータの信頼性や正確性を損ねること。

R: Repudiation（否認）
: 実行されたアクションやトランザクションを実行していないと主張し、責任を免れようとすること。

I: Information Disclosure（情報漏洩）
: 機密情報を不正な手段で漏らし、権限のない者がアクセスできるようにすること。

D: Denial of Service（サービス拒否）
: 合法的なユーザーの利用を妨げるため、サービスの提供を妨害すること。

E: Elevation of Privilege（特権昇格）
: 正当な権限を持たない者が権限を取得することでシステムを不正に操作すること。

- 参考: [Threats - Microsoft Threat Modeling Tool - Azure | Microsoft Learn](https://learn.microsoft.com/en-us/azure/security/develop/threat-modeling-tool-threats)

