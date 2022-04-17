---
title: "AWS CDK 入門: cdk コマンドのインストールから Hello World まで"
url: "/p/nujyfsy"
date: "2021-10-04"
tags: ["AWS", "AWS/CDK"]
weight: 1001
---

CDK とは
---

[AWS CDK (Cloud Development Kit)](https://aws.amazon.com/jp/cdk/) を使うと、TypeScript や Python を使って AWS リソースの生成を自動化することができます。
大きなプロジェクトでは、CloudFormation や SAM によるスタック生成用のテンプレートが長大になりがちですが、CDK を使うことで CloudFormation テンプレートの生成処理を隠蔽し、効率的にインフラ定義を行うことができます。
__CDK の利点__ をざっと挙げると以下のような感じです。

- TypeScript、Python などのパワフルな言語機能を使ってインフラ定義を行うことができる。
- 各種リソース間の参照を、オブジェクトのプロパティ参照という自然な形で表現できる。
- コンストラクト (Construct Library) という再利用可能なライブラリの提供により、様々なユースケースに対応した AWS リソース群を短いコードで定義できる。
- VS Code (TypeScript) 、PyCharm (Python) などで型情報の補完が効くため、AWS リソースに設定可能なプロパティを見つけやすい。明らかに間違った設定はコンパイル時に気付くことができる。
- Lambda 関数デプロイ時などに必要になる ZIP パッケージング、および S3 一時バケットの生成を自動で行ってくれる。

一方、__CDK の欠点__ としては、基本的に AWS に特化したツールであるため、Terraform や Serverless Framework のように様々なクラウド (Azure, AWS, GCP) に対応できないという点が挙げられます。
Terraform などの汎用ツールに比べて、CDK は学習コストも比較的高いです。
Azure なども同じツールで構築したいとか、それほど細かい制御は必要ないということであれば、Terraform などを使った方がいいかもしれません。
ただ、CDK を使っている限り、AWS の最新のリソースにも即対応できることが保証されますし、AWS CloudFormation に関しての知識は身につけやすいでしょう。


CDK Toolkit のインストール
----

CDK によるデプロイには __`cdk`__ コマンドを使用します。
まずはこのコマンドを使えるようにするため、__`aws-cdk`__ パッケージをインストールします（CDK 本体の実行には Node.js が必要です）。

{{< code >}}
$ npm install -g aws-cdk
{{< /code >}}

`cdk` コマンドが実行できるようになっていれば OK です。

{{< code >}}
$ cdk --version
1.125.0 (build 67b4921)
{{< /code >}}


CDK で Hello World
----

まずは、手始めに `cdk deploy` コマンドで S3 のバケットを含む CloudFormation スタックを作るところまでやってみます。
前提条件として、AWS の認証情報を設定しておく必要があります（`aws configure` などで情報が表示できれば OK）。

- [AWS の初期設定: AWS CLI と認証情報の設定](/p/j5iu7it)

### cdk init app

ここでは、空の `myapp` ディレクトリを作成して、そのディレクトリ内に CDK アプリを構成していくことにします。
CDK アプリというのは、CloudFormation スタックを生成する CDK コードと、Lambda 関数などのコードをまとめた概念です。

{{< code >}}
$ mkdir myapp && cd myapp
{{< /code >}}

__`cdk init app`__ コマンドを実行すると、カレントディレクトリ以下に、CDK の実行に必要な一連のファイルが生成されます。
今回は、TypeScript で CDK のコードを記述したいので、`--language` オプションで `typescript` を指定していますが、`python` なども指定できます。

{{< code >}}
$ cdk init app --language typescript
Applying project template app for typescript
# Welcome to your CDK TypeScript project!
...
✅ All done!
{{< /code >}}

{{< note title="TypeScript 以外の言語について" >}}
CDK を使ったコードは、TypeScript 以外にも Python、Java、C#、Go など、色々な言語で記述できるようになっていますが、CDK ライブラリ本体は TypeScript で実装されていて、他の言語用には JSII によるバインディングを行っています。
特にこだわりがなければ TypeScript で記述しておけばよいですが、Python を使っている例もよく見られます。
{{< /note >}}

言語として TypeScript を選んだ場合は、次のようなファイル群が生成されます（これら全体が CDK アプリと呼ばれます）。
CDK によるデプロイを行う場合、このディレクトリ以下で `cdk deploy` コマンドを実行することになります。

{{< accordion title="cdk init app で自動生成されるファイル群" >}}
{{< code >}}
myapp/
  +-- .gitignore
  +-- .npmignore
  +-- bin/
  +-- cdk.json
  +-- jest.config.js
  +-- lib/
  +-- node_modules/
  +-- package-lock.json
  +-- package.json
  +-- README.md
  +-- test
  +-- tsconfig.json
{{< /code >}}
{{< /accordion >}}

### cdk deploy でスタックを作成する

この時点で、__`cdk deploy`__ コマンドを実行する準備が整っています。
デフォルトではディレクトリ名（ここでは `myapp`）に基づいて、`MyappStack` という名前の CloudFormation スタックが生成されるようになっていますが、スタック名は用途に応じて適切な名前を付けた方がよいでしょう（後述）。
実際にデプロイできるか試してみたければ次のように実行します（ほぼ空っぽのスタックが生成されます）。

1. __`cdk deploy`__ コマンドを実行してデプロイ実行（スタックの作成）
1. [CloudFormation コンソール](https://console.aws.amazon.com/cloudformation/) を開いて、`MyappStack` スタックが生成されていることを確認

{{% note title="CI/CD 環境上での cdk コマンド" %}}
`cdk init app` で作成された `package.json` には、`devDependencies` として `aws-cdk` をインストールするように記述されています。
また、NPM スクリプトとして `cdk` コマンドが定義されています。
よって、GitHub Actions や AWS CodeBuild などの CI/CD 環境上で `cdk` コマンドを実行したければ、`npm ci` で依存モジュールをインスト―ルしてから、`npm run cdk ...` のように実行すれば OK です。
`npx` コマンドでダイレクトに `aws-cdk` を実行する方法もありますが、いずれにしても `npm ci` による依存モジュールのインストールは必要になります。
{{% /note %}}

### S3 バケットを追加してみる

`cdk init app` コマンドで CDK アプリのテンプレートを作成した場合、スタックの定義は __`lib`__ ディレクトリ以下の `.ts` ファイルで実装されています。
デフォルトでは、次のような内容の `Stack` サブクラスが定義されていますが、ここに必要な AWS リソースを追加していくことになります。
ここでは、ファイル名が `myapp-stack.ts`、クラス名が `MyappStack` となっていますが、これは親ディレクトリの名前 (`myapp`) から自動生成されています。

{{< code lang="ts" hl_lines="7" title="lib/myapp-stack.ts" >}}
import * as cdk from "@aws-cdk/core";

export class MyappStack extends cdk.Stack {
  constructor(scope: cdk.Construct, id: string, props?: cdk.StackProps) {
    super(scope, id, props);

    // The code that defines your stack goes here
  }
}
{{< /code >}}

S3 バケットなど、具体的な AWS リソースを生成するためには、専用のモジュール（コンストラクトライブラリ）をインストールする必要があります。
S3 の場合は [@aws-cdk/aws-s3](https://docs.aws.amazon.com/cdk/api/latest/docs/aws-s3-readme.html) をインストールします（`Stack` クラスなどのコアモジュールを提供する `@aws-cdk/core` は、`cdk init` を実行したときに自動インストールされています）。

{{< code >}}
$ npm install @aws-cdk/aws-s3
{{< /code >}}

例えば、カレントスタック (`this`) に S3 バケットを追加するには、次のように実装します。
`my-bucket` という名前はこのスタック階層内でのみ一意であればよい論理 ID であり、実際に AWS リソースが生成されるときにはもう少し複雑な物理 ID（例: `myappstack-mybucket15d133bf-c3w1q6f0j2xt`）が割り当てられるので注意してください。
このあたりの仕組みは CloudFormation と同様です。

{{< code lang="ts" hl_lines="2 9" title="lib/cdk-stack.ts" >}}
import * as cdk from '@aws-cdk/core';
import * as s3 from '@aws-cdk/aws-s3';

export class MyappStack extends cdk.Stack {
  constructor(scope: cdk.Construct, id: string, props?: cdk.StackProps) {
    super(scope, id, props);

    // The code that defines your stack goes here
    new s3.Bucket(this, 'my-bucket')
  }
}
{{< /code >}}

この修正で既存のスタックにどのような変更が行われるかは、__`cdk diff`__ コマンドで確認できます。

{{< code >}}
$ cdk diff
Stack MyappStack
Resources
[+] AWS::S3::Bucket my-bucket mybucket15D133BF
{{< /code >}}

プレフィックスとして `+` が付いたリソースが追加されることを示しています。
問題なさそうなので、再度 __`cdk deploy`__ コマンドを実行して、実際に CloudFormation スタックに反映します。

{{< code >}}
$ cdk deploy
MyappStack: deploying...
{{< /code >}}

{{< note title="TypeScript のビルドは必要ない？" >}}
CDK のコードを TypeScript で記述していますが、CDK CLI のコマンド（`cdk diff` や `cdk deploy`）を実行する前にトランスパイルしておく必要はありません。
これは、`ts-node` によって TypeScript コードを直接実行するようになっているからです。
先にビルドエラーを確認しておきたければ、明示的に `npm run build` (`tsc`) することも可能です。
{{< /note >}}

### cdk destroy でスタックを削除する

CDK の HelloWorld が終了したら、CloudFormation スタックを忘れずに削除しておきましょう。
__`cdk destory`__ コマンドでスタックごと削除できます。

{{< code >}}
$ cdk destroy
Are you sure you want to delete: MyappStack (y/n)? y
MyappStack: destroying...

 ✅  MyappStack: destroyed
{{< /code >}}


### （おまけ）確認なしでデプロイ実行する

`cdk deploy` を実行したときにスタックの情報が更新される場合は、y/n の確認プロンプトが表示されます。
GitHub Actions や CodeBuild などでデプロイを自動化したいときは、この確認で止まってしまうと都合が悪いので、__`--require-approval never`__ オプションを指定してすべて y を選択したように動作させることができます。

{{< code >}}
$ cdk deploy --require-approval never
{{< /code >}}


GitHub にプッシュする
----

CDK アプリを `cdk init app` コマンドで生成した場合は、Git リポジトリ (`.git`) も自動生成されて、初期コミットも済んでいる状態になっています。
これをベースに開発を続けていく場合は、GitHub などにプッシュしておきましょう。
すでにコミット内容があるので、GitHub のリポジトリを作成するときは、次のように空のリポジトリとして作成します。

1. GitHub の [New repository](https://github.com/new) を選択して新規リポジトリ作成画面を開く
2. `Initialize this repository with:` の項目では何もチェックを入れず、__`Create repository`__ ボタンを押す。

あとは、必要があればローカル編集をコミットし、

{{< code >}}
$ git add .
$ git commit
{{< /code >}}

次のように GitHub にプッシュすれば OK です。

{{< code >}}
$ git remote add origin https://github.com/<UserName>/<RepoName>.git
$ git branch -M main
$ git push -u origin main
{{< /code >}}


参考情報
----

上記の例では必要ありませんでしたが、CDK のデプロイ (`cdk deploy`) を行うとき、ブートストラップが必要だと表示されることがあります。
その場合は、下記を参考にして `cdk bootstrap` を実行してください。

- [AWS CDK メモ: ブートストラップ処理を実行する (cdk bootstrap)](/p/q7q8p6m)

CDK アプリを TypeScript 用に生成すると、デフォルトでは NPM が使われるようになっています。
Yarn を使いたい場合は下記を参考にしてください。

- [AWS CDK メモ: CDK アプリのパッケージ管理に Yarn を使う方法](/p/4h3jygw)

