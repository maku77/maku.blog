---
title: "Azure Table Strage を使ってみる (3) Python からテーブル操作してみる"
url: "/p/o8ufwct"
date: "2020-01-29"
tags: ["Azure Table Storage", "Python"]
weight: 103
---

azure-cosmosdb-table パッケージのインストール
----

Python から Azure Table Storage を操作するには、[`azure-cosmosdb-table`](https://pypi.org/project/azure-cosmosdb-table/) というライブラリを使用します。

{{< code title="azure-cosmosdb-table のインストール" >}}
$ pip install azure-cosmosdb-table
{{< /code >}}

{{% note %}}
Cosmos DB をまったく使わない場合でも cosmosdb と名前のついたライブラリを使わせようとするのは、 ~~Microsoft の策略~~ Cosmos DB に力を入れているという意思表示でしょう。
正直なところ Cosmos DB はお金がかかりすぎて個人の趣味レベルでは使えないのですが。。。

（追記: 2021年）CosmosDB に無料枠ができて少しずつ個人利用もできそうな感じになってきました。
{{% /note %}}


TableService オブジェクトの生成
----

Azure Storage にアクセスするには、接続情報（ストレージアカウント名とキー）が必要になるので、[Azure ポータル](https://portal.azure.com/) で確認しておいてください。

- 参考: [Azure Storage の接続情報（キー）を確認する](/p/dofzeua)

Table Storage にアクセスしてごにょごにょするには、[TableService クラス](https://docs.microsoft.com/ja-jp/python/api/azure-cosmosdb-table/azure.cosmosdb.table.tableservice.tableservice?view=azure-python) のメソッドを使用します。
`TableService` のコンストラクタには、ストレージアカウント名とキーを渡します。

{{< code lang="python" >}}
from azure.cosmosdb.table.tableservice import TableService

STORAGE_NAME = 'maku77storage'
STORAGE_KEY = 'XlgKN4Hv...(省略)...F55o3N9g=='

table_service = TableService(account_name=STORAGE_NAME, account_key=STORAGE_KEY)
{{< /code >}}

{{< note title="URL の指定は必要ない？" >}}
接続先アドレス (URL) の構築は `TableService` クラスがよろしくやってくれるので、実装コードがとてもスッキリします。
ストレージアカウント名は Azure 内（世界中）で一意になっているので、ストレージアカウント名さえ指定すれば、Web API の URL も自動的に決まるということですね。
{{< /code >}}

### ストレージアカウントのキーを環境変数から取得する

ストレージアカウントにアクセスするためのキー情報をスクリプト内にハードコーディングするのは望ましくないので、環境変数などから取得するようにしておくとよいでしょう。

{{< code lang="python" title="環境変数から Azure Storage のアクセスキーを取得" >}}
import os
import sys

if 'AZURE_STORAGE_KEY' not in os.environ:
    print('Error: AZURE_STORAGE_KEY not found', file=sys.stderr)
    sys.exit(1)
storage_key = os.getenv('AZURE_STORAGE_KEY')
{{< /code >}}

{{% private %}}
- 参考: [Node.js で .env ファイルを使う](https://maku77.github.io/nodejs/env/dotenv.html)
{{% /private %}}

### プロキシ環境からアクセスする場合

`TableService` クラスをプロキシ環境内で使用する場合は、OS の **`https_proxy`** 環境変数で設定してしまうのが簡単です。
OS の `https_proxy` 環境変数を設定するのを避けたいときは、Python スクリプトの中からプロキシを設定してしまうこともできます。

{{< code lang="python" title="プロキシ情報をハードコードする" >}}
import os
os.environ['https_proxy'] = 'http://proxy.example.com:12345'
{{< /code >}}

OS の `https_proxy` 環境変数を設定するのも嫌だし、Python スクリプト内にプロキシ情報をハードコードするのも嫌なときは、例えば、独自の `AZURE_PROXY` のような **独自の環境変数** を用意して、その値をプロキシ情報として使用するという方法があります。

{{< code lang="python" title="AZURE_PROXY 環境変数の値をプロキシ情報として使う" >}}
if 'AZURE_PROXY' in os.environ:
    os.environ['https_proxy'] = os.getenv('AZURE_PROXY')
{{< /code >}}


テーブルの操作
----

ここから先のテーブル操作に関しては、前述の `TableService` オブジェクトの生成が終わっていることを想定しています。

### テーブルのリストを取得する (list_tables)

Table Storage 上に存在するテーブルの一覧を取得するには、`TableService` クラスの [list_tables() メソッド](https://docs.microsoft.com/ja-jp/python/api/azure-cosmosdb-table/azure.cosmosdb.table.tableservice.tableservice?view=azure-python#list-tables-num-results-none--marker-none--timeout-none-) を使用します。
このメソッドの戻り値を for ループで処理すると、[azure.cosmosdb.table.models.Table](https://docs.microsoft.com/ja-jp/python/api/azure-cosmosdb-table/azure.cosmosdb.table.models.table?view=azure-python) オブジェクトを 1 つずつ取り出すことができます。
テーブル名を示す `name` プロパティしかありませんが。。。

{{< code lang="python" title="すべてのテーブルを列挙" >}}
for table in table_service.list_tables():
    print(table.name)
{{< /code >}}

仮に、`books` と `todos` というテーブルが作成済みであれば、次のように表示されます。

{{< code title="実行結果" >}}
books
todos
{{< /code >}}

### テーブルの存在を確認する (exist)

Table Storage 上に、指定した名前のテーブルが存在するかどうかをチェックするには、`TableService` クラスの [exists() メソッド](https://docs.microsoft.com/ja-jp/python/api/azure-cosmosdb-table/azure.cosmosdb.table.tableservice.tableservice?view=azure-python#exists-table-name--timeout-none-) を使用します。

{{< code lang="python" >}}
if table_service.exists('books'):
    print('books テーブルが存在するよ')
{{< /code >}}

### テーブルを作成する (create_table)

Table Storage 上に、新しいテーブルを作成するには、`TableService` クラスの [create_table() メソッド](https://docs.microsoft.com/ja-jp/python/api/azure-cosmosdb-table/azure.cosmosdb.table.tableservice.tableservice?view=azure-python#create-table-table-name--fail-on-exist-false--timeout-none-) を使用します。
テーブルの作成に成功すると `True` を返します。

{{< code lang="python" >}}
result = table_service.create_table('mytable')
if result:
    print('テーブルを作成しました')
{{< /code >}}

{{< note >}}
テーブルのプロパティ（RDB のカラムに相当するもの）の指定がありませんが、プロパティはエンティティを追加するときに自動的に生成されるので、ここで指定しておく必要はありません。
{{< /note >}}

### テーブルを削除する (delete_table)

Table Storage 上の、既存のテーブルを削除するには、`TableService` クラスの [delete_table() メソッド](https://docs.microsoft.com/ja-jp/python/api/azure-cosmosdb-table/azure.cosmosdb.table.tableservice.tableservice?view=azure-python#delete-table-table-name--fail-not-exist-false--timeout-none-) を使用します。
テーブルを削除すると、そこに含まれているすべてのエンティティも削除されてしまうので注意してください。

{{< code lang="python" >}}
table_service.delete_table('mytable')
{{< /code >}}


エンティティの操作
----

### テーブルにエンティティを追加する (insert_xxx)

指定したテーブルにデータ（エンティティ）を追加するには、`TableService` クラスの下記のいずれかのメソッドを使用します。
同じキー情報を持つエンティティを追加しようとしたときの振る舞いだけが異なります。

[insert_entity()](https://docs.microsoft.com/ja-jp/python/api/azure-cosmosdb-table/azure.cosmosdb.table.tableservice.tableservice?view=azure-python#insert-entity-table-name--entity--timeout-none-)
: テーブル内に既に同じ `PartitionKey` と `RowKey` を持つエンティティがある場合は、例外を発生します。

[insert_or_merge_entity()](https://docs.microsoft.com/ja-jp/python/api/azure-cosmosdb-table/azure.cosmosdb.table.tableservice.tableservice?view=azure-python#insert-or-merge-entity-table-name--entity--timeout-none-)
: テーブル内に既に同じ `PartitionKey` と `RowKey` を持つエンティティがある場合は、指定したプロパティだけ上書きされます（既存エンティティのプロパティとマージされる）。

[insert_or_replace_entity()](https://docs.microsoft.com/ja-jp/python/api/azure-cosmosdb-table/azure.cosmosdb.table.tableservice.tableservice?view=azure-python#insert-or-replace-entity-table-name--entity--timeout-none-)
: テーブル内に既に同じ `ParitionKey` と `RowKey` を持つエンティティがある場合は、指定したエンティティに置き換えられます。

これらのメソッドのパラメータはすべて同じで、追加先のテーブル名と、追加するエンティティを渡します。

{{< code lang="python" >}}
insert_entity(table_name, entity, timeout=None)
insert_or_merge_entity(table_name, entity, timeout=None)
insert_or_replace_entity(table_name, entity, timeout=None)
{{< /code >}}

`entity` パラメータには、辞書オブジェクトか [Entity クラス](https://docs.microsoft.com/ja-jp/python/api/azure-cosmosdb-table/azure.cosmosdb.table.models.entity?view=azure-python) のオブジェクトを渡すことができます。

{{< code lang="python" title="辞書でデータを指定する方法" >}}
book = {
    'PartitionKey': 'book',  # 必須のキー情報
    'RowKey': '0004',        # 必須のキー情報
    'Title': '雪男の秘密',
    'Author': '雪男'
}
table_service.insert_or_replace_entity('books', book)
{{< /code >}}

{{< code lang="python" title="Entity オブジェクトでデータを指定する方法" >}}
from azure.cosmosdb.table.models import Entity

book = Entity()
book.PartitionKey = 'book'  # 必須のキー情報
book.RowKey = '0004'        # 必須のキー情報
book.Title = '雪男の秘密'
book.Author = '雪男'
table_service.insert_or_replace_entity('books', book)
{{< /code >}}

別に `Entity` オブジェクトを使ったところで型安全になるわけでもないので、辞書オブジェクトをそのまま使った方が楽かもしれません。

{{< note title="存在しないプロパティは自動で追加される" >}}
`insert` 系メソッドでエンティティを追加するときに、**テーブルにまだ存在しないプロパティを指定することもできます**。
その場合、自動的にテーブルにそのプロパティが追加されるので、プロパティ名は間違えないように要注意です。
{{< /note >}}

### 指定したキーのエンティティを取得する (get_entity)

取得したいエンティティのキー（`PartitionKey` と `RowKey`）が分かっている場合は、`TableService` クラスの [get_entity() メソッド](https://docs.microsoft.com/ja-jp/python/api/azure-cosmosdb-table/azure.cosmosdb.table.tableservice.tableservice?view=azure-python#get-entity-table-name--partition-key--row-key--select-none--accept--application-json-odata-minimalmetadata---property-resolver-none--timeout-none-) を使って `Entity` オブジェクトを取得することができます。
指定したキーのエンティティが見つからない場合は、例外が発生します。

{{< code lang="python" >}}
book = table_service.get_entity(
    table_name='books',
    partition_key='book',
    row_key='0004')

print(book.Title)
print(book.Author)
{{< /code >}}

特定のプロパティしか参照しないことが分かっている場合は、`select` パラメータでプロパティ名を列挙しておくと、通信量を節約することができます。

{{< code lang="python" >}}
book = table_service.get_entity(
    table_name='books',
    partition_key='book',
    row_key='0004',
    select='Title,Author')  # Title と Author プロパティだけ取得
{{< /code >}}

### 指定したキーのエンティティを削除する (delete_entity)

指定したキー（`PartitionKey` と `RowKey`）のエンティティを削除するには、`TableService` クラスの [delete_entity() メソッド](https://docs.microsoft.com/ja-jp/python/api/azure-cosmosdb-table/azure.cosmosdb.table.tableservice.tableservice?view=azure-python#delete-entity-table-name--partition-key--row-key--if-match------timeout-none-) を使用します。
指定したキーのエンティティが見つからない場合は、例外が発生します。

{{< code lang="python" >}}
table_service.delete_entity(
    table_name='books',
    partition_key='book',
    row_key='0004')
{{< /code >}}

### エンティティのリストを取得する (query_entities)

テーブルから条件に一致するエンティティをすべて取得するには、`TableService` クラスの [query_entities() メソッド](https://docs.microsoft.com/ja-jp/python/api/azure-cosmosdb-table/azure.cosmosdb.table.tableservice.tableservice?view=azure-python#query-entities-table-name--filter-none--select-none--num-results-none--marker-none--accept--application-json-odata-minimalmetadata---property-resolver-none--timeout-none-) を使用します。

{{< code lang="python" title="条件なしで検索" >}}
books = table_service.query_entities(
    table_name='books',
    num_results=5)

for book in books:
    print(book.Title, book.Author)
{{< /code >}}

検索条件は、**`filter`** パラメータを使って指定することができます。`filter` パラメータには文字列でフィルタ条件を指定するのですが、どのようなフォーマットで指定すればよいかは、下記のサイトが参考になります。

- 参考: [Querying tables and entities (REST API) - Azure Storage | Microsoft Docs](https://docs.microsoft.com/ja-jp/rest/api/storageservices/Querying-Tables-and-Entities)
- 参考: [Azure ストレージ テーブルの設計パターン | Microsoft Docs](https://docs.microsoft.com/ja-jp/azure/storage/tables/table-storage-design-patterns#inter-partition-secondary-index-pattern)

#### 例: PartitionKey が Python である

`filter` パラメータに `ParitionKey eq 'Python'` と指定すると、`PartitionKey` が `Python` であるエンティティをすべて取得することができます。

{{< code lang="python" >}}
python_books = table_service.query_entities(
    'books', filter="PartitionKey eq 'Python'")

for book in python_books:
    print(book.Title, book.Author)
{{< /code >}}


#### 例: RowKey が 0002 より大きい

`filter` パラメータに `RowKey gt '0002'` と指定すると、`RowKey` が `0002` よりも大きいエンティティのみを取得することができます。

{{< code lang="python" title="filter 条件を指定して検索" >}}
books = table_service.query_entities(
    table_name='books',
    filter="RowKey gt '0002'")

for book in books:
    print(book.Title, book.Author)
{{< /code >}}

{{< code titiel="実行結果" >}}
へむの秘密 へむ
ちいの秘密 ちい
雪男の秘密 雪男
{{< /code >}}


バッチ処理化
----

複数のエンティティを追加したい場合などは、バッチ処理で行うようにすると、サーバーとの通信回数を減らすことができます。
バッチ処理を行うには、`TableService` クラスの [batch() メソッド](https://docs.microsoft.com/ja-jp/python/api/azure-cosmosdb-table/azure.cosmosdb.table.tableservice.tableservice?view=azure-python#batch-table-name--timeout-none-) を使用します。

{{< code lang="python" >}}
book1 = {'PartitionKey': 'book', 'RowKey': '001', Title: 'Title1'}
book2 = {'PartitionKey': 'book', 'RowKey': '002', Title: 'Title2'}
book3 = {'PartitionKey': 'book', 'RowKey': '003', Title: 'Title3'}

with table_service.batch('books') as batch:
    batch.insert_entity(book1)
    batch.insert_entity(book2)
    batch.insert_entity(book3)
{{< /code >}}

