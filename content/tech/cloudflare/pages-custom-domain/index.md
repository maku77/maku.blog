---
title: "Cloudflare Pages で独自ドメインで Web サイトを公開する"
url: "p/efcf8xn/"
date: "2026-05-08"
tags: ["cloudflare", "dns"]
---

何をするか
----

Web アプリをホスティングしたいとき、なんだかんだ言ってやっぱり **Cloudflare** が簡単かつ安価で高速です。
今後もこの図式が崩れることはしばらくなさそうです。
ということで、Cloudflare を使って独自ドメインの Web サイトを公開する手順をまとめておきます。
構成は以下の通りです。

- ドメイン取得（レジストラ）: お名前.com
- ネームサーバー（権威 DNS サーバー）: Cloudflare
- ホスティング: Cloudflare Pages
- コンテンツ用リポジトリ: GitHub

下記のページを読んでおくと、何の設定をしているのか理解しやすいと思います。

- 参考: [ドメイン管理と DNS 管理の違いを理解する](https://maku.blog/p/bwamwgp/)

ドメインはお名前.com で取得済みのものがあったので、とりあえずそれを使うことにします。
ここでは、**`example.com`** という独自ドメインを取得済みとして説明します。

{{% note title="Cloudflare Registrar でドメイン取得" %}}
独自ドメインを持っていない場合は、[Cloudflare Registrar](https://www.cloudflare.com/ja-jp/products/registrar/) でドメインを取得するのもおすすめです。
Cloudflare Registrar でドメインを取得した場合は、そのドメインのネームサーバーとして自動的に Cloudflare DNS が使われることになります。

日本でのドメイン運用会社（レジストラ）としては、お名前.com が有名ですが、実はレジストラも Cloudflare Registrar を使った方がお財布に優しく、設定画面もわかりやすいらしいので、あとでレジストラを移管するつもりです。→ [しました](/p/ead67i7/)
{{% /note %}}

ちなみに、ネームサーバーが正しく設定されていない状態だと、下記のように `nslookup` コマンドによるドメイン名の解決結果が `SERVFAIL` エラーになります。
ドメイン自体が存在しない場合は `NXDOMAIN` エラーになります。
ネームサーバーが正しく設定されているが、A レコードなどの情報がない場合は、`No answer` 表示になります。

```console
$ nslookup example.com
  ;; Got SERVFAIL reply from 1.1.1.1, trying next server
  Server:         1.0.0.1
  Address:        1.0.0.1#53

  ** server can't find example.com: SERVFAIL
```

アドレス解決ができないので、`ping` コマンドなども失敗します。

```console
$ ping example.com
ping: cannot resolve example.com: Unknown host
```


Cloudflare のネームサーバー設定
----

{{% private %}}
https://developers.cloudflare.com/dns/zone-setups/full-setup/setup/
{{% /private %}}

ドメイン（`example.com` など）の情報を Cloudflare のネームサーバー（権威 DNS サーバー）で管理してもらうよう設定します。
Cloudflare のネームサーバー設定は以下から行います。

- [Cloudflare のダッシュボード](https://dash.cloudflare.com/) → [ドメイン (Domains)](https://dash.cloudflare.com/?to=/:account/domains/overview)

最初は 1 つもドメインが登録されていない状態なので、次のように追加します。

1. **`ドメインを追加 (Add domain)`** を選択します。
1. **`ドメインを接続 (Connect a domain)`** を選択し、ドメイン名 (`example.com`) を入力します。
1. 「プラン選択」の画面が表示されるので、**`Free（無料プラン）`** を選択して次に進みます。
1. 「DNS レコードを確認する」の画面で現状の DNS 設定を確認できます。現状は何も設定されていない想定なので、そのまま **`アクティベーションに進む (Continue to activation)`** を選択します。

上記手順を踏むと、以下のように Cloudflare のネームサーバーのアドレス (`*.ns.cloudflare.com`) が表示されるので、このアドレスを、ドメイン自体を管理しているレジストラ側で設定します。

{{< image border="true" src="img-001.png" title="Cloudflare のネームサーバーのアドレス" >}}

このアドレスのサブドメイン部分（赤枠）は、登録するドメインごとに変わる可能性があるので、必ず自分の画面で表示されているアドレスを確認してください。
上記の例では、すでにレジストラ側に別のネームサーバー (`*.dns.ne.jp`) が設定されている、と表示されていますが、これを Cloudflare のネームサーバーのアドレスで上書きします。
例えば、お名前.com の場合は、以下のようにネームサーバーアドレスを設定します。

1. お名前.com Navi の [ドメイン設定](https://navi.onamae.com/domain) タブを開きます。
1. 購入したドメインの一覧が表示されるので、そこから対象のドメインを選択して **ネームサーバー情報** を変更します。

「反映されるまで 24〜72 時間かかることがあります」と表示されますが、通常はすぐに反映されます。
`dig NS example.com` や `nslookup -type=NS example.com` などのコマンドで、Cloudflare ネームサーバーのアドレスが表示されるようになれば反映完了です。

レジストラ側に Cloudflare ネームサーバーのアドレスを設定したら、Cloudflare 側の設定画面で **`ネームサーバーを更新しました (I updated my nameservers)`** ボタンを押して設定完了です。

{{< image border="true" src="img-002.png" title="Cloudflare ネームサーバーにドメイン登録が完了した" >}}

上のような表示が出ればうまく設定できています。
٩(๑❛ᴗ❛๑)۶ わーぃ


Cloudflare Pages で Web サイトを作る
----

{{% private %}}
https://developers.cloudflare.com/pages/get-started/git-integration/
{{% /private %}}

以上の設定で、独自ドメインが Cloudflare のネームサーバーで処理されるようになりました。
ただ、肝心の Web サイト（Web サーバー）がまだないので、Cloudflare Pages を使って簡単な Web サイトを作っておきます。
HTML コンテンツが格納された GitHub リポジトリがあれば、簡単に Cloudflare Pages で公開できます。

まずは、適当な GitHub リポジトリを作成して、次のような簡単な HTML ファイルをコミットしておきます。
`/public` ディレクトリ以下のコンテンツをトップページとして公開することにします。

{{< code lang="html" title="/public/index.html" >}}
<!DOCTYPE html>
<meta charset="UTF-8">
<title>Hello, Cloudflare Pages!</title>
<p>Hello, Cloudflare Pages!</p>
{{< /code >}}

Cloudflare Pages の作成は下記から行います。

- [Cloudflare のダッシュボード](https://dash.cloudflare.com/) → [Workers & Pages](https://dash.cloudflare.com/?to=/:account/workers-and-pages)

次のように新しいアプリケーションを作成します。

1. **`アプリケーションを作成する (Create application)`** を選択します。
1. **`Continue with GitHub`** を選択して、コンテンツを置いた GitHub リポジトリを選択します。
1. 「Setup your application」の画面で任意のプロジェクト名を設定し、**パスを `/public` に変更** して、**`デプロイ`** ボタンを押します。

これだけで、`https://<プロジェクト名>.<ユーザー名>.workers.dev/` のような URL で Web サイトが公開されます。

٩(๑❛ᴗ❛๑)۶ できたっ


Cloudflare Pages に独自ドメインを結びつける
----

最後に、Cloudflare Pages で公開した Web サイトに、独自ドメインでアクセスできるように設定します。

1. [Workers & Pages](https://dash.cloudflare.com/?to=/:account/workers-and-pages) を開いて、対象のプロジェクトを選択します。
1. **`設定 (Settings)`** タブの **`ドメインとルート (Domains and Routes)`** で、**`追加 (Add)`** ボタンを押します。
1. **`カスタムドメイン (Custom domain)`** を選択し、登録済みの独自ドメイン（例: `example.com`）を入力します。

これだけで、独自ドメイン（`https://example.com` など）で Web サイトにアクセスできるようになります。
サブドメイン（例: `hello.example.com`）を割り当てたいときは、上記のカスタムドメイン設定でサブドメインをそのまま入力するだけです。
https によるアクセスも自動で有効になるので、証明書の設定など面倒なことは一切不要です。

٩(๑❛ᴗ❛๑)۶ Cloudflare 便利すぎでしょ！

