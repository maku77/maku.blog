---
title: "AWS CDK で TypeScript の Lambda 関数をデプロイする (aws-lambda-nodejs)"
url: "/p/cj9i4m3"
date: "2021-10-06"
tags: ["AWS", "AWS/CDK"]
---

NodejsFunction コンストラクトについて
----

AWS CDK の [@aws-cdk/aws-lambda-nodejs パッケージ](https://docs.aws.amazon.com/cdk/api/latest/docs/aws-lambda-nodejs-readme.html) が提供する [NodejsFunction コンストラクト](https://docs.aws.amazon.com/cdk/api/latest/docs/@aws-cdk_aws-lambda-nodejs.NodejsFunction.html) を使用すると、CDK によって作成した CloudFormation スタック上に簡単に Lambda 関数をデプロイできます。
Lambda 関数用の汎用的なコンストラクトとしては、[@aws-cdk/aws-lambda パッケージ](https://docs.aws.amazon.com/cdk/api/latest/docs/aws-lambda-readme.html) が提供する `Functin` コンストラクトもあるのですが、Node.js による Lambda 関数実装用に特化した `NodejsFunction` を使うと、次のような恩恵を得られます。

* esbuild による Lambda 関数関連アセットの高速なパッケージング（バンドル）。
* Lambda 関数の TypeScript コードをいちいちコンパイルしなくてよい。

ちょっとややこしいのですが、CDK コード（TypeScript で書いた場合）の実行は `ts-code` で実行され、Lambda 関数のビルドとパッケージングは `esbuild` で行われます。


Lambda 関数の作成
----

ここでは、CDK アプリとして作成した CloudFormation スタック内に、TypeScript で実装した Lambda 関数を配置してみます。
CDK のインストールや、ブートストラッピング (`cdk bootstrap`) は、下記の記事を参考に完了させてください。

- 参考: [AWS CDK 入門 (1) インストールから Hello World まで](/p/nujyfsy)

### CDK アプリの作成

まずは、`cdk init app` で CDK アプリのテンプレートを作成します。

{{< code >}}
$ mkdir myapp && cd myapp
$ cdk init app --language typescript
{{< /code >}}

この時点で、`cdk deploy` コマンドを実行して、CloudFormation スタックが生成できるか試しておくのもよいです。

npm の代わりに yarn を使うときは下記も実行しておきます。

{{< code >}}
$ rm package-lock.json
$ yarn intall
{{< /code >}}


### Lambda 関数の実装

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


### Lambda 関数を含む CDK アプリのデプロイ

Lambda 関数用の AWS リソースを生成するために、[NodejsFunction コンストラクト](https://docs.aws.amazon.com/cdk/api/latest/docs/@aws-cdk_aws-lambda-nodejs.NodejsFunction.html) を使用したいので、次のようにコンストラクトパッケージをインストールします。

{{< code >}}
$ yarn add @aws-cdk/aws-lambda-nodejs
{{< /code >}}

CDK アプリのひな型として、CloudFormation スタックを構築するためのコンストラクト (`lib/myapp-stack.ts`) が生成されているはずなので、そのスタック内に、`NodejsFunction` コンストラクトを生成するよう記述します。

{{% private %}}
- 実装参考: [@aws-cdk/aws-lambda-nodejs README](https://docs.aws.amazon.com/cdk/api/latest/docs/aws-lambda-nodejs-readme.html)
{{% /private %}}

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
あとは、おもむろに __`cdk deploy`__ を実行すれば、Lambda 関数をデプロイすることができます。

{{< code >}}
$ cdk deploy
{{< /code >}}

CDK によるデプロイ処理が完了したら、[CloudFormation コンソール](https://console.aws.amazon.com/cloudformation/) を開いて、実際にスタックと Lambda 関数が生成されているか確認しましょう。

TypeScript コードのトランスパイルも esbuild で自動でやってくれるし、ZIP パッケージ化とアップロードも自動でやってくれる（これは CDK の機能ですが）ので、とっても楽ですね！


Lambda 関数コードだけの高速デプロイ (hotswap)
----

`cdk deploy` の実行には結構時間がかかりますが、Lambda 関数のコードだけ更新したいときは、__`hotswap`__ オプションを付けて実行することで高速にデプロイできます。

{{< code title="Lambda 関数だけ高速更新" >}}
$ cdk deploy --hotswap
{{< /code >}}

これは開発時のみ使うべき機能として提供されており、Production 環境においては、通常通り CDK アプリ全体のデプロイを行うことが推奨されています。


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

