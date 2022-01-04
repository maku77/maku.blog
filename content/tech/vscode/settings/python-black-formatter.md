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

{{% note title="頑固なフォーマッター" %}}
Black は自分自身を The __uncompromising__ code formatter と説明しています。
ようするに、「私が定義する設定に従いなさい」という意味で、ユーザーにほとんど設定の余地を残していません（行の長さ程度は設定できます）。
このようにすることで、チーム内でのフォーマット論争を防ぐ効果があります。
TypeScript (JavaScript) でよく使われている Prettier フォーマッターも同様の思想で作られており、ほとんど設定ができないようになっています（こちらは __opinionated__ という単語を使っています）。

- 参考: [TypeScript コードを Prettier で自動整形する](/p/au8iu6u)
{{% /note %}}


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

