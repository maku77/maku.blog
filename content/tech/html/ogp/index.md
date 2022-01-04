---
title: "Facebook や Twitter でシェアするときに画像や説明文が表示されるようにする (OGP: Open Graph Protocol)"
url: "/p/awakw8i"
date: "2020-03-14"
tags: ["HTML"]
---

OGP とは
----

Web ページ内に、[OGP: Open Graph Protocol](https://ogp.me/) に基づいた HTML タグを含めておくと、Facebook や Twitter などの SNS アプリで URL 共有したときに、サイト画像や説明文を表示することができます。
下記は、https://www.yahoo.co.jp/ を Facebook で共有したときの表示例です。

{{< image w="400" src="img-001.png" title="Yahoo! Japan の例" >}}

何も指定しなくても、ある程度はコンテンツから推測して表示してくれますが、想定外の画像が表示されてしまったりするので、ちゃんと指定しておいた方がよいです。


Web サイトに OGP メタ情報を付加する
----

{{% private %}}
- [Facebook 公式 OGP 記述例](https://developers.facebook.com/docs/sharing/webmasters#markup)
{{% /private %}}

Open Graph タグを使ってメタ情報を付加するときは、主に次のような情報を記述します。

- {{< label "og:site_name" >}}: サイト名（`og:title` の方は個々のページの名前）
- {{< label "og:title" >}}: ページのタイトル（サイト名は含まれないようにします）
- {{< label "og:type" >}}: コンテンツの種類（`website` で OK。ブログ記事の場合は `article` を使うことも）
- {{< label "og:url" >}}: ページの URL（絶対パスで指定）
- {{< label "og:image" >}}: サムネイル画像の URL（絶対パスで指定）
- {{< label "og:description" >}}: コンテンツの内容を示す説明文
- {{< label "og:locale" >}}: コンテンツのロケール（`ja_JP` で OK）

これらのメタ情報は、HTML の `meta` 要素として、`head` 要素内に記述します。

{{< code lang="html" >}}
<!DOCTYPE html>
<html lang="ja">
<head prefix="og: http://ogp.me/ns# fb: http://ogp.me/ns/fb#">
  <meta charset="UTF-8">
  <meta property="og:site_name" content="まくろぐ" />
  <meta property="og:title" content="OGP とは" />
  <meta property="og:type" content="website" />
  <meta property="og:url" content="https://maku.blog/p/awakw8i" />
  <meta property="og:image" content="https://maku.blog/assets/img/site-logo.png" />
  <meta property="og:description" content="ウェブサイトに OGP メタ情報を付加すると、SNS でリンクを共有したときの表示をカスタマイズすることができます。" />
  <meta property="og:locale" content="ja_JP" />
  <title>OGP とは | まくろぐ</title>
  ...
</head>
...
{{< /code >}}


OGP メタ情報の指定方法が正しいか確認する
----

### Facebook の表示確認

Facebook が提供している「[シェアデバッガー](https://developers.facebook.com/tools/debug/)」というサイトを使うと、URL でシェアされたときにどのように見えるかを確認することができます。
使用するには、Facebook にログインする必要があります。

{{< image w="750" src="img-002.png" title="Facebook のシェアデバッガー" >}}

メタ情報の指定が間違っていると、「修正が必要な問題」として指摘してくれます。
例えば、次のように指摘してくれたりします。

> - 画像が小さすぎます
>     - 指定されたog:image URL「https://maku.blog/assets/img/site-logo.png」は最小サイズ制限(200 x 200ピクセル)を満たしていないため無効となりました。
> - プロパティがありません
>     - 次のプロパティは必須です: og:description, fb:app_id

{{< note >}}
Facebook のシェアデバッガーでは、`fb:app_id` が指定されていないと怒られますが、指定しなくてもシェア時の表示はちゃんとされているみたいです。
{{< /note >}}


### Twitter の表示確認

Twitter も同様に、「[Card validator](https://cards-dev.twitter.com/validator)」という表示確認ツールが用意されています。

{{< image w="600" src="img-003.png" title="Twitter の Card validator" >}}


Facebook の app_id を指定する
----

Facebook での URL シェア時に Open Graph タグを使用するには、公式には、Facebook 独自のアプリ ID (**`fb:app_id`**) プロパティも指定しておかないといけないようです。

{{< code lang="html" >}}
<meta property="fb:app_id" content="123456789012345" />
{{< /code >}}

実際にはこの記述がなくてもうまく表示できるようですが、アプリ ID を指定しておくと Facebook のいろいろな分析ツールと連携できるようになるようなので、後回しでもいいので設定しておくとよいでしょう。

アプリ ID は、以下のようにして取得することができます。
詳細は、サイトに表示される手順に従ってください。

1. [Facebook for Developers](https://developers.facebook.com/) からデベロッパーアカウントを作成する。
2. デベロッパーとしてログインしたら、サイト上部のメニューなどから `マイアプリ` → `アプリを作成` を選択。


Twitter シェア時の表示をカスタマイズする
----

`meta` タグで次のような記述をしておくと、Twitter での URL シェア時に、どのような形式で表示するか（カードタイプ）を指定できます。

{{< code lang="html" >}}
<meta name="twitter:card" content="summary" />
{{< /code >}}

`content` 属性で、次のいずれかのカードタイプを指定できます。

- {{< label "summary" >}}: タイトルと説明文と画像（デフォルト）
- {{< label "summary_large_image" >}}: 同上だけど、大きめの画像（Facebook の表示に近い）
- {{< label "app" >}}: モバイルアプリの配信用（アプリのアイコンを表示する）
- {{< label "player" >}}: 動画や音楽などのメディア配信用

{{% private %}}
- [About Twitter Cards — Twitter Developers](https://developer.twitter.com/en/docs/tweets/optimize-with-cards/overview/abouts-cards)
{{% /private %}}

{{< note >}}
Open Graph 用のメタ情報記述では `property` 属性を指定していましたが、Twitter 用のメタ情報記述には `name` 属性を指定することに注意してください。
`og:` プレフィックスとは異なり、`twitter:` プレフィックスを定義しておく必要はありません。
ページのタイトル情報などは、Facebook と共通の `og:` プレフィックスで指定したものを使ってくれるので、Twitter 用に別途指定する必要はありません。
{{< /note >}}


その他の参考情報
----

- [Facebook - リンクシェアの画像](https://developers.facebook.com/docs/sharing/webmasters/images)
    - Facebook が推奨している画像サイズの説明があります。少なくとも 200x200 以上の画像を指定しないといけないことなどが記述されています。


