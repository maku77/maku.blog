---
title: "Alacritty で快適なターミナル環境を手にいれる（macOS/Windows/Linux 対応）"
linkTitle: "Alacritty"
url: "p/p9ppdqy/"
date: "2024-03-26"
tags: ["Alacritty"]
---

Alacritty とは
----

[Alacritty（アラクリッティ）](https://alacritty.org/) は、macOS や Windows の「ターミナル」アプリの代わりに使えるターミナルエミュレーターで、次のような特徴を持っています。

- Windows/macOS/Linux/BSD など主要な OS をサポート（ノウハウや設定を使いまわせる）
- Rust で実装されていて高速。レンダリングも GPU で高速。
- VI モードでキーボードによる快適な操作が可能
- 設定ファイルが TOML 形式でわかりやすい（シンプルさを追求しているため、GUI ベースの設定画面は提供しない）
- `vim` や `tmux` のようなターミナルに寄生するアプリと相性がよくて表示の問題が起きにくい
- スクロールバッファ内の検索が可能
- マルチウィンドウモードもあり（ただし思想としては `tmux` などのターミナルマルチプレクサーを使うべきとしている）

簡単にまとめると、`vim` や `tmux` 使いで、シンプルで高速なクロスプラットフォームなターミナルアプリを求めている人にうってつけということですね！
ちなみに、Alacritty という名前の末尾が tty になっているのがおしゃれです（「敏捷性」を表す英単語「alacrity」をもじってます）。


VI モード
----

VI モードに切り替えると、VI 風のキーバインドを使ったカーソルの移動、画面スクロール、検索などが可能になります。
VI モードへの入り方だけ特殊 (`Ctrl + Shift + Space`) ですが、あとはほぼ VI の操作方法に従って操作できます（参考: [まくまく Vim ノート](https://maku77.github.io/vim/)）。

| キーバインド | 説明 |
| ---- | ---- |
| **`Ctrl + Shift + Space`** | VI モード ON/OFF |
| **`i`** | VI モード OFF（普通のプロンプト操作に戻る） |
| `h` / `j` / `k` / `l` | カーソル移動 |
| `v` / `Ctrl + v` / `Shift + v` | 選択／矩形選択／行選択 |
| `y` | 選択したテキストをヤンク（コピー） |
| `Ctrl + B` / `Ctrl + F` | 上下スクロール |
| `/` / `?` | 前方検索／後方検索 |


スクロールバッファ内の検索
----

下記のキーコンビネーションで、スクロールバッファ内のテキストをインクリメンタルサーチできます。
まぁ、これを使わなくても VI モードに入って `/` で検索しちゃえばよい気はします。

| キーバインド | 説明 |
| ---- | ---- |
| `Ctrl + Shift + f` (`Cmd + f`) | 前方検索（インクリメンタルサーチ） |
| `Ctrl + Shift + b` (`Cmd + b`) | 後方検索（インクリメンタルサーチ） |
| `Enter` / `Shift + Enter` | 次／前の一致箇所へ |
| `Esc` | 検索を終了 |

次のような正規表現パターンを使って検索できます。
例えば、`\d{4}-\d\d-\d\d` と入力すると、`2024-03-26` のような日付を検索できます。

| パターン | 説明 |
| ---- | ---- |
| `.` | 任意の 1 文字に一致 |
| `[abc]` | `a`, `b`, `c` のいずれかに一致 |
| `\d` / `[0-9]` | 任意の数値に一致 |
| `*` | （前の文字が）0 回以上 |
| `+` | （前の文字が）1 回以上 |
| `?` | （前の文字が）0 回 or 1 回 |
| `{4}` | （前の文字が） 4 回 |
| `{1-2}` | （前の文字が）1 回 or 2 回 |

ちょっと高度な使い方:

1. `Cmd + f` で検索モードに入る
2. `doc.*` と入力して doc で始まるテキストを検索
3. `Esc` で検索モードを終了
4. 最後に選択されていたヒット箇所がハイライトされているので、`Cmd + c` → `Cmd + v` などでコピペ


Alacritty の設定
----

Alacritty の設定ファイルは下記のパスに配置します。
バージョン 0.13 以前は YAML 形式が使われていましたが、今は TOML 形式で作成します。

- Linux/macOS の場合: **`~/.config/alacritty/alacritty.toml`**
- Windows の場合: **`%APPDATA%\alacritty\alacritty.toml`**

`~/.alacritty.toml` というパスで配置しても認識しますが、設定を複数ファイルに分けて記述するのであれば、`~/.config/alacritty` ディレクトリにまとめて配置するのがよいでしょう（これは XDG 標準に従った配置でもあります）。

{{% note title="XDG 標準の .config ディレクトリ" %}}
`~/.config` の代わりに別のディレクトリを使いたいときは、`$XDG_CONFIG_HOME` 環境変数にそのディレクトリパスを設定します。
これは、Alacritty の仕組みではなく、Linux や macOS で config ディレクトリを設定するための標準的な仕組み (XDG) です。

- 参考: [XDG Base Directory Specification](https://specifications.freedesktop.org/basedir-spec/)
{{% /note %}}

設定ファイルの詳細は [マニュアルページ](https://alacritty.org/config-alacritty.html) で確認できますが、以下に簡単な設定例を紹介しておきます。

### 設定ファイルのオートリロード

Alacritty の設定ファイルをリロードするには Alacritty の再起動が必要ですが、下記の設定をしておくと、設定ファイルを修正したときに自動でリロードしてくれるようになります。
最初にこの設定をしておくと便利です。

{{< code lang="toml" title="~/.config/alacritty/alacritty.toml" >}}
# 設定ファイル変更時にオートリロード
live_config_reload = true
{{< /code >}}

ただし、ウィンドウの見た目の設定 (`[window]`) などは、Alacritty の再起動が必要なことがあります。

### ウィンドウの表示設定

__`[window]`__ セクションでウィンドウの表示をカスタマイズできます。

{{< code lang="toml" title="~/.config/alacritty/alacritty.toml" >}}
[window]
# ウィンドウサイズ
dimensions = { columns = 100, lines = 30 }
# 画面端のパディング
padding = { x = 4, y = 2 }
# 背景を透過させる
opacity = 0.9
{{< /code >}}

### 色の設定

__`[colors]`__ セクションで色の設定を行えます。

{{< code lang="toml" title="~/.config/alacritty/alacritty.toml" >}}
[colors.primary]
# 背景色
background = "#000055"

[colors]
# カーソル色
cursor = { text = "CellBackground", cursor = "#00ff00" }
# VI モードのカーソル色
vi_mode_cursor = { text = "CellBackground", cursor = "#ff0000" }
# 選択範囲の色
selection = { text = "CellBackground", background = "#ffaaaa" }
{{< /code >}}

### カーソル

__`[cursor]`__ セクションでカーソルの表示方法を設定できます。

{{< code lang="toml" title="~/.config/alacritty/alacritty.toml" >}}
[cursor]
# カーソルを点滅させる
style = { blinking = "On" }
# 点滅のインターバル
blink_interval = 500
# 点滅を停止するまでの秒数（0: 停止しない）
blink_timeout = 0
{{< /code >}}

### フォント

__`[font]`__ セクションでフォントの表示設定を行えます。

{{< code lang="toml" title="~/.config/alacritty/alacritty.toml" >}}
[font]
# フォントサイズ（デフォルトは 11.25）
size = 16
{{< /code >}}

### キーバインド

Alacritty のデフォルトのキーバインドは [alacritty-bindings(5)](https://alacritty.org/config-alacritty-bindings.html) で確認できます。
これ以外のキーバインドを設定したいときは __`[keyboard]`__ セクションで設定します。
キー名（`F1` や `PageUp`）のリストは [こちら](https://docs.rs/winit/latest/winit/keyboard/enum.NamedKey.html) で確認できます。

次の例では、`PageUp` と `PageDown` キーを画面スクロールに割り当てています（デフォルト設定でも `Shift + PageUp/PageDown` で画面スクロールできます）。

{{< code lang="toml" title="~/.config/alacritty/alacritty.toml" >}}
[keyboard]
binding = [
  { key = "PageUp", action = "ScrollPageUp" },
  { key = "PageDown", action = "ScrollPageDown" },
]
{{< /code >}}

### カラーテーマ

[カラーテーマ](https://github.com/alacritty/alacritty-theme) をインポートすることで、まとめて色の設定を行うこともできます。


Windows 用の設定
----

### WSL の Ubuntu を使う

Windows で Alacritty を起動したときに WSL 側のシェルを使いたいときは、**`[shell]`** セクションで以下のように設定しておきます。
起動時のワーキングディレクトリを移動したいときは `--cd` オプションで指定できます（Windows 形式の `C:/aaa` や、Linux 形式の `/mnt/x`、`~/gitwork` といったパスを指定できます）。

{{< code lang="toml" title="~/.config/alacritty/alacritty.toml" >}}
[shell]
program = "wsl"
args = ["--cd", "D:/y/gitwork"]
{{< /code >}}

通常はデフォルトの Ubuntu ディストリビューションが起動するはずですが、他のディストリビューションを起動したいときは `-d <dist>` オプションで指定できます。
WSL にどのようなディストリビューションがインストールされているかは、**`wsl -l`** で確認できます。

```
C:\> wsl -l
Linux 用 Windows サブシステム ディストリビューション:
Ubuntu (既定)
podman-machine-default
```

### Windows と macOS 用の設定ファイルを分ける

Alacritty はクロスプラットフォームなアプリですが、shell 設定などは OS 別の設定をしたくなります。
そのようなケースでは、次のように共通部分の設定ファイルと OS ごとの設定ファイルを分けて作っておいて、**`import`** で読み込むようにすれば OK です。

- `alacritty-common.toml` ... 共通の設定
- `alacritty-mac.toml` ... macOS 用の設定
- `alacritty-win.toml` ... Windows 用の設定

後から読み込んだ設定ファイルで設定が上書きされるので、OS ごとの設定ファイルは後ろに来るように並べてください。

下記は、各 OS 用の `alacritty.toml` の記述例です。
ここでは、`dotfiles` という Git リポジトリで設定ファイルを管理していることを想定しています。

{{< code lang="macOS 用の alacritty.toml" >}}
import = [
  "~/gitwork/dotfiles/alacritty/alacritty-common.toml",
  "~/gitwork/dotfiles/alacritty/alacritty-mac.toml",
]
{{< /code >}}

{{< code lang="Windows 用の alacritty.toml" >}}
import = [
  "D:/gitwork/dotfiles/alacritty/alacritty-common.toml",
  "D:/gitwork/dotfiles/alacritty/alacritty-win.toml",
]
{{< /code >}}


その他
----

Alacritty は、ターミナルマルチプレクサーである tmux と一緒によく使われます。
tmux を使うと、ターミナル内でタブのような機能を使えるようになります。

- 参考: [tmux チートシート](/p/hijkfza/)

