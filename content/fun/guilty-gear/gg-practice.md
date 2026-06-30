---
title: "Guilty Gear Strive 基礎練メニュー"
url: "p/73d9ami/"
date: "2026-01-14"
lastmod: "2026-04-19"
tags: ["Guilty Gear"]
weight: 1
---

<style>
  ul.practice-list {
    > li {
      border-bottom: 1px dashed #333;
    }

    details summary {
      cursor: pointer;
      &::marker {
        content: "";
      }
      &::after {
        content: "?";
        margin-left: 0.2em;
        padding: 0.2em 0.4em;
        border-radius: 1em;
        color: white;
        background: gray;
        font-size: 90%;
        vertical-align: top;
      }
    }
    details summary:hover {
      color: #f57;
      &::after {
        background: #f57;
      }
    }
    details[open] summary::after {
      content: "";
      background: none;
    }
  }

  @media print {
    .toggle-section-details { display: none; }
  }

  .toggle-section-details {
    cursor: pointer;
    padding: 0.3em 0.8em;
    border: 1px solid #888;
    border-radius: 0.4em;
    background: #444;
    color: #eee;
    font-size: 90%;
    &:hover {
      background: #f57;
      border-color: #f57;
    }
  }
</style>

Guilty Gear Strive を始めたので練習メニューを書き留めておきます。
下記は基本的な記法。

- {{< gg "623P" >}} 昇竜 / {{< gg "P > 214K" >}} キャンセル / {{< gg "[4]" >}} 後ろ溜め / {{< gg "[2]" >}} 下溜め / {{< gg "5D" >}} ニュートラルD
- {{< gg "c.S" >}} 近S (close S) / {{< gg "f.S" >}} 遠S (far S) / {{< gg "j.S" >}} ジャンプS / {{< gg "hj.S" >}} ハイジャンプS
- {{< gg "jc." >}} ジャンプキャンセル / {{< gg "hjc." >}} ハイジャンプキャンセル / {{< gg "dc." >}} ダッシュキャンセル
- {{< gg "RRC" >}} 赤ロマキャン / {{< gg "RRCC" >}} 赤ロマキャンキャン / {{< gg "66BRC" >}} 前スライド青ロマキャン
- {{< gg "dash" >}} ダッシュ / {{< gg "delay" >}} ディレイ（次の入力を遅らせる） / {{< gg "land" >}} 着地 / <s>{{< gg "WA" >}} ワイルドアサルト</s>


用語
----

詐欺飛び (Safe Jump)
: ジャンプ攻撃を着地の瞬間に出すことで、相手が無敵昇竜を出してきたときに着地＆ガードが間に合うようにすること。ただし、梅喧や闇慈の当て身は発生が 1F なので捕まってしまう。これらの相手には「そのまま着地→投げ」というパターンを混ぜる。

シミー／投げ釣り
: 投げるよーと見せかけて、ちょっと後退することで相手の投げや無敵技を空かす動き。
うまくいけばカウンターヒットからのフルコンボに繋げられる。

リーサル
: そのコンボで相手を倒しきること。倒し切れることがわかっていれば、ゲージをすべて使いきればよい。

F 式
: 立ちガード硬直中にしゃがみガード入力した場合、当たり判定だけが立ち状態のまま残ってしまう仕様を利用したガード崩し。
  ジャンプ攻撃をガードさせた直後にジャンプ攻撃をすぐにガードさせられるような状況ができれば F 式を狙える（低空 66BRC からなど）。


練習メニュー
----

### はじめの一歩（最初の一回だけ）

<button class="toggle-section-details" data-section="section-first-step">すべて展開</button>

<ul id="section-first-step" class="practice-list">
  <li><details>
    <summary>ミッションをクリアする</summary>
    <ul>
      <li>まずはひととおり Guilty Gear のシステムを覚える。結構時間がかかるので毎日少しずつ進める。</li>
      <li>ファジー系のガードは難しいので完璧にできなくても気にしない。</li>
      <li>二択ガードもとても難しいので頑張りすぎない。</li>
    </ul>
  </details></li>
  <li><details>
    <summary>メニュー画面で初期設定</summary>
    <ul>
      <li>メインメニューで「START」→「システム設定」
      <ul>
        <li>攻撃ボタン表示形式: 「プラットフォーム準拠」から「P/K/S/HS準拠」に変更
      </ul>
    </ul>
  </details></li>
  <li><details>
    <summary>トレーニングモードで初期設定</summary>
    <ul>
      <li>「トレーニング設定」→「画面表示設定」
      <ul>
        <li>入力履歴: ON
        <li>ダメージ情報: ON
        <li>フレーム情報: ON
        <li>ダミー情報: ON
      </ul>
    </ul>
  </details></li>
  <li><details>
    <summary>トレーニングモードの記録＆再生の使い方を覚える</summary>
    <ul>
      <li>基本操作（記録ボタンと再生ボタンの使い方）
      <ul>
        <li>記録ボタン（1回目）: ダミー側の操作に切り替わる
        <li>記録ボタン（2回目）: 現在のスロットに記録開始
        <li>記録ボタン（3回目）: 現在のスロットの記録終了（1P側の操作に戻る）
        <li>再生ボタン: 現在選択されているスロット記録した操作を再生
        <li>ポジションリセット: 記録せずにキャンセルしたいときに使う
      </ul>
      <li>応用操作
      <ul>
        <li>ダミー側の操作中に「再生」ボタンを押すとスロットを切り替えられる（スロット1~5、ランダム）。
        <li>各スロットにひと通りの動きを記憶したら「ランダム」にしておくと上書きの心配がなくなる。
        <li>記録済みのスロットで間違えて「記録」を始めてしまったら、「ポジションリセット」ボタンを押すと記録をキャンセルできる。
        <li>「ランダム」状態で「再生」ボタンを押すとランダム再生してくれる。ランダム比率は「トレーニング設定」→「レコーディング設定」で設定。
      </ul>
    </ul>
  </details></li>
  <li><details>
    <summary>ランクマのトレーニングモードで対戦を一時停止する裏技</summary>
    <p>ランクマッチ中のトレーニングモードでしばらく対戦せずに練習したいときは、次のように入力します。</p>
    <ol>
      <li>PAUSE メニューを開く
      <li>「マッチング一時停止」にフォーカスを合わせて「決定→キャンセル」と素早く入力
    </ol>
    <p>
      この技を使わないと、PAUSE メニューを閉じた瞬間にマッチングが再開されてしまいます（なぜ！）。
    </p>
  </details></li>
  <li><details>
    <summary>自キャラの攻撃発生速度を把握する</summary>
    <ul>
      <li>少なくとも自分で使うキャラについては、各種攻撃の発生速度を理解しておく。相手の連携攻撃に割り込むには、最速で出せる攻撃を把握しておく必要がある。
      <li>トレーニングモードで P/K/S/HS/D など各種基本攻撃の「攻撃発生」フレームを確認して、最速行動は何なのかを把握する。
      <li>(2026-04-08) GGST 2.00 アップデートにより、全キャラの最速攻撃発生フレームが <strong>5F</strong> に統一された。
    </ul>
  </details></li>
  <li><details>
    <summary>タイランコマンドを確実に入力できるようにする（レバーレスの場合）</summary>
    <ul>
      <li>→↓←→ の 4 方向の入力だけでよい（レバーレスで各ボタンを順番に押せば出る）。
      <li>入力に失敗する場合は、<b>最後の ← と → を確実に単独入力する</b>ことを意識する。
      <li>どうしても出ないときは、最後の → だけ中指と人差し指両方で押すようにすると確実に出る。
      <li>「微ダッシュ → タイラン」もできるようにしておく。
    </ul>
  </details></li>
  <li><details>
    <summary>いろんなキャラの連続技を試してみて特徴を覚える（簡易的なキャラ対策）</summary>
    <ul>
      <li>ミッションモード「★★フォレストエリア ▶︎ 連続技 必殺技キャンセル」
      <li>ミッションモード「★★★メイシップ ▶︎ 連続技 覚醒必殺技」
      <li>ミッションモード「★★★★ウェイストランド ▶︎ 連続技 ロマンキャンセル」
      <li>ミッションモード「★★★★★ヘルエリア ▶︎ 連続技 ウォールブレイク」
    </ul>
  </details></li>
  <li><details>
    <summary>リプレイの使い方を覚える</summary>
    <ul>
      <li>ランクマッチなどで相手の連携を返せなかったときは、対戦終了後にリプレイ用に保存しておいてトレモで対応の練習をする。
      <ul>
        <li>メインメニューで「START」→「システム設定」→ リプレイオートセーブ (VERSUS) を <b>OFF</b> にし、必要なものだけマニュアル保存するようにしておく。
        <li>対戦後にリプレイを保存したいときは画面に表示されたボタン（R1 ボタンなど）を押すと保存できる。
        <li>リプレイを再生したいときは、メインメニューから <b>COLLECTION → REPLAY</b> と辿り、<b>保存したリプレイ</b>を開く（わかりにくい^^;）。
      </ul>
    </ul>
  </details></li>
</ul>

### 基本練習

<button class="toggle-section-details" data-section="section-basic">すべて展開</button>

<ul id="section-basic" class="practice-list">
  <li>
    <details>
      <summary>レバーレス特有入力を試してみる</summary>
      <ul>
        <li>{{< gg "623" >}} 昇竜入力は、「右 → 左+下+右 → 左を離す」でも OK。
          慣れると速いらしいけど、ちょっと気持ち悪い。
          スト 6 だと昇竜入力が {{< gg "2" >}} 入れっぱで出せるけど、GGST では最初の {{< gg "6" >}} は単独入力が必要なので無理。
          ちなみに GGST で昇竜入力をミスらないコツは、最後の <strong>斜め下</strong> 入力を確実に入れること。
        <li>{{< gg "236236" >}} 入力は、「下 → 下+右 → 右 → 右を押したまま下を弾く」でも OK（つまり、{{< gg "23636" >}} と省略できる）。この入力方法は GGST だけでなくスト 6 でも通用する。
      </ul>
    </details>
  </li>
  <li>
    <details>
      <summary>ガード練習（トレーニングモードの CPU 相手）</summary>
      <ul>
        <li>ひたすらガードして逃げ回る練習。
        <li>起き上がりは、しゃがみ FD を張るのが基本。バックステップも織り交ぜる。
      </ul>
    </details>
  </li>
  <li>
    <details>
      <summary>6P 対空の練習（トレーニングモードの CPU 相手）</summary>
      <ul>
        <li>{{< gg "6P" >}} は直前で出しても間に合わないので、相手がジャンプ攻撃してくるのを察知できるようにする。
        <li>中距離での攻防では常に対空を意識する。特に<strong>空中ダッシュの光</strong>は見逃さない。
      </ul>
    </details>
  </li>
  <li>
    <details>
      <summary>ダッシュジャンプ／ダッシュハイジャンプの練習</summary>
      <ul>
        <li>一瞬ダッシュして前ジャンプすると遠くから奇襲できる。
          中距離からジャンプで攻めるときは常にダッシュジャンプできるように慣れておく。
          ハイジャンプにすればさらに長距離を飛べる。
          <strong>空中ダッシュと比べて空中ガードできる</strong>ので意外と使い所は多い。
        <li>レバーレスなら、
          <ul>
            <li>{{< gg "dash 6 Jump" >}} をずらし押しでダッシュジャンプ。
            <li>{{< gg "dash 2 6 Jump" >}} をずらし押しでダッシュハイジャンプ。
          </ul>
        <li>半歩だけダッシュした瞬間に前ジャンプするタイミングを覚える。
        <li>慣れたらジャンプ攻撃からのコンボを練習。
      </ul>
    </details>
  </li>
  <li>
    <details>
      <summary>しゃがみガードからのハイジャンプの練習</summary>
      <ul>
        <li>入力方法
        <ul>
          <li>① ●●◯（ガード状態） → ●◯◯ → ●●◯ → ●◯◯+J（王道の入力方法。下を離して、下上をタタンと入力）
          <li>② ●●◯（ガード状態） → ●●◯+J → ●●◯ → ●◯◯+J → ●●◯（レバーレス特有。上2連打している最中に下を離す）
        </ul>
        <li>ガード方向は入れっぱなので、ある程度距離が離れていれば比較的安全に飛べるはず。
        <li>画面端に追い詰められたとき、ハイジャンプがひとつの逃げの手段になるけど入力が難しいので慣れておく。
      </ul>
    </details>
  </li>
  <li>
    <details>
      <summary>空投げの練習</summary>
      <ul>
        <li><b>入力のコツ:</b>
        <ul>
          <li>{{< gg "6" >}} 押しっぱで {{< gg "jump D" >}} をタタッと押すと低空で投げられる（上方向には強いので低めに投げるのが理想）。
          ただし、{{< gg "jump" >}} ボタンを押したままだと {{< gg "j.D" >}} が出てしまうので注意。
          ジャンプは先行入力気味に入力すると {{< gg "j.D" >}} になるミスが出にくい。
          あとは、飛んだ瞬間に {{< gg "D" >}} 目押し入力すれば超低空で投げられる。
        </ul>
        <li><b>空投げのシチュエーション:</b>
        <ul>
          <li>起き上がりをシミーして相手がファジージャンプしたとき。
          <li>中距離からのダッシュ中に相手がジャンプしたとき。
          <li>相手と空中ダッシュがぶつかりそうなとき。
        </ul>
        <li><b>練習方法:</b>
        <ul>
          <li>起き上がりジャンプや無敵昇竜を空投げしまくる練習をするとよい（実戦では溜めダストなどを入れるけど）。
          <li>トレーニング設定 → 反撃設定 → 起き上がり後の反撃「ランダム」
          <ul>
            <li>アクション1: ジャンプ
            <li>アクション2: ヴェイパースラスト（カイの場合）
          </ul>
        </ul>
      </ul>
    </details>
  </li>
  <li>
    <details>
      <summary>壁割り後の青ロマキャン</summary>
      <ul>
        <li>壁割り後に仕切り直しになった場合に、最速で {{< gg "66BRC(前スライド青ロマキャン)" >}} を出してリーサルまで持っていく練習。
          意外と強いらしいので研究しておく価値あり。
      </ul>
    </details>
  </li>
  <li>
    <details>
      <summary>ダスト → ロマキャンからのガード崩し</summary>
      <ul>
        <li>テンションゲージが半分溜まったら自然にこの行動を行えるように指を慣れさせる。
        <li>ほぼ全キャラ共通で使えるガード崩し手段なので慣れておきたい。
        <li>この動きに慣れることにより、逆に相手に仕掛けられたときにも見切りやすくなるはず。
        <li>トレーニング設定 → 相手の状態 → ガードの基本設定:「最初だけガード」
        <li>崩し例:
        <ul>
          <li>{{< gg "dash c.S 5D RCC" >}}。起き上がりに重ねるなら c.S の硬直差が有利なこちらで。
          <li>{{< gg "dash K 5D RRC" >}}。中距離からの崩しに。下段のボタン配置を 「ダッシュ / K / D」 にしておくと、左から順番に流れるように押せる。{{< gg "D" >}} 最速で離して 20F で出すこと。ロマキャンは落ち着いて {{< gg "D" >}} が当たるのを見てから発動。
        </ul>
      </ul>
    </details>
  </li>
</ul>

### 中級練習

<button class="toggle-section-details" data-section="section-intermediate">すべて展開</button>

<ul id="section-intermediate" class="practice-list">
  <li>
    <details>
      <summary>ファジージャンプ／ファジーバクステ／ファジー投げ相殺</summary>
      <ul>
        <li>ファジー・ジャンプ
        <ul>
          <li><b>理論:</b>
            起き上がり直後の「投げ無敵時間 (5F)」を利用し、(1)一瞬しゃがみガード (2)一瞬ジャンプ (3)しゃがみガード、という入力をする。
            あくまで、しゃがみガード状態を保つのが基本で、一瞬だけジャンプを入れる。
            攻撃を重ねられた場合はジャンプが出ずに連続ガードを継続でき、投げを重ねられたときはジャンプで回避できる。
            投げられるようであればジャンプが遅すぎ、攻撃を喰らう（PUNISH する）ようであればジャンプが早すぎる（起き上がり 1F 目にジャンプ移行中になっている）。
          <li>レバーレス入力:
          <ul>
            <li>Level1: ●●◯ → ●◯◯+Jump → ●●◯
            <li>Level2: ●●◯+FD → ●◯◯+Jump → ●●◯+FD
            <br>慣れたらしゃがみガードに FD を張りながらやってみる。
            <li>Level3: ◯●◯+FD → ●●◯+FD → ●◯◯+Jump → ●●◯+FD
            <br>最初のガードを<b> FD 直ガ</b>にできるならより強いが難しい。
              起き上がり前から真下入力しておかないとハイジャンプになってしまう。
              直ガのコツは自分の起き上がりモーションに集中すること。
              バックジャンプになりやすいので、垂直ジャンプにしたいときは直ガはあきらめる。
          </ul>
          <li>トレーニング設定 → レコーディング設定 → ポジションリセット後の再生 → する
          <ul>
            <li>スロット1: {{< gg "jump 2D (down) dash 投げ" >}}（近S 連打されても投げられれば OK）
            <li>スロット2: {{< gg "jump 2D (down) dash 2K 2D" >}}（ジャンプ押しっぱで 2K をかわせなければ OK）
          </ul>
        </ul>
        <li>ファジー・バクステ
        <ul>
          <li><b>理論:</b>
            ファジージャンプのバックステップ版で、(1)一瞬しゃがみガード (2) 一瞬後ろ方向（下入力を外す）＋ダッシュボタン (3)しゃがみガード、という入力をする。
            これで攻撃ならガード、投げなら回避できる。
            投げを避けられたらカウンターからのコンボが入る。
            <b>画面端では使わない</b>。
          <li>レバーレス入力: ●●◯+FD → ●◯◯+Dash → ●●◯+FD
        </ul>
        <li>ファジー・投げ相殺
        <ul>
          <li><b>理論:</b>
            FD を張るとなぜか投げ相殺の猶予時間がなくなってしまうので、投げ相殺を狙うときは FD を張らないようにする。
            相殺できずに投げられてしまうようであれば、{{< gg "●●◯+D" >}} 入力になっている可能性がある。確実に {{< gg "●◯◯+D" >}} のみで入力すること。
          <li>レバーレス入力: ●●◯ → ●◯◯+D → ●●◯
        </ul>
      </ul>
    </details>
  </li>
  <li>
    <details>
      <summary>詐欺飛び（リバサ昇竜対策） 〜 溜めダスト</summary>
      <ul>
        <li>投げ後や覚醒での壁割り後に詐欺飛びを重ねる練習。
          無敵必殺技をガードできたら、溜めダストからのコンボ、あるいは {{< gg "c.S" >}} からのフルコン。
        <ul>
          <li>トレーニング設定 → 反撃設定 → 起き上がり後の反撃「ランダム」で下記を設定。
          <ul>
            <li>カイ:「ヴェイパースラスト」「立ちP」
            <li>ユニカ:「A.C.T.G. ブラストオフ」「立ちP」
            <li>ルーシー:「ライブワイヤー」「モノワイヤー・ブレイクアウト」「立ちP」
          </ul>
        </ul>
      </ul>
    </details>
  </li>
  <li>
    <details>
      <summary>めくりジャンプ攻撃</summary>
      <ul>
        <li>画面中央投げ → 垂直ジャンプ → 空ダ → {{< gg "j.K(めくり)" >}}
        <li>安定してめくり攻撃できるようタイミングを掴んでおく。ジャンプタイミングより、ジャンプ攻撃を出すタイミングの方が重要。
        <li>垂直ジャンプだけでなく、バックジャンプ、ハイジャンプなども試しておく。
        <li>例: ジオの場合、ジャンプ頂点近くで空ダ → 相手の腰あたりまで落ちてから {{< gg "j.K 2K 214K(セパル)" >}}
      </ul>
    </details>
  </li>
  <li>
    <details>
      <summary>遅らせ近 S 重ね</summary>
      <ul>
        <li>相手の起き上がりに、投げ間合い外まで離れて、ほんの少し遅らせて {{< gg "c.S" >}} を出す。
        <li>ファジージャンプを多用してくる相手をカウンターヒットできる（ジャンプ移行モーションを狩れる）。
        <li>近づきすぎると投げに負けるし、離れすぎると {{< gg "f.S" >}} になってしまうので、半歩だけ下がる。
      </ul>
    </details>
  </li>
  <li>
    <details>
      <summary>シミーからのカウンター</summary>
      <ul>
        <li>トレーニング設定 → 反撃設定 → 起き上がり後の反撃「ランダム」で下記を設定。
        <ol>
          <li>投げ → {{< gg "dash c.S ..." >}} で最大反撃
          <li>無敵昇竜 → {{< gg "dash c.S ..." >}} で最大反撃
          <li>ジャンプ → {{< gg "空投げ RRC ..." >}} で最大反撃
        </ol>
        <li>特にファジージャンプばかりしてくる相手は、すかさず空投げで捕まえられるようにする。
      </ul>
    </details>
  </li>
  <li>
    <details>
      <summary>低空青ロマキャン</summary>
      <ul>
        <li>ジャンプキャンセルできる {{< gg "5K" >}} や {{< gg "c.S" >}} から低空 {{< gg "66BRC" >}} でガード崩し。
        <ul>
          <li>{{< gg "dash K jc.66BRC" >}}
          <li>{{< gg "dash c.S jc.66BRC" >}}
        </ul>
        <li>{{< gg "jump" >}} はすぐに離さないと前スライドにならないので注意（{{< gg "88BRC" >}} になってしまう）。
        <li>起き上がりの {{< gg "c.S" >}} 重ねや、何もないところからいきなり出してもOK。
        <li>壁割後の仕切り直しでいきなり出すという高度な駆け引きもあり。
      </ul>
    </details>
  </li>
  <li>
    <details>
      <summary>FD 関連のテクニック</summary>
      <ul>
        <li>ジャンプめくり対策
        <ul>
          <li>FD しながら {{< gg "4P" >}} 連打 ... めくりの場合のみ {{< gg "6P" >}} が出る。
        </ul>
        <li>地上めくり対策
        <ul>
          <li>FD しながら {{< gg "4D" >}} 連打 ... 背後に回られた場合のみ投げが出る。レオなどの対策に。
        </ul>
        <li>覚醒必殺技のガード
        <ul>
          <li>通常技を重ねた瞬間に覚醒必殺技を出されたときは、ダメ元で FD を張る。
            通常技の出始め数フレーム以内なら FD キャンセルによりガードできる可能性がある。
        </ul>
        <li>空中ダッシュ FD
        <ul>
          <li>空中ダッシュ → {{< gg "6" >}} 入れながら {{< gg "K → FD" >}} をほぼ同時に一瞬入力すると、空中攻撃をキャンセルする FD が一瞬出て、空中ダッシュ後のブレーキがかからなくなる。
            結果的に着地位置が若干前になり、着地までの時間も短くなる。これですぐに着地して投げにいったりできる。
        </ul>
      </ul>
    </details>
  </li>
  <li>
    <details>
      <summary><s>バクステ青ロマキャン（2.00 で削除）</s></summary>
      <ul>
        <li>GGST 2.00 のアップデートでバックステップ中のロマキャンは廃止されたのでこれは使用不可。
        <li>（ダッシュが出ない状況で）{{< gg "dash 4 RC" >}} をタタタンと押すと、バクステから前スライドの低空青ロマキャンが出る。ダッシュが出ない状況というのは「前ダッシュ中」「しゃがみ中」「必殺技の硬直中」「ジャンプからの着地直後」「起き上がり」などなど。
        <li>理屈としては、{{< gg "dash 4" >}} のずらし入力でバクステが一瞬でているうちに、{{< gg "dash RC" >}} の効果で前スライド青ロマキャン {{< gg "66BRC" >}} が出ることになる。
          {{< gg "4 dash RC" >}} の順番で入力してしまうと、{{< gg "44BRC" >}} なってしまってうまくいかない。
      </ul>
    </details>
  </li>
</ul>

### 意識すること

- **防御ターンで暴れない** ... ゴールは「有利フレームまでガードしきる」ことや「距離をとる」こと。
- **相手の動きに集中** ... 画面全体を広く捉える。最低限相手の動きに集中＆次の展開を予想する。
- **中距離では対空を意識** ... 中距離で「空中ダッシュの光」が見えたら、{{< gg "6P" >}} を反射的に出す。
- **近距離での最速行動** ... 近距離の最速行動勝負では {{< gg "c.S" >}} を狙わず {{< gg "5P" >}} or {{< gg "2P" >}} で我慢。慣れたら {{< gg "5P" >}} 連打 → {{< gg "RCC" >}} へ繋ぐ。


<script>
(function () {
  // 各セクションに1ビットずつ割り当て: first-step=1, basic=2, intermediate=4
  const sections = [
    { id: 'section-first-step',   bit: 1 },
    { id: 'section-basic',        bit: 2 },
    { id: 'section-intermediate', bit: 4 },
  ];

  function getExpandedBits() {
    const m = location.hash.match(/[#&]e=(\d+)/);
    return m ? parseInt(m[1], 10) : 0;
  }

  function setExpandedBits(bits) {
    const url = location.pathname + location.search + (bits > 0 ? '#e=' + bits : '');
    history.replaceState(null, '', url);
  }

  function applyState(bits) {
    sections.forEach(function (s) {
      const expanded = !!(bits & s.bit);
      document.querySelectorAll('#' + s.id + ' details').forEach(function (d) {
        d.open = expanded;
      });
      const btn = document.querySelector('[data-section="' + s.id + '"]');
      if (btn) btn.textContent = expanded ? 'すべて折りたたむ' : 'すべて展開';
    });
  }

  applyState(getExpandedBits());

  document.querySelectorAll('.toggle-section-details').forEach(function (btn) {
    btn.addEventListener('click', function () {
      const sectionId = btn.getAttribute('data-section');
      const section = sections.find(function (s) { return s.id === sectionId; });
      if (!section) return;
      const bits = getExpandedBits() ^ section.bit;
      setExpandedBits(bits);
      applyState(bits);
    });
  });
})();
</script>
