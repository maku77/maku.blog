---
title: "チャットボット (1-1) Bot Builder SDK とは"
url: "/p/tzaeb9x"
linkTitle: "Bot Builder SDK とは"
date: "2019-03-04"
tags: ["Azure", "Chatbot", "Bot Builder SDK"]
weight: 211
---

Microsoft の Azure は Chatbot サービスを作成する機能を提供しています。
Microsoft が提供している Bot Framework は、この Chatbot サービスを作成するとき、あるいは Chatbot を使用するクライアントを作成するときに使用するツール群（あるいは仕組み）やそれらを取り巻く環境の総称です。

{{< image w="500" border="true" src="about.png" >}}

- [Microsoft Bot Framework](https://dev.botframework.com/)

ボットのサーバを実装するためのライブラリは、**Bot Builder SDK** という名前で提供されています。
現状では、ボットは Node.js と .NET による開発が想定されているため、Bot Builder SDK も Node.js と .NET 用のものが提供されています（2019年3月現在、Python と Java 版が preview リリースされているようです）。

- [Bot Builder SDK (for Node.js)](https://github.com/microsoft/botbuilder-js)
- [Bot Builder SDK (for .Net)](https://github.com/Microsoft/botbuilder-dotnet)
- [Bot Builder サンプルコード集](https://github.com/microsoft/botbuilder-samples)

Node.js と .NET のどちらを使って開発するかに迷ったら、非同期処理を前提にして設計されている Node.js 版を選択するのがよいでしょう。

Node.js の Bot Builder SDK は、NPM パッケージとして公開されているため、`npm` コマンドを使って簡単にインストールすることができます。

```
$ mkdir mybot
$ cd mybot
$ npm init -y
$ npm install --save botbuilder@4.x
```

Bot Builder SDK はメジャーバージョン間で API の互換性がまったくないので、上記のようにメジャーバージョンを指定してインストールすることをお勧めします。
インターネットに公開されているサンプルコードは、SDK V3 用のものと V4 用のものが混在しているため、どの SDK バージョンを使ったコードなのかを意識しておく必要があります。

