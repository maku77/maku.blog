---
title: "AWS CDK で外部パラメーターを扱う（コンテキスト・バリューと環境変数）"
url: "/p/vx5ta85"
date: "2021-10-11"
tags: ["AWS", "AWS/CDK"]
---

CDK コードに外部パラメーターを与える方法
----

AWS CDK による CloudFormation スタックの構築時に、外部からキー＆バリューの形でパラメーターを設定したいときは、主に次の 3 つの方法があります（クラウド上に値を保存するパラメーターストアなどは対象外とします）。

1. __Context values__ （コンテキスト・バリュー）
2. __Environment variables__ （環境変数）
3. __CloudFormation parameters__ （CloudFormation パラメーター）

S3 バケットの名前をパラメーター化したり、デプロイターゲットを `staging` と `production` の間で切り替えたりするときに使えます。


Context values（コンテキスト・バリュー）
----

コンテキスト・バリューは、CDK 特有の仕組みで、__`cdk deploy`__ 実行時のコマンドライン引数や、__`cdk.json`__ ファイルの中で、キー＆バリューのペアを設定することができます。
キーの型は `string` で、バリューの型は JSON がサポートするデータ型のいずれかです（`string`、`number`、オブジェクト、およびそれらの配列）。
コンテキスト・バリューは CDK の仕組みなので、CDK コードの中からしか参照できません。
Lambda 関数の中から値を参照したい場合は、Lambda 関数のコンストラクトを生成するときに、`environment` props などで間接的に渡す必要があります。

### コマンドライン引数で指定する方法

`cdk deploy` コマンド（あるいは `diff`、`synth`）を実行するときに、__`--context (-c)`__ オプションで、コンテキスト・バリューを設定できます。

{{< code >}}
$ cdk deploy --context key=value
{{< /code >}}

複数のキー＆バリューペアを設定したいときは、単純にオプション指定を繰り返します。

{{< code >}}
$ cdk deploy -c key1=value1 -c key2=value2
{{< /code >}}

コンテキスト・バリューは CDK アプリ内の全スタックに渡されますが、特定のスタックにのみ反映させることもできます。

{{< code >}}
$ cdk deploy -c Stack1:key1=value1 -c Stack2:key2=value2
{{< /code >}}

### cdk.json で指定する方法

`cdk init app` コマンドで CDK アプリを生成すると、__`cdk.json`__ というファイルがプロジェクトのルートに配置されます。
このファイルの中の、__`context`__ プロパティで、コンテキスト・バリュー用のキー＆バリューを記述しておくことができます。
ホームディレクトリの `~/cdk.json` に記述しておくこともできます。

{{< code lang="js" title="cdk.json" >}}
{
  "app": "npx ts-node --prefer-ts-exts bin/myapp.ts",
  "context": {
    "@aws-cdk/aws-apigateway:usagePlanKeyOrderInsensitiveId": true,
    "@aws-cdk/core:enableStackNameDuplicates": "true",

    // ★このあたりに追加
    "target": "dev",
    "bucket": {
      "region": "ap-northeast-1",
      "name": "my-bucket"
    }
  }
}
{{< /code >}}

ちなみに、`context` プロパティには最初からいくつか値が設定されていますが、これらは各モジュール用の機能を有効化するための設定です（`@aws-cdk/aws-lambda:` のようなモジュール名のプレフィックスが付いています）。
これらは各機能を Off/On するためのものなので、Feature flags と呼ばれています（省略した場合のデフォルトは Off です）。

`cdk.json` ファイルの `context` プロパティの値よりも、`cdk` コマンドの `--context` 引数で指定した値が優先されるので、`cdk.json` ファイルの方にデフォルト設定を書いておき、`cdk` コマンド実行時に上書きするという運用が可能です。

{{< code >}}
$ cdk deploy  # 省略時は cdk.json に記述した target=dev が使われる
$ cdk deploy -c target=prod  # 本番環境は明示的に target を上書き
{{< /code >}}

### cdk.App コンストラクト生成時に指定する方法

コンストラクト生成時の `context` オプションで、明示的にコンテキスト・バリューを設定することもできます。
この値は、そのコンストラクタ以下のノード全体に反映されます。

{{< code lang="ts" title="bin/myapp.ts" >}}
import * as cdk from '@aws-cdk/core'

const app = new cdk.App({
  context: {
    bucketName: 'bucket-123456789012',
  },
})
{{< /code >}}

このように、トップレベルの `cdk.App` コンストラクトに設定すれば、すべての `Stack` コンストラクトから、`this.node.tryGetContext()` でそのコンテキスト・バリューを参照できるようになります。

### コンテキスト・バリューの参照方法

CDK のコード内から、[construct.node.tryGetContext 関数](https://docs.aws.amazon.com/cdk/api/latest/docs/@aws-cdk_core.ConstructNode.html#trywbrgetwbrcontextkey) を呼び出すことで、コンテキスト・バリューを参照できます。
次の例では、キー名 `bucketName` のコンテキスト・バリューを参照しています。

{{< code lang="ts" title="bin/myapp.ts" >}}
import * as cdk from '@aws-cdk/core'

const app = new cdk.App()

const bucketName = app.node.tryGetContext('bucketName') as string
if (bucketName == undefined) {
  throw new Error('Context value [bucketName] is not set')
}
console.log(`bucketName = ${bucketName}`)

// ... 各スタックの定義 ...
{{< /code >}}

`tryGetContext` 関数の戻り値は `any` 型で、指定したキーに対応する値がセットされていないときは、`undefined` を返します（TypeScript の場合）。
デプロイのために必須の値であれば、戻り値が `undefined` のときに `Error` をスローすることで `cdk` コマンドの実行を終了させることができます。

### （おまけ）cdk.context.json ファイル

`cdk` コマンド実行時にキャッシュファイル (`cdk.context.json`) が生成されることがあります。
CDK のコードから Stack の `availabilityZones` を参照したり、パラメーターストアを参照したりすると、`cdk synth` のタイミングで生成されるようです。
これは、`cdk` コマンドの実行を効率化するためのものであり、2 回目以降の実行では、このファイルに保存されたコンテキスト・バリューが参照されるようになります。
このキャッシュは自動的に削除されることはないので、例えば、パラメーターストア側の設定値を変更してすぐに反映したいときは、__`cdk context --clear`__ コマンドなどで `cdk.context.json` の内容をクリアする必要があります。

### （おまけ）cdk context コマンド

__`cdk context`__ コマンドを引数なしで実行すると、`cdk.json` ファイルや `cdk.context.json` ファイルで設定されているコンテキスト・バリューの一覧を確認することができます。

{{< code >}}
$ cdk context     # テーブル形式で表示
$ cdk context -j  # JSON形式で表示
{{< /code >}}

`cdk context --clear` コマンドを実行すると、`cdk.context.json` ファイルに保存された情報（キャッシュ）をクリアできます。
`cdk.json` ファイルの `context` プロパティの値はクリアされません。

{{< code >}}
$ cdk context --clear         # 全クリア
$ cdk context --reset <Name>  # キー名を指定してクリア（インデックス指定も可）
{{< /code >}}


Environment variables（環境変数）
----

CDK コードの中から、__`process.env.環境変数名`__ で環境変数の値を取得することができます（設定されていない場合は `undefined` になります）。

{{< code lang="ts" >}}
new MyappStack(app, 'MyappStack', {
  env: {
    account: process.env.CDK_DEFAULT_ACCOUNT,
    region: process.env.CDK_DEFAULT_REGION
  },
})
{{< /code >}}

環境変数は CDK の仕組みではないので、CDK のコードと Lambda 関数のコードで同じように参照することができますが、環境変数の値は実行時の環境によって変わってくることに注意してください。
あと、環境変数は文字列値しか保持できないので、オブジェクトを持てるコンテキスト・バリューの方が柔軟性があります。
コンストラクト・ツリーの子ノードのみに設定値を伝搬させるという仕組みも、コンテキスト・バリューにしかありません。
これらを考慮すると、基本的には、CDK によるデプロイ用のパラメーターとしては、コンテキスト・バリューの仕組みを使うのがよさそうです。
CDK Developers Guide の [Best practices](https://docs.aws.amazon.com/ja_jp/cdk/latest/guide/best-practices.html#best-practices-constructs) でも、__コンストラクトの中での環境変数の参照はアンチパターン__ であると述べられています（トップレベル、つまり App コンストラクトでの参照はこの限りではないけれど、それ以下の階層へは props で伝搬させていくべき）。

一方、Lambda 関数からは CDK のコンテキスト・バリューを参照することはできないので、間接的にコンテキスト・バリューの値を使いたいときは、次のような感じで Lambda 関数コンストラクトの `environment` オプションで値を渡してやる必要があります（Lambda 関数の実行環境の環境変数として設定されます）。

{{< code lang="ts" >}}
const myLambda = new lambdaNodejs.NodejsFunction(this, 'MyLambda', {
  runtime: lambda.Runtime.NODEJS_14_X,
  entry: 'lambda/index.ts',
  environment: {
    S3_BUCKET_NAME: this.tryGetContext('s3BucketName') as string,
    S3_OBJECT_KEY: this.tryGetContext('s3ObjectKey') as string,
  },
})
{{< /code >}}


CloudFormation parameters（CloudFormation パラメーター）
----

__この仕組みは CDK では非推奨とされています。__

`cdk deploy` 時に、__`--parameters`__ オプションを指定することで、CloudFormation パラメーターに相当するキー＆バリューを設定することができます。

{{< code >}}
$ cdk deploy --parameters uploadBucketName=UploadBucket
{{< /code >}}

上記のように指定した CloudFormation パラメーターは、CDK コードの中から次のように参照することができます。

{{< code lang="ts" >}}
const uploadBucketName = new cdk.CfnParameter(this, 'uploadBucketName', {
  type: 'string',
  description: 'Name of the bucket to store image files'
})

const myBucket = new s3.Bucket(this, 'uploadBucket', {
  bucketName: uploadBucketName.valueAsString
})
{{< /code >}}

ただし、CloudFormation パラメーターはデプロイ時 (`cdk deploy`) にしか参照できず、Synthesize (`cdk synth`) のタイミングでは有効ではないという制約があります。
なぜなら、CloudFormation テンプレートは Synthesize 時に生成されるものであり、その時点では CloudFormation パラメーターはプレースホルダーとして残しておかなければいけないからです（コンテキスト・バリューのように Synthesize 時に値を展開できない）。
CDK アプリの他の部分とうまく連携がとれないため、コンテキスト・バリューの方を使うことが推奨されています。


応用： コンテキスト・バリューを使って実行環境 (staging/production) を切り替える
----

アプリの実行環境は、次のような感じで、用途ごとに作成することが多いと思います。

- 本番環境 (production, prod)
- ステージング環境 (staging, stg)
- 開発環境 (develop, dev)

例えば、本番環境用の CloudFormation スタックと、ステージング環境用の CloudFormation スタックは分けて作成することになります。
CDK のコンテキスト・バリューの仕組みを使って、次のように `cdk deploy` 時にターゲットとする環境を切り替えることができます（`targetEnv` というキー名は勝手に決めた名前です）。

{{< code >}}
$ cdk deploy --context targetEnv=production  # 本番環境用スタックの生成
$ cdk deploy --context targetEnv=staging  # ステージング用スタックの生成
$ cdk deploy --context targetEnv=development  # 開発環境用スタックの生成
{{< /code >}}

あとは、App コンストラクトの CDK コードなどで、`targetEnv` の値を使って、各種コンフィグ値を切り替えることができます。
次の例では、`config.ts` というファイルに各環境用のコンフィグ情報を記述し、`targetEnv` の値で切り替えるようにしています。

{{< code lang="ts" title="lib/config.ts（各環境の設定情報）" >}}
export type Config = {
  stackName: string
  stackDesc: string
  imageBucketName: string
  tags: {
    team: string
    targetEnv: string
  }
}

export function getConfig(targetEnv: string): Config {
  const stackName = `myapp-stack-${targetEnv}`
  const stackDesc = `MyApp stack for ${targetEnv}`
  switch (targetEnv) {
    case 'production':
      return {
        stackName,
        stackDesc,
        imageBucketName: 'image-bucket-prod',
        tags: {
          team: 'ProdTeam',
          targetEnv: 'production',
        },
      }
    case 'development':
      return {
        stackName,
        stackDesc,
        imageBucketName: 'image-bucket-dev',
        tags: {
          team: 'DevTeam',
          targetEnv: 'development',
        },
      }
    default:
      throw new Error(
        'Context value [targetEnv] is invalid (use production or development).'
      )
  }
}
{{< /code >}}

{{< code lang="ts" title="bin/myapp.ts（App コンストラクト）" >}}
import * as cdk from '@aws-cdk/core'
import { getConfig } from '../lib/config'
import { MyappStack } from '../lib/myapp-stack'

const app = new cdk.App()

// ここでコンテキスト・バリューに基づいて Config 値を決定する
const targetEnv = app.node.tryGetContext('targetEnv') as string
const config = getConfig(targetEnv)
// デフォルトを development とする場合
// const config = getConfig(targetEnv ?? 'development')

// スタックの props として渡したりする
new MyappStack(app, config.stackName, {
  env: { account: '123456789012', region: 'ap-northeast-1' },
  description: config.stackDesc,
  tags: config.tags,
  config,
})
{{< /code >}}

{{< code lang="ts" title="lib/myapp-stack.ts（Stack コンストラクト）" >}}
import * as cdk from '@aws-cdk/core'
import { Config } from './config'

interface MyappStackProps extends cdk.StackProps {
  config: Config
}

export class MyappStack extends cdk.Stack {
  constructor(scope: cdk.Construct, id: string, props: MyappProps) {
    super(scope, id, props)

    // あとは props.config の値を使って煮るなり焼くなり
}
{{< /code >}}

