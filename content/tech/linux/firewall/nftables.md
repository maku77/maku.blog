---
title: "Linuxメモ: nftables による Linux のファイアウォール設定（ドラフト）"
url: "p/fcv995k/"
date: "2022-07-13"
tags: ["Linux", "セキュリティ"]
draft: true
---

nftables 雑多メモ
----

- CentOS でも `iptables` コマンドを使うことはできるけど、実体は `nftables` になっている
  ```console
  $ iptables --version
  iptables v1.8.2 (nf_tables)
  ```
- `nftables` は `nft` コマンドで制御する
- 設定は `table ◇── chain` という構成
- `nft list tables` ... テーブルの一覧を表示する
- `nft list table ip nat` ... 指定したテーブル（この場合は `ip nat`）の chain を表示する


（旧）iptables コマンドメモ
----

```console
$ iptables --table filter --flush   # すべてのフィルターを削除
$ iptables --policy INPUT DROP      # 入ってくるパケットを拒否
$ iptables --policy OUTPUT ACCEPT   # 出ていくパケットを許可
$ iptables --policy FORWARD ACCEPT  # パケットの転送を許可
```

{{< code lang="console" title="TCP で 22 番ポートに新しく接続してきたパケットを許可" >}}
$ iptables --append INPUT --protocol tcp --match state --state NEW --dport 22 --jump ACCEPT
{{< /code >}}

{{< private >}}
シス女 p.159 参照
{{< /private >}}

