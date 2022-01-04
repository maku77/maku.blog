---
title: "AWS Lambda にデプロイするための ZIP パッケージを npm で作成する (npm-pack-zip)"
linkTitle: "Lambda にデプロイするための ZIP パッケージを npm で作成する (npm-pack-zip)"
url: "/p/zmydq3f"
date: "2021-03-20"
tags: ["AWS"]
---

何をするか？
----

AWS Lambda で実行する関数は、ZIP ファイルの形でデプロイすることになります。
ここでは、Node.js で関数を実装しているという前提で、`npm` スクリプトで簡単にデプロイ用の ZIP パッケージを作成する方法を説明します。

Lambda 関数用の ZIP パッケージを作成するときは、次のようなことを考慮する必要があります。

- __実行時に必要な `node_modules` 以下のモジュールを含める__（逆に `devDependencies` で指定したモジュールは含めない。例えば `typescript` とか `eslint` とかは含めない）
- AWS SDK (`aws-sdk`) は含めない（Lambda の実行環境にインストールされている）
- TypeScript を使っているのであれば、ビルド後の `.js` ファイルのみを含める（例えば、`src/*.ts` は含めず、`build/*.js` を含める）

{{< note title="AWS Lambda レイヤー" >}}
Lambda の「レイヤー」という機能を使うと、関数の実行に必要な `node_modules` を ZIP パッケージとは別に管理して、Lambda 関数から参照するということができます。
でも設定にひと手間かかります。AWS のこういうところは嫌いです。

ここでは、レイヤーの機能は使わずに、ZIP パッケージに全部入れちゃう方法を説明しています。
{{< /note >}}

npm-pack-zip で ZIP ファイルを作成する
----

### npm-pack-zip とは

ここでは、__`npm-pack-zip`__ パッケージを使って、Lambda 関数のデプロイ用 ZIP パッケージを作ってみます。

- [npm-pack-zip - npm](https://www.npmjs.com/package/npm-pack-zip)

NPM には、もともと標準で `npm pack` というデプロイ用の NPM パッケージを作成する仕組みが搭載されています。
Lambda 用にもこれが使えると楽なのですが、残念ながら `npm pack` は `.tgz` 形式のパッケージしか作成できません。
`npm-pack-zip` をインストールすると、`npm pack` の仕組みをそのまま使い、`.zip` 形式のパッケージを作成することができます。

{{< code title="npm-pack-zip のインストール" >}}
$ npm install npm-pack-zip --save-dev
{{< /code >}}

### パッケージング設定 (package.json)

`npm-pack-zip` のパッケージング設定は、`npm pack` と同様の仕組みで行います。

- `package.json` の __`files`__ が指定されている場合は、そこに指定されたファイル／ディレクトリのみをパッケージングする（`node_modules` は指定しなくて OK）
- `package.json` の __`bundledDependencies`__ に指定された依存モジュールをパッケージングする（`dependencies` に記述した依存モジュールのうち、パッケージングしたいものの名前を列挙する）

この仕組みは、[npm-packlist](https://www.npmjs.com/package/npm-packlist) モジュールによって提供されており、`npm-pack-zip` もこのモジュールを使っているので、`npm pack` と同じ設定方法が使えます。

例えばデプロイ対象として、`build` ディレクトリ以下のファイルと、`js-yaml` NPM モジュールを含めたいときは、__`package.json`__ で次のように指定します。
__`files`__ プロパティと __`bundledDependencies`__ プロパティに注目してください。

{{< code lang="json" title="packages.json" >}}
{
  "name": "hello-lambda",
  "version": "1.0.0",
  "scripts": {
    "start": "node build/index.js",
    "build": "tsc",
    "build:watch": "tsc --watch"
  },
  "files": [
    "build"
  ],
  "dependencies": {
    "js-yaml": "^4.0.0"
  },
  "bundledDependencies": [
    "js-yaml"
  ],
  "devDependencies": {
    "@types/node": "^14.14.35",
    "npm-pack-zip": "^1.2.9",
    "typescript": "^4.2.3"
  }
}
{{< /code >}}

`files` プロパティも `bundledDependencies` プロパティも、値として配列 (`[]`) を指定することに注意してください（`{}` にするとエラーになります）。
`dependencies` で指定した NPM モジュールのうち、ZIP パッケージにも含めたいものの名前を `bundledDependencies` に列挙します（名前だけでバージョンは指定しません）。

### ZIP パッケージを作成する

以上の設定が終われば、`npm-pack-zip` コマンドを実行して ZIP パッケージを作成できます（`npx` を使えば、`npm-pack-zip` コマンドは事前インストールなしでも実行できます）。
生成される ZIP ファイル名は、`package.json` の `name` プロパティによって自動的に決まります（ここでは `hello-lambda.zip`）。

{{< code title="ZIP パッケージの作成" >}}
$ npx npm-pack-zip
{{< /code >}}

ZIP パッケージを作成するコマンドは、次のように npm スクリプトにしておくと便利です。
ここでは、`zip` というスクリプト名にしました。

{{< code lang="js" title="package.json" >}}
{
  ...
  "scripts": {
    ...
    "zip": "npm-pack-zip"
  },
  ...
}
{{< /code >}}

こうしておけば、次のように ZIP パッケージを生成できます。

{{< code >}}
$ npm run build  # TypeScript を使っているなら先にビルド
$ npm run zip    # デプロイ用の ZIP パッケージを作成
{{< /code >}}

ちなみに、ここまでの設定通りに ZIP パッケージを作成していれば、その内容は次のようになります。

{{< code title="hello-lambda.zip の内容" >}}
hello-lambda.zip
  +-- build/
  |     +-- index.js
  +-- node-modules/
  |     +-- argparse/
  |     +-- js_yaml/
  +-- package.json
{{< /code >}}

`npm-packlist` の仕様上、`package.json` がデフォルトで含まれてしまいますが、特に問題はないかと思います。


Lambda に ZIP パッケージをデプロイする
----

デプロイ用の ZIP パッケージを作成できたら、あとは実際に AWS Lambda 上にデプロイするだけです。
ここでは、[Lambda コンソール](https://console.aws.amazon.com/lambda/home) で、`hello-lambda` という名前の Lambda 関数を作成済みであるとします（ランタイムは `Node.js 14.x` などを選択）。

### ハンドラー名を更新する

関数のエントリポイントとなるファイルを `build/index.js` などにした場合は、AWS 上の Lambda 関数のハンドラー設定もそれに合わせて変更しておく必要があります（デフォルトは `index.handler` になっているはず）。
例えば、AWS CLI を使って、次のようにハンドラー関数を指定できます（参考: [AWS CLI の設定](/p/j5iu7it)）。

{{< code title="ハンドラーを build/index.handler にする" >}}
$ aws lambda update-function-configuration \
      --function-name hello-lambda \
      --handler build/index.handler
{{< /code >}}

`--handler` オプションで指定するハンドラー名は、__`ファイルのベース名.関数名`__ であることに注意してください。
ドットの後ろは拡張子の `js` ではなく、関数名の `handler` を指定します。

### ZIP パッケージをデプロイする

Lambda 関数の ZIP パッケージをデプロイするには、Web ブラウザの Lambda コンソールからポチポチやるか、AWS CLI でコマンドラインから次のように実行します。
（必要に応じて `--region ap-northeast-1` なども追加）

{{< code title="Lambda に ZIP パッケージをデプロイする" >}}
$ aws lambda update-function-code \
      --function-name hello-lambda \
      --zip-file fileb://hello-lambda.zip
{{< /code >}}

次のように NPM スクリプト化しておくのもいいかもしれません。

{{< code title="package.json" >}}
{
  ...
  "scripts": {
    ...
    "deploy": "aws lambda update-function-code --function-name hello-lambda --zip-file fileb://hello-lambda.zip"
  },
  ...
}
{{< /code >}}

これで、次のように実行するだけで ZIP パッケージをデプロイできます。

{{< code >}}
$ npm run deploy
{{< /code >}}

### Lambda 関数を実行する

デプロイした Lambda 関数は AWS CLI の `lambda invoke` コマンドで呼び出すことができます。

{{< code >}}
$ aws lambda invoke --function-name hello-lambda response.json
{{< /code >}}

