---
title: "Linuxメモ: Linux のグループを管理する (groupadd, groupdel, gpasswd)"
linkTitle: "Linux のグループを管理する (groupadd, groupdel, gpasswd)"
url: "p/uexfvcs/"
date: "2022-07-04"
tags: ["Linux"]
---

グループの作成と削除 (groupadd, groupdel)
----

Linux でグループを作成／削除するには、__`groupadd`__ および __`groupdel`__ コマンドを使用します。
グループ作成時にグループ ID（数値）が自動的に割り当てられますが、`-g` オプションで明示することもできます。

{{< code lang="console" title="グループ admin を作成する" >}}
$ sudo groupadd admin          # グループ ID は自動で割り当てる
$ sudo groupadd -g 1234 admin  # グループ ID を明示する
{{< /code >}}

{{< code lang="console" title="グループ admin を削除する" >}}
$ sudo groupdel admin
{{< /code >}}

{{% note title="プライマリ・グループ" %}}
`useradd` コマンドでユーザーを作成すると、デフォルトでそのユーザーは、ユーザー名と同じ名前のグループに所属することになります。
このグループをプライマリ・グループと呼びます。
例えば、`useradd maku` でユーザー `maku` を作成すると、同時に `maku` というプライマリ・グループが作成されます。
`groupdel` コマンドでプライマリ・グループを削除することはできません。
{{% /note %}}


グループにユーザーを追加する (gpasswd)
----

Linux のグループに既存のユーザーを追加／削除するには、__`gpasswd`__ コマンドを使用します。

{{< code lang="console" title="グループ admin にユーザー maku を追加する" >}}
$ sudo gpasswd -a maku admin
Adding user maku to group admin
{{< /code >}}

{{< code lang="console" title="グループ admin からユーザー maku を削除する" >}}
$ sudo gpasswd -d maku admin
{{< /code >}}

これらの設定を反映させるには、そのユーザーでログインし直す必要があります。


グループの一覧を確認する (/etc/group)
----

__`/etc/group`__ ファイルには、既存グループの一覧情報が保存されています。

{{< code lang="console" title="グループリストを表示する" >}}
$ cat /etc/group
...（省略）...
group1:x:1003:user1,user2,user3
{{< /code >}}

上記の例では、グループ `group1` に、3 つのユーザー (`user1`、`user2`、`user3`) が登録されていることがわかります。
また、グループ ID が `1003` であることも分かります。


ユーザーがどのグループに所属しているか調べる (id)
----

__`id`__ コマンドを使用すると、指定したユーザーがどのグループに所属しているかを確認できます。
ユーザー名を省略すると、カレントユーザーの情報を表示します。

{{< code lang="console" >}}
$ id
uid=1000(maku) gid=1000(maku) groups=1000(maku),1001(group1),1002(group2)
{{< /code >}}

上記の例では、カレントユーザー `maku` が、グループ `maku`、`group1`、`group2` に所属していることが分かります。


グループ名やグループ ID を変更する (groupmod)
----

既存のグループの名前や ID を変更するには、__`groupmod`__ コマンドを使用します。

{{< code lang="console" title="グループ foo のグループ名を bar に変更する" >}}
$ sudo groupmod -n bar foo
{{< /code >}}

{{< code lang="console" title="グループ foo のグループ ID を 1234 に変更する" >}}
$ sudo groupmod -g 1234 foo
{{< /code >}}

