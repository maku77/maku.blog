---
title: "Ansible 関連用語"
url: "p/a9twaog/"
date: "2022-07-13"
tags: ["Ansible"]
draft: true
---

モジュール
: Ansible で使う機能の単位。例えば、`ansible.builtin.systemd` など。

インベントリ
: コントロール対象とするマネージドノードの一覧ファイル。INI 形式や YAML 形式で記述する。

ホスト変数
: インベントリで定義したホストに対して設定する変数。インベントリファイルから分離して、`host_vars` ディレクトリ内の YAML ファイルに記述することもできる。

グループ変数
: インベントリで定義したグループに対して設定する変数。インベントリファイルから分離して、`group_vars` ディレクトリ内の YAML ファイルに記述することもできる。

インベントリプラグイン (inventory plugins)
: Ansible 本体にインベントリの機能を提供しているプラグイン。INI ファイル用の `ansible.builtin.ini` や、YAML ファイル用の `ansible.builtin.yaml` などがある。他にも動的にインベントリを生成するものもある。参考: [Index of all Inventory Plugins — Ansible Documentation](https://docs.ansible.com/ansible/latest/collections/index_inventory.html)

プレイブック (playbook)
: マネージドノードに対する処理内容を YAML 形式で定義したもの。

プラグイン
: Ansible 本体に対して機能を追加する仕組み。参考: [Indexes of all modules and plugins — Ansible Documentation](https://docs.ansible.com/ansible/latest/collections/all_plugins.html)

ansible.cfg
: Ansible 本体に対する設定ファイルで、INI 形式で記述する。デフォルトで参照するインベントリファイル名や、SSH 接続時のホスト名登録の省略、といった設定を行える。参考: [/p/pamv6gq/](ansible.cfg ファイルの検索順序)

コレクション
: Ansible コンテンツの配布形式で、プラグインやモジュール、Playbook などをまとめたもの。ドット (`.`) で区切られた名前が付けられている（例: `ansible.utils`）。Ansible 本体 (ansible-core) に内蔵されているコレクションには、`ansible.builtin` という名前が付けられている。モジュールはさらにドットを付けた名前で表現される。例えば、`ansible.builtin` コレクションには、`ansible.builtin.systemd` モジュールが含まれている。

Ansible Galaxy
: コミュニティがコレクションを配布しているところ。参考: https://galaxy.ansible.com/

FQCN (Fully Qualified Collection Name)
: モジュールやプラグインの名前を、コレクション名を含む完全な名前で表現したもの。例えば、`ansible.windows` コレクションの `win_command` モジュールの FQCN は `ansible.windows.win_command`。

アドホックモード
: Playbook を使わず、コマンドラインから直接モジュールを呼び出す方法。

チェックモード
: このモードで Ansible を実行すると、実際にはホストに変更を加えず、どのような結果になるかを確認できる（一般に dry run と呼ばれるもの）。

ハンドラー
: システムに変更が加えられたときに実行される処理。

Ansible ロール
: 自己完結型の特殊な Playbook で、複雑なオーケストレーションを完了するために必要なタスク、変数、構成テンプレート、およびその他のサポートファイルとともに移動させることができます。コレクションには複数のロールを含むことができる。

