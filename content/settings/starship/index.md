---
title: "Starship でターミナルのプロンプトをカスタマイズする"
url: "p/s8doawu/"
date: "2025-11-03"
tags: ["starship", "PC設定"]
---

starship とは
----

[starship](https://starship.rs/) は、ターミナルのプロンプトを簡単にカスタマイズするためのツールです。
下記はデフォルト設定でのプロンプト表示例です。

{{< image border="true" w="500" src="img-001.png" title="starship でカスタマイズされたプロンプト表示" >}}

このプロンプトの内容は、カレントディレクトリがどのような言語のプロジェクトであるかによって自動的に変化します。
例えば、Git 管轄下のディレクトリであればブランチ名や編集状態が表示されるし、Golang のプロジェクトであれば Golang のバージョンが表示されます（`go.mod` ファイルなどが参照される）。

- starship の特徴
  - クロスプラットフォーム対応（Windows、macOS、Linux）
  - ほとんどのシェルに対応 (bash、zsh、fish、PowerShell、...)
  - デフォルト設定で多くの情報を表示（Git ブランチ、Python バージョン、AWS プロファイル、...）
  - Rust で実装されているため高速

starship は内部的には `PS1` 環境変数 (bash/zsh) や `prompt` 関数 (PowerShell) などのシェルプロンプト設定を置き換えるというシンプルな仕組みで動作しています。
もちろん、`PS1` 環境変数を自力で設定してプロンプトをカスタマイズすることは可能ですが、starship を使うと簡単に多くの情報を表示できます。
さらに、starship はクラスプラットフォーム対応しているので、一度設定ファイル (`~/.config/starship.toml`) を作ってしまえば、Windows、Linux、macOS などいろんな環境で同じプロンプト設定を使い回すことができます。

本質的に starship は、さまざまな言語環境や情報を表示するためのツールですが、背景色や Nerd Font によるアイコン表示を組み合わせることで、プロンプトの見た目をより魅力的にすることもできます。

{{< image border="true" w="600" src="img-002.png" title="Gruvbox Rainbow プリセットによるプロンプト表示" >}}


Nerd Font のインストール
----

Starship は様々なアイコン画像を表示するために [Nerd Font](https://www.nerdfonts.com/) 系のフォントがインストールされていることを前提としています（ちなみに、Nerd Font は Vim のステータスライン表示をカスタマイズする Powerline などでも使われています）。
Nerd Font 系のフォントはいろいろありますが、ここでは、日本語のグリフを含んでいる [HackGen Nerd Font](https://github.com/yuru7/HackGen) を使います。
リリースページから **`.ttf` ファイルをダウンロードしてダブルクリック** すればインストールできます。

{{% note title="2種類の白源フォント" %}}
白源（はくげん／HackGen）フォントには下記の 2 種類がありますが、35 の方は「半角:全角」の横幅を「3:5」に調整したものなので、基本は 35 の付いていない方（比率1:2）を使います。

- `HackGen Console NF` ←こっち
- `HackGen35 Console NF`
{{% /note %}}

フォントのインストールが完了したら、**ターミナルアプリのフォント設定でそのフォントを選択** してください。
これを忘れると、Starship の各種アイコン表示が文字化けしてしまいます。


Starship のインストールと初期化
----

### starship コマンドのインストール

starship はパッケージマネージャーで簡単にインストールできます。

{{< code title="macOS の場合" >}}
brew install starship
{{< /code >}}

{{< code title="Windows の場合" >}}
winget install Starship.Starship
{{< /code >}}

`starship --version` コマンドが実行できるようになっていればインストール成功です。

### starship の初期化設定

次に、各シェルの設定ファイルで **`starship init`** を実行するように設定します。

{{< code lang="zsh" title="~/.zshrc（zsh の場合）" >}}
# Starship によるプロンプトのカスタマイズ（インストール済みの場合のみ）
if command -v starship &> /dev/null; then
  eval "$(starship init zsh)"
else
  echo "⚠️ starship is not installed. See: https://starship.rs/"
fi
{{< /code >}}

{{< code lang="bash" title="~/.bashrc（bash の場合）" >}}
# Starship によるプロンプトのカスタマイズ（インストール済みの場合のみ）
if command -v starship &> /dev/null; then
  eval "$(starship init bash)"
else
  echo "⚠️ starship is not installed. See: https://starship.rs/"
fi
{{< /code >}}

{{< code lang="powershell" title="$PROFILE（PowerShell の場合）" >}}
# Starship によるプロンプトのカスタマイズ（インストール済みの場合のみ）
if (Get-Command starship -ErrorAction SilentlyContinue) {
    Invoke-Expression (&starship init powershell)
} else {
    Write-Warning "starship is not installed. See: https://starship.rs/"
}
{{< /code >}}

上記のように `starship init` コマンドを実行すると、内部的に `$PS1` 変数（bash/zsh）や `prompt` 関数（PowerShell）が書き換えられ、プロンプトがこんな感じの表示に切り替わります。

{{< image border="true" w="500" src="img-001.png" title="starship でカスタマイズされたプロンプト表示" >}}

٩(๑❛ᴗ❛๑)۶ 動いたっ


Starship のカスタマイズ
----

### 設定ファイル (starship.toml)

starship は **`~/.config/starship.toml`** という設定ファイルでカスタマイズできます（ファイルがなければデフォルト設定で動きます）。

- 参考: [Configuration | Starship](https://starship.rs/config/)

プロンプトとして表示される内容は、トップレベルの **`format`** オプションで指定しますが、[デフォルトの `format` 設定](https://starship.rs/config/#default-prompt-format) だけでもかなり多くの情報が表示されるようになっています。

{{< code lang="toml" title="デフォルトの format 設定" >}}
format = '$all'

# 下記と同様
format = """
$username\
$hostname\
...
$git_branch\
$git_commit\
$git_state\
..."""
{{< /code >}}

複雑なプロンプト設定を 1 つの文字列として表現するのは無理があるため、starship のプロンプト表示設定は上記のようにモジュール化されて管理されています。
例えば、**`$usename`** や **`$git_branch`** のように表示項目（モジュール）ごとに分割されて管理されており、それぞれの表示設定を TOML のセクションの形で定義します。

{{< code lang="toml" title="~/.config/starship.toml（抜粋）" >}}
[git_branch]
symbol = '🌱 '
truncation_length = 4
truncation_symbol = ''
ignore_branches = ['master', 'main']
{{< /code >}}

それぞれの表示項目（モジュール）でどのような設定ができるかは、Starship の Configuration ドキュメントを参照してください（例: [git_branch の設定方法](https://starship.rs/config/#git-branch)）。

例えば、下記のように `directory` モジュールを設定すると、ディレクトリ名を省略せずにフルパスで表示するようになります。

{{< code lang="toml" title="~/.config/starship.toml" >}}
[directory]
truncation_length = 0  # ディレクトリパスの最大表示セグメント数（0:無制限）
truncate_to_repo = false  # リポジトリルートからの相対パスにしない
{{< /code >}}

### プリセット機能で設定ファイルを生成する

**`starship preset <プリセット名>`** というコマンドを使用すると、starship が組み込みで用意してくれている設定例を出力してくれます。
どのようなプリセットが用意されているかは下記のページを参照してください。

- 参考: [Presets | Starship](https://starship.rs/presets/)

例えば、Gruvbox Rainbow というプリセットを使うには次のように設定ファイルを作成します。

```console
$ starship preset gruvbox-rainbow -o ~/.config/starship.toml
```

{{< image border="true" w="600" src="img-002.png" title="Gruvbox Rainbow プリセットによるプロンプト表示" >}}

{{% note title="macOS の Terminal で色がおかしくなる場合" %}}
macOS (Sequoia 15) に標準搭載されている Terminal アプリは True Color 非対応なので、starship の設定で `#a6da95` のような色指定をしてしまうと正しい色で表示されません（全て一色表示などになってしまう）。
代わりに、パレットのインデックス指定で色を指定するか、True Color 対応のターミナルアプリを使用する必要があります（例: [iTerm2](https://iterm2.com) / [Alacritty](https://alacritty.org/)）。
Visual Studio Code 内のターミナルなどは正しく True Color 表示されます。
{{% /note %}}


その他
----

{{< code title="現在の設定内容を出力する" >}}
starship print-config
{{< /code >}}

{{< code title="現在のプロンプトの説明を表示する" >}}
startship explain
{{< /code >}}

