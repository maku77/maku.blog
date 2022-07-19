---
title: "Linux コマンド: ufw による Ubuntu のファイアウォール設定"
url: "p/drar8o4/"
date: "2022-07-04"
tags: ["Linux", "セキュリティ"]
---

{{% private %}}
- [UFW - Community Help Wiki](https://help.ubuntu.com/community/UFW)
- [Ubuntu wiki - UncomplicatedFirewall](https://wiki.ubuntu.com/UncomplicatedFirewall)
{{% /private %}}

ufw とは
----

__ufw (Uncomplicated Firewall)__ は Debian/Ubuntu 系 Linux でパケットフィルタリングの設定（ファイアウォール設定）を行うためのツールです。
従来、パケットフィルタリングのツールとしては iptables が使われていましたが、iptables は設定が煩雑で扱いづらいという問題を抱えていたため、直感的な設定を行うためのツールとして ufw が開発されました。
ufw は Ubuntu 8.04 LTS 以降で使用することができます。


ufw のサブコマンド
----

### 通常コマンド

| コマンド | 説明 |
| ---- | ---- |
| [`ufw enable`](#ufw-enable) | enables the firewall |
| [`ufw disable`](#ufw-disable) | disables the firewall |
| `ufw default ARG` | set default policy |
| `ufw logging LEVEL` | set logging to LEVEL |
| [`ufw allow ARGS`](#ufw-allow) | add allow rule |
| `ufw deny ARGS` | add deny rule |
| `ufw reject ARGS` | add reject rule |
| [`ufw limit ARGS`](#ufw-limit) | add limit rule |
| [`ufw delete RULE\|NUM`](#ufw-delete) | delete RULE |
| `ufw insert NUM RULE` | insert RULE at NUM |
| `ufw prepend RULE` | prepend RULE |
| `ufw route RULE` | add route RULE |
| `ufw route delete RULE\|NUM` | delete route RULE |
| `ufw route insert NUM RULE` | insert route RULE at NUM |
| `ufw reload` | reload firewall |
| `ufw reset` | reset firewall |
| [`ufw status`](#ufw-status) | show firewall status |
| `ufw status numbered` | show firewall status as numbered list of RULES |
| `ufw status verbose` | show verbose firewall status |
| `ufw show ARG` | show firewall report |
| `ufw version` | display version information |

### アプリケーションプロファイル

| コマンド | 説明 |
| ---- | ---- |
| `ufw app list`           | list application profiles |
| `ufw app info PROFILE`   | show information on PROFILE |
| `ufw app update PROFILE` | update PROFILE |
| `ufw app default ARG`    | set default application policy |


ufw の基本的な使い方
----

### ufw で現在の設定を確認する (ufw status) {#ufw-status}

```console
$ sudo ufw status
Status: active

To                         Action      From
--                         ------      ----
OpenSSH                    ALLOW       Anywhere
OpenSSH (v6)               ALLOW       Anywhere (v6)
```

1 行目の表示で、ufw によるファイアウォールが有効 (`Status: active`) になっているか、無効 (`Status: inactive`) になっているかを確認することができます。

その下には、どのポート（サービス）へのアクセスが許可 (ALLOW) されているかのルール設定が表示されます。
デフォルトでは、ufw はすべての受信パケットを拒否するため、ここに列挙されていないサービスには外部から接続できないことになります。

### ufw を有効にする (ufw enable) {#ufw-enable}

ufw を有効にするには、__`ufw enable`__ コマンドを使用します。
ポート 20 番の SSH 接続を許可 (`ufw allow ssh`) しておかないと、ufw を有効にすることで SSH 接続ができなくなってしまうので、その警告メッセージが表示されますが、問題なければ `y` と入力します。

```console
$ sudo ufw enable
Command may disrupt existing ssh connections. Proceed with operation (y|n)? y
Firewall is active and enabled on system startup

$ sudo ufw status
Status: active
...（省略）...
```

### ufw を無効にする (ufw disable) {#ufw-disable}

ufw を無効にする（パケットフィルタリングを停止する）には、__`ufw disable`__ コマンドを使用します。

```console
$ sudo ufw disable
Firewall stopped and disabled on system startup

$ sudo ufw status
Status: inactive
```

上記のようにファイアウォールを無効化しても、後ほど `ufw enable` で有効化すれば、以前の設定を復旧することができます。
ufw の設定は `/etc/ufw` ディレクトリ以下にルールファイルとして保存されています。

### ufw でルールを追加する (ufw allow) {#ufw-allow}

__`ufw allow`__ コマンドで、特定のポートへのアクセスを許可できます。

{{< code lang="console" title="例: SSH (22/tcp) でのアクセスを許可" >}}
$ sudo ufw allow ssh
$ sudo ufw allow 22/tcp  # これでも同じ意味
{{< /code >}}

{{< code lang="console" title="例: TCP ポート 8000 へのアクセスを許可" >}}
$ sudo ufw allow 8000/tcp
{{< /code >}}

### ufw でルールを削除する (ufw delete) {#ufw-delete}

ufw で設定したルールを削除するには、__`ufw delete`__ で「ルール名」あるいは「ルール番号」を指定します。
ルール名を指定する場合は、例えば `ufw allow 8000/tcp` で追加したルールは次のように削除します。

```console
$ sudo ufw delete allow 8000/tcp
```

番号で削除するルールを指定する場合は、先に __`ufw status numbered`__ コマンドで現在設定されているルールの番号を調べます。

{{< code lang="console" >}}
# まずルールの番号を確認する
$ sudo ufw status numbered
Status: active

     To                         Action      From
     --                         ------      ----
[ 1] 8000/tcp                   ALLOW IN    Anywhere
[ 2] 8000/tcp (v6)              ALLOW IN    Anywhere (v6)

# 2 番目のルールを削除する
$ sudo ufw delete 2
Deleting:
 allow 22/tcp
{{< /code >}}

### ufw で連続アクセスを拒否する (ufw limit) {#ufw-limit}

`ufw allow` の代わりに __`ufw limit`__ を使用すると、特定ポートへの連続アクセスあった場合に一定時間接続を拒否することができます。
次の例では、SSH (22) ポートへの接続が 30 秒間に 6 回以上あった場合に接続を拒否します。

```console
$ sudo ufw limit ssh
```

これにより、ブルートフォースアタックによるログインを防ぐことができます。
`ufw limit` 設定をしたポートに関しては、`ufw allow` 設定を行う必要はありません（連続接続以外のアクセスは許可する、という意味になります）。

