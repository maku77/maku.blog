---
title: "読書メモ『ソフトウェアアーキテクトが知るべき97のこと』鈴木雄介 監修"
linkTitle: "『ソフトウェアアーキテクトが知るべき97のこと』鈴木雄介 監修"
url: "/p/ejm5wd2"
date: "2009-12-18"
tags: ["読書"]
---

{{% amazon
  itemId="4873114292"
  title="ソフトウェアアーキテクトが知るべき97のこと"
  author="鈴木 雄介 (監修)"
  publisher="オライリージャパン"
  imageUrl="https://images-na.ssl-images-amazon.com/images/I/51CEO5gHPDL.jpg"
%}}

共感した部分をメモ。

* 新しくてクールなソリューションを使ってみたいという誘惑は大きい。でも、正しいものを選んだ方が、チームも顧客も幸せになれますし、いろんなストレスも減ります。すると、時間に余裕ができてきます。
* 相手の言うことをよい方にとらえ、考え方を聞くチャンスと思って話し合いに臨みましょう。そうすれば、相手からより多くのことを学べますし、相手が防衛的になって空に閉じこもってしまう危険性も減るでしょう。
* ソフトウェア・アーキテクトは、自分がリーダーでもあることを忘れがちです。しかし、健全な職場で能力を発揮するためには、リーダーとして同僚から一目置かれるようにならなければなりません。
* アーキテクチャーの策定プロセスにデベロッパーたちを参加させていれば、彼らから自発的、積極的な姿勢を引き出すことができます。
* アプリケーションのパフォーマンスとスケーラビリティを左右する最大の要因は、アプリケーションのアーキテクチャだ。
* ユーザが必要だという機能や特徴に、実は何を求めているかをたずねれば、アーキテクトは本当の問題を考えることができます。
* アーキテクトという地位に就いてからは、コミュニケーションの相手は人間になります。
* 複数の相手に自分の考えを説明するときは、いつでも立って話をしなさい。── 立ち上がると、自動的に威厳と自身が伝わるようになります。
* システムがさまざまな形でエラーを起こすことを受け入れること。
* ソフトウェア・アーキテクトはエラーを吸収し、正常な部分を保護するセーフモードを作ることができます。
* 交渉では、絶対に最初の要求を取り下げてはいけません。
* 「速い」は、要件にはなりません。── 要件を満たすかどうかを判断するための客観的な指標がないからです。── 要件として認められるのは、このような形のものです。「1500ミリ秒以内にユーザーの入力に反応しなければならない」
* 実装するチーム・メンバーを尊重し、彼らの言葉に耳を傾けましょう。彼らが設計書に手こずっているなら、彼らが正しくて設計が間違っているか、そうでないにしても曖昧だということです。
* すべての設計書は、実装しながら書き直していく必要があるのです。
* パフォーマンステストはできる限り早い段階で開始したほうがいい。──最大の理由は、どのような変更を加えたときにパフォーマンスか急降下したかがわかること。
* ソフトウェア・アーキテクトの仕事は、単に技術的な問題をクリアすることだけではなく、それら技術要件と、プロジェクトに関わる利害関係者のビジネス要件との間でバランスを取ることが求められる。
* 時間を割いて、仕事が早くできるようにシステムを作りましょう。そうすれば、流れが良くなり、独りよがりな仕事をする理由がなくなるので、最終的には開発のペースを上げることができます。モックを使い、シミュレーターを作り、依存関係を減らし、システムを小さなモジュールに分割し、その他何でもできることをしましょう。コミットエンドランを決めてやろうなどと思う理由さえなくしてしまうのです。
* アーキテクトは、企業の目的と経営の現実に基づいて技術的な決定を導いていかなければなりません。
* 汎用性に至るルートとしてもっともよいのは、個別の具体例を理解し、そのエッセンスを煮詰めていくうちに、共通の解にたどり着くというものです。推測による汎用性よりも、経験を通じた単純性の方が役に立つのです。よく似た設計案から1つを選択するときは、汎用性よりも単純性という価値基準が決め手になります。──気まぐれに柔軟性を追求すると、意図的であれ偶然であれ、単純な設計が持っている優れた性質をみすみす失うことが多いのです。
* すぐれたアーキテクトは、模範となってチームをリードしなければなりません。──チームメンバーがすることなら何でもできる必要があります。──アーキテクトがテクノロジーを理解していないようなら、チームメンバーは自分たちのアーキテクトにどうやって信頼を置いたらよいでしょうか。
* ビジネスの役に立つという目標に向かってチームを動かしていくために、アーキテクトはビジネスのことも理解していなければなりません。
* プロジェクトマネージャーは、日常的な管理を行う人です。ありふれた仕事や人員の管理のためにアーキテクトが忙殺されないように、アーキテクトを助けます。
* アーキテクチャーを設計するときに、トレードオフがどのようなものになるかを把握するためのツールがいくつかあります。ポピュラーなのは、ATAM と CBAM です。
* 選択肢 A と B のどちらを選ぶかではなく、A、B のどちらを選んでも、それほど重大な意味を持たないようにするために、どう設計するかを考えるのです。
* どれを選ぶべきかがはっきりしない場合には、判断をしないという判断をしましょう。実際的な知識に基づいて、今よりも責任を持って判断を下せるようになるまで、実質的な判断を先延ばしにするのです。
* 信頼できる人で、必要なときに苦い現実を言ってくれるような人は、とても貴重な存在になります。
* あると思わないものを探す人はいません。再利用可能なものについての情報を「プッシュ」していけば、再利用に成功する確率が上がります。──わずかな研修を施すだけで、再利用ということでは全員が同じレベルを維持しているという状態に大きく近づけることができます。
* チームを大切にしなさい。チームメンバーは、単なるリソースではありません。あなたとともに設計を進めるコラボレーターであり、あなたのセーフティネットにもなってくれる人たちです。大切にされていないと感じている人たちは、セーフティネットにはなってくれません。
* 上空300ｍからの目は、適切なレベルでの情報を与えてくれます。──クラス、メソッドレベルの指標群である CheckStyle からの出力をスプレッドシートに取り込みし、グラフ化するようなことです。InfoViz ツールキットを使えば、同じ指標をツリーマップで表示できます。また、GraphViz は、複雑な依存グラフを図示できる優れたツールです。──図表が美しければ、おそらくシステムもよいものになっている。
* 判断の時期が迫ったことに気づいたアーキテクトは、数人のデベロッパーに問題のソリューションをそれぞれ考えさせ、しばらくそれぞれその方向で作業をするように頼みます。そして、最後の瞬間が近づいたときにミーティングを実施し、それらのソリューションの長所と短所を評価します。──アーキテクトは意思決定する必要すらありません。ただ、意思決定プロセスをリードすればよいのです。──結局、もっとも安くて済むのは、複数のソリューションを試してみる方法なのです。
* ソフトウェア・アーキテクトの仕事は、ビジネスの問題、目標、要件を理解し、それらを満足させることのできる技術アーキテクチャーを作ることです。──彼らは、最高幹部やビジネスユーザーが使っているドメイン言語でコミュニケーションをとることができます。すると、このソフトウェア・アーキテクトは仕事がわかっているという強い信頼が生まれます。
* チームメートに自己裁量を与えてそれぞれの創造力とコーディング能力を発揮させることがとても大切。
* 正しい課題を選ばなければ、危険な目に遭います。
* 複雑なアーキテクチャーは、単純なアーキテクチャーよりもかなり高い確率で失敗します。プロジェクトのスコープを縮小すれば、多くの場合、アーキテクチャーも小規模になります。スコープ縮小は、アーキテクトが成功の確率を上げるためにできるもっとも効果的な戦略の1つなのです。
* 自分の判断がそれらの人々に与える影響をかならず意識するようにしてください。自分の負担が重くなっても、彼らのためになる仕事をしようと思うようになるはずです。
* 優れたシステム仕様は、応答時間そのものだけでなく、作業時間も規定します。
* アーキテクチャー／デザインパターンは、基本的に、エンタープライズ・アーキテクチャーパターン、アプリケーション・アーキテクチャーパターン、インテグレーションパターン、デザインパターンの4種類に分類できます。
* アンチパターンについても学んでおくことも大切です。http://en.wikipedia.org/wiki/Anti-pattern
* まず何よりも状況について考え、そこからもっともシンプルなソリューションにたどり着くようにすべき。
* それぞれのタイプの中でもっとも優秀な人々だけを集めたとしても、そのチームには、問題へのアプローチのしかたが1種類しかありませんので、できることは大きく制約されます。
* むしろ、アーキテクトの役割は、さまざまな利害関係を持つ人々の間の対立を調停すること。
* 誰かが何かをしなければ重複は消えません。その誰かとはあなたなのです。
* 優れたアーキテクトは、プロジェクト、チーム、キャリアを進歩させるような新しいアイデア、ツール、デザインに対して開かれた心を持っています。時間の大半を管理会議に費やしたり、逆にコーディングばかりしていてはいけません。優れたアイデアは認め、アイデアが育つ雰囲気を作るのです。
* 開発関係者は、別のソリューションではなくそのソリューションが選ばれた理由や、どのようなトレードオフが含まれているかを明確に理解していなければなりません。──ドキュメントは、「決定の内容は何か」、「なぜそのような決定をしたか」という基本的な問いに答えるものでなければなりません。
* 自分のアイデアや信念が議論に耐えられないようなものなら、そんなものを基礎としてシステムを作るより、今それは誤りだと気付くほうがよほどよい。
* 早い段階で保守リーダーの参加を求める。保守リーダーを開発チームの中心メンバーに据える。アプリケーションのサポート計画に保守リーダーを加える。
* 抽象的に見ると、すべてのアルゴリズムは、データのあるバージョンを別のバージョンに変換する作業です。プログラム全体は、データをさまざまなリビジョンに変化させていく変換処理を集めたものにすぎません。──データ指向の見方を取れば、複雑なシステムでも、相手にできる程度の細部をまとめたものに圧縮できます。
* 解決策が1つしか見つからないことはまれです。おそらく、アーキテクトがそのドメインでは経験不足なのでしょう。自分を抑えて経験のある他の人に助けを求めたほうがよいです。
* 設計や実装を完璧なものにしたいという誘惑に負けてはなりません。「十分よい」レベルを狙い、そこまで達したら立ち止まるのです。──アーキテクチャーと設計にずれがなく、実装は正しく動作してパフォーマンス要件を満たし、コードが簡潔明快でドキュメント化されていれば十分よいと言えます。
* 意見の不一致を受け入れて仕事を進めることを覚えましょう。
* 失敗したプロジェクトを振り返ってみると、失敗したのは、能力がないからではなく、勤勉さと危機意識が欠けていたからだと分かります。
* アジャイルであろがなかろうが、自分の判断プロセスが完全に把握できていなければなりません。次の2つの条件が満たされるまで、アーキテクチャー上の決定を下したといってはなりません。──決定事項をきちんと文書化する。──影響を受ける人に決定事項を伝える。
* 未来のないテクノロジーのために、プロジェクトを危険にさらさないように注意すべきです。
* システムは設計した通りにはならない、と理解しなければなりません。
* 複数のフレームワークを導入する場合は、互いに重複がなく、何かに特化したフレームワークを選ぶこと。
* 最後に参加したプロジェクトを最初からやり直すことができるとすれば、何を変えたいのかを聞けば、教訓を応用する能力があるかどうかが推測できます。
* 今のニーズにもっとも合うソリューションを選びましょう。それ以外のものは、未来の選択として誤っているだけでなく、今の選択としても誤っているのです。
* 最初から大規模なシステムを設計することにはほとんどメリットはありません。──大きな設計ではなく、大きなビジョンを持つのです。
* 計算機を扱うにあたって、もっと本質的な知識とは？アルゴリズムとデータ構造、情報理論、計算機の動作原理、情報検索、情報圧縮…などなど。これらの知識は、ウェブでは手に入りにくい。──情報源は書籍（特に大学の教科書）や学術論文にあります。
* 開発中のドキュメントと保守ドキュメントを明確に区別することが重要となります。

