---
title: "Electron アプリで OS のプロキシ設定をメインプロセスに反映する"
url: "/p/t7ekrx4"
date: "2020-08-03"
tags: ["Electron"]
---

Electron アプリは「メインプロセス」と「レンダラープロセス」の 2 種類を扱わないといけないので、プロキシの設定でハマりがちです。
ここでは、システム (OS) のプロキシ設定を、自動的に両プロセスに反映する方法を説明します。
この仕組みを入れておけば、ユーザーにわざわざプロキシの設定をしてもらわなくて済むようになります。


レンダラープロセスの HTTP 通信
----

レンダラープロセス側は、Chromium の仕組みで __システムのプロキシ設定が自動的に反映される__ ようになっています。
例えば、Web ブラウザ上の JavaScript で使用する `XMLHttpRequest` 関数や `fetch` 関数による HTTP 通信は、自動的にプロキシ経由で実行されます。
この振る舞いで問題なければ、特に設定を行う必要はありません。

`BrowserWindow` で扱うプロキシを明示的に指定するには、[session オブジェクトの setProxy メソッド](https://www.electronjs.org/docs/api/session#sessetproxyconfig) を使用します。


メインプロセスの HTTP 通信
----

Electron のメインプロセスはいわゆる Node.js アプリケーションと同様の仕組みで動作するので、Web ブラウザ側のプロキシ設定が自動的に反映されるようなことはありません。
しかし、`electron` モジュールが提供する `session` オブジェクトには、Chromium 側のプロキシ設定を参照する機能が備わっています。
この情報を利用すれば、メインプロセス側のプロキシ設定も自動的に行うことができます。

下記の `autoProxy()` 関数をメインプロセス起動直後に呼び出しておけば、メインプロセス側で実行する HTTP 通信に対してシステムのプロキシ設定が反映されます。

{{< code lang="typescript" lang="autoProxy.ts" >}}
import {app, session} from 'electron';
import {bootstrap} from 'global-agent';

/**
 * システムのプロキシ設定情報をメインプロセスの HTTP 通信に反映します。
 */
export function autoProxy() {
  app.once('ready', () => {
    // このアドレスは有名どころであれば何でもよい
    const TARGET = 'https://www.google.com';

    session.defaultSession.resolveProxy(TARGET).then((info: string) => {
      // システムにプロキシが設定されていない場合はなにもしない
      if (info === 'DIRECT') {
        return;
      }

      // 'PROXY proxy.example.com:8080' のような文字列から URL 部分を抽出
      const url = info.replace(/^PROXY /, '');
      setGlobalAgent(url);
    });
  });

  function setGlobalAgent(proxy: string) {
    //console.log(`Proxy settings detected: ${proxy}`);
    if (!proxy.startsWith('http')) {
      proxy = 'http://' + proxy;
    }

    // デフォルトの HTTP 通信をプロキシ経由にする
    process.env.GLOBAL_AGENT_HTTP_PROXY = proxy;
    bootstrap();
  }
}
{{< /code >}}

処理の流れは次の通りです。

1. `app.once('ready')` になってから `session.defaultSession` で `session` オブジェクトを取得する。
2. `session.resolveProxy()` でシステムのプロキシ情報を取得する。
   プロキシ情報は文字列で返され、プロキシ設定されているときは `PROXY <address:port>`、プロキシ設定されていないときは `DIRECT` になる。
3. システムにプロキシが設定されている場合は、その値を `global-agent` モジュールの `bootstrap()` でメインプロセス側の HTTP 通信に反映する。

