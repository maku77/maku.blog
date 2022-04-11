---
title: "AWS のサービス一覧（2021年版）"
url: "/p/t8fmsz5"
date: "2021-01-25"
tags: ["AWS"]
---

AWS のサービスを概要説明付きでリスト化したものが欲しくて作っちゃったので置いておきます。
{{< file src="aws-services.pdf" caption="PDF 化したもの" >}} も置いておきます。

<style>
.local-aws td {
  padding: 0 0 0.2em 0.2em;
  background: #fafafa;
}
.local-aws th {
  padding-top: 1.5em;
  border: none;
  background: white;
  text-align: left;
}
.local-aws img {
  max-width: 1.5em;
}
</style>
<table class="local-aws">
  <tbody>
    <tr><th colspan="2">分析 (Analytics)</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Athena_16.svg" >}}Amazon Athena</td>
      <td>SQL を使用した S3 でのデータクエリ</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-CloudSearch_16.svg" >}}Amazon CloudSearch</td>
      <td>マネージド型検索サービス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Elasticsearch-Service_16.svg" >}}Amazon Elasticsearch Service</td>
      <td>Elasticsearch クラスターを実行し、スケールする</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-EMR_16.svg" >}}Amazon EMR</td>
      <td>ホスト型 Hadoop フレームワーク</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Kinesis_16.svg" >}}Amazon Kinesis</td>
      <td>リアルタイムストリーミングデータとの連携</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Managed-Streaming-for-Kafka_16.svg" >}}Amazon Managed Streaming for Apache Kafka</td>
      <td>フルマネージド型 Apache Kafka サービス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Redshift_16.svg" >}}Amazon Redshift</td>
      <td>高速かつシンプルで、費用対効果の高いデータウェアハウス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-QuickSight_16.svg" >}}Amazon QuickSight</td>
      <td>高速ビジネス分析サービス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Data-Exchange_16.svg" >}}AWS Data Exchange</td>
      <td>クラウド内サードパーティのデータを検索、購読、および使用</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Data-Pipeline_16.svg" >}}AWS Data Pipeline</td>
      <td>定期的なデータ駆動型ワークフローに対するオーケストレーションサービス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Glue_16.svg" >}}AWS Glue</td>
      <td>シンプルでスケーラブルなサーバーレスデータ統合</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Lake-Formation_16.svg" >}}AWS Lake Formation</td>
      <td>安全なデータレイクを数日で構築</td>
    </tr>
    <tr><th colspan="2">Application Integration（アプリケーション統合）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Step-Functions_16.svg" >}}AWS Step Functions</td>
      <td>分散アプリケーションの調整</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-AppFlow_16.svg" >}}Amazon AppFlow</td>
      <td>SaaS アプリケーションと AWS のサービス向けのコード統合が不要</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-EventBridge_16.svg" >}}Amazon EventBridge</td>
      <td>SaaS アプリと AWS のサービス向けサーバーレスイベントバス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}Amazon Managed Workflows for Apache Airflow</td>
      <td>可用性に優れたセキュアなマネージドワークフローオーケストレーション</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-MQ_16.svg" >}}Amazon MQ</td>
      <td>マネージド型メッセージブローカーサービス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Simple-Notification-Service_16.svg" >}}Amazon Simple Notification Service (SNS)</td>
      <td>Pub/sub、SMS、E メール、およびモバイルプッシュ通知</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Simple-Queue-Service_16.svg" >}}Amazon Simple Queue Service (SQS)</td>
      <td>マネージド型メッセージキュー</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-AppSync_16.svg" >}}Amazon AppSync</td>
      <td>多くのソースから適切なデータを使用して、大規模にアプリを強化</td>
    </tr>
    <tr><th colspan="2">AWS Cost Management（AWS コスト管理）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Cost-Explorer_16.svg" >}}AWS Cost Explorer</td>
      <td>AWS のコストと使用状況を分析する</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Budgets_16.svg" >}}AWS 予算</td>
      <td>カスタムコストと使用予算を設定する</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Cost-and-Usage-Report_16.svg" >}}AWS のコストと使用状況レポート</td>
      <td>包括的なコストと使用状況情報へのアクセス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Reserved-Instance-Reporting_16.svg" >}}リザーブドインスタンスレポート</td>
      <td>リザーブドインスタンス (RI) の詳細を把握する</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Savings-Plans_16.svg" >}}Savings Plans</td>
      <td>柔軟な料金設定でコンピューティング使用コストを最大 72% 節約</td>
    </tr>
    <tr><th colspan="2">Blockchain （ブロックチェーン）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Managed-Blockchain_16.svg" >}}Amazon Managed Blockchain</td>
      <td>スケーラブルなブロックチェーンネットワークを作成および管理</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Quantum-Ledger-Database_16.svg" >}}Amazon Quantum Ledger Database (QLDB)</td>
      <td>フルマネージド型台帳データベース</td>
    </tr>
    <tr><th colspan="2">Business Application（ビジネスアプリケーション）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_Alexa-For-Business_16.svg" >}}Alexa for Business</td>
      <td>Alexa を使って組織を強化</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Chime_16.svg" >}}Amazon Chime</td>
      <td>フラストレーションフリーの会議、ビデオ電話、チャット</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}Amazon Honeycode (ベータ)</td>
      <td>プログラミングなしでモバイルおよびウェブアプリケーションを構築</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-WorkDocs_16.svg" >}}Amazon WorkDocs</td>
      <td>エンタープライズドキュメントの安全なストレージと共有</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-WorkMail_16.svg" >}}Amazon WorkMail</td>
      <td>セキュリティで保護されたマネージド型の企業向け E メールおよびカレンダー</td>
    </tr>
    <tr><th colspan="2">Compute（コンピューティング）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-EC2_16.svg" >}}Amazon EC2</td>
      <td>クラウド内の仮想サーバー</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-EC2-Auto-Scaling_16.svg" >}}Amazon EC2 Auto Scaling</td>
      <td>需要に合わせてコンピューティング性能をスケール</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Lightsail_16.svg" >}}Amazon Lightsail</td>
      <td>仮想プライベートサーバーを起動および管理</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Batch_16.svg" >}}AWS Batch</td>
      <td>あらゆる規模でバッチジョブを実行</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Elastic-Beanstalk_16.svg" >}}AWS Elastic Beanstalk</td>
      <td>ウェブアプリの実行と管理</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Lambda_16.svg" >}}AWS Lambda</td>
      <td>イベント発生時にコードを実行</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Outposts_16.svg" >}}AWS Outposts</td>
      <td>AWS サービスをオンプレミスで実行</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Serverless-Application-Repository_16.svg" >}}AWS Serverless Application Repository</td>
      <td>サーバーレスアプリケーションを検索、デプロイ、公開する</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}AWS Snow ファミリー</td>
      <td>エッジロケーションでデータを集約および処理して AWS に転送するデバイス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Wavelength_16.svg" >}}AWS Wavelength</td>
      <td>5G デバイスのための超低レイテンシーアプリケーションを提供</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_VMware-Cloud-on-AWS_16.svg" >}}VMware Cloud on AWS</td>
      <td>カスタムハードウェアを使用せずにハイブリッドクラウドを構築する</td>
    </tr>
    <tr><th colspan="2">Containers（コンテナ）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Elastic-Container-Registry_16.svg" >}}Amazon Elastic Container Registry</td>
      <td>コンテナイメージを簡単に保存、管理、デプロイ</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Elastic-Container-Service_16.svg" >}}Amazon Elastic Container Service (ECS)</td>
      <td>コンテナを実行するためのきわめて安全で信頼性と拡張性が高い方法</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}Amazon ECS Anywhere (近日公開)</td>
      <td>インフラストラクチャでの ECS</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Elastic-Container-Kubernetes_16.svg" >}}Amazon Elastic Kubernetes Service (EKS)</td>
      <td>信頼性が最も高い Kubernetes の実行方法</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}Amazon EKS Anywhere (近日公開)</td>
      <td>インフラストラクチャでの Kubernetes</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}Amazon EKS Distro</td>
      <td>一貫した Kubernetes クラスターの実行</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}AWS App2Container</td>
      <td>既存のアプリケーションのコンテナ化と移行</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Fargate_16.svg" >}}AWS Fargate</td>
      <td>コンテナ向けサーバーレスコンピューティング</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}AWS での Red Hat OpenShift</td>
      <td>マネージド Red Hat OpenShift クラスター</td>
    </tr>
    <tr><th colspan="2">Customer Engagement（カスタマーエンゲージメント）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Connect_16.svg" >}}Amazon Connect</td>
      <td>クラウドベースのコンタクトセンター</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Pinpoint_16.svg" >}}Amazon Pinpoint</td>
      <td>チャンネル間でのパーソナライズされたユーザーエンゲージメント</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Simple-Email-Service_16.svg" >}}Amazon Simple Email Service (SES)</td>
      <td>E メールの送受信</td>
    </tr>
    <tr><th colspan="2">Database（データベース）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Aurora_16.svg" >}}Amazon Aurora</td>
      <td>高性能マネージドリレーショナルデータベース</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}Amazon Aurora Serverless v2 (プレビュー)</td>
      <td>毎秒 100,000 件を超えるトランザクションに瞬時にスケール</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-DynamoDB_16.svg" >}}Amazon DynamoDB</td>
      <td>マネージド型の NoSQL データベース</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-DocumentDB_16.svg" >}}Amazon DocumentDB (MongoDB 互換)</td>
      <td>フルマネージド型ドキュメントデータベース</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-ElastiCache_16.svg" >}}Amazon ElastiCache</td>
      <td>インメモリキャッシングシステム</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Keyspaces_16.svg" >}}Amazon Keyspaces (Apache Cassandra 用)</td>
      <td>マネージド型の Cassandra 対応データベース</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Neptune_16.svg" >}}Amazon Neptune</td>
      <td>フルマネージド型グラフデータベースサービス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Quantum-Ledger-Database_16_b.svg" >}}Amazon Quantum Ledger Database (QLDB)</td>
      <td>フルマネージド型台帳データベース</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-RDS_16.svg" >}}Amazon RDS</td>
      <td>MySQL、PostgreSQL、Oracle、SQL Server、MariaDB 向けのマネージドリレーショナルデータベースサービス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-RDS-for-VMware_16.svg" >}}Amazon RDS on VMware</td>
      <td>オンプレミスデータベースの管理を自動化</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Redshift_16_b.svg" >}}Amazon Redshift</td>
      <td>高速、シンプル、費用対効果の高いデータウェアハウジング</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Timestream_16.svg" >}}Amazon Timestream</td>
      <td>フルマネージド型の時系列データベース</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Database-Migration_16.svg" >}}AWS Database Migration Service</td>
      <td>最小限のダウンタイムでデータベースを移行</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Glue_16.svg" >}}AWS Glue</td>
      <td>シンプルでスケーラブルなサーバーレスデータ統合</td>
    </tr>
    <tr><th colspan="2">Developer Tools（デベロッパーツール）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-CodeGuru_16.svg" >}}Amazon CodeGuru</td>
      <td>最もコストがかかるコード行を見つける</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}Amazon Corretto</td>
      <td>本番環境に向けて OpenJDK を配信</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Cloud-Development-Kit_16.svg" >}}AWS Cloud Development Kit (CDK)</td>
      <td>コードを使用してクラウドインフラストラクチャをモデル化する</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Cloud9_16.svg" >}}AWS Cloud9</td>
      <td>Cloud IDE でコードを記述、実行、デバッグ</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}AWS CloudShell</td>
      <td>ブラウザベースのシェル環境</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-CodeArtifact_16.svg" >}}AWS CodeArtifact</td>
      <td>ソフトウェア開発のためのセキュアかつスケーラブルでコスト効率性に優れたアーティファクト管理</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-CodeBuild_16.svg" >}}AWS CodeBuild</td>
      <td>コードのビルドとテスト</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-CodeCommit_16.svg" >}}AWS CodeCommit</td>
      <td>プライベート Git リポジトリでのコードの保存</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-CodeDeploy_16.svg" >}}AWS CodeDeploy</td>
      <td>コードデプロイの自動化</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-CodePipeline_16.svg" >}}AWS CodePipeline</td>
      <td>継続的デリバリーを使用したソフトウェアのリリース</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Codestar_16.svg" >}}AWS CodeStar</td>
      <td>AWS アプリケーションの開発とデプロイ</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Command-Line-Interface_16.svg" >}}AWS コマンドラインインターフェイス</td>
      <td>AWS サービスを管理するための統合ツール</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Device-Farm_16.svg" >}}AWS Device Farm</td>
      <td>AWS クラウド内の実際のデバイスを使った Android、iOS、ウェブアプリケーションのテスト</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}AWS Fault Injection Simulator</td>
      <td>完全マネージド型のカオスエンジニアリングサービス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Tools-and-SDK_16.svg" >}}AWS ツールと SDK</td>
      <td>AWS のためのツールと SDK</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-X-Ray_16.svg" >}}AWS X-Ray</td>
      <td>アプリケーションの分析とデバッグ</td>
    </tr>
    <tr><th colspan="2">End User Computing（エンドユーザーコンピューティング）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-App-Stream_16.svg" >}}Amazon AppStream 2.0</td>
      <td>デスクトップアプリケーションを安全にブラウザへストリーミングするサービスです</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-WorkDocs_16_b.svg" >}}Amazon WorkDocs</td>
      <td>エンタープライズドキュメントの安全なストレージと共有</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-WorkLink_16.svg" >}}Amazon WorkLink</td>
      <td>社内のウェブサイトへのモバイルアクセスを可能にする</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-WorkSpaces_16.svg" >}}Amazon WorkSpaces</td>
      <td>デスクトップコンピューティングサービス</td>
    </tr>
    <tr><th colspan="2">Font-End Web & Mobile（ウェブとモバイルのフロントエンド）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Amplify-Console_16.svg" >}}AWS Amplify</td>
      <td>モバイルおよびウェブアプリケーションの構築とデプロイ</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-API-Gateway_16.svg" >}}Amazon API Gateway</td>
      <td>API を構築し、デプロイし、管理する</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}Amazon Location Service (プレビュー)</td>
      <td>アプリケーションにロケーションデータをセキュアかつ簡単に追加</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Pinpoint_16_b.svg" >}}Amazon Pinpoint</td>
      <td>チャンネル間でのパーソナライズされたユーザーエンゲージメント</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-AppSync_16_b.svg" >}}AWS AppSync</td>
      <td>多くのソースから適切なデータを使用して、大規模にアプリを強化</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Device-Farm_16.svg" >}}AWS Device Farm</td>
      <td>AWS クラウド内の実際のデバイスを使った Android、iOS、ウェブアプリケーションのテスト</td>
    </tr>
    <tr><th colspan="2">Game Tech</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-GameLift_16.svg" >}}Amazon GameLift</td>
      <td>シンプルで高速な費用対効果の高い専用ゲームサーバーホスティング</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Lumberyard_64.svg" >}}Amazon Lumberyard</td>
      <td>AWS や Twitch と統合された完全なソースを利用できる、無料のクロスプラットフォーム 3D ゲームエンジンです。</td>
    </tr>
    <tr><th colspan="2">Internet of Things（IoT: モノのインターネット）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-IoT-Core_16.svg" >}}AWS IoT Core</td>
      <td>デバイスをクラウドに接続</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-IoT-Greengrass-Core_16.svg" >}}AWS Greengrass</td>
      <td>デバイスのローカルでのコンピューティング、メッセージング、および同期</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-IoT-1-Click_16.svg" >}}AWS IoT 1-Click</td>
      <td>AWS Lambda トリガーのワンクリック作成</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-IoT-Analytics_16.svg" >}}AWS IoT Analytics</td>
      <td>IoT デバイスの分析</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-IoT-Button_16.svg" >}}AWS IoT ボタン</td>
      <td>クラウドのプログラミング可能なダッシュボタン</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-IoT-Device-Defender_16.svg" >}}AWS IoT Device Defender</td>
      <td>IoT デバイスのセキュリティ管理</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-IoT-Device-Management_16.svg" >}}AWS IoT Device Management</td>
      <td>IoT デバイスのオンボード、編成、リモート管理</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-IoT-Events_16.svg" >}}AWS IoT Events</td>
      <td>IoT イベントを検出し、対応</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-IoT-SiteWise_16.svg" >}}AWS IoT SiteWise</td>
      <td>IoT データコレクターおよびインタプリタ</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-IoT-Things-Graph_16.svg" >}}AWS IoT Things Graph</td>
      <td>デバイスおよびウェブサービスを簡単に接続</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}AWS Partner Device Catalog</td>
      <td>AWS 互換の IoT ハードウェアの精選カタログ</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-FreeRTOS_16.svg" >}}FreeRTOS</td>
      <td>マイクロコントローラ向けリアルタイムオペレーティングシステム</td>
    </tr>
    <tr><th colspan="2">Machine Learning（機械学習）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-SageMaker_16.svg" >}}Amazon SageMaker</td>
      <td>機械学習モデルを大規模に構築、トレーニング、デプロイ</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Augmented-AI-A2I_16.svg" >}}Amazon Augmented AI</td>
      <td>ML 予測のヒューマンレビューを簡単に導入</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-CodeGuru_16.svg" >}}Amazon CodeGuru</td>
      <td>最もコストがかかるコード行を見つける</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Comprehend_16.svg" >}}Amazon Comprehend</td>
      <td>テキストのインサイトや関係性を検出</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}Amazon DevOps Guru</td>
      <td>ML 駆動のクラウドオペレーションサービス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Elastic-Inference_16.svg" >}}Amazon Elastic Inference</td>
      <td>深層学習推論の高速化</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Forecast_16.svg" >}}Amazon Forecast</td>
      <td>機械学習を使用して予測の精度を向上させる</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Fraud-Detector_16.svg" >}}Amazon Fraud Detector</td>
      <td>オンライン詐欺をより素早く検知</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Kendra_16.svg" >}}Amazon Kendra</td>
      <td>ML を利用してエンタープライズ検索を改革</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Lex_16.svg" >}}Amazon Lex</td>
      <td>音声やテキストに対応するチャットボットの構築</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}Amazon Lookout for Equipment (プレビュー)</td>
      <td>センサーデータの分析による異常動作の検知</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}Amazon Lookout for Metrics</td>
      <td>メトリクス内における異常の検知</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}Amazon Lookout for Vision</td>
      <td>コンピュータビジョンを使用した製品欠陥の検出</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}Amazon Monitron</td>
      <td>機器モニタリングのためのエンドツーエンドシステム</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Personalize_16.svg" >}}Amazon Personalize</td>
      <td>アプリケーションへのリアルタイムレコメンデーションの構築</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Polly_16.svg" >}}Amazon Polly</td>
      <td>テキストを生きた話し声に変換</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Rekognition_16.svg" >}}Amazon Rekognition</td>
      <td>イメージとビデオを分析</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}Amazon SageMaker Data Wrangler</td>
      <td>ML 用にデータを準備するための最も速い方法</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-SageMaker-Ground-Truth_16.svg" >}}Amazon SageMaker Ground Truth</td>
      <td>精度の高い機械学習トレーニングデータセットの構築</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Textract_16.svg" >}}Amazon Textract</td>
      <td>ドキュメントからテキストやデータを抽出する</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Translate_16.svg" >}}Amazon Translate</td>
      <td>自然で流ちょうな言語翻訳</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Transcribe_16.svg" >}}Amazon Transcribe</td>
      <td>自動音声認識</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Deep-Learning-AMIs_16.svg" >}}AWS 深層学習 AMI</td>
      <td>EC2 で今すぐ深層学習を始める</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Deep-Learning-Containers_16.svg" >}}AWS Deep Learning Containers</td>
      <td>深層学習向け Docker イメージ</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Deep-Composer_16.svg" >}}AWS DeepComposer</td>
      <td>機械学習が有効化されたミュージカルキーボード</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Deep-Lense_16.svg" >}}AWS DeepLens</td>
      <td>深層学習に対応したビデオカメラ</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Deepracer_16.svg" >}}AWS DeepRacer</td>
      <td>機械学習による 18 分の 1 のスケールでの自律走行型レースカー</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}AWS Inferentia</td>
      <td>機械学習インファレンスチップ</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}AWS Panorama (プレビュー)</td>
      <td>エッジに設置したコンピュータビジョンによる運営改善</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_TorchServe_16.svg" >}}AWS での PyTorch</td>
      <td>柔軟なオープンソースの機械学習フレームワーク</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Apache-MXNet_16.svg" >}}AWS での Apache MXNet</td>
      <td>スケーラブルでパフォーマンスに優れた深層学習</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-TensorFlow-on-AWS_16.svg" >}}AWS での TensorFlow</td>
      <td>オープンソースの Machine Intelligence Library</td>
    </tr>
    <tr><th colspan="2">Management & Governance（マネジメントとガバナンス）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-CloudWatch_16.svg" >}}Amazon CloudWatch</td>
      <td>リソースとアプリケーションのモニタリング</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Auto-Scaling_16.svg" >}}AWS Auto Scaling</td>
      <td>需要に合わせて複数のリソースをスケール</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Chatbot_16.svg" >}}AWS Chatbot</td>
      <td>ChatOps for AWS</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-CloudFormation_16.svg" >}}AWS CloudFormation</td>
      <td>テンプレートを使ったリソースの作成と管理</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Cloud-Trail_16.svg" >}}AWS CloudTrail</td>
      <td>ユーザーアクティビティと API 使用状況の追跡</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Command-Line-Interface_16_b.svg" >}}AWS コマンドラインインターフェイス</td>
      <td>AWS サービスを管理するための統合ツール</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Compute-Optimizer_16.svg" >}}AWS Compute Optimizer</td>
      <td>最適な AWS コンピューティングリソースを特定</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Config_16.svg" >}}AWS Config</td>
      <td>リソースのインベントリと変更の追跡</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Control-Tower_16.svg" >}}AWS Control Tower</td>
      <td>安全かつ基準に準拠した複数のアカウント環境をセットアップおよび管理</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Console-Mobile-Application _16.svg" >}}AWS コンソールモバイルアプリ</td>
      <td>リソースの状態を外出先で確認</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}AWS Distro for OpenTelemetry (プレビュー)</td>
      <td>相関するメトリクスとトレースの収集</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-License-Manager_16.svg" >}}AWS License Manager</td>
      <td>ライセンスの追跡、管理、制御</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Management-Console_16.svg" >}}AWS マネジメントコンソール</td>
      <td>ウェブベースのユーザーインターフェイス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Managed-Services_16.svg" >}}AWS Managed Services</td>
      <td>AWS のインフラストラクチャ運用管理</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}Amazon Managed Service for Grafana</td>
      <td>強力でインタラクティブなデータ視覚化</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}Amazon Managed Service for Prometheus</td>
      <td>コンテナのためのセキュアで可用性に優れたモニタリング</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-OpsWorks_16.svg" >}}AWS OpsWorks</td>
      <td>Chef と Puppet を使用した運用の自動化</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Organizations_16.svg" >}}AWS Organizations</td>
      <td>AWS アカウント全体の一元管理</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Personal-Health-Dashboard_16.svg" >}}AWS Personal Health Dashboard</td>
      <td>AWS のサービス状態のパーソナライズされた表示</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}AWS Proton (プレビュー)</td>
      <td>コンテナとサーバーレスデプロイメントのための自動化された管理</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Service-Catalog_16.svg" >}}AWS Service Catalog</td>
      <td>標準化された製品の作成と使用</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Systems-Manager_16.svg" >}}AWS Systems Manager</td>
      <td>運用時の洞察を改善し、実行</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Trusted-Advisor_16.svg" >}}AWS Trusted Advisor</td>
      <td>パフォーマンスとセキュリティの最適化</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Well-Architect-Tool_16.svg" >}}AWS Well-Architected Tool</td>
      <td>ワークロードの見直しと改善</td>
    </tr>
    <tr><th colspan="2">Media Services（メディアサービス）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Elastic-Transcoder_16.svg" >}}Amazon Elastic Transcoder</td>
      <td>スケーラブルで使いやすいメディア変換サービス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Interactive-Video_16.svg" >}}Amazon Interactive Video Service</td>
      <td>マネージドライブ動画ソリューション</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Kinesis-Video-Streams_16.svg" >}}Amazon Kinesis Video Streams</td>
      <td>ビデオストリームの処理と分析</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Elemental-MediaConnect_16.svg" >}}AWS Elemental MediaConnect</td>
      <td>高い信頼性を安全性を持つライブ動画転送</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Elemental-MediaConvert_16.svg" >}}AWS Elemental MediaConvert</td>
      <td>ファイルベースのビデオコンテンツを変換</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Elemental-MediaLive_16.svg" >}}AWS Elemental MediaLive</td>
      <td>ライブビデオコンテンツを変換</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Elemental-MediaPackage_16.svg" >}}AWS Elemental MediaPackage</td>
      <td>動画の発信とパッケージ化</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Elemental-MediaStore_16.svg" >}}AWS Elemental MediaStore</td>
      <td>メディアストレージとシンプルな HTTP オリジン</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Elemental-MediaTailor_16.svg" >}}AWS Elemental MediaTailor</td>
      <td>動画のパーソナライズと収益化</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Elemental-Appliances-&-Software_16.svg" >}}AWS Elemental アプライアンスとソフトウェア</td>
      <td>オンプレミスメディアソリューション</td>
    </tr>
    <tr><th colspan="2">Migration & Transfer（移行と転送）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Migration-Hub_16.svg" >}}AWS Migration Hub</td>
      <td>複数の移行を 1 か所で追跡</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Application-Discovery-Service_16.svg" >}}AWS Application Discovery Service</td>
      <td>オンプレミスのアプリケーションを検出して合理的に移行</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Database-Migration_16_b.svg" >}}AWS Database Migration Service</td>
      <td>最小限のダウンタイムでデータベースを移行</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Data-Sync_16.svg" >}}AWS DataSync</td>
      <td>シンプルかつ高速なオンラインデータ転送</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Server-Migration-Service_16.svg" >}}AWS Server Migration Service</td>
      <td>AWS へのオンプレミスサーバーの移行</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}AWS Snow ファミリー</td>
      <td>AWS との間でデータを移行するためのデバイス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Transfer-Family_16.svg" >}}AWS Transfer Family</td>
      <td>フルマネージド SFTP、FTPS、および FTP サービス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-CloudEndure-Migration_16.svg" >}}CloudEndure Migration</td>
      <td>AWS への大規模な移行を自動化</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}Migration Evaluator（旧 TSO Logic）</td>
      <td>クラウド移行のビジネスケースを作成</td>
    </tr>
    <tr><th colspan="2">Networking & Content Delivery（ネットワーキングとコンテンツ配信）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Virtual-Private-Cloud_16.svg" >}}Amazon VPC</td>
      <td>独立したクラウドリソース</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-API-Gateway_16.svg" >}}Amazon API Gateway</td>
      <td>API を構築、デプロイ、管理</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-CloudFront_16.svg" >}}Amazon CloudFront</td>
      <td>グローバルコンテンツ配信ネットワーク</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Route-53_16.svg" >}}Amazon Route 53</td>
      <td>スケーラブルなドメインネームシステム (DNS)</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-PrivateLink_16.svg" >}}AWS PrivateLink</td>
      <td>AWS でホストされているサービスに安全にアクセス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-App-Mesh_16.svg" >}}AWS App Mesh</td>
      <td>マイクロサービスをモニタリングおよびコントロール</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-CloudMap_16.svg" >}}AWS Cloud Map</td>
      <td>マイクロサービス向けのアプリケーションリソースレジストリ</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Direct-Connect_16.svg" >}}AWS Direct Connect</td>
      <td>AWS への専用ネットワーク接続</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Global-Accelerator_16.svg" >}}AWS Global Accelerator</td>
      <td>アプリケーションの可用性とパフォーマンスを向上</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Transit-Gateway_16.svg" >}}AWS Transit Gateway</td>
      <td>VPC およびアカウント接続を簡単にスケール</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Elastic-Load-Balancing_16.svg" >}}Elastic Load Balancing</td>
      <td>複数のターゲットにわたる着信トラフィックの分配</td>
    </tr>
    <tr><th colspan="2">Quantum Technologies（量子テクノロジー）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Braket_16.svg" >}}Amazon Braket</td>
      <td>量子コンピューティングを探索して実験</td>
    </tr>
    <tr><th colspan="2">Robotics（ロボット工学）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-RoboMaker_16.svg" >}}AWS RoboMaker</td>
      <td>ロボット工学アプリケーションの開発、テスト、デプロイ</td>
    </tr>
    <tr><th colspan="2">Satellite（人工衛星）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Ground-Station_16.svg" >}}AWS Ground Station</td>
      <td>サービスとしてのフルマネージド型地上局</td>
    </tr>
    <tr><th colspan="2">Security, Identity & Compliance（セキュリティ、アイデンティティ、コンプライアンス）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Identity-and-Access-Management_16.svg" >}}AWS Identity & Access Management</td>
      <td>サービスとリソースへのアクセスを安全に管理</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Cognito_16.svg" >}}Amazon Cognito</td>
      <td>アプリの ID 管理</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Detective_16.svg" >}}Amazon Detective</td>
      <td>潜在的なセキュリティ問題を調査</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-GuardDuty_16.svg" >}}Amazon GuardDuty</td>
      <td>マネージド型脅威検出サービス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Inspector_16.svg" >}}Amazon Inspector</td>
      <td>アプリケーションのセキュリティの分析</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Macie_16.svg" >}}Amazon Macie</td>
      <td>大規模な機密データを検出して保護する</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Artifact_16.svg" >}}AWS Artifact</td>
      <td>AWS のコンプライアンスレポートへのオンデマンドアクセス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}AWS Audit Manager</td>
      <td>AWS 利用状況の継続的な監査</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Certificate-Manager_16.svg" >}}AWS Certificate Manager</td>
      <td>SSL/TLS 証明書のプロビジョニング、管理、およびデプロイメント</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-CloudHSM_16.svg" >}}AWS CloudHSM</td>
      <td>法令遵守のためのハードウェアベースキーストレージ</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Directory-Service_16.svg" >}}AWS Directory Service</td>
      <td>Active Directory のホスティングと管理</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Firewall-Manager_16.svg" >}}AWS Firewall Manager</td>
      <td>ファイアウォールルールの一元管理</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Key-Management-Services_16.svg" >}}AWS Key Management Service</td>
      <td>マネージド型の暗号化キー作成と管理</td>
    </tr>
    <tr>
      <td>{{< image-inline src="" >}}AWS Network Firewall</td>
      <td>VPC 保護のためのネットワークセキュリティ</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Resources-Access-Manager_16.svg" >}}AWS Resource Access Manager</td>
      <td>AWS のリソースを共有するためのシンプルでセキュアなサービス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Secrets-Manager_16.svg" >}}AWS Secrets Manager</td>
      <td>シークレットのローテーション、管理、取得</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Security-Hub_16.svg" >}}AWS Security Hub</td>
      <td>統合された AWS セキュリティ & コンプライアンスセンター</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Shield_16.svg" >}}AWS Shield</td>
      <td>DDoS 保護</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Single-Sign-On_16.svg" >}}AWS Single Sign-On</td>
      <td>クラウドシングルサインオン (SSO) サービス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-WAF_16.svg" >}}AWS WAF</td>
      <td>悪意のあるウェブトラフィックのフィルター</td>
    </tr>
    <tr><th colspan="2">Serverless（サーバーレス）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Lambda_16.svg" >}}AWS Lambda</td>
      <td>サーバーに煩わされずにコードを実行</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-API-Gateway_16.svg" >}}Amazon API Gateway</td>
      <td>API の構築、デプロイ、管理</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-DynamoDB_16.svg" >}}Amazon DynamoDB</td>
      <td>マネージド型の NoSQL データベース</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-EventBridge_16.svg" >}}Amazon EventBridge</td>
      <td>SaaS アプリと AWS のサービス向けサーバーレスイベントバス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Simple-Notification-Service_16.svg" >}}Amazon Simple Notification Service (SNS)</td>
      <td>Pub/sub、SMS、E メール、およびモバイルプッシュ通知</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Simple-Queue-Service_16.svg" >}}Amazon Simple Queue Service (SQS)</td>
      <td>マネージド型メッセージキュー</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-S3-Standard_16.svg" >}}Amazon Simple Storage Service (S3)</td>
      <td>クラウド内のスケーラブルなストレージ</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-AppSync_16.svg" >}}AWS AppSync</td>
      <td>多くのソースから適切なデータを使用して、大規模にアプリを強化</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Fargate_16.svg" >}}AWS Fargate</td>
      <td>コンテナ向けサーバーレスコンピューティング</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Step-Functions_16.svg" >}}AWS Step Functions</td>
      <td>分散型アプリケーションの調整</td>
    </tr>
    <tr><th colspan="2">Storage（ストレージ）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-S3-Standard_16.svg" >}}Amazon Simple Storage Service (S3)</td>
      <td>スケーラブルなクラウドストレージ</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Elastic-Block-Store_16.svg" >}}Amazon Elastic Block Store (EBS)</td>
      <td>EC2 ブロックストレージボリューム</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Elastic-File-System_16.svg" >}}Amazon Elastic File System (EFS)</td>
      <td>EC2 用フルマネージド型ファイルシステム</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-FSx-for-Lustre_16.svg" >}}Amazon FSx for Lustre</td>
      <td>S3 と統合されたハイパフォーマンスファイルシステム</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-FSx-For-WFS_16.svg" >}}Amazon FSx for Windows File Server</td>
      <td>フルマネージド型 Windows ネイティブのファイルシステム</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_Amazon-Glacier_16.svg" >}}Amazon S3 Glacier</td>
      <td>クラウド上の低コストなアーカイブストレージ</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Backup_16.svg" >}}AWS Backup</td>
      <td>AWS のサービス全体にわたる一元管理型バックアップ</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Snowball_16.svg" >}}AWS Snow ファミリー</td>
      <td>厳しい環境や切断された環境向けの物理エッジコンピューティングおよびストレージデバイス</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Storage-Gateway_16.svg" >}}AWS Storage Gateway</td>
      <td>ハイブリッドストレージの統合</td>
    </tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-CloudEndure-Disaster-Recovery_16.svg" >}}CloudEndure Disaster Recovery</td>
      <td>高度に自動化した災害対策</td>
    </tr>
    <tr><th colspan="2">VR & AR（VR および AR）</th></tr>
    <tr>
      <td>{{< image-inline src="Arch_AWS-Sumerian_16.svg" >}}Amazon Sumerian</td>
      <td>VR および AR アプリケーションの構築と実行</td>
    </tr>
  </tbody>
</table>

