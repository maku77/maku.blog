---
title: "React Router でコンポーネントの表示・非表示を切り替える"
url: "/p/nfxds8n"
date: "2020-07-27"
date: "2020-08-22"
tags: ["React"]
---

React の Router 系コンポーネント (`BrowserRouter` / `HashRouter` / `MemoryRouter`) を使用すると、論理的なパス管理によって React コンポーネントの表示の On/Off を切り替えることができます。
例えば、`/users` というパスでアクセスしたときには `Users` コンポーネントを表示し、`/projects` というパスでアクセスしたときには `Projects` コンポーネントを表示する、といったことを実現できます。

{{< private >}}
- [React Router - Quick Start](https://reactrouter.com/web/guides/quick-start)
{{< /private >}}


react-router-dom のインストール
----

Router 系のコンポーネントは __`react-router-dom`__ モジュールで提供されているので、まずはこれをインストールします。
TypeScript を使用する場合は、型定義ファイルも一緒にインストールしておきます。

{{< code >}}
$ npm install --save react-router-dom
$ npm install --save-dev @types/react-router-dom
{{< /code >}}


BrowserRouter による表示切り替え
----

{{< image w="300" border="true" title="Router による表示切り替え" src="img-001.png" >}}

次の `App` コンポーネントでは、[BrowserRouter](https://reactrouter.com/web/api/BrowserRouter) コンポーネントを使って、`Page1` と `Page2` コンポーネントの表示を切り替えます。
Router 系のコンポーネントには、`Router` というエイリアス名を付けるのが慣例なのでそれに従います。
`Router` コンポーネントの下には、表示切り替え用のリンク要素として [Link](https://reactrouter.com/web/api/Link) コンポーネントを配置し、表示要素として [Route](https://reactrouter.com/web/api/Route) コンポーネントを配置します。

{{< code lang="jsx" title="components/App.tsx" >}}
import * as React from 'react'
import { BrowserRouter as Router, Link, Route } from 'react-router-dom'

// 子コンポーネントを適当に用意
const Page1: React.FC = () => <h1>Page1</h1>
const Page2: React.FC = () => <h1>Page2</h1>

// Appコンポーネント
export const App: React.FC = () => {
  return (
    <Router>
      <nav>
        [<Link to="/page1">Page1</Link>]
        [<Link to="/page2">Page2</Link>]
      </nav>
      <Route path="/page1"><Page1 /></Route>
      <Route path="/page2"><Page2 /></Route>
    </Router>
  )
}
{{< /code >}}

あとは、この `App` コンポーネントを次のように表示します。

{{< code lang="jsx" title="index.tsx" >}}
import * as React from 'react'
import * as ReactDOM from 'react-dom'
import { App } from './components/App'

ReactDOM.render(<App />, document.getElementById('root'))
{{< /code >}}

`Link` コンポーネントとして表示された Page1 や Page2 リンクをクリックすると、Page1 や Page2 コンポーネントの内容が表示されます。

{{< note title="Uncaught DOMException: Failed to execute 'pushState' on 'History'" >}}
`BrowserRouter` を動作させるには、Web サーバー上の HTML ファイルをブラウザで開いている必要があります。
ローカルの HTML ファイルを使って動作させたい場合は、`BrowserRouter` の代わりに __`HashRouter`__ や __`MemoryRouter`__ を使用すると動作します。
{{< /note >}}


component パラメータによる表示コンポーネントの指定
----

下記のサンプルコードでは、`Route` コンポーネント以下にネストする形で子コンポーネント（`Page1` や `Page2`）を指定しています。

{{< code lang="jsx" >}}
<Router>
  <Route path="/page1"><Page1 /></Route>
  <Route path="/page2"><Page2 /></Route>
</Router>
{{< /code >}}

このように子コンポーネント指定が単純な場合は、次のように `Route` の [component プロパティ](https://reactrouter.com/web/api/Link/component-reactcomponent) を使って表示するコンポーネントを指定してしまうこともできます。

{{< code lang="jsx" >}}
<Router>
  <Route path="/page1" component={Page1} />
  <Route path="/page2" component={Page2} />
</Router>
{{< /code >}}

この書き方をすると入れ子が減ってスッキリしますが、細かいパラメータ指定がやりにくかったり、実はタイプ数が増えていたりするので、無理にこの書き方をする必要はないと思います。


パスが完全一致を必須とする (exact)
----

`Route` コンポーネントは、現在のパスと `path` プロパティの値が一致した場合に子コンポーネントを表示するものですが、この一致判断は、__デフォルトでは前方一致__ により行われます。

例えば、次のような `Route` コンポーネントが配置されているとします。

{{< code lang="jsx" >}}
<Router>
  <Route path="/" component={Home} />
  <Route path="/page1" component={Page1} />
  <Route path="/page2" component={Page2} />
</Router>
{{< /code >}}

このとき、現在のパスが `/page1/xxx` だとすると、`Home` コンポーネントと `Page1` コンポーネントの両方が同時に表示されます。

| 現在のパス | 表示されるコンポーネント |
| ---- | ---- |
| / | Home |
| /page1 | Home と Page1 |
| /page1/xxx | Home と Page1 |
| /page2 | Home と Page2 |
| /page2/xxx | Home と Page2 |

パスの一致判断を、前方一致ではなく完全一致にするには、次のように __`exact`__ プロパティを指定します。

{{< code lang="jsx" >}}
<Router>
  <Route exact path="/" component={Home} />
  <Route exact path="/page1" component={Page1} />
  <Route path="/page2" component={Page2} />
</Router>
{{< /code >}}

このように記述しておくと、`Home` と `Page` コンポーネントに関しては、パスが完全一致したときのみ表示されるようになります。

| 現在のパス | 表示されるコンポーネント |
| ---- | ---- |
| / | Home |
| /page1 | Page1 |
| /page1/xxx | ─ |
| /page2 | Home と Page2 |
| /page2/xxx | Page2 |


排他的にコンポーネントを切り替えて表示する (Switch)
----

これまでの例で気付いたと思いますが、`Router` 以下に配置した `Route` コンポーネントは、排他的に切り替え表示されるものではなく、あくまで各々の `Route` コンポーネントが個別に表示の On/Off を判断するものです。
複数の `Route` コンポーネントのうち、いずれか 1 つのみを表示したい場合、__つまり、ページ切り替えをしたい場合__ は、次のように [Switch コンポーネント](https://reactrouter.com/web/api/Switch) 以下に `Route` を配置するようにします。

{{< code lang="jsx" title="components/App.tsx" >}}
import * as React from 'react'
import { BrowserRouter as Router, Route, Switch } from 'react-router-dom'

const Home: React.FC = () => <h1>Home</h1>
const Page1: React.FC = () => <h1>Page1</h1>
const Page2: React.FC = () => <h1>Page2</h1>
const NotFound: React.FC = () => <h1>NotFound</h1>

export const App: React.FC = () => {
  return (
    <Router>
      <Switch>
        <Route path="/page1"><Page1 /></Route>
        <Route path="/page2"><Page2 /></Route>
        <Route path="/"><Home /></Route>
        <Route><NotFound /></Route>
      </Switch>
    </Router>
  )
}
{{< /code >}}

`Switch` 以下に `Route` を配置すると、上から順番に見て、最初にパスが一致したもののみが表示されるようになります。
ルートパス (`/`) には必ず一致してしまうので、__`path="/"` 指定されたものは他の `Route` よりも後ろに配置しなければいけない__ ことに注意してください。
もちろん、`exact` 指定をしている場合は先頭に配置することが可能です。

{{< code lang="jsx" >}}
<Switch>
  <Route exact path="/"><Home /></Route>
  <Route path="/page1"><Page1 /></Route>
  <Route path="/page2"><Page2 /></Route>
  <Route><NotFound /></Route>
</Switch>
{{< /code >}}

上記コードの最後の `Route` コンポーネントには `path` の指定がありませんが、このように記述すると、それより前に配置した `Route` のいずれにもパスが一致しなかった場合に、必ず表示されるようになります。
つまり、パスが不正な場合の __Not Found ページ__ を表示する場合に使用できます。

| 現在のパス | 表示されるコンポーネント |
| ---- | ---- |
| / | Home |
| /page1 | Page1 |
| /page1/xxx | Page1 |
| /page2 | Page2 |
| /page2/xxx | Page2 |
| /xxx | NotFound |


リダイレクト (Redirect)
----

[Redirect](https://reactrouter.com/web/api/Redirect) コンポーネントを使用すると、ユーザーが特定のパスにアクセスしたときに、別のパスにリダイレクトすることができます。
例えば、ルートパス (`/`) にアクセスしたときに、デフォルトページとして `/page1` にリダイレクトする、といったことが可能です。

{{< code lang="jsx" >}}
// import { BrowserRouter as Router, Redirect, Route } from 'react-router-dom'

<Router>
  <Redirect exact from="/" to="/page1" />
  <Route path="/page1"><Page1 /></Route>
  <Route path="/page2"><Page2 /></Route>
</Router>
{{< /code >}}

これは、次のようにルートパス (`/`) にアクセスしたときに表示するコンポーネントを、`/page1` にアクセスした場合と同じにするのと似ていますが、同一の内容を表示するのであれば、リダイレクトを使うことをおすすめします。

{{< code lang="jsx" title="よくない例" >}}
<Router>
  <Route exact path="/" component={Page1} />
  <Route path="/page1"><Page1 /></Route>
  <Route path="/page2"><Page2 /></Route>
</Router>
{{< /code >}}

なぜなら、React 内部のパス情報が変更されないと、`NavLink` 要素の `activeClassName` や `activeStyle` 属性が動作しないなどの問題が出るからです（`NavLink` については後述）。

次のように `Switch` と組み合わせて使用すると、どのパスにも一致しない場合にルートパス（`Home` ページ）に飛ばす、といったことが可能です。

{{< code lang="jsx" >}}
// import { BrowserRouter as Router, Redirect, Route, Switch } from 'react-router-dom'

<Router>
  <Switch>
    <Route exact path="/"><Home /></Route>
    <Route path="/page1"><Page1 /></Route>
    <Route path="/page2"><Page2 /></Route>
    <Redirect to="/" />
  </Switch>
</Router>
{{< /code >}}

これは、次のようにデフォルトルートを指定した場合と振る舞いは似ていますが、現在のパスがルート (`/`) に置き換わるかどうかの違いがあります。

{{< code lang="jsx" >}}
<Router>
  <Switch>
    <Route path="/page1"><Page1 /></Route>
    <Route path="/page2"><Page2 /></Route>
    <Route><Home /></Route>
  </Switch>
</Router>
{{< /code >}}


ルート・パラメーターを使用する（ディープリンク）
----

`Route` コンポーネントの `path` プロパティには、可変となるパラメーターを指定することができます。
パラメーター部分で表示する要素の ID などを渡すことで、いわゆる __ディープリンク__ を実現することができます。
次の例では、TODO を表示するための `ToDoPage` コンポーネントのためのルーティング定義を行なっています。

{{< code lang="jsx" >}}
<Router>
  <Switch>
    <Route path="/todos/:id" component={ToDoPage} />
    <Route path="/todos" component={ToDoPage} />
  </Switch>
</Router>
{{< /code >}}

最初の `Route` の `path` には `/todos/:id` が指定されており、この __`:id`__ の部分が可変のパラメーターになります。
`Link` コンポーネントなどで現在のパスが `/todos/123` に切り替わると、`ToDoPage` コンポーネントに `123` という値が渡されます。

下記に具体的な実装例を示します。

{{< image w="400" border="true" src="img-params.png" >}}

{{< code lang="jsx" title="components/App.tsx" >}}
import * as React from 'react'
import { BrowserRouter as Router, Link, Route, Switch } from 'react-router-dom'
import { ToDoPage } from './ToDoPage'

export const App: React.FC = () => {
  return (
    <Router>
      <nav>
        <Link to="/todos">ToDo (all)</Link><br />
        <Link to="/todos/123">ToDo (123)</Link>
      </nav>
      <Switch>
        <Route path="/todos/:id" component={ToDoPage} />
        <Route path="/todos" component={ToDoPage} />
      </Switch>
    </Router>
  )
}
{{< /code >}}

関数コンポーネント (`ToDoPage`) の中では、[useParams フック](https://reactrouter.com/web/api/Hooks/useparams) を使用することで、渡されたパラメーターを取り出すことができます。

{{< code lang="jsx" title="components/ToDoPage.tsx" >}}
import * as React from 'react'
import { useParams } from 'react-router-dom'

export const ToDoPage: React.FC = () => {
  const { id } = useParams();
  return <h1>ToDo: {id || 'ALL'}</h1>
}
{{< /code >}}

パラメータが省略された場合は、`id` の値は `undefined` になるので、その場合はすべての TODO を表示する、といった分岐処理が可能です。
上記の例では、単純に ALL と表示しています。


アクティブなページのリンクをハイライトする (NavLink)
----

{{< image w="300" border="true" title="NavLink によるハイライト" src="img-002.png" >}}

React Router によるナビゲーション（現在のパスの変更）には、`Link` コンポーネントによるリンクを使いますが、この拡張コンポーネントとして、[NavLink](https://reactrouter.com/web/api/NavLink) が用意されています。
`NavLink` を使ってナビゲーションを行うと、そのパスがアクティブ（対象の Route が表示されているとき）の CSS スタイルを指定することができます。
上記の表示例では、Page1 リンク (`NavLink`) をクリックしたときに、リンクの背景色を水色に設定しています。

アクティブな `NavLink` 用の CSS スタイルを直接指定するには __`activeStyle`__ 属性を使い、CSS クラス名で指定する場合は __`activeClassName`__ 属性を使います。
`style` 属性や `className` 属性で指定した CSS スタイルは、アクティブでないリンクとアクティブなリンクの両方に反映されます。

次のサンプルコードでは、`activeStyle` を使ってアクティブなリンクの背景色を設定しています。

{{< code lang="jsx" title="components/App.tsx" >}}
import * as React from 'react'
import { BrowserRouter as Router, NavLink, Route } from 'react-router-dom'

const Page1: React.FC = () => <h1>Page1</h1>
const Page2: React.FC = () => <h1>Page2</h1>

// スタイル定義
const styles: {[key: string]: React.CSSProperties} = {
  link: {
    color: 'white',
    background: 'gray',
    marginRight: '0.2em',
    padding: '0.5em',
    textDecoration: 'none',
  },
  active: {
    background: '#5ad',
  }
}

// Appコンポーネント
export const App: React.FC = () => {
  return (
    <Router>
      <nav>
        <NavLink to="/page1" style={styles.link} activeStyle={styles.active}>
          Page1
        </NavLink>
        <NavLink to="/page2" style={styles.link} activeStyle={styles.active}>
          Page2
        </NavLink>
      </nav>
      <Route path="/page1" component={Page1} />
      <Route path="/page2" component={Page2} />
    </Router>
  )
}
{{< /code >}}

次のサンプルコードもほぼ同様ですが、スタイルの直指定ではなく、`activeClassName` で CSS クラス名を指定しています。

{{< code lang="jsx" title="components/App.tsx" >}}
import * as React from 'react'
import { BrowserRouter as Router, NavLink, Route } from 'react-router-dom'
import styles from './App.scss';  // CSS Modules

const Page1: React.FC = () => <h1>Page1</h1>
const Page2: React.FC = () => <h1>Page2</h1>

// Appコンポーネント
export const App: React.FC = () => {
  return (
    <Router>
      <nav>
        <NavLink to="/page1" className={styles.link} activeClassName={styles.active}>
          Page1
        </NavLink>
        <NavLink to="/page2" className={styles.link} activeClassName={styles.active}>
          Page2
        </NavLink>
      </nav>
      <Route path="/page1" component={Page1} />
      <Route path="/page2" component={Page2} />
    </Router>
  )
}
{{< /code >}}

