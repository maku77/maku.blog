---
title: "D3.js のデータ結合について理解する (enter, update, exit)"
url: "p/bp2btie/"
date: "2023-10-01"
tags: ["D3.js"]
weight: 2
---

D3.js の宣言的なアプローチ
----

D3.js を使ったビジュアライゼーションでは、配列データを描画要素と対応付ける（データ結合する）とき、次のようにメソッドをチェーンさせる記述方法がよく出てきます (__`selectAll()` → `data()` → `enter()` → `append()`__)。

{{< code lang="js" title="D3.js コードのよくある書き方" hl_lines="3-6" >}}
const data = [1, 2, 3, 4, 5];

svg.selectAll("circle")
  .data(data)
  .enter()
  .append("circle")
    .attr("cx", (_d, i) => i * 35 + 25)
    .attr("cy", 25)
    .attr("r", 10);
{{< /code >}}

最初はとっつきにくいかもしれませんが、この記述方法は、D3.js が __宣言的プログラミング (Declarative Programming)__ のアプローチを採用した結果であり、これがあるからこそ D3.js のコードはシンプルに記述できるようになっています。

「宣言的」という言葉は、__「データが最終的にどのように表現されるべきか」__ をコードで表現するアプローチを指します。
その表現に到達するための詳細な処理手順をコーディングする必要がなくなるため、コードが読みやすく、不具合が発生しにくくなります。

D3.js によるデータ結合処理は、一般的に次のような流れで実装します。

1. `selectAll()` メソッドで、データの結合先にする描画要素群のセレクションオブジェクトを作成する。この段階では、描画要素は存在しなくてもよい（初回の呼び出し時には描画要素数は 0）。
2. `data()` メソッドで、データ配列を上記の描画要素群にマッピングする。
3. 3 種類のセレクション (enter/exit/update) に対する処理を定義する。

次に、3 種類のセレクションについて説明します。


データ結合時の 3 つのセレクション
----

D3.js のデータ結合では、次のような 3 種類のセレクションに分けて描画内容を定義します。

| セレクション | 説明 |
| ---- | ---- |
| __enter__ セレクション | 「データ数 ＞ 描画要素数」のときに、新しい描画要素を追加する処理 |
| __exit__ セレクション | 「データ数 ＜ 描画要素数」のときに、不要な描画要素を削除する処理 |
| __update__ セレクション | データが更新されたときに、既存の描画要素の表示を更新する処理 |

### enter セクション

{{< maku-common/d3 id="svg-okmyid7" w="220" h="70" title="enter セレクションのイメージ" >}}
const dataBefore = [1, 2];
const dataAfter = [1, 2, 3, 4, 5];
const svg = d3.select("#svg-okmyid7");
const dataGroup = svg.append("g");
const elemGroup = svg.append("g");

svg.append("text").attr("x", 10).attr("y", 25).attr("fill", "steelblue").text("Data");
svg.append("text").attr("x", 10).attr("y", 55).attr("fill", "crimson").text("Elements");

const dataCircles = dataGroup.selectAll("circle")
  .data(dataAfter)
  .join("circle")
    .attr("fill", "steelblue")
    .attr("cx", (_d, i) => i * 25 + 100)
    .attr("cy", 20)
    .attr("r", 10);

function updateElem(data) {
  elemGroup.selectAll("circle")
    .data(data)
    .join("circle")
      .attr("fill", "crimson")
      .attr("cx", (_d, i) => i * 25 + 100)
      .attr("cy", 50)
      .attr("r", 0)
      .transition()
        .delay((_d, i) => { if (i <= dataBefore.length - 1) return 0; else return 300 * (i); })
        .duration((_d, i) => { if (i <= dataBefore.length - 1) return 0; else return 300; })
        .attr("r", 10);
}

const startAnim = () => {
  updateElem(dataBefore);
  updateElem(dataAfter);
  setTimeout(startAnim, 3000); // 繰り返し
}

startAnim();
{{< /maku-common/d3 >}}

enter セレクションは、データ数の方が既存の描画要素数よりも多いときに、追加すべき要素を参照するためのセレクションです。
要は、enter セレクションに対して、__新しい描画要素を追加する処理__ を定義すれば OK です。
enter セレクションを取得するには、`data()` メソッドの戻り値のセレクションオブジェクトに対して、__`enter()`__ メソッドを呼び出します。

{{< code lang="js" title="enter セレクションの典型的な使用例" hl_lines="3" >}}
svg.selectAll("circle")
  .data(data)
  .enter().append("circle")  // enter セレクションの処理
    .attr("cx", (_d, i) => i * 25 + 100)
    .attr("cy", 20)
    .attr("r", 10)
{{< /code >}}

データが動的に変更されることがない Web サイトであれば（あるいは「追加」されていくのみのサイトであれば）、この enter セレクションの定義をすればデータ結合の実装は完成です。
逆に、データが増減したり、内容が変化したりするサイトの場合は、後述の exit、update セレクションの定義が必要になります。

### exit セレクション

{{< maku-common/d3 id="svg-tstjsb9" w="220" h="70" title="exit セレクションのイメージ" >}}
const dataBefore = [1, 2, 3, 4, 5]
const dataAfter = [1, 2]
const svg = d3.select("#svg-tstjsb9")
const dataGroup = svg.append("g")
const elemGroup = svg.append("g")

svg.append("text").attr("x", 10).attr("y", 25).attr("fill", "steelblue").text("Data")
svg.append("text").attr("x", 10).attr("y", 55).attr("fill", "crimson").text("Elements")

const dataCircles = dataGroup.selectAll("circle")
  .data(dataAfter)
  .join("circle")
    .attr("fill", "steelblue")
    .attr("cx", (_d, i) => i * 25 + 100)
    .attr("cy", 20)
    .attr("r", 10)

function updateElem(data) {
  const circles = elemGroup.selectAll("circle").data(data)
  circles.enter().append("circle")
    .attr("fill", "crimson")
    .attr("cx", (_d, i) => i * 25 + 100)
    .attr("cy", 50)
    .attr("r", 10)
  circles.exit()
    .transition()
      .delay(1000)
      .duration(1000)
      .attr("r", 0)
      .remove()
}

const startAnim = () => {
  updateElem(dataBefore)
  updateElem(dataAfter)
  setTimeout(startAnim, 3000) // 繰り返し
}
startAnim()
{{< /maku-common/d3 >}}

exit セレクションは、enter セレクションとは逆に、データ数が描画要素数よりも少ない場合に、その超過分の描画要素を選択するための D3 セレクションです。
exit セレクションに対して、__描画要素を削除する処理__ を定義するようにします。
exit セレクションを取得するには、`data()` メソッドの戻り値のセレクションオブジェクトに対して、__`exit()`__ メソッドを呼び出します。

{{< code lang="js" title="exit セレクションの典型的な使用例" hl_lines="7" >}}
const circles = svg.selectAll("circle").data(data);
circles.enter().append("circle")  // enter セレクションの処理
  .attr("fill", "crimson")
  .attr("cx", (_d, i) => i * 25 + 100)
  .attr("cy", 50)
  .attr("r", 10);
circles.exit().remove();  // exit セレクションの処理
{{< /code >}}

上記のように、exit セレクションに対して単純に `remove()` メソッドを呼び出すと、データが減ったときに、それに応じて不要な描画要素 (`circle`) が削除されます。
データ配列 (`data`) の内容を変更した後で、上記の処理を再び呼び出す必要があります。

描画要素を削除するときに、アニメーションさせたいときは、次のように `transition()` を組み合わせて使用します。

{{< code lang="js" title="1 秒間かけて半径を 0 にしてから削除する" >}}
circles.exit()
  .transition()
    .duration(1000)
    .attr("r", 0)
    .remove();
{{< /code >}}

### update セレクション

{{< maku-common/d3 id="svg-sxbs8e3" w="172" h="40" title="update セレクションのイメージ（赤色→青色）" >}}
const svg = d3.select("#svg-sxbs8e3")

svg.append("text").attr("x", 10).attr("y", 25).attr("fill", "crimson").text("Elements")

const data1 = [
  { id: 1, color: "red" },
  { id: 2, color: "red" },
  { id: 3, color: "red" },
];

const data2 = [
  { id: 1, color: "blue" },
  { id: 2, color: "blue" },
  { id: 4, color: "blue" },
];

function initElem(data) {
  const circles = svg.selectAll("circle").data(data, (d) => d.id)
  circles.join("circle")
    .attr("cx", (_d, i) => i * 25 + 100)
    .attr("cy", 20)
    .attr("fill", (d) => d.color)
    .attr("r", 0)
    .transition()
      .duration(500)
      .attr("r", 10)
}

function updateElem(data) {
  const circles = svg.selectAll("circle").data(data, (d) => d.id)
  circles.enter().append("circle")
    .attr("cx", (_d, i) => i * 25 + 100)
    .attr("cy", 20)
    .attr("fill", (d) => d.color)
    .attr("r", 0)
    .transition()
      .delay(1000)
      .duration(500)
      .attr("r", 10)
  circles.exit()
    .transition()
    .delay(1000)
    .duration(0)
    .remove()
  circles  // update セレクションの取得
    .attr("fill", "red")
    .transition()
      .delay(1000)
      .duration(500)
      .attr("fill", (d) => d.color)
}

const startAnim = () => {
  initElem(data1)
  updateElem(data2)
  setTimeout(startAnim, 3000) // 繰り返し
}
startAnim()
{{< /maku-common/d3 >}}

update セレクションは、データに対応する描画要素はすでに作成済みだけど、データの内容が変化したので描画内容も更新しなければいけない要素群を選択するためのセレクションオブジェクトです。
__`data()`__ メソッドの戻り値が update セレクションです。

例えば、次のような 2 つのデータ配列を用意して、データ結合するデータを `oldData` から `newData` に入れ替えることを考えてみます。

```js
const oldData = [
  { id: 1, color: "red" },
  { id: 2, color: "red" },
  { id: 3, color: "red" },
];

const newData = [
  { id: 1, color: "blue" },
  { id: 2, color: "blue" },
  { id: 4, color: "blue" },  // 3 つ目のデータは id が異なるため別のデータとみなされる
];
```

ここで問題となるのが、古いデータ配列 (`oldData`) のどのオブジェクトが、新しいデータ配列 (`newData`) のどのオブジェクトに対応しているかです。
上記のデータでは、`id` プロパティをデータオブジェクトの識別子として使えばよさそうです。
データオブジェクトの識別子として使う値は、`data()` メソッドの第 2 引数のコールバック関数の戻り値として返すように実装します。

```js
// oldData をデータ結合するとき
const circles = svg.selectAll("circle").data(oldData, (d) => d.id)

// newData をデータ結合するとき
const circles = svg.selectAll("circle").data(newData, (d) => d.id)
```

`oldData` と `newData` の中のデータオブジェクトのうち、先頭の 2 つは `id` プロパティの値が共通 (1, 2) なので、update セレクションではこれらに対応する描画要素が選択されることになります。
一方、3 つ目のデータオブジェクトは、`id` が 3 と 4 で異なるので、データの更新とはみなされず、`id: 3` のデータが削除され (exit)、`id: 4` のデータが追加された (enter) とみなされます。

{{< code lang="js" title="update セレクションの使用例" hl_lines="9" >}}
function render(data) {
  const circles = svg.selectAll("circle").data(data, (d) => d.id)
  circles.enter().append("circle")
    .attr("cx", (_d, i) => i * 25 + 100)
    .attr("cy", 20)
    .attr("fill", (d) => d.color)
    .attr("r", 10)
  circles.exit().remove()
  circles  // update セレクションの取得
    .attr("fill", (d) => d.color)
}

render(oldData);  // 古いデータで表示
render(newData);  // 新しいデータで更新
{{< /code >}}


便利な join メソッドを使う
----

ここまでの例では、3 種類のセレクションを取得するために、次のようなメソッドを使っていました。

- `enter()` メソッド ... enter セレクションの取得
- `data()` メソッド ... update セレクションの取得
- `exit()` メソッド ... exit セレクションの取得

これらの代わりに、__`join()`__ というユーティリティメソッドを使うと、各セレクションを処理するための関数を引数で指定することができます（引数の順番を間違えないように注意してください）。

```js
selection.selectAll("circle").data(data, (d) => d.id)
  .join(
    function (enter) { /* enter セレクションを使った処理 */ },
    function (update) { /* update セレクションを使った処理 */ },
    function (exit) { /* exit セレクションを使った処理 */ }
  )
```

下記は、データ配列 (`data`) の内容に基づいて描画要素の作成・更新・削除を行う関数の定義例です。
`join()` に渡す 3 つの関数は、戻り値として自分自身のセレクションオブジェクトを返さないといけないことに注意してください。

{{< code lang="js" title="join() メソッドを素直に使った場合" >}}
function render(data) {
  svg.selectAll("circle").data(data, (d) => d.id)
    .join(
      // enter セレクションの処理
      function (enter) {
        return enter.append("circle")
          .attr("cx", (_d, i) => i * 25 + 100)
          .attr("cy", 20)
          .attr("r", (d) => d.size)
          .attr("fill", (d) => d.color)
      },
      // update セレクションの処理
      function (update) {
        return update
          .attr("cx", (_d, i) => i * 25 + 100)
          .attr("cy", 20)
          .attr("r", (d) => d.size)
          .attr("fill", (d) => d.color)
      },
      // exit セレクションの処理
      function (exit) {
        return exit.remove()
      }
    )
}
{{< /code >}}

多くの場合、より簡潔なコードにするために、次のようにアロー関数を使って記述します。
可読性はあまり高いとは言えないですけど(^^;

{{< code lang="js" title="アロー関数を使ってシンプルに" >}}
function render(data) {
  svg.selectAll("circle").data(data, (d) => d.id)
    .join(
      (enter) => enter.append("circle")
        .attr("cx", (_d, i) => i * 25 + 100)
        .attr("cy", 20)
        .attr("r", (d) => d.size)
        .attr("fill", (d) => d.color),
      (update) => update
        .attr("cx", (_d, i) => i * 25 + 100)
        .attr("cy", 20)
        .attr("r", (d) => d.size)
        .attr("fill", (d) => d.color),
      (exit) => exit.remove()
    )
}
{{< /code >}}

よく見ると、enter セレクションと update セレクションの処理内容の差は `append("circle")` の部分だけです。
実は、`join()` メソッドの戻り値は enter セレクションと update セレクションをマージしたものになっており、共通の処理はこのセレクションオブジェクトを使って定義することができます。

{{< code lang="js" title="enter と update セレクションの処理を共通化" >}}
function render(data) {
  svg.selectAll("circle").data(data, (d) => d.id)
    .join(
      (enter) => enter.append("circle"),
      (update) => update,
      (exit) => exit.remove()
    )
    // enter セレクションと update セレクションに共通の処理
    .attr("cx", (_d, i) => i * 25 + 100)
    .attr("cy", 20)
    .attr("r", (d) => d.size)
    .attr("fill", (d) => d.color)
}
{{< /code >}}

そして、update セレクションと exit セレクションの処理が上記の実装でよければ、これらの引数は省略することができます。

{{< code lang="js" title="第 2 引数と第 3 引数を省略" >}}
function render(data) {
  svg.selectAll("circle").data(data, (d) => d.id)
    .join(
      (enter) => enter.append("circle")
    )
    .attr("cx", (_d, i) => i * 25 + 100)
    .attr("cy", 20)
    .attr("r", (d) => d.size)
    .attr("fill", (d) => d.color)
}
{{< /code >}}

さらに、enter セレクションの処理が、上記のような `append()` だけであれば、次のような省略記述が可能です（第 1 パラメーターが関数から文字列に変わります）。

{{< code lang="js" title="enter セレクションの append 呼び出しを省略" >}}
function render(data) {
  svg.selectAll("circle").data(data, (d) => d.id)
    .join("circle")
      .attr("cx", (_d, i) => i * 25 + 100)
      .attr("cy", 20)
      .attr("r", (d) => d.size)
      .attr("fill", (d) => d.color)
}
{{< /code >}}

とてもスッキリしました。

٩(๑❛ᴗ❛๑)۶ わーぃ

この記述方法は、多くの D3.js プロジェクトで利用されています。
ここまでの内容を理解していれば、どのような描画処理を定義しているのか読み取れるようになっているはずです。

