---
title: "DynamoDB を Python で操作する (boto3)"
url: "/p/wht5epz"
date: "2021-11-02"
tags: ["AWS", "AWS/DynamoDB", "boto3"]
---

{{% private %}}
- [Boto3 - DynamoDB サンプルコード](https://boto3.amazonaws.com/v1/documentation/api/latest/guide/dynamodb.html)
- [Boto3 - DynamoDB API リファレンス](https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/dynamodb.html)
- [Python および DynamoDB を使用する開発方法 - Amazon DynamoDB](https://docs.aws.amazon.com/ja_jp/amazondynamodb/latest/developerguide/GettingStarted.Python.html)
{{% /private %}}

（AWS SDK を使うときは、[aws configure](https://docs.aws.amazon.com/ja_jp/cli/latest/userguide/cli-configure-quickstart.html) によるアクセスキーの設定は完了しているものと想定します）

Boto3 のインストール
----

Python 用の AWS SDK として [Boto3](https://aws.amazon.com/jp/sdk-for-python/) が用意されているので、これをインストールして使います。
PC のグローバル環境を汚さないように、[venv による仮想環境を作って作業する](https://maku77.github.io/python/env/venv.html) ことをオススメします。

まず、仮想環境を作ってそこに入ります。

```
$ mkdir myapp && cd myapp    # アプリ用のディレクトリを作成
$ python3 -m venv .venv      # 仮想環境の作成
$ source .venv/bin/activate  # 仮想環境に入る
```

仮想環境 (`.venv`) 内に __`boto3`__ パッケージをインストールします。

```
(.venv) $ python3 -m pip install boto3
```

これで準備完了です。簡単！


高レベル API と低レベル API
----

Boto3 の API は、抽象度の高い API と、低い API の二種類が用意されています。

高レベル API（リソース API）
: 各 AWS リソースを、オブジェクト指向なコードで扱うことができる。`boto3.resource(リソース名)` でインスタンスを取得できる。（参考: [Resources](https://boto3.amazonaws.com/v1/documentation/api/latest/guide/resources.html)）

低レベル API（クライアント API）
: AWS のサービス API と 1:1 で対応する構成になっており、各種 API の戻り値は単純な dict オブジェクト。`boto3.client(リソース名)` でインスタンスを取得できる。（参考: [Low-level clients](https://boto3.amazonaws.com/v1/documentation/api/latest/guide/clients.html)、[client 関数](https://boto3.amazonaws.com/v1/documentation/api/latest/reference/core/session.html#boto3.session.Session.client)）

```python
import boto3

# 高レベル API を使うとき
dynamodb = boto3.resource('dynamodb')

# 低レベル API を使うとき
dynamodb_client = boto3.client('dynamodb')
```

リソース API は、内部でクライアント API を使って実装されています。


DynamoDB Local サーバーに接続する
----

```python
# 高レベル API を使うとき
dynamodb = boto3.resource('dynamodb', endpoint_url='http://localhost:8000')

# 低レベル API を使うとき
dynamodb_client = boto3.client('dynamodb', endpoint_url='http://localhost:8000')
```

dynamodb のリソースインスタンス、あるいはクライアントインスタンスを生成するときに、上記のように __`endpoint_url`__ を指定することで、ローカルで実行している DynamoDB Local サーバーに接続することができます。


DynamoDB テーブルのリストを取得する
----

### 高レベル API

[dynamodb_resource.tables.all()](https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/dynamodb.html#DynamoDB.ServiceResource.tables) で全ての DynamoDB テーブルを取得できます。
戻り値は [Dynamodb.Table](https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/dynamodb.html#table) のリストです。

```python
import boto3

dynamodb = boto3.resource('dynamodb')
# dynamodb = boto3.resource('dynamodb', endpoint_url='http://localhost:8000')

# すべてのテーブルの名前を列挙する
for tbl in dynamodb.tables.all():
    print(tbl.name)
```

### 低レベル API

[DynamoDB.Client.list_tables](https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/dynamodb.html#DynamoDB.Client.list_tables) メソッドでテーブルの一覧を取得できます。
戻り値は辞書オブジェクトで、`TableNames` プロパティにテーブル名の文字列リストが入っています。

```python
import boto3

dynamodb_client = boto3.client('dynamodb')
res = dynamodb_client.list_tables()

print(res['TableNames'])
```

{{< code title="実行結果" >}}
['Books', 'Games', 'Movies']
{{< /code >}}


DynamoDB テーブルを作成する (create_table リソース API)
----

DynamoDB テーブルを作成するには、[create_table](https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/dynamodb.html#DynamoDB.ServiceResource.create_table) リソース API を使用します。
テーブルのプライマリキーの定義方法は相変わらず古くてわかりにくい方法でしか記述できません。
次の例では、`Books` テーブルを作成して、テーブルが生成されるまで待機しています。
プライマリキーとしては、パーティションキーの `BookId`、ソートキーの `Title` を設定しています。

{{< code lang="python" title="Books テーブルを作成する" >}}
import boto3

dynamodb = boto3.resource('dynamodb')
# dynamodb = boto3.resource('dynamodb', endpoint_url='http://localhost:8000')

def create_books_table():
    table = dynamodb.create_table(
        TableName='Books',
        KeySchema=[
            {
                'AttributeName': 'BookId',
                'KeyType': 'HASH'
            },
            {
                'AttributeName': 'Title',
                'KeyType': 'RANGE'
            }
        ],
        AttributeDefinitions=[
            {
                'AttributeName': 'BookId',
                'AttributeType': 'S'
            },
            {
                'AttributeName': 'Title',
                'AttributeType': 'S'
            },
        ],
        # オンデマンドにする場合
        BillingMode='PAY_PER_REQUEST'
        # プロビジョンドにする場合
        # ProvisionedThroughput={
        #    'ReadCapacityUnits': 1,
        #    'WriteCapacityUnits': 1
        #}
    )
    return table

if __name__ == '__main__':
    print('Creat table...')
    table = create_games_table()

    table.wait_until_exists()
    print('Created!')
    print('Table status:', table.table_status)
    print('Item count:', table.item_count)
{{< /code >}}

テーブルがすでに存在する場合は、`ResourceInUseException` 例外が発生します。

`create_table` 引数には、他にも次のようなものを指定できます。

{{< code lang="python" >}}
# タグ
Tags=[
    { 'Key': 'key-1', 'Value': 'value-1' },
    { 'Key': 'key-2', 'Value': 'value-2' },
]

# GSI（グローバル・セカンダリー・インデックス）
GlobalSecondaryIndexes=[
    {
        'IndexName': 'string',
        'KeySchema': [
            {
                'AttributeName': 'string',
                'KeyType': 'HASH'|'RANGE'
            },
        ],
        'Projection': {
            'ProjectionType': 'ALL'|'KEYS_ONLY'|'INCLUDE',
            'NonKeyAttributes': [
                'string',
            ]
        },
        'ProvisionedThroughput': {
            'ReadCapacityUnits': 10,
            'WriteCapacityUnits': 10
        }
    },
]
{{< /code >}}


テーブルが必要なくなったら、次の AWS CLI コマンドで削除できます。

```
$ aws dynamodb delete-table --table-name Books
```

DynamoDB テーブルを削除する（table.delete / client.delete_table）
----

DynamoDB 内の既存のテーブル削除するには、下記のような API を使用します。
テーブル名を指定するだけなので、どちらも使い方はほぼ同じです。

- リソース API（高レベル）: [DynamoDB.Table.delete メソッド](https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/dynamodb.html#DynamoDB.Table.delete)
- クライアント API（低レベル）: [DynamoDB.ServiceResource.delete_table メソッド](https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/dynamodb.html#DynamoDB.Client.delete_table)

テーブル内のアイテムがすべて削除されるので、注意して実行してください。

{{< code lang="python" title="リソース API の場合 (Table.delete)" >}}
import boto3

# Books テーブルを参照する
dynamodb = boto3.resource('dynamodb').Table('Books')

# Books テーブルを削除する
table.delete()
{{< /code >}}

{{< code lang="python" title="クライアント API の場合 (delete_table)" >}}
import boto3

dynamodb_client = boto3.client('dynamodb')
dynamodb_client.delete_table(TableName='Books')
{{< /code >}}

存在しないテーブルを削除しようとすると、`ResourceNotFoundException` が発生します。
テーブルを削除すると、そこに設定されている DynamoDB Stream も 24 時間以内に削除されます。


DynamoDB テーブルにアイテムを追加する（put_item リソース API）
----

DynamoDB テーブルにアイテムを追加するには、[DynamoDB.Table.put_item](https://boto3.amazonaws.com/v1/documentation/api/latest/guide/dynamodb.html#creating-a-new-item) メソッドを使用します。

次の例では、既存の `Books` テーブルに、3 つのアイテムを追加しています。

{{< code lang="python" >}}
import boto3

# Books テーブルを参照する
table = boto3.resource('dynamodb').Table('Books')

# Books テーブルにアイテムを追加する
table.put_item(Item={'BookId': '001', 'Title': 'Title-1'})
table.put_item(Item={'BookId': '002', 'Title': 'Title-2'})
table.put_item(Item={'BookId': '003', 'Title': 'Title-3'})
{{< /code >}}

次の AWS CLI コマンドで、ちゃんとアイテムが追加できているかを確認できます。

```
$ aws dynamodb scan --table-name Books
```

DynamoDB テーブルに効率的に書き込む（batch_writer リソース API）
----

何度も `put_item` や `delete_item` をしたいときは、`DynamoDB.Table` オブジェクトのメソッドを直接呼び出すのではなく、[batch_writer](https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/dynamodb.html#DynamoDB.Table.batch_writer) メソッドで取得したバッチライターオブジェクト経由で実行すると、まとまった単位で効率的に書き込みを行うことができます（通信回数が減ります）。

```python
import boto3

# Books テーブルを参照する
table = boto3.resource('dynamodb').Table('Books')

# Books テーブルに複数のアイテムを追加する
with table.batch_writer() as batch:
    for i in range(10):
        book_id = '%03d' % i
        title = 'Title-%d' % i
        batch.put_item(Item={'BookId': book_id, 'Title': title})
```

DynamoDB テーブルをスキャンする（全てのアイテムを取得する） (scan)
----

[DynamoDB.Table.scan](https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/dynamodb.html#DynamoDB.Table.scan) メソッドで、テーブル内の全てのアイテムを取得（スキャン）することができます。

スキャン結果のアイテムのリストは、`scan()` の戻り値の __`Items`__ プロパティに格納されています。
多数のアイテムが格納されているテーブルに対してスキャンすると、RCU の消費量が大きくなるので注意してください。
__`Limit`__ 引数で取得件数を制限すれば、消費 RCU も減ります。

### 基本的なスキャン

```python
import boto3

# Games テーブルから 5 件のアイテムを取得
table = boto3.resource('dynamodb').Table('Games')
response = table.scan(Limit=5, ReturnConsumedCapacity='TOTAL')

# スキャン結果の表示
items = response['Items']
rcu = response['ConsumedCapacity']['CapacityUnits']
print('Scanned items: %d' % len(items))
print('Consumed capacity: %.2f RCU' % rcu)
for item in items:
    print(item['Date'], item['Title'])
```

{{< code title="実行結果" >}}
Scanned items: 5
Consumed capacity: 0.50 RCU
1993-07-30 バリ・アーム
1994-03-25 ダンジョンマスターII スカルキープ
1994-09-22 スター・ウォーズ レベル・アサルト
1993-11-19 ナイトトラップ
1992-09-11 ブライ 八玉の勇士伝説
{{< /code >}}

### 合計 1MB 以上のアイテムをスキャンする

DynamoDB の制約として、一度に 1MB までのデータしかスキャンできないという制約があります（クエリの場合も同様です）。
大量のアイテムを一気に取得したいときは、戻り値オブジェクトに含まれる __`LastEvaluatedKey`__ プロパティを使って、繰り返しスキャンを行う必要があります。
次の `scan()` の __`ExclusiveStartKey`__ 引数にその値を渡すことで、続きのアイテムを取得することができます。

```python
import boto3

table = boto3.resource("dynamodb").Table("Games")

# 繰り返しスキャンして全てのアイテムを取得
def scan_all_games():
    games = []
    response = table.scan()
    while True:
        games.extend(response["Items"])
        if "LastEvaluatedKey" not in response:
            break
        response = table.scan(ExclusiveStartKey=response["LastEvaluatedKey"])
    return games

# 全スキャンの実行と結果の表示
games = scan_all_games()
print("%d items" % len(games))
for game in games:
    print(game["Date"], game["Title"])
```


DynamoDB テーブルから 1 つのアイテムを取得する (get_item)
----

[DynamoDB.Table.get_item](https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/dynamodb.html#DynamoDB.Table.get_item) メソッドで、指定したプライマリキーに一致するアイテムを 1 件取得することができます。
取得結果は、戻り値の __`Item`__ プロパティに格納されています。
アイテムが見つからなかった場合は、`Item` プロパティ自体が存在しなくなります。

```python
import boto3

# Games テーブルから指定したプライマリキーに一致するアイテムを取得
table = boto3.resource("dynamodb").Table("Games")
response = table.get_item(Key={"GameId": "zjorzdp"})

# 取得結果を表示
if "Item" in response:
    item = response["Item"]
    print(item["Date"], item["Title"])
else:
    print("Not found")
```


（おまけ）DynamoDB から取得した結果を JSON 文字列に変換する
----

Python 標準モジュールの `json.dumps` 関数を使うと、任意のオブジェクトを JSON 形式の文字列に変換することができますが、DynamoDB のデータを変換するときは注意が必要です。
DynamoDB の数値 (number) カラムの値を取得すると、Python では `Decimal` 型として保持されるのですが、JSON では数値は `float` 型しか表現できません。
そのため、DynamoDB から取得したデータをそのまま JSON 文字列に変換しようとすると、次のようなエラーが発生します。

```
TypeError: Object of type Decimal is not JSON serializable
```

`json.dumps` 関数の __`default`__ 引数でフック関数を渡すと、JSON 形式に変換できない値（今回の場合は `Decimal`）が見つかったときに、どのように変換すべきかを定義することができます。

```python
import boto3
import decimal
import json

# json.dumps() のシリアライズ用フック（Decimal → float）
def decimal_serializer(obj):
    if isinstance(obj, decimal.Decimal):
        return float(obj)
    raise TypeError

if __name__ == "__main__":
    # DynamoDB の Games テーブルをスキャンして JSON 文字列で出力
    response = boto3.resource("dynamodb").Table("Games").scan()
    json_text = json.dumps(
        response, indent=2, ensure_ascii=False, default=decimal_serializer
    )
    print(json_text)
```

