---
title: "React + TypeScript の環境を整える (2) Parcel を使う方法"
url: "/p/r7fov4b"
date: "2020-08-15"
tags: ["TypeScript", "React"]
weight: 101
---

Parcel とは
----

{{< image src="img-001.png" >}}

[Parcel](https://parceljs.org/) は、__ゼロ設定__ をウリとした Web アプリバンドラーです。
React + TypeScript + SCSS + CSS Modules といった環境を自力でセットアップしようとすると [なかなか大変な設定が必要](/p/m4dmt3a) なのですが、Parcel を使うと、開発環境がサクサクッと完成します。

React プロジェクトを作成するときは、[create-react-app](https://github.com/facebook/create-react-app) を使って雛形を生成する方法もありますが、余計なファイルがたくさん作られたりして、あまり分かりやすいとは言えません。
そんなときは Parcel を使ってみると、そのシンプルさに驚くと思います。

Parcel を使うと、次のような機能がほとんど設定なしでいきなり使えます。
[コマンドラインオプションの一覧](https://parceljs.org/cli.html) を見ると、どのような機能があるかをざっと把握することができます。

- 開発用の Web サーバー機能（HMR: Hot Module Replacement 対応）
- TypeScript の自動変換（HTML ファイルから直接ロードする記述が可能）
- PostCSS による CSS 生成（SCSS、ベンダープレフィックス）
- CSS ファイルのインポート (CSS Modules)
- JSON ファイルのインポート
- PNG ファイルのインポート
- Pug (Jade) による HTML 生成
- リリース用の minify

また、Parcel は Node モジュールの自動インストール機能を備えており、`parcel` コマンドで開発用 Web サーバーを起動すると、自動的に依存モジュールをインストールしてくれます。
例えば、HTML ファイルから TypeScript ファイルを読み込んでいると、`typescript` モジュールが自動的にインストールされます。


Parcel のインストール
----

Web サイト用のディレクトリを作成し、__`parcel`__ モジュールを開発用にインストールします。

{{< code >}}
$ mkdir myapp && cd myapp
$ npm init -y
$ npm install --save-dev parcel
{{< /code >}}

これで、Parcel の開発用サーバーを起動したり、Web サイトコンテンツのビルドを行えるようになります。

TypeScript のインストールは Parcel による自動インストールに任せることができるので、ここでは手動ではインストールしないことにします。
明示的にインストールしておきたければ次のように実行しておきます。

{{< code title="これは手動で行わなくてよい" >}}
$ npm install --save-dev typescript
{{< /code >}}

{{< note >}}
依存モジュールの自動インストール機能は、リリース用ビルド (`parcel build`) を行う場合はデフォルトで無効になっています。
自動インストール機能は便利ですが、基本的には Node モジュールは明示的にインストールした方がよいかもしれません。
Parcel によって依存モジュールが自動インストールされるときは、`package.json` の依存情報が更新されます。
更新された `package.json` をコミットするのを忘れないようにしてください。
{{< /note >}}


Web サイトのコンテンツを用意する
----

Web サイトのコンテンツとして、適当な HTML ファイルと TypeScript ファイルを用意します。
ソースコードとなる HTML ファイルおよび TypeScript ファイルは、`src` ディレクトリ以下に配置することにします。

{{< code lang="html" title="src/index.html" >}}
<!DOCTYPE html>
<html lang="ja">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>My App</title>
  <script src="script/index.ts" async></script>
</head>
<body>
  <div id="root"></div>
</body>
</html>
{{< /code >}}

HTML ファイルの中から、__直接 TypeScript ファイル (`index.ts`) を読み込んでいる__ ことに注目してください。
このように記述しておくだけで、Parcel が自動的に TypeScript トランスパイルを実行し、HTML ファイルから正しい JavaScript ファイルを読み込むように変換してくれます。

下記の TypeScript コードは、上記の HTML ファイルから読み込むファイルです。

{{< code lang="typescript" title="src/script/index.ts" >}}
const root = document.getElementById('root') as HTMLElement;
root.innerText = 'Hello Parcel';
{{< /code >}}

これくらいのコードであれば TypeScript にする必要はほとんどありませんが、ここでは TypeScript のトランスパイルが自動的に実行されることを示すために `.js` ではなく `.ts` ファイルを使用しています。


開発用 Web サーバーを起動する (parcel serve)
----

`src` ディレクトリ以下に必要な HTML ファイルと TypeScript ファイルを作成したら、次のように __`parcel (parcel server)`__ コマンドを実行して、開発用の Web サーバーを起動します。
`--open` オプションを付けると、自動的に Web ブラウザを開いてくれます。
ポート番号をデフォルトの `1234` から変更したい場合は、`--port` オプションを使用してください。

{{< code >}}
$ npx parcel src/index.html --open
Server running at http://localhost:1234
✨  Built in 131ms.
{{< /code >}}

この時点で、TypeScript モジュールがインストールされていない場合は、Parcel が自動的にインストールし、さらにトランスパイルまで実行してくれます。
変換後の HTML ファイルや JavaScript ファイルは、デフォルトで __`dist`__ ディレクトリに出力されます。
出力先のディレクトリを変更したい場合は、__`-d`__ オプションを使用します。

開発用の Web サーバーでは HMR (Hot Module Replacement) 機能が有効になっているので、TypeScript コード (`index.ts`) の内容を変更すると、再起動なしで自動的に反映されます。
`root.innerText = 'Hello Parcel'` の行を変更して、表示が変更されることを試してみてください。

開発用の Web サーバーでは、環境変数 __`NODE_ENV=development`__ が設定されます。
これを利用して、開発時のみ有効なコードブロックを記述できます。

{{< code lang="typescript" >}}
if (process.env.NODE_ENV === 'development') {
  // 開発中のみ有効なコード
}
{{< /code >}}


デプロイ用にビルドする (parcel build)
----

__`parcel build`__ コマンドを実行すると、公開用の Web サーバーにデプロイするためのファイル群を作成します。

デフォルトの出力先が、`parcel (serve)` で開発用サーバーを起動した場合と同じ `dist` ディレクトリになっているので、ファイルが混在しないように、`-d` オプションで出力先を変えておいた方がよいでしょう。
次の例では、出力先を `build` ディレクトリに設定しています。

{{< code >}}
$ npx parcel build src/index.html -d build
✨  Built in 1.50s.

build/script.6d3c0223.js        1.18 KB    757ms
build/script.6d3c0223.js.map      280 B      2ms
build/index.html                  193 B    693ms
{{< /code >}}

あとは、`build` ディレクトリ以下に生成されたファイル群を Web サーバーにデプロイするだけです。

`parcel build` によるバンドル時には、デフォルトで HTML、CSS、JS ファイルの圧縮（改行削除や、変数名の簡略化）が有効になっています。
その他の特徴は、下記の公式ドキュメントを参照してください。

- 参考: [Parcel - Production](https://parceljs.org/production.html)


package.json にビルド用スクリプトを追加する
----

ここまでの例では、直接 `parcel` コマンドを実行していましたが、npm のスクリプトとして登録しておくと便利です。

{{< code lang="js" title="package.json（抜粋）" >}}
"scripts": {
  "start": "parcel src/index.html --open",
  "build": "rm -rf build && parcel build src/index.html -d build"
},
{{< /code >}}

このように記述しておくと、__`npm start`__ で開発用サーバーの起動、__`npm run build`__ で本番用ビルドを行えるようになります。

{{< note title="build ディレクトリの削除" >}}
`parcel build` コマンドは、前回のビルド時に出力したファイルを削除してくれません。
余計なファイルがデプロイされてしまうのを防ぐため、上記のように `build` スクリプトで古いファイルを削除しておくとよいでしょう。
{{< /note >}}


React (Preact) のセットアップ
----

次に React コンポーネント (`.tsx`) を読み込めるようにしてみます。

React 関連の NPM パッケージは先に手動でインストールしておきます。
React 本体は Parcel による自動インストールに任せることもできるのですが、TypeScript の型定義ファイルは自動インストールしてくれないみたいです。

{{< code >}}
$ npm install --save react react-dom
$ npm install --save-dev @types/react @types/react-dom
{{< /code >}}

ここでは、次のような簡単な `App` コンポーネントを作って表示してみます。
JSX コードが含まれているので、拡張子を `.tsx` にすることに注意してください。

{{< code lang="tsx" title="src/script/components/App.tsx" >}}
import * as React from 'react';

export const App: React.FC = () => {
  return <h1>Hello React</h1>;
};
{{< /code >}}

`index.ts` ファイルの拡張子も `.tsx` に変更し、上記の `App` コンポーネントを読み込んで使用します。

{{< code lang="tsx" title="src/script/index.tsx" >}}
import * as React from 'react';
import * as ReactDOM from 'react-dom';
import { App } from './components/App';

ReactDOM.render(<App />, document.getElementById('root'));
{{< /code >}}

最後に `index.html` ですが、`index.ts` ではなく `index.tsx` を読み込むようにするところだけ変更します。

{{< code lang="html" title="src/index.html" >}}

<!DOCTYPE html>
<html lang="ja">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>My App</title>
  <script src="script/index.tsx" async></script>
</head>
<body>
  <div id="root"></div>
</body>
</html>
{{< /code >}}

あとは何も設定しなくても、次のようにおもむろに Parcel の開発用サーバを起動するだけで、React を使った Web サイトが表示されます。

{{< code >}}
$ npm start
{{< /code >}}

本当に「ゼロ設定」ですね！
こんなに簡単でよいのでしょうか。


おまけ：SCSS の有効化
----

生の CSS をゴリゴリ記述するのはつらいので、SCSS は使えるようにしておいた方がよいでしょう。
Parcel で SCSS を使用するには、ちょっとだけ設定が必要です。

### Parcel で SCSS を使うための設定

[公式サイト](https://parceljs.org/scss.html) の記述通りですが、次のように `sass` モジュールをインストールし、

{{< code title="sass モジュールのインストール" >}}
$ npm install --save-dev sass
{{< /code >}}

下記のような設定ファイル (__`.sassrc`__) をプロジェクトルートに配置すれば OK です。

{{< code lang="js" title=".sassrc" >}}
{
  "includePaths": ["node_modules"]
}
{{< /code >}}

### SCSS を使ってみる

次のような簡単な SCSS ファイルを使ってみます。

{{< code lang="scss" title="src/css/main.scss" >}}
$BG_COLOR: yellow;

.app-title {
  background: $BG_COLOR;
}
{{< /code >}}

あとは、HTML ファイルから次のように参照しておけば、

{{< code lang="html" title="src/index.html（抜粋）" >}}
<link rel="stylesheet" href="css/main.scss">
{{< /code >}}

React コンポーネント (`.tsx`) から次のように CSS クラスを参照できるようになります。

{{< code lang="tsx" title="src/script/components/App.tsx" >}}
import * as React from 'react';

export const App: React.FC = () => {
  return <h1 className="app-title">Hello React</h1>
};
{{< /code >}}

`index.html` の中でスタイルシートを読み込むのではなく、`.tsx` の中でインポートして参照することもできます。
この場合は、`.tsx` ファイルのあるディレクトリからの相対パスになることに注意してください。
CSS クラスの使用方法は上記と同様です。

{{< code lang="ts" >}}
import '../../css/main.scss';
{{< /code >}}

ただ、このように `.tsx` ファイルの中から CSS ファイルをインポートするのであれば、下記の CSS Modules の仕組みを使って、そのコンポーネント専用の CSS ファイルをインポートするようにした方がよいでしょう。


おまけ：CSS Modules の有効化
----

CSS Modules の仕組みを使用するときも、ちょっとだけ設定が必要です。

### Parcel で CSS Modules を使うための設定

ほぼ [公式サイトの説明](https://parceljs.org/css.html#postcss) 通りですが、__`postcss-modules`__ のインストールが必要です。

{{< code >}}
$ npm install --save-dev postcss-modules
{{< /code >}}

そして、プロジェクトルートに次のような設定ファイル (__`.postcssrc`__) を配置します。

{{< code title=".postcssrc" >}}
{
  "modules": true
}
{{< /code >}}

### CSS Modules を使ってみる

CSS Modules の仕組みでスタイル定義する場合は、React コンポーネントと同じディレクトリに `.css` ファイル（あるいは `.scss` ファイル）を置いておくと、コンポーネントとしてのまとまりがよくなります。
前述の SCSS の設定を行っておけば、`.scss` ファイルも CSS Modules の仕組みで読み込むことができます。

下記の `App.scss` ファイルでは、`App` コンポーネント用のスタイル定義を行っています。
CSS Modules の仕組みを使うと、CSS クラス名の名前空間がファイルごとに分離されるため、シンプルなクラス名を付けることができます。

{{< code lang="scss" title="src/script/components/App.scss" >}}
.title {
  background: red;
}
{{< /code >}}

ここでは、`app-title` というクラス名から `app` プレフィックスを省略して、`title` というシンプルなクラス名に変更してみました。
モジュール化されたスタイシートは、`.tsx` ファイルから次のように使用します。

{{< code lang="tsx" title="src/script/components/App.tsx" >}}
import * as React from 'react';
import styles from './App.scss';

export const App: React.FC = () => {
  return <h1 className={styles.title}>Hello React</h1>
};
{{< /code >}}


おまけ： favicon の設定
----

エントリポイントとなる `index.html` の `head` 要素の中に、次のように favicon 設定をしておくと、Parcel によるサイトビルド時に自動的に favicon ファイルがコピーされます。

{{< code lang="html" title="index.html（抜粋）" >}}
<link rel="icon" type="image/png" href="assets/img/favicon-32x32.png" />
{{< /code >}}

favicon ファイルは、この `index.html` からの相対パスで配置します。
今回は `index.html` を `src` ディレクトリ以下に置くことにしたので、favicon ファイルを `src/assets/img/favicon-32x32.png` というパスで配置します。

