---
title: "Python で Jira の情報を取得する"
url: "p/thhqqc7/"
date: "2023-12-13"
tags: ["jira", "python"]
---

プロジェクト管理ツールである Jira は、REST API を使って情報を取得できるようになっています。
ここでは、Python を使って Jira の情報を取得してみます。


jira パッケージのインストール
----

Python から JIRA の情報を取得するには、PyPI で公開されている [jira パッケージ](https://pypi.org/project/jira/) を使用すると簡単です（`jira` パッケージ内部で REST API を使っています）。

{{< code lang="console" title="jira パッケージのインストール" >}}
$ pip install jira
{{< /code >}}


jira パッケージの基本的な使い方
----

下記は、JIRA プロジェクトの一覧を取得する簡単な例です。
ユーザー ID やパスワードは適切なものに置き換えてください。

{{< code lang="python" title="jira-projects.py" >}}
from jira import JIRA, JIRAError

JIRA_SERVER = "https://<server>"
JIRA_USER = "<user>"
JIRA_PW = "<password>"

try:
    jira = JIRA(JIRA_SERVER, basic_auth=(JIRA_USER, JIRA_PW))
    for prj in jira.projects():
        print(f"key: {prj.key}, name: {prj.name}")
except JIRAError as e:
    print(f"Error: {e.status_code} {e.url}")
{{< /code >}}

{{< code title="実行結果" >}}
Key: TST, Name: A Test Project
Key: TST2, Name: A Test Project 2
Key: ANALYTICS, Name: Analytics
Key: ANTIABUSETEST, Name: Anti-Abuse Test
Key: ATLAS, Name: Atlas
Key: ACCESS, Name: Atlassian Access
Key: CLOUD, Name: Atlassian Cloud
...
{{< /code >}}

Jira Cloud サービスを使用している場合、パスワードをそのまま指定する方法は [非推奨になった](https://developer.atlassian.com/cloud/jira/platform/deprecation-notice-basic-auth-and-cookie-based-auth/) ので、API トークンを発行してパスワードの代わりに指定します。

```python
jira = JIRA(basic_auth=("email", "API token"))
```

ユーザー名やパスワードの代わりに、パーソナルアクセストークン (PAT) を使うときは次のようにします。
パーソナルアクセストークンを使う場合は、コード内でユーザー名を指定する必要はありません。
パーソナルアクセストークンは、Jira の右上のユーザーアイコンをクリックして、`Profile` ページを開くと作成できます。

```python
JIRA_SERVER = "https://<server>"
JIRA_TOKEN = "JiCI65qxea............省略............MzYzOn3Jv1"

jira = JIRA(JIRA_SERVER, token_auth=JIRA_TOKEN)
```


接続情報を環境変数で設定する
----

実用上は、Jira のアクセストークンは環境変数や [`.env` ファイルで設定](https://maku77.github.io/p/gzo8d7y/) できるようにしておくのがよいです。
こうしておけば、CI 環境（GitHub Actions など）のシークレット変数としてトークンを設定できます。

{{< code lang="inf" title=".env" >}}
JIRA_SERVER=https://your-jira-server
JIRA_TOKEN=your-access-token
{{< /code >}}

次の `config` モジュールでは、環境変数あるいは `.env` ファイルで設定された `JIRA_SERVER` 変数と `JIRA_TOKEN` 変数の値を取得しています。
これらの変数が設定されていなければエラーを表示して終了します。

{{< code lang="python" title="config.py" >}}
import os
import sys
from dotenv import load_dotenv

# Load environment variables from .env file
load_dotenv()

def load_env_or_exit(env_name: str) -> str:
    """
    Get the value of the environment variable.
    If it is not set, an error message is displayed and the program is terminated.
    """
    env_val = os.getenv(env_name)
    if not env_val:
        sys.exit(
            f"Error: {env_name} not set. Please consider adding a .env file with {env_name}."
        )
    return env_val

# Export configuration variables
JIRA_SERVER = load_env_or_exit("JIRA_SERVER")
JIRA_TOKEN = load_env_or_exit("JIRA_TOKEN")
{{< /code >}}

上記の `config` モジュールを使うと、`JIRA` インスタンスの生成部分は次のように簡潔に記述できます。

{{< code lang="python" title="使用例" >}}
from jira import JIRA
import config

jira = JIRA(config.JIRA_SERVER, token_auth=config.JIRA_TOKEN)

sorted_projects = sorted(jira.projects(), key=lambda x: x.key)
for project in sorted_projects:
    print(f"{project.key}:\t{project.name}")
{{< /code >}}


スニペット集
----

{{% private %}}
https://jira.readthedocs.io/examples.html
{{% /private %}}

### プロジェクト内のイシューをすべて取得する

```python
issues = jira.search_issues("project = PROJECT_KEY")
for issue in issues:
    print(f"{issue.key}:\t{issue.fields.summary}")
```

`jira.search_issues()` はデフォルトで 50 件のイシュー情報を返します。
これ以上の情報をまとめて取得したいときは、`maxResults` オプションを指定します。

### 多数のイシューをページング処理する

多数のイシューをまとめて処理する場合は、必要に応じて下記のようにページング処理します。
`startAt` 引数の値をずらしながら少しずつデータ取得していきます。

```python
def process_issue(issue: Issue):
    """1 件のイシューを処理する"""
    print(f"{issue.key}:\t{issue.fields.summary}")

def process_all_issues():
    """すべてのイシューを 100 件ずつ処理する"""
    start_at = 0
    MAX_RESULTS = 100

    while True:
        issues = jira.search_issues("project=PROJ", startAt=start_at, maxResults=MAX_RESULTS)
        if len(issues) == 0:
            break
        for issue in issues:
            process_issue(issue)
        start_at += MAX_RESULTS
```


### 様々な条件でイシューを検索する

```python
# 自分にアサインされたイシューを取得する
issues = jira.search_issues("project = PROJ and assignee != currentUser()")

# 自分が報告したイシューを作成日順（新しい順）に 3 件取得する
issues = jira.search_issues("reporter = currentUser() order by created desc", maxResults=3)

# 自分にアサインされたイシューのうち、今週末が締め切りのものを優先度順に 5 件取得する
issues = jira.search_issues("assignee = currentUser() and due < endOfWeek() order by priority desc", maxResults=5)
```

### イシューの詳細情報を取得する

```python
# キー指定でイシュー情報を取得する
issue = jira.issue("MYPROJ-1234")

# 詳細情報を表示する
print("key:", issue.key)
print("project:", issue.fields.project)
print("summary:", issue.fields.summary)
print("status:", issue.fields.status)
print("created:", issue.fields.created)
print("updated:", issue.fields.updated)
print("assignee:", issue.fields.assignee)
print("creator:", issue.fields.creator)
print("reporter:", issue.fields.reporter)
print("priority:", issue.fields.priority)
print("labels:", issue.fields.labels)
print("issuetype:", issue.fields.issuetype)
print("--- description ---")
print(issue.fields.description)
```

取得するフィールドを指定すると、効率的にデータを取得できます。

```python
issue = jira.issue("MYPROJ-1234", fields="summary,status")
```

### イシュー内のすべてのコメントを取得する

```python
# キー指定でイシュー情報を取得する
issue = jira.issue("MYPROJ-1234")

# すべてのコメントを列挙
comments = issue.fields.comment.comments  # あるいは jira.comments(issue) でも OK
for comment in comments:
    print(f"■ {comment.id} / {comment.created} / {comment.author.displayName}")
    print(f"{comment.body}\n")
```

### イシュー内の特定のコメントを ID 指定で取得する

```python
comment = jira.comment("MYPROJ-1234", "6580301")
```

### コメントを削除する

```python
comment.delete()
```

### イシューにコメントを追加する

```python
# キー指定でイシュー情報を取得する
issue = jira.issue("MYPROJ-1234")

# そのイシューにコメントを追加する
comment = jira.add_comment(issue, "Comment text")
```

あるいは、イシューのキーを指定することで `Issue` オブジェクトを作成せずにコメントを追加することができます。

```python
jira.add_comment("MYPROJ-1234", "Comment text")
```

### 新しいイシューを作成する

```python
new_issue = jira.create_issue(
    project="MYPROJ",           # プロジェクトのキー
    summary="Summary",          # 課題の要約（タイトル）
    description="Description",  # 課題の詳細
    issuetype={"name": "Task"}  # 課題のタイプを指定
)

print(f"Issue ({new_issue.key}) created")
```

イシューを作成すると、`MYPROJ-1234` のようなキーが自動的に割り当てられます。

### イシューを削除する

```python
issue = jira.issue("MYPROJ-1234")
issue.delete()

# サブタスクまで削除する場合
# issue.delete(deleteSubtasks=True)
```

### イシューの Assignee を変更する

```python
issue = jira.issue("MYPROJ-1234")
jira.assign_issue(issue, "newassignee")

# Assignee を削除する場合
# jira.assign_issue(issue, None)
```

### イシューの各種情報をまとめて変更する

```python
issue = jira.issue("MYPROJ-1234")
issue.update(summary="new summary", description="A new summary was added")

# 更新時の Notification を抑制したいとき
issue.update(notify=False, summary="new summary")
```

### ユーザー情報を取得する

```python
user = jira.user("USER_ID")
for key, val in user.raw.items():
    print(f"{key}: {val}")
```

### フィールド情報を取得する

```python
fields = jira.fields()
for f in sorted(fields, key=lambda x: x["id"]):
    print(f"{f['id']}: {f['name']}")
```
