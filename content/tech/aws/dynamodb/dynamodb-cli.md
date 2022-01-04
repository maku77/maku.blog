---
title: "DynamoDB をコマンドライン (CLI) で操作する"
url: "/p/zkzamw8"
date: "2021-02-22"
tags: ["AWS", "AWS/DynamoDB"]
---

テーブルを作成する (dynamodb create-table)
----

{{< code >}}
aws dynamodb create-table --table-name <テーブル名> ...
{{< /code >}}

DynamoDB のテーブルを作成するときは、少なくともプライマリキーの設定や、課金モードの設定が必要になるので、少しだけコマンドが長くなります。

次の例では、DynamoDB に MusicCollection というテーブルを作成します。
`Artist` というパーティションキー (`KeyType=HASH`) と、`Artist` というソートキー (`KeyType=RANGE`) を定義しています。
課金体系は「プロビジョンドモード」で最小構成になるよう設定しています。

{{< code title="テーブル生成 (PartitionKey + SortKey)" >}}
$ aws dynamodb create-table \
    --table-name MusicCollection \
    --attribute-definitions AttributeName=Artist,AttributeType=S AttributeName=SongTitle,AttributeType=S \
    --key-schema AttributeName=Artist,KeyType=HASH AttributeName=SongTitle,KeyType=RANGE \
    --provisioned-throughput ReadCapacityUnits=1,WriteCapacityUnits=1
{{< /code >}}

パーティションキーのみで良い場合（ソートキーなし）は、次のような感じになります。

{{< code title="テーブル生成 (PartitionKey)" >}}
$ aws dynamodb create-table \
    --table-name MusicCollection \
    --attribute-definitions AttributeName=Artist,AttributeType=S \
    --key-schema AttributeName=Artist,KeyType=HASH \
    --provisioned-throughput ReadCapacityUnits=1,WriteCapacityUnits=1
{{< /code >}}

キャパシティモード（課金体系）をオンデマンド（本当にアクセスした分だけの支払い）にするには、__`--billing-mode PAY_PER_REQUEST`__ オプションを指定し、プロビジョン設定 (`--provisioned-throughput`) を省略します。
一定間隔でそれなりにアクセスがある場合は、プロビジョンドモード (`PROVISIONED`) にして常時稼働の形にしておいた方が総合的に安くなるようですが、テスト用途で作成する場合などは `PAY_PER_REQUEST` にしておくのが無難かと思います。

{{< code title="テーブル生成（ハッシュキーのみ）" hl_lines="5" >}}
$ aws dynamodb create-table \
    --table-name Books \
    --attribute-definitions AttributeName=BookId,AttributeType=S \
    --key-schema AttributeName=BookId,KeyType=HASH \
    --billing-mode PAY_PER_REQUEST
{{< /code >}}

テーブル定義を YAML ファイルや JSON ファイルに記述しておいて、それを読み込むこともできます。

{{< code title="テーブル定義をJSONファイルで指定する" >}}
$ aws dynamodb create-table --cli-input-json file://{JSONファイルの相対パス}
{{< /code >}}


テーブルを削除する (dynamodb delete-table)
----

{{< code >}}
aws dynamodb delete-table --table-name <テーブル名>
{{< /code >}}


テーブルの詳細情報を取得する (dynamodb describe-table)
----

{{< code >}}
aws dynamodb describe-table
{{< /code >}}

次の例では、`Games` テーブルの情報表示しています。
DynamoDB は基本的にスキーマレスなので、主にプライマリキー（Partition キー (HASH) と Sort キー (RANGE)）の情報だけ表示されます。

{{< code title="実行例（YAML形式）" >}}
$ aws dynamodb describe-table --table-name Games --output yaml

Table:
  AttributeDefinitions:
  - AttributeName: Hardware
    AttributeType: S
  - AttributeName: GameId
    AttributeType: S
  CreationDateTime: '2021-07-21T16:54:17.255000+09:00'
  ItemCount: 2
  KeySchema:
  - AttributeName: Hardware
    KeyType: HASH
  - AttributeName: GameId
    KeyType: RANGE
  ProvisionedThroughput:
    LastDecreaseDateTime: '1970-01-01T09:00:00+09:00'
    LastIncreaseDateTime: '1970-01-01T09:00:00+09:00'
    NumberOfDecreasesToday: 0
    ReadCapacityUnits: 1
    WriteCapacityUnits: 1
  TableArn: arn:aws:dynamodb:ddblocal:000000000000:table/Games
  TableName: Games
  TableSizeBytes: 134
  TableStatus: ACTIVE
{{< /code >}}


テーブルにアイテムを追加する (dynamodb put-item)
----

{{< code >}}
aws dynamodb put-item
{{< /code >}}

次の例では、MusicCollection テーブルにアイテム（項目）を 1 つ追加します。

{{< code title="実行例" >}}
$ aws dynamodb put-item \
    --table-name MusicCollection \
    --item '{
        "Artist": {"S": "No One You Know"},
        "SongTitle": {"S": "Call Me Today"} ,
        "AlbumTitle": {"S": "Somewhat Famous"}
      }' \
    --return-consumed-capacity TOTAL

{
    "ConsumedCapacity": {
        "CapacityUnits": 1.0,
        "TableName": "MusicCollection"
    }
}
{{< /code >}}


テーブルから全てのアイテムを読み取る (dyanmodb scan)
----

{{< code >}}
aws dynamodb scan
{{< /code >}}

次の例では、Games テーブル内のすべてアイテムを取得しています。

{{< code title="実行例（YAML形式）" >}}
$ aws dynamodb scan --table-name Games --output yaml

ConsumedCapacity: null
Count: 2
Items:
- GameId:
    S: 1990-SuperMarioWorld
  Hardware:
    S: SNES
  Maker:
    S: Nintendo
  Title:
    S: Super Mario World
- GameId:
    S: 1983-DonkeyKong
  Genre:
    S: ACT
  Hardware:
    S: NES
  Players:
    N: '2'
  Title:
    S: Donkey Kong
ScannedCount: 2
{{< /code >}}


テーブルから 1 つのアイテムを読み取る (dynamodb get-item)
----

{{< code >}}
aws dynamodb get-item
{{< /code >}}

次の例では、`my-table` という名前の DynamoDB テーブルからアイテムを取得します。

{{< code title="実行例" >}}
$ aws dynamodb get-item --table-name my-table --key '{"id": {"N":"1"}}'

{
    "Item": {
        "name": {
            "S": "John"
        },
        "id": {
            "N": "1"
        }
    }
}
{{< /code >}}


{{< private >}}
有効な JSON を 1 行のコマンドで作成するのは難しい場合があります。これを簡単にするために、AWS CLI は JSON ファイルを読み取ることができます。たとえば、expression-attributes.json という名前のファイルに格納されている次の JSON スニペットがあるとします。

{{< code lang="json" >}}
{
  ":v1": {"S": "No One You Know"},
  ":v2": {"S": "Call Me Today"}
}
{{< /code >}}

そのファイルを使用して、AWS CLI を使用する query リクエストを発行することができます。次の例では、expression-attributes.json ファイルの内容が --expression-attribute-values パラメータの値に使用されます。

{{< code >}}
$ aws dynamodb query --table-name MusicCollection \
    --key-condition-expression "Artist = :v1 AND SongTitle = :v2" \
    --expression-attribute-values file://expression-attributes.json
{
    "Count": 1,
    "Items": [
        {
            "AlbumTitle": {
                "S": "Somewhat Famous"
            },
            "SongTitle": {
                "S": "Call Me Today"
            },
            "Artist": {
                "S": "No One You Know"
            }
        }
    ],
    "ScannedCount": 1,
    "ConsumedCapacity": null
}
{{< /code >}}
{{< /private >}}

