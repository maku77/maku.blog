---
title: "Gimp チートシート"
url: "p/wob9frk/"
date: "2023-10-29"
tags: ["Gimp", "cheatsheet"]
---

{{% private %}}
- Gimp の各種操作アイコンが格納されているディレクトリ
  - `/Applications/GIMP.app/Contents/Resources/share/gimp/2.0`
{{% /private %}}

Gimp のショートカット操作を覚えると効率的に画像編集を行えるようになります。
デフォルトで画面左上に表示される各種ツールは頻繁に切り替えて使用することになるため、単一のキー（あるいは `Shift` との組み合わせ）によるショートカットが割り当てられています。

Gimp を使いこなす上での最初の関門は、おそらく「選択」に関する操作なので、最初に選択系の操作をまとめています。
下記の説明内の `Cmd` キーは、Windows の場合は `Ctrl` キーに読み替えてください。

選択範囲 (Selection)
----

| &nbsp; | キー | 説明 | メモ |
| :--: | ---- | ---- | ---- |
| {{< image-inline w="30" src="img/gimp-selection-all.svg" >}} | `Cmd + A` | すべてを選択 (Select <strong>A</strong>ll) | |
| {{< image-inline w="30" src="img/gimp-selection-none.svg" >}} | <code style="white-space: nowrap">Shift + Cmd + A</code> | 選択を解除 (Select None) | |
| {{< image-inline w="30" src="img/gimp-tool-rect-select.svg" >}} | `R` | 「矩形選択」ツール (Select <strong>R</strong>ectangle) | |
| {{< image-inline w="30" src="img/gimp-tool-ellipse-select.svg" >}} | `E` | 「楕円選択」ツール (Select <strong>E</strong>llipse) | |
| {{< image-inline w="30" src="img/gimp-tool-free-select.svg" >}} | `F` | 「自由選択」ツール (Select <strong>F</strong>ree) | |
| {{< image-inline w="30" src="img/gimp-tool-iscissors.svg" >}} | `I` | 「電脳はさみ」ツール (<strong>I</strong>ntelligent Scissors) | 境界を自動計算して領域選択 |
| {{< image-inline w="30" src="img/gimp-tool-fuzzy-select.svg" >}} | `U` | 「ファジー選択」ツール (Select F<strong>u</strong>zzy) | クリック位置から近似色領域を選択 |
| {{< image-inline w="30" src="img/gimp-tool-by-color-select.svg" >}} | `Shift + O` | 「色域を選択」ツール (Select by C<strong>o</strong>lor) | 全体から同一色の領域を選択 |
| {{< image-inline w="30" src="img/gimp-invert.svg" >}} | `Cmd + I` | 選択範囲を反転 (Select <strong>I</strong>nverse) | |
| {{< image-inline w="30" src="img/gimp-floating-selection.svg" >}} | `Shift + Cmd + L` | 選択範囲をフロート化 (F<strong>l</strong>oat Selection) | 選択範囲を切り抜いて独立レイヤー化（`Cmd + X` → `Cmd + P` でも可） |
| {{< image-inline w="30" src="img/gimp-floating-selection.svg" >}} | `Cmd + C` →<br/>`Cmd + P` | 選択範囲のコピー＆ペースト | 選択範囲をコピーして独立レイヤー化 |
| {{< image-inline w="30" src="img/gimp-selection-replace.svg" >}} | `Shift + V` | パスを選択範囲化 (Select from Path) | |
| {{< image-inline w="30" src="img/gimp-selection-to-path.svg" >}} | ─ | 選択範囲をパス化 (Selection to Path) | |
| ─ | `Cmd + T` | 選択範囲境界線（白色破線）の表示・非表示 (<strong>T</strong>oggle Selection) | 非表示になっても選択範囲は有効 |

- 選択範囲の境界線は __白色の破線__ で表示されます。
  - 黄色の破線はレイヤーの境界線なので混同しないようにしてください。
  - `Cmd + A` で全体を選択すると、白色の破線（選択範囲）が黄色の破線（レイヤー）の上に重なって、黄色の破線が見えなくなることがあります（落ち着いて `Shift + Cmd + A` で選択範囲を解除すれば見えるようになります）。
- 現在の選択範囲が __描画系ツールの描画対象__ になります。
  - ただし、どの領域も選択されていない場合は、アクティブなレイヤー全体が描画対象です。
  - より具体的には、「アクティブなレイヤー」と「選択範囲」の重複領域が描画対象になります。
  - 操作例: `R` で矩形選択 → `N` で鉛筆で描画
- レイヤーを切り替えても、現在の選択範囲はそのまま使いまわされます。
- 「選択」系ツールでは次のキーを押しながら選択することで領域を追加・削除できます。
  - `Shift` キーを押しながら範囲指定 ... 現在の選択範囲に追加します
  - `Cmd` キーを押しながら範囲指定 ... 現在の選択範囲から削除します
  - `Shift + Cmd` キーを押しながら範囲指定 ... 現在の選択範囲との交差部分 (XOR) を選択範囲にします

  たとえば、「ファジー選択」ツール (`U`) による選択範囲を、`Shift` キーを押しながら追加していくと、類似色の領域をどんどん広げていけます。


描画系・レタッチ
----

| &nbsp; <div style="width: 30px"></div> | キー | 説明 | メモ |
| :--: | ---- | ---- | ---- |
| {{< image-inline w="30" src="img/gimp-tool-pencil.svg" >}} | `N` | 「鉛筆で描画」ツール (Pe<strong>n</strong>cil) | 固いタッチの描画。「<strong>えぬ</strong>ぴつ」と覚える |
| {{< image-inline w="30" src="img/gimp-tool-paintbrush.svg" >}} | `P` | 「ブラシで描画」ツール (<strong>P</strong>aintbrush) | スムーズなタッチの描画 |
| {{< image-inline w="30" src="img/gimp-tool-airbrush.svg" >}} | `A` | 「エアブラシで描画」ツール (<strong>A</strong>irbrush) | 圧力調整可能なエアブラシ |
| {{< image-inline w="30" src="img/gimp-tool-ink.svg" >}} | `K` | 「インクで描画」ツール (In<strong>k</strong>) | カリグラフィスタイルの描画 |
| {{< image-inline w="30" src="img/gimp-tool-clone.svg" >}} | `C` | 「スタンプで描画」ツール (<strong>C</strong>lone Stamp) | `Cmd + クリック` で参照起点を指定 |
| {{< image-inline w="30" src="img/gimp-tool-heal.svg" >}} | `H` | 「修復ブラシ」ツール (<strong>H</strong>ealing Brush) | `Cmd + クリック` で参照起点を指定 |
| {{< image-inline w="30" src="img/gimp-tool-mypaint-brush.svg" >}} | `Y` | 「MyPaint ブラシで描画」ツール (M<strong>y</strong>Paint Brush) | いろんなブラシ |
| {{< image-inline w="30" src="img/gimp-tool-smudge.svg" >}} | `S` | 「にじみ」ツール (<strong>S</strong>mudge) | アクティブなブラシで滲ませる |
| {{< image-inline w="30" src="img/gimp-tool-warp.svg" >}} | `W` | 「ワープ」ツール (<strong>W</strong>arp) | |
| {{< image-inline w="30" src="img/gimp-tool-dodge.svg" >}} | `Shift + D` | 「暗室」ツール (<strong>D</strong>odge/Burn) | 「覆い焼き」or「焼き込み」|
| {{< image-inline w="30" src="img/gimp-tool-blur.svg" >}} | `Shift + U` | 「ぼかし/シャープ」ツール (Bl<strong>u</strong>r/Sharpen) | |
| {{< image-inline w="30" src="img/gimp-tool-bucket-fill.svg" >}} | <code style="white-space: nowrap">Shift + B</code> | 「塗りつぶし」ツール (<strong>B</strong>ucket Fill) | デフォルトは近似色領域を塗りつぶし。`Shift + クリック` で選択範囲全体を塗りつぶし |
| ─ | `Del` | 選択範囲をクリア (Clear Selection) | クリア ＝ 背景色で選択範囲全体を塗りつぶし |
| {{< image-inline w="30" src="img/gimp-tool-eraser.svg" >}} | `Shift + E` | 「消しゴム」ツール (<strong>E</strong>raser) | 「ブラシで描画 (`P`)」 との違いは、背景色を使うところ |
| {{< image-inline w="30" src="img/gimp-swap-colors.svg" >}} | `X` | 描画色と背景色の入れ替え (Swap Colors) | |
| {{< image-inline w="30" src="img/gimp-default-colors.svg" >}} | `D` | 描画色と背景色をデフォルトに戻す (<strong>D</strong>efault Colors) | デフォルト色は黒と白 |
| {{< image-inline w="30" src="img/gimp-tool-color-picker.svg" >}} | `O` | 「スポイト」ツール (C<strong>o</strong>lor Picker) | `クリック` で描画色を設定<br/>`Cmd + クリック` で背景色を設定 |
| | `[` / `]` | ブラシサイズを -1/+1 (Decrease/Increase size by 1) |
| | `{` / `}` | ブラシサイズを -1/+1 (Decrease/Increase size by 10) |

- 多くの描画系ツールでは、__`Cmd + クリック`__ でクリック位置の色を描画色に設定できます（スポイト）。「消しゴム」ツールを選択中の場合は、背景色の設定になります。
- 多くの描画系ツールでは、__`Shift + クリック`__ で直線の描画が可能です。


移動・変形
----

| &nbsp; | キー | 説明 | メモ |
| :--: | ---- | ---- | ---- |
| {{< image-inline w="30" src="img/gimp-tool-move.svg" >}} | `M` | 「移動」ツール (<strong>M</strong>ove) | レイヤーや選択範囲を移動可能 |
| {{< image-inline w="30" src="img/gimp-tool-rotate.svg" >}} | <code style="white-space: nowrap">Shift + R</code> | 「回転」ツール (<strong>R</strong>otate) | |
| {{< image-inline w="30" src="img/gimp-tool-scale.svg" >}} | `Shift + S` | 「拡大・縮小」ツール (<strong>S</strong>cale) | |
| {{< image-inline w="30" src="img/gimp-tool-crop.svg" >}} | `Shift + C` | 「切り抜き」ツール (<strong>C</strong>rop) | |
| {{< image-inline w="30" src="img/gimp-tool-perspective.svg" >}} | `Shift + P` | 「遠近法」ツール (<strong>P</strong>erspective) | 四隅を動かして台形補正 |
| {{< image-inline w="30" src="img/gimp-tool-shear.svg" >}} | `Shift + H` | 「剪断変形」ツール (S<strong>h</strong>ear) | 矩形を平行四辺形にする |
| {{< image-inline w="30" src="img/gimp-tool-transform-3d.svg" >}} | `Shift + W` | 「3D 変換」ツール (3D Transform) | 奥行きをともなってグルングルン回る |
| {{< image-inline w="30" src="img/gimp-tool-cage.svg" >}} | `Shift + G` | 「ケージ変形」ツール (Ca<strong>g</strong>e) | 任意のケージ領域を変形 |
| {{< image-inline w="30" src="img/gimp-tool-flip.svg" >}} | `Shift + F` | 「鏡像反転」ツール (<strong>F</strong>lip) | 水平か垂直方向に反転 |
| {{< image-inline w="30" src="img/gimp-tool-unified-transform.svg" >}} | `Shift + T` | 「統合変形」ツール (Unified <strong>T</strong>ransform) | 各種変形をまとめて行える |
| {{< image-inline w="30" src="img/gimp-tool-align.svg" >}} | `Q` | 「整列」ツール (Align) | 例: `Shift` キーを押しながら複数のテキストを選択して左揃え |


ズーム・表示位置 (Zoom / Move)
----

| &nbsp; | キー | 説明 | メモ |
| :--: | ---- | ---- | ---- |
| {{< image-inline w="30" src="img/gimp-navigation.svg" >}} | <code style="white-space: nowrap">中央ボタン + ドラッグ</code> | 表示位置の移動 | |
| {{< image-inline w="30" src="img/gimp-tool-zoom.svg" >}} | <code style="white-space: nowrap">Cmd + マウスホイール</code> | 拡大・縮小 | `Cmd + 中央ボタンドラッグ` でも可 |
| {{< image-inline w="30" src="img/gimp-tool-zoom.svg" >}} | `Z` | 「ズーム」ツール (<strong>Z</strong>oom) | `Cmd + マウスホイール` でも可 |
| {{< image-inline w="30" src="img/zoom-fit-best.svg" >}} | `Shift + Cmd + J` | ウィンドウ内に全体を表示 (Zoom Fit Best) | |
| {{< image-inline w="30" src="img/zoom-in.svg" >}} | `+` | ズームイン (Zoom In) | `Cmd + マウスホイール` でも可 |
| {{< image-inline w="30" src="img/zoom-out.svg" >}} | `-` | ズームアウト (Zoom Out) | `Cmd + マウスホイール` でも可 |
| {{< image-inline w="30" src="img/zoom-original.svg" >}} | `1` | 表示倍率 100% | |
| ─ | `2` | 表示倍率 200% | |
| ─ | `3` | 表示倍率 400% | |
| ─ | `` ` `` | 表示倍率や位置を戻す (Revert Zoom) | 戻すというよりは「表示倍率と位置」を 1 つ前の状態と交互に入れ替える感じ。拡大作業中に __`1`__ で一時的に 100% 表示を確認して、__`` ` ``__ ですぐに戻すという使い方ができます。連続した表示倍率、および連続した位置変更をした場合は、その途中経過は状態として保存されません。 |


レイヤー (Layer)
----

| &nbsp; | キー | 説明 | メモ |
| :--: | ---- | ---- | ---- |
| {{< image-inline w="30" src="img/gimp-layers.svg" >}} | `Cmd + L` | 「レイヤー」ダイアログへ (Go to Layers) | |
| {{< image-inline w="30" src="img/document-new.svg" >}} | `Shift + Cmd + N` | 新しいレイヤーの追加 (<strong>N</strong>ew Layer) | |
| {{< image-inline w="30" src="img/gimp-duplicate.svg" >}} | `Shift + Cmd + D` | レイヤーの複製 (<strong>D</strong>uplicate Layer) | |
| {{< image-inline w="30" src="img/gimp-floating-selection.svg" >}} | <code style="white-space: nowrap">Shift + Cmd + L</code> | 選択範囲をフロート化 (F<strong>l</strong>oat Selection) | 選択範囲を切り抜いて独立レイヤー化（`Cmd + X` → `Cmd + P` でも可） |
| {{< image-inline w="30" src="img/gimp-floating-selection.svg" >}} | `Cmd + C` →<br/>`Cmd + P` | 選択範囲のコピー＆ペースト | 選択範囲をコピーして独立レイヤー化 |
| {{< image-inline w="30" src="img/gimp-anchor.svg" >}} | `Cmd + H` | レイヤーの固定 (Anchor Layer) | フローティング選択範囲を下のレイヤーに統合 |
| {{< image-inline w="30" src="img/gimp-tool-offset.svg" >}} | `Shift + Cmd + O` | オフセット (<strong>O</strong>ffset Layer) | |
| {{< image-inline w="30" src="img/gimp-merge-down.svg" >}} | `Cmd + M` | レイヤーをマージ (<strong>M</strong>erge Layers) | |
| ─ | `PageUp` | 上のレイヤーをアクティブにする (Select Net Layer) | |
| ─ | `PageDown` | 下のレイヤーをアクティブにする (Select Previous Layer) | |
| ─ | `Home` | 一番上のレイヤーをアクティブにする (Select Top Layer) | |
| ─ | `End` | 一番下のレイヤーをアクティブにする (Select Bottom Layer) | |

### フローティング選択範囲

- 選択範囲のコピペ（`Cmd + C` → `Cmd + P`）などで作成される「__フローティング選択範囲__ {{< image-inline w="26" src="img/gimp-floating-selection.svg" >}}」は、レイヤー化される直前の状態で、一時的に様々な変形や移動が可能になっています。
- 「フローティング選択範囲」がアクティブになっているときは、他のレイヤーの操作は行えません。
- レイヤーダイアログの「__レイヤーの追加__ {{< image-inline w="26" src="img/document-new.svg" >}}」ボタンを押すと、「フローティング選択範囲」は独立したレイヤーに切り出されます。
- レイヤーダイアログの「__レイヤーの固定__ {{< image-inline w="26" src="img/gimp-anchor.svg" >}}」ボタンを押すと、「フローティング選択範囲」は下のレイヤーに統合されます。選択ツールを使用中に、「フローティング選択範囲」の外側をクリックすることでも同じ操作を行えます。
- 他のアプリケーション（プレビューなど）で `Cmd + C` でコピーした領域を、Gimp 上で貼り付け (`Cmd + P`) た場合も、「フローティング選択範囲」が生成されます。


編集（コピー＆ペースト）
----

| &nbsp; | キー | 説明 | メモ |
| :--: | ---- | ---- | ---- |
| ─ | <code style="white-space: nowrap">Shift + Cmd + C</code> | 可視部分のコピー (<strong>C</strong>opy Visible) |  通常の `Cmd + C` は、アクティブなレイヤーの選択範囲をコピーしますが、`Shift` を組み合わせると、見えているレイヤーすべてがコピー対象となります。 |


ナビゲーション (Navigation)
----

| &nbsp; | キー | 説明 | メモ |
| :--: | ---- | ---- | ---- |
| ─ | `Tab` | すべてのドックを隠す・表示する | |
| {{< image-inline w="30" src="img/gimp-layers.svg" >}} | `Cmd + L` | 「レイヤー」ダイアログへ (Go to Layers) | |
| ─ | <code style="white-space: nowrap">Shift + Cmd + B</code> | 「ブラシ」ダイアログへ (Go to Brushes) | |
| {{< image-inline w="30" src="img/gimp-pattern.svg" >}} | `Shift + Cmd + P` | 「パターン」ダイアログへ (Go to Patterns) | |
| {{< image-inline w="30" src="img/gimp-tool-gradient.svg" >}} | `Cmd + G` | 「グラデーション」ダイアログへ (Go to Gradients) | |
| ─ | `Shift + Cmd + T` | ガイドの表示・非表示 (Show/Hide Guides) | ちなみに、ガイドはルーラーからドラッグして追加できます。ガイドの移動・削除は、移動ツール (`M`) で `Shift + ドラッグ` です。 |
| ─ | `Shift + Cmd + R` | ルーラーの表示・非表示 (Show/Hide <strong>R</strong>ulers) | |


その他 (Other)
----

| &nbsp; | キー | 説明 | メモ |
| :--: | ---- | ---- | ---- |
| {{< image-inline w="30" src="img/gimp-tool-path.svg" >}} | `B` | 「パス」ツール (Path) (<strong>B</strong>ezier?) | |
| {{< image-inline w="30" src="img/gimp-tool-text.svg" >}} | `T` | 「テキスト」ツール (<strong>T</strong>ext) | 自動でテキストレイヤーが作成されるので後から編集可能 |
| {{< image-inline w="30" src="img/gimp-tool-measure.svg" >}} | <code style="white-space: nowrap">Shift + M</code> | 「定規」ツール (<strong>M</strong>easure) | 1 本目の線の端をさらに `Shift + ドラッグ` すると任意の角度で計測可能 |

