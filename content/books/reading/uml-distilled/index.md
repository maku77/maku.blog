---
title: "読書メモ『UMLモデリングのエッセンス第3版』マーチン・ファウラー"
linkTitle: "『UMLモデリングのエッセンス第3版』マーチン・ファウラー"
url: "/p/tc73ttt"
date: "2018-10-19"
tags: ["読書", "UML"]
working: true
---

{{% amazon
  itemId="4798107956"
  title="UMLモデリングのエッセンス第3版"
  author="マーチン・ファウラー"
  publisher="翔泳社"
  imageUrl="https://images-na.ssl-images-amazon.com/images/I/51P6D4Q7T8L.jpg"
%}}

大きな仕様である UML の中で重要なポイントを簡潔にまとめた UML のバイブル的書籍です。
マーチンファウラーの名前は、『リファクタリング』や『アナリシスパターン』といった書籍で目にした人も多いでしょう。
長年オブジェクト指向に携わってきたファウラー氏のまとめたエッセンスに振れれば、UML の重要なポイントを効率よく一気に学ぶことができます。
本書は、UML を創ったスリーアミーゴ（Grady Booch、Ivar Jacobson、James Rumbaugh）の推薦図書でもあります。


第1章: UMLの概要
----

UML の使い方に関する面倒な論争に対するアプローチとして、Steve Mellow と Martin Fowler は、UML の使い方を 3 種類のモードに分類している。

- スケッチ（もっとも一般的な使われ方）
    - <b>フォワードエンジニアリング</b>: コードを記述する前に UML ダイアグラムを作成する。
    - <b>リバースエンジニアリング</b>: 既存のコードを理解する補助手段としてコードから UML ダイアグラムを作成する。
- 設計図面（blueprint。実装のため完全性を重視）
- プログラミング言語（Executable UML など。うまくいかず、流行らなかった）

UML のユーザーは UML の本質はダイアグラムだと考えている人がほとんだが、UML の作成者は **UML の本質はメタモデルである**と考えている。ダイアグラムはメタモデルの表現にすぎない。

細部までフォワードエンジニアリングされた設計図面はうまく扱うのが難しく、開発作業の遅れに繋がる。
サブシステムのインタフェースレベルで設計図面を作成するのは合理的だが、実装するのに合わせてインタフェースが変わることを想定しておく必要がある。
このような考えから、著者のマーチン・ファウラーは、**UML をスケッチとして使用することを重視**している。

- [UMLダイアグラム一覧と分類](/p/yzs49gv)
- UMLダイアグラムが使用目的に適さない場合は、非UMLダイアグラムを積極的に使うべき。デシジョンテーブルや画面フロー図などは役に立つ。
- まずクラス図とシーケンス図の基本形態に集中することをお勧めする。これらは最も一般的であり、最も便利なダイアグラムである。


第2章: 開発プロセス
----

- モデリング技術は、それがプロセスにどのように適合するかを理解しなければ、意味を成さない。UMLの使い方は、使用するプロセスのスタイルによってかなり異なる。
- <b>ウォーターフォール型プロセス</b>では、プロジェクトをアクティビティに基づいて分解する。要求分析、設計、コーディング、テストを順番に実行する。
- <b>反復型プロセス</b>は、プロジェクトを機能のサブセットで分解する。1年間のプロジェクトでは、3ヵ月ごとの反復に分解することが考えられる。3ヵ月が終わるごとに、必要な機能の1/4がシステムに実装される。
- Steve McConnell は、{{% amazon-inline id="4756108032" title="ラピッドデベロップメント" %}}の中で、プロセスを組み合わせて使用する方法を説明している。最初の4ヵ月で要求分析と設計をウォーターフォール的に行い、コーディングとテストは2ヵ月の反復を4回繰り返す。
- **テストと統合は予測の難しいアクティビティであり、このような終了時期の見えにくいアクティビティをプロジェクトの最後に置かないことが重要**。
- 経験から言うと、単体テストのコードは製品本体のコードと同程度の量が必要。
- RUP (Rational Unified Process)
    - RUP はプロセスと呼ばれているが、実際にはプロセスのフレームワークである。RUP はプロセスを説明するための語彙と柔軟な構造を提供する。
    - RUP は本質的に反復型プロセスであり、ウォーターフォール型は RUP の考え方に適合しない。
    - RUP は UP (Unified Process) に基づいた Rational 社の製品であると考えることができる。また、RUP と UP を同じものと考えることもできる。どちらも正しい。
- **反復型開発の最大の利点のひとつは、プロセスを頻繁に改良できること**。
- **反復の最後に、チームでその反復を振り返り、次の3つのリスト (KPT) を作成するのがよい**。
    - <b>K: Keep（継続）</b>: 有効に機能していて、今後も継続して行う事項
    - <b>P: Problem（問題）</b>: 有効に機能していない事項
    - <b>T: Try（試み）</b>: プロセスを改良するための変更
- プロジェクトの最後、または主要なリリース時には、本格的にプロジェクトを振り返るのもよい。参考: https://en.wikipedia.org/wiki/Retrospective
- 要求分析で UML を使用する際の最大のリスクは、対象分野の専門家が理解できないダイアグラムを描いてしまうこと。専門家が理解できないダイアグラムは役に立たないだけでなく、開発チームに偽りの自信を植え付けてしまう。
- 詳細な文書は Javadoc のようにコードから生成されるべき。他の文書を追加するのは、重要な概念を強調するため。
- **文書化する必要があるもっとも重要な事柄のひとつに、採用しなかった設計上の選択肢とその理由がある**。これは最も忘れられがちだが、外部に対して文書化して提供できるものの中で最も実用的なものである。


第3章: クラス図（基本的要素）
----

- UML では、**特性 (feature)** という用語をクラスのプロパティや操作を指す一般的な用語として使用する。
- ソフトウェアを理解するためにクラス図を描く場合は、必ず振る舞いに関する何らかの技法と併用すべき。

### 属性と関連

- **属性と関連**は、ダイアグラムではまったく異なるように見えるが、実際は同じもの。ダイアグラムにとってあまり重要でないものに関しては属性を使用し、重要なクラスにはクラスボックスを割り当てて関連で結ぶ。何を強調するかを基準にして選択すればよい。
- **属性 (attribute)** の完全な形式は以下の通り。必須の要素は「名前」だけ。
    - `可視性 名前: タイプ 多重度 = デフォルト値 {プロパティ文字列}`
    - 例: `- name: String [1] = "Untitled" {readOnly}`
- **関連 (association)** は2つのクラスの間にある実線で、ソースクラスからターゲットクラスまで引かれる。属性とは異なり、関連では両端に多重度を表示できる。
- 多値の関連 (`*`) において順序に意味がある場合は、関連端に **{ordered}** を追加する。重複を許可する場合は **{nonunique}** を追加する。順序に意味がなく一意でもないものには、{bag} のようにコレクションを示す名前が付けられることもある。
- 関連の両端で**誘導可能性矢印 (navigability arrows)** を使用することで関連の双方向性がはっきりと示される。双方向関連を持つことを明確にするのであれば、両方向を向いた矢印を明示的に使用するのがオススメ。

### 操作 (operation)

- **操作 (operation)** とは、クラスが自分で実行しなければいけないと知っているアクションのこと。完全な構文は以下の通り。
    - `可視性 名前(パラメータリスト): 戻り値のタイプ {プロパティ文字列}`
- パラメータは属性と同様に下記のような構文で記述する。
    - `方向 名前: タイプ = デフォルト値`
    - 方向は、in、out、inout のいずれかで、デフォルトは in。
    - 例: `+ balanceOn(date: Date) : Money`
- 問い合わせ操作と、更新操作
    - システムの状態を変更せずに（副作用を伴わずに）クラスから値を取得する操作を**問い合わせ (query)**と呼び、**{query}** というプロパティ文字列で表すことができる。
    - システムの状態を編こする操作は**更新 (modifier)**、または**コマンド**と呼ばれる。
    - 厳密には、問い合わせ (query) と更新 (modifier) の違いは、観測可能な状態を変更するかどうか。
    - 一般的には、modifier 操作は値を戻さないように記述するとよい。逆に、戻り値を持つ操作は状態を変更しない（問い合わせ (query)）であることが明確になる。→ **Meyer の command-Query 分離原則**
- 操作とメソッドは次のように区別されて使用される。
    - <b>操作 (operation)</b>: オブジェクトに対して起動されるもの（手続き宣言）。
    - <b>メソッド (method)</b>: 手続きの本体。
- 操作とメソッドはポリモルフィズムを扱う場合に異なってくる。同一のスーパータイプの操作 getPrice をオーバーライドする 3 つのサブタイプがあるとすると、1 つの操作とそれを実装する 3 つのメソッドがあることになる。

### 汎化

- サブタイプとサブクラス
    - <b>サブタイプ</b>: 継承しているかどうかに関わらず、スーパータイプと置換可能であればサブタイプである。
    - <b>サブクラス</b>: サブクラス化 (subclassing) は、通常の「継承」と同義語として使用される。

### 依存関係

- 依存関係は通常はキーワードなし使用されるが、より詳細に表現したいのであれば、次のような適切なキーワードを付加するとよい。
    - <b>≪call≫</b>: ソースはターゲットの操作を呼び出す。
    - <b>≪create≫</b>: ソースはターゲットのインスタンスを生成する。
    - <b>≪derive≫</b>: ソースはターゲットから派生する。
    - <b>≪instantiate≫</b>: ソースはターゲットのインスタンスである（ソースがクラスである場合は、そのクラス自体がクラスのインスタンスになり、ターゲットクラスはメタクラスになる）。
    - <b>≪permit≫</b>: ターゲットはソースに対して、ターゲットのプライベート特性へのアクセスを許可する。
    - <b>≪realizes≫</b>: ソースはターゲットで定義されたインタフェースまたは使用の実装である。
    - <b>≪refine≫</b>: 異なる意味レベル間の関係を示す。例えば、ソースが設計クラスであり、ターゲットがそれに対応する分析クラスである場合など。
    - <b>≪substitute≫</b>: ターゲットの代わりにソースを使用できる。
    - <b>≪trace≫</b>: クラスに対する要求や、1つのモデルでの変更と他の場所での変更との関係などを追跡するのに使用する。
    - <b>≪use≫</b>: ソースはその実装においてターゲットを必要とする。
- クラス図のすべての依存関係を表示しようとすることは無駄な試みである。依存関係は頻繁に変化する。**依存関係の理解と制御には、パッケージ図が最も適している**。

### 制約規則

- UML では、制約の記述に関して制限はない。中カッコ ({}) の中に入れるということが唯一の規則である。
- 名前の後ろにコロンをつけて、`{近親結婚の禁止: 夫と妻は兄弟の関係であってはならない}` のように、成約に名前を付けることもできる。


第4章: シーケンス図
----

* シーケンス図は**相互作用図 (interaction diagram)** の中で最も一般的なもの。
* シーケンス図では、ページを縦に走る **lifeline（生存線）**で各 participant（参加者）が示され、上から下に並んだメッセージの順番によって**相互作用**が示される。

* 上に並んでいるボックスは参加者 (participant) で、UML1 ではこれらはオブジェクトだったが、UML2 ではより複雑になった。ここでは単純に参加者 (participant) と呼ぶ。UML1 では名前の下に下線が引かれていたが、**UML2 では参加者の名前に下線は引かない**。
* 参加者の名称の構文は「**名前：クラス名**」だが、名前とクラスのいずれかは省略できる。ただし、クラスだけを示す場合は「**：クラス名**」のように必ずコロンを付ける（名前なのかクラス名なのか分からなくなっちゃうので）。
* 各生存線には**アクティベーションバー**があり、相互作用の中でアクティブになる区間を表す。通常は振る舞いを明確にするために有効だが、ホワイトボードなどで議論しているときは時間の無駄なので省略して描くのが普通。
* すべての呼び出しにリターン矢印を付けるのは無駄だが、戻り値が明確になることでオブジェクト間の呼び出し関係が明らかになる場合は有用。下記の例のように記述すれば、`getProduct` の戻り値が `aProduct` であり、そのオブジェクトを使って `aProduct.getProcingDetails` を実行していることが明確になる。

{{< image src="uml-distilled-sequence1.png" code="uml-distilled-sequence1.txt" >}}

### 参加者（オブジェクト）の生成と削除

* 参加者 (participant) を生成するには、メッセージの矢印をその参加者のボックスに向かって引く。メッセージ名を示す必要はないが、`new` などを書いておけばよい。
* コンストラクタ内の処理は、参加者ボックスの下に接する形でアクティベーションボックスを描く。
* 参加者の削除は大きな「×」で表す。「×」に向かってメッセージの矢印が伸びている場合は、他のオブジェクトによって明示的に削除されることを示す。ライフライン（生存線）の最後に「×」がある場合は、GC などで自動的に削除されることを示す。

{{< image src="uml-distilled-sequence2.png" code="uml-distilled-sequence2.txt" >}}

### ループ、条件など

* シーケンス図は、オブジェクトが対話する様子を視覚化するためのものである。**制御ロジックをモデル化するためのものではないので、ループや条件付き振る舞いなどの制御構造を表すのには適していない**。制御構造を示したいときは、アクティビティ図や、コードを記述してしまうほうが優れている。
* ループと条件を表すには、**相互作用フレーム**を使用する。ループは `loop` オペランド、条件付きロジックは `alt` や `opt` のフレームで表現できる。

{{< image src="uml-distilled-sequence3.png" code="uml-distilled-sequence3.txt" >}}

### 同期的な呼び出しと非同期の呼び出し

* UML2 では同期メッセージは「頭が塗りつぶされた矢印」、非同期メッセージは「頭が塗りつぶされていない矢印」で表現する。これは見分けがつきにくいので、非同期メッセージを明確に示したいのであれば、旧式の「頭が半分しかない矢印」を使ったほうがよい。
* シーケンス図を読み取る場合、作成者が意図的に矢印を区別しているかがわかるまで、同期／非同期の判断には注意が必要。

