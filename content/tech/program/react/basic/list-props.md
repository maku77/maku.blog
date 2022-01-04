---
title: "React コンポーネントのプロパティで配列データを渡す"
url: "/p/av9mxak"
date: "2020-07-09"
tags: ["React", "TypeScript"]
weight: "300"
---

React コンポーネントのプロパティには、文字列や数値などの単純なスカラ値だけではなく、配列などの複雑なオブジェクトを渡すことができます。
使用イメージは次のような感じです。

{{< code lang="html" >}}
<MyComponent values={配列変数} />
{{< /code >}}

配列型のプロパティを扱うコンポーネントを作成する
----

次の `Books` コンポーネントは、プロパティ `titles` で文字列配列を受け取り、それぞれの値を `li` 要素で描画します。
配列の __`map`__ メソッドを使用すると、複数の `li` 要素を簡単に生成することができます。

{{< code lang="tsx" title="components/books.tsx（Books コンポーネント）" >}}
import * as React from 'react';

// Books コンポーネントのプロパティの型
export interface BooksProps {
  titles: string[]
}

// Books コンポーネントの定義
export const Books: React.FC<BooksProps> = (props) => {
  // titles プロパティの要素数が 0 であれば何も描画しない
  if (props.titles.length == 0) return null;

  // titles プロパティの値を使って、複数の li 要素を作る
  const listItems = props.titles.map((title: string) =>
    <li>{title}</li>
  );

  // ul 要素の描画
  return <ul>{listItems}</ul>;
};
{{< /code >}}

この `Books` コンポーネントは次のような感じで使用します。

{{< code lang="tsx" title="index.tsx（使用例）" >}}
import * as React from 'react';
import * as ReactDOM from 'react-dom';
import { Books } from './components/books';

const titles = ['タイトル1', 'タイトル2', 'タイトル3'];
ReactDOM.render(
  <Books titles={titles} />,
  document.getElementById('root')
);
{{< /code >}}

これで、次のような HTML 要素が生成されます。

{{< code lang="html" >}}
<ul>
  <li>タイトル1</li>
  <li>タイトル2</li>
  <li>タイトル3</li>
</ul>
{{< /code >}}


リスト要素に key を与える
----

上記のコードを実行すると、React は次のような警告を出力します。

{{< code >}}
Warning: Each child in a list should have a unique "key" prop.
{{< /code >}}

React は [配列要素の再描画処理を効率的に行うため](https://ja.reactjs.org/docs/reconciliation.html#recursing-on-children)、要素を特定するための __`key`__ プロパティ（数値 or 文字列）を要求します。
__配列の `map` メソッドで複数の要素を生成するとき__ に、それぞれの要素の `key` プロパティとして何らかのユニーク値を設定してやれば、この警告は消えます。
これは `li` 要素以外の要素（例えば `div` 要素）であっても同様です。

{{< code lang="tsx" >}}
const listItems = props.titles.map((title: string, index: number) =>
  <li key={index}>{title}</li>
);
{{< /code >}}

`key` プロパティの値は、同階層のリスト要素間で一意であればよいので、上記のようにリスト要素のインデックスを `key` に指定することである程度うまく動作します。
ただし、この方法ではリスト要素の順番が入れ替わった場合などに効率の悪い再描画が発生します。
よりよい方法は、次のように、表示するデータが持っている一意な ID やハッシュ値を指定することです。

{{< code lang="tsx" >}}
const listItems = books.map((book: Book) =>
  <li key={book.id}>{book.title}</li>
);
{{< /code >}}

このような書き方を可能にするには、元の `Books` コンポーネントのプロパティで、データオブジェクト (`Book`) の配列を受け取れるように修正する必要があります。
下記に修正後の全体のコードを示します。

{{< code lang="tsx" title="components/books.tsx" >}}
import * as React from 'react';

// Books コンポーネントの items 配列プロパティの要素の型
export interface Book {
  id: number;
  title: string;
}

// Books コンポーネントのプロパティ
export interface BooksProps {
  items: Book[]
}

// Books コンポーネント
export const Books: React.FC<BooksProps> = (props) => {
  if (props.items.length == 0) return null;
  const listItems = props.items.map((book: Book) =>
    <li key={book.id}>{book.title}</li>
  );
  return <ul>{listItems}</ul>;
};
{{< /code >}}

{{< code lang="tsx" title="index.tsx（使用例）" >}}
import * as React from 'react';
import * as ReactDOM from 'react-dom';
import { Book, Books } from './components/books';

const books: Book[] = [
  { id: 100, title: 'タイトル1' },
  { id: 200, title: 'タイトル2' },
  { id: 300, title: 'タイトル3' },
];

ReactDOM.render(
  <Books items={books} />,
  document.getElementById('root')
);
{{< /code >}}


（おまけ）コンポーネント内で自主的にデータを取得する
----

下記の `Books` コンポーネントは、DOM 要素として追加された段階 ([componentDidMount](https://ja.reactjs.org/docs/react-component.html#componentdidmount)) で、自分が表示すべきデータを取得して描画を行います。
コンポーネントのライフサイクルメソッドをオーバーライドするため、関数コンポーネントではなく、クラスコンポーネントとして定義する必要があります。
ここでは固定のデータを `state` に設定していますが、ネットワークからデータ取得するようなケースでも、このタイミングでデータ取得するとよいでしょう。

{{< code lang="tsx" title="components/books.tsx" >}}
import * as React from 'react';

interface Book {
  id: number;
  title: string;
}

// Books コンポーネントの state の型
interface BooksState {
  books: Book[]
}

// Books コンポーネントの定義
export class Books extends React.Component<{}, BooksState> {
  constructor(props: {}) {
    super(props);
    // 初期状態のデータは空っぽでOK
    this.state = { books: [] };
  }

  public render(): React.ReactNode | null {
    const books = this.state.books;
    if (books.length == 0) return null;
    const listItems = books.map((b: Book) =>
      <li key={b.id}>{b.title}</li>
    );
    return <ul>{listItems}</ul>;
  }

  // 描画準備ができたときに呼び出される
  public componentDidMount() {
    // 実際はここでデータフェッチを行う想定
    const newBooks: Book[] = [
      { id: 100, title: 'タイトル1' },
      { id: 200, title: 'タイトル2' },
      { id: 300, title: 'タイトル3' },
    ];

    // データ取得が完了したら state を更新して再描画
    this.setState({ books: newBooks });
  }
};
{{< /code >}}

`Books` コンポーネントは自らデータを取得するため、`Books` コンポーネントを使用する側ではデータを渡す必要はありません。

{{< code lang="tsx" title="index.tsx（使用例）" >}}
import * as React from 'react';
import * as ReactDOM from 'react-dom';
import { Books } from './components/books';

ReactDOM.render(<Books />, document.getElementById('root'));
{{< /code >}}

