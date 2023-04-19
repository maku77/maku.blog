---
title: "VS Code で YAML ファイルをソートする（YAML Sort 拡張）"
url: "p/2apzood/"
date: "2023-04-19"
tags: ["VS Code", "YAML"]
---

YAML Sort の概要
----

VS Code に [YAML Sort 拡張](https://marketplace.visualstudio.com/items?itemName=PascalReitermann93.vscode-yaml-sort) をインストールすると、YAML ファイルの内容をキー名でソートすることができます。
主にソート用に使うものですが、YAML フォーマッターとしても利用できます。
次のような簡単なカスタマイズを行うこともできます。

- 指定したキーを優先的に上から並べる
- 1 階層目の各キーの間に空白行を入れる
- 配列要素をソートする


YAML Sort の使い方
----

[YAML Sort](https://marketplace.visualstudio.com/items?itemName=PascalReitermann93.vscode-yaml-sort) のページで `Install` ボタンを押すだけで、VS Code への拡張のインストールは完了です。
ここでは、次のような YAML ファイルをソートしてみます。

{{< code lang="yaml" title="sample.yml（ソート前）" >}}
root2:
  ccc: 300
  aaa: 100
  bbb: 200

root1:
  fruits:
    - grape
    - apple
    - banana
  animals: ['wolf', 'fox', 'dolphin', 'eagle']
{{< /code >}}

VS Code で YAML ファイルを開いた状態で、コマンドパレットを開いて (__`Ctrl/Cmd + Shift + P`__)、__`YAML Sort: Sort YAML`__ を選択すると、YAML ファイルの内容がソートされます。

{{< code lang="yaml" title="sample.yml（ソート後）" >}}
---
root1:
  animals: ['wolf', 'fox', 'dolphin', 'eagle']
  fruits:
    - grape
    - apple
    - banana
root2:
  aaa: 100
  bbb: 200
  ccc: 300
{{< /code >}}

先頭行に自動でセパレーター (`---`) が挿入され、すべてのキーがアルファベット順にソートされていることが分かります。
キー間の空白行はすべて削除されています。
一方で、配列要素は自動ではソートされないようです（順番が意味を持つことがあるので当然ですが）。


YAML Sort のカスタマイズ
----

ソート方法は VS Code の設定ファイルでカスタマイズできるのですが、プロジェクトごと（あるいは YAML ファイルごと）に、ソート方法のルールは変わってくるはずなので、ワークスペース設定ファイル (__`<Project>/.vscode/settings.json`__) で設定するのがよいでしょう（参考: [VS Code の設定ファイルの場所](/p/tfq2cnw/)）。
ワークスペース設定ファイルは次のように開くことができます。

1. __`Ctrl/Cmd + Shift + P`__ でコマンドパレットを開く
2. __`Preferences: Open Workspace Settings (JSON)`__ を選択

設定可能な項目は [公式サイト](https://marketplace.visualstudio.com/items?itemName=PascalReitermann93.vscode-yaml-sort) を参照してください。
以下、いくつか設定例を示しておきます。

### 優先的に並べるキーを指定する (customSortKeywords_1)

{{< code lang="json" title="<Project>/.vscode/settings.json" >}}
{
  "vscode-yaml-sort.customSortKeywords_1": ["id", "name"]
}
{{< /code >}}

__`"vscode-yaml-sort.customSortKeywords_1"`__ というプロパティで、1 階層目のキーのソート順序を定義することができます。
残念ながら、2 階層目移行のキーは指定できないようです。
この設定を使ってソートするには、コマンドパレットから `YAML Sort: Sort YAML` を選択する代わりに、__`YAML Sort: Custom sort 1`__ を選択します。
プロパティ名の末尾の数字を `2`、`3` と変えることで、複数のカスタムソート設定を定義しておくことができます。

### キー間に空行を入れる (emptyLinesUntilLevel)

{{< code lang="json" title="<Project>/.vscode/settings.json" >}}
{
  "vscode-yaml-sort.emptyLinesUntilLevel": 1
}
{{< /code >}}

__`"vscode-yaml-sort.emptyLinesUntilLevel"`__ プロパティに 1 以上の数値を設定しておくと、その階層まで、各キー間に空白行が入るようになります。
上記の設定例の場合、ルート階層（1 階層目）のキー間に空白行が入ります。

