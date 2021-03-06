---
title: "読書メモ『J2EE パターン』 Deepak Alur、John Crupi、Dan Malks"
linkTitle: "『J2EE パターン』 Deepak Alur、John Crupi、Dan Malks"
url: "/p/saanpfh"
date: "2018-07-19"
tags: ["読書", "Java", "デザインパターン"]
working: true
---

J2EE のパターンコレクションを示した本です。
有名な GoF のデザインパターンとは異なる、21 のパターンが紹介されています。
既存のプロジェクトにパターンを適用するにはリファクタリングが必要になりますが、その方法も示されています。


### Grady Booch（Rational Software Corporation 主任研究員）のまえがき

パターンに名前を付けることによって、新たな語彙を手に入れ、それまでは気付かなかったであろう方法でパターンを適用できるようになる。

慢性的に時間に余裕がないところでは、美しいソフトウェアはなかなか書けない。
しかし、適切なパターンを選んで適用すれば、システムにある程度の優雅さをもたらすことができる。


### マイクロアーキテクチャとは

マイクロアーキテクチャは、アプリケーションおよびシステムの構築に利用できるビルディングブロック。個々のパターンよりも抽象度が高く、複数のパターンをリンクさせ組み合わせて表現することができる。


第1章: パターンとJ2EE
----

### パターンの定義

> パターンとは、特定の文脈、問題、および解決策の相関関係を表した、3要素から成る規則である。
> <div style="text-align:right">─Christopher Alexander 『A Pattern Language』</div>

> パターンとは、「特定の文脈」、「その文脈において繰り返し発生する問題」、および「これらの問題を解決する特定のソフトウェア構造」の3要素の相関関係を表した規則である。
> <div style="text-align:right">─Richard Gabriel『A Timeless Way of Hacking』</div>

> パターンとは、ある現実の文脈の中で有用であって、他の文脈の中でもおそらく有用であろうと思われるアイデアである。
> <div style="text-align:right">─Martin Fowler『Analysis Patterns』</div>

<b>**繰り返し発生する問題**と、その解決策に関する知識を表現して伝えるには、パターンが理想的なツールである</b>。
考慮事項、アイデア、メモなどの知識を公式に文書化して伝えることができるようになる。


第2章: プレゼンテーション層における設計上の考慮事項とバッドプラクティス
----

この章では、ユーザセッションやクライアントアクセスの制限に関しての概要が述べられている。
プレゼンテーション層におけるバッドプラクティスがざっと羅列してあるが、特に目新しさは感じられない（コントローラーが肥大化したら他のオブジェクトに処理を委譲する、といった程度のことしか書いてない）。
第6章以降のパターンカタログの方を見ていくのがよい。


第3章: ビジネス層における設計上の考慮事項とバッドプラクティス
----

ステートレスなセッションBeanであれば、Beanをプールしておいて、複数のクライアントで使いまわせる。
スケーラビリティの問題のほとんどは、ステートフルセッションBeanや、ステートレスセッションBeanの誤用によるものである。
スケーラビリティが必要なシステムでは、ステートレスセッションBeanを使用することがより実用的な設計戦略となり得る。
1回のメソッド呼び出しでサービスが完了するビジネスプロセス（非対話型ビジネスプロセス）を実装するには、ステートレスセッションBeanを使うとよい。

スケーラビリティの向上を狙って、どのようなケースでもステートレスセッションBeanを選択する設計者もいる。
しかし、こうした設計を行ったために、ネットワークトラフィックのオーバーヘッド、再構築時間、アクセス時間などが絡んできて、かえってスケーラビリティが低下してしまっては本末転倒である。

