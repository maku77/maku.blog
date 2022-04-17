---
title: "AWS CDK で TypeScript で実装した Lambda 関数をデプロイする (aws-lambda-nodejs)"
url: "/p/cj9i4m3"
date: "2021-10-06"
tags: ["AWS", "AWS/CDK"]
weight: 1500
---

何をするか？
----

ここでは、CDK アプリとして作成した CloudFormation スタック内に、__TypeScript で実装した Lambda 関数__ を追加してみます。
Lambda 関数のビルドもデプロイ時に自動で行われるようにします。

以降の説明では、CDK プロジェクトの作成自体は済んでいるものとします。

- 参考: [AWS CDK 入門: cdk コマンドのインストールから Hello World まで](/p/nujyfsy)


TypeScript で Lambda 関数を実装する
----

AWS SDK と Lambda 用の TypeScript 型情報をインストールします。

{{< code >}}
$ yarn add aws-sdk
$ yarn add @types/aws-lambda --dev
{{< /code >}}

Lambda 関数のコードは、プロジェクトのルートに `lambda` ディレクトリを作成して、そこに配置していくことにします。

{{< code hl_lines="3" >}}
myapp/
  +-- bin/     ... CDK の App コンストラクト (.ts)
  +-- lambda/  ... ラムダ関数の実装コード (.ts) ★これを追加
  +-- lib/     ... CDK の Stack コンストラクトなど (.ts)
  ...
{{< /code >}}

最低限の Hello World 的なラムダ関数を作成します。

{{< code lang="ts" title="lambda/index.ts" >}}
import { Handler } from 'aws-lambda'

// Lambda エントリーポイント
export const handler: Handler = async () => {
  console.log('Hello Lambda!')
}
{{< /code >}}


Lambda 関数を含むスタックの定義とデプロイ
----

### @aws-cdk/aws-lambda-nodejs パッケージ

AWS CDK の [@aws-cdk/aws-lambda-nodejs パッケージ](https://docs.aws.amazon.com/cdk/api/latest/docs/aws-lambda-nodejs-readme.html) が提供する [NodejsFunction コンストラクト](https://docs.aws.amazon.com/cdk/api/latest/docs/@aws-cdk_aws-lambda-nodejs.NodejsFunction.html) を使用すると、TypeScript 言語で実装した Lambda 関数を簡単にデプロイできます。
Lambda 関数用の汎用的なコンストラクトとしては、[@aws-cdk/aws-lambda パッケージ](https://docs.aws.amazon.com/cdk/api/latest/docs/aws-lambda-readme.html) が提供する `Function` コンストラクトもあるのですが、Node.js による Lambda 関数実装用に特化した `NodejsFunction` を使うと、次のような恩恵を得られます。

* esbuild による Lambda 関数関連アセットの高速なパッケージング（バンドル）。
* Lambda 関数の TypeScript コードをいちいちコンパイルしなくてよい。

ちょっとややこしいのですが、CDK コード（TypeScript で書いた場合）の実行は `ts-code` で実行され、Lambda 関数のビルドとパッケージングは `esbuild` で行われます。

コンストラクトパッケージは次のようにインストールします。

```console
$ npm install @aws-cdk/aws-lambda-nodejs  # npm の場合
$ yarn add @aws-cdk/aws-lambda-nodejs     # yarn の場合
```

### スタックの定義

CDK アプリのひな型として、CloudFormation スタックを構築するためのコンストラクト (`lib/myapp-stack.ts`) が生成されているはずなので、そのスタック内に、`NodejsFunction` コンストラクトを生成するよう記述します。

{{< code lang="ts" hl_lines="2 8-13" title="lib/myapp-stack.ts" >}}
import * as cdk from '@aws-cdk/core'
import * as lambdaNodejs from '@aws-cdk/aws-lambda-nodejs'

export class MyappStack extends cdk.Stack {
  constructor(scope: cdk.Construct, id: string, props?: cdk.StackProps) {
    super(scope, id, props)

    new lambdaNodejs.NodejsFunction(this, 'HelloFunction', {
      entry: 'lambda/index.ts',
      // handler: 'handler', // デフォルトのハンドラ関数名は 'handler'
      // runtime: Runtime.NODEJS_14_X, // デフォルトは Node.js 14.x
      // timeout: cdk.Duration.minutes(15), // デフォルトは 3 秒
    })
  }
}
{{< /code >}}

最小構成では、上記のように props オブジェクトの __`entry`__ プロパティで Lambda 関数の `index.ts` ファイルパスを指定するだけで OK です。

### デプロイ

スタックの定義が済んだら、__`cdk deploy`__ を実行して Lambda 関数をデプロイすることができます。

{{< code >}}
$ cdk deploy
{{< /code >}}

デプロイが完了したら、[CloudFormation コンソール](https://console.aws.amazon.com/cloudformation/) を開いて、実際にスタックと Lambda 関数が生成されているか確認しましょう。
TypeScript コードのトランスパイルも esbuild で自動でやってくれるし、ZIP パッケージ化とアップロードも自動でやってくれる（これは CDK の機能ですが）ので、とっても楽ですね！

- 参考: [AWS CDK メモ: Lambda 関数コードだけ高速デプロイする (cdk deploy --hotswap)](/p/ap8p7n4)


トラブルシューティング
----

### NodejsFunction の第1引数の this でエラーになるとき

`NodejsFunction` のコンストラクタの `this` を渡している部分で、次のような型情報エラーが発生するときは、`aws-cdk` 本体と、コンストラクトライブラリ（`@aws-cdk/aws-lambda-nodejs` など）のバージョンが合っていない可能性があります。

> Argument of type 'this' is not assignable to parameter of type 'Construct'.
> Type 'MyappStack' is not assignable to type 'Construct'.

`package.json` を開いて、両者のバージョンを新しい方に揃えて、`yarn install` で更新すれば直ります。

### デプロイ時に spawnSync docker ENOENT が出る場合

Windows や macOS で `cdk diff` や `cdk deploy` を実行したときに、__`spawnSync docker ENOENT`__ というエラーが発生する場合は、[esbuild をインストール](https://docs.aws.amazon.com/cdk/api/latest/docs/aws-lambda-nodejs-readme.html#local-bundling) するとうまくいくようです。

{{< code >}}
$ yarn add --dev esbuild@0
あるいは
$ npm install --save-dev esbuild@0
{{< /code >}}

