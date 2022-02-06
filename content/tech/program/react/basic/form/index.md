---
title: "React コンポーネントで入力フォームを作成する (1) 自力編"
url: "/p/tdpybmw"
date: "2020-07-10"
tags: ["React", "TypeScript"]
weight: 301
---

{{% private %}}
- [フォーム – React](https://ja.reactjs.org/docs/forms.html)
- [フォーム要素別の Synthetic Event の型](https://qiita.com/natsuhiko/items/5d2a526a217e05162a0a)
{{% /private %}}

制御コンポーネント (controlled components)
----

React コンポーネントでフォームを構成する場合、コンポーネントの状態 (`state`) に基づいて表示を行うように実装すると、フォームの表示内容を制御しやすくなります。

{{< image w="450" src="img-001.png" >}}

このように、コンポーネントの表示内容が、完全にその状態 (`state`) によって決まるように実装されたものを、__制御されたコンポーネント (controlled components)__ と呼びます。

このように設計することで、若干コード量は増えますが、表示内容を変更したいときはコンポーネントの `state` を変更するだけで済むようになります。
例えば、ネットワークから取得したデータをフォームに表示するような場合、そのフォームの構造を知る必要はなく、単純に `state` を更新するだけでよくなります。
これは、データとビューが分離された設計になっており、アプリ設計におけるベストプラクティスのひとつです。


input 要素の実装例
----

下記は、`<input type="text">` 要素と `<input type="submit">` 要素を持つ `MessageForm` コンポーネントの実装例です。
ユーザーがテキストを入力するたびに `handleChange()` が呼び出され、コンポーネントの状態 (`state`) が更新されます。
`setState()` の呼び出しにより `state` が変更が変更されると、再度 `render()` が実行され、表示内容が `state` の値に基づいて更新されます。

まずは、クラスコンポーネント形式での実装例。

{{< code lang="tsx" title="components/MessageForm.tsx" >}}
import * as React from 'react';

interface IState {
  msg: string;
}

// テキスト入力エリアと、submit ボタンを持つフォームを表示するコンポーネント
export class MessageForm extends React.Component<{}, IState> {
  constructor(props: {}) {
    super(props);
    // ステートの初期化（最初は入力エリアは空っぽ）
    this.state = {msg: ''};
  }

  // input 要素でのキー入力のたびに呼び出される
  private handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    // ステートを変更することで表示を更新する
    this.setState({msg: e.target.value});
  }

  // submit ボタンを押したときに呼び出される
  private handleSubmit = (e: React.FormEvent) => {
    // submit ボタンのデフォルトの振る舞い (GET や POST) を抑制する
    e.preventDefault();
    // 実際にはここでメッセージ送信を行う（内容は state から取得する）
    alert('次のメッセージが送信されました: ' + this.state.msg);
  }

  render() {
    return <form onSubmit={this.handleSubmit}>
      <label>メッセージ：
        <input type="text" value={this.state.msg} onChange={this.handleChange} />
      </label>
      <input type="submit" value="送信" />
    </form>;
  }
}
{{< /code >}}

下記は、関数コンポーネントでの実装例です。
コメントを削ったというのもありますが、やはり関数コンポーネントにするとコードがすっきりしますね。

{{< code lang="tsx" title="components/MessageForm.tsx" >}}
import * as React from 'react';

export const MessageForm: React.FC = () => {
  // ステート用のフック（テキストの初期値は空っぽ）
  const [message, setMessage] = React.useState('');

  const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    setMessage(e.target.value);
  };

  const handleSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    alert('次のメッセージが送信されました: ' + message);
  };

  return <form onSubmit={handleSubmit}>
    <label>メッセージ：
      <input type="text" value={message} onChange={handleChange} />
    </label>
    <input type="submit" value="送信" />
  </form>;
};
{{< /code >}}

上記のように作成した `MessageForm` コンポーネントは次のように使用します（といってもポンっと置いてるだけですが）。

{{< code lang="tsx" title="index.tsx（使用例）" >}}
import * as React from 'react';
import * as ReactDOM from 'react-dom';
import { MessageForm } from './components/MessageForm';

ReactDOM.render(<MessageForm />, document.getElementById('root'));
{{< /code >}}


他のフォーム要素の場合（textarea 要素など）
----

### 他の要素も同様に使える

他のフォーム要素を使う場合も、前述の例（`<input type="text">` 要素）とほぼ同様に記述することができます。
例えば、テキスト入力用に `<textarea>` 要素を使用する場合も、次のように `onChange` 属性で指定したイベントハンドラ内でステートを更新すれば OK です。

{{< code lang="tsx" >}}
const handleChange = (e: React.ChangeEvent<HTMLTextAreaElement>) => {
  setMessage(e.target.value);
};

// ...
<textarea value={message} onChange={handleChange} />
{{< /code >}}

注意点としては、__イベントハンドラのパラメータの型が微妙に変わる__ ということです（型パラメータとして `HTMLInputElement` ではなく、__`HTMLTextAreaElement`__ を使用します）。

また、通常の HTML の `textarea` 要素の場合、`<textarea>こんにちわ</textarea>` のように、開始タグと終了タグの間に値を記述しましたが、__JSX の `textarea` の値は `value` 属性で指定する__ ことに注意してください（他の `input` 要素と共通の `value` 属性で値をセットできるよう考慮されています）。

### 各種フォーム要素の実装例

#### input type="submit" 要素 / button type="submit" 要素

__`FormEvent`__ 型のイベントオブジェクトを受け取ります。

{{< code lang="tsx" >}}
const handleSubmit = (e: React.FormEvent) => {
  e.preventDefault();
  alert('メッセージ: ' + message);
};

<form onSubmit={handleSubmit}>
  ...
  <input type="submit" value="送信" />
  <button type="submit" onClick={handleSubmit}>送信</button>
</form>
{{< /code >}}

#### input type="text" 要素

__`ChangeEvent<HTMLInputElement>`__ 型のイベントオブジェクトを受け取ります。

{{< code lang="tsx" >}}
const handleChange = (event: React.ChangeEvent<HTMLInputElement>) => {
  setMessage(event.target.value);
}

<input type="text" value={message} onChange={handleChange} />
{{< /code >}}

#### input type="checkbox" 要素

__`ChangeEvent<HTMLInputElement>`__ 型のイベントオブジェクトを受け取ります（type="text" と同じ）。

{{< code lang="tsx" >}}
export const MyForm: React.FC = () => {
  const [isOpen, setIsOpen] = React.useState(false);

  const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    setIsOpen(e.target.checked);
  }

  return <form>
    <label>Is open:
      <input type="checkbox" checked={isOpen} onChange={handleChange} />
    </label>
  </form>;
};
{{< /code >}}

#### textarea 要素

__`ChangeEvent<HTMLTextAreaElement>`__ 型のイベントオブジェクトを受け取ります。

{{< code lang="tsx" >}}
const handleChange = (e: React.ChangeEvent<HTMLTextAreaElement>) => {
  setMessage(e.target.value);
}

<textarea value={message} onChange={handleChange} />
{{< /code >}}

#### select 要素

__`ChangeEvent<HTMLSelectElement>`__ 型のイベントオブジェクトを受け取ります。

{{< code lang="tsx" >}}
import * as React from 'react';

export const MyForm: React.FC = () => {
  const [color, setColor] = React.useState('red');

  const handleSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    alert('Color: ' + color);  //=> 'red'
  }

  const handleChange = (e: React.ChangeEvent<HTMLSelectElement>) => {
    setColor(e.target.value);
  }

  return <form onSubmit={handleSubmit}>
    <label>
      Pick your favorite color:
      <select value={color} onChange={handleChange}>
        <option value="red">Red</option>
        <option value="green">Green</option>
        <option value="yellow">Yellow</option>
      </select>
    </label>
    <input type="submit" value="Submit" />
  </form>;
};
{{< /code >}}

#### キーハンドル

`onKeyDown` や `onKeyPress` 属性でセットしたイベントハンドラには、__`KeyboardEvent`__ オブジェクトが渡されます。
このオブジェクトからどんな値を取得できるかは、[公式の Keyboard Events の説明](https://reactjs.org/docs/events.html#keyboard-events) を参照してください。

{{< code lang="tsx" >}}
import * as React from 'react';

export const MyForm: React.FC = () => {
  const [message, setMessage] = React.useState('');

  const handleKeyEvent = (e: React.KeyboardEvent) => {
    console.log(e);
  }

  const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    setMessage(e.target.value);
  }

  return <form>
    <input type="text" value={message}
        onKeyDown={handleKeyEvent} onChange={handleChange} />
  </form>;
};
{{< /code >}}


カスタムフックで input 要素の属性を簡単にセットする
----

フォームに複数の input 要素を配置する場合、各要素の `value` 属性と `onChange` 属性をセットするのが煩わしく感じるかもしれません。
そのようなときは、下記の `useInput` 関数のように、input 要素用の属性値（`value` と `onChange`）を生成して返す関数を作ると楽になるかもしれません。

{{< code lang="tsx" title="components/MyForm.tsx" >}}
import * as React from 'react';

// カスタムフックを定義（input 要素用の属性を生成する）
const useInput = (initValue) => {
  const [value, setValue] = React.useState(initValue);
  return {
    value,
    onChange: (e: React.ChangeEvent<HTMLInputElement>) => setValue(e.target.value)
  };
};

export const MyForm: React.FC = () => {
  const msg1 = useInput('');
  const msg2 = useInput('');

  const handleSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    alert(msg1.value + ', ' + msg2.value);
  };

  return <form onSubmit={handleSubmit}>
    <label>Message1: <input type="text" {...msg1} /></label>
    <label>Message2: <input type="text" {...msg2} /></label>
    <input type="submit" value="送信" />
  </form>;
};
{{< /code >}}

ちなみに、この `useInput` 関数のように内部でフック関数（`useState` など）を呼び出すものを、__カスタムフック__ と呼びます。
カスタムフックにも呼び出し順序などの制約が生まれる、標準のフック関数と同様に `use` で始まる名前を付けることが推奨されています。

- 参考: [独自フックの作成 – React](https://ja.reactjs.org/docs/hooks-custom.html)


React アプリでフォームを作るなら
----

React アプリにおけるフォームの扱い方を理解したら、実際には __React Hook Form (react-hook-form)__ のようなライブラリを使ってフォームを作成することをオススメします。
下手に自力でフォームを処理するよりも、簡潔で高速なコードを記述できます。

- [React Hook Form で入力フォームを作る](/p/k7it5fp)

