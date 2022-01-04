---
title: "Electron アプリのレイアウトにはフレックスボックスなど新しいスタイルを使用する"
url: "/p/awamxak"
date: "2020-07-14"
tags: ["CSS", "Electron"]
---

Electron アプリのレンダリングには、最新の Chromium を使用できるため、比較的新しい CSS 機能を安心して使用することができます。
Web ブラウザで動作するわけではないので、ベンダープレフィックスなどのケアをする必要もありません。

例えば、次の例では、サイドバーと領域の広がる本文部分に別れるレイアウトを、CSS のフレックスボックス (Flexbox) で実現しています（ここでは React を使用しています）。

{{< image w="400" src="img-001.png" >}}

{{< code lang="tsx" title="app.tsx" >}}
import * as React from 'react';

const styles: {[key: string]: React.CSSProperties} = {
  container: {
    display: 'flex',
    minHeight: '100vh',
  },
  sidebar: {
    background: 'lightgray',
    width: '150px',
  },
  body: {
    background: '#ddd',
    flex: 1,
  }
};

export class App extends React.Component {
  public render(): React.ReactNode {
    return (
      <div style={styles.container}>
        <div style={styles.sidebar}>Sidebar</div>
        <div style={styles.body}>Body</div>
      </div>
    );
  }
}
{{< /code >}}

フレックスボックスはとても柔軟で、[画面上端で左右に広がるメニュー](https://developer.mozilla.org/ja/docs/Web/CSS/CSS_Flexible_Box_Layout/Aligning_Items_in_a_Flex_Container#%E4%B8%BB%E8%BB%B8%E4%B8%8A%E3%81%A7%E3%81%AE%E4%BD%8D%E7%BD%AE%E5%90%88%E3%82%8F%E3%81%9B%E3%81%AB_auto_%E3%83%9E%E3%83%BC%E3%82%B8%E3%83%B3%E3%82%92%E4%BD%BF%E3%81%86) など、いろいろな用途に使用することができます。


