---
title: "TypeScript とは"
url: "/p/tdouo5p"
date: "2019-09-24"
tags: ["JavaScript", "TypeScript"]
weight: 1
---

TypeScript は、JavaScript に静的な型付けを行えるようにしたプログラミング言語です。
トランスパイラ (`tsc` コマンド）を使って、TypeScript で記述したコードを JavaScript のコードに変換するのが主な使い方になります。
大規模な JavaScript アプリケーションを開発するときに TypeScript を導入すると、品質の高いコードを効率よく作成できるようになります。

TypeScript はマイクロソフトによって開発が進められており、同じくマイクロソフトによって開発されている [Visual Studio Code でコーディングを行う](/p/ak7u3h3)のがよいとされています。

数年前に CoffeeScript と呼ばれる同様の言語が一時的にブームになりましたが、現在は TypeScript が主流です。
TypeScript には下記のような特徴があり、しばらくは JavaScript alternative として主流であり続けるでしょう。

- Microsoft により強力にサポートされており、**Visual Studio Code が最新の TypeScript バージョンに迅速に対応します**。
- 静的な型付けにより、実行前（トランスパイル時）に**コーディングのミスを発見しやすくなります**。型の推論がしやすくなるため、IDE（VS Code など）のプロパティ名の**自動補完が効く**ようになります。これがほんとに便利で、特にサードパーティ製のライブラリを使っているときにありがたみが分かります。
- JavaScript (ECMAScript) の**新しい仕様を使ってコーディングできます**。TypeScript が新しい構文で書かれたコードを過去バージョンの JavaScript コードに変換してくれるため、各ブラウザベンダーが新しい仕様に対応するのを待つ必要がありません。
- JavaScript のコードは有効な TypeScript のコードとして動作します（構文に互換性があります。専門用語では JavaScript の「スーパーセット」であると言う）。そのため、既存の JavaScript プロジェクトに TypeScript を導入しようとするとき、**既存の JavaScript コードを修正する必要がありません**。また、これまでに身に着けた JavaScript のノウハウをそのまま活かし続けることができます。

