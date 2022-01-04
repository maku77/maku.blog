---
title: "React実装例: クリックで開閉可能なツリービューを作る"
linkTitle: "実装例: クリックで開閉可能なツリービューを作る"
url: "/p/weow5dm"
date: "2020-11-11"
tags: ["React"]
weight: 1000
---

ここでは、React コンポーネントとして、開閉可能なツリービューを作ってみます。
コンポーネント名はツリーのノードを示す `TreeNode` です。

{{< image src="img-001.gif" border="true" title="TreeNode コンポーネントの表示例" >}}

初期の表示内容としては、ルートの `TreeNode` を 1 つだけ配置し、そのラベルをクリックしたときに、子要素となる `TreeNode` を 3 つ生成して表示します。
実際のアプリケーションでは、このタイミングで GraphQL サーバーなどからデータを取得してツリー展開していく、といったことができると思います。

下記は `TreeNode` コンポーネントの実装です。

{{< code lang="tsx" title="components/TreeNode.tsx" >}}
import * as React from 'react';
import styles from './TreeNode.scss';

export const TreeNode: React.FC<{label: string}> = ({label}) => {
  const [isOpen, setIsOpen] = React.useState(false);
  const [childNodes, setChildNodes] = React.useState(null);

  // チェックボックスのクリックで isOpen ステートを更新
  const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    setIsOpen(e.target.checked);
  }

  // isOpen ステートの変更を監視し、childNodes に子ノードとなる TreeNode をセットする
  React.useEffect(() => {
    setChildNodes(isOpen ? createChildNodes(label) : null);
  }, [isOpen])

  return <>
    <div className={styles.TreeNode}>
      <label>
        <input type="checkbox" onChange={handleChange} />
        <span className={styles.TreeNode_icon} />
        {label}
      </label>
      <div className={styles.TreeNode_children}>
        {childNodes}
      </div>
    </div>
  </>;
};

// 3つの TreeNode 要素を適当に作る
function createChildNodes(labelPrefix: String): React.ReactElement {
  const labels = [...Array(3)].map((_, i) => labelPrefix + '-' + (i + 1));
  return <>
    {labels.map(x => <TreeNode key={x} label={x} />)}
  </>;
};
{{< /code >}}

ツリーノードの開閉状態は、HTML のチェックボックス (`<input type="checkbox">`) から取得できる情報をそのまま利用しています（上記の例では `e.target.checked` で参照しています）。
この状態が `true` になったとき、動的に子ノードとなる `TreeNode` を 3 つ作成して表示しています (`createChildNodes`)。
子ノードも `TreeNode` なので、クリックすれば無限に掘り進んでいくことができます。

下記は表示をカスタマイズするためのスタイルシートです。
上記コードから CSS Modules の仕組みでインポートしています。
開閉処理自体は `TreeNode.tsx` 側のコードで完結しているので、スタイル設定がなくても最低限の動作はしますが、このスタイル設定により、

- チェックボックスの代わりにフォルダアイコン（open/closed) を表示
- 子ノードは少しインデントして表示

といったことを行っています。

{{< code lang="scss" title="components/TreeNode.scss" >}}
.TreeNode {
  label {
    display: block;
    &:hover { background: #eee; }
  }
  input[type=checkbox] {
    display: none;  /* チェックボックスの四角は非表示 */
  }
  input[type=checkbox] ~ .TreeNode_icon::before {
    content: '\1F4C1';  /* Closed folder icon */
  }
  input[type=checkbox]:checked ~ .TreeNode_icon::before {
    content: '\1F4C2';  /* Open folder icon */
  }
  &_children {
    padding-left: 1em;
  }
}
{{< /code >}}

あとは、どこかのコンポーネントから、次のようにルート要素となる `TreeNode` を配置すれば表示できます。

{{< code lang="tsx" title="components/App.tsx" >}}
import * as React from 'react';
import { TreeNode } from './TreeNode';

export const App: React.FC = () => {
  return <div style={{margin: '1rem'}}>
    <TreeNode label="項目1" />
  </div>;
};
{{< /code >}}

