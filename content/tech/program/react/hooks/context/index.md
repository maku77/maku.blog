---
title: "React Context で複数のコンポーネント間でデータを共有する"
url: "/p/qoz8fow"
date: "2020-08-27"
tags: ["TypeScript", "React"]
weight: 400
---

{{% private %}}
- [React - Updating Context from a Nested Component](https://reactjs.org/docs/context.html#updating-context-from-a-nested-component)
{{% /private %}}

Context とは
----

React の [Context（コンテクスト）](https://ja.reactjs.org/docs/context.html) は、いわゆるグローバル変数の格納領域のようなもので、複数のコンポーネント間でのデータ共有に使用できます。
各コンポーネントから Context 情報にダイレクトにアクセスできるため、prop のように上位のコンポーネントから情報を伝搬させていく必要がありません。

Context をむやみに使うと、コンポーネントの再利用性が下がってしまいますが、アプリケーション全体で使用する次のような情報を Context で管理するとコードがすっきりします。

* ログイン中のユーザー情報（認証情報）
* 言語設定
* テーマ設定


Context の使い方
----

### Context を作成する (createContext)

Context オブジェクトは、__`React.createContext()`__ で作成することができます。
複数のコンポーネントから参照することになるので、Context オブジェクトは単独のファイルとしてモジュール化しておきます。

`React.createContext()` の引数に任意のオブジェクトを渡すと、その値を初期値とする `Context` オブジェクトが生成されます。
次の例では、`string` 型のデータを保持する `Context` オブジェクトを生成しています。


{{< code lang="typescript" title="MyContext.ts" >}}
import * as React from 'react';

export const MyContext: React.Context<string> =
  React.createContext<string>('Default message');
{{< /code >}}

### Context を参照する (useContext)

関数コンポーネント内で、Context が保持するデータを参照するには、__`React.useContext()`__ を使用します。
デフォルトでは、`React.createContext()` の引数に設定したデフォルト値が返されます。
上記の例では、`Default message` というデフォルトテキストを設定しているので、その値が返されることになります。

{{< code lang="jsx" title="App.tsx" >}}
import * as React from 'react';
import { MyContext } from './MyContext';

export const App: React.FC = () => {
  const message: string = React.useContext(MyContext);  //=> "Default message"
  return (
    <div>{message}</div>
  );
};
{{< /code >}}

このように、どの階層にあるコンポーネントからも、`React.useContext()` を使って Context が保持する情報を参照することができます。
しかし、このままだと、Context が保持する値を変更することができません。


### Context の値を更新してコンポーネントを再描画する

前述の `React.createContext()` で作成した `MyContext` には、__`MyContext.Provider`__ というコンポーネントが付随しています。
このコンポーネントは、次のように __`value`__ 属性と一緒に使用します。

{{< code lang="xml" >}}
<MyContext.Provider value={データ}>
  子コンポーネント
</MyContext.Provider>
{{< /code >}}

このようにすると、子コンポーネント内で `React.useContext()` を呼び出したときに返される値が、上記の `value` 属性で指定した値に変化します。
つまり、この `value` 属性の値を、ステートオブジェクトを使って設定することで、子コンポーネントを新しい Context データで再描画できるようになります。

次の例では、`MyContext.Provider` の `value` 属性に、`React.useState()` で作成した `message` ステートオブジェクトを設定しています。
`message` ステートオブジェクトの初期値は、`React.useContext()` が返すデフォルト値 (`Default message`) に設定しています。
この `message` ステートオブジェクトの値を `setMessage` 関数で変更することにより、`MyContext.Provider` 以下のコンポーネントに再描画がかかります。

{{< code lang="jsx" title="App.tsx" >}}
import * as React from 'react';
import { MyContext } from './MyContext';
import { Child } from './Child';

export const App: React.FC = () => {
  const [message, setMessage] = React.useState(React.useContext(MyContext));

  return (
    <MyContext.Provider value={message}>
      <Child /><br />
      <button onClick={()=>setMessage('New message')}>Change</button>
    </MyContext.Provider>
  );
};
{{< /code >}}

`Child` コンポーネントは、次のように `MyContext` の内容を表示しているだけです。

{{< code lang="jsx" title="Child.tsx" >}}
import * as React from 'react';
import { MyContext } from './MyContext';

export const Child: React.FC = () => {
  const message = React.useContext(MyContext);
  return <b>{message}</b>;
};
{{< /code >}}

{{< image w="500" border="true" src="img-001.png" >}}

Change ボタンを押すと、`Child` コンポーネントに表示されるメッセージが `Default message` から `New message` に変化します。


どのコンポーネントからでも Context の値を更新できるようにする
----

ここまで見てきたように、`Context` データの変化を子コンポーネントに伝えるには、`Context.Provider` コンポーネントの `value` 属性の値をうまく更新してやる必要があります。
このような更新処理は、`Context.Provider` を配置しているコンポーネント内では容易に行えますが（前述の例）、__任意の階層のコンポーネントから `Context` データを更新できるようにする__ には少々工夫が必要です。
ここでは、`Context` データとして、自身のデータを更新するためのセッター関数を持つようにする例を示します。

下記は、作成するアプリの表示例です。

{{< image border="true" src="img-002.png" >}}

`UserInfo` コンポーネントは `Context` データの内容を表示し、`UpdateButtons` コンポーネントは `Context` データの内容を変更するボタンを表示します。
例えば、`Change username` ボタンを押すと、`Context` が保持する `username` の値が変化し、画面上の表示も更新されます。
`UserInfo` と `UpdateButtons` は prop を介した親子構造により連携しているわけではなく、`Context` によってのみ連携しています。

### AppContext コンポーネントの作成

ここで定義する `AppContext` は、アプリ全体で共有する 2 つの文字列データ（`username` と `apiToken`）を持ち、さらにそれらの値を更新するためのセッター関数 (`setUsername` と `setApiToken`) を持ちます。

{{< code lang="jsx" title="AppContext.tsx" >}}
import * as React from 'react';

// AppContext が保持する値の型
export interface AppContextType {
  username: string;
  apiToken: string;
  setUsername: (username: string) => void;
  setApiToken: (apiToken: string) => void;
}

// AppContext の生成
export const AppContext = React.createContext<AppContextType>({
  username: 'Default username',  // デフォルト値
  apiToken: 'Default apiToken',  // デフォルト値
  setUsername: (username: string) => {},  // ダミー関数
  setApiToken: (apiToken: string) => {},  // ダミー関数
});

// AppContext にセッター関数を登録するためのコンポーネント
export const AppContextProvider: React.FC = ({children}) => {
  // デフォルト値の取得用
  const context: AppContextType = React.useContext(AppContext);

  // ステートオブジェクト作成
  const [username, setUsername] = React.useState(context.username);
  const [apiToken, setApiToken] = React.useState(context.apiToken);

  // 下位コンポーネントへ渡す Context
  const newContext: AppContextType = {
    username, setUsername, apiToken, setApiToken
  };

  return (
    <AppContext.Provider value={newContext}>
      {children}
    </AppContext.Provider>
  );
};
{{< /code >}}

ここでのポイントは、任意の子コンポーネントを `AppContext.Provider` 以下に配置するための __`AppContextProvider`__ を定義しているところです。
ここで、`React.useState()` で作成したステートオブジェクトを使って `AppContext` データを再構成することで、各種セッター関数が正しく初期化されます。
子コンポーネント (`{children}`) からセッター関数を呼ぶことにより、`AppContextProvider` コンポーネントの再描画が走るため、結果的に、関連するすべての子コンポーネントが再描画されることになります。

### UserInfo コンポーネントの作成

`UserInfo` コンポーネントは、`AppContext` が保持するユーザーデータを単純に表示します。
ここでは `React.useContext()` をそのまま使っていますが、`useAppContext()` のようなカスタムフックを作成すれば、よりスッキリしたコードになります。

{{< code lang="jsx" title="UserInfo.tsx" >}}
import * as React from 'react';
import { AppContext, AppContextType } from './AppContext';

export const UserInfo: React.FC = () => {
  // AppContext.Provider から提供される Context データを参照
  const context: AppContextType = React.useContext(AppContext);

  return <>
    <h2>UserInfo</h2>
    <ul>
      <li><b>username:</b> {context.username}</li>
      <li><b>apiToken:</b> {context.apiToken}</li>
    </ul>
  </>;
};
{{< /code >}}

### UpdateButtons コンポーネントの作成

`UpdateButtons` コンポーネントは、`AppContext` が保持するデータを変更するためのボタン（`Change username` と `Change apiToken`）を配置します。
`AppContext` の内容を更新したいときは、`AppContext` 自身が提供するセッター関数を呼び出すだけで済みます。

{{< code lang="jsx" title="UpdateButtons.tsx" >}}
import * as React from 'react';
import { AppContext, AppContextType } from './AppContext';

export const UpdateButtons: React.FC = () => {
  // AppContext.Provider から提供される Context データを参照
  const context: AppContextType = React.useContext(AppContext);

  const handleChangeUsername = () => {
    context.setUsername('New username');
  };
  const handleChangeApiToken = () => {
    context.setApiToken('New apiToken');
  };

  return <>
    <h2>UpdateButtons</h2>
    <button onClick={handleChangeUsername}>Change username</button>
    <button onClick={handleChangeApiToken}>Change apiToken</button>
  </>;
};
{{< /code >}}

### App コンポーネントの作成

最後に、最上位のコンポーネントである `App` コンポーネントです。
`AppContextProvider` の子コンポーネントとして配置された `UserInfo` や `UpdateButtons` からは、`AppContext` が保持するデータを参照・変更することができます。

{{< code lang="jsx" title="App.tsx" >}}
import * as React from 'react';
import { AppContextProvider } from './AppContext';
import { UserInfo } from './UserInfo';
import { UpdateButtons } from './UpdateButtons';

export const App: React.FC = () => {
  return (
    <AppContextProvider>
      <UserInfo />
      <UpdateButtons />
    </AppContextProvider>
  );
};
{{< /code >}}

