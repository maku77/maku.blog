---
title: "読書メモ『ダイアグラム別 UML 徹底活用』井上樹"
linkTitle: "『ダイアグラム別 UML 徹底活用』井上樹"
url: "/p/xfgoaq3"
date: "2018-05-30"
tags: ["読書", "UML"]
---

{{% amazon
  itemId="4798118443"
  title="ダイアグラム別UML徹底活用 第2版"
  author="井上 樹"
  publisher="翔泳社"
  imageUrl="https://images-na.ssl-images-amazon.com/images/I/81gVRJsq4cL.jpg"
%}}

（全13章）

1章: モデリングのメリットを考える
----
* モデリングの連続の末にプログラムというモデルの最終形態に辿り着くことが、システムを開発するということ。
* 図によるモデル化のメリット
    * 情報量が多い
    * 理解しやすい
    * 誤解が少ない
* UML2.0 について
    * ダイアグラムが10種類から13種類に増加（コンポジット図、相互作用概要図、タイミング図）し、次のようなことをモデル化できるようになった。
        * 複数のインスタンスを内包するクラスの内部構造
        * 複数のインタラクションの時系列上の流れ
        * 複数のインスタンス間でのメッセージのやり取りと、それに伴う状態変化
    * ダイアグラム名称の一部変更
        * ステートチャート図 → ステートマシン図
        * コラボレーション図 → コミュニケーション図
    * 「フレーム」が追加され、ダイアグラム全体を囲んだ入り、一部を囲んで説明を加えることができるようになった。ダイアグラムの一部をフレームとして囲んだところを畳み込めば、大きなダイアグラムでも全体を見渡しやすくなる。


2章: ユースケース図の注意点と使いどころ
----

* ユースケースは、**「こういうものを作るんだ」**ということを明らかにするために描く。
* 関連を結んだアクター＝ユースケースの起動者ではない。関連はあくまで関わり合いがあることだけを示す。
* UML2.0 におけるユースケースの変更点は、extend の関係に対してノートで条件を書けるようになったこと。

### 役立つユースケースにするには

* 目的／読者を確認する
* 名前付けに注意する
    * 抽象度: 「一般会員」や「レジ係」というように文章に出てくるくらいの抽象度がよい。
    * 正確性: 対象を明示する（例: 貸出→ビデオを貸し出す、返却→ビデオの返却を受け付ける）
    * 表現の統一: 同じ意味を示す言葉は揃える（例: 「貸し出す」と「レンタルする」）
* 粒度を揃える
* 機能分割にしない: 機能単位で描くと粒度が細かくなりすぎて、そのモデル化した対象がどんなサービスを提供しているのかわからなくなってしまう。サービスで分割することを意識する。
* ≪include≫は1段階まで: 2段階以上にするとアクターにとって意味のある粒度のユースケースになりにくい。
* ≪include≫、≪extend≫、汎化関係を混同しない
    * <b>≪include≫</b>: ≪include≫先のユースケースは必ず必要になることを意味する。プログラムの関数呼び出しのような感じで、複数のユースケースから共有できる（例: ビデオを借りる─include→会員かどうかを確認する）。
    * <b>≪extend≫</b>: ≪extend≫でベースになっているユースケースにサービスを付加した場合は、ベースとなるユースケースの実行には≪extend≫で定義したユースケースは必ずしも必要ない（例: ビデオを借りる←extend─カードで料金を払う）。逆に、≪extend≫で定義されたユースケースを実行するには、必ずベースのユースケースが必要になる。
    * <b>汎化関係</b>: 汎化関係は機能を追加するのではなく、概念だけ共通だがまったく新しいユースケースを定義する（例: ビデオを借りる←汎化─ビデオを宅配で借りる）。
* ユースケース記述と合わせる: ユースケース図だけでは誤解を招きやすいので、ユースケース図を描いたら、ユースケース記述も合わせて作成する。ユースケース図とユースケース記述を合わせて**「ユースケースモデル」**と呼んだりする。


3章: ユースケース記述の注意点と使いどころ
----

### ユースケース記述の概要

* ユースケース記述は**システム開発の初期に**ユースケース図と一緒に作られ、**システム外部から（利用者から）見たときの振る舞いを明確にする**。
* 特に担当者を入れ替えながら開発が進んでいくプロジェクトなどでは、**開発者間でシステムイメージを共有**できるということは重要。
* ユースケース記述は書くのに非常に手間のかかる成果物だが、**システム開発に一貫したゴールを与える**ことができるのは大きなメリット。**ユースケースのレベルで再利用**できるようになると、より楽をできるようになる。

### ユースケース駆動開発

* ユースケースは**進捗を計るベースとして使用**できる。ユースケースを一単位とすると、代替フローや例外フローが含まれているため、作業単位の粒度としては大きくなりすぎることがある。そのような場合、ユースケース記述に書かれた**「シナリオ」を作業単位とする**とよい。
* ユースケース駆動開発では、ユースケースの**シナリオごとに、(1)分析、(2)設計、(3)実装、(4)テスト**という作業を進めていく。ユースケースを実現できたかどうか（それぞれのステップが完了したかどうか）のポイントは下記の通り。
    1. 分析: **分析のクラス図に書かれているクラス群**を使って、ユースケース記述に書かれたシナリオを実現する相互作用図（シーケンス図、コレボレーション図）を作ることができる
    2. 設計: **設計のクラス図に書かれているクラス群**を使って、ユースケース記述に書かれたシナリオを実現する相互作用図（シーケンス図、コラボレーション図）を作ることができる
    3. 実装: シナリオを実現するためのプログラムが書けている
    4. テスト: シナリオに書いてある通りにシステムが動いている

### ユースケース記述のテストへの活用

* ユースケース記述に書かれた振る舞いは、**要求元と合意されたもの**となるので、テストケースの元ネタとして使うことができる。
* テストケースとシナリオは対応するものなので、テストを意識してシナリオリストを作成すると、テストケースの作成が容易になる。イベントフロー（メインフロー＋代替フロー＋例外フロー）の組み合わせを網羅する形でシナリオリストを作るのがコツ。
* イベントフローは３種類
    * メインフロー: 正常に処理が進んだ場合。
    * 代替フロー: 正常系の代わり（エラーが発生した場合など）。代替フローを実行した場合でも**事後条件は満たされる**。
    * 例外フロー: ユースケースの実行を断念しなければならないような場合。例外フローを実行した場合は**事後条件は満たされない**。

### ユースケース記述のポイント

* 厳密に漏れなく書く
    * **曖昧性のない**厳密な記述がされており、システムの振る舞いが具体的に理解できること。
    * ユースケース記述の作成と合わせて**用語集を作成する**とよい。
    * イベントフローの書き出しは「アクターは～」、「システムは～」と動作の**主語を必ず書く**。
    * **システムで実現すべきことはすべて**ユースケースに書かれているようにする
    * 仕様変更が発生した場合など、**ユースケース記述はこまめにメンテナンスする**。
* 細かくしすぎない
    * エンドユーザにもわかるレベルで記述する（エンドユーザーと開発者が共有するものなので）。**ユーザマニュアルのようなレベル**を想定するとちょうどよい。
    * データベースのテーブルや、画面レイアウト、画面遷移を意識する記述が出てきたら危険。
    * イベントフローに「システムは～する」という記述が連続して 4 ～ 5 ステップ続いたら詳細になりすぎている可能性が高い。
    * **危険なワード**: DB、テーブル、クエリ、キー、ID、コネクション、チャネル、セッション、トランザクション、ロック、ボタン、クリック、入力欄、インタフェース
* 補足資料を使う
    * 文章だけで記述するのではなく、**アクティビティ図や、スケッチ、表**なども併用すればよい。
* 空欄を作らない
    * ユースケース記述のフォーマットには項目がたくさんあるが、空白状態で放置しない。**「T.B.D.」なのか「なし」なのかを明確に記述**。T.B.D. であれば、期日と担当者を併記するとよい。
* 書き続けない
    * システムを動かしてから見えてくる部分は少なからずある。決まっていない部分は T.B.D. として先の作業に進むことも必要。
* 無理にユースケース記述で書かない
    * ユースケース記述に向いているのは、ユーザーとシステムのやりとりがあるインタラクティブシステム。
    * ユースケース記述に向いていないのは、フィードバック系のシステム（エアコンなど）や、複雑なアルゴリズムや計算式の記述。


4章: クラス図～基本編
----

* クラス図はいろいろな場面で使用される。開発者は一般的に設計モデルを作成するのは得意だが、分析モデルを作成するのが苦手な傾向がある。
    * <b>概念図</b>: 問題領域に**どのような概念が存在し、どのような構成になっているか**を記述する。概念は用語集で定義されるが、それぞれのつながりを表すのが概念図としてのクラス図。開発の初期段階で作成する。
    * <b>分析モデル</b>: システム**要求を満たすには何が必要か**を表現する。分析モデルはあくまで意味レベルのモデルなので、設計モデルとは違い、**実現方法そのものは記述しない**。システムの要件定義（ユースケースモデルの作成、日機能要求のリスト化）が終わった後に作成する。
    * <b>設計モデル</b>: システムの作り方を表現する。実装レベルのクラス図。
* 中央集権モデルになってしまわないようにモデリングできるとよい。
* 関連はただ線を引くだけではなく、その**関連の意味が分かるように「多重度」と「ロール名」を付ける**。「ロール名」が思いつかない場合は、「関連名」を付けるとよい。
* クラス図は、左から右、上から下へ自然に読んでいくことができるのが望ましい。
* ユーザインタフェース、I/O、デバイスなどを表すクラスは端に配置するとよい。


5章: クラス図～応用編
----

### 実現関係

UML2.0 では、あるクラスの実現に必要なインタフェースを、Y字型のアイコンを使って明示できるようになった（インタフェースの要求）。

```
RentalBusiness -------( o------- CounterRentalProcess
               RentalProcess
```

上記の図では、RentalBusiness は RentalProcess インタフェースを必要としており、RentalProcess インタフェースを実現した CounterRentalProcess を参照することを示す。

実現関係（インタフェースとその実装）は、実装の方法を意識した関係になるため、分析レベルのクラス図には登場しない。


### 依存関係と関連

**関連**がクラスの属性と同じ扱いなのに対し、**依存関係**はある操作の中だけといった一時的な利用関係を表す。


### 関連クラス

関連クラスの使い方を覚えておくと、分析としてのクラス図の表現力がぐっと上がる。
ただし、**関連クラスは設計のクラス図には使えない**ことに注意。
なぜなら、関連クラスをそのまま実装できるプログラミング言語が存在しないから。
関連クラスは、設計段階では３つのクラスの関係に置き換えればよい。


### ステレオタイプ

ステレオタイプはギルメット（≪≫）で囲まれた文字列で、ステレオタイプはクラス名だけではなく、すべてのモデリング要素に複数付与することができる。

ステレオタイプは UML であらかじめ数種類用意されているが、ユーザーが自由に定義してよいことになっている。
ステレオタイプはあくまで理解を助けるためのものであって、あまり多くの観点のステレオタイプを作成するのはおすすめしない。クラスを色分けするなどして、1つのクラスに付けるステレオタイプは多くても2つまで、できれば1つにすると良いモデルになる。


### ステレオタイプを活用したクラスの役割整理

クラス図を作成したら、以下のようなステレオタイプを使ってクラスの責務や関連を整理するとよい。

| ステレオタイプ | 役割 | 例 |
| ---- | ---- | ---- |
| <b>≪boundary≫</b> | システムとその外側をつなぐインタフェース。UI やデータ入力に関わるクラス。デバイス制御を行うクラスなど。**(プレゼンテーション層)** | 貸出画面Servlet、レシートプリンタ |
| <b>≪control≫</b> | まとまった処理手順を司る。業務フローやデータフロー、画面遷移に関わるクラスなど。 | 貸出処理クラス |
| <b>≪entity≫</b> | 情報を表す。データ自身の基礎的な処理や、保持を行うクラスが該当する。**(ドメイン層)** | 会員クラス、貸出クラス |

1. すべてのクラスに上記のステレオタイプを付けていく。複数に該当するものは複数付ける。
2. ステレオタイプが2つ以上付いているクラスを分割する（責務を1種類にする）。
3. 次のようなルールに従って依存を整理する。

{{< image w="600" src="uml-tettei-5-1.png" >}}

▽これだと ≪boundary≫ が ≪entity≫ の情報を表示できないかのように見えるけど、≪control≫ を介して取得した情報を表示するのは OK なので念のため。

▽ちなみに、ロバストネス分析という分析手法では以下のようなシンボルを使用するが、考え方は同じ。

{{< image w="400" src="uml-tettei-5-2.png" >}}


6章: オブジェクト図
----

### オブジェクト図の概要

* オブジェクト図はオブジェクト（インスタンス）を対象とした図で、モデリング対象の**ある一瞬だけを捉えた図**。どの一瞬を表した図なのか、ノートとして残しておくとよい。
* クラスに対するインスタンスと同様に、1つのクラス図からはオブジェクト図をいくつでも作ることができる。
* オブジェクト図はクラス図ほど描かれることはないが、オブジェクト図は描いてみると役に立つことが分かる。オブジェクト図をきちんと描けるかどうかによって、その人が**オブジェクト指向をどれだけ理解しているかが分かる**。
* UML1.x ではオブジェクト図はクラス図の一部分とされていたが、UML2.0 からは正式なダイアログとして定義された。

### オブジェクト図の使いどころ

1. システム化対象からオブジェクトやクラスを発見するために作成する。
    * システムをオブジェクトの集合としてとらえた場合に、どのようになるかをモデル化する。
    * **システム要件からいきなりクラス図を作成するのは難易度が高い**。なぜなら、「概念群」を直接導き出す抽象化力が必要だから。システム要件からは、まずは実体をベースとするオブジェクト図を作成し、そこからクラスを導き出してクラス図を作成する方が容易。
2. クラス図が作成者の考えている通りのオブジェクトの構造を表せるか確認する。
    * これはクラス図を描いてからオブジェクト図を描くケース。クラス図のレイアウトに近い形でオブジェクト図を描くと分かりやすくなる。
3. オブジェクト図を2つ使い、システム内の変化を表現する。
    * ユースケースの事前条件と事後条件に相当する部分をそれぞれオブジェクト図で表現することで、システム内部にどのような変化が起きたのかを分かりやすくする。


7章: 相互作用図～シーケンス図とコラボレーション図（UML2.0ではコミュニケーション図）
----

* UML2.0 ではコラボレーション図は、**コミュニケーション図**と呼ぶ。
* 実現すべきシナリオ（ユースケース記述）から、相互作用図（シーケンス図 or コミュニケーション図）を作成すると、必要なオブジェクトを抽出することができる。このような相互作用図先行の開発では、相互作用図として**シーケンス図ではなくコミュニケーション図を使うと、クラス図との対応を取りやすい**。
* シーケンス図とコミュニケーション図の比較
    * <b>コミュニケーション図</b>: オブジェクト図と似ているので、オブジェクト構造との対応付けがしやすい。ただし、メッセージの順序が分かりにくい。
    * <b>シーケンス図</b>: オブジェクト構造は見えにくいが、メッセージの順序がわかりやすい。
* **シーケンス番号**を「通し番号表記」ではなく、「**階層化番号表記**」で書くと、メッセージのネスト構造がわかりやすくなるため、RAM使用量や実行速度のチューニングの際に役に立つ。
* メッセージのメッセージ名の前には、ガード条件を記述することができる（例: `[会員検索結果 != null] 借主登録`）
* シーケンス図では、メッセージの終了を表す**リターン**（`<----` という破線矢印）が描かれることがある。必ずしも表記する必要はなく、**戻り値があることを明示したいとき**に用いればよい。リターンは必ず**活性区間の最後の部分から**引かれる。
* 活性区間（白抜きの矩形）は「**制御フォーカス**」とも呼ばれ、オブジェクトが活動していることを表す。同じクラス内のメソッドを同期呼び出しする場合は、活性区間が重なっているように描く。オブジェクトの生存期間を表す「ライフライン」と間違えやすいので注意。
* 相互作用図を作るときは、**オブジェクトにちゃんと名前を付ける**癖をつけるとよい（`オブジェクト名：クラス名`）。オブジェクト名は、ロール名や戻り値をもとに命名すると分かりやすくなる。
* メッセージには引数も記述しておくと、データの流れが分かりやすくなる。

### シーケンス図の UML2.0 での変化点

UML2.0 のシーケンス図にはいろいろと変更が入り、コミュニケーション図よりも表現力が向上している。シーケンス図で凝った記述を行うと、コミュニケーション図との相互変換はできなくなるのでその点は注意。

* UML2.0 のシーケンス図では、UML1.5のオブジェクトとライフラインをまとめてライフラインと呼ぶ。オブジェクトの名前の下に下線は引かない。
* **オブジェクトの生成と消滅の線は破線矢印**を使う。
* **コリージョン (coregion)**の表記を使うことで、「サーバーから複数の応答メッセージを待つ」、といった表現が可能になった。
* **汎用順序付け (general ordering)**の表記を使うことで、２つの非同期メッセージの順序を明確にすることができる。▼ただ、これが使われてるシーケンス図は見たことがない (^^;
* **フレーム**を使ってインタラクションの制御構造を表現できるようになった。▽たくさんあるけど、よく使われるのは以下くらいかな？
    * <b>alt (Alternatives)</b>: 条件による選択実行。
    * <b>opt (Option)</b>: 条件が満たされたときのみ実行。
    * <b>par (Parallel)</b>: 並列実行。
    * <b>loop (Loop)</b>: 反復処理。
* フレームの左端から矢印を引くことができるため（**ゲート**と呼ぶ）、システム外部からの呼び出し（シーケンスの起点）を表現しやすくなった。UML1.5 までは、仮のアクターを左端に置いて表現していた。


8章: アクティビティ図
----

* アクティビティ図も相互作用図（シーケンス図、コミュニケーション図）と同様に振る舞いを表す図だが、**ソフトウェアの開発に限らず、様々な場面で利用できる**ので使いこなせると便利。アクティビティ図は下記のような場面で使用できる。
    * <b>業務フローのモデル化</b>: ビジネスモデリングでは、アクティビティ図を使って業務手順をモデル化できる。スイムレーンの使い方がポイントで、社内の組織を割り当てることで役割分担が明らかになる。
    * <b>ユースケース記述の補足</b>: イベントフローは通常箇条書きで書かれるが、代替フローや例外フローがたくさんあったり、繰り返し、並行処理がある場合は、アクティビティ図を添付すると分かりやすくなる。**アクターとシステムのスイムレーン**を用意すると、アクティビティをよりイベントフローの記述と対応させやすくなる。
    * <b>ロジックのモデル化</b>: オブジェクトの構成に捉われずに処理の流れを記述したい場合や、複数のシナリオを含んだモデルを描きたい場合にアクティビティ図を使用できる（従来のフローチャートと同じ）。完成したアクティビティ図にオブジェクトの視点を追加したければ、個々のオブジェクトに対してスイムレーンを割り当てればよい。
* <b>スイムレーン (Activity Partition)</b>はアクティビティの実行者を表す。スイムレーンには、「組織名」、「役割名」、「サブシステム名」、「オブジェクト名」といったものを入れることが多い。UML2.0 では、**スイムレーンは入れ子で描くことができる**。
* <b>分岐</b>の描き方はフローチャートと異なり、分岐マーク（◇）の中には何も記述しない。分岐マークを省略して、アクティビティから直接2本線を出し、2つのアクティビティに繋げるように描いてよい。
* <b>アクティビティ</b>は、１つの処理や業務の作業を表すが、**アクティビティ名は達成すべきこと（目的）をベースに命名する**のがよい。実際にやることをベースに記述してしまうと、モデルを頻繁に書き換えることになる。特にビジネスモデリングでアクティビティ図を作成するときは、アクティビティ名の付け方で、そのモデルが長い期間使えるかどうかが決ってくる。
    * <b>よい例</b>: 「会員を特定」→「貸出対象を特定」→「貸出期間を決定」
    * <b>悪い例</b>: 「会員カードの読取」→「DVDのバーコードの読取」→「何泊か入力」
* アクティビティ図を描く手が止まってしまったら、**後ろから描いていくという方法**を試してみるとよい。何を達成したいのかは分かっているので、終了状態から描いていくと自然とできあがっていく。
* 開始点は Initial Node （黒丸●）の 1 種類だけだが、UML2.0 では**タイムイベント**という記号（▽と△を組み合わせた記号）が追加されており、時間に依存したイベントを表現できる（例: 「毎月25日」）。
* 終了点は**フロー終了点 (Flow final node)** と、**アクティビティ終了点 (Activity final node)** の2種類がある。
    * <b>フロー終了点 (Flow final node)</b>: そこに流れてきたフローだけ止める。例えば、並行処理の一つだけを終了するときに使用する（アクティビティ図全体は終了しない）。記号は、白丸の中に× ($\bigotimes$)。
    * <b>アクティビティ終了点 (Activity final node)</b>: アクティビティ図全体の処理を終了するときに使用する。記号は、白丸の中に黒丸 (&#x25C9;)。


9章: ステートチャート図（UML2.0ではステートマシン図）
----

ステートマシン図には、**ライフサイクル全体を扱える、振る舞いの網羅性が高い**、といった特徴がある。

* UML2.0 には振る舞いを表す図として相互作用図（シーケンス図とコミュニケーション図）もあるが、それらは特定のシナリオに沿った振る舞いを表現するためのもので、ライフサイクル全体を表現することができない。
* ユースケース図だけでは、どのユースケースがどのタイミングで実行できるようになるのかが分からない。ステートマシン図でシステムの取り得る状態を明らかにし、各状態でどのユースケースが実行されるのかを描くことで、**システム全体のライフサイクルとユースケースの関係を明らかにすることができる**。
* 特にイベントの種類が多く、平行動作の多い組み込み系システムではステートマシン図は必須。

状態遷移（矢印）では、次のような定義を行うことができる。

```
　イベント(引数リスト)[ガード条件]／アクション
────────────────────────＞
```

* <b>イベント</b>: この状態遷移を発生させるイベント。
* <b>引数リスト</b>: イベントの発生に伴って渡される引数。複数定義可能。
* <b>ガード条件</b>: この条件が成立しているときのみ遷移が発生する。
* <b>アクション</b>: この状態遷移に伴って実行されるアクション。

上記の定義はすべて省略可能で、「イベント」の定義が省略された場合は、「**無条件遷移**」あるいは「**ラムダ遷移**」と呼ばれ、遷移元の状態の entry アクションの実行が終わり次第、自動的に遷移先の状態に遷移することを表す。


10章: コンポーネント図と配置図
----

### 概要

コンポーネント図と配置図は、主に物理的なものをモデリングするために使用する（論理的なものも表現可能）。
多くの UML 解説書では、なぜかコンポーネント図と配置図の説明だけおざなりにされているが、実装担当者／ビルド担当者／運用担当者にとっては役に立つ場面が多いので使ってみるとよい。

* コンポーネントとは、**「交換可能なシステムの構成部品で、実装を内部に含み、外部にインタフェースを公開しているもの」**（UML1.5の定義）。コンポーネント図では、そのようなコンポーネントの間の関係を示す。
* 配置図は、システムを構成するハードウェア構成とその上でどのようなソフトウェアが動作しているかを表す。

### 使いどころ

コンポーネント図と、配置図は、システムアーキテクチャを検討する際に役に立つ。

* コンポーネント図では、コンポーネント＝ファイルとしてダイアグラムを描けるので、ファイルの依存関係を表現するために使用できる。
* コンポーネント図を描くと、システムを構成するコンポーネントの依存関係が明らかになるので、システムを運用環境にセットアップする際のインストール順序を示すためにも使用できる。
* 分散システムやデバイス制御を伴うシステムにおいて、システム内でハードウェアをどのような接続するかの検討に使用できる。

### パッケージ図と混同しない

Java のパッケージ構成などを示すことができるパッケージ図は、コンポーンネント図と混同されがち。
パッケージはあくまでフォルダのようなものであり、ソフトウェア部品にはなり得ないという点でコンポーネント図とは異なる。
混乱してきたら、「メッセージが送れるもの＝コンポーネント」、「メッセージが送れないもの＝パッケージ」と考えるとよい。


11章: ダイアグラム間の整合性
----

ダイアグラム間の整合性は、既存のダイアグラムを参照しながら作っていく、あるいは別のダイアグラムを作ることで分かったことを既存のダイアグラムにフィードバックするという方法で保つ。
下記のようなダイアグラム間の整合性をチェックする。

* クラス図とオブジェクト図
* クラス図と相互作用図（シーケンス図やコミュニケーション図）
* クラス図とステートマシン図


12章: UMLとプロセス
----

### 開発プロセスと管理プロセス

**プロセスには次の2つがあり、どちらかをおろそかにしてはいけない**。

- <b>開発プロセス</b>: ソフトウェアの開発作業に関するプロセス。「分析→設計→実装→テスト」といったもの。
- <b>管理プロセス</b>: プロジェクトを正しく進めるためのプロセス。プロジェクト計画、要求管理、障害管理、構成管理、品質管理、リスク管理など。

管理プロセスに関しては、PMBOK や CMM を参照するのがよい。

### UML を使ったプロセス

UML は特に、(1)要求定義、(2)分析、(3)設計という工程の中で使用される。
各工程の中で、複数のダイアグラムを組み合わせて対象の特定の側面を表現していく。
このダイアグラムの組み合わせをビューという。

プロセスを定義するときは、各工程でどのようなビューを作成するのか、その目的は何か？どのように妥当性をチェックするのか？などを定義する。
下記は、各工程の中でどのように UML を利用してビューを構築していくかのプロセス定義の例。

#### 1. 要求定義

要件定義の工程では、システムとして何を実現すべきかを明確にする。

- 手順
    1. <b>要求リストの作成</b>: 利害関係者から聞き出したシステムに対する要望から実現すべきことを要求リスト（**機能要求と非機能要求**）にまとめる。
    2. <b>ユースケース図の作成</b>: 要求リストのうち機能要求に相当するものをユースケース図としてモデル化する。
    3. <b>ユースケース記述の作成</b>: ユースケース図のそれぞれのユースケースに関して、ユースケース記述を作成する。イベントフローが複雑になるのであれば、アクティビティ図も作成する。
- 成果物
    - <b>要件ビュー</b>（ユースケース図＋ユースケース記述＋アクティビティ図）

#### 2. 分析

分析の工程では、要件定義で定めたシステム化対象範囲を、オブジェクト指向で見たときにどのようなオブジェクトがあれば実現できるかを検討する。
分析の段階では、**非機能要求は作業の対象としない**ことに注意。
使用する言語やデータベースなどの制約に捉われず、機能要求を満たすために何が必要なのかだけを考える。

- 手順
    1. <b>クラス図の作成</b>: 要件ビューの情報を基にクラス図を作成する。
    2. <b>相互作用図の作成</b>: ユースケースのシナリオに基いた相互作用図（シーケンス図、コミュニケーション図）を作成し、クラス図がユースケースを満たしているかを確認する。
    3. <b>オブジェクト図の作成</b>: クラス構造が複雑な場合は、オブジェクト図を作成し、クラス図、相互作用図との整合性を確認する。
    4. <b>ステートマシン図の作成</b>: システム全体の状態遷移をモデル化し、アクションとしてユースケースをマッピングし、すべてのユースケースがマッピングされるかを確認する。
- 成果物
    - <b>分析ビュー</b>（クラス図、オブジェクト図、相互作用図、ステートマシン図）

#### 3. 設計

設計の工程では、**非機能要求（環境制約や性能）を満たしながら具体的にどのように実現していくか**を検討する。この工程ではすべてのダイアグラムを利用する。
インプットとしては、これまでの工程で作成した下記のものを使用する。

- <b>非機能要求リスト</b>
- <b>要件ビュー</b>（ユースケース図＋ユースケース記述＋アクティビティ図）
- <b>分析ビュー</b>（クラス図、オブジェクト図、相互作用図、ステートマシン図）

設計には明確な手順は存在しないが、システム全体を視野に入れた設計の方向性を示すアーキテクチャ設計を最初のステップとして実施することがある。
設計の工程で作成されたダイアグラム群を<b>設計ビュー</b>といい、すべての非機能要求を満たすことができた時点で設計は完了となる。
これらのダイアグラム群に関しても、要件ビューで定義したユースケースを満たしているかを確認する。

### 最低限のプロセスでは、、、

ミニマムなプロセス定義をするのであれば、ユースケース図、クラス図、シーケンス図あたりを必須にしておくとよい。


