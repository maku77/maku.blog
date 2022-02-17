---
title: "Ansible でターゲットホストをグループ化して一度に制御する"
linkTitle: "ターゲットホストをグループ化して一度に制御する"
url: "/p/h4fq2bm"
date: "2016-10-20"
tags: ["Ansible"]
weight: 201
draft: true
---

{{< code lang="ini" title="/etc/ansible/hosts（記述例）" >}}
localhost
192.168.100.1
red.example.com
blue.example.com

[webservers]
192.168.100.2
red.example.com
green.example.com

[dbservers]
db1.example.com
db2.example.com
{{< /code >}}

`[webservers]` や `[dbservers]` という部分では __グループ名__ を定義しており、複数のホストに対して同時に命令する場合に、ホスト名の代わりにこのグループ名を使用することができるようになります。

