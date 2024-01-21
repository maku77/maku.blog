---
title: "(DRAFT) JWT 関連メモ"
url: "p/xx73pk7/"
date: "2023-07-19"
tags: ["jwt", "security"]
draft: true
---

- __`hose`__ ... Deno の JWT 検証ライブラリ
- JWT の __iss__ クレーム（発行者）の例
  - Amazon Cognito の場合
    - `https://cognito-idp.us-east-1.amazonaws.com/<userPoolId>`
  - Firebase Authentication の場合
    - `https://securetoken.google.com/<projectId>`

