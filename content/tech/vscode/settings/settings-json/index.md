---
title: "VS Code の設定ファイルの場所 (settings.json)"
url: "/p/tfq2cnw"
date: "2021-07-11"
tags: ["VS Code"]
weight: 100
---

ユーザー設定とワークスペース設定
----

VS Code の設定画面は `Ctrl(Cmd) + ,` で起動できますが、ここで行った設定は、__`settings.json`__ というユーザー設定ファイル（あるいはワークスペース設定ファイル）に保存されます。
このファイルの内容と設定画面の内容は連動しているので、設定はどちらで行ってもいいのですが、慣れてくると `setttings.json` を直接編集した方がすばやく設定を行えます。

### ユーザー設定ファイル

VS Code のユーザー設定ファイル (`settings.json`) は、OS ごとに下記のディレクトリに保存され、全ての VS Code インスタンス（ワークスペース）に共通の設定として使用されます。

| OS | ユーザー設定ファイルのパス |
| ---- | ---- |
| Windows | __`%APPDATA%\Code\User\settings.json`__ |
| macOS | __`$HOME/Library/Application Support/Code/User/settings.json`__ |
| Linux | __`$HOME/.config/Code/User/settings.json`__ |

### ワークスペース設定ファイル

ワークスペースごとの設定は、プロジェクトのルートディレクトリの `.vscode` ディレクトリに保存されます。

- __`<プロジェクトルート>/.vscode/settings.json`__

ワークスペース設定は、前述のユーザー設定よりも優先されます（同じ設定項目の値が上書きされます）。
`.vscode` ディレクトリを Git にコミットしておけば、チーム内で VS Code の設定を共有できます。
`.vscode` ディレクトリには、プロジェクトのタスク設定ファイル (`tasks.json`) なども格納されます。


設定ファイルのサンプル
----

`settings.json` ファイルの拡張子は `.json` ですが、JavaScript 形式のコメントを記述できるようになっています (JSON with Comments)。

{{< code lang="js" title="settings.json の例" >}}
// Place your settings in this file to overwrite the default settings
{
  "breadcrumbs.enabled": false,
  "editor.fontSize": 14,
  "editor.minimap.enabled": false,
  "editor.renderControlCharacters": true,
  "editor.tabSize": 2,
  "files.trimTrailingWhitespace": true,
  "terminal.integrated.tabs.enabled": true,
  "typescript.updateImportsOnFileMove.enabled": "always",
  "vim.useSystemClipboard": true,
  "workbench.startupEditor": "newUntitledFile"
}
{{< /code >}}


settings.json を素早く開く
----

VS Code の設定画面 (`Ctrl/Cmd + P`) の右上の {{< image-inline w="32" src="img-open-json-icon.png" >}} アイコンをクリックすると、`settings.json` ファイルを簡単に開くことができます。
設定画面のタブで「ユーザー設定」と「ワークスペース設定」を切り替えられるようになっているので、開きたい `settings.json` ファイルのタブを選択してからアイコンをクリックしてください。

{{< image border="true" w="600" src="img-001.png" >}}


デフォルトの設定 (defaultSettings.json)
----

`settings.json` でユーザー設定（or ワークスペース設定）を行わない場合は、VS Code はデフォルトの設定を使用します。
デフォルトの設定は、`defaultSettings.json` に記述されており、次のようにして開いて確認することができます。

- コマンドパレット (`Ctrl/Cmd + Shift + P`) → __`Preferences: Open Default Settings (JSON)`__

このファイルは 5000 行近くありますが、次のようにコメント付きで表示されるので、ざっと眺めてみると何か発見があるかもしれません。

{{< code lang="js" title="defaultSettings.json（抜粋）" >}}
{
  // Controls whether the editor shows CodeLens.
  "diffEditor.codeLens": false,

  // When enabled, the diff editor ignores changes in leading or trailing whitespace.
  "diffEditor.ignoreTrimWhitespace": true,
  // ...
}
{{< /code >}}

このデフォルト設定値は、下記の公式ドキュメントでも参照できます。

- [Visual Studio Code - Default settings](https://code.visualstudio.com/docs/getstarted/settings#_default-settings)


プログラミング言語別の設定 (per-language configuration) {#per-language}
----

ある言語タイプのファイルを開いているときのみ有効にしたい設定は、`setting.json` の中で、__`[ファイルタイプ]`__ というプロパティに設定を記述します。
下記の例では、TypeScript ファイル (`.ts`) と Markdown ファイル (`.md`) に対する設定を行っています。

{{< code lang="js" title="settings.json" >}}
{
  // ...
  "[typescript]": {
    "editor.formatOnSave": true,
    "editor.formatOnPaste": true
  },
  "[markdown]": {
    "editor.formatOnSave": true,
    "editor.wordWrap": "on",
    "editor.renderWhitespace": "all",
    "editor.acceptSuggestionOnEnter": "off"
  }
}
{{< /code >}}

言語別の設定は、指定できるプロパティが限られていますが、IntelliSense による入力補完機能 (`Ctrl + Space`) を使うと、どのようなプロパティを指定できるかがすぐにわかります。

複数の言語タイプをまとめて `[typescript, typescriptreact]` のように指定することはまだできないようです（2021年7月時点）。

- 参考: [Multiple languages specific editor settings · Issue #51935 · microsoft/vscode](https://github.com/microsoft/vscode/issues/51935)


マニュアル追加する項目は settings.json の上の方に書く
----

`settings.json` の書き方のちょっとしたコツですが、直接このファイルに設定項目やコメントを記述する場合は、末尾よりは先頭あたりに追記していくのがよいです。
VS Code の設定 UI で新しく設定した項目（デフォルト設定じゃなくなった項目）は、`settings.json` の末尾に自動追加されていくので、これと混ざらないようにするためです。
例えば、次のようにカテゴライズしておくと、設定 UI で変更した内容は、末尾の「その他の設定」のところに追加されていくので管理しやすくなります。

{{< code lang="js" title="settings.json" >}}
{
  // エクスプローラーに表示しないディレクトリ
  "files.exclude": {
    "**/.next": true,
    "**/node_modules": true
  },

  // 自動フォーマット（Prettier関連）設定
  "typescript.format.enable": false,
  "editor.defaultFormatter": "esbenp.prettier-vscode",
  "editor.formatOnSave": true,
  "editor.formatOnType": true,
  "editor.formatOnPaste": true,

  // その他の設定
  // ...
}
{{< /code >}}


（おまけ）プラグイン (extensions) が保存されている場所
----

ちなみに、VS Code の拡張 (extensions) は、ホームディレクトリの `.vscode` ディレクトリ以下に保存されています。

| OS | 拡張関連のファイルが格納されているディレクトリ |
| ---- | ---- |
| Windows | `%USERPROFILE%\.vscode\extensions` |
| macOS | `~/.vscode/extensions` |
| Linux | `~/.vscode/extensions` |

