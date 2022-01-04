---
title: "TypeScript で AWS SDK を使う開発環境を整える"
url: "/p/taiqx6d"
date: "2021-02-25"
tags: ["AWS", "TypeScript"]
weight: 110
---

{{% private %}}
- [AWS SDK Verion 3 for JavaScript](https://docs.aws.amazon.com/sdk-for-javascript/v3/developer-guide/welcome.html)
- [サンプルコード](https://github.com/awsdocs/aws-doc-sdk-examples/tree/master/javascriptv3/example_code)
{{% /private %}}

TypeScript のプロジェクトを作成する
----

### 何をできるようにするか？

ここでは、Node.js はインストール済みであるという前提で、次のようなことを行える TypeScript 環境を整えます。

- __`npm run build`__ で `src` ディレクトリ以下の `.ts` ファイルをコンパイルする  
  （`.js` ファイルが `build` ディレクトリ以下に出力される）
- __`npm start`__ でコンパイルされた `build/main.js` を起動する

下記のセットアップ手順の [詳しい意味はこちらを参照](/p/ak7u3h3) していただくとして、ここでは一気にセットアップを終わらせてしまいます。

### セットアップ

{{< code lang="shell" >}}
# アプリ用のディレクトリを作成
$ mkdir myapp
$ cd myapp

# Node.js アプリの設定ファイル (package.json) を生成
$ npm init -y

# TypeScript および Node 型情報をインストール
$ npm install typescript -D
$ npm install @types/node -D

# TypeScript の設定ファイル (tsconfig.json) を生成
$ npx tsc --init
{{< /code >}}

`package.json` を次のような感じで修正します。
主に `scripts` の定義です。

{{< code lang="js" title="package.json" >}}
{
  "name": "myapp",
  "version": "1.0.0",
  "scripts": {
    "start": "node build/main.js",
    "build": "tsc",
    "build:watch": "tsc --watch"
  },
  "devDependencies": {
    "@types/node": "^14.14.31",
    "typescript": "^4.2.2"
  }
}
{{< /code >}}

`tsconfig.json` を次のような感じで修正します。
主に、`outDir` と `include` の調整です。

{{< code lang="js" title="tsconfig.json" >}}
{
  "compilerOptions": {
    "target": "ES2018",
    "module": "commonjs",
    "strict": true,
    "esModuleInterop": true,
    "skipLibCheck": true,
    "forceConsistentCasingInFileNames": true,
    "outDir": "./build",
  },
  "include": ["./src/**/*"]
}
{{< /code >}}

これで、基本的な環境構築は完了です。

### 動作確認

TypeScript で HelloWorld を実行してテストします。
`src` ディレクトリ以下に次のような `main.ts` ファイルを作成してください。

{{< code lang="typescript" title="src/main.ts" >}}
console.log('Hello World');
{{< /code >}}

次のようにビルドして、起動できることを確認します。

{{< code >}}
$ npm run build
$ npm start
{{< /code >}}

`build/main.js` が実行されて、`Hello World` と表示できれば成功です。

あとは、必要に応じて AWS SDK のパッケージをインストールして、TypeScript でプログラムを作成していきます。
例えば、S3 や DynamoDB サービスを使うプログラムを作成するのであれば、次のようにライブラリをインストールします。

{{< code >}}
$ npm install @aws-sdk/client-s3
$ npm install @aws-sdk/client-dynamodb
{{< /code >}}

{{% note title="AWS SDK は devDependencies でインストールすべき？" %}}
`npm install` で NPM パッケージをインストールするとき、実行時に必要なものは `--save` で、開発時のみ必要なものは `--save-dev` でインストールします。
この考え方からすると、AWS SDK (`@aws-sdk`) は `--save` オプションでインストールするのが自然なのですが、Lambda 関数用のプロジェクトではちょっと事情が違ってきます。
なぜなら、AWS の Lambda 実行環境にはデフォルトで AWS SDK がインストールされているからです。

AWS SDK を `--save` と `--save-dev` のどちらでインストールすべきかは、そのプロジェクトでデプロイ用の ZIP パッケージをどう作成するかによっても変わってきます（パッケージングするときに `@aws-sdk` パッケージを含めないようにするのであれば、`--save` でインストールしても問題ない）。
よく分からないときは、ZIP パッケージの肥大化を防ぐため、AWS SDK はとりあえず `--save-dev` でインストールしておくのがよいと思います。

- 参考: [Lambda にデプロイするための ZIP パッケージを npm で作成する](/p/zmydq3f)
{{% /note %}}


（おまけ）AWS SDK Version 2 ではなく Version 3 を使う
----

Node.js 用の AWS SDK は 2020年12月に Version 3 が公開されました。
世の中には Version 2 のコードがあふれていますが、Version 3 には次のような利点があります。

- __使用する AWS サービスのパッケージだけを個別にインストール／インポートできる__
    - Version 2 では、`aws-sdk` パッケージでたくさんのパッケージを丸ごとインポートしていた
    - Version 3 では、例えば DynamoDB だけを使うのであれば、`@aws-sdk/client-dynamodb` だけインポートする → バンドルサイズを小さくでき、インポート時のオーバーヘッドも減らせる
- __ミドルウェアの仕組みを利用できる__
    - 例えば、全てのリクエストをフックして、任意のヘッダを付加することができる
- __デフォルトで TypeScript に対応__
    - 例えば、TypeScript コードから `@aws-sdk/client-s3` をインポートすると、型付けされた S3 用クラスを扱える

Version 2 と 3 では、パッケージのインポート方法も次のように異なっています。
Version 3 のパッケージ名には、先頭に `@` が付くので簡単に判別できます（`@aws-sdk` というプレフィックスは、AWS SDK チームが開発したパッケージであることを示しています）。

{{< code lang="js" title="Version 2（古い）" >}}
// SDK 全体のインポート
import * as AWS from 'aws-sdk';

// モジュールを個別にインポート
import { DynamoDB } from 'aws-sdk';
{{< /code >}}

{{< code lang="typescript" title="Version 3（新しい）" >}}
// DynamoDB クライアントを使用する（V3 のコマンドベースの API (send) を使う場合）
import { DynamoDBClient, ListTablesCommand } from '@aws-sdk/client-dynamodb';

// DynamoDB を使用する（V2 のような API を使う場合）
import { DynamoDB } from '@aws-sdk/client-dynamodb';
{{< /code >}}

今から AWS SDK for JavaScript (Node.js) を使う場合は、余計なオーバーヘッドを防ぐためにも、__Version 3 の方を使い、`XxxxClient` のようなクラスを個別にインポートする__ 方法をお勧めします。
さらに、デフォルトで TypeScript の型情報が提供されているので、できるだけ TypeScript でコーディングするようにしましょう。

{{< reference >}}
- [DynamoDB を Node.js (AWS SDK) で操作する](/p/5mv5dkt)
{{< /reference >}}

