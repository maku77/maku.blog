---
title: "VS Code の Vim プラグインで OS のクリップボードと同期する (vim.useSystemClipboard)"
url: "/p/tb6s2ck"
date: "2020-03-16"
tags: ["Vim", "VS Code"]
description: "Visual Studio Code の Vim プラグインを使っているときに、OS のクリップボードと Vim のコピー＆ペーストを同期させる方法です。"
---

Vim エディタ使いにとって、Visual Studio Code の [Vim プラグイン](https://marketplace.visualstudio.com/items?itemName=vscodevim.vim) はほぼ必須の機能になっています。

- [Vim - Visual Studio Marketplace](https://marketplace.visualstudio.com/items?itemName=vscodevim.vim)

デフォルトでは、`y` キーによるヤンクバッファ（Vim 用語では unnamed register）のコピーが、OS のクリップボードと同期されません。
次のように Vim プラグインの設定を行っておく、OS のクリップボードと同期してくれるようになります。


settings.json で設定する方法
----

{{< code lang="js" title="settings.json" >}}
{
  "vim.useSystemClipboard": true, // OS のクリップボードと同期
  // ...
}
{{< /code >}}

- 参考: [settings.json について](/p/tfq2cnw)


設定画面で設定する方法
----

{{< image w="700" border="true" src="img-001.png" >}}

1. __`Cmd + ,`__（Windows では `Ctrl + ,`）で設定メニューを開く
2. __`Vim: Use System Clipboard`__ の項目にチェックを入れる

これで、アプリ間をまたいだコピー＆ペーストを行うことができるようになります。

