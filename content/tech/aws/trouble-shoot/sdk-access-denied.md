---
title: "AWS CLI でアクセスできるのに AWS SDK で Access Denied (403) になるとき"
url: "/p/7tds7bw"
date: "2021-04-26"
tags: ["AWS"]
---

認証情報の食い違いを調べる
----

例えば、ローカルで AWS CLI を使って S3 の情報にアクセスできているのに、AWS SDK 使った Node.js プログラムで S3 にアクセスしたときに Access Denied (403) になるときは、__異なる認証情報 (credentials) を使ってアクセスしている可能性__ があります。

AWS CLI が、どのようなユーザーでアクセスしているかは、下記のようにして確認できます。

{{< code >}}
$ aws sts get-caller-identity
Account: '123456789012'
Arn: arn:aws:sts::123456789012:assumed-role/MyDeveloperRole/botocore-session-9876543210
UserId: A6B3EVWX58AR9AVTXAP5T:botocore-session-9876543210
{{< /code >}}

次に、Node.js のプログラムなどで、AWS SDK を使って上記と同様の情報を取得します。
Node.js 用の SDK ver.2 では、`AWS.STS.getCallerIdentity()`、SDK ver.3 では `STSClient.send()` を使います。

- 参考: [AWS.STS.getCallerIdentity() - AWS SDK v2](https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/STS.html#getCallerIdentity-property)
- 参考: [GetCallerIdentityCommand() - AWS SDK v3](https://docs.aws.amazon.com/AWSJavaScriptSDK/v3/latest/clients/client-sts/classes/getcalleridentitycommand.html)

{{< code lang="ts" title="printCallerIdentity.ts（SDK ver.2 の場合）" >}}
import { STS } from 'aws-sdk';

async function printCallerIdentity() {
    try {
        const sts = new STS();
        const result = await sts.getCallerIdentity().promise()
        console.log(result)
    } catch (e) {
        console.error(e);
    }
}

printCallerIdentity();
{{< /code >}}

{{< accordion title="SDK ver.3 の場合" >}}
{{< code lang="ts" >}}
import {
  STSClient,
  GetCallerIdentityCommand,
  GetCallerIdentityCommandInput,
  GetCallerIdentityCommandOutput,
} from "@aws-sdk/client-sts";

async function printCallerIdentity() {
  const client = new STSClient({ region: "ap-northeast-1" });
  const input: GetCallerIdentityCommandInput = {};
  const command = new GetCallerIdentityCommand(input);
  try {
    const output: GetCallerIdentityCommandOutput = await client.send(command);
    console.log(output);
  } catch (e) {
    console.error(e);
  }
}

printCallerIdentity();
{{< /code >}}
{{< /accordion >}}

{{< code title="実行結果" >}}
$ ts-node printCallerIdentity.ts
{
  ResponseMetadata: { RequestId: '2ce10620-9158-4e5e-8bca-8258bfd02927' },
  UserId: 'MWUCVBID75KAC5PB68LSP',
  Account: '777888999000',
  Arn: 'arn:aws:iam::777888999000:user/your-user-name'
}
{{< /code >}}

上記の結果と `aws sts get-caller-identity` の結果を比べると、別のユーザー (credential) でアクセスしていることが分かります。
つまり、AWS SDK 対して、正しく認証情報 (credentails) を設定してやれば問題は解決するはずです。

AWS CLI に設定されている認証情報は、`aws configure list` コマンドなどで確認できます。

{{% reference %}}
- [AWS SDK の使用中に発生する S3 アクセス拒否エラーの解決](https://aws.amazon.com/jp/premiumsupport/knowledge-center/s3-access-denied-aws-sdk/)
{{% /reference %}}


（おまけ）今回の原因
----

ちなみに、今回の Access Denied (403) の原因は、AWS CLI では Assume Role したユーザーでアクセスできていたけれど、AWS SDK の方は直接アクセスしていたので弾かれていたというオチでした。
AWS SDK で Assume Role したいときは、`AWS.SMS` オブジェクトの __`assumeRole`__ メソッドを使います。

例えば、下記のような感じで Assume Role して取得した認証情報をセットしてから、S3 などの API を呼び出せば、うまくアクセスできるようになります。

{{< code lang="ts" >}}
import * as AWS from 'aws-sdk';

async function assumeRole() {
    const roleToAssume = {
        RoleArn: 'arn:aws:iam::123456789012:role/MyDeveloperRole',
        RoleSessionName: 'session1',
        DurationSeconds: 900,
    };

    try {
        const sts = new AWS.STS();
        const data = await sts.assumeRole(roleToAssume).promise();
        AWS.config.update({
            credentials: {
                accessKeyId: data.Credentials?.AccessKeyId!,
                secretAccessKey: data.Credentials?.SecretAccessKey!,
                sessionToken: data.Credentials?.SessionToken!
            }
        });
    } catch (err) {
        console.error(err);
    }
}
{{< /code >}}

複雑すぎでしょ・・・（´へ｀;

{{% reference %}}
- [sts_assumerole.js - AWS Code Sample](https://docs.aws.amazon.com/ja_jp/code-samples/latest/catalog/javascript-iam-sts_assumerole.js.html)
{{% /reference %}}


（おまけ）Access Key Id の情報を調べる
----

次のようにすれば、AWS SDK が使用しようとしているアクセスキーの ID を調べることができます。
この情報もトラブル発生時のヒントになるかもしれません。

{{< code lang="ts" >}}
import * as AWS from 'aws-sdk';

function printAccessKey() {
    AWS.config.getCredentials(err => {
        if (err) console.error(err.message);
        else console.log('Access Key: ' + AWS.config.credentials?.accessKeyId);
    });
}

printAccessKey();
{{< /code >}}

{{% reference %}}
- [SDK v2 開発者ガイド ─ グローバル設定オブジェクトの使用](https://docs.aws.amazon.com/ja_jp/sdk-for-javascript/v2/developer-guide/global-config-object.html)
{{% /reference %}}

