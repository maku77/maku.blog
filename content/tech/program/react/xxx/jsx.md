---
title: "React の JSX 記述のコツ"
url: "/p/xjv6gqy"
date: "2021-06-17"
lastmod: "2023-12-12"
changes:
  - 2023-12-12: 辞書要素のループ方法
tags: ["React"]
---

React アプリケーションでは、コンポーネントの UI を JSX 構文を用いて記述します。
下記は、JSX コードを記述するときのポイントのまとめです。
通常の JavaScript コードとは異なる記述方法が必要になるため慣れが必要ですが、ここに挙げたポイントをおさえておけば大体対応できます。


JSX の最上位要素はひとつ
----

JSX 要素を作成するとき、トップレベルの要素は 1 つだけにする必要があります。
次のコードでは、トップレベルに 2 つの `p` 要素が並んでいるのでエラーになります。

{{< code lang="jsx" title="ダメな例" >}}
const MyComponent: React.FC = () => {
  return (
    <p>Hello</p>
    <p>World</p>
  )
}
{{< /code >}}

これを解決するには、例えば次のようにルート要素として `div` を配置します。

{{< code lang="jsx" >}}
return (
  <div>
    <p>Hello</p>
    <p>World</p>
  </div>
)
{{< /code >}}

もちろん、これはこれで動作するのですが、ルートに余計な `div` 要素が作られてしまうのを防ぎたいときは、次のように __`<>...</>`__ で囲います（これは `<React.Fragment>` の省略記法です）。

{{< code lang="jsx" >}}
return (
  <>
    <p>Hello</p>
    <p>World</p>
  </>
)
{{< /code >}}


条件を満たすときのみ出力する
----

次のコードは、`n > 0` を満たしたときに、後半の `p` 要素を表示します。

{{< code lang="jsx" >}}
return <>
  {n > 0 && <p>条件を満たしたよ</p>}
</>
{{< /code >}}

JSX コードの中では、`if` や `for` といった値を返さない文 (statement) を記述することができません。
なので、出力を条件分岐させたいときは、この例のように、__`&&`__ や __`||`__ のような演算子を駆使する必要があります。

JSX の `{}` 内に記述した式の評価結果が __`undefined`__、__`null`__、__`true`__、__`false`__ のいずれかになった場合、`{}` 全体は __何も出力しない__ ことを表します。
`true` の場合に何も表示されないのは直感的ではないと思うかもしれませんが、画面上に `true` と表示されても困るし、まぁそんなものだと受け入れるしかないですね。
上記コードの例でいうと、`n > 0` の部分が「偽」になった瞬間に `{}` 全体が `false` として評価されるため、結果的に何も出力されないということになります。


if-else 出力
----

ある条件を満たしたときは A、満たさなかったときは B、と表示分けしたいときは、JavaScript の __三項演算子（`?` と `:`）__ を使います。
次のコードは、`n` が偶数のときに `Even`、奇数のときに `Odd` と出力します。

{{< code lang="jsx" >}}
{n % 2 == 0 ? 'Even' : 'Odd'}
{{< /code >}}

オブジェクトのあるプロパティが未設定 (`undefined`) のときに代替値を表示したいときは、ES2020 の [Nullish Coalescing (??)](/p/5oyaju5) を使うとシンプルに記述できます。
次の例では、`movie` オブジェクトの `genre` プロパティが設定されているときはその値を表示し、設定されていないときは `不明` と表示します。

{{< code lang="jsx" >}}
{movie.genre ?? '不明'}
{{< /code >}}


配列要素のループ出力
----

配列要素を `forEach` 風に繰り返し出力するには、__`map`__ メソッドを使用します。
初めて見るとウッとなりますが、React/JSX の世界ではこの書き方は一般教養。
慣れるしかありません。

{{< code lang="jsx" >}}
const foods = [
  { id: 1, name: 'apples' },
  { id: 2, name: 'bananas' },
  { id: 3, name: 'lemons' },
]

const FoodList: React.FC = () => {
  return (
    <ul>
      {foods.map((f) => (
        <li key={f.id}>I like {f.name}</li>
      ))}
    </ul>
  )
}
{{< /code >}}

このように要素を繰り返し出力する場合は、React が効率的な差分出力を行えるように、各要素に一意な __`key`__ プロパティ値を指定しなければいけないことに注意してください（参考: [リストと key](https://ja.reactjs.org/docs/lists-and-keys.html)）。
これを忘れると、`Each child in a list should have a unique "key" prop.` といった警告が出ます。


辞書要素のループ出力
----

辞書（ディクショナリ）要素を繰り返し出力する場合は、__`Object.keys()`__ でキーの配列を取り出し、それを __`map`__ メソッドで処理します。

{{< code lang="jsx" >}}
const myDict = {
  key1: 'value1',
  key2: 'value2',
  key3: 'value3',
}

function MyComponent() {
  return (
    <ul>
      {Object.keys(myDict).map((key) => (
        <li key={key}>{`${key}: ${myDict[key]}`}</li>
      ))}
    </ul>
  )
}
{{< /code >}}

キー順にソートして出力したいときは、`Object.key()` で取り出したキー配列に対して `sort()` をかけます。

```jsx
{Object.keys(myDict).sort().map((key) => ( /* ... */ ))}
```


n 回のループ出力
----

ちょっとハック的ですが、n 回の繰り返しは次のように記述できます。

{{< code lang="jsx" >}}
{[...Array(3)].map((_, i) => (
  <p key={i}>{i}: Hello</p>
))}
{{< /code >}}

`[...Array(3)]` という部分で、`[undefined, undefined, undefined]` というサイズ 3 の配列を作っておいて、各要素を `map` でループ処理してます。


コメントを記述する
----

JSX コードの中では、JavaScript と同様のコメント記法が使えます。

{{< code lang="jsx" >}}
<>
  <div>
    {
      // 一行コメント
      n > 0 && <p>Hello</p>
    }
  </div>
  <div>
    {/*
      複数行にわたるコメントは、
      こんな感じで記述できます。
    */}
  </div>
</>
{{< /code >}}


ユーザーコンポーネント名は大文字で始める
----

JSX コード内で使うユーザー定義コンポーネントの名前は、大文字で始めなければいけません。
小文字で始まる名前 (`p` や `div`）は、組み込みのコンポーネントとして予約されているので使えません。
なお、JSX 組み込みのコンポーネントは `JSX.IntrinsicElements` として定義されており、200 近くのコンポーネントが定義されています。

