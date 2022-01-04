---
title: "Azure Functions で簡単な関数を作ってみる"
url: "/p/vgt5g7f"
date: "2020-08-16"
tags: ["Azure", "Azure Functions"]
weight: 100
---

{{< image src="img-001.png" >}}

Azure Functions を使うと、Web API 的なものを、サーバーの存在を意識せずに作成することができます。
ここでは、最初のステップとして、HTTP リクエストで送ったメッセージをオウム返しするだけの簡単な関数を作ってみます。


Functions アプリを新規作成する
----

新しい Functions アプリを作成するには、[Azure ポータルのリソースの作成画面](https://portal.azure.com/#create/hub) から、__Functions App（関数アプリ）__ を選択します。
Azure のアカウントがない場合は先に作成する必要があります。

画面に従って入力していけば作成できますが、いくつかポイントがあるので説明しておきます。

- 基本タブ
    - {{< label "関数アプリ名" >}} ... 任意のアプリ名を付けることができますが、`<アプリ名>.azurewebsites.net` というアドレスが割り当てられるので、世界で一意な名前を指定する必要があります。
    - {{< label "ランタイムスタック" >}} ... 関数の実装に使用する言語を選択します。JavaScript で記述するなら `Node.js`、C# で実装するなら `.NET Core` を選択しておきます。
- ホスティングタブ
    - {{< label "プランの種類" >}} ... 今回のテストのように、ときどき実行するだけなら `消費量（サーバーレス）` を選択しておきます。`App Service プラン` は常時起動型の VM でホスティングするもので、ほとんど関数呼び出ししなくても月額数千円はかかってしまうので、最初は避けておくのが無難です。ただし、すでに他の Web サーバに App Service リソースを使用しているのであれば、そちらに相乗りしてホスティングすることが可能です。

最後に `作成` ボタンを押せば、数分で Functions のリソース作成が完了します。


Functions アプリに関数を追加する
----

{{< image border="true" src="img-002.png" >}}

Functions リソースに新しい関数を追加するには、__`関数`__ → __`追加`__ と選択します。
ここでは HTTP リクエストにより関数を実行するので、__`HTTP trigger`__ を選択します。

関数名はデフォルトのままで `HttpTrigger1`、Authorization level はキーなしでリクエストできるように __`Anonymous`__ を選択しておきます。

最後に `作成` ボタンを押すと、数秒で新しい関数が登録されます。


関数の実装を確認する
----

作成された関数を選択すると、次のようにデフォルトの実装を確認することができます。

{{< image border="true" src="img-003.png" >}}

実は、最初からオウム返しする Hello World 実装が入っているので、今回はこれをそのまま使うことにします。
関数の振る舞いを変更したければ、ここで直接コードを変更して `保存` ボタンを押すだけで一瞬で反映されます。

右上の __`関数の URL の取得`__ というボタンを押して、関数を呼び出すための URL を取得してください。


関数を呼び出す
----

### Web ブラウザから URL を開いて実行する

HTTP trigger タイプの関数は HTTP の GET リクエストで呼び出せるので、Web ブラウザのアドレスバーに次のようなアドレスを入力するだけでレスポンスを確認できます。

{{< code >}}
https://<アプリ名>.azurewebsites.net/api/HttpTrigger1?name=Maku
{{< /code >}}

ブラウザに次のようなメッセージが表示されれば成功です！

{{< code >}}
Hello, Maku. This HTTP triggered function executed successfully.
{{< /code >}}

### curl コマンドで実行する

コマンドラインからサクッと動作確認したいときは、`curl` コマンドを使うと便利です。

{{< code >}}
$ curl https://<アプリ名>.azurewebsites.net/api/HttpTrigger1?name=Maku
Hello, Maku. This HTTP triggered function executed successfully.
{{< /code >}}

ヘッダ情報などの詳細を確認したいときは、`-v` オプションを付けます。

### Web ブラウザの JavaScript から実行する

Web ブラウザ上で実行する JavaScript から Azure Functions を呼び出す場合は、クロスオリジン通信になるので、そのままだと以下のような CORS ポリシーエラーが発生してうまくいきません。

> Access to fetch at 'https://XXX.azurewebsites.net/api/HttpTrigger1?name=Maku' from origin 'http://localhost:1234' has been blocked by CORS policy: No 'Access-Control-Allow-Origin' header is present on the requested resource. If an opaque response serves your needs, set the request's mode to 'no-cors' to fetch the resource with CORS disabled.

クロスオリジン通信を許可するには、Azure Functions の __`CORS`__ の項目から次のように設定します。

{{< image w="650" border="true" src="img-004.png" >}}

アクセスを許可するドメインの指定部分には、`http://localhost:1234` のようにひとつずつドメインを指定することもできますが、今回はワイルドカード __`*`__ ですべてのドメインからのアクセスを許可しています。
ワイルドカードを使用する場合は、他のエントリはすべて削除しないとうまく動作しないので注意してください（このあたり何も警告が出ないのは意地悪ですね...）。
最後に忘れずに __`保存`__ ボタンを押せば設定が反映されます。

JavaScript からは、次のように `fetch` 関数を使って HTTP リクエストを発行できます。

{{< code lang="javascript" >}}
const API = 'https://<アプリ名>.azurewebsites.net/api/HttpTrigger1';
const url = API + '?name=' + encodeURIComponent('まく');

fetch(url)
  .then(res => {
    if (!res.ok) {
      throw new Error(`${res.status} ${res.statusText}`);
    }
    return res.text();
  })
  .then(text => {
    // これがレスポンス本文のテキスト
    alert(text);
  })
  .catch(error => {
    throw new Error(JSON.stringify(error))
  });
{{< /code >}}

