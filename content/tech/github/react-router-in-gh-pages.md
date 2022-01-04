---
title: "GitHub Pages で React Router を使った SPA サイトを動かす方法"
url: "/p/9u8it5f"
date: "2020-09-07"
tags: ["GitHub Pages", "React"]
---

React Router などを使った SPA (Single Page Application) な Web サイトは、GitHub Pages でそのまま動作させようとしてもうまく動きません。
ここでは、その理由と、対応方法について説明します。


GitHub Pages で SPA サイトが動作しない理由
----

たとえば、React Router を使った Web サイトは、次のような URL を使って目的のコンテンツ (`/book/123`) を表示するようルーティングします。

{{< code >}}
https://yourname.github.io/repo-name/book/123
{{< /code >}}

React Router を使ったアプリで、この URL を実際に処理するファイルは、

{{< code >}}
https://yourname.github.io/repo-name/index.html
{{< /code >}}

であり、そこに記述された JavaScript ファイル内で、URL の末尾の `/book/123` という部分をルーティング用のパス文字列として処理します。
つまり、前述の URL の `/book/123` という部分は、単なるアプリ用のデータであり、実際に存在するファイルを示しているわけではありません。

一方、GitHub Pages はそのような事情を知らないので、__`/book/123` という URL でアクセスしようとすると、`/book/123/index.html` というファイルを見つけようとして 404 エラー__ になってしまいます。

先頭ページ (`/index.html`) を開いた後で、JavaScript でルーティング（React の `Link` コンポーネントによる遷移）を行っている間はうまく動作するのですが、ページをリロードしたり、Web ブラウザのアドレスバーに URL を直接入力したりすると、やはり 404 エラーになってしまいます。
OAuth 認証を使用した Web サイトの場合は、リダイレクトによって指定された URL へのアクセスが必要になるため、これは大きな制約になります。


解決方法
----

ここでは、[こちら](https://github.com/rafgraph/spa-github-pages) で紹介されている方法を参考にして、カスタム 404 ページを利用したハックで React Router をうまく動作させるようにしてみます。

上記サイトのコードは若干複雑なので、もう少し直感的なコードで同様のことを行えるようにします。
具体的には、クエリ文字列を自力で分解／再構築しているところを、`encodeURIComponent`/`decodeURIComponent` でサクッと終わらせちゃいます。

### 仕組み

この 404 ページハックは、次のような仕組みで GitHub Pages 上で SPA アプリ (React Router) を動作可能にします。

1. `https://yourname.github.io/repo-name/book/123` といった URL でアクセスすると、カスタム `404.html` ページが呼び出される。
2. `404.html` の中の JavaScript で、`https://yourname.github.io/repo-name` にリダイレクトする。このとき、URL から `/book/123` というパラメータを抽出し、`?q=book%2F123` のように URL エンコードしたクエリパラメータとして付加する。
3. Web サイトトップの `index.html` が呼び出されるので、JavaScript を使って URL 末尾のクエリ文字列 (`?q=book%2F123`を取り出し、もとのパラメータの形 (`/book/123`) に戻す。
4. `window.history.replaceState()` で Web ブラウザのアドレスを `/book/123` にセットし、React Router を動作させる（このときサーバーアクセスは発生しません）。


実装
----

### 404.html のアップロード

GitHub Pages として公開するリポジトリのルートに、次のような __`404.html`__ ファイルをアップロードします。

{{< code lang="html" title="404.html" >}}
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>My App</title>
  <script>
    // Web サイトをサブディレクトリに配置するなら、この値を 1 にする。
    const SEGMENT_COUNT = 0;

    // 例えば、SEGMENT_COUNT = 1 の場合、
    // https://yourname.github.io/repo-name/book/123 のような URL のうち、
    // repo-name までをベースアドレスとみなし、末尾のパス (books/123)
    // をエンコードして次のような URL へリダイレクトする。
    // https://yourname.github.io/repo-name?p=book%2F123
    const loc = window.location
    const origin = loc.origin;  //=> https://yourname.github.io
    const path = loc.href.substr(origin.length + 1);  //=> repo-name/book/123
    const segments = path.split('/');  //=> [repo-name, book, 123]
    const repo = segments.slice(0, SEGMENT_COUNT).join('/');  //=> repo-name
    const param = segments.slice(SEGMENT_COUNT).join('/');  //=> book/123
    const url = origin + '/' + repo + '?p=' + encodeURIComponent(param);
    loc.replace(url);
  </script>
</head>
<body></body>
</html>
{{< /code >}}

`https://yourname.github.io/` といった URL ではなく、`https://yourname.github.io/project名/` といった、1階層下の URL で公開するページの場合は、上記の __`SEGMENT_COUNT`__ の値を __`0`__ から __`1`__ に変更しておく必要があります（リダイレクト先の URL が調整されます）。

#### （コラム）バンドルツールで 404.html を追加する

Parcel などのバンドルツールで Web サイトのビルドを行っている場合は、エントリポイントとなるページとして `404.html` を追加します。
例えば、`parcel` コマンドを使用している場合は、次のように指定すると `build/404.html` ファイルが出力されます。
`build/404.html` の内容は基本的に `src/404.html` のコピーですが、記述内容が圧縮されたファイルになっています。

{{< code >}}
$ parcel build src/index.html src/404.html -d build
{{< /code >}}

### index.html に JavaScript を追加

プロジェクトルートの __`index.html`__ の先頭の方に、次のような JavaScript コードを追加します。
このコードは、React などの JavaScript コードよりも先に実行する必要があります（Router コンポーネントがパスを処理する前に実行する）。

{{< code lang="html" title="index.html" >}}
<!DOCTYPE html>
<html lang="ja">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>My App</title>
  <script>
    // GitHub Pages での SPA アドレス解決用
    (function(){
      const query = window.location.search;
      if (query.startsWith('?p=')) {
        const route = decodeURIComponent(query.substr(3));
        window.history.replaceState(null, null, route);
      }
    })();
  </script>
  <script src="script/index.tsx" async></script>
</head>
<body>
  <div id="root"></div>
</body>
</html>
{{< /code >}}

以上のように、`404.html` を配置し、`index.html` に JavaScript コードを追加するだけで、GitHub Pages 上で SPA アプリケーションが動作するようになります。


React Router のベース URL の調整（必要に応じて）
----

React Router は、デフォルトで URL のドメインを除いた残りの部分 (例: `/repo-name/books/123`）をルーティング用のパスとして使用します。
Web サイトのルート階層に `index.html` を配置しない場合は、__`basename`__ プロパティでそのディレクトリ名を知らせておく必要があります。
次の例では、`basename` として、開発時はデフォルトの `/` を、実運用時は `/repo-name` を使用するように設定しています。

{{< code lang="jsx" title="App.tsx" >}}
import * as React from 'react';
import { BrowserRouter as Router, Redirect, Route, Switch } from 'react-router-dom';
import { Main } from './Main';
import { Top } from './Top';

const ROUTER_BASENAME =
  process.env.NODE_ENV === 'development' ? '/' : '/repo-name';

export const App: React.FC = () => {
  return (<div style={{margin: '1em'}}>
    <Router basename={ROUTER_BASENAME}>
      <Switch>
        <Route path="/" exact={true} component={Top} />
        <Route path="/main" component={Main} />
        <Redirect to="/" />
      </Switch>
    </Router>
  </div>);
};
{{< /code >}}

この設定により、`/main` というパスは、実際には `/repo-name/main` としてハンドルされるようになります。

