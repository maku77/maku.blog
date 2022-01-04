---
title: "React でモーダルダイアログを表示する (react-modal)"
url: "/p/q6fnu29"
date: "2020-08-02"
tags: ["React"]
---

[react-modal](https://github.com/reactjs/react-modal) パッケージが提供する `ReactModal` コンポーネントを使用すると、React アプリ内で簡単にモーダルダイアログを実現することができます。

{{< image w="500" src="img-001.png" >}}

上記は、`設定` ボタンを押してモーダルな設定ダイアログを開いたときの表示例です。


react-modal のインストール
----

`ReactModal` コンポーネントを使うために、`react-modal` パッケージと TypeScript の型定義ファイルをインストールします。

{{< code >}}
$ npm install --save react-modal
$ npm install --save-dev @types/react-modal
{{< /code >}}


ReactModal コンポーネントを使用する
----

### ReactModal の使い方

`ReactModal` コンポーネントは、__`isOpen`__ プロパティでダイアログの表示・非表示を制御するようになっています。

{{< code lang="typescript" >}}
// import ReactModal from 'react-modal';

<ReactModal
  isOpen={this.props.isOpen}
  onAfterOpen={this.handleOpen}
  onRequestClose={this.handleClose}
  style={this.customStyles}
  contentLabel="Settings"
>
  // ... フォームの内容などをここに記述 ...
</ReactModal>
{{< /code >}}

__`onAfterOpen`__ でオープン時、__`onRequestClose`__ でクローズ時のイベントをハンドルすることができます。
`ReactModal` は自分自身のダイアログを自動で閉じたりしないので、`onRequestClose` に設定したハンドラ内で、`isOpen` プロパティに渡す値を `false` に設定してダイアログを閉じる必要があります。
`onRequestClose` ハンドラは、`Esc` キーを押したときや、ダイアログ外の領域をクリックしたときなどに呼び出されます。

### コンポーネントの作成

下記のサンプルコードは `ReactModal` を使ったコンポーネントの作成例です。
ここでは、ユーザー名を入力可能な設定ダイアログを想定しています。
React を使ったフォームの作成方法などは次の記事を参照してください。

- 参考: [React コンポーネントでフォームの入力を処理する](/p/tdpybmw)

{{< code lang="tsx" title="components/settingsDialog.tsx" >}}
import * as React from 'react';
import ReactModal from 'react-modal';

interface Props {
  /** このダイアログを表示するなら true */
  isOpen: boolean;
  /** このダイアログを閉じるときのコールバック */
  onClose?: () => void;
}

interface State {
  username: string;
}

export class SettingsDialog extends React.Component<Props, State> {
  constructor(props: Props) {
    super(props);
    this.state = {
      username: ''
    };

    // 具体的に #root 要素などを指定した方がよい？
    ReactModal.setAppElement('body');
  }

  public render(): React.ReactNode {
    return <div>
      <ReactModal
        isOpen={this.props.isOpen}
        onAfterOpen={this.handleOpen}
        onRequestClose={this.handleClose}
        style={this.customStyles}
        contentLabel="Settings"
      >
        <form onSubmit={this.handleSubmit}>
          <label>ユーザー名
            <input type="text" autoFocus value={this.state.username}
              onChange={this.handleChangeUsername}></input>
          </label>
        </form>
      </ReactModal>
    </div>;
  }

  // フォームのサブミット時にダイアログを閉じる
  private handleSubmit = (event: React.FormEvent<HTMLFormElement>) => {
    event.preventDefault();
    this.handleClose();
  }

  private handleChangeUsername = (event: React.ChangeEvent<HTMLInputElement>) => {
    this.setState({username: event.target.value})
  }

  // ダイアログが開いたときに呼び出される
  private handleOpen = () => {
    // ここで設定情報などを読み込む
  }

  // ダイアログ領域外のクリックや、ESCキーを押したときに呼び出される
  private handleClose = () => {
    // 親コンポーネントにダイアログを閉じてもらうためのコールバック通知
    this.props.onClose?.();
  }

  // スタイルのカスタマイズ
  private customStyles: ReactModal.Styles = {
    // ダイアログ内のスタイル（中央に表示）
    content: {
      top: '50%',
      left: '50%',
      right: 'auto',
      bottom: 'auto',
      marginRight: '-50%',
      transform: 'translate(-50%, -50%)'
    },
    // 親ウィンドウのスタイル（ちょっと暗くする）
    overlay: {
      background: 'rgba(0, 0, 0, 0.2)'
    }
  }
}
{{< /code >}}

### 作成したコンポーネントを使用する

上記で作成した `SettingsDialog` コンポーネントの使用例です。
`設定` ボタンを押したときに、`SettingsDialog` の `isOpen` プロパティを `true` に設定することでダイアログを表示しています。

{{< code lang="tsx" title="components/app.tsx" >}}
import * as React from 'react';
import {SettingsDialog} from './settingsDialog';

interface State {
  isDialogOpen: boolean;
}

export class App extends React.Component<{}, State> {
  constructor(props: {}) {
    super(props);
    this.state = {
      isDialogOpen: false,
    };
  }

  public render(): React.ReactNode {
    return <div>
      <button onClick={this.openDialog}>設定</button>
      <SettingsDialog isOpen={this.state.isDialogOpen} onClose={this.closeDialog} />
    </div>;
  }

  // ダイアログを開く
  private openDialog = () => {
    this.setState({isDialogOpen: true});
  }

  // ダイアログからのコールバックでダイアログを閉じてあげる
  private closeDialog = () => {
    this.setState({isDialogOpen: false});
  }
}
{{< /code >}}


設定値の保存と読み出し
----

`ReactModal` コンポーネントの `onAfterOpen` と `onRequestClose` プロパティを設定しておくと、ダイアログを開いたときと閉じたときに任意の処理を行うことができます。

{{< code lang="tsx" >}}
<ReactModal
  isOpen={this.props.isOpen}
  onAfterOpen={this.handleOpen}
  onRequestClose={this.handleClose}
  ...>
{{< /code >}}

設定ダイアログのようなものを作るのであれば、このタイミングでフォームに入力した設定情報を保存・復旧させるとよいでしょう。
次の例では、Web ブラウザの `localStorage` に設定情報を保存しています。

{{< code lang="typescript" >}}
private handleOpen = () => {
  // 設定情報を読み込む
  const name = localStorage.getItem('username') || '';
  this.setState({username: name});
}

private handleClose = () => {
  // 設定情報を保存
  localStorage.setItem('username', this.state.username);

  // 親コンポーネントにダイアログを閉じてもらうためのコールバック通知
  this.props.onClose?.();
}
{{< /code >}}

ここでは、簡易的な説明のため `localStorage` を使用しましたが、`localStorage` はセキュアではないとされているので、ユーザーの秘密情報をそのまま保存するのは避けてください。
他の保存手段としては、Web アプリであればサーバーサイドセッションを使う方法、Electron アプリであればローカルファイルに保存する方法（[electron-store](https://github.com/sindresorhus/electron-store) など）があります。

