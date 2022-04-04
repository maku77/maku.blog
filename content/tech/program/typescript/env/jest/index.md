---
title: "Jest で TypeScript コードのユニットテストを記述する"
url: "/p/9xxpe4t"
date: "2022-04-04"
tags: ["ユニットテスト", "Jest", "TypeScript"]
---

Jest とは
----

Jest は Facebook が公開した JavaScript 用のシンプルなテストフレームワークで、Node.js 環境で実行することができます。

- [Jest · 🃏 Delightful JavaScript Testing](https://jestjs.io/)

Jest は下記のような特徴を持っています。

- ゼロコンフィグで使い始められる（設定ファイルなしで実行可能）
- テストを並列実行するので高速
- コードカバレッジレポートの出力を標準搭載
- わかりやすいマッチャー表現 (expect ～ toBe、toContain など、自然な文章として読める）
- TypeScript に対応（ts-jest を利用）

Jest 用のテストコードは、次のようなコードジェネレーターでも採用されており、利用者は増え続けています（2022 年現在）。

- `create-react-app` ... React アプリのジェネレーター
- `cdk init app` ... AWS のインフラ生成コードのジェネレーター


Jest のインストール
----

Jest 本体の __`jest`__ モジュールをインストールします。
TypeScript を使用する場合は、Jest ライブラリの型情報である __`@types/jest`__ と、Jest 用の TypeScript プロセッサ ([ts-jest](https://kulshekhar.github.io/ts-jest/)) もインストールする必要があります。
これらはすべてテスト時のみ使用する NPM モジュールなので、`devDependencies` としてインストールします。

{{< code lang="console" title="Jest 本体と TypeScript 関連モジュールをインストール" >}}
### npm の場合
$ npm install --save-dev jest @types/jest ts-jest

### yarn の場合
$ yarn add --dev jest @types/jest ts-jest
{{< /code >}}


package.json の修正
----

Jest によるテストを簡単に起動できるように、`package.json` に NPM スクリプト (`test`) を定義しておきます。
あと、Jest が TypeScript のコードを理解できるように、`jest.preset` に __`ts-jest`__ を設定しておきます。
これを設定しておかないと、`import` 構文などを理解できなくてエラーになります。

{{< code lang="js" title="package.json" >}}
{
  // ...
  "scripts": {
    // ...
    "test": "jest"
  },
  "jest": {
    "preset": "ts-jest"
  }
}
{{< /code >}}

上記の `jest` プロパティの値は、__`jest.config.js`__ という別ファイルとして定義することもできます。

{{< code lang="js" title="jest.config.js（これがあればこちらが優先される）" >}}
module.exports = {
  preset: 'ts-jest',
}
{{< /code >}}

これで、次のようにユニットテストを起動できます。

```console
$ yarn test  # yarn の場合
$ npm test   # npm の場合
```

最初は何もテストコードがないので、実行しても次のようなエラーで終了するはずです（終了コード 1 はコマンドが失敗したことを表します）。

```console
$ yarn -s test
No tests found, exiting with code 1
```

テストがないときにエラーにならないようにするには、__`--passWithNoTests`__ オプションを付けて実行します。
自動ビルド環境などで、常に `yarn test` は実行しておきたい場合にお世話になるかもしれません。

```console
$ yarn -s jest --passWithNoTests
No tests found, exiting with code 0
```


テストの記述
----

ここでは、次のような計算ライブラリをテスト対象のコードとして使うことにします。

{{< code lang="ts" title="src/math.ts（テスト対象）" >}}
export function add(a: number, b: number): number {
  return a + b
}
{{< /code >}}

Jest はデフォルトで次のようなファイルを検索してテストコードとして実行します。

- 拡張子が __`.test.ts`__ (`.test.js`) のファイル
- 拡張子が __`.spec.ts`__ (`.spec.js`) のファイル
- __`__test__`__ ディレクトリ以下に配置した `.ts` (`.js`) ファイル

前述の計算ライブラリ (`math.ts`) をテストするためのテストコードを、__`math.test.ts`__ というファイル名で作成します。
ファイル名のベース部分 (`math`) はテスト対象のファイル名と合わせておくと分かりやすいでしょう。

{{< code lang="ts" title="src/math.test.ts（テストコード）" >}}
import { add } from './math'

test('add - positives', () => {
  const result = add(1, 2)
  expect(result).toBe(3)
})

test('add - negatives', () => {
  const result = add(-1, -2)
  expect(result).toBe(-3)
})
{{< /code >}}

テストコード内では、上記のように __`test(テスト名, テスト内容)`__ という形で各テストを定義します（`test` の代わりに `it` も使えます）。
計算結果の検証は、__`expect(実行結果).toBe(期待結果)`__ のように記述します。
次のようにテストを起動できます（`yarn` の出力をシンプルにするために `-s` オプションを指定しています）。

```console
$ yarn -s test
 PASS  src/math.test.ts
  √ add - positives (1 ms)
  √ add - negatives

Test Suites: 1 passed, 1 total
Tests:       2 passed, 2 total
Snapshots:   0 total
Time:        2.316 s, estimated 3 s
Ran all test suites.
```

すべてのテストが通りました！

さらに、__`--coverage`__ オプションを付けて実行することで、テストカバレッジのレポートを出力することができます。
テスト対象となったソースコードのうち何％がテストでカバーされているかを表示してくれます。

```console
$ yarn -s test --coverage
 PASS  src/math.test.ts
  √ add - positives (2 ms)
  √ add - negatives (1 ms)

----------|---------|----------|---------|---------|-------------------
File      | % Stmts | % Branch | % Funcs | % Lines | Uncovered Line #s
----------|---------|----------|---------|---------|-------------------
All files |     100 |      100 |     100 |     100 |
 math.ts  |     100 |      100 |     100 |     100 |
----------|---------|----------|---------|---------|-------------------
Test Suites: 1 passed, 1 total
Tests:       2 passed, 2 total
Snapshots:   0 total
Time:        3.201 s
Ran all test suites.
```


Jest のマッチャー
----

```ts
expect(add(1, 2)).toBe(3)
```

上記の検証コードの `toBe` の部分は __マッチャー__ と呼ばれており、`toBe` 以外にも次のようなマッチャーが用意されています。

### 真偽値や null との比較

- `toBeTruthy()` ... 値が `true` であることを確認する
- `toBeFalthy()` ... 値が `false` であることを確認する
- `toBeNull()` ... 値が `null` であることを確認する
- `toBeUndefined()` ... 値が `undefined` であることを確認する
- `toBeDefined()` ... 値が `undefined` でないことを確認する

```ts
test('null', () => {
  const n = null
  expect(n).toBeNull()
})
```

### 否定 (not)

`not` を使うと、条件を反転できます。

```ts
expect(1).not.toBeNull()
```

### 数値の比較

数値の同値比較には `toBe` あるいは `toEqual` が使えますが、大小比較用のマッチャーも用意されています。

```ts
test('two plus two', () => {
  const value = 2 + 2
  expect(value).toBe(4)
  expect(value).toBeGreaterThan(3)
  expect(value).toBeGreaterThanOrEqual(3.5)
  expect(value).toBeLessThan(5)
  expect(value).toBeLessThanOrEqual(4.5)
})
```

浮動小数点数の同値比較には、`toBe` ではなく `toBeCloseTo` を使うようにします。

```ts
test('add floating point numbers', () => {
  const value = 0.1 + 0.2
  expect(value).toBeCloseTo(0.3)  // Do not use 'toBe'
})
```

### 正規表現

文字列が正規表現に一致するかを調べるには、`toMatch` を使用します。
次の例では、チーム名が `team-` プレフィックスで始まっているかを調べています。

```ts
test('starts with "team-" prefix', () => {
  const teamName = 'team-xxx'
  expect(teamName).toMatch(/^team-/);
})
```

### 配列、セットのテスト

配列やセットの内容が等しいことを確認するには、`toEqual` を使います。
`toBe` を使うと参照の比較になってしまうので、`toEqual` で実際の値を比較するようにします。

```ts
// 配列の比較
const arr = [1, 2, 3]
expect(arr).toEqual([1, 2, 3])

// セットの比較
const set = new Set([1, 2, 3])
expect(set).toEqual(new Set([1, 2, 3]))
```

配列やセットに特定の値が含まれているかどうかを調べるには、`toContain` を使います。

```ts
test(`contain 200`, () => {
  const arr = [100, 200, 300]
  expect(arr).toContain(200)
})
```

指定した複数の値がすべて含まれているかどうかを調べるには、ちょっと複雑ですが、`expect.arrayContaining` を組み合わせて次のようにします。

```ts
const arr = [1, 2, 3, 4, 5]
expect(arr).toEqual(expect.arrayContaining([3, 5, 1]))
```

### 例外のスロー

ある関数が例外を投げるかどうかを調べるには、`toThrow` を使用します。
テスト対象の関数が `expect` 内で呼び出されるように、ラムダ式の形で渡すことに注意してください。

```ts
function foo() {
  throw new Error()
}

test(`foo throws exeception`, () => {
  expect(() => foo()).toThrow()  // 何らかの例外をスローすることを確認
  expect(() => foo()).toThrow(Error)  // Error をスローすることを確認
})
```

### その他のマッチャー

マッチャーの詳細は、[expect API](https://archive.jestjs.io/docs/ja/expect) のページで確認することができます。

