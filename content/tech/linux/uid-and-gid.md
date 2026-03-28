---
title: "Linuxメモ: セキュリティ/ユーザIDとグループIDを理解する"
url: "p/kntdnhi/"
date: "2012-06-18"
tags: ["linux", "security"]
---

3 つの ID
----

Linux プロセスには、ユーザ ID（UID）とグループ ID（GID）がそれぞれ 3 種類ずつ割り当てられています。
これらの ID は、プロセスがどのユーザやグループの権限で動作しているかを示すもので、セキュリティやアクセス制御の観点から重要な役割を果たします。

| 種類 | 英語名 | 説明 |
| ---- | ---- | ---- |
| **実ユーザ ID（実グループ ID）** | Real UID (GID) | プロセスを起動したユーザ（グループ）を示す |
| **実効ユーザ ID（実効グループ ID）** | Effective UID (GID) | 実際のアクセス権限の判定に使われる |
| **保存ユーザ ID（保存グループ ID）** | Saved set-user-ID (set-group-ID) | 権限の一時的な切り替えを可能にするために保存された ID |

### 実効ユーザ ID がアクセス権限を決める

プロセスがあるファイルにアクセスできるかどうかは、そのプロセスの **実効ユーザ ID（Effective UID）** によって決まります。
つまり、実効ユーザ ID が、プロセスの実際の権限を表しています。

### 実ユーザ ID と実効ユーザ ID の関係

**実ユーザ ID（Real UID）** は、そのプロセスを起動したユーザの UID です。
通常のプログラムでは、実効ユーザ ID も実ユーザ ID と同じ値に設定されます。

### setuid ビットによる権限昇格

setuid ビットがセットされたプログラムを実行すると、そのプログラムファイルの所有ユーザの UID が「実効ユーザ ID」として設定されます。
例えば、所有ユーザが root のプログラムに setuid ビットが設定されている場合、一般ユーザが起動しても実効ユーザ ID は root (=0) になります。
ただし、実ユーザ ID は変わらず、起動したユーザの UID がそのまま引き継がれます。

### 保存ユーザ ID の役割

**保存ユーザ ID（Saved set-user-ID）** は、プログラム起動時の実効ユーザ ID を保存しておくためのものです。
これにより、プログラムは一時的に権限を下げた後、必要に応じて元の権限に復帰できます。

例えば、setuid ビットが設定された root 所有のプログラムの場合、起動時の実効ユーザ ID (=0) が保存ユーザ ID に記録されます。
プログラムが `seteuid(getuid())` で一般ユーザの権限に切り替えた後でも、`seteuid(0)` を呼べば root 権限に戻れるのは、保存ユーザ ID に元の値が残っているからです。

### setuid と seteuid の違い

実効ユーザ ID は、`setuid` あるいは `seteuid` システムコールによってプログラム内から変更できます。

- **`setuid(uid)`**: 現在の実効ユーザ ID が root (=0) の場合、実ユーザ ID、実効ユーザ ID、保存ユーザ ID の **3 つすべて** を変更します。
  root 以外の場合は、実効ユーザ ID のみを変更します。
- **`seteuid(euid)`**: 常に実効ユーザ ID **のみ** を変更します。

つまり、root 権限のプロセスが `setuid` で一般ユーザに切り替えると、保存ユーザ ID も書き換わるため、**root 権限に戻ることができなくなります**。
一方、`seteuid` で切り替えた場合は保存ユーザ ID が維持されるため、後から root 権限に復帰できます。

{{% note %}}
**`man credentials`** を実行すると、Real user ID、Effective user ID、Saved set-user-ID などの詳しい説明を確認できます。
{{% /note %}}


プログラム内での ID の取得／設定
----

### ID の取得

各 ID は以下のシステムコールで取得できます。

```cpp
uid_t getuid(void);   // 実ユーザ ID を取得
gid_t getgid(void);   // 実グループ ID を取得
uid_t geteuid(void);  // 実効ユーザ ID を取得
gid_t getegid(void);  // 実効グループ ID を取得
int getresuid(uid_t *ruid, uid_t *euid, uid_t *suid);  // 3 つの UID をまとめて取得
int getresgid(gid_t *rgid, gid_t *egid, gid_t *sgid);  // 3 つの GID をまとめて取得
```

### ID の設定

設定には、以下のシステムコールを使用します。

```cpp
int setuid(uid_t uid);    // UID のセット（root なら 3 つすべて、それ以外は実効のみ）
int setgid(gid_t gid);    // GID のセット（root なら 3 つすべて、それ以外は実効のみ）
int seteuid(uid_t euid);  // 実効ユーザ ID のセット
int setegid(gid_t egid);  // 実効グループ ID のセット
int setreuid(uid_t ruid, uid_t euid);   // 実ユーザ ID と実効ユーザ ID のセット
int setregid(gid_t rgid, gid_t egid);   // 実グループ ID と実効グループ ID のセット
int setresuid(uid_t ruid, uid_t euid, uid_t suid);  // 3 つの UID をまとめてセット
int setresgid(gid_t rgid, gid_t egid, gid_t sgid);  // 3 つの GID をまとめてセット
```

### 使用例

{{< code lang="cpp" title="例: 実効ユーザ ID を root (=0) に設定" >}}
if (seteuid(0) != 0)
    abort();
{{< /code >}}

{{< code lang="cpp" title="例: 実効ユーザ ID を実ユーザ ID に戻す（権限を一時的に下げる）" >}}
if (seteuid(getuid()) != 0)
    abort();
{{< /code >}}


専用グループでファイルへのアクセスを制限する方法
----

特定のファイルへのアクセスを、指定したプログラムだけに制限したい場合、専用のグループを作成する方法が有効です。

### 設定手順

1. 専用のグループを作成する（例: `myappgroup`）。
2. アクセスを制限したいファイルの所有者を `root:myappgroup` に設定し、パーミッションを `0060`（グループにのみ読み書き権限）に設定する。
3. アクセスを許可したいプログラムファイルの所有グループを `myappgroup` に設定し、setgid ビットをセットする。

### 具体例

```bash
# 1. 専用グループの作成
sudo groupadd myappgroup

# 2. ファイルの所有グループとパーミッションの設定
sudo chown root:myappgroup /path/to/secret-data
sudo chmod 0060 /path/to/secret-data

# 3. プログラムに setgid ビットを設定
sudo chown root:myappgroup /path/to/myapp
sudo chmod 2755 /path/to/myapp
```

これにより、`myapp` を実行すると実効グループ ID が `myappgroup` に設定されるため、`secret-data` ファイルにアクセスできるようになります。
一方、`myapp` 以外のプログラムからは、そのファイルにアクセスできません。
