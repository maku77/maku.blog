---
title: "Next.js の Image コンポーネントで画像を表示する (next/image)"
linkTitle: "Image コンポーネントで画像を表示する (next/image)"
url: "/p/fw7gpx7"
date: "2021-05-06"
lastmod: "2021-11-28"
tags: ["Next.js"]
---

Image コンポーネントの特徴
----

Next.js が提供している [Image コンポーネント (`next/image`)](https://nextjs.org/docs/api-reference/next/image) を使用すると、`image` 要素をそのまま配置するのに比べて次のような恩恵を受けられます。

- 遅延ロード (Lazy loading)
  - Web ブラウザでその画像がビューポート内（画面内）に入って来たときに初めてダウンロードされるようになります。大きなページの末尾部分に配置された画像が、無駄にダウンロードされてしまうのを防ぐことができます。
- 画像の最適化
  - アクセスしてきたクライアントに応じて画像ファイルを最適化して配信します。例えば、圧縮効率のよい WebP フォーマットなどに変換してくれます。リクエスト時にサーバーサイドでオンデマンドで最適化するため、`Image` コンポーネントを使うことでビルド時間が伸びてしまうことはありません。外部サーバーの画像を間接的に表示する場合も最適化できます。
- レスポンシブ
  - 画面サイズに応じたレスポンシブ表示 (CSS) がデフォルトで行われます。

画像最適化に関しては、Next.js サーバー上でホスティングしているときしか動作しないといった制約がありますが、遅延ローディングがデフォルトで有効になるのは便利です。


Image コンポーネントの基本的な使い方
----

次のサンプルコンポーネントでは、`Image` コンポーネントを使って __`/public/images/avatar.png`__ ファイルを表示しています。

{{< code lang="tsx" title="components/Avatar.tsx" >}}
import Image from 'next/image'

export const Avatar: React.FC = () => (
  <Image src="/images/avatar.png" width={64} height={64} alt="My avatar" />
)
{{< /code >}}

{{% note title="public ディレクトリ" %}}
`public` ディレクトリ以下に配置したファイルは、Web サイトへのアクセス時には、ルートパス (`/`) からの相対パスで参照できるようになります。
`public` ディレクトリは、必ず Next.js プロジェクトのルートディレクトリに配置する必要があります。
`.ts`、`.tsx` ファイルは `src` ディレクトリ以下にも配置できますが、`public` ディレクトリは必ずルートに置かなければいけません。
{{% /note %}}

`width` や `height` プロパティを明示しておくと、画像のロード前にそのサイズの枠が確保されることになります。
これは、[CLS (Cumulative Layout Shift)](https://web.dev/cls/) を発生させないために重要です。
表示サイズがあらかじめ決まっていないと、画像ロード後にレイアウトがガクッとずれたりして UX が低下します。
Google によるサイト評価でも CLS スコアは重要視されています。


Image コンポーネントによる画像の拡大・縮小
----

### 指定したサイズに拡大・縮小 (width, height)

{{< image border="true" w="300" src="img-001.png" >}}

{{< code lang="jsx" >}}
<Image src="/images/sample.png" width={300} height={150} />
{{< /code >}}

一番基本的な使い方です。
`Image` コンポーネント内に表示される画像は、デフォルトで `Image` コンポーネント自体のサイズ、つまり、`width`、`height` プロパティで指定したサイズに拡大・縮小されて表示されます。
画像ファイルのアスペクト比を考慮して指定しないと、上の例のように変形して表示されてしまうので注意してください。

ちなみに、`Image` コンポーネントが出力する `img` 要素には、`max-width: 100%` という CSS スタイルが設定されているため、`width` プロパティで指定する数値が親コンポーネントの横幅を超えていてもうまく収まるように表示してくれます。

### アスペクト比を保って大きく表示 (objectFit=contain)

{{< image border="true" w="300" src="img-002.png" >}}

`Image` コンポーネントの [objectFit プロパティ](https://nextjs.org/docs/api-reference/next/image#objectfit) に __`contain`__ を指定すると、画像のアスペクト比を保ちながら、縦か横にできるだけ大きく表示してくれます。

{{< code lang="jsx" >}}
<Image src="/images/sample.png" width={300} height={150} objectFit="contain" />
{{< /code >}}

この `objectFit` プロパティは、HTML の `img` 要素用の CSS プロパティである、[object-fit プロパティ](https://developer.mozilla.org/en-US/docs/Web/CSS/object-fit) そのものです。
このプロパティのデフォルト値は `fill` なので、これを指定しない場合は、画像が `width`、`height` プロパティで指定された通りに引き伸ばして表示されるというわけです。

### 親コンポーネントのサイズに連動させる (layout=fill)

{{< image border="true" w="300" src="img-002.png" >}}

{{< code lang="jsx" >}}
<div style={{ position: 'relative', width: '100%', height: 150 }}>
  <Image src="/images/sample.png" layout="fill" objectFit="contain" />
</div>
{{< /code >}}

`Image` コンポーネントの `width`、`height` プロパティを指定する代わりに、[layout プロパティ](https://nextjs.org/docs/api-reference/next/image#layout) を __`fill`__ に設定すると、親要素のサイズが `Image` コンポーネントのサイズとして使われます。
上記の例でいうと、上の階層に配置した `div` 要素のサイズが、`Image` 要素のサイズになります。
ここでは、`div` 要素自体の横幅も、さらにその親の要素の横幅に合わせるように `100%` 指定しています。

`Image` コンポーネントのサイズを `layout="fill"` で親要素のサイズに合わせるときは、`objectFit="contain"` も一緒に指定して、アスペクト比を保って拡縮表示するとよいです。
これを指定しないと、デフォルトの `objectFit="fill"` が使われて、画像が親要素の形に変形して表示されてしまいます。


（応用）環境によって画像ファイルのダウンロード先を変える (NODE_ENV)
----

開発環境では `/public` ディレクトリ以下の画像リソースを参照し、本番環境では別のサーバー上の画像リソースを参照する、といったことをやりたい場合は、例えば __`NODE_ENV`__ 環境変数の値で条件分岐させます。
`process.env.NODE_ENV` の値は、実行環境によって次のように変化します。

| NODE_ENV の値 | 実行環境の例 |
| ---- | ---- |
| __`production`__ | 本番サーバー (`next start`) |
| __`development`__ | 開発サーバー (`next dev`) |
| __`test`__ | テスト (`jest`) |

次の関数は、実行環境に応じて画像ファイルの URL を作り分けます。

{{< code lang="tsx" >}}
export function getImageSrc(filename: string): string {
  if (process.env.NODE_ENV === 'production') {
    return `http://img.example.com/img/${filename}`
  } else {
    return `/img/${filename}`
  }
}
{{< /code >}}

あとは、次のように `Image` コンポーネントなどで使います。

{{< code lang="tsx" >}}
// import Image from 'next/image'
<Image src={getImageSrc('avatar.png')} width={64} height={64} />
{{< /code >}}

ちなみに、Next.js の `Image` コンポーネントの `src` プロパティで、外部サーバー上の画像ファイルを指定する場合は、__`next.config.js`__ でドメイン名を指定しておく必要があります。

{{< code lang="js" title="next.config.js" >}}
module.exports = {
  images: {
    domains: ['img.example.com'],
  },
}
{{< /code >}}

他にも、`next.config.js` ファイル内で __`assetPrefix`__ の値を切り替えることで、画像ファイルの取得先を切り替える方法もあります。

- 参考: [next.config.js: CDN Support with Asset Prefix | Next.js](https://nextjs.org/docs/api-reference/next.config.js/cdn-support-with-asset-prefix)

{{< code lang="js" title="next.config.js" hl_lines="5" >}}
const isProd = process.env.NODE_ENV === 'production'

module.exports = {
  // Use the CDN in production and localhost for development.
  assetPrefix: isProd ? 'https://img.example.com' : '',

  images: {
    domains: ['img.example.com'],
  },
}
{{< /code >}}

これであれば、`Image` コンポーネントの `src` プロパティは次のようにシンプルに記述できます。

{{< code lang="tsx" >}}
<Image src='/avatar.png' width={64} height={64} />
{{< /code >}}

単純に URL のプレフィックス部分だけ切り替えられればよいのであれば、こちらの方法を使った方が簡単かもしれません。
ただし、`assetPrefix` の設定は、`next/image` が提供する `Image` コンポーネントを使った場合にしか効果がないことに注意してください（生の `img` コンポーネントをそのまま使った場合はプレフィックスが反映されません）。
また、`assetPrefix` は全てのリソースの参照パスを切り替えてしまうので、特定のリソースのみ別のサーバーにおきたい場合などは、前述の `getImageSrc` 関数のような独自の関数を用意したり、`Image` や `img` をラップするコンポーネントを作成する必要があります。


（応用）src で指定した画像が見つからなかったときにプレースホルダー画像を表示する
----

Next.js の `Image` コンポーネントに限らず、通常の HTML の `img` 要素でも同じなのですが、`src` 属性で指定した画像ファイルが見つからなかったときは、{{< image-inline src="img-003.png" >}} のようなアイコンが表示されます（どう表示されるかは Web ブラウザによって異なります）。

このようなケースでは、`Image` コンポーネントの __`onError`__ prop に設定したコールバック関数が呼ばれるようになっているので（`img` の場合は `onerror`）、この中で代わりの画像の URL をセットするということができます。

次の例では、指定した URL の画像が存在しなかったときに、プレースホルダー画像を表示するように `src` の URL を変更しています。

```ts
function renderImage(title: string, url: string): JSX.Element {
  return (
    <Image
      src={url}
      unoptimized={true}
      layout="fixed"
      width={300}
      height={200}
      title={title}
      alt={title}
      onError={(e) => {
        e.currentTarget.src = `https://placehold.jp/32/003060/e0e0e0/300x200.png?text=${title}`
      }}
    />
  )
}

const MyPage: NextPage= () => {
  return (
    <>
      {renderImage('画像タイトル', 'https://example.com/404.png')}
    </>
  )
}

export default MyPage
```

{{< image title="表示されるプレースホルダー画像" src="https://placehold.jp/32/003060/e0e0e0/300x200.png?text=画像タイトル" >}}

ここでは、プレースホルダー画像を自動生成するために、[Placehold.jp](https://placehold.jp) を使わせてもらっています。
プレースホルダー生成サービスとしては、他にも [Placeholder.com](https://placeholder.com) などがありますが、こちらは日本語テキストの表示に対応していないようです。
次のような URL でアクセスするだけで、指定したサイズ、カラー、テキストのプレースホルダー画像を返してくれます。

- https://placehold.jp/32/003060/e0e0e0/300x200.png?text=Title
- https://via.placeholder.com/300x200/003060/e0e0e0.png?text=Title

こういうサービスはとってもありがたいです。感謝 ٩(๑❛ᴗ❛๑)۶

外部サービスに頼りたくないのであれば、次のようにクライアントサイド JavaScript でプレースホルダー画像を生成してしまう方法もあります。

```ts
/**
 * プレースホルダー画像を動的に生成して、img 要素の src にセットするための URL を返します。
 * `document` オブジェクトを参照するため、クライアントサイド JS でのみ実行可能です。
 */
function createPlaceHolderImage(
  width: number,
  height: number,
  text: string
): string {
  // Canvas 要素を動的に生成
  const canvasElem = document.createElement('canvas')
  canvasElem.width = width
  canvasElem.height = height
  const ctx = canvasElem.getContext('2d') as CanvasRenderingContext2D

  // 背景を描画
  ctx.fillStyle = '#003060'
  ctx.fillRect(0, 0, width, height)

  // テキストを描画（中央寄せ）
  ctx.fillStyle = 'white'
  ctx.font = `bold 24px Arial, meiryo, sans-serif`
  const textWidth = ctx.measureText(text).width // 左右の中央寄せ用
  ctx.textBaseline = 'middle' // 上下の中央寄せ用
  ctx.fillText(text, (width - textWidth) / 2, height / 2, width)

  // 描画内容をデータ化して URL にして返す
  return canvasElem.toDataURL()
}
```

関数内で `document` オブジェクトを参照しているため、ビルド時に実行されないように注意してください。
次のように、クライアントサイド JS でしか実行されないようになっていれば大丈夫です（`useEffect` の中で呼び出すのも大丈夫です）。

```tsx
<Image
  ...
  onError={(e) => {
    e.currentTarget.src = createPlaceHolderImage(300, 200, '画像タイトル')
  }}
/>
```

