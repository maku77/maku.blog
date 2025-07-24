---
title: "Azure: Node.js アプリを App Service へデプロイする（Kudu ビルド編）"
url: "p/wx3fvib/"
date: "2019-10-09"
tags: ["Azure", "Azure App Service", "Azure Pipelines"]
---

Azure App Service には、**デプロイセンター** という仕組みがあり、そこからソースコードのビルドからデプロイまでの自動化の設定を行うことができます。
簡単に言うと、Azure Repos や GitHub のリポジトリに最新の JavaScript コードをコミットするだけで、最新の Node.js アプリが自動で立ち上がるようになります。

ビルドの仕組みとしては、クラウド上の Azure Pipelines を使ったり、ローカルでビルドしてしまってからデプロイする方法があります。
App Service には [Kudu エンジン](https://gihub.com/projectkudu/kudu/wiki) が組み込まれており、デプロイ時に自動で実行されるスクリプトを使って簡易的なビルド処理を行うこともできます。


はじめに（用語定義）
----

Azure ではデプロイ処理を構成するコンポーネントを下記のような名前で呼んでいます。

デプロイソース
: GitHub や Azure Repos など。
ソースコードが置いてある場所（リポジトリ）のこと。
Azure App Service は手軽なデプロイソースとして OneDrive や Dropbox などのフォルダ共有サービスを設定することもできますが、本格的な運用で使用することは推奨されていません。

ビルドパイプライン（ビルドプロバイダー）
: Azure Pipelines など。デプロイソースからソースコードを取得し、一連のビルド処理を行う仕組み。
App Service には組み込みで Kudu エンジンが搭載されており、デフォルトではデプロイ時にこの Kudu エンジンによって `npm install` などが実行されるようになっています。
また、デプロイ時に実行する [カスタムスクリプトを .deployment ファイルで定義する](https://github.com/projectkudu/kudu/wiki/Deployment-hooks) こともできます。
これらの仕組みだけで十分であれば、Azure Pipelines を使う必要はありません。

デプロイメカニズム
: ビルドしたアプリを Azure App Service などに配置するためのアクション。Kudu エンジンや FTP (SFTP)、WebDeploy などのデプロイメカニズムが提供されています。


リポジトリの準備 (Azure Repos)
----

ここでは、Azure Repos に Git リポジトリを作成し、そこに Node.js アプリのコードをコミットしてあると想定します。
GitHub でも大丈夫です。

{{% maku-common/reference %}}
- [Azure DevOps で無料のプライベート Git リポジトリ (Repos) を使用する](/p/qt5qyzu)
{{% /maku-common/reference %}}


デプロイセンターの設定 (Azure Repos + Kudu)
----

Azure ポータルからデプロイ先の App Service を選択し、<samp>デプロイセンター</samp> を開くと、使用するリポジトリやビルドパイプラインの設定を行えます。

{{< image border="true" src="deploy-kudu-001.png" >}}

まずは、使用するソースコードのリポジトリを設定します。
上記の例では、<samp>Azure Repos</samp> を選択していますが、<samp>GitHub</samp> を選択することもできます。
<samp>Local Git</samp> を選択すると、App Service 内の Git リポジトリに対して、開発者のローカル Git リポジトリから直接 push するというデプロイスタイルになります（そのためのリポジトリアドレスは後から表示されます）。
ただ、大本のソースコードは Azure Repos や GitHub で管理しておくのが望ましいでしょう。

{{< image w="600" border="true" src="deploy-kudu-002.png" >}}

次に、ビルドエンジンとして App Service 組み込みの Kudu の仕組みを使用するか、Azure Pipelines を使用するかを選択します。
ここでは、Kudo を選択します。

{{< image w="600" border="true" src="deploy-kudu-003.png" >}}

あとは使用する Azure DevOps のプロジェクト名や Git リポジトリ名を選択すれば、デプロイセンターの設定は完了です。
ここでは最初のステップで Azure Repos を選択したのでこの画面が表示されていますが、GitHub を選択していれば、GitHub の organization やリポジトリ名を選択する画面が表示されます。

この設定が完了すると、自動的に最新のコミットがデプロイされます。
デプロイ先のディレクトリパスは気にする必要はありませんが、App Service 内部では `D:\home\site\wwwroot` のようなパスになっています。

Node.js アプリであれば、その後、自動的に `npm install` などが実行され、`index.js` などが起動します。
つまり、これだけの設定で、Azure Repos や GitHub のリポジトリにソースコードをデプロイするだけで、最新のコードで Node.js アプリが起動するという自動化が完了します。


（応用） Kudu によるビルド／デプロイ処理
----

デプロイセンターのログのリンクを辿ると、リポジトリからのコード取得後にどのような処理が行われたかのログを見ることができます。
例えば、こんな感じ。

{{< code >}}
Command: deploy.cmd
Handling node.js deployment.
Creating app_offline.htm
KuduSync.NET from: 'D:\home\site\repository' to: 'D:\home\site\wwwroot'
Deleting file: '.env'
Deleting file: 'bot.js'
Deleting file: 'index.js'
Copying file: '.eslintrc.js'
Copying file: '.gitignore'
Copying file: 'iisnode.yml'
Copying file: 'package-lock.json'
Copying file: 'package.json'
Copying file: 'publish.cmd'
Copying file: 'README.md'
Copying file: 'tsconfig.json'
Deleting file: 'mylibs\util.js'
Copying file: 'src\bot.js'
Copying file: 'src\index.js'

Deleting app_offline.htm
Start script "index.js" from package.json
The iisnode.yml file explicitly sets nodeProcessCommandLine. Automatic node.js version selection is turned off.
Selected npm version 6.4.1
added 1 package from 1 contributor and audited 4520 packages in 37.513s
found 62 vulnerabilities (1 low, 1 moderate, 57 high, 3 critical)
  run `npm audit fix` to fix them, or `npm audit` for details
Finished successfully.
{{< /code >}}

### カスタムデプロイスクリプト (.deployment / deploy.cmd)

Kudu は、リポジトリのルートに置かれた `.deployment` ファイルに従い、デプロイ時にカスタムスクリプトを実行します。

{{< code lang="ini" title=".deployment" >}}
[config]
command = deploy.cmd
{{< /code >}}

通常は、このように記述されており、`deploy.cmd` というバッチファイルが実行されるようになっています。
これらのファイルは、App Service 用のアプリをテンプレートから作成するとき、あるいは最初に [Kudu によるデプロイ処理が走るときに自動的に生成される](https://github.com/projectkudu/kudu/wiki/Custom-Deployment-Script#easily-downloading-your-current-deployment-script) ようです。

自動生成された `deploy.cmd` の中身を見てみると、例えば下記のような感じで記述されています。

{{< code lang="bat" title="deploy.cmd（抜粋）" >}}
:Deployment
echo Handling node.js deployment.

:: 1. KuduSync
IF /I "%IN_PLACE_DEPLOYMENT%" NEQ "1" (
  call :ExecuteCmd "%KUDU_SYNC_CMD%" -v 50 -f "%DEPLOYMENT_SOURCE%" -t "%DEPLOYMENT_TARGET%" -n "%NEXT_MANIFEST_PATH%" -p "%PREVIOUS_MANIFEST_PATH%" -i ".git;.hg;.deployment;deploy.cmd"
  IF !ERRORLEVEL! NEQ 0 goto error
)

:: 2. Select node version
call :SelectNodeVersion

:: 3. Install npm packages
IF EXIST "%DEPLOYMENT_TARGET%\package.json" (
  pushd "%DEPLOYMENT_TARGET%"
  call :ExecuteCmd !NPM_CMD! install --production
  IF !ERRORLEVEL! NEQ 0 goto error
  popd
)
{{< /code >}}

コード中に出てくる変数には、例えば下記のようなディレクトリのパスが格納されます。

- <b>DEPLOYMENT_SOURCE</b>: リポジトリのコード (`D:\home\site\repository`)
- <b>DEPLOYMENT_TARGET</b>: デプロイ先のルート (`D:\home\site\wwwroot`)

大まかには、リポジトリのコードをデプロイ先にコピーしておいて、`npm install --production` で必要な NPM パッケージをインストールするという流れになっています。
上記の後ろあたりに、

{{< code lang="bat" >}}
echo Hellooooooooooooooooooooooo
{{< /code >}}

のような感じで追記しておくと、デプロイ時にメッセージがログに出力されることを確認できます。

デプロイ時に TypeScript のトランスパイルの実行などを行いたい場合は、下記のように `npm run build` を実行する感じでしょうか。
この場合は `npm install --production` となっていた部分を、`npm install` に変更しておかなければいけません。
`tsc` などの開発ツールをインストールするためです。

{{< code lang="bat" title="deploy.cmd（抜粋）" >}}
:: 3. Install npm packages AND transpile TypeScript files
IF EXIST "%DEPLOYMENT_TARGET%\package.json" (
  pushd "%DEPLOYMENT_TARGET%"

  call :ExecuteCmd !NPM_CMD! install
  IF !ERRORLEVEL! NEQ 0 goto error
  call :ExecuteCmd !NPM_CMD! run build
  IF !ERRORLEVEL! NEQ 0 goto error

  popd
)
{{< /code >}}


### カスタムデプロイスクリプトのテンプレート

Kudu のカスタムデプロイスクリプト（`.deployment` と `deploy.cmd`）は、**`kuduscript`** コマンドを使って簡単に生成することができます。
下記の例では、Node.js アプリ用のデプロイスクリプトを生成しています。

{{< code lang="console" title="Node.js アプリ用の Kudu デプロイスクリプトの作成" >}}
$ npm install -g kuduscript
$ kuduscript --node -y
Generating deployment script for node.js Web Site
Generated deployment script files
{{< /code >}}

生成されたファイルはこちら。

- {{< file src="deploy-kudu-dot-deployment.txt" caption=".deployment" >}}
- {{< file src="deploy-kudu-deploy-cmd.txt" caption="deploy.cmd" >}}

これらのファイルをリポジトリのルートにコミットしておけば、App Service へのデプロイ時に自動的に実行されるようになります。


### カスタムデプロイスクリプトが実行されるトリガー

ちなみに、オンラインコードエディタ (App Service Editor) 上でコードを直接編集した場合は、`.deployment` によるカスタムスクリプトは呼び出されないようです。
このあたりの起動条件がはっきりしないので Microsoft さんに問い合わせたところ、下記のようなパターンで Web フックによりカスタムスクリプトがキックされるようです。

- App Service の <samp>デプロイセンター</samp> を設定して継続的なデプロイを有効にした状態でリポジトリに Push したとき
- App Service の環境変数で `SCM_DO_BUILD_DURING_DEPLOYMENT` を `true` に設定し、ZIP デプロイを行ったとき（参考: [ZIP または WAR ファイルを使用した Azure App Service へのアプリのデプロイ](https://docs.microsoft.com/ja-jp/azure/app-service/deploy-zip)）

いずれにしても継続的デプロイの仕組みがオススメです。
Repos への Push 時にいきなりアプリが公開されてしまうのが心配なときは、App Service で <samp>デプロイスロット</samp> を設定しておけば OK です。
いわゆるブルーグリーンデプロイメントの仕組みで、ステージング環境を使って事前にテストできるようになります。


Kudu で TypeScript ビルドとかはキツい
----

`deploy.cmd` あたりをカスタマイズすれば、TypeScript のトランスパイル処理などもこの中でできそう！
と思いましたが、すでにバリバリ記述されているバッチファイルを編集するのは厳しそうです。

そもそもテンプレートで実行されている `npm install` は `--production` オプションが指定されており、これはデプロイ後のサーバー上で実行されることを前提としています。
TypeScript のトランスパイラを実行するのであれば、事前に開発用に `npm install` を実行してインストールしておかなければいけません。
やはり、`npm install` 以上のビルド処理を行う場合は、別の仕組み (Azure Pipeline) を利用したほうがよさそうです。

結論としては、こんな感じで使い分ければよいと思います。

- <b>Kudu ビルド</b>: Git リポジトリに実行可能な index.js などをコミットする場合
- <b>Azure Pipelines ビルド</b>: Git リポジトリに TypeScript ファイルなどをコミットし、クラウド上で JavaScript に変換したい場合

Kudu によるビルドは、CI/CD サービスを追加しなくて済むのである意味手軽ですが、実行環境上で初めて実行可能なアプリが生成されるため、どうしてもエイヤッ的なリリースになってしまいます。
また、その最終成果物をそのまま別のサーバーに展開するというポータビリティもありません。
なので、できればビルドとリリースを分離できる Azure Pipelines の使用をオススメするというのが Microsoft さんの見解のようです。
同様の機能を提供しようとしている GitHub Actions も気になりますね。

