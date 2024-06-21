---
title: "AWS ソリューションアーキテクトまとめ（復習用資料）"
url: "p/tqmr4od/"
date: "2024-06-21"
tags: ["AWS"]
draft: true
---

### {{< image-inline w="38" src="Arch_Amazon-EC2_16.svg" >}} Amazon EC2 (Elastic Compute Cloud)

- 仮想サーバーを提供するコンピューティングサービス。
- EC2 を起動するときは __AMI (Amazon Machine Image)__ を選ぶ。
- 既存の EC2 インスタンスを AMI としてバックアップすることができる。
- EC2 では「__インスタンスタイプ__」でスペックを指定する。
  - 先頭文字の `m` や `p` は「__インスタンスファミリー__」を表し、どのような用途に最適化されているかを示す。
  - インスタンスファミリーの後ろの数値は「__世代__」を表す。

### {{< image-inline w="38" src="Arch_Amazon-Elastic-Container-Service_16.svg" >}} Amazon ECS (Elastic Container Service)

- Docker コンテナの実行環境。

### {{< image-inline w="38" src="Arch_Amazon-Elastic-Container-Registry_16.svg" >}} Amazon ECR (Elastic Container Registry)


### {{< image-inline w="38" src="Arch_Amazon-Virtual-Private-Cloud_16.svg" >}} Amazon VPC (Virtual Private Cloud)

- データセンター内の仮想的な閉域ネットワーク。

### {{< image-inline w="38" src="Arch_AWS-Lambda_16.svg" >}} AWS Lambda

- サーバーレスのプログラム実行環境。

### {{< image-inline w="38" src="Arch_Amazon-Elastic-Block-Store_16.svg" >}} Amazon EBS (Elastic Block Store)

### {{< image-inline w="38" src="Arch_Elastic-Load-Balancing_16.svg" >}} Amazon ELB (Elastic Load Balancing)


セキュリティ／コンプライアンス分野
----

### {{< image-inline w="38" src="Arch_AWS-Shield_16.svg" >}} AWS Shield

- Web アプリを DDos 攻撃から保護する。
- スタンダードとアドバンスドがある。
  - AWS Shield Standard
    - __デフォルトで有効、無料__。
    - __L3、L4 層（インフラストラクチャーレイヤー）__ での DDoS 攻撃からの保護。
  - AWS Shield Advanced
    - L3、L4 層に加え、__L7 層（アプリケーションレイヤー）__ での DDoS 攻撃からの保護。
    - DDoS 攻撃の履歴確認、レポート生成が可能。
    - 24 時間 365 日のサポート体制。

### {{< image-inline w="38" src="Arch_AWS-WAF_16.svg" >}} AWS WAF

- WAF (Web Application Firewall) は Web アプリに特化したファイアウォールのことで、SQL インジェクションや XSS などの脅威からアプリを保護するもの。
- AWS WAF は、__API Gateway__、__CloudFront__、__ALB__ へのアクセスを保護する。
- AWS WAF の __Web ACL__ 内に __ルール__ を作成することで通信リクエストを制限する。
  - Web ACL ◇─ ルール1、ルール2、...
- Web ACL 1 つにつき __月5ドル__、ルール 1 つにつき __月1ドル__、100万リクエストにつき __0.6ドル__ かかる（2024年6月時点）。

