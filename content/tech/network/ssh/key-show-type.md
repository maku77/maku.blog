---
title: "SSH キーの管理: SSH キーの種類やフィンガープリントを確認する (ssh-keygen -l)"
url: "p/m2k4j2h/"
date: "2022-11-24"
tags: ["SSH"]
---

作成済みの SSH 鍵ファイルの種類（暗号アルゴリズム）やフィンガープリントを確認したいときは、__`ssh-keygen -l`__ コマンドを使用します。
`-f` オプションで指定する鍵ファイルのパスは、秘密鍵でも公開鍵でも構いません。
次の例では、OpenSSH の `ssh-keygen` コマンドで作成したさまざまな種類の SSH キーの内容を表示しています。

```console
$ ssh-keygen -l -f ~/.ssh/id_ed25519.pub
256 SHA256:xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx yourname@example.com (ED25519)

$ ssh-keygen -l -f ~/.ssh/id_ecdsa.pub
256 SHA256:xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx yourname@example.com (ECDSA)

$ ssh-keygen -l -f ~/.ssh/id_rsa.pub
2048 SHA256:xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx yourname@example.com (RSA)

$ ssh-keygen -l -f ~/.ssh/id_dsa.pub
1024 SHA256:xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx yourname@example.com (DSA)
```

各フィールドは、「鍵のビット長」「フィンガープリント」「コメント」「暗号アルゴリズム」を示しています。

Windows PC の場合は、`~/.ssh` の部分は `%USERPROFILE%\.ssh` と読みかえてください。

