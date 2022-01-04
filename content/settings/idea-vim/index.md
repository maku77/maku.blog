---
title: "Android Studio の IdeaVim プラグインで快適コーディング"
url: "/p/f4ugxri"
date: "2021-06-08"
tags: ["設定"]
---

{{% private %}}
- [IntelliJ(Android Studio)のVimプラグイン「IdeaVim」の使い方と設定](https://ikenox.info/blog/ideavim-introduction/)
{{% /private %}}


IdeaVim プラグインのインストール
----

Android Studio (IntelliJ IDEA) に __IdeaVim__ プラグインを入れると、Vim 系のキーマッピングが有効になり、高速にコーディングできるようになります（もちろん Vim の操作に精通しておく必要がありますが）。
IdeaVim プラグインは Android Studio から下記のように辿って簡単にインストールできます。

{{< image border="true" src="img-001.png" >}}

1. `File` → `Settings` → `Plugins`
2. `IdeaVim` で検索してインストール


IdeaVim の設定 (.ideavimrc)
----

通常の Vim エディタの設定ファイルは `~/.vimrm` ですが、IdeaVim の設定ファイルは __`~/.ideavimrc`__ です。
Windows の場合は、`%USERPROFILE%\.ideavimrc` です（or `_ideavimrc`）。

{{< code title="~/.ideavimrc" >}}
set clipboard=unnamed
nnoremap gd :action GotoDeclaration<CR>
nnoremap gi :action GotoImplementation<CR>
nnoremap gs :action GotoSuperMethod<CR>
nnoremap gb :action Back<CR>
nnoremap gf :action Forward<CR>
nnoremap <C-Left> :action Back<CR>
nnoremap <C-Right> :action Forward<CR>
{{< /code >}}

`.ideavimrc` ファイルを変更したときは、次のようにして反映できます。

{{< code >}}
:source ~/.ideavimrc
{{< /code >}}

### OS のクリップボードと連携

{{< code >}}
set clipboard=unnamed
{{< /code >}}

`x` キーによる切り取りや、`yy` での行ヤンク（コピー）されたレジスタ内容を、Windows などの OS クリップボードと連携させることができます。

### キーマッピング

{{< code >}}
nnoremap gd :action GotoDeclaration<CR>
nnoremap gi :action GotoImplementation<CR>
nnoremap gs :action GotoSuperMethod<CR>
nnoremap gb :action Back<CR>
nnoremap gf :action Forward<CR>
nnoremap <C-Left> :action Back<CR>
nnoremap <C-Right> :action Forward<CR>
{{< /code >}}

IdeaVim も Vim と同様に `map`、`noremap` 系コマンドでキーマップを定義することができます。

- 参考: [キーマップの基本 (map, noremap) | まくまくVimノート](https://maku77.github.io/vim/keymap/basic.html)

このとき、__`:action`__ コマンドを組み合わせて使うと、Android Studio (IntelliJ IDEA) 特有の機能を呼び出すことができます。
例えば、`:action Back` で「戻る (Navigate Back)」操作を行うことができるし、`:action GotoDeclaration` で「定義位置、参照箇所へのジャンプ」を行えます。

- 参考: [IdeaVim - Executing actions](https://github.com/JetBrains/ideavim#executing-actions)


どんなコマンドがあるか？
----

IdeaVim の __`:action XXX`__ で実行できるコマンドにどのようなものがあるかは、__`:actionlist`__ コマンドで検索することができます。

{{< code >}}
:actionlist <キーワード>
{{< /code >}}

例えば、`:actionlist Goto` と実行すると、`GotoDeclaration` や `GotoImplementation` といったコマンドがあることが分かります。

