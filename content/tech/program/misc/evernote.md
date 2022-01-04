---
title: "Node.js で Evernote API を使用する（evernote モジュールインストールする）"
url: "/p/ti537g4"
date: "2013-11-11"
tags: ["Evernote", "Node.js"]
---

Evernote API を使用するには、API キーの取得と、Sandbox 用アカウントの作成を行っておく必要があります。

- API キーの取得: http://dev.evernote.com/#apikey
- Sandbox 用アカウントの作成: https://sandbox.evernote.com

Node.js から Evernote API を使用するために、`evernote` モジュールをインストールします。

```
$ npm install evernote
```

下記のようにして、`evernote` モジュールをロードできれば準備完了です。

{{< code lang="js" >}}
var Evernote = require('evernote').Evernote;
console.log(Evernote.EDAM_VERSION_MAJOR);
console.log(Evernote.EDAM_VERSION_MINOR);
{{< /code >}}

