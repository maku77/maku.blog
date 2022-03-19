---
title: "Vercel のウェブサイトに独自ドメインを割り当てる"
url: "/p/9vakw8i"
date: "2021-05-07"
tags: ["お名前.com", "Next.js", "Vercel"]
weight: 112
---

Vercel で公開している Web サイトに、[お名前.com](https://www.onamae.com/) で取得した独自ドメインを割り当てる方法を説明します。
Vercel 上での Next.js アプリ公開までの手順は、下記の記事を参考にしてください。

- 参考: [Next.js アプリを Vercel で公開する](/p/j6ht5fq)

ここでは、独自ドメインを「お名前.com」で取得済みだと仮定しますが、別のレジストラで取得している場合もほぼ同様に設定できるはずです。
大きく分けて、次の 2 種類の設定方法があります。

- Vercel の DNS サーバーを使う方法
- Vercel 以外の DNS サーバーを使う方法

独自ドメインの「レジストラ」と「DNS サーバー」の関係については、下記の記事で簡単にまとめていますので参考にしてください。
後述の設定で何をしているのかが分かりやすくなると思います。

{{< reference >}}
- [ドメイン管理と DNS 管理の違いを理解する](/p/bwamwgp)
{{< /reference >}}


Vercel の DNS サーバーを使う方法
----

お名前.com などのレジストラで独自ドメインを取得すると、そのドメインをどの DNS サーバーで運用するかという設定を行えるはずです。
次のように Vercel 側の DNS サーバー設定を行うと、お名前.com 側に設定すべき DNS サーバー名を確認できます。

(1) [Vercel のサイト](https://vercel.com/) から対象アプリのページを開き、__Settings__ → __Domains__ と選択し、取得済みの独自ドメイン名を入力して __Add__ ボタンを押してください。ここでは、例として `example.com` というドメインを管理するとします（ドメイン自体は「お名前.com」で取得したものです）。

{{< image w="800" border="true" src="img-001.png" title="Vercel に独自ドメインを設定する" >}}

(2) `example.com` に加えて `www.example.com` もエイリアスとして設定するかというダイアログが出ますが、`example.com` だけでよければ、__`Add example.com`__ を選択すれば　OK です。

(3) Vercel にドメインが登録されて、一時的に `Invalid Configuration` の状態になります。ここで、__Nameservers__ を選択すると、お名前.com 側に設定すべき DNS サーバーアドレスが分かります。下の図のように、Vercel の DNS サーバーは、`ns1.vercel-dns.com` と `ns2.vercel-dns.com` であることが分かります。

{{< image w="600" border="true" src="img-002.png" title="Vercel の DNS サーバーアドレス" >}}

あとは、お名前.com 側にこのアドレスを設定してやれば OK です。
[お名前.com Navi](https://www.onamae.com/navi/login/) にログインして、__`ドメイン`__ → __`利用ドメイン一覧`__ → __`ネームサーバー`__ のような感じで辿ると、外部の DNS サーバーを設定できるはずです。

DNS のレコード設定をしてしばらく待つと、次のように `Invalid Configuration` だったところが `Valid Configuration` の表示に代わり、独自ドメインを使って Web サイトにアクセスできるようになります。

{{< image w="600" border="true" src="img-006.png" title="Vercel の独自ドメイン設定完了！" >}}

{{< reference >}}
- [お名前.com - DNS設定の変更手続きをしてから有効になるまでの期間は？](https://help.onamae.com/answer/8081)）
- [DNS Checker（DNS 設定がどれだけ伝搬しているかの確認）](https://www.whatsmydns.net/)
{{< /reference >}}


Vercel 以外の DNS サーバーを使う方法
----

すでにどこかの DNS サーバーでドメインを管理している場合は、そこに Vercel サーバーのアドレスを A レコード（ドメイン名 → IP アドレスのマッピング）として登録するという方法が使えます。
登録すべき A レコードの情報は、Vercel サーバー側で次のように設定すると確認できます。

(1) Vercel の対象アプリのページを開き、__Settings__ → __Domains__ と選択し、取得済みの独自ドメイン名を入力して __Add__ ボタンを押してください。ここでは、例として `example.com` というドメインを管理するとします（ドメイン自体は「お名前.com」で取得したものです）。

{{< image w="800" border="true" src="img-001.png" title="Vercel に独自ドメインを設定する" >}}

(2) `example.com` に加えて `www.example.com` もエイリアスとして設定するかというダイアログが出ますが、__`Add example.com`__ を選択すれば　OK です。

(3) 一時的に `Invalid Configuration` の状態になり、DNS サーバーに設定すべき A レコードの値を確認できます（A レコードとして `76.76.21.21` を設定すればいいことが分かります）。

{{< image w="600" border="true" src="img-003.png" title="DNS サーバーに設定すべき値" >}}

`example.com` ではなく、`myapp.example.com` のようなサブドメインを割り当てることも可能です。
その場合は、A レコードの代わりに次のように CNAME が表示されます。

{{< image w="600" border="true" src="img-004.png" >}}

あとは、DNS サーバー側に上記のレコード情報（A レコード or CNAME レコード）を登録をするだけですが、この設定画面は使っているサービスによって UI が異なるので、詳しくはそのサービスのマニュアルに従ってください。
例えば、お名前.com の DNS サーバー（`01.dnsv.jp` など）を使っている場合は、[お名前.com Navi](https://www.onamae.com/navi/login/) にログインして、次のような感じで辿れば設定できるはずです。

1. トップメニューから __DNS__ → __ドメインのDNS設定__ を選択
2. サイドバーから __ネームサーバーの設定__ → __DNS設定/転送設定__ を選択
3. __DNSレコード設定を利用する__ の __設定する__ ボタンを押す

{{< image border="true" src="img-005.png" title="お名前.com の DNS サーバーの設定画面" >}}

{{% note title="お名前.com のレンタルサーバーの DNS を使っている場合" %}}
すでにお名前.com のレンタルサーバを使って Web サイトを運用している場合は、おそらくレンタルサーバー側の DNS サーバー（`dns01.gmoserver.jp` など）を使う設定になっています。その場合は、上記の画面からではなく、[レンタルサーバーのコントロールパネル](https://cp.rentalserver.jp/Login.aspx) の「独自ドメイン設定」メニューから DNS レコード設定を行う必要があります。
{{% /note %}}

DNS のレコード設定をしてしばらく待つと、次のように `Invalid Configuration` だったところが `Valid Configuration` の表示に代わり、独自ドメインを使って Web サイトにアクセスできるようになります。

{{< image w="600" border="true" src="img-006.png" title="Vercel の独自ドメイン設定完了！" >}}

{{< image w="600" border="true" src="img-007.png" >}}

DNS の設定がどれだけ早く反映されるかに依存しますが、私の場合は数分で全ての設定が反映されました。


最初に生成された xxxxx.vercel.app を削除する
----

独自ドメインの設定が完了すると、Vercel でアプリを登録したときに自動生成された `myapp-xxx-xxx.vercel.app` といったドメイン名は不要になります。
__`Domains`__ の画面から __`Edit`__ ボタンを押して、ドメイン自体を削除してしまうか、新しく設定した独自ドメインにリダイレクトするように指定します。
削除してしまうのが心配であれば、__`308 Permanent Redirect`__ を選んで、独自ドメインにリダイレクトするようにしておけば OK です。

