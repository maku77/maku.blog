---
title: "IGDB API でゲーム情報のデータベースにアクセスしてみる"
url: "p/mu7gqya/"
date: "2022-10-24"
tags: ["IGDB"]
---

[IGDB](https://www.igdb.com/) は、世の中の（ビデオ）ゲームの情報をデータベースとして集約しようとしている壮大なプロジェクトで、現在 Twitch が運営しています。
この情報には [IGDB API](https://api-docs.igdb.com/) という Web API でアクセスできるようになっていて、簡単に試す程度なら無料で利用できます。
ちょっと試してみたのでメモに残しておきます。
とりあえず、`curl` コマンドで API のレスポンスを確認するところまで。

大まかには、Twitch developers console でアプリクライアントを登録し、アクセストークンを発行して API を叩く、という流れになります。


Twitch アカウントを登録する
----

IGDB API を使うには、Twitch developers コンソールでアプリケーション（クライアント）の登録が必要です。
このサイトにサインインするには、__Twitch のアカウントが必要__ なので、下記から作成します。
すでに Twitch アカウントを持っている場合は、そのまま使えます。

- [Twitch Developers - Login](https://dev.twitch.tv/login)

さらに、API を使用する場合は、さらに二段階認証の設定が必須になっているので、次のような感じでたどって設定しておきます。
メールアドレスや電話番号を登録すれば二段階認証を有効化できます。

- [Twitch - 設定 - セキュリティとプライバシー](https://www.twitch.tv/settings/security)


アプリケーションを登録してクライアント ID とシークレットを発行する
----

[Twitch developers コンソール](https://dev.twitch.tv/console/) の [アプリ管理画面](https://dev.twitch.tv/console/apps/) でアプリを登録すると、クライアント ID とシークレットキーを取得できます。

1. __`アプリケーションを登録`__ ボタンをクリック
   - `名前`: 適当な名前を入力（グローバルに一意な名前じゃないといけないっぽい）
   - `OAuthのリダイレクトURL`: 決まっていないなら `http://localhost:3000` などを入力しておけば OK
   - `カテゴリー`: プルダウンから適当に選択（Website Integration など）
2. __`作成`__ ボタンを押して完了
3. 作成されたアプリの __`管理`__ ボタンをクリック
4. __`新しい秘密`__ ボタンを押してクライアントシークレットを発行

上記の手順で、IGDB API を呼び出すためのクライアント ID とシークレットを確認できます。

- クライアント ID の例: `mhtlekfud5qy706n5wpl568uz1k7j9`
- クライアントシークレットの例: `aoz73xqlpnry1lnw7ohdz7ry5qb4nu`


アクセストークンを発行する
----

IGDB API でゲーム情報を取得するためには、__アクセストークンが必要__ なのですが、これは前述の手順で発行したシークレットとは異なります。
[Authentication API](https://api-docs.igdb.com/#authentication) でクライアント ID とシークレット情報を送ることで、一時的なアクセストークンを発行する必要があります。
次のように `curl` コマンドで POST リクエストを送れば、レスポンスでアクセストークンを取得できます。

{{< code title="アクセストークンの取得" >}}
$ curl -d "client_id=XXXX&client_secret=XXXX&grant_type=client_credentials" \
       https://id.twitch.tv/oauth2/token
{{< /code >}}

{{< code lang="json" title="レスポンスの例" >}}
{"access_token":"1rp02cemrb59wzbhoi2ei8yx6nms3l","expires_in":5217184,"token_type":"bearer"}
{{< /code >}}

{{% note title="OAuth フローの種類" %}}
ここでは、直接レスポンスでアクセストークンを取得する OAuth Client Credentials Flow というフローを使いましたが、これは、サーバーサイドでトークンのやりとりを完結できる場合のみに使用できるフローです。
他の OAuth フローに関しては、下記のドキュメントを参照してください。

- 参考: [Getting OAuth Access Tokens | Twitch Developers](https://dev.twitch.tv/docs/authentication/getting-tokens-oauth)
{{% /note %}}


IGDB API でゲーム情報を取得する
----

アクセストークンを取得したら、ゲーム情報を取得するための API を呼び出すことができます。
下記はゲームのリストを取得する API (`https://api.igdb.com/v4/games`) の呼び出し例です。
REST API なので、取得する情報ごとにエンドポイント (URL) を使い分ける必要があります。

```
$ curl -H "Client-ID: mhtlekfud5qy706n5wpl568uz1k7j9" \
       -H "Authorization: Bearer 1rp02cemrb59wzbhoi2ei8yx6nms3l"
       -d "fields *;"
       https://api.igdb.com/v4/games
```

アクセストークンは、上記のように `Authorization` ヘッダーで指定してください。
デフォルトでは 10 件分のゲーム ID が返ってくるようです（本当に ID だけ）。
POST リクエストの Body 部分（`-d` オプション）で `fields *;` と指定すると、ゲームの詳細情報が含まれるようになります。

{{< accordion title="レスポンスの例（JSON 形式）" >}}
{{< code lang="json" >}}
[
  {
    "id": 40104,
    "category": 0,
    "created_at": 1498435200,
    "external_games": [
      20127,
      1988884
    ],
    "first_release_date": 536457600,
    "genres": [
      5
    ],
    "name": "Dogou Souken",
    "platforms": [
      52
    ],
    "release_dates": [
      91007
    ],
    "similar_games": [
      12364,
      27270,
      43367,
      103281,
      103292,
      103298,
      103301,
      105049,
      106805
    ],
    "slug": "dogou-souken",
    "summary": "An overhead-view shoot\u0027em up game.",
    "tags": [
      268435461
    ],
    "updated_at": 1604620800,
    "url": "https://www.igdb.com/games/dogou-souken",
    "checksum": "5e7446ff-58d9-e744-f0d3-5e10241304c1"
  },
  {
    "id": 85031,
    "age_ratings": [
      37944
    ],
    "category": 0,
    "cover": 61466,
    "created_at": 1517392813,
    "external_games": [
      216805,
      1969751,
      2005770
    ],
    "first_release_date": 1404345600,
    "genres": [
      9
    ],
    "involved_companies": [
      130531,
      130532
    ],
    "name": "City Mysteries",
    "platforms": [
      37
    ],
    "release_dates": [
      138333
    ],
    "screenshots": [
      155025,
      155026,
      155027
    ],
    "similar_games": [
      10603,
      19222,
      25905,
      41349,
      86974,
      87507,
      88511,
      90788,
      95776,
      106992
    ],
    "slug": "city-mysteries",
    "summary": "Search buildings, streets, landmarks as you seek and find hidden objects in New York, Paris, Moscow and London!",
    "tags": [
      268435465
    ],
    "updated_at": 1641425677,
    "url": "https://www.igdb.com/games/city-mysteries",
    "checksum": "87b342e7-b912-ca8d-4ffb-f91f9e4f1870"
  },
  {
    "id": 99234,
    "category": 0,
    "cover": 91582,
    "created_at": 1524843905,
    "external_games": [
      603512,
      1857496,
      1917109
    ],
    "first_release_date": 1527120000,
    "game_modes": [
      1
    ],
    "genres": [
      31,
      32
    ],
    "involved_companies": [
      155850,
      155852
    ],
    "name": "Sword of the Black Stone",
    "platforms": [
      6
    ],
    "player_perspectives": [
      2
    ],
    "release_dates": [
      188385
    ],
    "screenshots": [
      231528,
      231529,
      231530,
      231531,
      231532
    ],
    "similar_games": [
      25646,
      29783,
      68271,
      80916,
      96217,
      105269,
      106987,
      111130,
      113360,
      113895
    ],
    "slug": "sword-of-the-black-stone",
    "summary": "If your looking for action and adventure then Sword of the Black Stone is for you.",
    "tags": [
      1,
      268435487,
      268435488
    ],
    "themes": [
      1
    ],
    "updated_at": 1643857546,
    "url": "https://www.igdb.com/games/sword-of-the-black-stone",
    "websites": [
      84617
    ],
    "checksum": "0b575316-96dc-b056-86e0-c958c1c95efb"
  },
  {
    "id": 50524,
    "category": 0,
    "created_at": 1500048896,
    "external_games": [
      72115,
      1974683
    ],
    "first_release_date": 1499904000,
    "game_modes": [
      2
    ],
    "genres": [
      32
    ],
    "name": "OtterBash",
    "platforms": [
      6,
      14
    ],
    "release_dates": [
      93447,
      93448
    ],
    "screenshots": [
      110966,
      110967,
      110968,
      110969,
      110970
    ],
    "similar_games": [
      32902,
      37419,
      40524,
      76263,
      96217,
      105233,
      106987,
      111130,
      113895,
      119121
    ],
    "slug": "otterbash",
    "summary": "Otter Bash!!! The Otters have escaped their habitat and are now wreaking havoc for world dominance. Battle your friends with the cute and adorable little critters. Destroy your opponents with unique weapons; dousing each other with Tar Balloons, smack them with the nifty Taco Boomerang, shove them into your \"Portable Oven\" and see them roast!!!",
    "tags": [
      1,
      268435488
    ],
    "themes": [
      1
    ],
    "updated_at": 1643856963,
    "url": "https://www.igdb.com/games/otterbash",
    "websites": [
      52093,
      52094
    ],
    "checksum": "4a9c2852-25c3-91bb-ad34-5379507ce2d3"
  },
  // ...省略...
]
{{< /code >}}
{{< /accordion >}}


ここまでできれば、あとはいろんな API を試せますね。

٩(๑❛ᴗ❛๑)۶ わーぃ

