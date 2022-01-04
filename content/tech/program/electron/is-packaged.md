---
title: "Electron アプリがパッケージングされた環境 (production) で動作しているか調べる"
url: "/p/5ku5dmt"
date: "2020-08-02"
tags: ["Electron"]
---

Electron の [app モジュールの isPackaged プロパティ](https://www.electronjs.org/docs/api/app#appispackaged-readonly) を参照すると、アプリが `electron-builder` などでパッケージングされた状態で動作しているかどうかを調べることができます。
一般的に、`app.isPackaged` が `true` となる場合、production モードで動作していると考えられます。

次の例では、アプリが開発中の場合のみ Chromium の DevTools を開くようにしています。

{{< code lang="typescript" >}}
// const win = new BrowserWindow(this.options);

// 開発中のみ起動時に DevTools を開く
if (!app.isPackaged) {
  win.webContents.openDevTools()
}
{{< /code >}}

