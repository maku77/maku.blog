---
title: "Blender: ショートカットキーまとめ"
linkTitle: "ショートカットキーまとめ"
url: "/p/uq9w7yv"
date: "2019-10-15"
tags: ["Blender", "cheatsheet"]
draft: true
---

全般／ ウィンドウ操作
----

| ショートカットキー | 説明 | Python |
| ---- | ---- | ---- |
| <kbd>Ctrl/Cmd + ,</kbd> | 設定画面を開く (Show Preferences) | `screen.userpref_show` |
| <kbd>Ctrl/Cmd + PageUp/Down</kbd> | ワークスペースの切替 (Cycle Workspace) | `screen.workspace_cycle` |
| <kbd>Ctrl + Space</kbd> | エディタを最大化 | |
| <kbd>Ctrl + Alt + Space</kdb> | フルスクリーン表示 | |
| <kbd>T</kbd> | ツールバーの表示 | |
| <kbd>N</kbd> | サイドバーの表示 | |
| コーナーを <kbd>Ctrl</kbd> 押しながらドラッグ | エディタの領域をスワップ | |
| コーナーを <kbd>Shift</kbd> 押しながらドラッグ | エディタをウィンドウに分離 | |

- インタフェースを日本語化する
  - Edit → Interface → Translation
- 10キーをエミューレートする
  - Input → Keyboard → Emulate Numpad


3D ビューエディタ (3D View)
----

| ショートカットキー | 説明 | Python |
| ---- | ---- | ---- |
| <kbd>Ctrl + \`</kbd> | [ギズモの表示／非表示 (Toggle Gizmo)](/p/fdqc4on) | |
| <kbd>Ctrl + Tab</kbd> | モードの切り替え（パイメニュー） | |
| <kbd>F2</kdb> | アクティブなオブジェクトの名前を変更 (Rename active item) | |
| <kbd>F11</kdb> | Show render window | |
| <kbd>F12</kbd> | Render the current frame | |
| <kbd>Shift + 右クリック</kbd> | 3Dカーソルを移動 (Set 3D Cursor) | `view3d.cursor3d` |
| <kbd>Shift + Space</kbd> | ツールバーを表示 (Toolbar) | `wm.toolbar` |

- ワークスペースのウィンドウレイアウトはトップバーから切り替え可能


プロパティエディタ
----

| ショートカットキー | 説明 | Python |
| ---- | ---- | ---- |
| <kbd>Ctrl + Tab</kbd> /<br><kbd>Cmd + マウスホイール</kbd> | バーチカルタブの切り替え | |
| <kbd>A</kbd> | マウスカーソル下のパネルを開閉 | |
| <kbd>Ctrl + クリック</kbd> | マウスカーソル下のパネル以外をすべて閉じる | |


その他コラム
----

- Blender で __`//`__ というパス表現は __カレントディレクトリ__ を示す。

