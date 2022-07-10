---
title: "各種 Linux のパッケージマネージャーまとめ"
url: "p/64fpz9h/"
date: "2022-07-09"
tags: ["Linux"]
draft: true
---

| 系列 | ディストリビューション | パッケージマネージャー | パッケージ形式 |
| ---- | ---- | ---- | ---- |
| Debian | Debian 11 | `apt` | `.deb` |
| Debian | Ubuntu 22.04 | `apt` | `.deb` |
| Red Hat | Red Hat Enterprize Linux 8 | `dnf` | `.rpm` |
| Red Hat | Rocky Linux 8 | `dnf` | `.rpm` |
| Red Hat | Alma Linux 8 | `dnf` | `.rpm` |

- パッケージマネージャー
  - __`apt`__ ... 「Advanced Packaging Tools」の略。Debian 系のシステムで使われているパッケージマネージャー。
  - `dpkg` ... 「Debian Package Manager」の略。`apt` の下回りとして使われている。パッケージの依存関係を解決する機能がないので、通常は `apt` を使う。ローカルの `.deb` パッケージをインストールするのに使用できる。
  - __`dnf`__ ... 「Dandified yum」の略。Red Hat 系のシステムで使われていた `yum` コマンドの後継。
  - `yum` ... 「Yellowdog Updater Modified」の略。過去に Red Hat 系のシステムで使われていたパッケージマネージャー。現在は後継の `dnf` が使われている。

- パッケージ形式
  - __`.deb`__ ... 「Debian Software Package ファイル」 Debian 系のシステムで使われているパッケージ形式。
  - __`.rpm`__ ... 「Red Hat Package Manager ファイル」 Red Hat 系のシステムで使われているパッケージ形式。

