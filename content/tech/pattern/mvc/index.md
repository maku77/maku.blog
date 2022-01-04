---
title: "デザインパターン: MVC パターンのメモ"
linkTitle: "MVC パターンのメモ"
url: "/p/cps3nt8"
date: "2018-10-12"
tags: ["デザインパターン"]
---

MVC パターンの構造
----

### オブジェクト図

{{< image src="mvc-object.png" code="mvc-object.puml.txt" >}}

### シーケンス図

{{< image src="mvc-sequence.png" title="シーケンス図" code="mvc-sequence.puml.txt" >}}

### M (Model)

**処理。ビジネス・ロジック。**

アプリケーションが使用するデータを保持し、アプリケーション固有の処理を実行する。
外部のストレージ（データベース）に保存するためのインタフェースなどを備えていてもよい。

特に、保持するデータを参照するための public インタフェースだけを備えているものを Model ということもある。J2EE デザインパターンでは、純粋にデータのみを保持するものを Value Object と呼んでいる。

**View、Controller とは関連性を持たないのが望ましい。**
Model オブジェクトは、自分自身がどのように表示されるかを知らない。

**特定の実行環境に依存しないのが望ましい。**
例えば、特定のフレームワーク上に構築されたアプリケーションであっても、Model クラスだけは別のフレームワークでも使用できるようにするべき。つまり、フレームワークに特化したインタフェースを備えてはいけない。


### V (View)

**表示。プレゼンテーション・ロジック。**

データを目に見える形で表示するためのコード。ウィンドウアプリケーションでは、ウィンドウ内に表示するウィジット等の表示、Web アプリケーションでは、HTML の出力などが View に当たる。


### C (Controller)

**入力。インタラクション・ロジック。**

ユーザやシステムからのイベントを受けとり、Model や View の作成、更新、相互作用を管理する。


MVC パターンの Pros and Cons
----

- 利点 (Pros.)
    - UI コードにビジネスロジックが入るのを防ぐことができる。
    - ユニットテストしやすい。
- 欠点 (Cons.)
    - スケールしない（UI は個々に作成できるが、モデルを分離しにくい）
    - Contoller が肥大化しやすい（View からの処理がすべて Controller 経由になる）
    - SOLID 原則の S (Single responsibility principle) と I (Interface segregation principle) に違反してしまう。


View と Controller の関連
----

多くの GUI toolkit では、V (View) と C (Controller) は複雑に絡み合っている。なぜなら、C (Controller) は、それ自体が画面上に表示される widget であることが多いから。

例えば、 wxPython では、画面上に表示されるウィジット wx.Window (View) は、ユーザの入力を受け取る wx.EvtHandler (Controller) のサブクラスとして定義されているため、「View = Controller」 である。

逆に、一般的な Web アプリケーション・フレームワークは、Controller（サーバー上での処理）と、View（ブラウザへの表示）が明確に分かれていることが多い。

- 参考: {{% amazon-inline id="1932394621" title="wxPython in Action" %}}


MVC の パッシブ・モデルとアクティブ・モデル
----

### パッシブ・モデル

1. ユーザ入力 → Controller
2. Controller →（更新）→ Model
3. Controller →（更新通知）→ View
4. View →（最新データ取得）→ Model

パッシブ・モデルでは、Model が更新されたとき、Controller が View に対して通知する。

### アクティブ・モデル

1. ユーザ入力 → Controller
2. Controller →（更新）→ Model
3. Model →（更新通知）→ View
4. View →（最新データ取得）→ Model

アクティブ・モデルでは、Observer パターンを利用し、Model オブジェクトに更新があったときに Model オブジェクト自身が、リスナとして登録されている View に対して通知する。


Android プログラミングにおける MVC パターン
----

例えば、Android アプリのコーディングでは、エントリポイントが Activity クラスや Fragment クラスになるので、構造を何も考えずに機能を追加していくと、Activity や Fragment が肥大化してしまいます。
最初から MVC パターンを意識したコーディングを行うことで、見通しがよく保守性の高いコードを作成できます。

- Activity や Fragment が肥大化してしまうのを防ぐことができる。
- ロジックが分かりやすく、新しい機能を追加するのに時間がかからない。
- Activity や Fragment からビジネスロジックを分離できるのでテスト可能になる。

