---
title: "VS Code で Python 用の Linter ＆フォーマッターの Ruff を使う"
url: "p/6hnm4hy/"
date: "2024-09-19"
tags: ["Python", "Ruff"]
---

Ruff とは
----

[Ruff](https://docs.astral.sh/ruff/) は Python 用の Lint & Format ツールで、Rust で記述されているため非常に高速に動作します。
__Ruff だけで Lint やフォーマットをまとめてかけることができる__ ため、次のようなツールを個別にインストールする必要がなくなります。

- Flake8（Linter）
- Black（フォーマッター）
- isort（import 文のソート）

これまで、Python コードに Lint や Format をかける場合は、複数のツールを組み合わせて使うのが主流で、効率的な処理が行われているとは言いにくい状況でした。
例えば、広く使われている Lint ツールに Flake8 がありますが、これは `pyflakes` や `pycodestyle` の組み合わせであり、内部で何度もファイルの読み込みやパースが行われるため処理に時間がかかっていました。
Ruff は様々な処理が一度で済むように実装されています。
同様に広く使われているフォーマットツールとして Black がありますが、よくよく考えると、Black でフォーマット済みのコードを別の Lint ツール (Flake8) でスタイルチェックするのは無駄です。
Ruff で Lint とフォーマットの処理を一括で行うのは理にかなっています。

ちなみに、Ruff の開発元である Astral 社は、Python 用のパッケージマネージャ（プロジェクト管理ツール）である `uv` も公開しています。
こちらも Rust で実装されており、高速かつ使いやすいのでオススメです。

- 参考: [Python プロジェクト用の爆速パッケージマネージャ uv を導入する - まくまく Python ノート](https://maku77.github.io/p/fjsfjpw/)


VS Code に Ruff 拡張をインストールする
----

Visual Studio Code で Ruff を使うには、下記の公式拡張をインストールするだけです。
`ruff` コマンドを別途インストールする必要はありません。

- [Ruff - Visual Studio Marketplace](https://marketplace.visualstudio.com/items?itemName=charliermarsh.ruff)

Ruff 拡張のインストールが完了すると、Python コードの編集時に自動的に Lint チェックがかかるようになります。
Python コードのフォーマットをかけるには、コマンドパレット (`Shift` + `Cmd/Ctrl` + `P`) から __`Ruff: Format document`__ を選択します。

ファイル保存時に自動でフォーマットをかけるようにするには、VS Code の設定ファイルに次のように記述しておきます（参考: [VS Code の設定ファイルの場所 (settings.json)](/p/tfq2cnw/)）。

{{< code lang="js" title="settings.json（抜粋）" >}}
"[python]": {
  // フォーマッターとして Ruff を使う
  "editor.defaultFormatter": "charliermarsh.ruff",
  // ファイル保存時に自動でフォーマットをかける
  "editor.formatOnSave": true,
  "editor.codeActionsOnSave": {
    // ファイル保存時に Lint violations を自動で修正する
    "source.fixAll.ruff": "explicit",
    // ファイル保存時に自動でインポートを整理する
    "source.organizeImports.ruff": "explicit"
  }
},
{{< /code >}}

