---
title: "(DRAFT) SAML まわりの動きの図"
url: "p/jsrdeie/"
date: "2023-07-19"
tags: ["saml", "security"]
draft: true
---

{{< mermaid >}}
flowchart LR
    app[アプリ] -->|"(1) サインイン"| sso(SSOサービス)
    sso -->|"(2) SAML要求"| idp["ユーザープール (Idp)"]
    idp -->|"(3) ログイン要求"| app
    idp -->|"(4) SAML応答（アサーション）"| sso
    sso -->|"(5) SAML アサーション"| app
{{< /mermaid >}}

