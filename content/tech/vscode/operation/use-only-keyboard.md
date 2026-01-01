---
title: "VS Code をキーボードだけで操作する"
url: "p/wds8njv/"
date: "2025-12-28"
tags: ["VS Code"]
---

<style>
.table-xg92ozp .key {
  font-weight: bolder;
  white-space: nowrap;
  color: darkblue;
}
</style>

Visual Studio Code をキーボード操作で効率的に使うためのショートカットキーのまとめです。
Vim/Neovim などのエディタに比べると、Visual Studio Code ではマウスを使いがちですが、慣れればほとんどの操作をキーボードだけで完結できます。
エディタ内での編集操作は Vim 拡張や Emacs キーバインドなど好みが分かれるため、ここでは主にペーン移動などの UI 操作に絞っています。


ファイル名で開く、シンボル名で開く
----

<table class="table-xg92ozp">
  <thead>
    <tr><th>キー操作</th><th>説明</th></tr>
  </thead>
  <tbody>
    <tr>
      <td class="key">Cmd + P<br>(Ctrl + P)</td>
      <td>ファイル名を検索して開く<br><code>workbench.action.quickOpenNavigateNextInFilePicker</code></td>
    </tr>
    <tr>
      <td class="key">Cmd + T<br>(Ctrl + T)</td>
      <td>シンボルを検索してジャンプ（ワークスペース内）<br><code>workbench.action.showAllSymbols</code><br><span class="key">Cmd + P</span> 後に <code><strong>#</strong></code> プレフィックスでも OK</td>
    </tr>
    <tr>
      <td class="key">Cmd + Shift + O<br>(Ctrl + Shift + O)</td>
      <td>シンボルを検索してジャンプ（ファイル内）<br><code>workbench.action.gotoSymbol</code><br><span class="key">Cmd + P</span> 後に <code><strong>@</strong></code> プレフィックスでも OK</td>
    </tr>
    <tr>
      <td class="key">Cmd + W<br>(Ctrl + W)</td>
      <td>ファイルを閉じる<br><code>workbench.action.closeActiveEditor</code></td>
    </tr>
  </tbody>
</table>


タブの切り替え
----

<table class="table-xg92ozp">
  <thead>
    <tr><th>キー操作</th><th>説明</th></tr>
  </thead>
  <tbody>
    <tr>
      <td class="key">Ctrl + Tab</td>
      <td>タブの一覧を表示して切り替え<br><code>workbench.action.quickOpenNavigatePreviousInEditorPicker</code></td>
    </tr>
    <tr>
      <td class="key">Cmd + Option + [<br>Cmd + Option + ]</td>
      <td>前のタブへ <code>workbench.action.previousEditor</code><br>次のタブへ <code>workbench.action.nextEditor</code></td>
    </tr>
  </tbody>
</table>


ペーンの移動、表示・非表示
----

<table class="table-xg92ozp">
  <thead>
    <tr><th>キー操作</th><th>説明</th></tr>
  </thead>
  <tbody>
    <tr>
      <td class="key">Cmd/Ctrl + 0</td>
      <td>サイドバーをフォーカス<br><code>workbench.action.focusSideBar</code></td>
    </tr>
    <tr>
      <td class="key">Cmd/Ctrl + 1<br>Cmd/Ctrl + 2<br>Cmd/Ctrl + 3</td>
      <td>1/2/3番目のエディタペーンをフォーカス<br><code>workbench.action.focusFirstEditorGroup</code><br><code>workbench.action.focusSecondEditorGroup</code><br><code>workbench.action.focusThirdEditorGroup</code></td>
    </tr>
    <tr>
      <td class="key">Cmd + Shift + I</td>
      <td>GitHub Copilot チャット（セカンダリサイドバー）をフォーカス<br><code>workbench.action.chat.openagent</code></td>
    </tr>
    <tr>
      <td class="key">Ctrl + `</td>
      <td>エディタ↔︎ターミナル間のフォーカス移動（同時に表示・非表示）<br><code>workbench.action.terminal.toggleTerminal</code></td>
    </tr>
    <tr>
      <td class="key">Cmd + Shift + E</td>
      <td>エディタ↔︎エクスプローラー間のフォーカス移動<br><code>workbench.view.explorer</code></td>
    </tr>
    <tr>
      <td class="key">Cmd + B</td>
      <td>プライマリサイドバーの表示・非表示（フォーカスは移動しない）<br><code>workbench.action.toggleSidebarVisibility</code></td>
    </tr>
    <tr>
      <td class="key">Cmd + Opt + B</td>
      <td>セカンダリサイドバーの表示・非表示（フォーカスは移動しない）<br><code>workbench.action.toggleAuxiliaryBar</code></td>
    </tr>
    <tr>
      <td class="key">Cmd/Ctrl + \</td>
      <td>エディタ／ターミナルの分割<br><code>workbench.action.splitEditor</code><br><code>workbench.action.terminal.split</code></td>
    </tr>
    <tr>
      <td class="key">Cmd + Ctrl + 右<br>Cmd + Ctrl + 左</td>
      <td>現在のエディタを右/左グループに移動（ついでに分割／統合可能）<br><code>workbench.action.moveEditorToNextGroup</code><br><code>workbench.action.moveEditorToPreviousGroup</code></td>
    </tr>
  </tbody>
</table>


カーソル下の関数の定義へジャンプ
----

カーソル位置にある関数や変数の定義にジャンプする場合は、**`F12`** キーだけ覚えておけばなんとかなります。
個人的には [Vim 拡張](https://marketplace.visualstudio.com/items?itemName=vscodevim.vim) を入れているので、**`gd`** (<b>g</b>o to <b>d</b>efinition) をよく使います。

<table class="table-xg92ozp">
  <thead>
    <tr><th>キー操作</th><th>Vim拡張</th><th>説明</th></tr>
  </thead>
  <tbody>
    <tr>
      <td class="key">F12</td>
      <td class="key">gd</td>
      <td>カーソル下のシンボルの定義／参照へジャンプ</td>
    </tr>
    <tr>
      <td class="key">Ctrl + -</td>
      <td class="key">Ctrl + O</td>
      <td>ジャンプ位置を 1 つ戻る</td>
    </tr>
    <tr>
      <td class="key">Ctrl + Shift + -</td>
      <td class="key">Ctrl + I</td>
      <td>ジャンプ位置を 1 つ進む</td>
    </tr>
    <tr>
      <td class="key">（該当なし）</td>
      <td class="key">*</td>
      <td>カーソル下の単語をファイル内検索</td>
  </tbody>
</table>


さらなる高みへ ...... 独自のキーバインド
----

デフォルトのキーバインドに慣れてきたら、さらに自分好みにカスタマイズするのもオススメです。
次のようにしてキーバインド用の設定ファイル (**`keybindings.json`**) を開くことができます。

1. `Cmd/Ctrl + Shift + P` でコマンドパレットを開く
1. `基本設定: キーボード ショートカットを開く (JSON)` (`Preferences: Open Keyboard Shortcuts (JSON)`) を選択する

下記は設定の一例です。

{{< code lang="js" title="keybindings.json の例" >}}
[
  // エクスプローラー内で新しいファイルを作成
  {
    "key": "cmd+n",
    "command": "explorer.newFile",
    "when": "explorerViewletVisible && filesExplorerFocus"
  },
  // エクスプローラー上で新しいフォルダーを作成
  {
    "key": "cmd+shift+n",
    "command": "explorer.newFolder",
    "when": "explorerViewletVisible && filesExplorerFocus"
  },
  // タブを左に移動（Chrome のショートカットと合わせる）
  {
    "key": "ctrl+shift+pageup",
    "command": "workbench.action.moveEditorLeftInGroup"
  },
  // タブを右に移動（Chrome のショートカットと合わせる）
  {
    "key": "ctrl+shift+pagedown",
    "command": "workbench.action.moveEditorRightInGroup"
  }
]
{{< /code >}}

このファイルを直接編集してもよいですし、**`Cmd + K` → `Cmd + S`** で開くキーバインド一覧画面からも設定可能です。

UI 上の特定のボタンを右クリックして「**キーバインドの構成**」を選択すると、そのボタン用のキーバインドを確認＆設定できます。
マウス操作で頻繁に押しているボタンがあれば、ショートカットキーを割り当てておくと便利です。

