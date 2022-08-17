---
title: "GitHub Pages で Jekyll による変換を無効化する（プッシュした HTML をそのまま表示する）"
url: "/p/my8fmsy"
date: "2020-09-03"
tags: ["GitHub Pages", "Jekyll"]
---

GitHub プロジェクトの設定画面から、GitHub Pages を有効化すると、簡単にウェブサイトをホスティングすることができます。
デフォルトでは、リポジトリ内の Markdown ファイル (`.md`) が Jekyll によって HTML に変換されるのですが、直接 HTML ファイルをデプロイする場合は、この変換処理は無駄です。

そのような場合は、次のようにして Jekyll による変換を無効化できます。

1. リポジトリのルート（GitHub Pages のソースディレクトリ）に __`.nojekyll`__ ファイルを置く

これだけです。

Hugo などの静的 Web サイト生成ツールで作成したサイトをデプロイするような場合は、この設定を行っておくと、ファイルのプッシュ後に GitHub Pages に反映されるまでの時間がほんの少し早くなるかもしれません。

速度を気にしない場合はこの設定をする必要はありませんが、プッシュする Web サイトリソース内に、アンダースコア (`_`) で始まるファイルやディレクトリなどが含まれている場合は、この設定は必須になります。
なぜなら、Jekyll がアンダースコアで始まるファイルやディレクトリを取り除いてしまうからです。

GitHub Pages のソースブランチとして `gh-pages` ブランチを設定している場合は、そのブランチのルートに `.nojekyll` を配置しなければいけないことに注意してください。
ちなみに、GitHub Actions でデプロイするときに、[peaceiris/actions-gh-pages](https://github.com/peaceiris/actions-gh-pages) を使っている場合は、[自動的にデプロイ先のブランチ (`gh-pages`) に `.nojekyll` ファイルを作成してくれます](https://github.com/peaceiris/actions-gh-pages#%EF%B8%8F-enable-built-in-jekyll-enable_jekyll)。

```yaml
- name: Deploy
  uses: peaceiris/actions-gh-pages@v3
  with:
    github_token: ${{ secrets.GITHUB_TOKEN }}
    publish_dir: ./public
```

