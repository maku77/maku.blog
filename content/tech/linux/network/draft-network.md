---
title: "Linux: ネットワーク関連メモ"
url: "p/6asz2eu/"
date: "2024-01-31"
tags: ["linux"]
draft: true
---

ネットワークインタフェース名の構成
----

- `eno0`
  - `en` の部分は __有線 LAN (ethernet)__ という意味
  - `o0` の部分は __オンボードの 0 番目__ という意味
- `enp0s3`
  - `en` の部分は __有線 LAN (ethernet)__ という意味
  - `p0` の部分は __PCI バスの 0 番目__ という意味
  - `s3` の部分は __スロットの 3 番目__ という意味
- `wlp0s1`
  - `wl` の部分は __無線 LAN (wireless LAN)__ という意味
  - `p0` の部分は __PCI バスの 0 番目__ という意味
  - `s1` の部分は __スロットの 1 番目__ という意味


その他
----

- CentOS や Fedora などの RedHat 系 Linux では、ネットワークの管理に NetworkManager サービスを使用します。
  コマンドラインから操作する場合は、__`nmcli`__ コマンドを使用します。

  ```console
  $ nmcli device show  # 現在のネットワーク設定を確認
  $ nmcli connection modify  # ネットワーク設定を変更
  $ nmcli connection reload  # ネットワーク設定を反映
  ```

- Ubuntu では標準で __`ufw`__ コマンドが使えます。
  他のディストリビューションでも、次のようなコマンドでインストールできます。

  ```console
  $ apt install ufw
  $ dnf install ufw
  ```

