---
title: "Astro と Svelte を使ってみた所感（Web サイト作るときに何を使うか）"
url: "p/zays9nw/"
date: "2023-08-15"
tags: ["Astro", "Svelte"]
---

Next.js 以外の選択肢
----

2023 年現在、ある程度の規模の Web アプリを作れと言われたら、間違いなく Next.js (React.js) を使うのですが、小規模でほとんど静的な Web サイトを作れと言われたらちょっと迷ってしまうので、[Astro](https://astro.build/) と [Svelte](https://svelte.dev/) を少しだけ触ってみました。
もちろん、HTML や CSS を生でガリガリ書いて Web サイトを作ることもできますが、これらのフレームワークを使うことで、コンポーネント単位で Web ページを組み上げていくことができるのでコードの見通しがよくなります。


Astro vs. Svelte
----

感覚としては、__完全に静的な Web サイトを作るのであれば Astro がとてもシンプル__ でよさそうです。
ただし、フォームなどの入力要素が必要な場合は、__データバインドを独自構文でサポートしている Svelte__ の方が便利かもしれません。
Svelte は簡単に使えるというけれど、独自のテンプレート構文などは若干 Vue.js の匂いがするので、そこを嫌う人は多そうです。
一方、Astro であれば、ほとんどドキュメントすら読まなくても理解できるくらい簡単です（React.js 触ったことがあれば即理解できるはず）。

Astro も Svelte も `npm` でビルドツールをインストールしますが、Astro であれば `package.json` の `dependencies` セクションが `astro` モジュールだけになるくらいシンプルです。

{{< code lang="js" title="package.json" >}}
{
  // ...
  "dependencies": {
    "astro": "^2.10.7"
  }
}
{{< /code >}}

Astro モジュール自体が TypeScript や Vite を内包してくれているので、別途 TypeScript や Vite をインストールする必要はないし、[VS Code の Astro 拡張](https://marketplace.visualstudio.com/items?itemName=astro-build.astro-vscode) をポチッとインストールするだけで、Prettier による自動整形や TypeScript の型チェックがかかるようになります。
Svelte の場合は、Svelte 本体の他にも、SvelteKit や Vite や TypeScript などが `dependencies` にごちゃっと入ってきてしまうので、シンプルさに欠けます（こちらを好む人もいるのかもですが）。
まぁどちらも `npm create` で雛形生成すれば、ほぼゼロコンフィグで使えるんですが、Hello World レベルのコードで依存ファイルが多くなるのは避けたいところです。

というわけで、今後は簡単な Web ページを作るときは、おもむろに __`npm create astro@latest`__ を実行して雛形を作るのがいいかなと思ってます。


Astro ってどんなの？
----

Astro はビルドツールです。
基本的に、__生成される成果物は静的な HTML と CSS だけ__ であり、JavaScript ベースの成果物になる React.js とは対照的です。
もちろん動的なコンポーネントも作れますし、React や Svelte で作ったコンポーネントをインポートすることもできたりします。
でもネットワーク通信しながら動的に動く Web アプリを作るのであれば、Next.js を使った方がよいです。
Astro の公式ドキュメントでも、「__よりアプリケーションに特化した選択肢としては、Next.js をチェックしてみてください__」と言っています。
潔い！
惚れてまうやろ。

Astro を使って Web サイトを生成する場合、従来の HTML、CSS、JS ファイルの代わりに `.astro` という拡張子のコンポーネントファイルを作ります。
といっても、HTML コードをそのまま書くだけでもビルドできるので、従来の HTML コードと同じ書き方で初めて、徐々に Astro の機能を利用した書き方に移行していくということができます。

{{< code lang="html" title="src/pages/index.astro" >}}
<!DOCTYPE html>
<meta charset="UTF-8" />
<title>Astro Test</title>
<h1>Astro Test</h1>
{{< /code >}}

上記のように `src/pages` 以下にページを作成したら、__`npm run dev`__ で開発サーバーを起動して表示を確認できます。
Vercel や GitHub Pages にデプロイするときは、__`npm run build`__ でデプロイ用のファイル群（`dist` ディレクトリ）を作成します。
Vercel であれば、GitHub リポジトリを選択するだけでデプロイできちゃいますが。

他のコンポーネントをインポートしたいときや、JavaScript で外部データをフェッチしたいときは、Markdown のフロントマターのような部分に JavaScript を記述します (公式サイトでは、frontmatter script とか component script とか呼んでいます)。
ここに記述した JavaScript は、あくまでビルド時に実行されるので、最終的な成果物には JavaScript コードは残りません（逆に残したいときは、従来の `<script>` 要素を使います）。
次の例では、`Hello` コンポーネントをインポートして使っています。
HTML 部分では、ほぼ JSX と同じ構文が使えます（例: `{name}` で `name` 変数を参照できます）。

{{< code lang="tsx" title="src/pages/index.astro" >}}
---
// 自作した Hello コンポーネントをインポートする
import Hello from "../components/Hello.astro";

// Web API でユーザー情報を取得 （await をそのまま使える！）
const users = await fetch("https://jsonplaceholder.typicode.com/users").then(r => r.json());
---

<!DOCTYPE html>
<meta charset="UTF-8">
<title>Astro Test</title>
<h1>Astro Test</h1>
{users.map((user) => <Hello name={user.name} />)}
{{< /code >}}

`Hello` コンポーネントも同様に `.astro` 拡張子のファイルとして作り、`src/pages` 以外の任意のディレクトリに配置します（慣例的には `src/components`）。
下記は、`name` prop を受け取るだけの簡単な `Hello` コンポーネントの実装例です。
末尾に `<style>` 要素を配置することで、このコンポーネント内でのみ有効なスタイルを設定できます。

{{< code lang="tsx" title="src/components/Hello.astro" >}}
---
export interface Props {
  name: string;
}
const { name } = Astro.props as Props;
---

<p class="message">Hello, {name}!</p>

<style>
  .message {
    color: red;
  }
</style>
{{< /code >}}

これだけでも、Astro を使うメリットが感じられると思います。
`.astro` という再利用可能なコンポーネント単位で開発できるようになるので、素の HTML や CSS ファイルを作成するより効率がよいです。
というか、素の HTML ファイルだけだと、ビルド時に外部データのフェッチなどはできませんね。


Web サイト生成フレームワークの使い分け
----

Web サイトを作るためのフレームワークはいっぱいありますが、まくの場合、今のところ次のような使い分けがしっくりきています。

| 作りたいもの | フレームワーク | デプロイ先 |
| ---- | ---- | ---- |
| Markdown ベースのブログ | __Hugo__ | GitHub Pages |
| 重量級の Web アプリ | __Next.js__ | Vercel / Cloudflare |
| 静的な Web サイト | __Astro__ | Vercel / Cloudflare |
| ちょっとインタラクティブな Web サイト | __Svelte__ | Vercel / Cloudflare |

月 10 万アクセス程度の Web サイトであれば、この構成で無料で運用できるし、デプロイ設定も一瞬で終わるのでお手軽です。
また、ここで挙げたフレームワークはいずれもパフォーマンスに優れています。

最後に、YouTube の Astro Crash Course が分かりやすかったので貼っておきます。
従来の HTML + CSS で構成された Web サイトを、段階的に `.astro` コンポーネントに分解していく過程を学べます。

{{< youtube Oi9z5gfIHJs >}}

