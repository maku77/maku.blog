---
title: "rg (ripgrep) コマンドの使い方まとめ"
url: "p/4jhbtm3/"
date: "2025-09-15"
lastmod: "2026-02-28"
tags: ["cheatsheet", "ripgrep"]
changes:
  - 2026-02-28: トラブルシュートを追加
---

超高速なテキスト検索コマンド [**rg (ripgrep)**](https://github.com/BurntSushi/ripgrep) にはずっとお世話になっていますが、ちょっと凝った検索をしようとしたときに、オプションを忘れてしまうことが多いので、チートシートの形でまとめておきます。
`rg` は Windows でも macOS/Linux でもほぼ同じように使えるので、使いこなせるようになると便利です。


Ripgrep チートシート
----

### 検索パターン

`rg "^def "`
: カレントディレクトリ以下のすべてのファイルから、行頭が `def ` で始まる行を検索します。
`rg` はデフォルトで、**入力パターンを正規表現として扱います**。

<code>rg <strong>-F</strong> "*****"</code>
: 検索文字列を正規表現として解釈せず、そのままの文字列 (Fixed String) として検索します。上記の例では、`*****` という文字列を検索します。

<code>rg <strong>-i</strong> "PATTERN"</code>
: 大文字・小文字を区別せずに「PATTERN」を検索します。

<code>rg <strong>-w</strong> "PATTERN"</code>
: 単語単位で「PATTERN」を検索します。例えば `PATTERNS` や `MYPATTERN` はマッチしません（安心してください、`TODO:` にはマッチします）。
`-w` オプションの代わりに、正規表現の単語境界 **`\b`** を使って、`rg "\bPATTERN\b"` とすることもできます。

### 対象ファイル／ディレクトリの絞り込み

<code>rg PATTERN <strong>path/to/dir</strong></code>
: `path/to/dir` ディレクトリ以下（あるいは同名のファイル）で「PATTERN」を検索します。
  ディレクトリであることを明示したい場合は、末尾にスラッシュを付けます（`path/to/dir/`）。

<code>rg <strong>-t py</strong> PATTERN</code>
<code>rg <strong>-t sh</strong> PATTERN</code>
: Python ファイル（拡張子 `.py`）やシェルスクリプト（拡張子 `.sh` や `.zsh`）のみを検索対象にします。
  ここで指定するタイプは拡張子ではなくファイルタイプです。指定できるファイルタイプの一覧は **`rg --type-list`** で確認できます。
  逆に、特定のファイルタイプを除外したい場合は **`-T py`** のように大文字の `-T` を使います。例えば **`-T minified`** と指定すると `min` 系のファイル (`minified: *.min.css, *.min.html, *.min.js`) を除外できます。

<code>rg <strong>-g "*.{js,ts}"</strong> PATTERN</code>
: glob パターンで指定したファイルのみを検索対象にします。引数がシェルによって展開されないように、必ずクォートで囲みます。
  大文字小文字を区別せずにマッチさせたい場合は、`-g` の代わりに **`--iglob`** を使います。
  glob パターンを `!` で始めると、指定したパターンにマッチするファイルやディレクトリを検索対象から除外できます（zsh による履歴展開が働かないようにするためには、パターンをシングルクォートで囲みます）。

  - glob パターンの例: 
    - `-g '*.{js,ts}'` ... 拡張子が `.js` または `.ts` のファイルを検索
    - `-g '*test*'` ... ファイル名に `test` を含むファイルを検索
    - `--iglob '*test*'` ... ファイル名に `test` を含むファイルを検索（大文字小文字を区別しない）
    - `-g '**/hoge/**/*.py'` ... 任意の階層の `hoge` ディレクトリ以下の `.py` ファイルを検索
    - `-g '**/hoge/*.py'` ... 任意の階層の `hoge` ディレクトリ直下の `.py` ファイルを検索
    - `-g 'hoge/**'` ... この階層の `hoge` ディレクトリ以下のファイルを検索
    - `-g 'hoge/*'` ... この階層の `hoge` ディレクトリ直下のファイルを検索
    - `-g '!*.min.js'` ... 拡張子が `.min.js` のファイルを検索対象から除外
    - `-g '!hoge/'` ... この階層の `hoge` ディレクトリを検索対象から除外
    - `-g '!**/hoge/'` ... 任意の階層の `hoge` ディレクトリを検索対象から除外

<code>rg <strong>--hidden</strong> PATTERN</code>
: 隠しファイル・隠しディレクトリも検索対象に含めます。
  ただし、ドットファイルだけを検索したい場合は、`-g ".*"` のように glob パターンを使う方がよいです。

<code>rg <strong>--no-ignore</strong> PATTERN</code>
: `.gitignore` や `.ignore`、`$HOME/.rgignore` などで無視されているファイル・ディレクトリも検索対象に含めます。

<code>rg <strong>--max-depth 1</strong> PATTERN src/</code>
: `src/` ディレクトリの直下だけを検索対象にします。サブディレクトリは検索されません。

### 表示オプション

<code>rg <strong>-C2</strong> PATTERN</code>
: マッチした行の前後 2 行も一緒に表示します（`-A2 -B2` と指定するのと同様）。

<code>rg <strong>-c</strong> PATTERN</code>
: ファイルごとにマッチした行数のみを表示します。

<code>rg <strong>-l</strong> PATTERN</code>
: ファイル名のみを表示します。


どのファイルが検索対象になっているかを調べる (`--files`)
----

**`--files`** オプションを使うと、実際にどのファイルが検索対象になっているかを確認できます。
試行錯誤しながら検索条件を調整したいときに便利です。

- <code>rg <strong>--files</strong></code>
- <code>rg <strong>--files</strong> -t js</code>
- <code>rg <strong>--files</strong> -t js src/</code>
- <code>rg <strong>--files</strong> --hidden</code>
- <code>rg <strong>--files</strong> -g "/.*"</code>

これを利用して、ファイル名に特定のパターンを含むファイルの一覧を取得することもできます。

{{< code lang="console" title="ファイル名に test を含むファイルの一覧を取得" >}}
$ rg --files | rg test
{{< /code >}}


具体的な使用例
----

{{< code lang="console" title="src ディレクトリ以下で TODO コメントを検索" >}}
$ rg -w "TODO" src/
{{< /code >}}

{{< code lang="console" title="test や Test で終わる .kt ファイルを除外" >}}
$ rg --iglob '!*test.kt' PATTERN src/
{{< /code >}}


トラブルシュート
----

### Operation not permitted という警告が大量に出る

```
./Library/Calendars: Operation not permitted (os error 1)
```

のようなエラーは、アクセス権限のないファイルやディレクトリを検索しようとしたときに出る警告です。
これは標準エラー出力への出力なので、次のように **`2>`** を使ってリダイレクトすればこれらの表示を抑制できます。

{{< code lang="console" title="標準エラー出力を /dev/null にリダイレクト" >}}
$ rg PATTERN 2> /dev/null
{{< /code >}}

glob オプション (`-g/--glob`) を使って、アクセス権限のないディレクトリを検索対象から除外することもできます。
**`!`** を使うと、指定したパターンにマッチするファイルやディレクトリを検索対象から除外できます。

{{< code lang="console" title="Library ディレクトリを検索対象から除外" >}}
$ rg -g '!Library/' PATTERN
{{< /code >}}

{{% note title="glob パターンの説明" %}}
zsh では `!` が履歴展開として解釈されるため、glob パターンをシングルクォートで囲む必要があります。
例えば、zsh で `"!ls"` とすると、最後に実行した `ls` コマンドの履歴展開が行われてしまいますが、`'!ls'` とすればそのまま `!ls` という文字列が glob パターンとして渡されます。
{{% /note %}}

常に検索対象から除外したいファイルやディレクトリがある場合は、**`.ignore`** ファイルや `.gitignore` ファイルを作成して、そこに除外したいパターンを記述しておくと便利です。

{{< code lang="ignore" title=".ignore（検索除外パターン）"  >}}
/Downloads/
/Library/
/Movies/
{{< /code >}}

