---
title: "VS Code で YAML ファイルの構文をチェックする（YAML 拡張）"
url: "p/c7sp7ut/"
date: "2023-04-19"
tags: ["JSON", "YAML"]
---

YAML 拡張とは？
----

VS Code の [YAML 拡張 (redhat.vscode-yaml)](https://marketplace.visualstudio.com/items?itemName=redhat.vscode-yaml) をインストールすると、スキーマ定義に基づいて、YAML ファイルの記述内容が正しいかをチェックすることができます。
スキーマ定義を [JSON スキーマファイル](https://json-schema.org/) の形で作成し、YAML ファイルの先頭に次のようなコメントを記述することで適用できます。

{{< code lang="yaml" title="YAML ファイルの先頭に追加" >}}
# yaml-language-server: $schema=./schema.json
{{< /code >}}

YAML ファイルの構文チェックなのに JSON スキーマ？と思うかもしれませんが、YAML と JSON のデータ形式には互換性があるので、スキーマは JSON スキーマでよいんですね。


スキーマによる YAML ファイルの構文チェック
----

### 公開スキーマファイルを参照する

スキーマファイルは、インターネット上に公開されているものを URL で指定できるようになっています。
有名どころの設定ファイルのスキーマは、下記のサイトに一覧があるので、いろいろなプロジェクトからそのまま利用できます。

- [JSON Schema Store](https://www.schemastore.org/json/)

Ansible Playbook など、内容が複雑になりがちな YAML ファイルに設定しておくと役に立ちそうです。

{{< code lang="yaml" title="playbook.yml（の先頭に記述する）" >}}
# yaml-language-server: $schema=https://raw.githubusercontent.com/ansible/ansible-lint/main/src/ansiblelint/schemas/ansible.json
{{< /code >}}

GitHub Actions のワークフローファイル (`.github/workflows/*.yml`) などは、そのファイルパスから、どのスキーマを使用すればいいかを推測できるので、上記のようなコメント行すら記述せずに自動的に [GitHub Actions 用のスキーマ](https://json.schemastore.org/github-workflow.json) を適用してくれます。
つまり、VS Code に YAML 拡張をインストールするだけで、GitHub Actions のワークフローファイルの構文チェックが有効になります。
便利！

### 独自定義したスキーマファイルを参照する

アプリケーション独自の設定ファイルやデータファイルを YAML 形式で作成する場合は、そのスキーマも独自であることが多いので、専用の JSON スキーマファイルを作成することになるでしょう。
例えば、次の JSON スキーマファイルでは、ユーザー情報を表現するためのスキーマを定義しています（コメントを記述できるようにするために、拡張子を `.jsonc` としています）。

{{< code lang="json" title="user-schema.jsonc（スキーマ定義）" >}}
{
  "type": "object", // ルート要素はオブジェクト
  "required": ["name", "email"], // 必須のプロパティ
  "additionalProperties": false, // properties 以下に定義されたプロパティのみを許可

  // 各プロパティの定義
  "properties": {
    "name": {
      "type": "string"
    },
    "email": {
      "type": "string",
      "format": "email"
    },
    "age": {
      "type": "integer"
    }
  }
}
{{< /code >}}

このスキーマでは、ルート要素として `name`、`email`、`age` というプロパティを持つオブジェクトを配置することを定義しています。
`name` と `email` プロパティは必須であり、定義されていないプロパティを含むことを禁止しています。
ここでは、JSON スキーマの記述方法は解説しないので、詳しくは [JSON Schema の仕様](https://json-schema.org/) を参照してください。

あとは、YAML ファイルの先頭行で次のように参照設定すれば、VS Code 上で YAML ファイルの記述内容がチェックされるようになります。

{{< code lang="yaml" title="user.yml" >}}
# yaml-language-server: $schema=./user-schema.jsonc
name: Maku       # Good
email: maku@com  # NG（メールアドレスの形式ではない）
age: 100         # Good（このプロパティは省略可能）
foo: 200         # NG（こんなプロパティは定義されていない）
{{< /code >}}


（応用）VS Code の設定ファイルでスキーマを関連付ける
----

前述の例では、参照する JSON スキーマファイルを該当の YAML ファイル内にコメントとして記述しましたが、[VS Code の設定ファイル](/p/tfq2cnw/) で、使用するスキーマファイルを関連付けておくこともできます。
複数の YAML ファイルで、同一のスキーマ定義を参照する場合に便利です。

{{< code lang="json" title="<Project>/.vscode/settings.json" >}}
{
  "yaml.schemas": {
    "data/user-schema.jsonc": ["data/*.yml", "data/*.yaml"]
  }
}
{{< /code >}}

上記のように設定しておくと、スキーマ定義 (`data/user-schema.jsonc`) が、`data` ディレクトリ以下の `*.yml` および `*.yaml` ファイルに適用されます。
ここでは、プロジェクト内に配置した JSON スキーマファイルを参照していますが、次のようにインターネット上に公開されている JSON スキーマファイルを参照することもできます。

{{< code lang="json" title="<Project>/.vscode/settings.json" >}}
{
  "yaml.schemas": {
    "https://example.com/books-schema.json": "data/books.yml",
    "https://example.com/musics-schema.json": "data/musics.yml"
  }
}
{{< /code >}}

