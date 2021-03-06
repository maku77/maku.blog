---
title: "GitHub Pages で Jekyll による変換を無効化する（プッシュした HTML をそのまま表示する）"
url: "/p/my8fmsy"
date: "2020-09-03"
tags: ["GitHub Pages", "Jekyll"]
---

GitHub プロジェクトの設定画面から、GitHub Pages を有効化すると、簡単にウェブサイトをホスティングすることができます。
デフォルトでは、リポジトリ内の Markdown ファイル (`.md`) が Jekyll によって HTML に変換されるのですが、直接 HTML ファイルをデプロイする場合は、この変換処理は無駄です。

そのような場合は、次のようにして Jekyll による変換を無効化できます。

1. リポジトリのルートに __`.nojekyll`__ ファイルを置く

これだけです。
公開用ブランチとして `gh-pages` を設定している場合は、そのブランチのルートに置いてください。

Hugo などの静的 Web サイト生成ツールで作成したサイトをデプロイするような場合は、この設定を行っておくと、ファイルのプッシュ後に GitHub Pages に反映されるまでの時間がほんの少し早くなるかもしれません。

速度を気にしない場合はこの設定をする必要はありませんが、プッシュする Web サイトリソース内に、アンダースコア (`_`) で始まるファイルやディレクトリなどが含まれている場合は、この設定は必須になります。
なぜなら、Jekyll がアンダースコアで始まるファイルやディレクトリを取り除いてしまうからです。

