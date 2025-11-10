---
title: "WezTerm を使いこなすためのメモ"
url: "p/adcz2mf/"
date: "2025-11-08"
tags: ["PC設定"]
draft: true
---

[WezTerm](https://wezterm.org) は、クロスプラットフォームのターミナルエミュレーターです。
Windows でも macOS でも Linux でも使えるので、使いこなせるようになるととても便利です。
NeoVim と同様に設定を Lua 言語で行うことができ、高度なカスタマイズが可能です。
不慣れなうちは Lua でどのように設定すればよいか分からないことも多いので、ここにいろんな設定方法をメモしておきます。

- （参考）まく設定: https://github.com/maku77/dotfiles/tree/master/wezterm


操作
----

- `Ctrl + Shift + P` ... コマンドパレットを開く（迷ったらコレ）
- タブ
  - `Ctrl + Shift + T` / `Cmd + T` ... タブを開く
  - `Ctrl + Shift + W` / `Cmd + W` ... タブを閉じる
  - `Ctrl + Tab` / `Ctrl + Shift + Tab` ... 左右のタブへ移動
  - `Ctrl + Shift + 1-9` / `Cmd + 1-9` ... その番号のタブへ移動
  - `Ctrl + Shift + PageUp/Down` ... カレントタブの位置を左右に移動
- ペーン
  - `Ctrl + Shift + Alt + %` ... ペーンを左右に分割
  - `Ctrl + Shift + Alt + "` ... ペーンを上下に分割
  - `Ctrl + Shift + ↑↓←→` ... ペーンを移動
  - `Ctrl + Shift + Alt + ↑↓←→` ... ペーンのサイズを変更


設定
----

### Windows かどうかの判断

```lua
local is_windows = wezterm.target_triple:find("windows") ~= nil

if is_windows then
  -- Windows 固有の設定
else
  -- その他の OS 固有の設定
end
```

### 設定ファイルをモジュール化する

{{< code lang="lua" title="~/.config/wezterm/wezterm.lua（メインファイル）" >}}
local wezterm = require "wezterm"
local config = wezterm.config_builder()

require("core").setup(wezterm, config)  -- core.lua を読み込み
require("keys").setup(wezterm, config)  -- keys.lua を読み込み
require("windows").setup(wezterm, config)  -- windows.lua を読み込み
{{< /code >}}

{{< code lang="lua" title="~/.config/wezterm/keys.lua（読み込まれるファイル）" >}}
local M = {}

function M.setup(wezterm, config)
  -- Claude Code での複数行入力のため、Shift + Enter で改行シーケンスを送信
  config.keys = {
    {key="Enter", mods="SHIFT", action=wezterm.action{SendString="\x1b\r"}},
  }
end

return M
{{< /code >}}

Neovim の設定ファイルを分割する手法とまったく同じです。
各モジュールからテーブル（上記例では `M` オブジェクト）を `return` し、メインファイルから `require()` で読み込むだけです。

### 別タブで何らかの出力があったことを知る

```lua
wezterm.on('format-tab-title', function(tab, tabs, panes, config, hover, max_width)
  local has_unseen_output = false
  for _, pane in ipairs(tab.panes) do
    if pane.has_unseen_output then
      has_unseen_output = true
      break
    end
  end
  local title = tab.active_pane.title
  if has_unseen_output then
    title = title .. " 💡"
  end
  return title
end)
```

別のタブで何らかのバックグラウンド処理を実行していて、その処理結果が出力されたときにタブに電球アイコン 💡 を表示するようにしています。
WezTerm のタブの表示内容をカスタマイズするには、[`format-tab-title` イベント](https://wezterm.org/config/lua/window-events/format-tab-title.html) をハンドルします。

### タブバーの右端に現在時刻を表示する

```lua
wezterm.on('update-status', function(window, pane)
  local time = wezterm.strftime("%H:%M:%S")
  window:set_right_status("🕒" .. time)
end)
```

WezTerm 内で 1 秒おきに [`update-status` イベント](https://wezterm.org/config/lua/window-events/update-status.html) が呼び出されるので、このイベントハンドラーの中でタブバーの表示を更新しています。
[`set_right_status()`](https://wezterm.org/config/lua/window/set_right_status.html) で、タブバーの右端に任意のテキストを表示することができます。

WezTerm の設定楽しいなぁ ٩(๑❛ᴗ❛๑)۶

