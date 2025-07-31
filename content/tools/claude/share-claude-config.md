---
title: "Claude Code のカスタムコマンドを Git や Dropbox で共有する"
url: "p/xpwvvfz/"
date: "2025-07-31"
tags: ["Claude"]
---

何をするか？
----

プロジェクトに依存しない汎用的な Claude Code カスタムスラッシュコマンドは、`~/.claude/commands` ディレクトリ以下に `*.md` ファイルを作成することで定義できます。
よく使うプロンプトをカスタムコマンドとして登録しておくと、どのディレクトリからでも呼び出せるのでとても便利です。
ただし、これらの設定ファイルはユーザーのホームディレクトリ以下で管理されるため、複数の PC で同じ設定を使いたい場合は、通常は手動でコピーする必要があります。

ここでは、**Windows のジャンクションや macOS/Linux のシンボリックリンク** を使って、`~/.claude` 以下のディレクトリを GitHub や Dropbox で管理しているディレクトリにリンクさせる方法を説明します。
こうすることで、複数の PC で Claude Code の設定を簡単に共有できるようになります。
この方法は、特にチームメンバーと汎用的なカスタムコマンドを共有したい場合に便利です。


共有する Claude Code の設定ファイルを用意する
----

Claude Code の設定ファイルを **`claude-config`** という Git リポジトリで管理することにします（もっと手軽に Dropbox などで管理してもよいです）。
まず、テスト用に下記のような `hello.md` カスタムコマンドを用意します。

{{< code lang="yaml" title="~/gitwork/claude-config/commands/hello.md" >}}
---
description: 簡単な挨拶コマンド
---

「てすと、てすと」とだけ出力してください。
{{< /code >}}

このカスタムコマンドを複数の PC で共有できるようにします。


~/.claude 以下からリンクする
----

上記で作成した Claude Code 用の設定ファイル（カスタムコマンド）を、各 PC の `~/.claude/commands` 以下からリンクします。
`.md` ファイルを 1 つずつリンクしていくのは大変なので、ディレクトリ単位でリンクしてしまうのがよいです。
ここでは、**`~/.claude/commands/maku`** というディレクトリを、`~/gitwork/claude-config/commands` ディレクトリにリンクすることにします。
`maku` という名前は適当に付けましたが、ここは任意の名前でよいです。
例えば、チーム名やプロジェクト名などにしておくと分かりやすいと思います。

### Windows の場合

Windows の場合は **`mklink /J`** コマンドを使ってディレクトリジャンクションを作成します。
シンボリックリンクを作る機能もありますが、そちらは管理者権限が必要だったりするので、ここではジャンクションを使います。
`mklink` はコマンドプロンプト用のコマンドなので、PowerShell から実行する場合は、次のように `cmd /c` 経由で実行します。

{{< code lang="powershell" title="Windows (PowerShell) でディレクトリのリンクを作成" >}}
cmd /c mklink /J "$HOME\.claude\commands\maku" "$HOME\gitwork\claude-config\commands"
{{< /code >}}

ディレクトリジャンクションを消したいときは、単純にエクスプローラー上で見えているフォルダ (`~/.claude/commands/maku`) を削除するだけで OK です。

### macOS/Linux の場合

macOS や Linux の場合は **`ln -s`** コマンドを使ってディレクトリのシンボリックリンクを作成します。

{{< code lang="bash" title="macOS/Linux でディレクトリのリンクを作成" >}}
ln -s ~/gitwork/claude-config/commands ~/.claude/commands/maku
{{< /code >}}

これで、`~/gitwork/claude-config/commands/hello.md` というファイルが、`~/.claude/commands/maku/hello.md` として見えるようになります。


テスト
----

上記のリンク作業を終えると、`~/.claude/commands/maku/hello.md` というスラッシュコマンドを参照できる状態になっています。
今回は `maku/hello.md` というパスとして見えているので、Claude Code からは **`/maku:hello`** という形でコマンドを起動します。

{{< code title="Claude Code からのカスタムコマンド実行" >}}
> /maku:hello

● てすと、てすと
{{< /code >}}

うまく動きました！
٩(๑❛ᴗ❛๑)۶ わーぃ

新しいカスタムコマンドを作成したいときは、Git リポジトリで管理している `~/gitwork/claude-config/commands` ディレクトリ以下に `.md` ファイルを追加していけば OK です。
このようにすれば、GitHub や Dropbox で簡単に Claude Code の設定ファイルを共有できます。

- 参考: 実際の GitHub での管理例
  - [https://github.com/maku77/claude-config](https://github.com/maku77/claude-config)

