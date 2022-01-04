---
title: "React コンポーネントのプロパティでハンドラメソッドを渡す"
url: "/p/e2dpycn"
date: "2020-07-18"
tags: ["React", "TypeScript"]
weight: 300
---

ここでは、次のようにプロパティ（属性）でハンドラメソッドを設定可能な React コンポーネントの実装例を示します。

{{< code lang="html" >}}
<MyButton onClick={handleClick} />
{{< /code >}}

ハンドラメソッドを設定可能なコンポーネントを実装する
----

次の `MyButton` コンポーネントは、ボタンクリック時に呼び出されるイベントハンドラを、`onClick` プロパティで設定できるようにしています。
この `MyButton` コンポーネントは、クリックするたびに内部のカウンタ (`state.count`) を +1 し、その値を指定されたイベントハンドラへ通知します。

{{< code lang="tsx" title="components/myButton.tsx" >}}
import * as React from 'react';

// MyButton のプロパティ（属性）の型
export interface MyButtonProps {
  onClick?: (count: number) => void;
}

// MyButton の状態（ステート）の型
interface MyButtonState {
  count: number;
}

// MyButton コンポーネント
export class MyButton extends React.Component<MyButtonProps, MyButtonState> {
  constructor(props: MyButtonProps) {
    super(props);
    this.state = {count: 0};
  }

  public render(): React.ReactNode {
    const {count} = this.state;
    return <div>
      <button onClick={this.handleClick}>MyButton {count}</button>
    </div>;
  }

  private handleClick = () => {
    // カウンタを +1 して再描画
    const newCount = this.state.count + 1;
    this.setState({count: newCount});

    // onClick 属性でハンドラが指定されていたら呼び出す
    this.props.onClick?.(newCount);
  }
}
{{< /code >}}

MyButton のプロパティの型を定義しているのは次の部分です。
`onClick?` とプロパティ名の末尾に __`?`__ を付けることで、属性の指定をオプショナルにしています。

{{< code lang="typescript" >}}
export interface MyButtonProps {
  onClick?: (count: number) => void;
}
{{< /code >}}

属性として設定されたハンドラメソッドは、コンポーネント内の `handleClick()` メソッドの中で呼び出しています。

{{< code lang="typescript" >}}
private handleClick = () => {
  // カウンタを +1 して再描画
  const newCount = this.state.count + 1;
  this.setState({count: newCount});

  // onClick 属性でハンドラが指定されていたら呼び出す
  this.props.onClick?.(newCount);
}
{{< /code >}}

ハンドラメソッドの指定はオプショナルなので、演算子 __`?.`__ を使って、ハンドラメソッドが指定されている場合のみ呼び出しています。


MyButton の使用例
----

{{< image w="500" border="true" src="img-001.png" >}}

下記は、上記で作成した `MyButton` の使用例です。
ボタンをクリックすると、`onClick` 属性で指定したハンドラメソッドが呼び出されます。

{{< code lang="tsx" title="index.tsx" >}}
import * as React from 'react';
import * as ReactDOM from 'react-dom';
import {MyButton} from './components/myButton';

const handleClick = (count: number) => {
  alert(`新しい値は ${count} です`);
};

ReactDOM.render(
  <MyButton onClick={handleClick} />,
  document.getElementById('root')
);
{{< /code >}}

