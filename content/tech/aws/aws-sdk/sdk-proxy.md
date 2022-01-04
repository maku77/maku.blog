---
title: "AWS SDK for Node.js でプロキシ環境変数 (https_proxy) を反映させる"
url: "/p/6vs27aa"
date: "2021-03-29"
tags: ["AWS", "AWS/SDK"]
---

{{% private %}}
- [Node.js 用のプロキシの設定](https://docs.aws.amazon.com/ja_jp/sdk-for-javascript/v2/developer-guide/node-configuring-proxies.html)
{{% /private %}}


AWS SDK for Node.js のプロキシ設定
----

社内のプロキシ環境下などから AWS SDK (for Node.js) を使って API 呼び出しを行うには、次のようにプロキシエージェント設定を行います。
ここでは、AWS SDK version 2 の設定例を示しています。

{{< code lang="ts" title="プロキシ設定の例 (AWS SDK v2)" >}}
import * as AWS from 'aws-sdk';
import { HttpsProxyAgent } from 'https-proxy-agent';

AWS.config.update({
  httpOptions: {
    agent: new HttpsProxyAgent('http://proxy.example.com:8080')
  }
});
{{< /code >}}

これで、それ以降の AWS サービス (`AWS.S3` など）の API 呼び出しがプロキシ経由で行われるようになります。


環境変数 https_proxy の設定を使用する
----

次のようにすれば、環境変数 __`https_proxy`__ に設定されたプロキシアドレスを、AWS SDK にも反映させることができます（といっても、`process.env.https_proxy` を参照しているだけです）。

{{< code lang="ts" >}}
import * as AWS from 'aws-sdk';
import { HttpsProxyAgent } from 'https-proxy-agent';

// プロキシ設定を反映
function setupAwsProxy() {
  const proxy = process.env.https_proxy;
  if (proxy) {
    AWS.config.update({
      httpOptions: { agent: new HttpsProxyAgent(proxy) },
    });
  }
}
{{< /code >}}

あとは、AWS の各種サービス用の API を使用するだけです。

{{< code lang="ts" >}}
async function putObjectToBucket() {
  setupAwsProxy();  // 環境変数のプロキシ設定を反映

  const s3 = new AWS.S3();
  try {
    const output = await s3.putObject({
      Bucket: 'my-s3-bucket-name',
      Key: 'key-1',
      Body: 'body-1'
    }).promise();
    console.log('SUCCESS - Object added:', output);
  } catch (err) {
    console.error('ERROR:', err);
  }
}

putObjectToBucket();
{{< /code >}}

