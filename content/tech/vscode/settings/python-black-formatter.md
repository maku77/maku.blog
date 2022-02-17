---
title: "VS Code で Python 用のフォーマッター (Black) を使う"
url: "/p/4oybku6"
date: "2021-11-13"
tags: ["VS Code", "Python"]
---

何をするか？
----

Visual Studio Code に [Python 拡張](https://marketplace.visualstudio.com/items?itemName=ms-python.python) を入れることで、Python コードの編集（自動補完など）ができるようになりますが、コードの自動フォーマットを行うには、フォーマッターを別途インストールしておく必要があります（いろんなフォーマッターがあるため）。

Python のフォーマッターとしては、autopep8 や yapf などもありますが、ここでは、最近人気がある [Black](https://black.readthedocs.io/) をインストールして VS Code の自動フォーマッターとして設定します。
Facebook や Dropbox、Mozilla などでも導入されており、採用実績としては申し分なさそうです。

{{% note title="頑固なフォーマッター" %}}
Black は自分自身を The __uncompromising__ code formatter と説明しています。
ようするに、「私が定義する設定に従いなさい」という意味で、ユーザーにほとんど設定の余地を残していません（行の長さ程度は設定できます）。
このようにすることで、チーム内でのフォーマット論争を防ぐ効果があります。
TypeScript (JavaScript) でよく使われている Prettier フォーマッターも同様の思想で作られており、ほとんど設定ができないようになっています（こちらは __opinionated__ という単語を使っています）。

- 参考: [TypeScript コードを Prettier で自動整形する](/p/au8iu6u)
{{% /note %}}


Black ではどのようなスタイルになるか？
----

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


Black のインストール
----

Black は pip で簡単にインストールできます。

```
$ python3 -m pip install black
```


VS Code の自動フォーマット設定
----

`Cmd(Ctrl) + Shift + P` → __`Preferences: Open Settings (JSON)`__ で `settings.json` フィアルを開き、次のように設定します。

ここでは、Python 用のフォーマッターとして `black` を使用することと、ファイルの保存時に自動でフォーマットを実行することを指定しています。

{{< code lang="js" title="settings.json" >}}
{
  // Python コードを black でフォーマットする設定
  // （Python 拡張をインストールして pip install black しておく）
  "python.formatting.provider": "black",
  "[python]": {
    "editor.defaultFormatter": null, // Prettier を使わないようにする
    "editor.formatOnSave": true // ファイル保存時に自動フォーマット
  },

  // ...
}
{{< /code >}}

ちなみに、`"editor.defaultFormatter": null` という設定は、デフォルトのフォーマッターとして次のように Prettier などを設定している場合に必要になります （Prettier は Python コードのフォーマットには対応していません）。

{{< code lang="js" title="settins.json" >}}
{
  "editor.defaultFormatter": "esbenp.prettier-vscode", // Prettier を使う
  // ...
}
{{< /code >}}

- 参考: [Editing Python Code in Visual Studio Code - Formatting](https://code.visualstudio.com/docs/python/editing#_formatting)


Black によるフォーマットを無効化する
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


GitHub の README.md に Black バッジを表示する
----

Python プロジェクトに Black を適用したら、GitHub リポジトリの `README.md` の先頭に下記のコードを入れて、Black バッジを表示しておきましょう。

[![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black)

```md
[![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black)
```

