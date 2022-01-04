---
title: "VS Code のフォーマッターで自動整形する (editor.formatOnSave)"
url: "/p/qcoz9ju"
date: "2021-07-14"
tags: ["VS Code", "Prettier"]
---

{{% private %}}
- https://code.visualstudio.com/docs/editor/codebasics#_formatting
{{% /private %}}

VS Code には、標準で各種言語用のフォーマッターが搭載されており、JavaScript、TypeScript、JSON、HTML などのコードを自動整形することができます。


手動でフォーマッターを起動する
----

### ファイル全体をフォーマット (editor.action.formatDocument)

- コマンドパレット[^a] を使う場合: __Format Document__ を選択
- ショートカットキーを使う場合:
    - (Windows) __`Shift + Alt + F`__
    - (macOS) __`Shift + Option + F`__

### 選択中の行をフォーマット (editor.action.formatSelection)

- コマンドパレット[^a] を使う場合: __Format Selection__ を選択
- ショートカットキーを使う場合:
    - (Windows) __`Ctrl + K → Ctrl + F`__
    - (macOS) __`Cmd + K → Cmd + F`__

[^a]: `Cmd/Ctrl + Shift + P`


ファイル保存時などに自動でフォーマットする
----

下記の設定をしておくと、ファイル保存時や、コードの編集中に自動的にフォーマッターを起動することができます。
設定ファイル (`settings.json`) の場所に関しては、[こちらの記事](/p/tfq2cnw) を参考にしてください。

### 全ての種類のファイルで自動フォーマットする場合

{{< code lang="js" title="settings.json" >}}
{
  // 自動フォーマット設定
  "editor.formatOnSave": true,   // 保存時にフォーマット
  "editor.formatOnType": true,   // 入力中（改行時）にフォーマット
  "editor.formatOnPaste": true,  // ペースト時にフォーマット

  // その他...
}
{{< /code >}}

フォーマッターによっては、保存時のフォーマット (`editor.formatOnSave`) にしか対応していません。

### 特定の種類のファイルで自動フォーマットする場合

下記は、TypeScript ファイル (`.ts`, `.tsx`) の編集中のみオートフォーマットを有効にする例です（参考: [per-language configuration](/p/tfq2cnw/#per-language)）。

{{< code lang="js" title="settings.json" >}}
{
  // .ts ファイル用の設定
  "[typescript]": {
    "editor.formatOnSave": true,
    "editor.formatOnType": true,
    "editor.formatOnPaste": true,
  },

  // .tsx ファイル用の設定
  "[typescriptreact]": {
    "editor.formatOnSave": true,
    "editor.formatOnType": true,
    "editor.formatOnPaste": true,
  },

  // ...
}
{{< /code >}}

おすすめは、デフォルトで自動フォーマットを有効にしておいて、フォーマットしたくない種類のファイルだけ OFF にする設定です。

{{< code lang="js" >}}
{
  // デフォルトで自動フォーマットを ON
  "editor.formatOnSave": true,

  // Markdown ファイルは自動フォーマットしない
  "[markdown]": {
    "editor.formatOnSave": false,
    "editor.wordWrap": "on",
    "editor.renderWhitespace": "all",
    "editor.acceptSuggestionOnEnter": "off",
    "files.trimTrailingWhitespace": false
  },

  // ...
}
{{< /code >}}


Prettier などの拡張フォーマッターを使用する
----

VS Code に標準搭載されているフォーマッターの代わりに、拡張機能が提供するフォーマッターを使用するには、__`editor.defaultFormatter`__ にその拡張機能の ID を設定します。
ここでは、下記の Prettier 拡張を使ってみます。
[Prettier 自体のインストールや設定](/p/au8iu6u) は終わっているものとします。

- [Prettier - Code formatter - Visual Studio Marketplace](https://marketplace.visualstudio.com/items?itemName=esbenp.prettier-vscode)

この拡張機能の ID は `esbenp.prettier-vscode` なので、設定ファイルの中で次のように指定します。

{{< code lang="js" title="settings.json" >}}
{
  // 自動フォーマット設定
  "typescript.format.enable": false,  // 組み込みのフォーマッターは無効にする
  "editor.defaultFormatter": "esbenp.prettier-vscode",  // Prettier を使う
  "editor.formatOnSave": true,
  "editor.formatOnType": true,
  "editor.formatOnPaste": true,

  // ...
}
{{< /code >}}

上記では、ファイル保存時 (`formatOnSave`) などに自動でフォーマットをかけるように設定していますが、コマンドパレット[^a] から `Format Document` などを選んだ場合も同様に Prettier 拡張のフォーマッターが使われるようになります。

