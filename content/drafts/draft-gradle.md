---
title: "DRAFT: Gradle"
url: "/p/mwa4wsz"
date: "2021-09-01"
tags: ["Gradle"]
draft: true
---

プロジェクト（サブプロジェクト）の一覧を表示する
----

{{< code >}}
$ gradlew projects
{{< /code >}}


定義されているタスクの一覧を表示する
----

{{< code >}}
$ gradlew tasks
$ gradlew app:tasks  （app プロジェクト内のタスクだけ表示する）
{{< /code >}}


タスクの詳細説明を表示する
----

{{< code >}}
$ gradlew help --task <タスク名>
{{< /code >}}


その他
----

Gradle を使ったプロジェクトには、多くの場合、慣例的タスクとして __`clean`__、__`check`__、__`assemble`__、__`build`__ が含まれています。

