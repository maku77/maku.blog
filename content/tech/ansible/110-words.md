---
title: "Ansible 関連用語"
url: "p/a9twaog/"
date: "2022-07-13"
tags: ["Ansible"]
weight: 110
---

{{% private %}}
- https://docs.ansible.com/ansible/latest/reference_appendices/glossary.html
- https://docs.ansible.com/ansible/latest/reference_appendices/playbooks_keywords.html
{{% /private %}}

モジュール
: Ansible で使う機能の単位。例えば、`ansible.builtin.systemd` など。
プレイブック中の各プレイで指定するタスクリストは、モジュールを指定して定義していきます。

インベントリ
: コントロール対象とするマネージドノードの一覧ファイル。
INI 形式、あるいは YAML 形式で記述します。
シンプルに記述したければ INI 形式、階層構造を明確にしたければ YAML 形式を使います。

ホスト変数
: インベントリで定義したホストに対して設定する変数。
インベントリファイルから分離して、`host_vars` ディレクトリ内の YAML ファイルに記述することもできます。

グループ変数
: インベントリで定義したグループに対して設定する変数。
インベントリファイルから分離して、`group_vars` ディレクトリ内の YAML ファイルに記述することもできます。

インベントリプラグイン (inventory plugins)
: Ansible 本体にインベントリの機能を提供しているプラグイン。
INI ファイル用の `ansible.builtin.ini` や、YAML ファイル用の `ansible.builtin.yaml` などがあります。
他にも動的にインベントリを生成するものもあります。
参考: [Index of all Inventory Plugins — Ansible Documentation](https://docs.ansible.com/ansible/latest/collections/index_inventory.html)

プレイブック (playbook)
: マネージドノードに対する処理内容を YAML 形式で定義したもの。
プレイブックは、複数のプレイ (Play) で構成されており、プレイは複数のタスクで構成されています (Playbook ◇─ Play ◇─ Task)。

プラグイン
: Ansible 本体に対して機能を追加する仕組み。
参考: [Indexes of all modules and plugins — Ansible Documentation](https://docs.ansible.com/ansible/latest/collections/all_plugins.html)

コントロールノード (control node)、コントローラーノード (controller node)
: `ansible` コマンドや `ansible-playbook` コマンドを実行するホスト、つまり Ansible を使って対象のシステムの制御を行うマシンを指します。

管理対象ノード (managed node)、ターゲットホスト (target host)
: Ansible のコントロール対象となるホストのこと。コントロールノード側で Inventory として管理されます。

ansible.cfg
: Ansible 本体に対する設定ファイルで、INI 形式で記述します。
デフォルトで参照するインベントリファイル名や、SSH 接続時のホスト名登録の省略、といった設定を行えます。
参考: [/p/pamv6gq/](ansible.cfg ファイルの検索順序)

コレクション
: Ansible コンテンツの配布形式で、プラグインやモジュール、Playbook などをまとめたもの。
ドット (`.`) で区切られた名前が付けられています（例: `ansible.utils`）。
Ansible 本体 (ansible-core) に内蔵されているコレクションには、`ansible.builtin` という名前が付けられています。
モジュールはさらにドットを付けた名前で表現されます。
例えば、`ansible.builtin` コレクションには、`ansible.builtin.systemd` モジュールが含まれています。

Ansible Galaxy
: コミュニティがコレクションを配布しているところ。
参考: https://galaxy.ansible.com/

FQCN (Fully Qualified Collection Name)
: モジュールやプラグインの名前を、コレクション名を含む完全な名前で表現したもの。
例えば、`ansible.windows` コレクションの `win_command` モジュールの FQCN は `ansible.windows.win_command` になります。

アドホックモード
: Playbook を使わず、コマンドラインから直接モジュールを呼び出す方法。

チェックモード
: このモードで Ansible を実行すると、実際にはホストに変更を加えず、どのような結果になるかを確認できます（一般に dry run と呼ばれるものです）。

ハンドラー
: システムに変更が加えられたときに実行される処理。

Ansible ロール
: 自己完結型の特殊な Playbook で、複雑なオーケストレーションを完了するために必要なタスク、変数、構成テンプレート、およびその他のサポートファイルとともに移動させることができます。
コレクションには複数のロールを含むことができます。

