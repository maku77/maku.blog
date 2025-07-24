---
title: "Parcel でビルドした Web サイトが GitHub Pages で動作しないとき"
url: "/p/m7jw8ju"
date: "2020-09-01"
tags: ["GitHub Pages", "Parcel"]
---

React アプリを Parcel でビルドして GitHub Pages で公開しようとしたら、真っ白な画面になってしまったので対応方法のメモです。
原因は、トップページの `index.html` から読み込んでいる JavaScript ファイルのパスが、`/index.js` のように、ドメインルートからの相対パスになっていることでした。

GitHub Pages で公開する Web サイトの URL が、

- `https://yourname.github.io/`

であれば問題ないのですが、

- `https://yourname.github.io/project名/`

になっている場合は、読み込むファイルは `/project名/index.js` になっていないといけません。
これを解決するには、例えば、Parcel でビルドしているときは __`--public-url`__ オプションを使って、次のように指定します。

{{< code >}}
$ parcel build src/index.html -d build --public-url "/project名/"
{{< /code >}}

こうすることで、トップページから `/index.js` というパスで参照していたものが `/project名/index.js` に変わってうまく動作するようになります。

`npm run build` で Web サイトのビルドを行っているのであれば、`package.json` 内のスクリプト定義で次のように記述しておけばよいでしょう。

{{< code lang="js" title="package.json" >}}
{
  // ...
  "scripts": {
    "start": "parcel src/index.html --open",
    "build": "rm -rf build && parcel build src/index.html -d build --public-url '/project名/'"
  },
  // ...
}
{{< /code >}}

NPM スクリプト内に GitHub のプロジェクト名を含めるのが嫌なときは、CI/CD サーバー上のビルド設定でパラメータ指定するのがよいかもしれません。
例えば、GitHub Actions を使っている場合は、次のような感じで NPM スクリプトに追加パラメータを指定することができます。

{{< code lang="yaml" title=".github/workflows/main.yml（抜粋）" >}}
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
    ...
    - name: Build website
      run: npm run build --if-present -- --public-url '/project名/'
{{< /code >}}

`build` スクリプトの実行内容に対してパラメータを渡すには、上記のように __`--`__ の後ろに続けてパラメータを指定しないといけないことに注意してください。
こうしないと、`--public-url` オプションは、`npm` コマンド自体のオプションとして処理されてしまいます。

