---
title: "VS Code で Python 用のフォーマッター (Black) を使う"
url: "p/4oybku6/"
date: "2021-11-13"
lastmod: "2023-06-21"
changes:
  - 2023-06-21: VS Code の Black Formatter 拡張について追記
tags: ["VS Code", "Python"]
---

何をするか？
----

Visual Studio Code に [Python 拡張](https://marketplace.visualstudio.com/items?itemName=ms-python.python) を入れることで、Python コードの編集（自動補完など）ができるようになりますが、コードの自動フォーマットを行うには、フォーマッターを別途インストールしておく必要があります（いろんなフォーマッターがあるため）。

Python のフォーマッターとしては、autopep8 や yapf などもありますが、ここでは、最近人気がある [Black](https://black.readthedocs.io/) を VS Code の自動フォーマッターとして設定します。
Facebook や Dropbox、Mozilla などでも導入されており、採用実績としては申し分なさそうです。

{{% note title="頑固なフォーマッター" %}}
Black は自分自身を The __uncompromising__ code formatter と説明しています。
ようするに、「私が定義する設定に従いなさい」という意味で、ユーザーにほとんど設定の余地を残していません（行の長さ程度は設定できます）。
このようにすることで、チーム内でのフォーマット論争を防ぐ効果があります。
TypeScript (JavaScript) でよく使われている Prettier フォーマッターも同様の思想で作られており、ほとんど設定ができないようになっています（こちらは __opinionated__ という単語を使っています）。

- 参考: [TypeScript コードを Prettier で自動整形する](/p/au8iu6u)
{{% /note %}}

（2023-06 追記）Python 拡張とは別に、[Black Formatter 拡張](https://marketplace.visualstudio.com/items?itemName=ms-python.black-formatter) が単独で提供されています。
この拡張には `black` コマンドがバンドルされているため、別途外部コマンドとして `black` をインストール (`pip install black`) する必要がありません。
VS Code での開発が前提であれば、Black Formatter 拡張を使うのが手っ取り早いです。
ここでは、下記の 2 種類の方法を説明します。

- 方法1: Python 拡張 ＋ Black Formatter 拡張を使う方法
- 方法2: Python 拡張から外部の `black` コマンドを使う方法

Python 拡張と Black Formatter 拡張は、両方とも Microsoft 公式の拡張なので、将来的には統合されるかもしれません。


Black ではどのようなスタイルになるか？
----

Black フォーマッターは、次のようなルールで Python コードをフォーマットします。

- インデントは __スペース 4 文字__
  - [PEP 8](https://www.python.org/dev/peps/pep-0008/) 通り。
- トップレベルのクラス定義や関数定義の間には __2 行の空白行__ を入れる（クラス内のメソッド間は 1 行）
  - PEP 8 通り。
- 1 行あたり最大 __88 文字__ まで
  - PEP 8 は 79 文字と言っているが、それだと改行がたくさん入りすぎる傾向があるので、Black では 1 割増しの 88 文字にした。Raymond Hettinger の講演 [Beyond PEP 8](https://www.youtube.com/watch?v=wf-BqAjZb8M&t=260s) でも 90 文字くらいがよいと指摘している。
- 文字列リテラルは __ダブルクォートで囲む__（[Issue #51](https://github.com/psf/black/issues/51) などの議論を経てダブルクォートに落ち着いた。理由をまとめると以下のような感じ）
  - [PEP 257](https://www.python.org/dev/peps/pep-0257/#what-is-a-docstring) で docstring はダブルクォートで囲むべしとされており、それに合わせるのが一貫性があってよい。PEP 8 では docstring 以外の文字列リテラルにシングルクォートを使っているが、バラバラにする理由がない。
  - 空白文字列をシングルクォートで表現すると `''` となって、フォントによっては判別しにくくなる。ダブルクォートであれば一目瞭然 (`""`)。
  - ダブルクォートの中でシングルクォートを使うことはよくあるが（例: `"don't"`）、その逆は少ない。
  - UK レイアウトや German レイアウトのキーボードでは、`"` の入力は苦じゃないよ。シングルクォートの方が入力しやすければ、それで入力しておいて Black に `"` に変換させればよい。

文字列リテラルをシングルクォートで囲っている Python プロジェクトは多いので、Black が一貫してダブルクォートを採用しているところは要注意かもしれません。
ただ、上記のような理由を読むと、ダブルクォートを使う方が理にかなっているような気がします。


方法 1) Black Formatter 拡張を使う方法
----

Microsoft が提供している [Black Formatter 拡張](https://marketplace.visualstudio.com/items?itemName=ms-python.black-formatter) をインストールすると、簡単に Python コードの自動フォーマットを行えるようになります。
この拡張自体に `black` がバンドルされているため、外部ツールとして `black` コマンドをインストールする必要はありません（もし `pip install black` でインストールされていたら、そちらの `black` が優先的に使用されます）。

### Black Formatter 拡張のインストール

VS Code の 拡張メニュー (`Cmd(Ctrl) + Shift + X`) から、Black Formatter を検索して `Install` ボタンを押すだけでインストール完了です。

{{< image border="true" src="img-001.png" title="VS Code への Black Formatter 拡張のインストール" >}}

### 自動フォーマット設定

VS Code のデフォルトフォーマッターとして、Black Formatter を使用するように設定します。

`Cmd(Ctrl) + Shift + P` → __`Preferences: Open User Settings (JSON)`__ で VS Code の設定ファイル (`settings.json`) ファイルを開き、次のように設定してください。

{{< code lang="js" title="settings.json" >}}
{
  // Python ファイル用の設定
  "[python]": {
    // VS Code のフォーマッターとして Black Formatter 拡張を使う
    "editor.defaultFormatter": "ms-python.black-formatter",
    // ファイル保存時に自動でフォーマットする
    "editor.formatOnSave": true
  },

  // Python 拡張が提供するフォーマッター連携は無効にしておく
  "python.formatting.provider": "none",

  // black にコマンドライン引数を渡す場合（Black Formatter 拡張用）
  // "black-formatter.args": ["--line-length", "100"],

  // ...
}
{{< /code >}}

これで、Python のソースコードを保存したときに、自動で Black Formatter 拡張によるフォーマットがかかるようになります。


方法 2) Python 拡張から外部の black コマンドを使う方法
----

VS Code の [Python 拡張](https://marketplace.visualstudio.com/items?itemName=ms-python.python) にも、Python コードのフォーマット機能があります。
ただし、こちらは外部のツールを呼び出す形になっているため、`black` コマンドを単独でインストールしておく必要があります。

### black コマンドのインストール

`black` コマンドは `pip` で簡単にインストールできます。
システムの Python 環境を汚したくない場合は、[venv などの仮想環境を作成](https://maku77.github.io/p/wozpogm/) してからインストールしましょう。

```console
$ python3 -m pip install black

（または）

$ pip install black
```

### 自動フォーマット設定

VS Code 自体のデフォルトフォーマッター (`editor.defaultFormatter`) を無効にし、Python 拡張が提供するフォーマット機能 (`python.formatting.provider`) を有効にします。
ここでは、外部ツールとしてインストールした `black` コマンドを参照するように設定します（参考: [Editing Python Code in Visual Studio Code - Formatting](https://code.visualstudio.com/docs/python/editing#_formatting)）。

`Cmd(Ctrl) + Shift + P` → __`Preferences: Open User Settings (JSON)`__ で VS Code の設定ファイル (`settings.json`) ファイルを開き、次のように設定してください。

{{< code lang="js" title="settings.json" >}}
{
  // Python ファイル用の設定
  "[python]": {
    // VS Code のフォーマッターは無効にしておく
    "editor.defaultFormatter": null,
    // ファイル保存時に自動でフォーマットする
    "editor.formatOnSave": true
  },

  // Python 拡張が使用する外部フォーマッターとして black コマンドを指定する
  // （black コマンドは pip install black でインストールしておく）
  "python.formatting.provider": "black",

  // black にコマンドライン引数を渡す場合（Python 拡張用）
  // "python.formatting.blackArgs": ["--line-length", "100"],

  // ...
}
{{< /code >}}

ちなみに、`"editor.defaultFormatter": null` という設定は、別の場所でデフォルトフォーマッターを次のような感じで設定している場合に、それを無効化するために必要になります （Prettier は Python コードのフォーマットには対応していません）。

{{< code lang="js" title="settins.json" >}}
{
  // VS Code のデフォルトフォーマッターとして Prettier を使う
  "editor.defaultFormatter": "esbenp.prettier-vscode",

  // ...
}
{{< /code >}}

これで、Python のソースコードを保存したときに、自動で `black` コマンドによるフォーマットがかかるようになります。

### トラブルシューティング（venv 環境で自動フォーマットが効かない場合）

`black` を Python 仮想環境 (venv) にインストールしたときに、VS Code 上で `black` による自動フォーマットが効かないときは、`python` コマンド自身が venv 内のものに切り替わっているかを確認してください。
コマンドパレット (__`Cmd/Ctrl + Shift + P`__) から、__Python: Select Interpreter__ を選択するか、VS Code の右下に表示されているインタープリター名（`3.10.11` など）をクリックすることで、venv 内の `python` に切り替えることができます。


（応用）GitHub の README.md に Black バッジを表示する
----

Python プロジェクトに Black を適用したら、GitHub リポジトリの `README.md` の先頭に下記のコードを入れて、Black バッジを表示しておきましょう。

```md
[![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black)
```

ちゃんとフォーマットを意識してコードを記述していることをアピールできます。

[![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black) ← バッジ表示


（応用）Black によるフォーマットを無効化する
----

配列で行列データを表現する場合など、Black によるフォーマットを抑制したいことがあります。
そのような場合は、__`# fmt: off`__ と __`# fmt: on`__ で囲みます（同じインデントレベルに記述してください）。

```python
# fmt: off
matrix = {
    0,  1,  2,
    3,  4,  5,
    6,  7,  8,
}
# fmt: on
```

1 行だけに適用したいときは、行末コメントで __`# fmt: skip`__ と指定できます。

