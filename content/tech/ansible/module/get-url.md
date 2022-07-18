---
title: "Ansible タスク例: Web からファイルをダウンロードする (ansible.builtin.get_url)"
linkTitle: "Web からファイルをダウンロードする (ansible.builtin.get_url)"
url: "p/s9tctaq/"
date: "2022-07-19"
tags: ["Ansible"]
---

- 参考: [ansible.builtin.get_url module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/get_url_module.html)


HTTPS でファイルをダウンロードする
----

```yaml
- name: Download foo.conf
  ansible.builtin.get_url:
    url: https://example.com/path/file.conf
    dest: /etc/foo.conf
    mode: '0440'
```

- パラメーターの説明:
  - [__`url`__](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/get_url_module.html#parameter-url) ... （必須）ダウンロードするファイルの URL
  - [__`dest`__](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/get_url_module.html#parameter-dest) ... （必須）保存先の絶対パス。ディレクトリ名かファイル名。ディレクトリ名の場合は、ダウンロード元のファイル名が使われます。
  - [__`mode`__](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/get_url_module.html#parameter-mode) ... （オプション）作成するファイルのモード (permission)

`dest` で指定したファイルが既に存在する場合は、ダウンロードはスキップされます。
ただし、`dest` で指定したパスがディレクトリの場合は、ファイルは毎回ダウンロードされてしまうので、`dest` ではファイルのパスを指定すべきです。
`force: yes` オプションを指定すると、ファイルは必ずダウンロードされます。


SSL エラーが発生する場合
----

Ubuntu の Docker イメージには CA 証明書がインストールされておらず、`get_url` モジュールで HTTPS アクセスしたときに次のように SSL 関連のエラーが出ることがあります。

```
Request failed: <urlopen error [SSL: CERTIFICATE_VERIFY_FAILED]
certificate verify failed: unable to get local issuer certificate (_ssl.c:997)>
```

このような場合は、`get_url` の __`validate_certs: no`__ オプションを指定して SSL 検証をスキップしてしまうこともできますが、次のように CA 証明書をインストールするのが正しい対処方法です。

{{< code lang="yaml" title="CA 証明書をインストールする" >}}
- name: Install CA certificates
  ansible.builtin.apt:
    name: ca-certificates
{{< /code >}}

ちなみに、`ca-certificates` パッケージは、`apt install` するときに推奨パッケージとして自動的にインストールされるものですが、`--no-install-recommends` オプションを指定するとインストールされません。

