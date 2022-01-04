---
title: "DRAFT: Electron"
url: "/p/g2doz9j"
date: "2020-11-14"
tags: ["Electron"]
draft: true
---

Electron Packager
----

### インストールと設定

{{< code >}}
$ npm install electron-packager --save-dev
{{< /code >}}

{{< code lang="js" title="package.json" >}}
"scripts": {
  "build": "electron-packager .",
  // ...
}
{{< /code >}}

デフォルトの出力はルートになっていますが、`--out=build` で切り替えられます。

パッケージ生成には、`package.json` に記述されたプロパティが参照されます。

- `productName` ... アプリ名。これが指定されていない場合は、代わりに `name` プロパティが参照されますが、`name` プロパティにはスペースや大文字を含むことができないので、`productName` プロパティを使うことが推奨されます。
- `version` ... アプリのバージョン情報。

### ヘルプ

```
$ npm electron-packager --help
```

#### アーカイブ

`electron-packager` コマンドに __`--asar`__ オプションを付けると、`app.asar` というアーカイブとして出力してくれます。
`.asar` は Electron アプリのアーカイブフォーマットです。

### パッケージングに yarn を使う

{{< code lang="js" title="package.json" >}}
"electronPackagerConfig": {
  "packageManager": "yarn"
}
{{< /code >}}


.env ファイル
----

`.env` のパスを

```js
path.join(__dirname, '..', '.env')
```

のように指定していると、`MyApp.app/Contents/Resources/app.asar/.env` というパスで探しにいってしまう。。。

`app.getPath('userData')` を使わないとだめかも。
あるいは、`.env` は基本使わない？

