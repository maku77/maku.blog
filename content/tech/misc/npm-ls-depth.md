---
title: "npm でインストール済みのモジュールを簡潔にリスト表示する"
url: "/p/nuvbg6k"
date: "2020-05-21"
tags: ["npm"]
---

__`npm list (ls)`__ コマンドは NPM (Node Package Manager) でインストールしたパッケージの一覧を表示するコマンドですが、デフォルトでは依存モジュールをすべてツリー構造で表示するので、出力が大量になります。

{{< code title="インストールパッケージの一覧を表示" >}}
$ npm ls

myapp@1.0.0 D:\y\gitwork\myapp
+-- textlint@11.6.3
| +-- @textlint/ast-node-types@4.2.5
| +-- @textlint/ast-traverse@2.1.7
| | `-- @textlint/ast-node-types@4.2.5 deduped
| +-- @textlint/feature-flag@3.1.6
| | `-- map-like@2.0.0 deduped
| +-- @textlint/fixer-formatter@3.1.13
| | +-- @textlint/module-interop@1.0.2 deduped
| | +-- @textlint/types@1.3.1 deduped

... なが～い出力 ...
{{< /code >}}

次のように __`--depth`__ オプションを付けると、トップレベルのモジュール名だけを簡潔に表示することができます。

{{< code >}}
$ npm ls --depth=0

myapp@1.0.0 D:\y\gitwork\myapp
+-- textlint@11.6.3
+-- textlint-rule-preset-ja-technical-writing@3.1.3
`-- textlint-rule-web-plus-db@1.1.5
{{< /code >}}

グローバルにインストールしたモジュールを表示するときも同様です。

{{< code >}}
$ npm ls -g --depth=0
{{< /code >}}

