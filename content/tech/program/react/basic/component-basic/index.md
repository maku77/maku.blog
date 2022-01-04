---
title: "React コンポーネント実装の基本（関数コンポーネントとクラスコンポーネント）"
url: "/p/vfr3cnw"
date: "2020-07-09"
tags: ["React", "TypeScript"]
weight: 150
---

React で独自コンポーネントを作成する方法として、大きく次の 2 種類の方法があります。

- 関数コンポーネント (Function Components)
- クラスコンポーネント (Class Components)

昔は、ステートを持つコンポーネントは「クラスコンポーネント」で作成し、ステートを持たないものは「関数コンポーネント」として作成するという使い分けがありました。
現在は、関数コンポーネントでも Hook の仕組みでステートを管理することができるようになったため、関数コンポーネントの使用が推奨されています。


関数コンポーネント
----

### 関数コンポーネントの基本

下記は、固定のテキストを表示するシンプルな関数コンポーネントの定義例です。
TypeScript (`@types/react`) では、関数コンポーネントの型は __React.FunctionComponent__ インタフェースとして定義されています。
エイリアスとして __`React.FC`__ が定義されているので、こちらを使えばより短く記述できます。

{{< code lang="jsx" title="components/Hello.tsx" >}}
import * as React from 'react';

// Hello コンポーネントの定義
export const Hello: React.FC = () => {
  return <h1>Hello</h1>
};
{{< /code >}}

HTML ファイルから読み込む JS ファイルでは、`ReactDOM.render()` で上記の `Hello` コンポーネントを描画します。
次のコードを実行すると、`<div id="root">` 要素の内容が、`Hello` コンポーネントの内容に置き換えられます。

{{< code lang="jsx" title="index.tsx（使用例）" >}}
import * as React from 'react';
import * as ReactDOM from 'react-dom';
import { Hello } from './components/Hello';

ReactDOM.render(<Hello />, document.getElementById('root'));
{{< /code >}}

一見すると、1 行目の `React` モジュールのインポートは必要ないように見えますが、JSX コードが変換されると `React` を参照するコードになるので、この行を消してはいけません。

### プロパティを定義する (props)

関数のパラメータとして、表示すべき値（プロパティ: __`props`__）を受け取ることができます。
このプロパティには、HTML 要素でいうところの「属性」として指定された値が格納されています。
TypeScript を使っているのであれば、プロパティの型をちゃんと定義して、`React.FC` の型パラメータとして指定します。
次の例では、2 つのプロパティ（`name` と `age`）を受け取る関数コンポーネントを定義しています。

{{< code lang="jsx" title="components/Hello.tsx" >}}
import * as React from 'react';

// Hello コンポーネントのプロパティ
type Props = {
  name: string;
  age: number;
};

// Hello コンポーネントの定義
export const Hello: React.FC<Props> = (props) => {
  return <h1>私は{props.name}です {props.age}歳です</h1>
};
{{< /code >}}

{{< code lang="jsx" title="index.tsx（使用例）" >}}
ReactDOM.render(
  <Hello name="まく" age={14} />,
  document.getElementById('root')
);
{{< /code >}}

`私はまくです 14歳です` と表示されれば成功です。

### プロパティをオプショナルにする

あるプロパティをオプションにしたいときは、TypeScript の型定義をするときに、プロパティ名の末尾に `?` を付けます。
次の例では、`age` プロパティの指定をオプショナルにしています。

{{< code lang="jsx" title="components/hello.ts" >}}
import * as React from 'react';

// Hello コンポーネントのプロパティ
type Props = {
  name: string;
  age?: number;
};

// Hello コンポーネントの定義
export const Hello: React.FC<Props> = (props) => {
  const ageText = props.age ? `${props.age}歳` : '秘密';
  return <h1>私は{props.name}です 年齢は{ageText}です</h1>
};
{{< /code >}}

### プロパティのデフォルト値を設定する

`React.FC` の __`deafultProps`__ で、プロパティのデフォルト値を定義しておくことができます。
次の例では、`Hello` コンポーネントの `name` と `age` をオプショナルプロパティとして定義し、それぞれ指定されなかった場合のデフォルト値を設定しています。

{{< code lang="jsx" title="components/Hello.tsx" >}}
import * as React from 'react';

type Props = {
  name?: string;
  age?: number;
};

export const Hello: React.FC<Props> = (props) => {
  return <h1>私は{props.name}です {props.age}歳です</h1>
};

// プロパティのデフォルト値
Hello.defaultProps = {name: '名無し', age: 0};
{{< /code >}}

実は、`defaultProps` を使わずに、次のように分割代入の構文を使って、デフォルト値を設定してしまった方がシンプルです。

{{< code lang="jsx" >}}
export const Hello: React.FC<Props> = (props) => {
  const {name = '名無し', age = 0} = props;
  return <h1>私は{name}です {age}歳です</h1>
};
{{< /code >}}

TypeScript の Nullish coalescing operator (__`??`__) を使ってデフォルト値を設定してしまう方法もあります。
分割代入の構文に慣れていないうちはこっちのほうが直感的かもしれません。

{{< code lang="jsx" >}}
export const Hello: React.FC<Props> = (props) => {
  const name = props.name ?? '名無し';
  const age = props.age ?? 0;
  return <h1>私は{name}です {age}歳です</h1>
};
{{< /code >}}

### （おまけ）props の型定義を簡略化する

次の例では、1 つの文字列を持つ `Props` 型を定義し、その型で受け取った値を分割代入によって `name` 変数に格納しています。

{{< code lang="jsx" >}}
type Props = { name: string; };

export const Hello: React.FC<Props> = (props) => {
  const {name} = props;
  return <h1>私は{name}です</h1>
};
{{< /code >}}

これくらいシンプルな `Props` であれば、`React.FC` の型パラメータ部分にインライン記述してしまうことも可能です。
次の例では、`name` 変数への分割代入も、アロー関数のパラメータ部分で行っています。

{{< code lang="jsx" >}}
export const Hello: React.FC<{name: string}> = ({name}) => {
  return <h1>私は{name}です</h1>;
};
{{< /code >}}

オプショナルな `props` を定義して、デフォルト値を設定する場合も同様に記述できます。

{{< code lang="jsx" >}}
export const Hello: React.FC<{name?: string}> = ({name = '名無し'}) => {
  return <h1>私は{name}です</h1>;
};
{{< /code >}}


クラスコンポーネント
----

### クラスコンポーネントの基本

React コンポーネントをクラスの形で定義することもできます。
TypeScript であれば、[React.Component](https://ja.reactjs.org/docs/react-component.html) を継承し、__`render()`__ メソッドを実装します。
コンポーネントにプロパティを持たせることができるのは、関数コンポーネントの場合と同様です。

{{< code lang="jsx" title="components/Hello.tsx" >}}
import * as React from 'react';

type Props = {
  name: string;
};

export class Hello extends React.Component<Props> {
  public render() : JSX.Element {
    return <h1>私は{this.props.name}です</h1>
  }
};
{{< /code >}}

{{< code lang="jsx" title="index.tsx（使用例）" >}}
import * as React from 'react';
import * as ReactDOM from 'react-dom';
import { Hello } from './components/Hello';

ReactDOM.render(<Hello name="まく" />, document.getElementById('root'));
{{< /code >}}

注意点として、関数コンポーネントのときに関数のパラメータで受け取っていたプロパティ (`props`) は、クラスコンポーネントの場合はメンバ変数（こちらをプロパティと呼ぶと紛らわしい^^;）として参照するというところです。
つまり、`this` を付けて、__`this.props.プロパティ名`__ という形で参照する必要があります。

### プロパティのデフォルト値を設定する

クラスコンポーネントのオプショナルなプロパティにデフォルト値を設定するには、次のように static な __`defaultPorps`__ を定義します。

{{< code lang="jsx" title="components/Hello.tsx" >}}
import * as React from 'react';

type Props = {
  name?: string;
};

export class Hello extends React.Component<Props> {
  private static defaultProps: Props = {
    name: '名無し'
  }

  public render() : JSX.Element {
    return <h1>私は{this.props.name}です</h1>
  }
};
{{< /code >}}

プロパティの一部だけがオプショナルになっている場合は、`defaultProps` の型は `HelloProps` ではなく、ES5 の __`Partial`__ を使って `Partial<HelloProps>` のように定義する必要があります（こうしないと、必須プロパティの `name` が指定されていないと言うエラーになります）。
次の例では、プロパティ `age` だけをオプショナルにし、そのデフォルト値を 0 に設定しています。

{{< code lang="jsx" title="components/Hello.tsx" >}}
import * as React from 'react';

type Props = {
  name: string;
  age?: number;
};

export class Hello extends React.Component<Props> {
  private static defaultProps: Partial<Props> = {
    age: 0
  }

  public render() : JSX.Element {
    return <h1>私は{this.props.name}です {this.props.age}歳です</h1>
  }
};
{{< /code >}}

と、ここまで `defaultProps` を使う方法を説明しましたが、実は次のようにプロパティの値を見て条件分岐してしまった方が簡単です。

{{< code lang="jsx" >}}
public render() : JSX.Element {
  const name = this.props.name;
  const age = this.props.age ?? 0;
  // const {name, age = 0} = this.props;
  return <h1>私は{name}です {age}歳です</h1>
}
{{< /code >}}

### 状態を持たせる (state)

クラスコンポーネントには、状態を持たせることができ、その値を使って描画内容を動的に変更することができます。
状態の型は、`React.Component` の 2 番目の型パラメータとして指定します（1 番目はプロパティの型）。
現在の状態は、インスタンスプロパティ __`state`__ によって管理します。
`state` の値は、コンストラクタで初期化できます。

次の例では、ボタンを押すごとにカウントアップする `CountButton` コンポーネントを定義しています。

{{< image src="img-001.png" title="CountButton の表示結果" >}}

{{< code lang="jsx" title="components/CountButton.tsx" >}}
import * as React from 'react';

// CountButton コンポーネントの状態の型
type CountButtonState = {
  count: number;
};

// CountButton コンポーネントの定義
export class CountButton extends React.Component<{}, CountButtonState> {
  constructor(props: {}) {
    super(props);
    this.state = { count: 0 };  // 状態を初期化
  }

  public render(): JSX.Element {
    return (
      <div>
        <button onClick={this.handleClick}>増やす</button>
        <b>カウント = {this.state.count}</b>
      </div>
    );
  }

  // ボタンが押されたときの処理
  private handleClick = (e: React.SyntheticEvent) => {
    // デフォルト動作を抑制したい場合
    e.preventDefault();

    // コンポーネントの状態を変更する → 新しい状態で render() が実行される
    this.setState({
      count: this.state.count + 1
    });
  }
}
{{< /code >}}

{{< code lang="jsx" title="index.tsx（使用例）" >}}
import * as React from 'react';
import * as ReactDOM from 'react-dom';
import { CountButton } from './components/CountButton';

ReactDOM.render(<CountButton />, document.getElementById('root'));
{{< /code >}}

いくつか実装上のポイントがあるので、まとめておきます。

- {{< label "コンストラクタで props を受け取る：" >}} クラスコンポーネントのコンストラクタを定義するときは、パラメータとしてプロパティ (`props`) を受け取り、親クラス (`super`) に渡してやる必要があります。上記の `CountButton` コンポーネントはプロパティを持たないので、型パラメータとしては `{}` を指定していますが、その場合でもコンストラクタのパラメータでは `props` を受け取るように実装しておく必要があります。
- {{< label "render 関数の return 直後に開き括弧：" >}} `return` の直後に改行を入れると、JavaScript が自動的にセミコロンを挿入してしまうので、すぐに改行したいときは `return (` のように、開き括弧の後ろで改行する必要があります。
- {{< label "onClick と handleClick：" >}} JSX コードの中で配置したボタンのクリックイベントをハンドルするには、`onClick` という属性にイベントハンドラを指定します。イベントハンドラ名は `handleClick` のように、名前に `handle` というプレフィックスを付けるのが慣例となっています。
- {{< label "イベントハンドラはアロー関数で：" >}} `this` で呼び出し元のオブジェクトを参照できるように、`handleClick` メソッドは `function` キーワードを使わずに、アロー関数の形で定義します。
- {{< label "デフォルト動作を抑止する：" >}} `button` 要素などのクリック時のデフォルト動作（フォーム内容の post など）を抑制するには、イベントハンドラのパラメータで渡される [React.SyntheticEvent](https://ja.reactjs.org/docs/events.html) オブジェクトの `preventDefault()` メソッドを呼び出します。
- {{< label "状態の変更は setState 関数で：" >}} `state` プロパティはリードオンリーになっているので、コンポーネントの状態を変更したいときは、__`setState()`__ 関数を使用します（コンストラクタでは例外的に代入できます）。`setState()` を使って状態を変更すると、自動的に __`render()`__ メソッドが呼び出され、新しい状態 (`state`) で画面上の表示が更新されます。

