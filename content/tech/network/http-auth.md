---
title: "HTTP/1.1 の認証スキームに関するメモ"
url: "p/ufpqqno/"
date: "2022-05-01"
tags: ["認証", "HTTP"]
---

__認証スキーム__ とは、HTTP のリクエストヘッダに指定する Authentication ヘッダーの先頭部分に指定する文字列のこと。

- `Basic` ... [RFC2617](https://datatracker.ietf.org/doc/html/rfc2617)
  - Web ブラウザが標準でサポートしている。
- `Bearer` ... [RFC6750](https://datatracker.ietf.org/doc/html/rfc6750)
  - OAuth 2.0 で使われている認証スキームのひとつ。
  - 署名を行う必要はなく、受け取った値をそのまま使える。
- `Digest` ... [RFC2617](https://datatracker.ietf.org/doc/html/rfc2617)
  - Web ブラウザが標準でサポートしている。
- `Negotiate` ... [RFC4559](https://datatracker.ietf.org/doc/html/rfc4559)
- `OAuth` ... [RFC5849](https://datatracker.ietf.org/doc/html/rfc5849)
  - OAuth 1.0 で使われていた認証スキーム。
  - 署名のルールが複雑すぎて廃れた。

参考リンク

- [RFC7235 - Hypertext Transfer Protocol (HTTP/1.1): Authentication](https://datatracker.ietf.org/doc/html/rfc7235)
- [RFC7236 - Initial Hypertext Transfer Protocol (HTTP): Authentication Scheme Registrations](https://datatracker.ietf.org/doc/html/rfc7236)

