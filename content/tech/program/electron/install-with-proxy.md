---
title: "プロキシ環境から Electron をインストールできない場合の対処方法"
url: "/p/t7gqxyf"
date: "2020-06-24"
tags: ["Electron"]
weight: 900
---

問題
----

プロキシ環境下で、Electron (9.0.5) をインストールしようとしたら、まぁいろいろとエラーになってハマりました。
どうも、`node install.js` の実行がうまくいっていないようです。

{{< code >}}
D:\sandbox> npm install electron --save-dev

> core-js@3.6.5 postinstall D:\sandbox\node_modules\core-js
> node -e "try{require('./postinstall')}catch(e){}"

> electron@9.0.5 postinstall D:\sandbox\node_modules\electron
> node install.js

RequestError: read ECONNRESET
    at ClientRequest.<anonymous> (D:\sandbox\node_modules\got\source\request-as-event-emitter.js:178:14)
    at Object.onceWrapper (events.js:422:26)
    at ClientRequest.emit (events.js:327:22)
    at ClientRequest.origin.emit (D:\sandbox\node_modules\@szmarczak\http-timer\source\index.js:37:11)
    at TLSSocket.socketErrorListener (_http_client.js:467:9)
    ...
{{< /code >}}

`ECONNRESET` ってことはプロキシの設定かなぁと思ったけど、プロキシは `npm config set proxy ...` でちゃんとセットしているし、なんだろう。。。


解決方法
----

と調べていたら、下記の Electron のチュートリアルに書いてありました。

- [Installation - Proxies | Electron](https://www.electronjs.org/docs/tutorial/installation#proxies)

> If you need to use an HTTP proxy, you need to set the ELECTRON_GET_USE_PROXY variable to any value, plus additional environment variables depending on your host system's Node version.

なんと、 __`ELECTRON_GET_USE_PROXY`__ などをセットしないといけないとのこと。
次のような感じでプロキシ設定したら、うまくインストールできました。

{{< code title="Windows の場合" >}}
$ set ELECTRON_GET_USE_PROXY=1
$ set GLOBAL_AGENT_HTTPS_PROXY=http://proxy.example.com:8080
$ npm install electron --save-dev
{{< /code >}}

わかりにくい。。。

（おまけ）electron-builder の場合
----

ちなみに、Electron アプリの配布パッケージを作成するための `electron-builder` は、プロキシ設定として `HTTPS_PROXY` 環境変数を参照するようです。
プロキシ環境から実行するときは、これをセットして実行しないと必要なモジュールをダウンロードできずにエラーになります。

{{< code title="Windows の場合" >}}
$ set HTTPS_PROXY=http://proxy.example.com:8080
$ npx electron-builder --win --x64 --dir
{{< /code >}}

