---
title: "React Context の情報をローカルストレージに保存する"
url: "/p/bv9kv7h"
date: "2020-08-29"
tags: ["TypeScript", "React"]
weight: 401
---

何をするか？
----

ここでは、React の Context オブジェクトにセットした値をローカルストレージに保存し、次回のアプリ起動時にそこから値を復帰させる方法を説明します。
Context の使い方の基本は、次の記事を参照してください。

- [React Context で複数のコンポーネント間でデータを共有する](/p/qoz8fow)


Context の実装
----

下記の `UserContext.tsx` モジュールでは、`React.createContext()` で `UserContext` というオブジェクトを作成しています。
このオブジェクトは、`gitHubToken` という文字列データと、その値を更新するための `setGitHubToken` 関数を保持しています。
次のように実装することで、`UserContext` のデータをローカルストレージと同期させることができます。

- `gitHubToken` の初期値に __`localStorage.getItem()`__ から取得した値を設定する
- `setGitHubToken` の呼び出し時に __`localStorage.setItem()`__ で値を保存する

{{< code lang="jsx" title="UserContext.tsx" >}}
import * as React from 'react';

// UserContext が保持する値の型
export interface UserContextType {
  gitHubToken: string;
  setGitHubToken: (token: string) => void;
}

// ローカルストレージ用のキー
const KEY_GITHUB_TOKEN = 'gitHubToken';

// UserContext の生成
export const UserContext = React.createContext<UserContextType>({
  gitHubToken: localStorage.getItem(KEY_GITHUB_TOKEN),  // デフォルト値
  setGitHubToken: (token: string) => {}  // ダミーセッター
});

// UserContext にセッター関数を登録するためのコンポーネント
export const UserContextProvider: React.FC = ({children}) => {
  const context: UserContextType = React.useContext(UserContext);
  const [gitHubToken, setGitHubToken] = React.useState(context.gitHubToken);

  const newContext: UserContextType = {
    gitHubToken,
    setGitHubToken: (token: string) => {
      localStorage.setItem(KEY_GITHUB_TOKEN, token);
      setGitHubToken(token);
    }
  };

  return (
    <UserContext.Provider value={newContext}>
      {children}
    </UserContext.Provider>
  );
};
{{< /code >}}


使用例
----

上記で作成した、`UserContext` を使用するには、上位のコンポーネント (`App` コンポーネントなど）に `UserContextProvider` を配置し、その子コンポーネントとして `UserContext` の Consumer となるコンポーネントを配置します。
ここでは、`SignInAndOut` コンポーネント（後で実装）を配置しています。

{{< code lang="jsx" title="App.tsx" >}}
import * as React from 'react';
import { UserContextProvider } from './UserContext';
import { SignInAndOut } from './SignInAndOut';

export const App: React.FC = () => {
  return (
    <UserContextProvider>
      <SignInAndOut />
    </UserContextProvider>
  );
};
{{< /code >}}

`SignInAndOut` コンポーネントは `UserContext` を参照し、`Sign In` あるいは `Sign Out` のボタンを表示します。
`UserContext` に `gitHubToken` が設定されている場合は、サインイン済みと判断しています。

{{< code lang="jsx" title="SignInAndOut.tsx" >}}
import * as React from 'react';
import { UserContext, UserContextType } from './UserContext';

export const SignInAndOut: React.FC = () => {
  const ctx: UserContextType = React.useContext(UserContext);
  if (ctx.gitHubToken) {
    return <>
      <button onClick={() => ctx.setGitHubToken('')}>Sign Out</button>
      <div>gitHubToken = {ctx.gitHubToken}</div>
    </>;
  } else {
    return <>
      <button onClick={() => ctx.setGitHubToken('DUMMY')}>Sign In</button>
    </>;
  }
};
{{< /code >}}

表示結果は次のようになります。
サインイン後は、トークンの値も表示するようにしています。

{{< image border="true" w="300" src="img-001.png" >}}

`Sign In` ボタンを押すと、トークンの値がローカルストレージに保存され、ボタンが `Sign Out` に変化します。
この状態で Web ブラウザをリロードすると、ローカルストレージからトークンの値が復元されるため、最初からサインインした状態で表示されます（`Sign Out` ボタンが表示されます）。

