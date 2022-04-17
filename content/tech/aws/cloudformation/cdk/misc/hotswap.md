---
title: "AWS CDK メモ: Lambda 関数コードだけ高速デプロイする (cdk deploy --hotswap)"
url: "/p/ap8p7n4"
date: "2021-10-06"
tags: ["AWS", "AWS/CDK"]
---

AWS CDK によるデプロイ (`cdk deploy`) の実行には結構時間がかかりますが、Lambda 関数のコードだけ更新したいときは、__`hotswap`__ オプションを付けて実行することで高速にデプロイできます。

{{< code title="Lambda 関数だけ高速更新" >}}
$ cdk deploy --hotswap
{{< /code >}}

ただし、これは開発時のみ使うべき機能として提供されており、Production 環境においては、通常通り CDK アプリ全体のデプロイを行うことが推奨されています。

