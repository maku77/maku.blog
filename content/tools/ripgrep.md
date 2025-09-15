---
title: "rg (ripgrep) コマンドの使い方まとめ"
url: "p/4jhbtm3/"
date: "2025-09-15"
tags: ["cheatsheet", "ripgrep"]
---

超高速なテキスト検索コマンド [**rg (ripgrep)**](https://github.com/BurntSushi/ripgrep) にはずっとお世話になっていますが、ちょっと凝った検索をしようとしたときに、オプションを忘れてしまうことが多いので、チートシートの形でまとめておきます。

Ripgrep チートシート
----

- `rg TODO`
  - カレントディレクトリ以下ですべての「TODO」を検索します。
- `rg "^def add"`
  - 行頭が `def add` で始まる行を検索します（デフォルトで正規表現とみなされます）。
- <code>rg <strong>-F</strong> "* + *"</code>
  - 検索文字列を正規表現として解釈せず、そのままの文字列として検索します。上記の例では、`* + *` という文字列を検索します。
- <code>rg <strong>-i</strong> TODO</code>
  - 大文字・小文字を区別せずに「TODO」を検索します。
- <code>rg <strong>-w</strong> TODO</code>
  - 単語単位で「TODO」を検索します。例えば `TODOS` や `MYTODO` はマッチしません（安心してください、`TODO:` にはマッチします）。
- <code>rg <strong>-C2</strong> TODO</code>
  - マッチした行の前後 2 行も一緒に表示します（`-A2 -B2` と指定するのと同様）。
- <code>rg <strong>-c</strong> TODO</code>
  - 各ファイルごとにマッチした行数のみを表示します。
- <code>rg TODO <strong>src/main</strong></code>
  - `src/main` ディレクトリ以下（あるいは同名のファイル）で「TODO」を検索します。
    ディレクトリであることを明示したい場合は、末尾にスラッシュを付けます（`src/main/`）。
- <code>rg <strong>-t py</strong> TODO</code>
  - Python ファイル（拡張子 `.py`）のみを検索対象にします。
  - ここで指定するタイプは拡張子ではなくファイルタイプです。指定できるファイルタイプの一覧は **`rg --type-list`** で確認できます。
  - 逆に、特定のファイルタイプを除外したい場合は <code><strong>-T</strong> py</code> のように大文字の `-T` を使います。
  - 例えば、ファイルタイプとして `minified: *.min.css, *.min.html, *.min.js` が定義されているので、**`-T minified`** と指定すると `min` 系のファイルを除外できます。
- <code>rg <strong>-g "*.{js,ts}"</strong> TODO</code>
  - glob パターンで指定したファイルのみを検索対象にします。引数がシェルによって展開されないように、必ずクォートで囲みます。
  - 大文字小文字を区別せずにマッチさせたい場合は、`-g` の代わりに **`--iglob`** を使います。
  - glob パターンの例: 
    - `-g "*.{js.ts}"` ... 拡張子が `.js` または `.ts` のファイル
    - `--iglob "*test*` ... ファイル名に `test` や `Test` を含むファイル
    - `-g "**/game/**/*.py"` ... 任意の階層の `game` ディレクトリ以下の `.py` ファイル
    - `-g "**/game/*.py"` ... 任意の階層の `game` ディレクトリ直下の `.py` ファイル
    - `-g "/.*"` ... この階層のドットファイル
- <code>rg <strong>--hidden</strong> TODO</code>
  - 隠しファイル・隠しディレクトリも検索対象に含めます。
  - ただし、ドットファイルだけを検索したい場合は、`-g ".*"` のように glob パターンを使う方がよいです。
- <code>rg <strong>--no-ignore</strong> TODO</code>
  - `.gitignore` や `.ignore`、`$HOME/.rgignore` などで無視されているファイル・ディレクトリも検索対象に含めます。


どのファイルが検索対象になっているかを調べる
----

`--files` オプションを使うと、実際にどのファイルが検索対象になっているかを確認できます。
試行錯誤しながら検索条件を調整したいときに便利です。

- <code>rg <strong>--files</strong></code>
- <code>rg <strong>--files</strong> -t js</code>
- <code>rg <strong>--files</strong> -t js src/</code>
- <code>rg <strong>--files</strong> --hidden</code>
- <code>rg <strong>--files</strong> -g "/.*"</code>


具体的な使用例
----

{{< code lang="console" title="src ディレクトリ以下で TODO を検索" >}}
$ rg -i -w TODO src/
{{< /code >}}

