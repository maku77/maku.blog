---
title: "VS Code のショートカットキーを使いこなす（ウィンドウ操作編）"
url: "/p/as4cmu4"
date: "2020-06-10"
tags: ["cheatsheet", "VS Code"]
description: "Visual Studio Code で快適・爆速な開発をするために、主要なショートカットキーを覚えましょう。"
weight: 200
---

ショートカットキーで `Cmd` になっているところは、Windows の場合は `Ctrl` に置き換えればほぼ動作します。
とりあえずは、サイドバーやターミナルパネルを On/Off する `Cmd + B` や `Cmd + J` から覚えて、画面を広く使えるようになりましょう。


アクティビティバー／サイドバーの操作
----

{{< image src="img-001.png" >}}

| ショートカット | 説明 |
| ---- | ---- |
| __`Cmd + B`__ | サイドバー全体の表示／非表示 |
| __`Cmd + Shift + E`__ | Explorer を開く |
| `Cmd + Shift + F(H)` | Search を開く（`H` なら置換モード） |
| `Ctrl + Shift + G` | Source Control を開く |
| `Cmd + Shift + D` | Debug を開く |
| `Cmd + Shift + X` | Extension を開く |


ターミナル／出力パネル
----

画面の下の方に表示されるターミナルなどのパネルを開くためのショートカットキーです。

| ショートカット | 説明 |
| ---- | ---- |
| __`Cmd + J`__ | パネル全体の表示／非表示 |
| __``Ctrl + ` ``__ | Terminal パネルを開く |
| `Cmd + Shift + Y` | Debug Console パネルを開く |
| `Cmd + Shift + M` | Problem パネルを開く |
| `Cmd + Shift + U` | Output パネルを開く |

外部コマンドを実行するときは、Terminal パネルを使用する代わりに、__`Cmd + Shift + C`__ で OS 標準のターミナルを開くこともできます（Windows なら `Ctrl + Shift + C` でコマンドプロンプトが開きます）。


フルスクリーン／禅モード
----

| ショートカット | 説明 |
| ---- | ---- |
| `Ctrl + Cmd + F` | フルスクリーン表示（終了方法も同じ） |
| `Cmd + K` → `Z` | 禅モード表示（終了は `Esc` x 2） |

禅モードに入ると、編集テキストのみの表示になり、集中して作業できるようになります。

禅モードに入るためのショートカットキーは覚えにくいので、`Cmd + Shift + P` でコマンドパレットを開き、`zen` → `Enter` で入る方が簡単かもしれません。
禅モードから通常表示に戻るときは `Esc` → `Esc` ですが、フルスクリーン化のショートカットキー `Ctrl + Cmd + F` でも戻れます。

