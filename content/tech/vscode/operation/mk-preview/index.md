---
title: "VS Code で Markdown ファイルのプレビューを表示する"
url: "/p/ornz2yx"
date: "2020-03-16"
tags: ["VS Code"]
image: "img-001.png"
description: "Visual Studio Code は、デフォルトで Markdown ファイル (.md) をプレビューする機能を搭載しています。ブログ記事やドキュメントサイトを Markdown で記述している場合にオススメです。"
weight: 100
---

{{< image border="true" src="img-001.png" title="VSCode の Markdown プレビュー表示" >}}

Visual Studio Code で Markdown ファイルのプレビュー表示を行うには、`F1` キー（あるいは `Ctrl + Shift + P`) でコマンドパレットを開き、`Markdown` メニューから `Open Preview` を選択します。

次のように、新しいタブで開くか、左右に分割して表示するかを選択できます。

- プレビューを新しいタブで表示: **`Markdown: Open Preview`**
- プレビューを左右分割して表示: **`Markdown: Open Preview to the Side`**

Markdown ファイルを編集中に右上に表示される「プレビューボタン」を押すことでも、プレビューを表示することができます（こっちのが早いかも）。

{{< image border="true" src="img-002.png" title="VSCode の分割プレビューボタン" >}}

1 文字入力するごとにリアルタイムにプレビュー更新してくれるので気持ちよく作業できます。
フロントマターを解析してプレビュー表示してくれないなどの欠点はありますが、きっと近いうちに拡張されて表示できるようになると思います。

