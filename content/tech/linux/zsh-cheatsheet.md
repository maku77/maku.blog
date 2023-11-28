---
title: "zsh チートシート"
url: "p/vhbe8f8/"
date: "2023-11-23"
tags: ["cheatsheet", "linux"]
---

| 種類 | キー操作 | 説明 |
| ---- | ---- | ---- |
| 移動 | `Ctrl + B` | カーソルを左へ移動 |
| 移動 | `Ctrl + F` | カーソルを右へ移動 |
| 移動 | `Ctrl + A` | カーソルを行頭へ移動 (beggining-of-line) |
| 移動 | `Ctrl + E` | カーソルを行末へ移動 (end-of-line) |
| 移動 | `Alt + ←` / `Esc, B` | カーソルを前の単語へ移動 (backward-word) |
| 移動 | `Alt + →` / `Esc, F` | カーソルを後の単語へ移動 (forward-word) |
| 削除 | `Ctrl + U` | 行全体を削除 (kill-hole-line) |
| 削除 | `Ctrl + K` | カーソル位置から行末までを削除 (kill-line) |
| 削除 | `Ctrl + H` | カーソルの前の 1 文字を削除 (Backspace) |
| 削除 | `Ctrl + D` | カーソル位置の 1 文字を削除 (Del) |
| 削除 | `Ctrl + W` | カーソル位置から単語の先頭までを削除 (backward-kill-word) |
| 削除 | `Esc, D` | カーソル位置から単語の末尾までを削除 (kill-word) |
| 編集 | `Ctrl + Y` | 貼り付け |
| 編集 | `Ctrl + T` | 前の文字と 1 文字入れ替え (transpose-chars) |
| 編集 | `Esc, U` | カーソル位置から単語末尾までを大文字にして、カーソルを単語末尾へ移動 (up-case-word) |
| 編集 | `Esc, L` | カーソル位置から単語末尾までを小文字にして、カーソルを単語末尾へ移動 (down-case-word) |
| 編集 | `Esc, C` | カーソル位置の 1 文字を大文字にして、カーソルを単語末尾へ移動 (capitalize-word) |
| 編集 | `Esc, T` | 前の単語と 1 単語入れ替え (transpose-words) |
| 履歴 | `Ctrl + P` | 1 つ前のコマンド履歴を表示 (up-line-or-history) |
| 履歴 | `Ctrl + N` | 1 つ後のコマンド履歴を表示 (down-line-or-history) |
| 履歴 | `Ctrl + R` | コマンド履歴のインクリメンタルサーチ (history-incremental-search-backward) |
| 履歴 | `Ctrl + O` | コマンド履歴からコマンドを実行して次の履歴を表示 (accept-line-and-down-history) |
| その他 | `Ctrl + J` / `Ctrl + M` | Enter (accept-line) |
| その他 | `Ctrl + I` / `Tab` | 入力補完 |
| その他 | `Ctrl + G` | 入力を破棄して新しい入力を開始する。インクリメンタルサーチを終了するのにも使える (send-break) |
| その他 | `Ctrl + L` | 端末（ターミナル）のクリア |
| その他 | `Ctrl + X, U` | Undo |

- メモ
  - `Alt + ←/→` で単語単位で移動して、`Ctrl + W` で単語先頭までを削除すれば、任意の位置の単語を削除できます。
  - `Esc` の代わりに __`Ctrl + [`__ を使うことで、ホームポジションを崩さずに操作できます。
  - `Ctrl + D` はカーソル位置の文字を削除するけど、何も入力されていない状態で入力するとシェルを閉じるので注意。

