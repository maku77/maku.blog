---
title: "TypeScriptの環境: tsconfig.json の基本"
linkTitle: "tsconfig.json の基本"
url: "p/27m3brm/"
date: "2019-10-01"
tags: ["JavaScript", "TypeScript"]
weight: 101
---

{{% private %}}
- https://www.typescriptlang.org/docs/handbook/tsconfig-json.html
{{% /private %}}


tsconfig.json があれば TypeScript プロジェクト
----

**`tsconfig.json`** は TypeScript の設定ファイルであり、このファイルが置かれたディレクトリが TypeScript プロジェクトのルートディレクトリだとみなされます。
`tsconfig.json` の記述内容は空っぽ（`{}` の2文字）でも正しい設定ファイルであり、その場合は、すべてデフォルトの設定値で動作することになります。

TypeScript のトランスパイラである **`tsc`** コマンドを実行すると、カレントディレクトリにある `tsconfig.json` が読み込まれてトランスパイラの動作設定が行われます。
`tsconfig.json` が見つからない場合は、親ディレクトリを上りながらファイルを探します。
つまり、`tsc` コマンドは TypeScript のプロジェクト内であれば、どのディレクトリからでも実行できます。


変換対象とするファイルを指定する (files/include/exclude)
----

最も大切な設定は、TypeScript のトランスパイラがどのファイルを変換対象とみなすかの設定です。
入力ファイルの指定は、設定ファイルの最上位プロパティとして指定する `files`、`include`、`exclude` プロパティを使って行います。

### ファイル名を 1 つずつ指定する (files)

{{< code lang="json" title="tsconfig.json" >}}
{
  "files": [
    "index.ts",
    "module1.ts",
    "module2.ts"
  ]
}
{{< /code >}}

**`files`** プロパティを使って、変換対象の TypeScript ファイルを 1 ファイルずつ指定することができます。

### ファイル名のパターン（グロブ）で指定する (include)

{{< code lang="json" title="tsconfig.json" >}}
{
  "includes": [
    "src/**/*"
  ],
  "exclude": [
    "node_modules",
    "**/*.spec.ts"
  ]
}
{{< /code >}}

**`include`** プロパティを使用すると、ファイルグロブを使って変換対象とするファイルを指定できます。
ファイルグロブは、下記のようなワイルドカードを使ってファイル名をパターン指定する仕組みです。

- `**` は 0 文字以上の任意の文字にマッチします（ディレクトリセパレータも含みます）
- `*` は 0 文字以上の任意の文字にマッチします（ディレクトリセパレータは含みません）
- `?` は 1 文字の任意の文字にマッチします

つまり、上記の `src/**/*` という表現は、`src` ディレクトリ以下のすべてのファイルにマッチします。
ただし、パスの末尾が `*` で終わっている場合、**デフォルトでは TypeScript の拡張子 (`.ts` / `.tsx` / `.d.ts`) を持つファイルにだけマッチする**ようになっています。

{{< note >}}
末尾のワイルドカード <code>*</code> を JavaScript ファイルの拡張子 (.js /.jsx) にもマッチさせたい場合は、明示的に拡張子を指定するか、<code>compilerOptions.allowJs</code> オプションを true に設定します。
{{< /note >}}

**`exclude`** プロパティには、`include` の対象外とするファイル（フォルダ）のパターンを指定します。
つまり、`include` のパターンに一致したファイルであっても、`exclude` のパターンに一致するファイルは変換対象とはみなされません。
一方で、`files` で指定されているファイルは必ず変換対象になることに注意してください。
`exclude` のパターンに一致しても除外されません。

`exclude` プロパティを省略すると、デフォルトで `node_modules` や `bower_components` などのディレクトリが除外対象になります（上記の例では、他のファイルパターンも指定しているので、`node_modules` を明示的に指定しています）。


### デフォルトの振る舞い

`files` プロパティと `include` プロパティのいずれも指定されなかった場合は、**デフォルトでプロジェクト内のすべての TypeScript ファイルが変換対象**となります。
テンポラリのつもりで作成した TypeScript ファイルも変換対象になったりしてしまうので、できれば `include` プロパティで `src/**/*` のようなディレクトリを指定することをお勧めします。

### ファイル依存の解決

TypeScript コード間の依存関係も自動的に考慮されます。
例えば、`A.ts` がコード内で `B.ts` を参照している場合は、`B.ts` は自動的にトランスパイラによる変換対象になります。
`tsconfig.json` の `files` プロパティや `include` プロパティで `B.ts` ファイルが指定されている必要はありません。

同一ディレクトリに `A.ts` と `A.js` というファイルが存在する場合、`A.ts` が入力ファイル、`A.js` が出力ファイルとみなされます。
`tsc` コマンドを実行するたびに `A.js` の内容は上書きされます。


コンパイラオプションの設定 (compilerOptions)
----

`tsc` コンパイラ自体の振る舞いは **`compilerOptions`** プロパティで指定します。
下記サイトに記述されているように、たくさんのコンパイラオプションが用意されています。

- [Compiler Options | Type Script](https://www.typescriptlang.org/docs/handbook/compiler-options.html)

オプションという名の通り、これらの設定は省略してもデフォルトの振る舞いでちゃんと動作してくれます。
少しずつオプションの意味を理解しながらカスタマイズしていけばよいでしょう。

下記は、`tsc --init` コマンドを実行したときに自動生成される `tsconfig.json` の設定内容です（実際には説明コメントがたくさん記述されています）。

{{< code lang="json" title="tsconfig.json" >}}
{
  "compilerOptions": {
    "target": "es5",
    "module": "commonjs",
    "strict": true,
    "esModuleInterop": true
  }
}
{{< /code >}}

`compilerOptions.target` プロパティでは、JavaScript に変換するときにどの ECMAScript バージョンに準拠したコードで出力するかを指定します。
デフォルトでは `ES3` バージョンのコードで出力されますが、若干古すぎるので、**`ES2015 (ES6)` あたりを指定しておくのがよい**でしょう（ES2015 は JavaScript に正式にクラスが導入されたバージョンです）。
Web アプリであれば、どれだけ古い Web ブラウザに対応するかによって決めることになりますが、最新の Node.js 上で動かすようなサービスであれば、**`ES2020`** などで攻めてみるのもよいでしょう。
一般的には、新しい構文を使ったコードの方が効率的に動作すると考えられます。

他にも、JavaScript コードを入力ファイルとみなす `allowJs` オプションや、出力先のディレクトリを設定する `outDir` などがあります。
これらは、既存の JavaScript プロジェクトに TypeScript を適用する際に便利なオプションです。
このあたりは、下記の記事で詳しく紹介します。

- [既存の JavaScript プロジェクトを TypeScript に乗り換える](/p/3eccb2t/)

