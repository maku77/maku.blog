---
title: "AWS ソリューションアーキテクト復習シート"
url: "p/3vedvwg/"
date: "2024-06-21"
tags: ["AWS"]
draft: true
---

<h3>{{< image-inline w="38" src="img-az.drawio.svg" >}} AZ (Availability Zone)</h3>

- リージョン内には AZ: Availability Zone が複数ある(東京リージョンには 4 つの AZ がある）。
- AZ は __100km 圏内__ で繋がるように配置されている。
- AZ 間の通信は __1～2ms__ 程度（参考値）。
- AZ はユーザーが特に意識することなく裏で勝手に決まる（ただし、VPC を作るときは意識する必要がある）。

<h3>{{< image-inline w="38" src="Arch_Amazon-ElastiCache_16.svg" >}} Amazon ElastiCache</h3>

- インメモリーキャッシュ。Redis や Memcached をサービス化したもの。
- 小さなマスターデータや、セッション管理用のデータなどに適している。
- Redis は 1 台のプライマリーと、複数のリードレプリカでクラスターを構成する。
- ElastiCache のパッチ適用中はダウンタイムが発生する可能性がある。

<h3>{{< image-inline w="38" src="Arch_Amazon-EC2_16.svg" >}} Amazon EC2 (Elastic Compute Cloud)</h3>

- 仮想サーバーを提供するコンピューティングサービス。
- EC2 を起動するときは __AMI (Amazon Machine Image)__ を選ぶ。
- 既存の EC2 インスタンスを AMI としてバックアップすることができる。
- EC2 では「__インスタンスタイプ__」でスペックを指定する。
  - 先頭文字の `m` や `p` は「__インスタンスファミリー__」を表し、どのような用途に最適化されているかを示す。
  - インスタンスファミリーの後ろの数値は「__世代__」を表す。
  - インスタンスファミリーの分類:
    - 汎用 ... M5 や T3 など。CPU とメモリのバランス型。
    - コンピューティング最適化 ... C5 など。CPU 性能重視。
    - メモリ最適化 ... R5 や X1 など。メモリ容量重視。
    - 高速コンピューティング ... P系（画像処理）、G系（機械学習）、GPU などのリソース重視。
    - ストレージ最適化 ... D2 (HDD)、I3 (SSD) など。
- EC2 インスタンスの 3 つの状態:
  - Running（起動中） ... 課金対象。
  - Stopped（停止中） ... EBS の費用はかかる。Running 状態に戻せる。
  - Terminated（削除済み） ... Running 状態に戻せない。
- EC2 の利用形態:
  - __オンデマンド・インスタンス__ ... 通常。
  - __スポット・インスタンス__
    - AWS が余らせている EC2 リソースを入札形式で安く利用する。
    - 余剰なリソースがなくなるとインスタンスが自動的に中断される。
  - __リザーブド・インスタンス (RI)__
    - 長期間の利用契約による割引（1年、3年など）。
    - スケジュールされたリザーブド・インスタンス ... 毎日・毎週・毎月の一定時間のみ使う。
- EC2 の「__Savings Plans__」
  - インスタンスの __利用額__ にコミットして割引。Lambda や Fargate なども対象。
  - __EC2 Instance Savings Plans__ ... リザーブド・インスタンスと同様に、リージョンやインスタンスファミリーを指定する。
  - __Compute Savings Plans__ ... すべての EC2 インスタンスを対象に単位時間あたりの利用料を指定する。
  - スケジュールされたリ
- {{< image-inline w="26" src="Res_Amazon-EC2_Elastic-IP-Address_48.svg" >}} __EC2 Elastic IP (EIP) アドレス__ ... AWS の固定のグローバル IP アドレス。

<h3>{{< image-inline w="38" src="Arch_Amazon-Elastic-Container-Service_16.svg" >}} Amazon ECS (Elastic Container Service)</h3>

- Docker コンテナの実行環境。
- ECS 用語:
  - __Cluster__ ... EC2 インスタンスのこと。1 つの Cluster 上で複数の Task を実行可能。
  - __Task__ ... EC2 インスタンス上で実行される __コンテナ__。
  - __Task Definition__ ... Task の定義。
  - __Service__ ... __同じ Task を複数用意して__ ELB に紐付けるときなど。Task のブルー・グリーン・デプロイメントにも使える。
- Task ごとに IAM ロールを割り当て可能。
- EC2 (Cluster) の管理（Auto Scaling 設定など）は、利用者が意識する必要あり。
- Task 定義を作成するときに EC2 の代わりに Fargate を選択すれば、EC2 (Cluster) の管理は必要なくなる。

<h3>{{< image-inline w="38" src="Arch_Amazon-Elastic-Container-Registry_16.svg" >}} Amazon ECR (Elastic Container Registry)</h3>

- コンテナイメージを管理するためのレジストリ。
- レジストリへの push/pull 権限は IAM で管理できる。


<h3>{{< image-inline w="38" src="Arch_Amazon-Virtual-Private-Cloud_16.svg" >}} Amazon VPC (Virtual Private Cloud)</h3>

{{< image w="400" src="img-vpc.drawio.svg" >}}

{{< image w="400" src="https://docs.aws.amazon.com/ja_jp/vpc/latest/userguide/images/security-group-overview.png" >}}

- VPC はクラウド内の仮想的な閉域ネットワーク。
- __リージョンの下に VPC__ を作成できる。
- __VPC の下に Public/Private サブネット__ を作成できる（VPC CIDR ブロックのサブネット）。__各サブネットは AZ (Availability Zone) 内__ に配置される。
  - __EC2 インスタンスに対して、セキュリティグループ__ を設定してインターネット側からの通信を遮断できる。許可する通信をホワイトリスト形式で設定する。
  - __サブネットに対しては、ネットワーク ACL（アクセスコントロールリスト）__ を設定することでファイアウォールとして機能させられる。主に DDoS 攻撃を受けたときの拒否リスト。
  - 通常はセキュリティグループの方だけ使えば OK。
- VPC のアドレス空間内でプライベート IP アドレスを EC2 などに割り当てられる。
- VPC とインターネットの境界には __インターネットゲートウェイ__ を配置する。
- Public サブネットは、インターネット通信できるようになったサブネットのこと。インターネットゲートウェイに加え、「__ルートテーブル__」と「__パブリック IP アドレス__」が必要。
- Private サブネット内の EC2 からインターネットにアクセスするには、Public サブネットに配置した __NAT ゲートウェイ__ 経由になる。
- NAT ゲートウェイは、外向けの IP アドレスを固定したいときにも使用する。

<h3>{{< image-inline w="38" src="Arch_AWS-Lambda_16.svg" >}} AWS Lambda</h3>

- サーバーレスのプログラム実行環境。
- CloudWatch Events で Lambda を定期実行できる。
- Lambda の設定項目:
  1. メモリ容量（CPU パワーはメモリ量に連動して決まる）
  2. IAM ロール（他の AWS サービスへのアクセス権を付加）
  3. タイムアウト時間
  4. VPC 内／外どちらに配置するか
- Lambda の課金体系 ... リクエスト数＆処理時間

<h3>{{< image-inline w="38" src="Arch_Elastic-Load-Balancing_16.svg" >}} Amazon ELB (Elastic Load Balancing)</h3>

- ELB の 3 タイプのロードバランサー:
  1. __CLB: Classic Load Balancer__
     - L4/L7 レイヤーでの負荷分散。
  2. __ALB: Application Load Balancer__
     - L7 レイヤーでの負荷分散。つまり Web レイヤー。
     - CLB より後発で、安価かつ機能が豊富。
     - URL パターンによる「__パスベースルーティング__」機能あり。
  3. __NLB: Network Load Balancer__
     - L4 レイヤーでの負荷分散。
     - HTTP(S) 以外の TCP プロトコルで負荷分散させたいとき。
     - __固定の IP アドレス__ を持つ。
- ELB の __Auto Scaling__ 機能:
  - ELB に紐付く EC2 インスタンスの台数を増減させる（スケールアウト／イン）。
  - 設定項目:
    1. 最小・最大インスタンス数。
    2. インスタンス増減の条件と増減数。
    3. どのようなインスタンスから減らしていくか。
  - 「最小＝最大＝2」のように設定することで耐障害性を上げることができる。
  - スケーリング方法 3 つ:
    1. 動的なスケーリング:
       - 簡易スケーリング（非推奨） ... CPU 使用率など 1 つの閾値だけをトリガーにする。
       - ステップスケーリング ... 複数の閾値で段階的にスケーリング。
       - ターゲット追跡スケーリング ... メトリクスの目標値を指定し（CPU 利用率50％以下など）、そうなるようにスケーリングしてくれる。
    2. 予測スケーリング
    3. スケジュールスケーリング
  - __スケールアウトの猶予期間__ ... EC2 インスタンス起動中にヘルスチェックするのは意味がないので、起動直後しばらくはヘルスチェックしないようになっている（デフォルト: 5分）。
  - Auto Scaling の __ライフサイクルフック__ ... Auto Scaling グループによるインスタンスの起動・削除時に、インスタンスを一時停止（最大48時間）し、カスタムアクションを実行する。
  - Auto Scaling の __終了ポリシー__
    - スケールイン時にどのインスタンスを削除するかの設定。
    - デフォルトでは AZ に均等に配分されるように動作する（台数の多いところから減らす）。
    - 最も古いインスタンスを削除する OldestInstance などの終了ポリシーがある。
  - ELB + Auto Scaling 導入時のポイント:
    - EC2 サーバーはステートレスになるよう設計する。
    - 複数の AZ にまたがるように EC2 サーバーを配置する。
- Web サーバー (EC2) のセキュリティグループでは、ELB が属するサブネットに送信元を限定する（10.0.0.0/24 など）。サブネットで指定するのは、__ELB の IP アドレスが固定ではない__ から。
- ELB の「__スティッキーセッション__」機能
  - ELB が作成する Cookie をもとにクライアントからのアクセスを同一サーバーに振り分ける。
- ELB の「__SSL ターミネーション__」機能
  - 個々の Web サーバーで SSL 証明書を管理しなくてもよくなる。
- ELB の「__ヘルスチェック__」機能
  - デフォルトでは、間隔: 30 秒、応答タイムアウト: 5 秒、非正常の閾値: 2 回。
- ELB の「__アイドルタイムアウト__」
  - Web サーバーがこの時間応答しないと（デフォルトは 60 秒）、ELB は Web サーバーを切り離して HTTP 504 レスポンスを返す。

<h3>{{< image-inline w="38" src="Arch_AWS-Auto-Scaling_16.svg" >}} AWS Auto Scaling</h3>

- サーバー負荷に応じて自動でスケールアウトするサービス。
- CloudWatch で EC2 のインスタンスを監視することで Auto Scaling にアラーム通知する。
- {{< image-inline w="26" src="Arch_Amazon-CloudWatch_16.svg" >}} <b>CloudWatch</b><br>
  &nbsp;↓ アラーム通知<br>
  {{< image-inline w="26" src="Arch_AWS-Auto-Scaling_16.svg" >}} <b>Auto Scaling</b><br>
  &nbsp;↓ インスタンス起動<br>
  {{< image-inline w="26" src="Auto-Scaling-group_32.svg" >}} <b>Auto Scaling グループ</b>（EC2 群）

<h3>{{< image-inline w="38" src="Arch_Amazon-RDS_16.svg" >}} Amazon RDS</h3>

- {{< image-inline w="26" src="Res_Amazon-RDS_Multi-AZ_48.svg" >}} __Multi-AZ 機能__:
  - DB サブネットグループを作成 ... 2 つの AZ (Availability Zone) のサブネットをグループ化したもの。
  - マスターとスレーブの 2 台分のコストがかかる。
  - RDS の SLA (Service Level Agreement) は、Multi-AZ の利用が前提条件（EC2 も同様）。
- 自動バックアップ
  - データの保持期間 ... デフォルト 1 日、最大 35 日間。
- スナップショット
  - システムバックアップとして恒久的に保管したいとき。
- {{< image-inline w="26" src="Arch_Amazon-Aurora_16.svg" >}} RDS for __Aurora__
  - MySQL をベースに高速化の仕組みを取り入れたもの。
  - __Log-Structured Storage__ と呼ばれる追記型のストレージシステムを採用しており、MySQL のような更新時のロックが発生しない。
  - プライマリーとリードレプリカで同じストレージを使用するため、MySQL のようなトランザクションログ（バイナリーログ）の生成や転送が不要。つまり、リードレプリカを増やすとき（レプリケーション）の負荷が少ない。


ストレージサービス
----

- 3 タイプのストレージサービス
  1. ブロックストレージ (Amazon EBS)
     - DB や仮想サーバーのイメージ保存領域。
  2. ファイルストレージ (Amazon EFS)
     - ブロックストレージ上にファイルシステムを構成する。
     - 複数クライアントからネットワーク経由 (NFS) でファイルにアクセス可能。
  3. オブジェクトストレージ (Amazon S3、S3 Glacier)
     - ファイルにメタ情報を付けてオブジェクトとして管理。
     - ファイルの内容を編集することはできない。

<h3>{{< image-inline w="38" src="Arch_Amazon-Elastic-Block-Store_16.svg" >}} Amazon EBS (Elastic Block Store)</h3>

- EC2 の OS 領域や追加ボリュームとしてアタッチできるディスク機能。
- RDS のデータ保存領域としても使われる。
- EBS は作成時に AZ を指定する。__同じ AZ 内の EC2 からのみアタッチ可能__。
- __EBS マルチアタッチ機能__:
  - 複数の EC2 インスタンスからアタッチ。同じ AZ 内からのみという制約は変わらない。
  - プロビジョンド IOPS タイプのボリュームのみ使える。
- 別の AZ に持って行きたいときは、EBS のスナップショットを取得して、別の AZ に EBS ボリュームを作成する。
- EBS のボリュームタイプ 4 つ:
  1. 汎用 SSD (gp3, gp2)
     - EC2 インスタンス作成時のデフォルトボリューム。
     - ベースライン性能: 3 IOPS/GB、16,000 IOPS/ボリューム（EBS 利用時間の 99% を満たす）。
     - バースト機能: 1TB 未満のボリュームで一時的に 3,000 IOPS。
  2. プロビジョンド IOPS SSD (io2, io1)
     - 最も高性能な SSD ベースのボリューム。__用途は DB のデータ領域など__。
     - 性能指標は IOPS。
     - ベースライン性能: 最大 50 IOPS/GB、64,000 IOPS/ボリューム（EBS 利用時間の 99.9% を満たす）。
     - スループットはボリュームあたり最大 1,000 MB/秒。
  3. スループット最適化 HDD (st1)
     - 大容量ファイルを高速に読み取るユースケース（ログやバッチ処理）。
     - 性能指標はスループット（MB/秒）。
     - ベースライン性能: 1TB あたり 40MB/秒。ボリュームあたり最大 500 MB/秒。EBS 利用時間の 99% を満たす。
  4. Cold HDD (sc1)
     - 最も低コスト。アーカイブ用途。
     - ベースライン性能: 40 MB/秒、ボリュームあたり最大 250 MB/秒。
- EC2 はクライアントとの通信と EBS との通信で __ネットワーク帯域を共有する__。
  - 「__EBS 最適化インスタンス__」のオプションを有効化すると EBS 用の帯域を別途確保できる。
- __プロビジョンド IOPS__:
  - I/O 帯域を確保して性能を上げる。IOPS は 1 秒あたりのディスク I/O 回数で、通常は約 100 IOPS。
  - プロビジョンド IOPS 以外のストレージタイプには、ストレージ容量に応じたベースライン性能とバースト性能がある。
  - プロビジョンド IOPS タイプの IOPS 値は増減できる。__IOPS 値の変更は 24 時間以上かかることがある__。
- バースト性能に頼ったサイジングはすべきでない。
- 同一 EBS ボリュームに対する変更は __6 時間以上あける__。
- EBS のボリュームあたりの __最大容量は 16 TB__。
- ボリュームサイズの拡張はできるが、__縮小はできない__。ボリュームサイズを拡張した場合は、OS レベルでの拡張作業も必要。
- ボリュームの __タイプの変更は可能__。
- EBS の SLA は月当たりの利用可能時間が __99.99%__。
- 内部的に AZ 内の複数の物理ディスクに複製している。
- 定期的に __EBS のスナップショット__ を取っておくと安心。
- EBS の暗号化を有効化すると、ボリューム（ストレージ）、スナップショット、EBS 間のデータ通信などが暗号化される。

<h3>{{< image-inline w="38" src="Arch_Amazon-EFS_16.svg" >}} Amazon EFS (Elastic File System)</h3>

- __容量無制限__ のファイルストレージ。
- __複数の EC2 インスタンスから__ 同時アクセス可能。
- 一般的な NFS クライアントからアクセス可能。
- EFS へ接続するクライアントには、`amazon-efs-tools` を導入するとよい。
- EFS にファイルが作成されると、__3 つの AZ__ に分散保存される。
- EFS の 3 つの構成要素:
  1. ファイルシステム（AZ ごと）
  2. マウントターゲット（AZ ごと）
  3. セキュリティグループ
- AZ ごとにマウントターゲット用の IP アドレスは異なるが、各 AZ 内の EC2 からは同一の FQDN（ターゲットポイント）でアクセスできる。
- マウントターゲットにはセキュリティグループを設定でき、不要なアクセスを排除できる。
- システムに最適なモード（__パフォーマンスモード__ と __スループットモード__）を選択する必要あり。
- EFS のパフォーマンスモード（2 種類）
  1. __汎用パフォーマンスモード__ ... ほとんどはコレで間に合う。
  2. __最大 I/O パフォーマンスモード__:
     - 数百台以上のクライアントからアクセスするとき。
     - __ファイル操作時のレイテンシーは少し高くなってしまう__。
  - パフォーマンスモードの選定には、CloudWatch の `PercentIOLimit` というメトリクスが参考になる。
  - パフォーマンスモードはファイルシステム作成後に __変更できない__。
- EFS のスループットモード（2 種類）
  1. __バースト・スループットモード__
     - EFS 内のデータ容量に応じたスループットのベースライン。
     - 1GB あたり 50KB/秒。
     - 最低バーストスループット: 100MB/秒。
  2. __プロビジョニング・スループットモード__
     - 任意のスループット。
     - 容量によらず最大 1GB/秒。
     - Web 配信用コンテンツなどに。
     - スループットは __増減どちらも可能__（24 時間あける）。
  - スループットモードの選定には、CloudWatch の `BurstCreditBalance` というメトリクスが参考になる。
  - スループットモードは EFS 運用中でも __変更可能__。

<h3>{{< image-inline w="38" src="Arch_Amazon-Simple-Storage-Service_16.svg" >}} Amazon S3 (Simple Storage Service)</h3>

- 容量無制限のオブジェクトストレージ。
- ファイルストレージとは異なり、ディレクトリ構造を持たない。
- HTTP ベースの API (REST API) でアクセス可能。
- S3 の構成要素:
  - __バケット__ ... バケット名は AWS 内で一意。格納できる __オブジェクト数は無制限__。
  - __オブジェクト__ ... キー名（オブジェクト名）をもとに「バケット名＋キー名＋バージョン」で一意な URL を割り当て。オブジェクトの __最大サイズは 5TB__。
  - __メタデータ__ ... システム定義メタデータ（日時、サイズ）と、ユーザー定義メタデータ。
- S3 は __結果生合成方式__。
- __複数の AZ、複数の物理ストレージ__ に保存され、__耐久性はイレブンナイン__。
- __ストレージクラス__:
  1. __S3 標準__
     - 可用性: 99.99 %
  2. __S3 標準 ─ 低頻度アクセス__
     - 可用性: 99.9 %
     - 読み出し容量に対する従量課金。
  3. __S3 1 ゾーン - IA__
     - 単一の AZ であること以外「S3 標準 ─ 低頻度アクセス」と同様。
     - 可用性: 99.5 %
  4. __S3 Intelligent-Tiering__
     - 「S1 標準」と「S3 標準 ─ 低頻度アクセス」のハイブリッド。
     - __30 日以上__ 参照されないと「S3 標準 ─ 低頻度アクセス」に移動される。
     - 可用性: 99.9 %
  5. __S3 Glacier__ （アーカイブ用） {{< image-inline w="26" src="Arch_Amazon-Simple-Storage-Service-Glacier_16.svg" >}}
     - __オブジェクト作成時には選択できない__ ので、ライフサイクル管理機能で指定する。
     - Glacier のデータを参照する場合、アクセスリクエストから __数時間__ かかる。
     - データの取り出しに標準で __3〜5 時間__ かかる。
     - 可用性: 99.99 %
  6. __S3 Glacier Deep Archive__
     - 「S3 Glacier」よりもアクセス頻度の低いもので、「S3 Glacier」より最大 75% 安価。
     - データの取り出しは標準で __12 時間以内__。バルク取り出しは __12〜48 時間__。
- S3 の __ライフサイクル管理__（2 種類のライフサイクル設定）
  1. __移行アクション__ ... __利用頻度__ に応じて自動でストレージクラスを変更。
  2. __有効期限アクション__ ... __有効期限__ を越えたら自動でオブジェクトを削除。
- S3 のバージョニング機能:
  - オブジェクトの複数バージョンを管理。
  - バージョニングは __バケット単位__ で有効化する。
  - 差分管理ではないので、バージョン分の容量を消費する。
- S3 の Web サイトホスティング機能:
  - 静的なコンテンツのみ。
  - 自動的に FQDN（ドメイン）が作成されるが、独自ドメインを使いたいときは、Route 53 などの DNS で CNAME レコードを設定する。このとき、__バケット名をドメイン名と同じ__ にする必要がある。
- S3 のアクセス管理
  - __バケットポリシー__ ... バケット単位でアクセス制御。バケット内の __特定のパス__ にのみルール適用できる。
  - __ACL__ ... オブジェクト単位で公開・非公開。
  - __IAM__ ... ユーザー単位でのアクセス制御（バケットポリシーでもユーザー名称での制御は可能だが、IAM を使った方がよい）。
- S3 の __署名付き URL__:
  - オブジェクトを参照するための期限付きの URL を発行する。
  - URL を知っていれば誰でもアクセスできてしまう。
- S3 のデータ暗号化:
  - サーバー側での暗号化、クライアント側での暗号化がある。
- S3 のブロックパブリックアクセス機能:
  - 新規のバケットは、__デフォルトでパブリックアクセスがブロック__ される。
  - ACL やバケットポリシーでパブリックアクセスを禁止できる。
  - S3 Access Analyzer でもバケットの監視や保護ができる。
- S3 Select
  - SQL で S3 オブジェクトの内容をフィルタリングすることによりデータ転送量を削減。
- S3 Transfer Acceleration
  - CloudFront のエッジロケーションを利用して、海外のリージョンへ効率的にデータ転送。

<h3>{{< image-inline w="38" src="Arch_Amazon-Simple-Storage-Service-Glacier_16.svg" >}} Amazon S3 Glacier</h3>

- アーカイブストレージサービス。
- S3 オブジェクトのように名前を付けることはできず、__自動で付けられたアーカイブ ID__ で管理する。
- S3 Glacier への保存は、__ライフサイクル管理機能__ か __API による操作__ でしか行えない。
- S3 Glacier の 4 つの構成要素:
  1. __ボールト__ ... 各アーカイブの保存領域（S3 バケットに相当）。ボールト名はリージョンおよびアカウント内で一意であればよい。
  2. __アーカイブ__ ... データ自身（S3 オブジェクトに相当）。アーカイブ ID として 138 バイトの文字列が付与される。
  3. __インベントリ__ ... 各ボールとのアーカイブ情報（1 日 1 回程度の更新）。最新情報は `ListVaults` API かコンソールで確認。
  4. __ジョブ__ ... 検索やデータダウンロードの処理を実行する。
- S3 Glacier のデータ取り出しオプション: __高速__／__標準__／__バルク__（後ろほど安くて遅い）
- S3 Glacier Select
  - アーカイブデータを SQL で検索して、一致したものだけを S3 バケットに取り出す。
  - アーカイブデータは __非圧縮の CSV 形式__ でなければいけない。
- S3 Glacier のデータ暗号化
  - __デフォルトで暗号化__ されて保存される。
  - データ転送には __SSL__ が使用される。
- S3 Glacier の __ボールトロック（削除禁止機能）__
  - ボールトロックポリシーで 5 年間アーカイブの削除を禁止する、など。
  - 手順:
    1. S3 Glacier API でボールトに対してボールトロックポリシーを関連づける。
    2. ボールトロックが InProgress になり、ロック ID が返される。
    3. ロック ID を使ってボールトロックを開始する。

<h3>{{< image-inline w="38" src="Arch_AWS-Storage-Gateway_16.svg" >}} AWS Storage Gateway</h3>

- オンプレミスにあるデータをクラウドと連携する。
- __ストレージとしては S3 や S3 Glacier__、__キャッシュには EBS__ を使う。
- 参照頻度の高いデータはオンプレミスに置くという使い分けも可能。
- Storage Gateway の 3 種類のタイプ
  1. __ファイルゲートウェイ__
     - S3 を NFS マウントして使う。
     - 保存したファイルは S3 API で参照できる利点があるが、単に NFS サーバーが欲しいのであれば EFS を使った方が高速。
  2. __ボリュームゲートウェイ__
     - S3 のデータ保存領域全体を 1 つのボリュームとして管理する。
     - S3 API でファイルを参照することは __できない__。
     - 接続方式は NFS ではなく __iSCSI 接続__ になる。
     - ボリュームのスナップショットから EBS を作成できる。
     - __ボリューム型のボリュームゲートウェイ__
       - 参照頻度の高いデータはオンプレミスのキャッシュボリュームに格納する。
     - __保管型のボリュームゲートウェイ__
       - オンプレミスのプライマリストレージにすべてのデータを保存し、定期的にスナップショットを S3 へ転送する。
       - オンプレミスのデータのバックアップ用途に使える。
  3. __テープゲートウェイ__
     - テープデバイスの代替として S3 や S3 Glacier にバックアップ。
- Storage Gateway のセキュリティ
  1. __CHAP 認証__ ... iSCSI 接続時。クライアントのなりすましや盗聴を防ぐ。
  2. __データ暗号化__ ... S3 への保存時に AWS KMS で暗号化。
  3. __通信の暗号化__ ... S3 に送信するときは HTTPS で送られる。

<h3>{{< image-inline w="38" src="Arch_Amazon-FSx_16.svg" >}} Amazon FSx</h3>

- フルマネージドなファイルストレージ。
- 2 種類の FSx ストレージ
  1. FSx for Windows ファイルサーバー
     - 単一のサブネットにエンドポイントとなる ENI (Elastic Network Interface) を配置し、SMB プロトコルでアクセス。
  2. FSx for Lustre
     - Linux 用のサービスで、専用のクライアントをインストールする。
     - その後は通常の NAS のようにマウントできる。
     - ファイルシステムを S3 のバケットと関連付ける。


運用支援サービス
----

<h3>{{< image-inline w="38" src="Arch_Amazon-CloudWatch_16.svg" >}} Amazon CloudWatch</h3>

- 定期的に AWS リソース状態（メトリクス）を取得して問題があるときにアラーム通知するサービス。
- メトリクスがアラーム条件を満たしたときに SNS に通知することでメール送信などを行う。
- メトリクスの種類:
  - __標準メトリクス__ ... AWS が定義しているメトリクス。
  - __カスタムメトリクス__ ... 独自定義するメトリクス。
- アラームの定義例:
  - EC2 インスタンスの CPU 使用率が 90% を上回った。
  - Lambda 関数が一定期間内に 3 回エラーを出した。
- {{< image-inline w="26" src="Res_Amazon-CloudWatch_Logs_48.svg" >}} __CloudWatch Logs__
  - Apache などのログを監視するため、EC2 インスタンスに __独自のエージェント__ をインストールして CloudWatch Logs にログを収集する。
  - EC2 インスタンスには CloudWatch アクセス用の IAM 権限が必要。
  - 収集したログをもとにしたアラーム設定が可能（SNS 通知）。

<h3>{{< image-inline w="38" src="Arch_AWS-CloudTrail_16.svg" >}} Amazon CloudTrail</h3>

- AWS リソースの作成やマネジメントコンソールへのログイン／API実行などの __操作ログを記録__ する（つまり、誰が、いつ、何をしたかという監査ログ）。
- __デフォルトで 90 日分のログ__ をマネジメントコンソールから確認できるが、__それ以上残したいときは S3 にログ保管するよう設定__ する。
- CloudTrail で取得可能なもの:
  - __管理イベント__ ... コンソールへのログイン、S3 バケットの作成など。__デフォルトではこの管理イベントのみ記録される__。
  - __データイベント__ ... Lambda の実行、S3 バケット上のデータ操作など。
- CloudTrail と __CloudWatch Logs を連携__ させることで、ユーザーの不正な操作を早期に発見できる。

<h3>{{< image-inline w="38" src="Arch_Amazon-EventBridge_16.svg" >}} Amazon EventBridge</h3>

- アプリの間の統合を可能にするサーバーレスイベントバス。
- CodeBuild、CodeDeploy、CodePipeline、CodeCommit などのサービスからのイベントを通知可能。
- 独自トリガー（__イベントソース__）と、後続アクション（__ターゲット__）を定義できる。
- イベントソース:
  - __スケジュール__ ... 時間ベースのトリガー。
  - __イベント__ ... AWS リソースの状態変化のトリガー（例: AutoScaling によるインスタンスの増減）
- ターゲットの例 ... Lambda 関数を呼び出す。


セキュリティ／コンプライアンス分野
----

<h3>{{< image-inline w="38" src="Arch_AWS-Shield_16.svg" >}} AWS Shield</h3>

- Web アプリを DDos 攻撃から保護する。
- スタンダードとアドバンスドがある。
  - AWS Shield Standard
    - __デフォルトで有効、無料__。
    - __L3、L4 層（インフラストラクチャーレイヤー）__ での DDoS 攻撃からの保護。
  - AWS Shield Advanced
    - L3、L4 層に加え、__L7 層（アプリケーションレイヤー）__ での DDoS 攻撃からの保護。
    - DDoS 攻撃の履歴確認、レポート生成が可能。
    - 24 時間 365 日のサポート体制。

<h3>{{< image-inline w="38" src="Arch_AWS-WAF_16.svg" >}} AWS WAF</h3>

- WAF (Web Application Firewall) は Web アプリに特化したファイアウォールのことで、SQL インジェクションや XSS などの脅威からアプリを保護するもの。
- AWS WAF は、__CloudFront__、__ALB: Load Balancer (ELB)__、__API Gateway__ へのアクセスを保護する。
  - 例: Internet → ELB __(+WAF)__ → EC2群
  - 例: Internet → CloudFront __(+WAF)__ → ELB → EC2群（この構成の方が上記より高速）
  - 例: Internet → CloudFront __(+WAF)__ → オンプレミス
- __Web ACL (Access Control List)__
  - AWS WAF の実体は Web ACL で、この中に __ルール__ を作成することで通信リクエストを制限する。
    - Web ACL ◇─ ルール1、ルール2、...
  - ルール ＝ ステートメント（1つ以上） ＋ アクション (Deny/Allow/Count)
  - 届いたリクエストについてルールが評価され、すべてにマッチしなかった場合は __デフォルトアクション__ で処理される。デフォルトアクションが Deny であれば、__HTTP 403 Forbidden__ が返される。
  - __キャパシティ (WCU: WAF Capcacity Unit)__ ... Web ACL にどれだけルールを含められるか。デフォルトの WCU は __1500__（ソフトリミット）。
- __マネージドルールグループ__ ... AWS やセキュリティベンダーが事前に定義したルールのグループ。有料のものもあり、AWS マーケットプレイスでサブスクリプション購入する。
- 料金（2024年6月時点）
  - Web ACL 1 つ ... __月5ドル__
  - ルール 1 つ ... __月1ドル__
  - 100 万リクエスト ... __0.6ドル__
- 連携例:
  - WAF のログを S3 バケットに出力
  - S3 バケット上の WAF ログを Amazon Athena で分析
  - WAF メトリクスを CloudWatch で集約して 3rd パーティサービスへ通知

<h3>AWS セキュリティグループ</h3>

- __インスタンスに対して__ ファイアウォールを導入する仕組み。OS レベルのネットワークのフィルタリングルールを設定する。
- 1 つのセキュリティグループを __複数のインスタンスに適用可能__。
- デフォルトで __すべてのインバウンド通信を遮断する__。
  - ただし、SSH アクセスはすべての接続元からのアクセスが許可されている。
- 通信のインバウンドルール（__許可ルール__）を設定する（拒否ルールというものは存在しない）。
- セキュリティグループは __ステートフル__ であり、応答（アウトバウンド通信）は常に許可される。
- セキュリティグループの「送受信先」としてセキュリティグループを指定することができる。
- （比較）ネットワーク ACL
  - ネットワーク ACL は __サブネット__ に設定する（個々の EC2 インスタンスに設定する必要がない）。
  - ネットワーク ACL はステートレスで、デフォルトでインバウンドもアウトバウンドも許可。
    - 応答トラフィック（アウトバウンド）も明示的に設定する必要あり（エフェメラルポート 1025～65535 を許可しておく）。
  - ネットワーク ACL の「送受信先」としてはセキュリティグループは指定できない。

