---
title: "Next.js アプリのディレクトリ構成を考える（Atomic Design と Presentational and Container Components）"
url: "/p/4is2ahp"
date: "2021-07-13"
tags: ["Atomic Design", "Next.js"]
---

Web サイトを構築するにあたって、よく参照されるコンポーネントの分類手法として、__Atomic Design__ と __Presetational and Container Components__ があります。

- Atomic Design ... UI の粒度と具体性によって 5 レベルに分類する
- Presentational and Container Components ... 「表示」と「振る舞い」の役割で分類する

ここでは、それぞれに関して概要をざっと眺めた上で、Next.js プロジェクトにどんな形で適用していけばいいかを考えてみます。


Atomic Design とは
----

Web デザインにおける UI コンポーネントの分割粒度の考え方として、Brad Frost 氏の __Atomic Design__ があります。

- [Atomic Design - Brad Frost 氏のブログ記事](https://bradfrost.com/blog/post/atomic-web-design/)
- [Atomic Design - 書籍版（Webで読めます）](https://atomicdesign.bradfrost.com/table-of-contents/)

Atomic Design では、UI コンポーネントを粒度の小さい順に次のように分類します。

{{< image w="700" src="img-001.png" title="Atomic Design の 5 つのレベル（Brad Frost 氏のブログより）" >}}

- __Atoms（原子）__ ... これ以上分割できない単位（例: ラベル、入力、ボタン）
- __Molecules（分子）__ ... 意味のある UI パーツ単位 （例: 検索フォーム (ラベル + 入力 + ボタン)）
- __Organisms（有機体、生物）__ ... ページの一部分を構成する。Molecules をどう組み合わせるべきかという、アプリドメインの知識が入ってくる。
- __Templates（テンプレート）__ ... ページ全体のレイアウト。最初はプレースホルダーだらけかもしれないが、徐々に具体的な Organisms が配置されて最終形態に近づいていく。
- __Pages（ページ）__ ... Templates に具体的なデータを入れたもの（特定のページ）。

よく Organisms の扱いで迷うようですが、汎用性の側面から次のような感じで分類すると、プロジェクト内でコンポーネントを整理しやすくなります。
Pages に関しては、Templates に対してデータを流し込んだものなので、ここでは省略しています。

<table>
  <thead>
    <tr>
      <th></th>
      <th>汎用性</th>
      <th>考え方</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th>Atoms</th>
      <td style="text-align:center">◯</td>
      <td rowspan="2">純粋な UI パーツ。通常、アプリドメインの知識を持たず、渡されたデータをそのまま描画するだけ。React であれば、渡された <code>props</code> を表示する。表示幅やマージンなどは、より上位のモジュールから自由に設定できるようになっているのが望ましい。</td>
    </tr>
    <tr>
      <th>Molecules</th>
      <td style="text-align:center">◯</td>
    </tr>
    <tr>
      <th>Organisms</th>
      <td style="text-align:center">△</td>
      <td rowspan="2">アプリドメインの知識を持って、表示内容を構成する。例えば、アプリヘッダーなど、そのアプリ特有のパーツを構成する。Atoms/Molecules は渡されたデータをただ表示する役割しか持たないので、おそらくデータフェッチ処理などのロジックは、この Organisms より上位のコンポーネントで行うことになる。
    </tr>
    <tr>
      <th>Templates</th>
      <td style="text-align:center">×</td>
    </tr>
  </tbody>
</table>

正確には、Atomic Design では「振る舞い」をどのレベルで導入するかに関しては言及されていないのですが、少なくとも Atoms/Molecules は「表示」のみを責務とするコンポーネントとして定義するのがよいでしょう。


Presentational and Container Components とは
----

Dan Abramov 氏の [Presentational and Container Components](https://medium.com/@dan_abramov/smart-and-dumb-components-7ca2f9a7c7d0) は、コンポーネントを「見た目」と「振る舞い」の観点で分類します。

- __Presentational components__ ... 「見た目」だけを提供するコンポーネント。上位のコンポーネントから渡された `props` を単純に表示する。
- __Container components__ ... 「振る舞い」を提供するコンポーネント。データフェッチや状態の保持、変更などの責務を負い、表示すべきデータを Presentational components に渡す。

「見た目」と「振る舞い」を分離することによって、それぞれの観点からテストしやすくなったり、コンポーネントとしての再利用性が上がるといった利点があります。
多くの場合は、「見た目」と「振る舞い」の定義は一対一になりますが、一対多の関係になることもあります。
例えば、1 つの UI コンポーネントに流し込むデータの取得先が 2 種類あるようなケースです。
このようなケースでは、Presentational and Container Components パターンでコンポーネント分離できていれば対応が簡単です。

下記は、簡単な Presentational components と Container components の実装例です。

{{< code lang="tsx" title="Presentational components（見た目）の例" >}}
export const AvatarCard: FC<{ name: string }> = ({ name }) => {
  const classes = useStyles()
  return <div className={classes.avatar}>I am {name}</div>
}
{{< /code >}}

表示するデータ自体は `props` パラメーターで受け取っていて、それをそのまま表示しているだけなので、これは「見た目」の実装です。
ここではフック関数 (`useStyles`) を呼んじゃってますが、これは装飾のためのスタイル設定なので OK。

{{< code lang="ts" title="Container components（振る舞い）の例" >}}
export const AvatarCardContainer: FC = () => {
  const name = useAvatar()
  return <AvatarCard name={name} />
}
{{< /code >}}

Container components の方で呼び出している `useAvatar` フックは、データフェッチを行って情報を取得します。
こちらは「振る舞い」の実装です。
「表示」の処理に関しては、Presentational components に委譲しています。

React にフックの仕組みが導入されたことにより、「振る舞い」の大部分はフック関数に切り出すことが可能になりました。
そのため、Container componets へ「振る舞い」を切り出すという方法は、以前ほど採用されなくなっているかもしれません。
それでも、完全に「見た目」と「振る舞い」を確実に分離したいときは、Presentational and Container Components の考え方が必要になってきます。
フックだけでは、`useXxx` という関数呼び出し部分が、どうしても UI コンポーネント内に残ってしまうからです。


Next.js アプリのディレクトリ構成を考える
----

上記を考慮した、Next.js アプリの（`src` 以下の）ディレクトリ構成をいくつか考えてみます。
実際のプロジェクトでは、次のようなディレクトリも作ることになると思いますが、ここではコンポーネント用のディレクトリだけ考えます。

- `hooks/` ... React のカスタムフック
- `types/` or `interfaces/` ... TypeScript 型定義
- `utils/` or `libs/` ... その他のユーティリティ（データフェッチ関数など）

まず、Next.js アプリでは、ページコンポーネントは `pages` ディレクトリに格納することが決まっているので、Atomic Design の Templates (Pages) に相当するコンポーネントは自然にここに入れることになります。

### パターン(1) components ディレクトリ 1 つだけ

一番シンプルなディレクトリ構成だとこんな感じでしょうか。

- __`components/`__ ... ページコンポーネント以外のコンポーネント
- __`pages/`__ ... ページコンポーネント

この方法では、Atomic Design も Presentational and Container Componets も特に意識せずに、全てのコンポーネントを __`conpoments`__ に入れちゃいます。
最初はここからスタートしてもよいかもしれません。

`components` ディレクトリ以下は、コンポーネントのカテゴリごとに、さらに階層化されることがあります。

- `components/`
    - `button/`
    - `card/`

ここで Atomic Design による階層化を行うこともあります（このやり方は割とよく見受けられます）。

- `components/`
    - `atoms/`
    - `molecules/`
    - `organisms/`

各ディレクトリ以下のコンポーネントの依存関係は、基本的には `atoms <- molecules <- organisms` となります。
ディレクトリ名でソートしたときに、この順番に並ぶところがいいですね。

`components` 以下に全てのコンポーネントを入れてしまう場合、「見た目」と「振る舞い」の実装は、特に意識して分離しないと保守性が下がってしまいます。
アプリケーションのロジック（振る舞い）に関しては、必ず `hooks` ディレクトリ内にカスタムフックとして実装する、といったルールを決めておくとよいでしょう。

### パターン(2) Presentational/Container components のディレクトリを分ける

- __`components/`__ ... 「見た目」を定義するコンポーネント (Presentational Components)
- __`containers/`__ ... 「振る舞い」を定義するコンポーネント (Container Components)
- __`pages/`__ ... ページコンポーネント

「振る舞い」を定義する Container Components 用に、__`containers`__ というディレクトリを別に用意する方法です。
例えば、`BookList` というコンポーネントを作る場合、「見た目」を定義するコンポーネントとして `components/BookList.tsx` を作成し、そこにデータを詰める「振る舞い」を定義するコンポーネントとして `container/BookList.tsx` を作成します。
別のコンポーネントからこの `BookList` を使用するときは、通常 `container/BookList` の方を参照することになります。

コンポーネントが増えてきたときは、`components` や `containers` の下を、Atomic Design で階層化するのもいいでしょう。

- `components/`
    - `atoms/`
    - `molecules/`
    - `organisms/`
- `containers/`
    - `atoms/`
    - `molecules/`
    - `organisms/`

アプリ特有のカテゴリとして、`button` や `card` といったディレクトリを作るのもいいと思います。
アプリの規模が小さいうちは、こちらの方が直感的でわかりやすいかもしれません。

- `components/`
    - `button/`
    - `card/`
- `containers/`
    - `button/`
    - `card/`

これらのやり方の利点は、表示だけを担う UI パーツが全て `components` に集まるところです。
UI カタログみたいなものを作りたいときは、`components` 以下のコンポーネントだけを意識すれば済みます。
欠点としては、「見た目」と「振る舞い」の定義が異なるディレクトリに散らばってしまうので、若干対応付けがしにくくなるところでしょうか。

ちなみに、このディレクトリ名（`components` と `containers`）はすごく分かりにくいと感じるのは私だけでしょうか・・・？
どちらもコンポーネントなので、どっちがどっち？ってなる (^^;

もっと明確に、次のような階層にしてしまうことも考えられますが、これはちょっとネストしすぎかな。。。

- components
    - presentation
    - container

### パターン(3) Presentational/Container components をセットで置く

「見た目」を定義するコンポーネントと、「振る舞い」を定義するコンポーネントを、同じディレクトリに入れてしまう方法です。
例えば、`BookList` コンポーネントであれば次のような感じ。

- `components/`
    - `BookList.tsx` （Presentaional components)
    - `BookListContainer.tsx` (Container components)

上記のようなファイル名にすると、実際にこのコンポーネントを参照するときに `BookListContainer` の方を使うことになるので、次のように Container components の方をシンプルな名前にした方がいいかもしれません。

- `components/`
    - `BookList.tsx` (Container components)
    - `BookListPresenter.tsx` (Presentational components)

特殊なやり方として、`BookList` コンポーネント用にディレクトリを掘ってしまう方法もあります。

- `components/`
    - `BookList/`
        - `index.tsx` (Container components)
        - `Presenter.tsx` (Presentational components)

`Presenter.tsx` は、もっと具体的に `BookListPresenter.tsx` にしてもいいかもしれません。
他のコンポーネントから参照するときは、これまで通り `components/BookList` というパスでインポートできます（`index.tsx` を読み込んでくれます）。
もちろん、「振る舞い」の定義が必要ないコンポーネントの場合は、わざわざディレクトリを掘らずに、`components` ディレクトリに直接 Presentation components を配置してしまえば大丈夫です。

これらのやり方においても、コンポーネントが増えてきたときに `components/` 以下を Atomic Desigin などで階層化するのは自由です。


README.md にディレクトリ構成を書いておく
----

どのようなディレクトリ構成を採用するにせよ、プロジェクトの `README.md` に、__`src` ディレクトリ以下の構成__ と、そこにどのようなファイルを置くべきかの指針を書いておきましょう。
チーム内で何らかのルールが守られてさえいれば、後から配置ルールを変更することになっても、それほど苦労せずに対応できるはずです。

### README.md の記述例（抜粋）

```md
### src ディレクトリ以下の構成

- __`components/`__ -- Presentational components
  - 「見た目」を定義する UI コンポーネント (`.tsx`) を配置します。
  - ロジックを持たず、渡された `props` を純粋に描画するコンポーネントです。
  - 何らかのロジック（フックによるデータフェッチや状態管理）を持つコンポーネントは、ここではなく `containers` ディレクトリに配置してください。
  - このディレクトリ以下は、さらにカテゴリ別に階層化することができます。
- __`containers/`__ -- Container components
  - 「振る舞い」を定義するコンポーネント (`.tsx`) を配置します。
  - フック関数などによるデータフェッチ、状態の保存・変更などのロジックを含み、最終的に表示すべきデータを別のコンポーネントの `props` オブジェクトとして渡します。
  - このディレクトリ以下は、さらにカテゴリ別に階層化することができます。
- __`hooks/`__ -- Custom hooks
  - React のカスタムフックを配置します（例: `useAuth.ts`）。
  - アプリケーションのロジックはコンポーネント内には記述せず、できるだけフック関数として実装します。
  - 基本的に、フック関数は `comtainers` ディレクトリ以下のコンポーネントや、別のフック関数から呼び出されます。
- __`pages/`__ -- Page components
  - Next.js のページコンポーネント (`.tsx`)、および、`_app.tsx` と `_document.tsx` を配置します。
  - Web サイトアクセス時の URL パスに応じたファイル (`xxx.tsx`) が読み込まれます。
  - Atomic Design における Templates (Pages) に相当します。
- __`types/`__ -- Type definitions
  - 複数ファイルで共有する TypeScript 型定義を配置します。
  - `index.ts` から `export` した型は、`import { UserInfo } from '../types'` のようにインポートできます。
- __`utils/`__ -- Other utilities
  - 上記いずれにも当てはまらないユーティリティ（コンフィグ、データフェッチ関数など）を配置します。
```

コンポーネントを作るときに `components` に入れるか `containers` に入れるかで迷ったら、とりあえず `containers` の方に入れておいて、後ほど「見た目」の定義だけを切り出して `components` に入れるとよいです。

「振る舞い」の定義をフックに切り出すだけにとどめる場合（Container components を作らない）場合は、`components` ディレクトリ以下のコンポーネントで「振る舞い」を実装しないように記述しておくとよいです。
上記の説明のうち、`containers` の項目を削除して、次のように書き換えます。

```md
- __`components/`__ -- UI components
  - 「見た目」を定義する UI コンポーネント (`.tsx`) を配置します。
  - アプリケーションのロジック（データフェッチや状態管理）は、ここではなく `hooks` ディレクトリ内にカスタムフックとして実装してください。
  - このディレクトリ以下は、さらにカテゴリ別に階層化することができます。
- __`hooks/`__ -- Custom hooks
  - React のカスタムフックを配置します（例: `useAuth.ts`）。
  - アプリケーションのロジックはコンポーネント内には記述せず、できるだけフック関数として実装します。
  - 基本的に、フック関数は `components` ディレクトリ以下のコンポーネントや、別のフック関数から呼び出されます。
```

