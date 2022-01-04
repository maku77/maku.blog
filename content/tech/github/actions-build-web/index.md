---
title: "GitHub Actions で Web サイトをビルドして GitHub Pages へ公開する"
url: "/p/5q3eq2c"
date: "2020-09-01"
lastmod: "2020-04-07"
tags: ["GitHub", "GitHub Actions"]
---

何をするか？
----

GitHub Actions を使うと、GitHub で管理されている Web サイト用リソースの「ビルドとデプロイ」を簡単に自動化することができます。

ここでは、

1. `npm run build` による Web サイトのビルド
2. ビルドされたリソースの GitHub Pages へのデプロイ（`gh-pages` ブランチ）

を行う設定を行います。
最終的に、`https://yourname.github.io/project名/` という URL で Web サイトが公開されます。

前提条件として、GitHub リポジトリに __`npm run build`__ でビルド可能な Web サイト用リソースがコミットされているものとします。

{{< note title="他のビルド方法は？" >}}
ここでは、`npm run build` による Web サイトのビルドを前提としましたが、例えば、Hugo によるビルドなどもほぼ同様に行えます。
{{< /note >}}


GitHub Actions の設定
----

GitHub プロジェクトの __`Actions`__ タブから、次のように新規ワークフローを作成します。

1. __`set up a workflow yourself`__ をクリック
2. `main.yml` の編集画面になるので、次のように入力して __`Start commit`__ ボタンを押す

{{< code lang="yaml" title=".github/workflows/main.yml" >}}
name: Build and deploy website

on:
  push:
    branches: [ master ]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v2
    - name: Setup Node.js
      uses: actions/setup-node@v1
      with:
        node-version: 14.x
    - name: Install NPM packages
      run: npm ci
    - name: Build website
      run: npm run build --if-present
    - name: Deploy website
      uses: peaceiris/actions-gh-pages@v3
      with:
        github_token: ${{ secrets.GITHUB_TOKEN }}
        publish_dir: build
{{< /code >}}

これだけで、初回の Web サイトビルドが開始され、ビルド結果が `gh-pages` ブランチに格納されます。
今後は、`master` ブランチへのプッシュが行われるたびに、このビルドが実行されます。

ワークフローの内容は読めば簡単に理解できると思いますが、次のようなことをやっています。

1. ビルド環境として Ubuntu Linux の最新版を使用
1. master ブランチのソースコードをチェックアウト (`actions/checkout@v2`)
1. Node.js 14.x を使用するよう設定 (`actions/setup-node@v1`)
1. NPM パッケージをインストール (`npm ci`)
1. Web サイトをビルド (`npm run build`)
1. Web サイトをデプロイ (`peaceiris/actions-gh-pages@v3.6.4`)
   * 注: ビルド結果は __`build`__ ディレクトリに出力されているものとします

最後に、`gh-pages` ブランチの内容を Web サイトとしてホスティングするように、__`Settings`__ → __`Pages`__ から GitHub Pages を有効化します（2021-04 追記: 以前は Settings トップページ内に Pages の項目がありましたが、現在はサイドバーに Pages という独立した項目があります）。

{{< image border="true" src="img-001.png" >}}

これで、`https://yourname.github.io/project名/` にアクセスすれば、Web サイトが表示されます。
う〜ん、お手軽すぎる！

{{% reference %}}
- [Next.js アプリを GitHub Actions でビルドして GitHub Pages でホスティングする](/p/au8ju6g)
{{% /reference %}}

