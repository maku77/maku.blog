---
title: "D3.js のコードに TypeScript の型注釈 (Type Annotation) をつける"
url: "p/jrqnujw/"
date: "2023-11-05"
tags: ["D3.js"]
---

{{% private %}}
- https://github.com/DefinitelyTyped/DefinitelyTyped/blob/master/types/d3-selection/index.d.ts
  - `@types/d3-selection` による `d3.Selection` まわりの型定義
{{% /private %}}

D3.js アプリを TypeScript で実装する場合、その型情報の扱いが若干難しいのでメモしておきます。


D3.js の型情報のインストール
----

D3.js の npm パッケージ `d3` は TypeScript の型情報を含んでいないので、TypeScript コードで D3.js を使用するときは、別途 DefinitelyTyped が提供している __`@types/d3`__ パッケージをインストールしておく必要があります。
以下のコマンドを使用してインストールします。

```console
$ npm install d3  # D3.js 本体
$ npm install --save-dev @types/d3  # D3.js 用の型情報
```


D3 セレクションオブジェクトの型注釈
----

TypeScript で D3.js を使ったアプリを開発していると、最初につまずくのは `select()` や `selectAll()` メソッドで取得する D3 セレクションオブジェクトの型注釈だと思います。
この型情報は __`d3.Selection`__ というインターフェースとして以下のように定義されています。

{{< code lang="js" title="d3.Selection 型の定義（抜粋）" >}}
/**
 * A D3 Selection of elements.
 *
 * The first generic "GElement" refers to the type of the selected element(s).
 * The second generic "Datum" refers to the type of the datum of a selected element(s).
 * The third generic "PElement" refers to the type of the parent element(s) in the D3 selection.
 * The fourth generic "PDatum" refers to the type of the datum of the parent element(s).
 */
export interface Selection<
    GElement extends BaseType,
    Datum,
    PElement extends BaseType,
    PDatum> {
  // ...
}
{{< /code >}}

この型パラメーターは、選択した要素の型とデータの型、親要素の型とデータの型を表します。
データというのは、`data()`/`datum()` メソッドで結びつけるオブジェクトのことを示しています。

この `d3.Selection` 型のオブジェクト、つまり D3 セレクションオブジェクトを取得するには、以下のような `d3.select()` 関数や `d3.selectAll()` 関数を使用することになります。

```js
export function select<GElement extends BaseType, OldDatum>(selector: string)
    : Selection<GElement, OldDatum, HTMLElement, any>;

export function selectAll<GElement extends BaseType, OldDatum>(selector: string)
    : Selection<GElement, OldDatum, HTMLElement, any>;
```

よって、次のように `svg` 要素のセレクションを取得すると、その型は暗黙的に `d3.Selection<d3.BaseType, unknown, HTMLElement, any>` になります（もちろん型パラメーターを明示することもできます）。

```js
// d3.Selection<d3.BaseType, unknown, HTMLElement, any> 型になる
const svg = d3.select('#mysvg');
```

さらに、既存の `d3.Selection` オブジェクトに対して `select()` メソッドや `selectAll()` メソッドを呼び出すと、そのセレクションの子孫要素を示す `d3.Selection` オブジェクトを取得できます。
その型情報のうち `PElement` と `PDatum` は親セレクションのものが引き継がれるようになっています。

```js
export interface Selection<GElement extends BaseType, Datum, PElement extends BaseType, PDatum> {
  select<DescElement extends BaseType>(selector: string)
      : Selection<DescElement, Datum, PElement, PDatum>;
  selectAll<DescElement extends BaseType, OldDatum>(selector: string)
      : Selection<DescElement, OldDatum, GElement, Datum>;
  // ...
}
```

D3.js プログラミングの王道である、[`data()` によるデータ結合](/p/bp2btie/) の仕組みを使う場合、次のように各ステップごとに `d3.Selection` の型パラメーターが具体化されていきます。

```js
// 適当にデータ配列を用意
type MyData = { x: number; y: number };
const data: MyData[] = [{ x: 10, y: 10 }, { x: 20, y: 20 }];

// d3.Selection の型パラメーターは徐々に具体化されていく
d3.select('#mysvg') // d3.Selection<d3.BaseType, unknown, HTMLElement, any>
  .selectAll('circle') // d3.Selection<d3.BaseType, unknown, d3.BaseType, unknown>
  .data(data) // d3.Selection<d3.BaseType, MyData, d3.BaseType, unknown>
  .join('circle') // d3.Selection<d3.BaseType | SVGCircleElement, MyData, d3.BaseType, unknown>
  .attr('cx', (d) => d.y)
  .attr('cy', (d) => d.y)
  .attr('r', 5)
```

特に、`attr()` メソッドのコールバックに渡される `d` パラメーター (Datum) は、自動的に `MyData` 型として認識され、型安全なコーディングを行えるようになっています。
このように、<s>スパゲッティコーディング</s> メソッドチェーンする場合は `d3.Selection` の型パラメーターを明示せずに済むことがほとんどですが、次のように `d3.Selection` に関する処理を関数化しようとすると、明示的な型指定が必要になることがあります。

```js
// const data: MyData[] = [ ... ];
d3.select('#mysvg').call(updateCircles, data);

function updateCircles(
  parent: d3.Selection<d3.BaseType, unknown, d3.BaseType, unknown>,
  data: MyData[]
) {
  parent
    .selectAll<SVGCircleElement, MyData>('circle')
    .data(data)
    .join('circle')
    .call(setCircleProps);
}

function setCircleProps(
  circle: d3.Selection<SVGCircleElement, MyData, d3.BaseType, any>
) {
  circle
    .attr('cx', (d) => d.x)
    .attr('cy', (d) => d.y)
    .attr('r', 5)
    .attr('fill', 'red');
}

```

ちなみに、上記で使用している `SVGCircleElement` という型は、`d3.BaseType` をより具体化したものであり、他にも `rect` 要素用の `SVGRectElement` や、`g` 要素用の `SVGGElement` などが用意されています。
具体的に型指定するのが難しい場合は、`d3.BaseType` 型を使っておけば大丈夫です。

`d3.Selection` の型パラメーターは毎回 4 つも指定しなければいけないので、コードがちょっと煩雑になってしまいます。
次のように型エイリアスを定義しておくと便利かもしれません。

```js
/** デフォルトの型パラメーターを設定した基本的な d3.Selection 型 */
type BaseSelection<
  Elem extends d3.BaseType = d3.BaseType,
  Datum = unknown,
  PElem extends d3.BaseType = d3.BaseType,
  PDatum = unknown
> = d3.Selection<Elem, Datum, PElem, PDatum>;

function updateCircles(parent: BaseSelection, data: MyData[]) {
  // ...
}

function setCircleProps(circle: BaseSelection<SVGCircleElement, MyData>) {
  // ...
}
```

