---
title: "暗号技術まとめ"
url: "/p/6rn67mc"
date: "2005-12-20"
tags: ["セキュリティ", "技術"]
---

昔の暗号
----

* **シーザー暗号** - 文字をずらすだけの最も古い暗号。ジュリアス・シーザー（ユリウス・カエサル）が作った。


共通鍵暗号
----

### ブロック暗号

ブロック暗号というのは、共通鍵暗号の一種。
データを一定のブロックごとに暗号化する方式。
高速な暗号化処理を行える。

* **Triple DES** - 1990年代。64 bit のブロック暗号 DES を 3 回繰り返す暗号化アルゴリズム。かつては米国政府標準暗号だった。
* **MISTY1** - 1990年代。三菱電機が開発した 128 bit 暗号鍵を持つ 64 bit ブロック暗号アルゴリズム。
* **CAST-128** - 64 bit ブロック暗号。
* **Camellia** - 2000年代。128 bit ブロック暗号。NTT・三菱電機。
* **AES: Advanced Encryption Standard** - 2000年代。128 bit ブロック暗号。
* **SEED** - 128 bit ブロック暗号。

国際標準のブロック暗号 (**ISO/IEC18033-3**) は、Camellia、AES、SEED、CAST-128、MISTY1、Triple DES。

### ストリーム暗号

* **MUGI**
* **MULTI-S01**
* **SNOW**

国際標準のストリーム暗号 (**ISO/IEC18033-4**) は、MUGI、MULTI-S01、SNOW。


公開鍵暗号
----

### RSA

* **RSA: Rivest - Shamir - Adleman** - 大きな素数の素因数分解が難しいという根拠に基づいた暗号方式。

### 楕円曲線暗号

楕円曲線状での離散対数問題に基づいた公開鍵暗号方式。

* **PSEC-KEM: Provably Secure Elliptic Curve encryption - Key Encapsulation Mechanisms** - NTT。
* **ECDSA: Elliptic Curve Digital Signature Algorithm**

国際標準の公開鍵暗号 (**ISO/IEC18033-2**) は、ACE-KEM, ECIES-KEM, PSEC-KEM, RSA-KEM, RSA-OAEP, HIME(R)。


参考: NTT 技術ジャーナル 2005.12


暗号アルゴリズムの使用期限
----

{{< image src="crypto-nist-expiration.jpg" title="NISTが提示する暗号アルゴリズムの使用期限 『日経ビジネス』2018.07.23より" >}}

米国の国立標準技術研究所 (NIST) が提示する鍵の長さとその使用期限は、世界最高レベルのスーパーコンピューターを使っても解読に何十年もかかるかどうかが基準になっている。

