---
title: "お名前.com から Cloudflare Registrar にドメイン移管する"
url: "p/ead67i7/"
date: "2026-05-09"
tags: ["cloudflare", "お名前.com"]
---

お名前.com で取得した独自ドメインを Cloudflare に移管する手順をまとめておきます。
思ったより簡単でした。

レジストラ移管の経緯など
----

世界最強のネットワーク（たぶん）である Cloudflare には今後もお世話になり続けると思うので、ドメイン管理＋ネームサーバー管理も Cloudflare にまとめてすっきりさせたいと考えました。
Cloudflare のドメイン管理サービス（レジストラ）は、**Cloudflare Registrar** という名前で提供されています。

### Cloudflare Registrar でドメインを管理するメリット

- <b>更新費用が明確で安い: </b>
  「原価 + ICANN 手数料のみ」に近い価格設定。
  お名前.com はサービス維持調整費という名目で＋20％とか取られます。
  これは為替変動によるところが大きいと思いますが、マージン手数料も含まれているようです。
- <b>強力なネットワーク機能: </b>
  DNS 関連は最強・最速。
  HTTPS 対応とかも簡単・無料。
  CDN、WAF、DDoS 対策、キャッシュなどと統合しやすい。
  WHOIS 情報保護が実質標準。
  転送量対策やセキュリティ強化がしやすい。
- <b>不要な営業メールがない: </b>
  お名前.com は謎の追加オプションの案内とか来ます。ドメインプロテクションとか何それ、標準で守ってよ。
  あと、ドメイン取得無料（初年度無料）の案内とかマジでいらない。更新費用ぼったくられる予感しかしない。
- <b>UI がわかりやすい: </b>
  Cloudflare は世界中で使われているサービスだけに、設定画面は万人にわかりやすくシンプルです。
  Workers や R2 などの強力なサービスを使わせていただいているので、ドメインや DNS 管理も統合された画面でできるのは便利です。

### デメリット

- <b>Cloudflare DNS の使用が必須: </b>
  ネームサーバー（権威 DNS）は Cloudflare のものを使うことになります。
  とはいえ、もともと Cloudflare の DNS を使いたいのだからこれはデメリットとは言えないかも。
- <b>サポート体制: </b>
  基本はオンラインサポート中心で、詳細なやりとりは英語になるかもしれません。
  まぁ、サポートに連絡するようなことは滅多にないし問題なさそう。
- <b>ドメインの売買機能がない: </b>
  お名前.com だと、保有しているドメインをオークションに出せたりしますね。
  それで稼げることはほぼないと思いますが、面白いサービスではあります。
- <b>1 年分の更新費用を先払い: </b>
  ドメイン移管の際に、Cloudflare Registrar に 1 年分の更新費用を支払う必要があります。
  これはドメイン移管の一般的なルールで、どのレジストラでも同様です。
  次回の更新日が 1 年延長されるだけなので、継続利用する予定のドメインであれば特に問題はないと思います。
- <b>一部の TLD に非対応: </b>
  .jp ドメインなど取得・管理できない TLD（トップレベルドメイン）もあります。

.jp ドメインが使えないところは要注意ですね。
.jp は日本の JPRS（日本レジストリサービス）が管理していて、Cloudflare はまだ契約・統合対応していないようです。
幸い私は .jp ドメインを持っていないので問題なしです。
というわけで、Cloudflare にどっぷり浸かってしまっても大丈夫そうです。


ドメイン移管の手順
----

{{% private %}}
- ドメイン名を他のレジストラに移管する方法 | Cloudflare
  - https://www.cloudflare.com/ja-jp/learning/dns/how-to-transfer-a-domain-name/
- Transfer your domain to Cloudflare · Cloudflare Registrar docs
  - https://developers.cloudflare.com/registrar/get-started/transfer-domain-to-cloudflare/
- お名前.comから他の登録業者へ移管する方法は？｜お名前.com
  - https://help.onamae.com/answer/8593
{{% /private %}}

ドメイン移管のおおまかな流れは以下のようになります。
申請作業自体は 30 分もあれば終わります。
その後、メール承認などを終えると実際の移管が完了します。
すんなりいけば、合計 1 時間もかからないと思います。

1. お名前.com でドメイン移管の準備（AuthCode の取得）
2. Cloudflare でドメイン移管申請
3. お名前.com からのドメイン移管確認メールを承認

### 1. お名前.com でドメイン移管の準備

まず、現在使用しているレジストラ（今回はお名前.com）でドメイン移管の準備を行います。
安全のためのドメイン移管ロックがかかっている場合は先に解除しておきます。

1. お名前.com Navi のドメイン機能一覧の [ドメイン移管ロック](https://navi.onamae.com/domain/setting/transfer/lock) のページを開きます。
1. 対象ドメインに移管ロックがかかっている場合は **`OFF`** に切り替えます。

次に、**移管認証コード (Authorization Code)** を取得します。
これは、後の Cloudflare 側でのドメイン移管申請の際に使用します。

1. お名前.com Navi の [ドメイン設定](https://navi.onamae.com/domain) タブを開き、**対象のドメインを選択** します。
1. **`AuthCode → 表示`** を選択し、移管認証コード (AuthCode) をメモします。
   - Whois 情報公開代行を利用している場合は、[解除ページ](https://navi.onamae.com/domain/setting/whoisprotect/cancel/list) から解除しておきます。

### 2. Cloudflare でドメイン移管申請

Cloudflare に移管予定のドメイン名を登録します。

1. [Cloudflare のダッシュボード](https://dash.cloudflare.com/) の [ドメイン (Domains)](https://dash.cloudflare.com/?to=/:account/domains/overview) タブを開きます。
1. **`ドメインを追加 (Add domain)`** を選択します。
1. **`ドメインを転送 (Transfer a domain)`** を選択し、ドメイン名 (例: `example.com`) を入力します。
   - このドメインのエントリーを初めて登録するときは、「プラン選択」の画面が表示されるので、**`Free（無料プラン）`** を選択して次に進みます。
   - すでに別のネームサーバーで運用されている場合は、「DNS レコードを確認する (Review your DNS records)」の画面が表示されるので、引き継ぐレコードの内容を確認して **`アクティベーションに進む (Continue to activation)`** を選択します。
     Cloudflare のドメインネームサーバーのアドレスが表示されるので、これを現在使用中のレジストラ（例: お名前.com）に設定しておきます。
     その時点で名前解決に関しては Cloudflare のネームサーバーが使われるようになります。
1. **`認証コード (Authorization code)`** の入力を求められるので、先ほどお名前.com で取得した移管認証コード (AuthCode) を入力します。
1. **`登録者や支払い情報`** を入力して、**`移管ステータスを続行 (Confirm transfer)`** を選択します。
   - 登録者情報はそのまま WHOIS に表示されるので英語で入力しておくのがよいです（あとから設定画面で簡単に編集できますが）。

これで、お名前.com 側にも移管要求が送られるようです。
次のような移管待ち画面が表示されれば申請完了です。

{{< image w="700" border="true" src="img-001.png" title="Cloudflare にドメイン移管申請の完了" >}}

View transfer status というリンクから、移管の進捗状況を確認できます。
わかりやすいですね。

{{< image w="700" border="true" src="img-002.png" title="ドメイン移管の状況" >}}

### 3. お名前.com からのドメイン移管確認メールを承認

数分待つと、お名前.com から本当にドメイン移管していいのかの確認メールが届くので、**記載されたリンクをクリックして承認** します。
私の場合は Cloudflare で申請してから数分で届きました。

> Subject: 【重要】トランスファー申請に関する確認のご連絡 example.com
>
> ドメイン名: example.com
> 
> お名前.com  by GMO は、上記ドメインについて 2026/XX/XX に他社レジストラへのトランスファー申請を承りました。
> トランスファー手続きにつきまして、他社レジストラへ移管をご希望の場合は期日までに以下 URL から承認のお手続きをお願いいたします。

承認後、数分待つと今度は Cloudflare とお名前.com の両方から、ドメイン移管が完了したことを知らせるメールが届きます。
お名前.com のドメイン一覧画面からも、対象ドメインの表示が消えているはずです。

> Subject: Your transfer to Cloudflare Registrar has completed!
>
> Hello,
> Your domain, example.com, is now registered with Cloudflare.

これでドメイン移管はすべて完了です。

٩(๑❛ᴗ❛๑)۶ わーぃ

