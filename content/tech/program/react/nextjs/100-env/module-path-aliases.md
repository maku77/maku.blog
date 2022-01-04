---
title: "Next.js で src からの相対パスで import できるようにする (tsconfig.json)"
url: "/p/qgvamzc"
date: "2021-08-26"
tags: ["Next.js"]
---

{{% private %}}
- [Advanced Features: Absolute Imports and Module Path Aliases | Next.js](https://nextjs.org/docs/advanced-features/module-path-aliases)
{{% /private %}}

何をするか？
----

Next.js (TypeScript) のプロジェクトで、`src` ディレクトリ以下の階層構造が深くなってくると、別のディレクトリのモジュールをインポートするときの相対パスがわかりにくくなってきます。

{{< code lang="ts" >}}
import { sleep } from '../../../../../utils/timeUtil'
{{< /code >}}

このような場合は、次に説明するような __`tsconfig.json`__ の設定をしておくと、シンプルなパスでインポートできるようになります。


compilerOptions.paths プロパティ (module alias)
----

`tsconfig.json` の __`compilerOptions.paths`__ プロパティ（と __`baseUrl`__ プロパティ）を設定すると、特定のパスに配置されたモジュール（.ts ファイル）を、エイリアス名（ここでは `@`）を使って参照できるようになります。

{{< code lang="js" title="tsconfig.json" >}}
{
  "compilerOptions": {
    // ...
    "baseUrl": ".",
    "paths": {
      "@/*": ["./src/*"]
    }
  }
}
{{< /code >}}

上記のように設定すると、プロジェクトルートの `src` ディレクトリ以下のモジュールを、どの階層にあるコードからでも、__`@/moduleName`__ という名前で参照できるようになります。
例えば、`src/utils/timeUtil.ts` というモジュールをインポートするとき、これまでは、次のようにカレントディレクトリからの相対パスで指定しなければいけなかったところを、

{{< code lang="ts" >}}
import { sleep } from '../../../../../utils/timeUtil'
{{< /code >}}

次のように `src` ディレクトリを示すエイリアス名 `@` を使って指定できるようになります。

{{< code lang="ts" >}}
import { sleep } from '@/utils/timeUtil'
{{< /code >}}

{{% note %}}
上記のようなエイリアス名を使った場合でも正しくモジュールを参照できるのは、Next.js のビルドシステムが JavaScript へのトランスパイル時に本来のモジュールのパスに変換してくれるおかげです（おそらく webpack などによるバンドル時に処理されています）。
このあたりにも、Next.js のゼロコンフィグ思想が垣間見えます。
{{% /note %}}

ちなみに、一緒に設定している `"baseUrl": "."` というオプションは、本来は次のように `./` プレフィックスを付けない名前でインポートしたときに、プロジェクトルートからの相対パスでモジュールを検索してくれるようにするためのオプションです。

{{< code lang="ts" >}}
import { sleep } from 'src/utils/timeUtil'
{{< /code >}}

この `baseUrl` の設定は `paths` の設定とは関係なさそうに見えますが、一緒に設定しておかないとビルド時のパス解決に失敗するので、必ず `baseUrl` と `paths` の両方の設定が必要です。
`baseUrl` を設定すると、3rd パーティ製の npm モジュールとの見分けがつきにくくなるという弊害があるため、このあたりは将来的に改善されそうな気がします。

