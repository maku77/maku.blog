---
title: "Flutter ドラフトメモ"
url: "p/qbt5dmu/"
date: "2022-08-05"
tags: ["Flutter"]
draft: true
---

Flutter の歴史
----

{{% private %}}
- [Flutter release notes](https://docs.flutter.dev/development/tools/sdk/release-notes)
{{% /private %}}

- Ver 2.10 (2022-02) ... Windows 対応が安定版になった。
- Ver 2.2 (2021-05) ... Dart 2.13 が同梱され、タイプエイリアスをサポート。
- Ver 2.0 (2021-03) ... Web 対応が安定版になった。Dart 2.12 が同梱され、Null 安全をサポート。
- Ver 1.12 (2019-12) ... Dart 2.7 が同梱され、Extension methods をサポート。
- Ver 1.0 (2018-12) ... Android/iOS 対応が安定版になった。


Flutter の特徴
----

- __Dart 言語__ を採用。Java に似ていてとっつきやすく、型安全で拡張性も高い。async/await などの非同期メカニズムも標準でサポートしている。
- 描画エンジンとして __Skia__ を搭載し、高速なレンダリングが可能。
- ホットリロードによりアプリを動かしながら UI 調整が可能。
- マテリアルデザインを UI/UX の基本として採用（Material Components ウィジェットを提供）。


その他
----

- Flutter のコード（`main.dart` ファイルなど）は、__`lib`__ ディレクトリの下に入れていく。

- ウィジェット
  - Stateless ... すべてのフィールドが final で常に同じ状態で表示されるウィジェット。
  - Stateful ... State とともに記述され？状態の変更が可能なウィジェット。

