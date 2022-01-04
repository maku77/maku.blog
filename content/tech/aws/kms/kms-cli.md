---
title: "AWS KMS をコマンドライン (CLI) で操作する"
url: "/p/m8jv7hr"
date: "2021-10-14"
tags: ["AWS", "AWS/KMS"]
---

SSM パラメーターストア用の AWS マネージドキーを確認する
----

{{< code >}}
$ aws kms describe-key --key-id alias/aws/ssm
{{< /code >}}

{{% accordion title="実行結果" %}}
{{< code >}}
KeyMetadata:
  AWSAccountId: '123456789012'
  Arn: arn:aws:kms:ap-northeast-1:123456789012:key/d7ce1afa-a7d4-fe43-2da2-4ddd769480d7
  CreationDate: '2021-10-14T03:34:31.467000+09:00'
  CustomerMasterKeySpec: SYMMETRIC_DEFAULT
  Description: Default key that protects my SSM parameters when no other key is defined
  Enabled: true
  EncryptionAlgorithms:
  - SYMMETRIC_DEFAULT
  KeyId: d7ce1afa-a7d4-fe43-2da2-4ddd769480d7
  KeyManager: AWS
  KeyState: Enabled
  KeyUsage: ENCRYPT_DECRYPT
  Origin: AWS_KMS
{{< /code >}}
{{% /accordion %}}


KMS キーを作成する (kms create-key)
----

{{< code >}}
aws kms create-key
{{< /code >}}

KMS キー (CMK: Customer Master Key) は作成は一瞬でできますが、削除は最低 7 日かかるので注意してください。


KMS キーを削除する (kms schedule-key-deletion / cancel-key-deletion)
----

KMS キーの削除にはリスクを伴うので、削除するまでの猶予期間として 7～30 日を指定する必要があります（`kms delete-key` のような即削除のコマンドは用意されていません）。

{{< code title="7 日後に KMS キーを削除する" >}}
$ aws kms schedule-key-deletion \
    --key-id 9a6dff25-5ba8-364e-5e92-4123d37d43a1 \
    --pending-window-in-days 7
{{< /code >}}

削除スケジュールをキャンセルするには次のように実行します。

{{< code >}}
$ aws kms cancel-key-deletion \
    --key-id 9a6dff25-5ba8-364e-5e92-4123d37d43a1 \
{{< /code >}}

上記の操作を行うには、それぞれ、`kms:ScheduleKeyDeletion` 権限と `kms:CancelKeyDeletion` 権限が必要です。

