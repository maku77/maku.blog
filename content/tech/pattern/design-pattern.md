---
title: "１時間で分かる GoF デザインパターン"
url: "/p/7gmjvza"
date: "2008-05-13"
tags: ["プログラム", "読書", "デザインパターン"]
weight: 1
---

{{% amazon
    itemId="4797311126"
    title="オブジェクト指向における再利用のためのデザインパターン"
    author="エリック ガンマ, ラルフ ジョンソン, リチャード ヘルム, ジョン ブリシディース"
    publisher="ソフトバンククリエイティブ"
    imageUrl="https://images-na.ssl-images-amazon.com/images/I/61zDpaqhh7L.jpg"
%}}

{{% amazon
    itemId="4797327030"
    title="増補改訂版Java言語で学ぶデザインパターン入門"
    author="結城 浩"
    publisher="ソフトバンククリエイティブ"
    imageUrl="https://images-na.ssl-images-amazon.com/images/I/71aAui0y3bL.jpg"
%}}

デザインパターンの輪講をしたときのメモ。


生成に関するパターン (Creational Patterns)
----

### (1) Abstract Factory パターン

同種の複数のオブジェクト生成を concrete factory クラスとして種類ごとにまとめ、実際の生成は abstract factory クラスの抽象化されたインタフェース経由で行う。**一貫した種類のインスタンスを作ることができる**。

{{< code lang="java" title="実装イメージ" >}}
TvFactory tvFactory = new SonyTvFactory();  // Sony 製の TV を作るための TvFactory
...
tvFactory.CreatePanel();  // SonyPanel を生成
tvFactory.CreateTuner();  // SonyTuner を生成
...
{{< /code >}}

どのような手順で factory method を呼び出すかを定義すると、それは Builder パターンになる。


### (2) Builder パターン

オブジェクトの**生成手順を共通化**して使いまわし、異なるオブジェクトを作成。

{{< code lang="java" title="実装イメージ" >}}
Product* Director::CreateProduct(Builder *builder) {
    builder->CreateStep1();
    builder->CreateStep2();
    builder->CreateStep3();
    return builder->GetProduct();
}
{{< /code >}}

上記のメソッドに異なる Builder オブジェクトを渡せば、異なる Product が生成される。ただし、作成手順は共通化されている。


### (3) Factory Method パターン

インスタンス生成メソッドを抽象化し、サブクラスに具体的なインスタンス生成を任せる。インスタンス生成メソッド (factory method) を呼び出す部分がすべて抽象化されるのがポイント。

{{< code lang="java" title="実装イメージ" >}}
Component component = new TextComponent();  // 具象クラスが出るのはこの１回のみ
Renderer renderer = component.CreateRenderer();  // その後のインスタンス生成は抽象化される

// factory method の実装
Renderer TextComponent::CreateRenderer() {
    return new TextRenderer();  // このクラスに適したインスタンスを生成
}
{{< /code >}}

Factory Method と Abstract Factory の違いは下記のような感じ（個人的解釈あり）。

同系統のクラスをインスタンス化するための factory method を複数持つように実装すると、それは、Abstract Factory パターンになる。
Abstract Factory の場合、一貫性を保持して同種のオブジェクトを factory method で作成するようにしているのがポイント。
Factory Method パターンの場合、抽象化する対象は単一のメソッドなので、クラス名にそのパターンを使っていることを暗示するような名前をわざわざ付けるようなことはしない。
一方、Abstract Factory パターンの場合、どのような種類のインスタンスを生成するためクラスなのかを示す名前を付けるべき。というか付けないと相当分かりにくくなる。


### (4) Prototype パターン

プロトタイプ・オブジェクトをコピーして似たようなインスタンスを作る。コンストラクタでなく、オブジェクト (prototype) を元にインスタンスを生成するため、作成されるオブジェクトをカスタマイズできる。

AbstractFactory オブジェクトに Prototype オブジェクトをセットできるようにすれば、生成するオブジェクトの種類を制御できる。

{{< code lang="java" title="実装イメージ" >}}
PrototypeHoge hoge = prototypeHoge.Clone();
{{< /code >}}


### (5) Singleton パターン

省略。


構造に関するパターン (Structual Patterns)
----

### (6) Adapter パターン

既存クラスのインタフェース (Adaptee) を、クライアントが使用したいインタフェース (Target) に合うように変換する。

{{< code lang="cpp" title="実装イメージ（object adapter の場合）" >}}
class Adapter : public Target {
private:
    Adaptee *m_pAdaptee;  // インタフェースの合わない既存のクラス

public:
    // object adapter の一般的な作り方
    Adapter(Adaptee *adaptee) : m_adaptee(adaptee) {}

    // クライアントは Target::DoSomething() の形で呼び出したい
    vitual void DoSomething() {
        m_adaptee->ProprietaryMethod();
        ...
    }
}
{{< /code >}}

上記のように委譲させる実装方法 (delegation) の他に、Adapter クラスが Adaptee を private 継承するようにする class adapter という実装形態もある。


### (7) Bridge パターン

機能の階層（アプリドメインにおける機能など (Abstraction)）と、実装の階層（プラットフォームごとの実装や、データクラスの内部表現など (Implementor)）をごっちゃにするなということ。抽象化された実装のインタフェースを使って機能部分を実装することで、プラットフォームごとの実装にとらわれずにアプリの本質的な処理を実装できる。つまり、Abstraction レイヤーをいじれば（アプリなどの）機能が変わり、Implemetor レイヤーをいじっても表面的な機能は変わらないということ。

性質上、Implementor のインタフェースは Abstraction のインタフェースよりもプリミティブなものになる。機能部分と実装部分は別々の継承関係を持つので、別々にサブクラス化して拡張していける。

{{< code lang="cpp" title="実装イメージ" >}}
// 機能の階層 (Abstraction & RefinedAbstraction)
class Frame {
    void Draw() {
        Graphics *p = GetGraphics();
        p->DrawLine() で枠を描く
    }
}
class BeautifulFrame : public Frame {
    void Draw() {
        Graphics *p = GetGraphics();
        p->DrawLine() で綺麗な枠を描く
    }
}

// 内部実装の階層 (Implementor & ConcreteImplementor)
class Graphics {
    void DrawLine(Point, Point);
}
class BresenhamGraphics : public Graphics {
    void DrawLine(Point a, Point b) { Bresenham アルゴリズムによる線の描画 }
}
class EccentricGraphics : public Graphics {
    void DrawLine(Point a, Point b) { 変わったアルゴリズムによる線の描画 }
}
{{< /code >}}

階層を分けるというと複雑に感じるけど、Abstraction と Implementor 階層のクラス関係を見ると、それは単なる Strategy パターンだと気づく。だから、Bridge パターンって、役割の違うクラスを同じ階層に入れるなという単なる教訓みたいなもの。


### (8) Composite パターン

あるオブジェクトと、それを含む composite のインタフェースを統一する。

{{< code lang="cpp" title="実装イメージ" >}}
// File と Directory の共通の親クラス
class Entry {
public:
    virtual int GetSize();
};

class File : public Entry {
public:
    virtual int GetSize() { return 自分自身のサイズ; }
}

class Directory : public Entry {
public:
    virtual int GetSize() { return ディレクトリ内の合計サイズ; }
    void AddEntry(Entry *entry) { ディレクトリに追加; }
};

// Directory には File を格納できる
Directory *dir = new Directory();
dir->AddEntry(new File());
dir->AddEntry(new File());

// Directory に Directory を格納することもできる
Directory *dir2 = new Directory();
dir2->AddEntry(dir);

// Directory 内のファイルサイズ合計を取得
Print(dir2->GetSize());
{{< /code >}}


### (9) Decorator パターン

継承せずにオブジェクトを拡張する。
拡張対象のオブジェクト (Component) を delegate として持ち、メソッド呼び出しをラッピングする形で拡張する。インタフェースは変わらないので、再帰的に拡張することができる。継承ではなく、オブジェクトコンポジションによる拡張なので、動的 (at run-time) に拡張することができる。

{{< code lang="cpp" title="実装イメージ" >}}
// 拡張される側 (Component)
class Greeter {
public:
    virtual void Greet() = 0;
};

class OrdinaryGreeter : public Greeter {
public:
    virtual void Greet() { cout << "Hello. "; }
};

// 拡張する側 (Decorator)
class Decorator : public Greeter {
private:
    Greeter *m_greeter;
public:
    Decorator(Greeter *greeter) : m_greeter(greeter) {}
    virtual void Greet() { m_greeter->Greet(); }
    ...
};

class YahooDecorator : public Decorator {  // ConcreteDecorator
public:
    YahooDecorator(Greeter *greeter) : Decorator(greeter) {};
    virtual void Greet() {
        cout << "Yahoo! ";
        Decorator::Greet();
        cout << "WYYYRYYY!"
    }
};

// main
Greeter *greeter = new YahooDecorator(
                       new YahooDecorator(
                           new OrdinaryGreeter()));
greeter->Greet();  // "Yahoo! Hello. WYYYRYYY!"
{{< /code >}}

上記の例では、ConcreteDecorator は 1 種類しか存在しないので、実は Decorator 部分が ConcreteDecorator（ここでは YahooDecorator）であってもよい。


### (10) Facade パターン

複数のオブジェクトを使用して行わなければならない処理をまとめ、シンプルなインタフェースで実行できるようにする。

{{< code lang="cpp" title="実装イメージ" >}}
class Facade {
private:
    static void DoEverything() {
        // 複数オブジェクトを駆使した処理をまとめる
        ClassA a;
        a.DoSomething();
        ClassB b;
        b.DoSomething(a);
        ClassC c;
        c.DoSomething(a, b);
    }
};

// main
Facade::DoEverything();  // クライアントはシンプルなインタフェースで呼び出すだけ
{{< /code >}}


### (11) Flyweight パターン

多数のオブジェクトを作る必要があるとき、使いまわせるデータを複数のオブジェクトが共有することで効率よく内部データを表現する。共有できるデータを intrinsic state といい、これを保持するオブジェクトをテーブルに保持するなどして使いまわす。逆に、コンテキストによって変化する共有できないデータを extrinsic state といい、メソッド実行時にパラメータで渡す。どちらのアプローチもメモリ節約に貢献する。

{{< code lang="cpp" title="実装イメージ" >}}
class Character {
private:
    char m_ch;    // 共有できる部分 (intrinsic state)
public:
    Character(char ch) : m_ch(ch) {}
    Draw(Position pos) { ... }    // 各々の Position は共有できない (extrinsic state)
};

class CharacterFactory {
private:
    Character m_char[MAX_CHAR];  // intrinsic state を持つオブジェクトはテーブルに入れて共有
public:
    static *Character CreateCharacter(char c) {
        if (m_characters[c]) {
            m_characters[c] = new Character(c);
        }
        return m_characters[c];
    }
};

// main
Character *pCh1 = CharacterFactory::GetCharacter('a');
Character *pCh2 = CharacterFactory::GetCharacter('a');  // 同じオブジェクトを参照する
pCh1->Draw(Position(0, 10));  // それぞれの描画位置は共有できないので引数で渡す
pCh2->Draw(Position(0, 20));
{{< /code >}}

intrinsic なデータを持つオブジェクトも extrinsic なデータを扱ってよい。ただし、extrinsic なデータは毎回パラメータとして渡す必要がある。上の例では Character オブジェクトをテーブルに保持して共有しているが、実際に共有されているデータは Character オブジェクトの intrinsic state である `m_ch` のみ。


### (12) Proxy パターン

あるオブジェクト (RealSubject) へのアクセス方法を制御する。クライアントは RealSubject へ直接アクセスするときと同じインタフェースで Proxy にアクセスできる。
「遅延ロード (on demand)」「キャッシュ」「インスタンス共有」「アクセス先の隠蔽」「スマートポインタの実装」などの用途に適用できる。

{{< code lang="cpp" title="実装イメージ（遅延ロードを実現する場合）" >}}
// (Subject) クライアントが使用するインタフェース
class Data {
    virtual void PrintData() = 0;
};

// (RealSubject) インスタンス化にコストがかかると仮定
class LargeData : public Data {
public:
    virtual void PrintData() { ... }
    ...
};

// (Proxy) RealSubject へのアクセスを制御（必要になってから RealSubject をインスタンス化）
class DataProxy : public Data {
private:
    LargeData *m_pData;
public:
    DataProxy() : m_pData(0) {}
    virtual void PrintData() {
        if (m_pData == 0) {
            m_pData = new LargeData();
        }
        m_pData->PrintData();
    }
    ...
}

// main
Data *pData = new DataProxy();  // この段階では LargeData はインスタンス化されない
pData->PrintData();  // この段階で Proxy 内部で LargeData が生成される
{{< /code >}}


振る舞いに関するパターン (Behavioral Patterns)
----

### (13) Chain of Responsibility パターン

リクエストを伝播させ、適切なオブジェクトで処理させる。Linked list か何かでメソッド呼び出しを伝播させるように実装する。

{{< code lang="cpp" title="実装イメージ" >}}
class Handler {
private:
    Handler *m_pSuccessor;  // 次のリクエスト転送先
public:
    Handler(Handler *pSuccessor = 0) : m_pSuccessor(pSuccessor) {}
    virtual void HandleRequest(const Request *pRequest) {
        if (m_pHandler) {
            m_pHandler->HandleRequest();
        }
    }
};

class ConcreteHandler1 : public Handler {
public:
    ConcreteHandler1(Handler *pSuccessor) : Handler(pSuccessor) {}
    virtual void HandleRequest(const Request *pRequest) {
        if (pRequest->GetType() == Request::SOME_KIND_OF_REQUEST) {
            // 自分の興味のあるリクエストだったら処理して終了
            cout << "Yahoooo!" << endl;
            return;
        }
        // 次の Handler へリクエストを転送
        Handler::HandleRequest();
    }
};

class ConcreteHandler2 : public Handler {
    ... 同様 ...
};

// main
Handler *pHandler = new ConcreteHandler1(new ConcreteHandler2(new CocreteHandler3());
pHandler->HandleRequest(someKindOfRequest);
{{< /code >}}

#### メモ

- ツリー構造が既に構成されているなら、親ノードへのポインタを successor ポインタの代わりに利用できる。
- チェーンがループしないように注意しなきゃね。
- リクエストの転送部分以外の実際の処理部分だけを子クラスで実装するようにしてもよい。その場合は、リクエストを処理したかどうかを示す真偽値をハンドラメソッドの戻り値で返すようにして転送させる必要があるかどうかを親クラス側で判断する。いくつかの Windows API がこの方法を採用している。
- Q. すでに何らかのクラスを継承済みのクラスに Handler の役割を持たせたい場合は？
    - A. 既存のクラスに Handler を継承させるのではなく、チェーンを構成するための ConcreteHandler クラス群を別に作り、それに既存のクラスの処理を委譲させればよい。


### (14) Command パターン

要求の処理方法を Command オブジェクトにカプセル化し、好きなタイミングで要求を処理できるようにする。処理を発火させるオブジェクト (Invoker) は、処理方法を知らなくてもよいので、簡単に要求の種類 (Command) を増やすことができる。

処理内容は Command オブジェクト内に直接実装してもよいし、Command オブジェクトに保持させた他のオブジェクト (Receiver) に委譲してもよい。

{{< code lang="cpp" title="実装イメージ" >}}
class Command {
public:
    virtual ~Command();
    virtual void Execute() = 0;
};

class HelloCommand : public Command {
private:
    string m_name;
public:
    HelloCommand(string name) : m_name(name) {}
    virtual void Execute() { cout << "Hello " << name; }
};

class GoodByeCommand : public Command {
public:
    virtual void Execute() { cout << "Good bye"; }
};

// main
buttonA.SetCommand(new HelloCommand("Mike"));  // ボタン A が押された時の処理を設定
buttonB.SetCommand(new GoodByeCommand());  // ボタン B が押された時の処理を設定
timer.SetCommand(new GoodByeCommand(), 10);  // 10 分のタイマーが切れた時の処理を設定
{{< /code >}}

`Command::Execute()` を実行する前の状態を Command オブジェクトが保持するようにしておけば、`Command::Undo()`、`Command::Redo()` などを実装することができる。


### (15) Interpreter パターン

ある言語ルールに従った表現 (Expression) をオブジェクトの構造（構文木など）で表現し、そのオブジェクト構造をもとに言語を解釈（式の評価など）できるようにする。

{{< code lang="cpp" title="実装イメージ" >}}
class Expression {
public:
    virtual int Evaluate() = 0;
};

class Number : public Expression {
private:
    int m_num;
public:
    Number(int num) : m_num(num) {}
    virtual int Evaluate() { return m_num; }
};

class PlusExpression : public Expression {
public:
    PlusExpression(Expression *exp1, Expression *exp2) :
        m_exp1(exp1), m_exp2(exp2) {}

    // 足し算の解釈方法を定義（解釈方法は実装言語に依存した形で実装する）
    virtual int Evaluate() {
        return m_exp1.Evaluate() + m_exp2.Evaluate();
    }
};

// main
// "(100 + 200) + 300" を表現し、解釈する
Expression *pExp = new PlusExpression(
    new Expression(new Number(100), new Number(200)),
    new Number(300));
cout << pExp->Evaluate() << endl;    // 600
{{< /code >}}

生成したオブジェクト構造それ自身に意味を解釈する (interpret) 能力を持たせるところがポイント。オブジェクト構造を構成する要素 (Expression) それぞれに自分自身を解釈する機能を持たせているため、新しい表現を追加する場合は Expression を追加するだけで、自動的に言語の解釈方法が拡張されることになる（当然、パースしてこういったオブジェクト構造を作成する部分は変更しなければならないが）。


### (16) Iterator パターン

様々な集約構造 (aggregate) 内の要素を、統一されたインタフェースで順にアクセスする。

{{< code lang="cpp" title="実装イメージ" >}}
template<Item>
class ListIterator : public Iterator {  // List の要素を走査する Iterator
public:
    ListIterator(const List<Item> *pList) : m_pList(pList), m_index(0) {}
    virtual void First() { m_index = 0; }
    virtual void Next() { ++m_index; }
    virtual void bool IsDone() const { return m_index >= m_pList.Size(); }
    virtual Item CurrentItem() const { return m_pList.Item(m_index); }
private:
    const List<Item> *m_pList;
    int m_index;
};

// main
List<int> *pList = new List<int>;
pList->Add(100);
pList->Add(200);

ListIterator it(pList);
for (it->First(); !it->IsDone(); it->Next()) {
    cout << pList->CurrentItem() << endl;
}
{{< /code >}}

特定の条件を満たす要素だけを抽出するフィルタ用途の iterator を作ったり、走査の順序を変えた iterator を作ったりすることができる。
aggregate がインタフェースにより抽象化されているのなら、ひとつの iterator を様々な aggregate に対して適用できる。


### (17) Mediator パターン

複数のオブジェクト間のやり取りを、ひとつのオブジェクトに集約する。
オブジェクト間の連携方法は Mediator オブジェクトに隠蔽される。

{{< code lang="cpp" title="実装例 （ListBox、TextBox への入力を互いに反映させる Mediator）" >}}
class QuickEntryMediator : public ListBoxListener, public TextBoxListener {
public:
    QuickEntryMediator(ListBox *pListBox, TextBox *pTextBox) {
        : m_pListBox(pListBox), m_pTextBox(pTextBox) {
        m_pListBox.addListener(this);
        m_pTextBox.addListener(this);
    }
    virtual ~QuickEntryMediator() {
        m_pListBox.deleteListener(this);
        m_pTextBox.deleteListener(this);
    }

    // ListBox の選択が変更されたら、TextBox に内容を表示
    virtual void OnListBoxChanged(const Event& e) {
        m_pTextBox->SetText(m_pListBox->GetSelectedText());
    }

    // TextBox の内容が変更されたら、対応する ListBox の項目を選択
    virtual void OnTextBoxChanged(const Event& e) {
        int i = GetCorrespondingIndex(m_pTextBox.GetText());
        if (i > 0) {
            m_pListBox->Select(i);
        } else {
            m_pListBox->Unselect();
        }
    }

    int GetCorrespondingIndex(const string& text) { ... }
};
{{< /code >}}

言い換えれば、汎用性のない部分が Mediator に集約される。


### (18) Memento パターン

あるオブジェクト (Originator) の状態をオブジェクト (Memento) に保存し、後からその状態を復元できるようにする。Memento オブジェクトの内容は Originator のみが設定／参照できる。

{{< code lang="cpp" >}}
// Originator のある時点の状態（スナップショット）を保存
class Memento {
public:
    virtual ~Memento();
private:
    // Memento の内容は対象の Originator だけが設定／参照する
    friend class Originator;

    Memento();
    void GetState(State &state) { state = m_state; }
    void SetState(const State &state) { m_state = state; }
    State m_state;  // Originator の状態
};

class Originator {
public:
    Originator() { m_state.ChangeState(); }
    Mement CreateMemento();  // 現在のスナップショットを Memento オブジェクトに保存
    void SetMemento(const Memento& memento);  // 状態を Memento オブジェクトから復帰
    void ChangeState() { ... }

private:
    State m_state;
};

// main
Originator org;
org.ChangeState();
Mement mement = org.CreateMemento();  // 現在の状態を Memento オブジェクトに保存
org.ChangeState();
org.SetMemento(memento);  // 状態を復帰
{{< /code >}}


### (19) Observer パターン

オブジェクト (Subject) に変化があったときに、関連する他のオブジェクト (Observers) に自動で通知されるようにする。Subject は Observer が何を行うかを知らないでよい。

{{< code lang="cpp" title="実装イメージ" >}}
class Subject {  // Subject interface
public:
    virtual ~Subject();
    virtual void *Attach(Observer *observer) {
        m_observers.Add(observer);
    }
    virtual void *Detach(Observer *observer) {
        m_observers.Remove(observer);
    }
    void Notify() {
        ListIterator it(m_observers);
        for (it.First(); !it.IsDone(); it.Next()) {
            it.CurrentItem()->Update(this);
        }
    }
protected:
    Subject();
    List m_observers;
}

class Observer {  // Observer interface
public:
    virtual ~Object();
    virtual void Update(Subject *changedSubject) = 0;
protected:
    Observer();
};
{{< /code >}}

自動で通知といっても、どこかで `Subject#Notify()` を呼ばなきゃならないのだけど。


### (20) State パターン

オブジェクトの状態を State オブジェクトとして表現し、それぞれの状態における振る舞いを定義する。State クラスのサブクラスを定義することによって、新たな状態の追加を柔軟に行える。

{{< code lang="cpp" title="実装イメージ" >}}
class Context {
public:
    Context() : m_pState(new ConcreteState1()) {}
    virtual Context() { delete m_pState; }
    void Request() {
        // 現在の状態に応じた処理を行う。中で状態が変化することもある。
        m_pState->Handle(this);
    }

private:
    friend class State;  // State オブジェクトにだけ ChangeState() を許可
    void ChangeState(State *pState) {
        delete m_pState;
        m_pState = pState;
    }
    State *m_pState;
};

class State {
public:
    virtual Handle(Context *pContext) = 0;
};

class ConcreteState1 : public State {
public:
    virtual Handle(Context *pContext) {
        cout << "state1" << endl;
        pContext->ChangeState(new ConcreteState2());
    }
};

class ConcreteState2 : public State {
public:
    virtual Handle(Context *pContext) {
        cout << "state2" << endl;
        pContext->ChangeState(new ConcreteState1());
    }
}
{{< /code >}}


### (21) Strategy パターン

アルゴリズムをオブジェクトにカプセル化し、自由に取り替えられるようにする。多態性の基本的な考え方。委譲によりアルゴリズムを切り替える。

{{< code lang="cpp" title="実装イメージ" >}}
class TextBox {  // Strategy を利用するクラス
public:
    bool IsValid() {
        // このテキストボックスに入力された値が正しいかをチェック。
        // チェックの仕組み (Strategy) は自由に入れ換えられる。
        return m_pValidator->IsValid(GetText());
    }
    ...
private:
    Strategy *m_pValidator;
};

class Validator {  // テキストのフォーマットが正しいかを検証する Strategy
public:
    virtual bool IsValid(string text) = 0;
};

class PostalCodeValidator : public Validator {
public:
    virtual bool IsValid(string text) {
        ... text が郵便番号として正しいなら true を返す
    }
};
{{< /code >}}


### (22) Template Method パターン

メソッドの呼び出し方法（アルゴリズムの大枠）を template method として定義し、各メソッド (primitive operations) の実装をサブクラスで行えるようにする。

アルゴリズムの不変な部分、変化する部分を明確に分けることができる。

{{< code lang="cpp" title="実装イメージ" >}}
class AbstractClass {
public:
    // TemplateMethod 内でメソッドの呼び出し順序（アルゴリズム）が固定される
    void TemplateMethod() {
        PrimitiveOperation1();
        PrimitiveOperation2();
        ...
    }
protected:
    virtual void PrimitiveOperation1() = 0;
    virtual void PrimitiveOperation2() = 0;
};

class ConcreteClass : public AbstractClass {
protected:
    // TemplateMethod の中で呼ばれる各ステップの実装はサブクラスで定義できる
    virtual void PrimitiveOperation1() { ... }
    virtual void PrimitiveOperation2() { ... }
};
{{< /code >}}

abstract メソッドを定義した時点で、ほぼこのパターンになる。

Strategy パターンが委譲によってアルゴリズムの一部を変更するのに対し、Template Method パターンは継承によってアルゴリズムの一部を変更する。


### (23) Visitor パターン

ある構文をあらわしたオブジェクト構造（ツリーなど）を意味解釈するために、意味解釈するためのオブジェクト (Visitor) を各オブジェクト (Element) に巡回させ、その過程で意味を解釈していく。オブジェクト構造を解釈するためのオペレーションを、各 Element に散らばらせることなく、ひとつの Visitor オブジェクトにまとめられる。

オブジェクト構造を構成する各 Element は、巡回してきた Visitor を Accept() メソッドで処理する。ここでは単純に自分自身を Visitor に渡し、意味解釈を Visitor オブジェクトに委譲すればよい。

{{< code lang="cpp" >}}
void ElementA::Accept(Visitor& visitor) {
    visitor.VisitElementA(this);  // 意味解釈は Visitor に任せる
}
{{< /code >}}

オブジェクト構造を別の方法で意味解釈したい場合は、Visitor のサブクラスを新たに作成し、上記の例における VisitElementA() などを適切に実装するだけでよい。この時、各 Element の実装を変更する必要はない。

Interpreter パターンでは、オブジェクト構造自体に意味解釈する機能を持たせるが、さらに Visitor パターンを適用することで、いろんな方法で意味解釈できるようになる。

ConcreteVisitor を実装するのは、XML の SAX Parser を実装するのに似てるかな。

