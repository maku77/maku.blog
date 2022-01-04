---
title: "C# で Microsoft Outlook の情報を取得する"
url: "/p/eb5nx4z"
date: "2018-06-18"
tags: ["プログラム", ".NET"]
---

組織内で Outlook を使用している場合は、プログラムから Outlook の情報（Exchange サーバの情報）を取得してごにょごにょすると、日々の作業を効率化できるかもしれません。
ここでは、C# から Outlook の情報を取得する方法を紹介します。

プログラミング言語に C# を使っていますが、もともとは VBA などのインタフェース (COM) として使われていたものなので、本格的にコーディングするときは、VBA のリファレンスが参考になります。

- [Outlook VBA リファレンス｜MSDN](https://msdn.microsoft.com/ja-jp/vba/vba-outlook)

以下のサンプルは、Visual Studio Community 2017 を使って確認しています。
Visual Studio で新規のプロジェクト（ここではシンプルにコンソールアプリを選択）を作成したら、まずは次のようにして Outlook のインタフェースの参照を追加してください。

1. プロジェクト(P) → 参照の追加(R)
2. COM → **Microsoft Outlook 14.0 Object Library**

次のコードは、現在 Outlook を使用しているユーザの名前とメールアドレス、および上司の名前とメールアドレスを表示します。

{{< code lang="csharp" >}}
using System;
using Outlook = Microsoft.Office.Interop.Outlook;

namespace ConsoleApp1
{
    class Program
    {
        static Outlook.Application outlookApp = new Outlook.Application();

        // Exchange ユーザの情報を表示する
        static void ShowRecipientInfo(Outlook.ExchangeUser user)
        {
            Console.WriteLine(user.Name);  // 名前
            Console.WriteLine(user.PrimarySmtpAddress);  // メールアドレス
        }

        static void Main(string[] args)
        {
            // カレントユーザーの情報を表示
            Outlook.Recipient rcp = outlookApp.Session.CurrentUser;
            Outlook.ExchangeUser currentUser = rcp.AddressEntry.GetExchangeUser();
            ShowRecipientInfo(currentUser);

            // 上司の情報を表示
            Outlook.ExchangeUser manager = currentUser.GetExchangeUserManager();
            if (manager != null)
            {
                ShowRecipientInfo(manager);
            }
        }
    }
}
{{< /code >}}

{{< code title="実行結果" >}}
Yamada Taro
yamada-taro@example.com
Tokoro George
tokoro-george@example.com
{{< /code >}}


参考
----

- [Office - Dev Center - 操作方法 (Outlook リファレンス)](https://msdn.microsoft.com/ja-jp/library/office/bb612741.aspx)

