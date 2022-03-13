---
title: "Next.js アプリを GitHub Actions でビルドして GitHub Pages で公開する"
url: "/p/au8ju6g"
date: "2021-04-24"
tags: ["GitHub Actions", "Next.js", "React"]
weight: 104
---

何をするか？
----

ここでは、Next.js アプリを次のような構成でホスティング（Web サイトとして公開）できるようにします。

- `create-next-app` で作成した Next.js アプリのコードを GitHub で管理する
- `main` ブランチにプッシュ（マージ）したら __GitHub Actions__ で下記を実行
    - Next.js アプリをビルド (`npm run build`, `npm run export`)
    - __GitHub Pages__ にデプロイ

つまり、この設定が終わると、GitHub に Next.js アプリのコードをプッシュするだけで、自動的にビルドされて、Web サイトに反映されるようになります。


GitHub リポジトリの準備
----

空の GitHub リポジトリを作成し、そこに `create-next-app` で作成した Next.js アプリをプッシュします。
リポジトリ名は、最終的な Web サイトのアドレスをどうしたいかで、次のように作り分けます。

- ユーザーサイト（あるいは Organization サイト）
    - 公開 URL:  `https://<username>.github.io/`
    - 作成するリポジトリ名: __`<username>.github.io`__
- プロジェクトサイト
    - 公開 URL: `https://<username>.github.io/<repository>/`
    - 作成するリポジトリ名: __`<repository>`__

ここでは、プロジェクトサイトとして `myapp` というリポジトリを作成した場合の例で説明していきます。

{{< code >}}
$ npx create-next-app myapp
$ cd myapp
$ git remote add origin https://github.com/<username>/myapp.git
$ git push -u origin main
{{< /code >}}

上記のように実行すると、初期状態の Next.js アプリが GitHub にプッシュされます。

{{% reference %}}
- [Next.js アプリのソースコードを GitHub で管理する](/p/xjw8it5)
{{% /reference %}}


package.json および next.config.js の修正
----

### package.json

__`package.json`__ を修正して、Next.js の静的 HTML のエクスポート機能 (`next export`) を NPM スクリプト経由で実行できるようにしておきます。
これは、後述の GitHub Actions のビルドスクリプトから `npm run export` で起動できるようにするためです。

{{< code lang="js" >}}
...
"scripts": {
  "dev": "next dev",
  "build": "next build",
  "export": "next export",  // これを追加
  "start": "next start"
}
...
{{< /code >}}

修正したら、コミット＆プッシュしておきます。

{{< code >}}
$ git commit -m "Add export script" .
$ git push
{{< /code >}}

### next.config.js

GitHub Pages のサイトを「プロジェクトサイト」として作成する場合、つまり、`https://<username>.github.io/<repository>/` という URL で公開する場合は、トップページがドメインルートではなくなります。
そのままだと、Next.js アプリ内の JS ファイルや CSS ファイルが軒並み 404 Not Found になってしまうので、Next.js ビルド用に追加のプレフィックス設定 (__`assetPrefix`__) が必要です。
さらに、`Link` (next/link) や `Router` (next/router) によるリンクのベースパスとしては、__`basePath`__ の設定の方が反映されるので、こちらも合わせて設定が必要です。

次の `next.conf.js` の記述例では、環境変数 __`URL_PREFIX`__ が設定されているときに、その値を URL のプレフィックスとして使用するように設定しています（先頭にスラッシュ `/` を付けないとうまくいかないようなので注意）。
この環境変数は、後ほど GitHub Actions のビルドスクリプトの中で、リポジトリ名に合わせて設定します。

{{< code lang="js" title="next.config.js（新規作成）" >}}
const urlPrefix = process.env.URL_PREFIX ? '/' + process.env.URL_PREFIX : ''

module.exports = {
  // ...
  assetPrefix: urlPrefix,
  basePath: urlPrefix,
  trailingSlash: true,
  // ...
};
{{< /code >}}

また、__`trailingSlash`__ を true に設定しておくことで、`pages/aaa.js` というコードが、`aaa.html` というファイルではなく、`aaa/index.html` というファイルとして出力されるようになります。
これにより、Web ブラウザに直接 `https://<username>.github.io/myapp/aaa/` といった URL を入力したときにも正しくページが表示されるようになります（Web ブラウザはこのような URL を入力すると、`aaa/index.html` を探しに行くので、`aaa.html` というファイルを配置するとうまく動作しません）。
トップページからリンクを辿っていくだけであればデフォルト状態でも動作するのですが、特定のページでリロードやブックマークできるようにするにはこの設定が必要です。

ちなみに、クライアントサイド JS からこういった URL プレフィックス情報を直接参照するには、ちょっとした工夫が必要です（Next.js の `Image` コンポーネントなどを使う場合は気にする必要はありません）。

- 参考: [Next.js の public 以下のファイルのパスを正しく扱う](/p/xjjbwes)


GitHub Actions の設定
----

GitHub リポジトリのページで __Actions__ タブを選択し、次のように新規ワークフローを作成します。

1. __`set up a workflow yourself`__ をクリック
2. YAML ファイルの編集画面になるので、次のように入力して __`Start commit`__ をクリック

各ステップで何をしているかは、`name` プロパティを読めば大体わかると思います。

{{< code lang="yaml" title="myapp/.github/workflows/main.yml" >}}
name: Deploy Next.js app

on:
  push:
    branches: [ main ]
  pull_request:
    branches: [ main ]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - name: Checkout repository
        uses: actions/checkout@v2

      - name: Setup Node.js
        uses: actions/setup-node@v2
        with:
          node-version: 14.x

      - name: Install NPM packages
        run: npm ci

      - name: Build Next.js app
        run: npm run build
        env:
          URL_PREFIX: myapp

      - name: Export Next.js app
        run: npm run export

      - name: Deploy to GitHub Pages
        uses: peaceiris/actions-gh-pages@v3
        with:
          github_token: ${{ secrets.GITHUB_TOKEN }}
          publish_dir: out
{{< /code >}}

`npm run build` の環境変数 `URL_PREFIX` の値 (`myapp`) は、GitHub リポジトリ名に合わせて修正してください。
この値は、前述の `next.config.js` の中で参照しています。
ただし、GitHub Pages をユーザーサイトとして作成している場合（リポジトリ名が `<username>.github.io` の場合）は、この環境変数は必要ありません。

肝心の GitHub Pages 用のデプロイには、[actions-gh-pages](https://github.com/peaceiris/actions-gh-pages) を使用しています。
このアクションは、`publish_dir` で指定したディレクトリ（上記では `out`）の中身を、`gh-pages` ブランチにコミットしてくれます。
ここで `out` ディレクトリを指定しているのは、`npm run export` (`next export`) のデフォルトの出力先が `out` ディレクトリになっているからです。

`main.yml` のコミットが完了したら、自動的に GitHub Actions によるビルドが開始され、`Actions` タブからビルドの状況を確認できます。
数十秒くらいしてビルドが完了すると、GitHub リポジトリに `gh-pages` ブランチが生成されて、そこに Web サイトのコンテンツ（`npm run export` で生成した `out` ディレクトリの内容）が格納されていることを確認できます。

今後は、GitHub にソースコードをプッシュするたびに、このビルドプロセスが自動的に実行されます。


GitHub Pages を有効にする
----

最後に、GitHub Pages 機能を有効にして、`gh-pages` ブランチの内容を Web サイトの形で見えるようにします。

1. GitHub リポジトリのページから __`Settings → Pages`__ と選択して GitHub Pages の設定画面を開きます
2. __`Source`__ の項目でブランチ __`gh-pages`__ を選択して __`Save`__ ボタンを押します
3. `Enforce HTTPS` のチェックも入れておきましょう（`http` によるアクセスが自動で `https` にリダイレクトされます）

しばらくして、`https://<username>.github.io/myapp/` にアクセスして、Next.js アプリのトップページが表示されれば成功です！

{{% reference %}}
- [GitHub Actions で Web サイトをビルドして GitHub Pages へ公開する](/p/5q3eq2c)
{{% /reference %}}

