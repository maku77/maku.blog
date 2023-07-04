---
title: "Azure: DRAFT"
url: "p/er6bg2o/"
date: "2023-07-04"
tags: ["Azure"]
draft: true
---

Azure Web Apps のファイルシステム
----

- Web Apps をホストするときは、`D:/home/site/wwwroot` にコンテンツが保存されます。
  実体は FileServer インスタンスにマウントされた Azure Storage 上に保存されています。
- Web Apps 上で生成した大切なデータを永続化するには、「Azure Storage」や「SQL Database」などに保存しておく必要があります。

