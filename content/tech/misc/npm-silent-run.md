---
title: "npm run スクリプト実行時の ERR! 出力を抑制する (npm run --silent)"
url: "/p/epm9ipy"
date: "2020-05-21"
tags: ["npm"]
---

__`npm run`__ コマンドを使うと、`package.json` に定義されているスクリプトを実行することができるのですが、そこで実行したコマンドがエラー終了（`exit 1` など）すると、`npm run` の実行自体もエラー扱いとなり、下記のようにエラー情報がたくさん出力されます。

{{< code >}}
$ npm run test

> myapp@1.0.0 test C:\myapp
> echo "Error: no test specified" && exit 1

"Error: no test specified"
npm ERR! code ELIFECYCLE
npm ERR! errno 1
npm ERR! myapp@1.0.0 test: `echo "Error: no test specified" && exit 1`
npm ERR! Exit status 1
npm ERR!
npm ERR! Failed at the myapp@1.0.0 test script.
npm ERR! This is probably not a problem with npm. There is likely additional logging output above.
npm WARN Local package.json exists, but node_modules missing, did you mean to install?

npm ERR! A complete log of this run can be found in:
npm ERR!     C:\Users\maku\AppData\Roaming\npm-cache\_logs\2020-05-21T09_29_36_426Z-debug.log
{{< /code >}}

この __`npm ERR!`__ という出力を抑制するには、次のように __`--silent`__ オプションを使用します。

{{< code >}}
$ npm run --silent test
"Error: no test specified"
{{< /code >}}

ちなみに、上記で使っている `test` スクリプトは、`npm init` で `package.json` を新規作成したときにデフォルトで作成されるスクリプトです。

{{< code title="npm init で作られる package.json の抜粋" >}}
{
  // ...
  "scripts": {
    "test": "echo \"Error: no test specified\" && exit 1"
  },
  // ...
}
{{< /code >}}

