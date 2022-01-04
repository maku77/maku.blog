---
title: "AWS CDK 入門 (2) コンストラクトの概念を理解する"
url: "/p/nqkav2m"
date: "2021-10-04"
tags: ["AWS", "AWS/CDK"]
weight: 1002
---

コンストラクト・ツリー (Construct Tree)
----

AWS CDK を使って CloudFormation スタックを作成するとき、そのリソース構成は、論理的な __コンストラクト (construct)__ のツリー構造によって表現します。
下記は、典型的な CDK アプリケーションにおけるコンストラクト・ツリー構造です。

{{< code >}}
App
  +-- Stack
  |     +-- Construct
  |     +-- Construct
  |     +-- ...
  +-- Stack
        +-- Construct
        |     +-- Construct
        |     +-- Construct
        +-- Construct
        +-- ...
{{< /code >}}

`App`、`Stack`、`Construct` はそれぞれ抽象度の異なるオブジェクトですが、すべて [IConstruct インタフェース](https://docs.aws.amazon.com/cdk/api/latest/docs/@aws-cdk_core.IConstruct.html) を実装したコンストラクトの一種です。
CDK アプリ―ケーションには、構成のエントリポイントとなる __`App` コンストラクト__ が 1 つあり、複数の `Stack` コンストラクトを含むことができます。

__`Stack` コンストラクト__ は、その名の通り CloudFormation スタックを表現するコンストラクトです。
`App` が複数の `Stack` から構成されている場合、`cdk deploy` コマンドでデプロイを実行したときに複数の CloudFormation スタックが生成されることになります。
`cdk deploy <Stack名>` とすれば、特定のスタックのみをデプロイすることも可能です。
指定可能な Stack 名の一覧を確認したいときは、`cdk ls` コマンドを使います。

`Stack` コンストラクトはスタックを構成する入れ物であり、実際に AWS リソースを配置するには、`Stack` 内に具体的な AWS リソースを生成するための __`Construct` コンストラクト__ を配置します。
末端に配置する `Construct` としては、CDK が提供する L1/L2/L3 コンストラクト（後述）や、`cdk.Construct` を継承して作った独自のコンストラクトなどを指定します。
`Construct` には、ほとんど CloudFormation のリソースそのままの低レベルなもの（例: `CfnBucket`）もあれば、高度に抽象化されたもの（例: `LambdaRestApi`）もあります。
`Construct` クラスには、複数の CDN アプリで使えるように汎用化されたものもあり、それらをコンストラクト・ライブラリ (Construct Library) と呼びます。
CDN 自体が提供している `s3.Bucket` もコンストラクト・ライブラリの一種です。

{{% note title="入れ子構造の制約" %}}
上の例のような階層構造はとても直感的ですが、実際には `App` 直下に独自の `Construct` を配置して、その下に複数の `Stack` を置くということもできます。
`Construct` は柔軟な構成が可能ですが、`Construct` を入れ子にするたびに、生成される AWS リソースの物理名も長くなっていくことに注意してください。
`Stack` を入れ子構造にしたいときは、`Stack` 以下に [NestedStack](https://docs.aws.amazon.com/cdk/api/latest/docs/@aws-cdk_aws-cloudformation.NestedStack.html) を配置します。
{{% /note %}}


L1/L2/L3 コンストラクト
----

CDK はコンストラクト・ライブラリとして、L1 ～ L3 までの異なる抽象度レベルの `Construct` クラスを提供しています。
最初は、L2 あたりの `Construct` をよく使うことになると思います。

L1 コンストラクト (CloudFormation-only)
: `CfnBucket` のように、名前が `Cfn` で始まるもっとも低レベルなコンストラクトで、__Cfn リソース__ とも呼ばれます。CloudFormation の仕様から自動生成されており、最新の AWS リソースであっても必ずこの L1 コンストラクトは提供されています。CloudFormation でリソースを定義するのと同様に詳細なプロパティ設定する必要があり、AWS リソースおよび CloudFormation に関する知識が必要です。

L2 コンストラクト (Curated)
: 1 つ以上の L1 コンストラクトをカプセル化し、デフォルトの設定やポリシーを提供したコンストラクトです。例えば、S3 バケットを生成するために `s3.Bucket` コンストラクトが提供されており、その実装内部では L1 コンストラクトの `CfnBucket` が使われています。L1 コンストラクトの `CfnBucket` で S3 バケットを作ろうとすると、`bucketName` prop で世界で一意な物理バケット名を指定する必要がありますが、L2 コンストラクトの `s3.Bucket` を使うと、コンストラクトにシンプルな論理名を指定するだけで済みます。また、L2 コンストラクトは便利メソッドを提供していることもあります（`s3.Bucket` の `addLifeCycleRule()` など）。

L3 コンストラクト (Patterns)
: 複数の L1、L2 コンストラクトを組み合わせて作られた、特定のユースケースを実現するためのコンストラクトです。例えば、API Gateway と Lambda 関数の組み合わせを簡単に定義する `aws-apigateway.LambdaRestApi` コンストラクトなどがあります。


コンストラクトの階層を作る
----

コンストラクトの入れ子構造を作るには、各種コンストラクトのコンストラクタ（ややこしい＾＾;）の __第1引数__ で親となるコンストラクトを指定します。

{{< code lang="ts" hl_lines="7" title="lib/my-app-stack.ts" >}}
import * as cdk from '@aws-cdk/core'
import * as s3 from '@aws-cdk/aws-s3'

export class MyAppStack extends cdk.Stack {
  constructor(scope: cdk.Construct, id: string, props?: cdk.StackProps) {
    super(scope, id, props)
    new s3.Bucket(this, 'MyBucket')
  }
}
{{< /code >}}

この例では、`s3.Bucket` コンストラクトの第1引数で `this`、つまり、`MyAppStack` コンストラクトを指定しています。
さらに、この `MyAppStack` をインスタンス化するときは、その親コンストラクトとして、CDK アプリのエントリポイントである `App` コンストラクトを指定します。

{{< code lang="ts" hl_lines="5" title="bin/my-app.ts" >}}
import * as cdk from '@aws-cdk/core'
import { MyAppStack } from '../lib/my-app-stack'

const app = new cdk.App();
new MyAppStack(app, 'MyAppStack', {})
{{< /code >}}

これらによって、次のようなコンストラクト・ツリーが作られたことになります。

{{< code >}}
App
 +-- MyAppStack
      +-- s3.Bucket
{{< /code >}}

ちなみに、`App` はトップレベルのコンストラクトなので、そのコンストラクタには親コンストラクトを指定するパラメーターはありません（参考: [class App](https://docs.aws.amazon.com/cdk/api/latest/docs/@aws-cdk_core.App.html)）。

通常、コンストラクトのコンストラクタの __第2引数__ では、そのコンストラクトの論理 ID を指定します。
この ID はそのコンストラクト階層の中で一意であれば十分なので、CDK アプリ内で意味が通じる程度にシンプルな名前を付けるようにします。

__第3引数__ は、コンストラクトごとの設定情報である props オブジェクトを指定します。
例えば、`s3.Bucket` コンストラクトの場合は、[s3.BucketProps](https://docs.aws.amazon.com/cdk/api/latest/docs/@aws-cdk_aws-s3.BucketProps.html) オブジェクトを渡すことで、S3 バケットの設定を行うことができます（例: `{ versioned: true }`）。
多くの場合、props オブジェクトの多くのプロパティは省略可能であり、第3引数の指定自体を省略できるコンストラクトもあります（`s3.Bucket` もそのひとつです）。

