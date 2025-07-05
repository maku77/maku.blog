---
title: "Jekyll と Hugo を共存させて GitHub Actions でビルドする"
url: "p/qbmv6fp/"
permalink: "p/qbmv6fp/"
date: "2022-08-17"
tags: ["Hugo", "Jekyll", "GitHub Actions", "GitHub Pages"]
---

何をするか？
----

こことは違う Web サイト (GitHub Pages) の話ですが、いい加減 [Jekyll での Web サイト管理は限界](/p/xwfwdy6/) になってきたので、重い腰を上げて全面的に Hugo に乗り換えようと思います。
待っていれば Jekyll も高速化されるかなという淡い期待を抱いて 3 年が経ちましたが、一向にその気配はないので、残念ですがここで Jekyll は見限ります（決して Ruby と Golang の速度の差だけだとは思いませんが、やはり Golang 製のツールの方が高速な傾向はあるのかも）。

とはいえ、現状 Jekyll で管理している Web ページは何千ページにも膨れ上がっており、一気に乗り換えるのは困難です。
そこで、Jekyll 運用されている Web サイトに、少しずつ Hugo 管理のページを加えていく、という作戦を取りたいと思います。
ドメイン名はそのまま `xxx.github.io` を使いたいので、ホスティングはこれまで通り GitHub Pages で行い、ビルドやデプロイには GitHub Actions を使うことにします。


基本方針
----

GitHub Pages のデフォルトの仕組みとして、Markdown ファイルをソース用リポジトリに置いておくだけで、Jekyll で HTML に変換して公開してくれるというのがあるのですが、今回はこの仕組みは使えません。
なぜなら、Jekyll によるビルド結果と、Hugo によるビルド結果をマージしたものをデプロイしなければいけないからです。
イメージとしては、GitHub リポジトリに push したときに、GitHub Actions で次のような処理を実行できればうまくいくはずです。

{{< image src="img-001.drawio.svg" title="GitHub Actions でやること" >}}

1. Jekyll プロジェクトの Markdown ファイルをビルドする
   - Jekyll のビルド対象から Hugo のディレクトリを除外しておきます
2. Hugo プロジェクトの Markdown ファイルをビルドする
3. Hugo プロジェクトのビルド結果を Jekyll プロジェクトのビルド結果にマージする
   - 同名のファイルは上書きしないことにします（先輩である Jekyll の出力を優先）
4. GitHub Pages のホスティング先にデプロイ
   - ここでは `gh-pages` ブランチを公開ブランチとして使います

Hugo のプロジェクトをフラットに配置してしまうと、`content` ディレクトリや `layouts` ディレクトリが混在してわかりにくくなるので、Hugo 側のファイル群は、`hugo-files` ディレクトリを作ってそこに配置することにします。
最終的に、Jekyll から Hugo への以降がすべて完了したら、この中のファイル群をルートに移動させる想定です。


設定作業
----

GitHub Actions を設定し、Jekyll と Hugo の共存プロジェクトをビルドできるようにしていきます。

### gh-pages をデプロイ先に設定

GitHub Pages のソースディレクトリが `gh-pages` ブランチになっていない場合は、対象の GitHub リポジトリのページから `Settings` → `Pages` と辿り、次のように変更しておきます。

- Source: __`Deploy from a branch`__
- Branch: __`gh-pages`__

ブランチの選択肢に `gh-pages` が出てこない場合は、次のように作成しておきます。

```console
# 親なしのブランチとして gh-pages を作成する
$ git switch --orphan gh-pages

# 適当な index.html をコミット
$ echo Hello > index.html
$ git add index.html
$ git commit -m "First commit"

# GitHub へ gh-pages ブランチをプッシュ
$ git push --set-upstream origin gh-pages
```

### GitHub Actions で Jekyll ビルド

GitHub Actions で Jekyll サイトをビルドする設定の例は、[Jekyll の GitHub Actions 説明ページ](https://jekyllrb.com/docs/continuous-integration/github-actions/) に一応書かれていますが、今回のように Jekyll と Hugo を共存させる場合には同じ設定は使えないので参考程度にとどめます。

GitHub Pages + Jekyll の環境で Web サイトを構築している場合は、RubyGems の Bundler 設定 (`Gemfile`) は次のような感じになっていると思います。
ここはおそらくそのままで大丈夫です。

{{< code lang="ruby" title="Gemfile" >}}
source "https://rubygems.org"

gem "github-pages", group: :jekyll_plugins

# ...省略...
{{< /code >}}

GitHub Actions 用の Workflow ファイルを作成し、main ブランチへのプッシュをトリガーとして Jekyll ビルドが自動実行されるようにします。
この段階では Hugo のビルドは行わず、まずは Jekyll のビルドとデプロイまでを設定します。
一歩ずつ一歩ずつ…。

{{< code lang="yaml" title=".github/workflows/github-pages.yml" >}}
name: Build and deploy Jekyll with Hugo site to GitHub Pages

on:
  push:
    branches: [main, master]

jobs:
  github-pages:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
        with:
          submodules: true  # Fetch Hugo themes (true OR recursive)
          fetch-depth: 0    # Fetch all history for .GitInfo and .Lastmod

      - name: Install Ruby and Gems
        uses: ruby/setup-ruby@v1
        with:
          ruby-version: '3.0' # Not needed with a .ruby-version file
          bundler-cache: true # Runs 'bundle install' and caches installed gems automatically

      - name: Build with Jekyll
        run: JEKYLL_ENV=production bundle exec jekyll build

      - name: Deploy to GitHub Pages
        uses: peaceiris/actions-gh-pages@v3
        with:
          github_token: ${{ secrets.GITHUB_TOKEN }}
          publish_dir: _site # Default is public
{{< /code >}}

GitHub Pages のデフォルトの振る舞いでは、ソースブランチ（今回は `gh-pages`）内のファイルを自動的に Jekyll で処理しようとします。
今回は、GitHub Actions で明示的にビルドするので、これを抑制するために `gh-pages` ブランチのルートに [`.nojekyll` ファイルをおく必要がある](/p/my8fmsy/) のですが、上記の例のように `peaceiris/actions-gh-pages` を使っている場合は、自動的にこの処理を行ってくれます。

ここまで準備できたら、GitHub にプッシュして GitHub Actions が起動するかを確認します。
Jekyll ビルドとデプロイが完了したら、Web サイトにアクセスできるようになっているはずです。

### GitHub Actions で Hugo ビルド

いよいよ、Jekyll のプロジェクトの中に、Hugo のプロジェクトを共存させます。

リポジトリのルートに、__`hugo-files`__ というディレクトリを作って、Hugo プロジェクトはそこで管理していきます。
共存させたい Hugo プロジェクトがすでに存在する場合は、そのディレクトリごとリポジトリルートにコピーして、`hugo-files` という名前にリネームしておきます。
Hugo プロジェクトがまだ存在しない場合は、次のような感じで作成します。

```console
$ hugo new site hugo-files
```

Hugo コンテンツの具体的な作り方はここでは割愛しますが、シンプルな構成例を下記 GitHub リポジトリにコミットしてあるので参考にしてください。

- [jekyll-with-hugo](https://github.com/maku77/jekyll-with-hugo)

`hugo-files` という Hugo 管轄のディレクトリを追加したので、Jekyll ビルド時には、このディレクトリを無視するように設定しておきます。

{{< code lang="yaml" title="_config.yml（Jekyll の設定ファイル）" >}}
# ...（省略）...
exclude:
  - .jekyll-cache/
  - .sass-cache/
  - Gemfile
  - Gemfile.lock
  - gemfiles/
  - hugo-files/
  - node_modules/
  - vendor/
{{< /code >}}

GitHub Actions の Workflow ファイルを修正して、Hugo のインストールとビルドを行うようにします。
ここでは、いったん Hugo 用のビルド結果はデフォルトのまま `hugo-files/public` ディレクトリに出力しておき、ビルド後にリポジトリルートの `_site` ディレクトリに中身をマージすることにします。
このように段階的に出力結果をマージすることで、同名ファイルの扱いを制御しやすくなります。

{{< code lang="yaml" title=".github/workflows/github-pages.yml（抜粋）" >}}
- name: Install Hugo
  uses: peaceiris/actions-hugo@v2
  with:
    hugo-version: '0.101.0'
    extended: true  # Enable scss

- name: Build with Hugo
  run: hugo --minify
  working-directory: hugo-files

- name: Merge build results without overriding
  run: cp -RTn hugo-files/public _site
{{< /code >}}

Linux の `cp` コマンドで `-RT` オプションを指定することで、`hugo-files/public` ディレクトリの中身だけをまるごと `_site` へコピーしています（ちなみに、macOS の `cp` コマンドの場合は BSD 版なので `-T` オプションが存在せず、代わりに `cp -Rn hugo-files/public/ _site` のように、src ディレクトリ名に `/` サフィックスを付けます）。
さらに、`cp` コマンドの `-n` オプションを指定することで、コピー先に同名のファイルがある場合に上書きしないようにしています。
おそらく、トップページ用の `index.html` は両方のビルド結果として出力されていますが、今回は Jekyll の方のファイルを採用することになります（Hugo の方のトップページを使わないのであれば、最初から [`hugo` コマンドのオプション](https://gohugo.io/commands/hugo/) で `--disableKinds home` を指定して出力しないのがよいかもしれません）。

サイトマップファイル (`sitemap.xml`) を両方のプロジェクトで生成している場合は、どちらのファイルもデプロイしたいので、次のような感じで Hugo 側のファイルをリネームしてコピーしておきます。
サイトマップファイルが 2 つに分かれるので、Google Search Console などでサイトマップを送信する場合は、2 つの XML ファイル（`sitemap.xml` と `sitemap-hugo.xml`）を指定することになります。

```yaml
- name: Rename and copy Hugo's sitemap.xml
  run: cp hugo-files/public/sitemap.xml _site/sitemap-hugo.xml
```

あるいは、Hugo と Jekyll が出力した sitemap.xml を以下のようにマージしてしまった方がよいかもしれません。

```yaml
- name: Merge sitemap.xml files
  run: |
    SITEMAP_JEKYLL="_site/sitemap.xml"
    SITEMAP_HUGO="hugo-files/public/sitemap.xml"
    SITEMAP_MERGED="sitemap-merged.xml"
    echo '<urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">' > $SITEMAP_MERGED
    perl -0777 -ne 'print "$1\n" while /(<url>.*?<\/url>)/gs' $SITEMAP_JEKYLL >> $SITEMAP_MERGED
    perl -0777 -ne 'print "$1\n" while /(<url>.*?<\/url>)/gs' $SITEMAP_HUGO >> $SITEMAP_MERGED
    echo '</urlset>' >> $SITEMAP_MERGED
    mv $SITEMAP_MERGED $SITEMAP_JEKYLL
```

これで、Hugo と Jekyll の共存環境は完成です。
GitHub リポジトリに Markdown コンテンツをプッシュするたびに、両方のビルド結果がマージされてホスティングされるようになります。
最終的な [Workflow ファイルはこちら](https://github.com/maku77/jekyll-with-hugo/blob/main/.github/workflows/github-pages.yml) を参照してください。


（おまけ）普段の記事の執筆中はどうするか？
----

GitHub Actions で Jekyll と Hugo を同時にビルドするときはよいのですが、ローカルサーバーを起動して記事を執筆しているときはどうやって共存させればよいでしょうか？
デフォルトでは、Jekyll のローカルサーバー (`bundle exec jekyll serve`) は 4000 番ポートで動作し、Hugo のローカルサーバー (`hugo serve`) は 1313 番ポートで動作します。
`-p` オプションでポート番号を変更することはできますが、TCP/IP の仕様上、同じポート番号でサーバーを立ち上げることはできません。
よって、どちらかのサーバーに相乗りする形で動作させる必要があります。

ここでは、Jekyll のローカルサーバーが `_site` ディレクトリを使う性質を利用して、Hugo のコンテンツをそこに随時放り込むという方法をとってみます。
まず、Jekyll のローカルサーバーを起動します。

{{< code lang="console" title="Jekyll サーバーを起動 (http://localhost:4000)" >}}
$ bundle exec jekyll serve --baseurl ""
{{< /code >}}

ここでは、`--baseurl ""` オプションを指定して、`http://localhost:4000` というシンプルな URL でトップページにアクセスできるようにしています。
これは、GitHub のリポジトリ名に合わせて、Jekyll の設定ファイル (`_config.yml`) で `baseurl: "/jekyll-with-hugo"` のような設定をしているからです。
baseurl をクリアしておかないと、トップページにアクセスするときに、`http://localhost:4000/jekyll-with-hugo/` という URL を使わないといけません。

次に、Hugo のビルドコマンドを、Watch モードで起動します。
Hugo はサーバーとしては起動しないところがポイントです。

{{< code lang="console" title="Hugo ビルドを Watch モードで起動" >}}
$ cd hugo-files
$ hugo --watch --destination ../_site --disableKinds home
{{< /code >}}

`-w` (`--watch`) オプションを付けて Hugo ビルドを実行すると、記事ディレクトリ (`content`) を監視して随時ビルドをかけてくれるようになります。
そのビルド結果は `--destination ../_site` という指定により、Jekyll がホスティングしているディレクトリ (`../_site`) にマージされていきます。
さらに、`--disableKinds home` というオプションで、Hugo 側のトップページ (`index.html`) の出力を抑制し、Jekyll 側のトップページが上書きされてしまうのを防いでいます。

これで、ブラウザで `http://localhost:4000` を開いたまま、記事の執筆を進めることができます。
ただ、ちょっと強引な組み合わせ方をしているので、いくつか問題もあります。

- Jekyll 側の記事を更新すると `_site` 内の Hugo 記事が消されてしまう
  - これは Jekyll が出力をきれいにしてくれるための機能なので、この振る舞いを変えることはできないみたいです。もし、Hugo 側の記事が必ず `p` ディレクトリ以下に出力されるなど決まっているのであれば、`_config.yml` に `keep_files: ["p"]` と記述しておけば、それらの記事だけは消去されずに済みます。
- Live Reload などの機能が使えない
  - Jekyll の出力先ディレクトリに直接ファイルを突っ込んでいるので、記事の修正を検出してくれないのは仕方ないですね…。

まぁこのあたりは、Hugo への完全移行が終わるまでの辛抱です。

