---
title: "VS Code の Explorer で特定のファイルやディレクトリを非表示にする (files.exclude)"
url: "/p/raku5dn"
date: "2021-07-24"
tags: ["VS Code"]
---

VS Code のエクスプローラービューは、サイドバー上にプロジェクト内のファイル一覧を表示してくれる便利な機能ですが、編集対象ではないファイルまで表示されていると地味に邪魔だったりします。
例えば、Node.js アプリの `node_modules` ディレクトリなどは常に表示されていてもあまり役に立たないかもしれません。

{{< image w="650" border="true" src="img-001.png" title="不要なディレクトリが表示されている" >}}

このような場合は、[設定ファイル (settings.json)](/p/tfq2cnw) の __`files.exclude`__ プロパティで非表示にしたいファイルやディレクトリのパターンを指定します。

{{< code lang="js" title="settings.json" >}}
{
  // Explorer で非表示にするもの
  "files.exclude": {
    "**/.next": true,  // Next.js サーバーのキャッシュ
    "**/node_modules": true,
    "out": true  // Next.js の export 先
  },

  // その他の設定...
}
{{< /code >}}

上記のようにパターンに `out` と指定すると、トップディレクトリの `out` という名前のファイルおよびディレクトリが非表示になります。
`**/node_modules` と指定すると、任意の階層の `node_modules` という名前のファイルおよびディレクトリが非表示になります。
末尾にスラッシュ (`/`) を付けるのは何も効果がないようです。
グロブパターン（`**` など）の詳細な指定方法は、[Advanced search options](https://code.visualstudio.com/docs/editor/codebasics#_advanced-search-options) のドキュメントが参考になります。

{{< image w="650" border="true" src="img-002.png" title="指定したディレクトリが非表示になった" >}}

ちなみに、[デフォルト設定](https://code.visualstudio.com/docs/getstarted/settings#_default-settings)では次のようなファイルとディレクトリが非表示になります。

{{< code lang="js" >}}
{
  // Configure glob patterns for excluding files and folders.
  // For example, the file Explorer decides which files and folders
  // to show or hide based on this setting.
  // Refer to the `search.exclude` setting to define search specific excludes.
  "files.exclude": {
    "**/.git": true,
    "**/.svn": true,
    "**/.hg": true,
    "**/CVS": true,
    "**/.DS_Store": true
  },
  // ...
}
{{< /code >}}

