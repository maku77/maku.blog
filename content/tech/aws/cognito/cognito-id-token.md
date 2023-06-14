---
title: "Amazon Cognito の ID トークンを使った認証付き API の仕組み"
url: "p/p4462y2/"
date: "2023-06-14"
tags: ["AWS", "AWS/Cognito"]
---

Amazon Cognito サービスを使うと、アプリケーションにユーザー認証（サインイン）の仕組みを提供し、認証されたユーザーだけが呼び出し可能な Web API などを実現できます。
その際に内部的に使用される ID トークンやアクセストークンなどの関係が若干複雑なのでメモしておきます。


Cognito で認証が必要な API を実現する仕組み
----

{{< mermaid >}}
graph LR;
  browser[Web ブラウザ] --サインイン--> cognito[Cognito<br>ユーザープール];
  cognito --ID トークン--> browser
  browser --"認証が必要な API の呼び出し<br>（with ID トークン）"--> gateway["API Gateway<br>（ユーザープールと連携させておく）"]
{{< /mermaid >}}

Web アプリなどで Amazon Cognito の __ユーザープール__ を使って認証（サインイン）すると、Cognito は __ID トークン__ を返します（JWT: JSON Web Token）。
この ID トークンは、サービスの正式なユーザーであるということを証明するものです。
この ID トークンを API 呼び出し時に検証することで、認証が必要な API を実現することができます。

AWS で何らかの Web API を作成する場合は、API Gateway (REST API) や AppSync (GraphQL API) などを使うことになりますが、これらのリソースには Cognito ユーザープールをオーソライザーとして設定することができます。
これにより、Web ブラウザからの API Gateway アクセスを、Cognito でのサインインが終わったユーザーに限定することができます（認証付き API）。
デフォルトでは、`Authorization` ヘッダーで ID トークンを送ってもらう設定になっています。
ID トークンはセッション情報として保存されます。

上記の例では、Cognito が返した ID トークンを API Gateway で使う構成になっていますが、ID トークン (JWT: JSON Web Token) は標準化されており、独自の API サーバーで認証付き API を実現したい場合にも使用できます。
その場合は、API サーバーはクライアントから ID トークンを受け取り、それが正しいものかどうかを Cognito サービスに問い合わせて検証します。

{{< mermaid >}}
graph LR;
  browser[Web ブラウザ] --サインイン--> cognito[Cognito<br>ユーザープール];
  cognito --ID トークン--> browser
  browser --"認証が必要な API の呼び出し<br>（with ID トークン）"--> api["独自の API サーバー<br>（Cognito で ID トークンを検証）"]
{{< /mermaid >}}


Cognito の ID プールは関係ない？
----

Cognito には、ユーザープールの他に、__ID プール__（フェデレーティッド ID）というものがあって、ちょっとややこしいです。

{{< mermaid >}}
graph LR;
  browser[Web ブラウザ] --サインイン--> cognito["Cognito<br>ユーザープール<br> + IDプール"];
  cognito --アクセストークン--> browser;
  browser --"AWS のアクセス権限が必要な API 呼び出し<br>（with アクセストークン）"--> aws["AWS サービス<br>（S3 や DynamoDB）"];
{{< /mermaid >}}

ID プールの方は、Cognito によって認証されたユーザーに対して、AWS 内のサービス（S3 や DynamoDB など）へのアクセス権限を直接付加（認可）するために使用します。
Cognito で ID プールを設定しておくと、認可された AWS サービスにアクセスするための __アクセストークン__ を返してくれるようになります（ID トークンと一緒に返されます）。
ID トークンを使った API Gateway 経由での API アクセスとは異なり、アクセストークンには AWS リソースを参照する権限が付いているので、Web アプリなどのフロントエンドから直接 AWS リソースにアクセスする形になります。

React アプリなどを作成する場合は、Amplify SDK を使用することで、ID トークンやアクセストークンなどの複雑な処理を隠蔽できます。
ちなみに、API Gateway 経由で実行される Lambda 関数の実行権限は、あくまで AWS 内のリソースに割り当てられた IAM ロールなどで決まるため、Cognito の ID プール（フェデレーティッド ID）によって割り当てられた権限（アクセストークン）は使われません。

