---
title: "Ansible タスク例: 任意のコマンドを実行する (ansible.builtin.command/shell)"
linkTitle: "任意のコマンドを実行する (ansible.builtin.command/shell)"
url: "p/ihqz7em/"
date: "2022-08-28"
tags: ["Ansible"]
---

Ansible のターゲットノード上で任意のコマンドを実行するには、次のような組み込みモジュールを使用します。

[ansible.builtin.command モジュール](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/command_module.html)
: 任意のコマンドを実行します。
シェルを経由せず、指定したコマンドを直接実行するので、シェル特有の機能は使えません。
Windows の場合は代わりに [ansible.builtin.win_command](https://docs.ansible.com/ansible/latest/collections/ansible/windows/win_command_module.html) を使用します。

[ansible.builtin.shell モジュール](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/shell_module.html)
: シェル (`/bin/sh`) で任意のコマンドを実行します。
シェルの機能（`$HOSTNAME` のような変数展開や `*` によるファイルグロブ、`>` によるリダイレクトなど）を使用することができます。
Windows の場合は代わりに [ansible.builtin.win_shell](https://docs.ansible.com/ansible/latest/collections/ansible/windows/win_shell_module.html) を使用します。


command モジュールの基本
----

指定したコマンドを実行したいときは、__`command`__ モジュールの __`cmd`__ パラメーターでそのコマンド（と引数）を指定します。

```yaml
- name: Run command
  ansible.builtin.command:
    cmd: cat /etc/hostname
```

上記のような簡単なコマンドであれば、次のように省略して記述できます。

```yaml
- name: Run command
  ansible.builtin.command: cat /etc/hostname
```

実行するコマンドの引数として `"user name"` のようなスペースを含む値を渡したいときは、次のように __`argv`__ を使ってコマンドと各引数を分割して指定します（この場合、`cmd` パラメーターは使用しません）。

```yaml
- name: Run command
  ansible.builtin.command:
    argv:
      - /usr/bin/add_user_to_db.sh
      - user name
      - database name
```


ファイルの有無で実行するかどうか制御する (creates/removes)
----

`command` モジュールで指定したコマンドは、デフォルトで毎回実行されますが、__`creates`__ および __`removes`__ パラメーターを使用すると、特定のファイルの有無によってコマンドを実行するかしないかを制御することができます。

### creates パラメーター

__`creates`__ パラメーターでファイル名を指定しておくと、そのファイルが存在しない場合だけタスクを実行します。
つまり、そのファイルを生成するようなコマンドを実行することを示唆します。

```yaml
- name: Create an empty database
  ansible.builtin.command:
    cmd: /usr/bin/make_database.sh
    creates: /path/to/database
```

### removes パラメーター

逆に、特定のファイルが存在する場合だけコマンドを実行したいときは、__`removes`__ パラメーターでそのファイル名を指定します。


リダイレクトやパイプなどのシェル機能を利用する
----

リダイレクト (`>`) やパイプ (`|`) はシェル特有の機能なので、これらの機能を使う場合は、`command` モジュールの代わりに __`shell`__ モジュールを使用する必要があります。

```yaml
- name: List all entries in etc
  ansible.builtin.shell:
    cmd: ls /etc > files.txt
    creates: files.txt
```

`shell` モジュールで指定可能なパラメーターは、`command` モジュールとほぼ同様です。


コマンドを複数行に分けて記述する
----

長いコマンドは複数行に分けて記述した方が見やすくなります。
これは Ansible というより YAML の構文ですが、次のようにして `cmd` パラメーターの値を複数行に分けて記述できます。

```yaml
- name: Multi-line command
  ansible.builtin.shell:
    cmd: >
      echo AAA > aaa.txt &&
      echo BBB > bbb.txt &&
      echo CCC > ccc.txt
```

末尾の `>` は、複数行にわたるテキストをスペースで結合するという意味を持っています。

