---
title: "AWS アカウント、IAM ユーザー、グループ、ポリシーの違い"
url: "/p/4c7sdwg"
date: "2021-02-03"
tags: ["AWS"]
---

{{< private >}}
- [AWS 認証情報 - AWS 全般のリファレンス](https://docs.aws.amazon.com/ja_jp/general/latest/gr/aws-security-credentials.html)
{{< /private >}}

AWS を使う上で最初に理解しておくべき、「アカウント」と「ユーザー」まわりの概念についてまとめておきます。

- 簡単なイメージとしてはこんな感じ。
    - AWS アカウント ... 組織およびルートユーザーのこと。通称アカウント。
    - IAM ユーザー ... 組織内の個別ユーザー。通称ユーザー。

ちなみに、IAM は Identity and Access Management の略。


AWS アカウント（のルートユーザー）
----

- AWS サインアップ時に作られる、組織で 1 つだけ用意されるルートユーザーです。
- AWS アカウントの ID は管理者のメールアドレスです。
- AWS アカウントは、契約／支払い設定などを含むフルアクセス権限があるので、通常の業務ではこのアカウントは使わず、IAM ユーザーを使います。


IAM ユーザー
----

- AWS アカウントから作成されるユーザーで、通常の業務ではこの IAM ユーザーを使います。Linux の一般ユーザーのようなものです。
- 1 つの AWS アカウントから複数の IAM ユーザーを作成することができます。
- IAM ユーザーに IAM ポリシーを割り当てることで、各種リソースへのアクセスが許可されます。IAM ユーザー作成直後は、何もアクセス権限がありません。ちなみに、IAM ポリシーは JSON 形式で記述されます。


IAM グループ
----

- 複数の IAM ユーザーをグルーピングするためのものです。
- IAM グループに対しても IAM ポリシーを割り当てることができ、複数の IAM ユーザーにまとめて権限設定することができます。


IAM ロール
----

- 特定の AWS サービス、IAM ユーザー、IAM グループに一時的に権限を与えるために使われます。例えば、EC2 インスタンスへのアクセス権割り当てなどに使われます。
- IAM ロールがどのような権限を表現するかは、IAM ロール自体に IAM ポリシーを割り当てることで制御します。
- 協力会社のメンバーに自社リソースの権限を与えたいときに、協力会社側で使っている IAM ユーザーに対して、自社で作成した IAM ロールを割り当てる、という使い方ができます。→ 参考: [IAM ユーザーにアクセス権限を委任するロールの作成](https://docs.aws.amazon.com/ja_jp/IAM/latest/UserGuide/id_roles_create_for-user.html)

