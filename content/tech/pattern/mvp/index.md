---
title: "デザインパターン: MVP パターンのメモ"
linkTitle: "MVP パターンのメモ"
url: "/p/5wu6fbv"
date: "2018-10-12"
tags: ["デザインパターン"]
---

MVP (Model-View-Presenter) パターンでは、MVC (Model-View-Controller) パターンと異なり、View と Model が直接やり取りせず、すべて Presenter 経由でのやり取りになります（亜種としてデータバインディングを使用するケースもあります）。


MVP パターンの構造
----

#### オブジェクト図

{{< image src="mvp-object.png" code="mvp-object.puml.txt" >}}

#### シーケンス図

{{< image src="mvp-sequence.png" code="mvp-sequence.puml.txt" >}}


MVP の構成要素
----

- <b>View</b>: 描画ロジックを持つ。自分自身が能動的に描画処理を行うことはなく、Presenter からの要求で描画処理を行うため、Passive View と呼ばれる。Presenter に言われたとおりに、渡されたデータを描画するだけ。ユーザの入力を受け、Presenter へ通知する。
- <b>Presenter</b>: ユーザイベントをハンドルし、具体的なアクションを実行する (Proxy)。データを Model から取得し、View が扱える形に加工して描画情報として渡す。
- <b>Model</b>: ビジネスロジックとデータ保持（およびデータ取得処理）を担う。DB や Web サービスと通信してデータを取得する役割を持つ。必要に応じて Presenter へ変更通知を送る。


MVP の Pros and Cons
----

- Pros.
    - 複雑なタスクをシンプルなタスクに分割できる。
    - クラスが小さくなり、不具合が入りにくくデバッグしやすくなる。
    - ユニットテストできる。
- Cons.
    - 各レイヤを繋ぐ退屈な作業が必要。
    - Model が特定のユースケースに結びついてしまい再利用しにくい。
    - View と Presenter が特定のデータ形式により結び付けられてしまう（Model の提供するデータを Presenter 経由で View に渡すときにデータの依存ができる）。

MVP の各コンポーネントの生成過程
----

MVP パターンを導入しようとするときに、最初に迷うのがオブジェクトの所有関係をどうするかだと思います。
下記は MVP の一般的なオブジェクトの生成過程です。

1. アプリを起動すると **View が生成される**。
2. **View は**自分自身を制御してもらうための **Presenter を生成する**。Presenter に自分自身の参照を渡しておく（これは、Presenter から View を制御してもらう必要があるため）。
3. **Presenter は**ビジネスロジックを実行するための **Model を生成する**。

Presenter は、View や Model の参照を持つ必要があるわけですが、このとき View の具象クラスの参照を保持するのではなく、インタフェース型の参照（IView的な）として保持するのがポイントです。
こうすることにより、Presenter の単体テストが可能になります（View のモックを作成する）。

下記は、Wikipedia にある View の実装例です。
View のコンストラクタで Presenter を生成しています。

{{< code lang="csharp" >}}
public class DomainView : IDomainView
{
    private IDomainPresenter domainPresenter = null;

    /// <summary>Constructor</summary>
    public DomainView()
    {
        domainPresenter = new ConcreteDomainPresenter(this);
    }
}
{{< /code >}}


MVP のテストで何を行うか？
----

### View のテスト

{{< image src="mvp-test-view.png" >}}

描画ロジックをテストする。
Presenter とのインタラクションをテストする。
Presenter のモックを作成する。

### Presenter のテスト

{{< image src="mvp-test-presenter.png" >}}

View からのイベントを受けて、正しく Model のメソッドを呼び出しているかをテストする。
View と Model のモックを作成する。

### Model のテスト

{{< image src="mvp-test-model.png" >}}

ビジネスロジックをテストする。
Presenter と DataSource のモックを作成する。

