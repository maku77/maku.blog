---
title: "Ansible タスク例: Docker と Docker Compose をインストールする"
linkTitle: "Docker と Docker Compose をインストールする"
url: "p/8k4j2gw/"
date: "2022-08-28"
tags: ["Ansible", "Docker"]
---

何をするか？
----

下記の Docker のインストールマニュアルに従い、Ansible で Ubuntu 22.04 に Docker の実行環境をセットアップします。

- [Install Docker Engine on Ubuntu | Docker Documentation](https://docs.docker.com/engine/install/ubuntu/)


Playbook の例
----

次の YAML ファイルは、Docker の公式サイトのインストール手順をほぼそのまま Playbook 化したものです。
この Playbook を `ansible-playbook` で実行することで、Docker (+ Docker Compose) のインストール、および Docker デーモンの起動まで完了します。

{{< code lang="yaml" title="install-docker.yml" >}}
- hosts: all
  gather_facts: false
  become: true

  tasks:
    - name: Install APT packages to use a repository over HTTPS
      ansible.builtin.apt:
        pkg:
          - ca-certificates
          - curl
          - gnupg
          - lsb-release
        update_cache: yes
        cache_valid_time: 86400 # cache for 1 day

    - name: Create APT's keyrings directory
      ansible.builtin.file:
        path: /etc/apt/keyrings
        state: directory

    - name: Add Docker's official GPG key
      ansible.builtin.shell:
        cmd: curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
        creates: /etc/apt/keyrings/docker.gpg

    - name: Setup the repository
      ansible.builtin.shell:
        cmd: >
          echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"
          | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
        creates: /etc/apt/sources.list.d/docker.list

    - name: Install Docker
      ansible.builtin.apt:
        pkg:
          - docker-ce
          - docker-ce-cli
          - containerd.io
          - docker-compose-plugin
        update_cache: yes

    - name: Start Docker
      ansible.builtin.service:
        name: docker
        state: started
{{< /code >}}

本質的にやりたいことは、最後の方のステップの `Install Docker` と `Start Docker` だけなのですが、その前に APT 用の GPG キーの取得などを行うので若干長くなっています。

上記の処理に加えて、`docker` コマンドを実行するユーザーを `docker` グループに参加させておくと便利かもしれません（コマンド実行時に `sudo` する必要がなくなります）。
Ansible でユーザーをグループに追加する方法は下記を参考にしてください。

- 参考: [ユーザーを特定のグループに参加させる (ansible.builtin.user)](/p/s2fs5gs/)

