---
title: "SSH キーの管理: Windows で SSH の秘密鍵のパーミッションエラーが出る場合"
url: "p/ret8kky/"
date: "2024-01-31"
tags: ["ssh"]
---

Windows から SSH 接続するときに、次のような秘密鍵（プライベートキー）のパーミッションエラーが出ることがあります。

{{< code title="Windows 上での SSH 接続時の秘密鍵エラー" >}}
D:\> ssh -i D:\temp\private.key maku@example.com
Permissions for 'D:\\temp\\private.key' are too open.
It is required that your private key files are NOT accessible by others.
This private key will be ignored.
Load key "D:\\temp\\private.key": bad permissions
maku@example.com's password:
（↑ SSH キーではなくユーザーパスワードでの接続にフォールバックされてしまう）
{{< /code >}}

このエラーは、秘密鍵のパーミッションがカレントユーザーのみのアクセスに絞られていないときに発生します。
下記のいずれかの方法で解決できます。

- （方法1）秘密鍵を __%USERPFOFILE%__ ディレクトリ以下に格納する
  - SSH 秘密鍵を `C:\Users\maku\.ssh\private.key` のようなパスに配置するだけで OK です。
  - ユーザーディレクトリに配置したファイルはカレントユーザーしか参照できないので、パーミッションのエラーが出なくなります。
- （方法2）秘密鍵のパーミッション情報を変更する
  - コマンドプロンプトから __`icacls`__ コマンドを使って SSH 秘密鍵のアクセス権を変更します。

    ```
    icacls .\private.key /inheritance:r
    icacls .\private.key /grant:r "%USERNAME%:R"
    ```
  - `icacls` コマンド (Integrity Control Access Control List) は、Windows コマンドプロンプトおよび PowerShell で使用できるコマンドラインツールで、ファイルやディレクトリのアクセス制御リスト (ACL) を変更するために使用されます（ファイル自体の内容が変更されるわけではありません）。
    上記のように実行することで、秘密鍵ファイルの読み込み権限をカレントユーザーのみに制限できます。

