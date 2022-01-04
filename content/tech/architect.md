---
title: "ソフトウェアアーキテクトが考えること"
url: "/p/x2b2gw3"
date: "2015-07-21"
tags: ["技術"]
---

ソフトウェアアーキテクチャとは
----

アーキテクチャとは「**要求と設計の橋渡し**」である ─『オブジェクト開発の神髄』より

- 外部から見える特性
- 構成要素
- 構成要素間の関係


### アーキテクトが考えること

- 何をもとにシステムを設計するか
- 部品をどのように組み立てるか
- システムをどのように動作保証するか
- パフォーマンス


アーキテクチャドライバとは
----

アーキテクチャを決定する要件のことを「**アーキテクチャドライバ**」といいます。
つまり、アーキテクチャドライバはアーキテクチャ設計のためのインプットとなります。
アーキテクチャドライバには以下の３つの要素があります。

- 制約（最重要。変更できないものだから）
    - ビジネス制約
    - 技術制約
- 品質特性
    - Performance 性能
    - Modifiability 変更容易性
    - Usability ユーザビリティ
    - Availability 可用性
    - Security セキュリティ
    - Testability テスト容易性
- 主な機能要件（重要性としては一番低い。ほとんどの場合、制約にかかわらず実現できるので）

アーキテクチャドライバはステークホルダーとのコミュニケーションの道具であり、このアーキテクチャドライバの作成（文章作成）を行うのがアーキテクトの最初の仕事です。


アーキテクチャ設計のステップ
----

1. アーキテクチャ要件抽出
    1. アーキテクチャドライバの作成（要求を要件として分類する。上の３つの要素）
1. ステークホルダーと仕様優先度レビュー
1. アーキテクチャドライバに基づくシステムの分割
1. ソフトウェアアーキテクチャレビュー


アーキテクチャドキュメント
----

ソフトウェアは様々な視点で捉えられ、これをビュー (View) と呼びます。
アーキテクチャドキュメントはビューの集合と相互作用の説明で構成されます。

### アーキテクトが作成すべき図の例

- パッケージ図
    - パッケージ間の依存関係
    - それぞれのパッケージに何が配置されるか
    - ソースコードやライブラリの物理構造
- コンポーネント図
- 配置図
    - 物理的なハードウェアやソフトウェアの配置
    - 実行環境を表すノードとの接続関係など


アーキテクチャパターン
----

アーキテクチャパターンとは、「**アーキテクチャの要素と要素間の関係を使い方の制約と共に特定したもの**」です。 ─出展『Principles of software architecture』。

様々なアーキテクチャパターンが提唱されており、GOF のデザインパターンとは違い統一された解釈は存在しません。

- Shaw and Garlan "Software Architecture" ― <b>Shaw と Garlan のリスト</b>
- Buschmann et al. "Pattern-oriented Software" ― <b>Buschmann らのパターンシステム</b>
- Clements et al. "Documenting Software Architecture" ― <b>CMU SEI のパターンカタログ</b>

パターンの説明を覚えるより、「ルール」や「どの品質特性に影響するのか」を理解することが重要です。
例えば、Layered Pattern に関しては、

- <b>ルール</b> -- 各レイヤはすぐ下のレイヤのサービスのみ呼び出せる。
- <b>特性</b> -- Modifiability は上がり、Performance は下がる。

といった点を抑えておくことが重要です。


パフォーマンス解析／チューニング
----

- Profiling (computer programming)（日本語: 性能解析）
    - [Performance analysis - Wikipedia](https://en.wikipedia.org/wiki/Performance_analysis)
    - [性能解析 - Wikipedia](https://ja.wikipedia.org/wiki/%E6%80%A7%E8%83%BD%E8%A7%A3%E6%9E%90)
        - ページの最後にプロファイラのリストがあります。
- [Performance tuning - Wikipedia](https://en.wikipedia.org/wiki/Performance_tuning)
- Performance counter
    - [Hardware performance counter - Wikipedia](https://en.wikipedia.org/wiki/Hardware_performance_counter)
    - [Working with Performance Counters - Microsoft TechNet Archive](https://docs.microsoft.com/en-us/previous-versions/tn-archive/bb734903(v=technet.10))


並列処理に関する制御モデル
----

並列性を利用するコードは、一般的に 3 種類のモデルに基いて構築されます。

- <b>ワーククルーモデル</b>: 類似した一連のタスクを並列的に実行。
- <b>ボス、ワーカーモデル</b>: ボスがワーカーに作業を配分。
- <b>パイプラインモデル</b>: データを中心に一連のタスクを実行し、データを次のタスクへ渡す。

出展:『Code Reading』


