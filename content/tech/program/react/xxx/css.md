---
title: "React コンポーネントに CSS スタイルを設定する"
url: "/p/eu4cksy"
date: "2020-07-12"
tags: ["React", "TypeScript"]
---

React は Web サイトの View レイヤのコンポーネントを作るためのライブラリですが、CSS ファイルの扱い方は特に決められておらず、今でも多くの人が試行錯誤しています。
ここでは、React アプリにおける CSS の扱い方を、下記のように分類して順番に説明していきます。

1. 従来通り HTML 起点でスタイルを読み込む方法
2. インラインスタイル
3. CSS Modules
4. CSS in JS ライブラリ


HTML ファイルで読み込んだ CSS ファイルを参照する
----

これは React を使わない従来の HTML/CSS のやり方に近い方法です。
HTML ファイル内の `style` 要素で定義した CSS クラスや、HTML から読み込んだ CSS ファイル内で定義した CSS クラスを React コンポーネントから使用します。

例えば、HTML ファイル内で次のようにスタイル定義されているとします。

{{< code lang="html" title="index.html" >}}
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>MyApp</title>
    <meta name="viewport" content="width=device-width, initial-scale=1"></head>
    <style>
      .hello {
        color: blue;
        background: #aaccff;
        padding: 0.5em;
        font-weight: bolder;
        border-radius: 0.5em;
      }
    </style>
  </head>
  <body>
    <div id="root"></div>
    <script src="index.js"></script>
  </body>
</html>
{{< /code >}}

次の `Hello` コンポーネントは、1 つの `div` 要素を生成するだけの簡単な React コンポーネントで、上記で定義した CSS クラス `hello` を使用しています。

{{< code lang="tsx" title="components/hello.tsx" >}}
import * as React from 'react';

export const Hello: React.FC = () => {
  return (
    <div className="hello">Hello</div>
  );
};
{{< /code >}}

JSX 要素で CSS クラスを指定する場合は、`class` 属性ではなく、__`className`__ 属性を使用することに注意してください。
`class` という名前は JavaScript のキーワードと競合するため、このような仕様になっています。

下記は、この `Hello` コンポーネントの使用例です。

{{< code lang="tsx" title="index.tsx（使用例）" >}}
import * as React from 'react';
import * as ReactDOM from 'react-dom';
import { Hello } from './components/hello';

ReactDOM.render(<Hello />, document.getElementById('root'));
{{< /code >}}

このようなスタイル定義の方法は、伝統的な HTML/CSS のやり方に近いのでシンプルですが、

- React コンポーネントからスタイル定義が分離してしまう
- グローバルな名前空間でスタイル定義しないといけない

といった課題があります。


インラインスタイル（TypeScript にハードコード）
----

次の例では、TypeScript コードにハードコードする形でスタイルを定義しています。
このようにすることで、スタイル定義をコンポーネント内で完結できるため、別のコンポーネントと CSS クラス名が競合してしまうという心配がありません。

{{< code lang="tsx" title="components/hello.tsx" >}}
import * as React from 'react';

// JSX要素用のスタイル定義（React.CSSProperties形式）
const style: React.CSSProperties = {
  color: 'blue',
  background: '#aaccff',
  padding: '0.5em',
  fontWeight: 'bolder',
  borderRadius: '0.5em',
};

export const Hello: React.FC = () => {
  return <div style={style}>Hello</div>;
};
{{< /code >}}

### React.CSSProperties 形式でのスタイル記述

ポイントは、スタイル定義のために __React.CSSProperties__ 型のオブジェクトを作成するところです。

{{< code lang="js" >}}
const style: React.CSSProperties = {
  color: 'blue',
  background: '#aaccff',
  padding: '0.5em',
  fontWeight: 'bolder',
  borderRadius: '0.5em',
};
{{< /code >}}

プロパティ名が通常の CSS とは異なることに注意してください。
通常の CSS では、`font-weight` や `border-radius` のように、プロパティ名の単語はハイフンで区切られますが、`React.CSSProperties` では `fontWeight` や `borderRadius` といったキャメルケースでプロパティ名が定義されています。
これは、JavaScript (TypeScript) の文法上、プロパティ名にハイフンを含められないという制約によるものです。
あと、各プロパティがセミコロン (`;`) ではなく、カンマ (`,`) で区切られているところも、通常の CSS とは異なりますね。

### CSS プロパティ名を使ったスタイル定義

どうしても CSS オリジナルなプロパティ名を使いたいのであれば、次のように、プロパティ名をシングルクォートで囲むことで、CSS のプロパティ名をそのまま使うことができます。
ただし、このようにするとプロパティ名の補完機能が効かなくなるので、できれば `React.CSSProperties` 型で適宜することをお勧めします。

{{< code lang="js" >}}
// JSX要素用のスタイル定義（CSS形式）
const style = {
  'color': 'blue',
  'background': '#aaccff',
  'padding': '0.5em',
  'font-weight': 'bolder',
  'border-radius': '0.5em',
};
{{< /code >}}

### スタイル定義を入れ子で作成する

複数のスタイル定義を TypeScript コードにハードコードする場合、スタイル定義が複数の変数に散らばってしまうとコードが煩雑になってしまいます。
次の例では、入れ子構造の `styles` オブジェクト 1 つでスタイル定義をまとめています。

{{< code lang="tsx" title="components/app.tsx" >}}
import * as React from 'react';

const styles: {[key: string]: React.CSSProperties} = {
  container: {
    display: 'flex',
    minHeight: '100vh',
  },
  sidebar: {
    background: 'lightgray',
    width: '150px',
  },
  body: {
    background: '#ddd',
    flex: 1,
  }
};

export class App extends React.Component {
  public render(): React.ReactNode {
    return (
      <div style={styles.container}>
        <div style={styles.sidebar}>Sidebar</div>
        <div style={styles.body}>Body</div>
      </div>
    );
  }
}
{{< /code >}}

このようなインラインスタイルでのスタイル指定は、React コンポーネント内にスタイル定義を完結できるという利点はありますが、パフォーマンスの低下や、XSS などのセキュリティの懸念が常に付きまといます。
React に限らず、一般的な Web 設計のプラクティスになりますが、できるかぎり CSS クラスを使った方法を採用した方がよいでしょう。


CSS Modules
----

[CSS Modules](https://glenmaddern.com/articles/css-modules) の仕組みを利用すると、スタイルの名前空間を CSS ファイル単位（モジュール単位）で分離することができるようになります。
具体的には、外部の CSS ファイルを React コンポーネントなどの TypeScript (JavaScript) コードからインポートすることにより、スタイル設定をそのコンポーネントに閉じて行えるようになります。

{{< code lang="tsx" title="目指す形" >}}
import * as React from 'react';
import css from './hello.css';

export const Hello: React.FC = () => {
  return <div className={css.box}>Hello</div>;
};
{{< /code >}}

従来の HTML + CSS の構成では、CSS の名前空間がグローバルに共有されるため、クラス名が競合しないように最新の注意を払って設計する必要がありました。
BEM などの命名規則の登場により、CSS の設計手法は大分整ってきた感がありますが、やはり大きなサイトになってくると CSS の管理は大きなストレスになります。
このような場合に CSS Modules の仕組みを採用すると、コンポーネントごとに CSS の名前空間が独立しているので、`.button` のようなシンプルなクラス名を気軽に使うことができるようになります。

ここでは、最も一般的な？ webpack の __`css-loader`__ と __`style-loader`__ を使った CSS Modules の実現方法を説明します。

### CSS ファイルをインポートできるようにする

JavaScript コードには本来 CSS ファイルを解釈する機能は備わっていないので、 webpack などのバンドルツールの力を利用する必要があります。
webpack の拡張として `css-loader` を使うと、JS ファイルから CSS ファイルを `import` できるようになり、さらに `style-loader` を使うことで、JS コードから CSS を出力できるようになります（つまり両方必要です）。

{{< code title="インストール" >}}
$ npm install --save-dev css-loader style-loader
{{< /code >}}

webpack の設定を修正し、`css-loader` と `style-loader` を使って CSS ファイルを TypeScript (JavaScript) コードから使用できるようにします。
処理順序の都合上、`style-loader` を先に指定しないといけないことに注意してください。

{{< code lang="js" title="webpack.config.js（抜粋）" >}}
module.exports = {
  // ...
  module: {
    rules: [
      // CSS ファイルの読み込み
      {
        test: /\.css$/,
        use: ['style-loader', 'css-loader']
      },
    ]
  }
};
{{< /code >}}

これで、次のように TypeScript コードから CSS ファイルをインポートして、React コンポーネントで使用できるようになります。
`.css` ファイルは、`.ts` ファイルと同じディレクトリに格納しておけば OK です。

{{< code lang="css" title="styles.css" >}}
.hello {
  color: blue;
  background: #aaccff;
  padding: 0.5em;
  font-weight: bolder;
  border-radius: 0.5em;
}
{{< /code >}}

{{< code lang="typescript" title="components/hello.tsx" >}}
import * as React from 'react';
import './styles.css';

export const Hello: React.FC<IProps> = (prop) => {
  return <div className="hello">{prop.text}</div>;
};
{{< /code >}}

これで、TypeScript コードから CSS ファイルをインポートできるようになりましたが、CSS の名前空間はグローバルなままです。
上記の `hello` という CSS クラス名は、他のコンポーネントで使っている CSS とバッティングする可能性があります。

### CSS をモジュール化する

webpack の設定を次のように変更すると、`css-loader` の CSS Modules 機能を有効化できます。
この設定により、React コンポーネントごとに CSS の名前空間が分離されるようになります。

{{< code lang="js" title="webpack.config.js（抜粋）" >}}
module.exports = {
  // ...
  module: {
    rules: [
      // CSS Modules
      {
        test: /\.css$/,
        use: [
          {
            loader: 'style-loader',
          },
          {
            loader: 'css-loader',
            options: {
              modules: {
                localIdentName: '[name]__[local]___[hash:base64:5]',
              }
            }
          }
        ]
      },
      // ...
    ]
  }
};
{{< /code >}}

（入れ子がやばい。。。）

実際に出力される CSS のクラス名は、上記の `localIdentName` で指定したフォーマットで生成されます。
上記の設定の場合は、「CSSファイル名__CSSクラス名___ハッシュ値」というクラス名になります。

`Hello` コンポーネント (`hello.tsx`) 用の CSS ファイルは、ベースネームを合わせて `hello.css` としてしまうのがシンプルです。
CSS Modules により CSS クラス名の名前空間が独立するので、下記のようなシンプルなクラス名 (`.box`) を付けることができます（別に `.root` とか `.container` とか何でもよいです）。

{{< code lang="css" title="hello.css" >}}
.box {
  color: blue;
  background: #aaccff;
  padding: 0.5em;
  font-weight: bolder;
  border-radius: 0.5em;
}
{{< /code >}}

この CSS ファイルは、React コンポーネントの中から下記のように使用します。
デフォルトエクスポートされたものとして、変数経由（下記の場合は `css`）で使用するところがポイントです。
こうすることで、複数の CSS ファイルを複数インポートした場合にも、クラス名がバッティングしません。

{{< code lang="tsx" title="components/hello.tsx" >}}
import * as React from 'react';
import css from './hello.css';

export const Hello: React.FC = () => {
  return <div className={css.box}>Hello</div>;
};
{{< /code >}}

基本的にここまでで CSS Modules の導入は完了なのですが、TypeScript を使っている場合は、CSS ファイルをインポートしているところで次のようなエラーが出ます。

{{< code title="TypeScriptエラー" >}}
Cannot find module './hello.css' or its corresponding type declarations.
{{< /code >}}

これは、CSS ファイルを TypeScript のモジュールとして読み込んでいるのに、肝心の型定義が存在しないよというエラーです。
CSS ファイルごとに型定義ファイルを作ると完璧なのですが、そんなことはやっていられないので、次のような型定義ファイルをソースツリーのルートにおいてしまうのが手っ取り早いです。

{{< code lang="typescript" title="typings.d.ts" >}}
declare module "*.css" {
  const styles: { [className: string]: string };
  export default styles;
}
{{< /code >}}

まじめに CSS ファイルごとに型定義をするのであれば、`hello.css` の型定義ファイルは次のように作成します。
こうすると、VS Code での補完機能なども働くようになりますが、労力には見合わないと思います（こういった型定義ファイルを自動生成する npm モジュールもあります）。

{{< code lang="typescript" title="hello.css.d.ts" >}}
export const box: string;

// CSSクラスが増えるごとに次のように同様に追加していく
// export const className1: string;
// export const className2: string;
// export const className3: string;
{{< /code >}}

と、ここまで webpack を使った CSS Modules の導入方法を説明してきましたが、__はっきりいって面倒__ ですね。
思想自体は素晴らしいものなのですが。

ということで、もう少しライトにかつ柔軟にコンポーネント側でスタイル定義を行いたい、という思いから CSS-in-JS なライブラリが数多く登場してきています。


CSS in JS ライブラリ
----

コンポーネントの TypeScript (JavaScript) コード内で柔軟なスタイル定義も行えるようにしたライブラリが、CSS in JS 系のライブラリです。
下記のように多くのライブラリが開発されています。

- [React: CSS in JS techniques comparison](https://github.com/MicheleBertoli/css-in-js)

有名どころは [styled-components](https://styled-components.com/) でしょうか。
自動でベンダープレフィックスを付けてくれたり、入れ子構造でスタイル定義できたりします。

{{< code title="インストール" >}}
$ npm install --save-dev styled-components
$ npm install --save-dev @types/styled-components
{{< /code >}}

使い方は次のような感じでとてもシンプルです。

{{< code lang="tsx" title="components/hello.tsx" >}}
import * as React from 'react';
import styled from 'styled-components';

// Title コンポーネントの定義
const Title = styled.h1`
  font-size: 1.5em;
  text-align: center;
  color: palevioletred;
`;

// Hello コンポーネントの定義
export const Hello: React.FC = () => {
  return <Title>Hello</Title>;
};
{{< /code >}}

このコードでは `Hello` コンポーネントを定義しているのですが、出力関数の中で使っている `Title` コンポーネントは、styled-components の機能により生成されています（`styled.h1` の部分）。
最終的に、`Title` タグを使用した JSX コードは、スタイル付きの `h1` 要素として出力されます。

Web ページのスタイル定義方法は、これからも試行錯誤が続きそうです。

