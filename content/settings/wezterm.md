---
title: "WezTerm を使いこなすためのメモ"
url: "p/adcz2mf/"
date: "2025-11-08"
tags: ["PC設定"]
---

[WezTerm](https://wezterm.org) は、クロスプラットフォームのターミナルエミュレーターです。
Windows でも macOS でも Linux でも使えるので、使いこなせるようになるととても便利です。
Neovim と同様に設定を Lua 言語で行うことができ、高度なカスタマイズが可能です。
慣れないうちは Lua でどのように設定すればよいか分からないことも多いので、ここにいろんな設定方法をメモしておきます。

- （参考）まく設定: https://github.com/maku77/dotfiles/tree/master/wezterm


操作
----

WezTerm のウィンドウ操作系のショートカットは、`Ctrl + Shift` の組み合わせが多いです。

- `Ctrl + Shift + P` ... コマンドパレットを開く（迷ったらコレ）
- タブ
  - `Ctrl + Shift + T` / `Cmd + T` ... タブを開く
  - `Ctrl + Shift + W` / `Cmd + W` ... タブを閉じる
  - `Ctrl (+ Shift) + Tab` / `Ctrl + Shift + PageUp/Down` ... 左右のタブへ移動
  - `Ctrl + Shift + 1-9` / `Cmd + 1-9` ... その番号のタブへ移動
  - `Ctrl + Shift + PageUp/Down` ... カレントタブの位置を左右に移動（Chrome も同じ）
- ペーン分割
  - `Ctrl + Shift + Alt + %` ... ペーンを左右に分割
  - `Ctrl + Shift + Alt + "` ... ペーンを上下に分割
  - `Ctrl + Shift + ↑↓←→` ... ペーンを移動
  - `Ctrl + Shift + Alt + ↑↓←→` ... ペーンのサイズを変更


設定（見た目）
----

### インアクティブなペーンの彩度や輝度を調整する

```lua
config.inactive_pane_hsb = {
  saturation = 0.9,  -- 彩度（デフォルトは 0.9）
  brightness = 0.4,  -- 輝度（デフォルトは 0.8）
}
```

ペーン分割したときに、アクティブなペーンを判別しにくいときは、インアクティブなペーンの彩度 (saturation) や輝度 (brightness) を下げるとよいです。
彩度を下げると、グレスケールに近づきます。
彩度よりも輝度を下げて調整するのがおすすめです。
一応 hue も設定できますが、色が変わるので気持ち悪いです。

### 背景の透過

```lua
config.window_background_opacity = 0.9  -- ウィンドウを透過させる (1.0で不透過）
config.macos_window_background_blur = 10  -- ウィンドウの背景をぼかす（macOSのみ）
```

背景は少しだけ透過させるとかっこいいです。

次の設定を入れると、Neovim などのエディタの背景色も完全に透過されますが、余計なところ（カーソル行ハイライトなど）も見えなくなってしまうのでこの設定は入れない方がよいです。

```lua
-- これは副作用が大きいので入れない
-- config.text_background_opacity = 0
```

Neovim の背景を透過させたいときは、Neovim の設定の方で highlight グループを指定するのがよいです。

{{< code lang="lua" title="Neovim の透過設定例 (init.lua)" >}}
vim.api.nvim_set_hl(0, "Normal", { bg = "none" })
{{< /code >}}


設定（応用）
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

### タブにカレントディレクトリ名を表示する

```lua
-- タブのタイトル表示用のキャッシュ（「pane_id → ディレクトリ名」のマップ）
g_tab_titles = {}

-- タブバーの右端にファイルパスを表示
wezterm.on("update-status", function(window, pane)
  local cwd_uri = pane:get_current_working_dir()
  if cwd_uri then
    local full_path = cwd_uri.file_path
    local dir_name = full_path:match("([^/]+)$") or full_path
    g_tab_titles[pane:pane_id()] = dir_name
  end
end)

wezterm.on("format-tab-title", function(tab, tabs, panes, config, hover, max_width)
  local prefix = "【" .. tab.tab_index + 1 .. "】"
  local title = g_tab_titles[tab.active_pane.pane_id] or tab.active_pane.title
  return prefix .. title
end)
```

これは簡単に実現できそうで意外と難しいです。
というのも、タブ名を設定するための `format-tab-title` イベントハンドラーは同期関数のみの呼び出しで素早く完了する必要があり、`pane:get_current_working_dir()` を呼び出すことができないからです。
そのため、`update-status` イベントハンドラーで定期的にカレントディレクトリ名を取得してキャッシュしておき、`format-tab-title` イベントハンドラーではそのキャッシュを参照するようにしています。

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

