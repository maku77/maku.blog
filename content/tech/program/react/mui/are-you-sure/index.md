---
title: "MUI で GitHub のようなリソース削除の確認ダイアログを作る（AreYouSure ダイアログ）"
url: "p/cbuan5h/"
date: "2022-08-10"
tags: ["Material-UI"]
---

何を作るか？
----

GitHub や AWS などで、何らかのリソースを削除するときに、次のような確認ダイアログが表示されることがあります。

{{< image src="img-001.png" title="GitHub のリポジトリ削除時の確認ダイアログ" >}}

ユーザーは、削除対象のリソース名をタイピングするまで、削除ボタンを押すことができません。
これは、不用意にリソースを削除してしまわないための措置で、このようなダイアログは様々な場所で活用できます。
ここでは、MUI (Material-UI) で同様のリソース削除確認ダイアログ (`AreYouSureDialog`) の実装例を紹介します。
コードをシンプルにするために、表示テキストをハードコーディングしていますが、props に切り出せば汎用的な確認ダイアログとして使えるはずです。


AreYouSureDialog を実装する
----

完成時のイメージはこんな感じです。
UI のベースは、[MUI の Dialog コンポーネント](https://mui.com/material-ui/react-dialog/)です。

{{< image src="img-002.png" >}}

### ダイアログコンポーネントの実装

{{< code lang="tsx" title="components/AreYouSureDialog.tsx" >}}
import { FC, useCallback, useState } from 'react'
import {
  Button,
  Dialog,
  DialogContent,
  DialogContentText,
  DialogTitle,
  TextField,
  Typography,
  IconButton,
} from '@mui/material'
import CloseIcon from '@mui/icons-material/Close'

/** ダイアログコンポーネントの props の型定義 */
type Props = {
  /** ダイアログを表示するなら true */
  open: boolean

  /** 操作対象のリソース名（= ユーザーに入力させるテキスト） */
  targetName: string

  /**
   * ダイアログを閉じるべきときに呼び出されます。
   *
   * ユーザーが処理を承認したときは、submit の値が true となり、
   * 捜査対象のリソース名が targetName に格納されます。
   */
  onClose?: (submit: boolean, targetName?: string) => void
}

/**
 * ユーザーに確認を促すダイアログコンポーネントです。
 * ユーザーは提示されたテキストを入力することで、承認ボタンを押せるようになります。
 */
export const AreYouSureDialog: FC<Props> = ({
  open,
  targetName,
  onClose,
}: Props) => {
  // 削除ボタンを有効にするか
  const [submitAvailable, setSubmitAvailable] = useState(false)

  // ユーザー入力に応じて、削除ボタンの有効・無効を切り替える
  const handleInputChange = useCallback(
    (input: string) => {
      setSubmitAvailable(input === targetName)
    },
    [targetName, setSubmitAvailable]
  )

  // ダイアログを閉じるときの処理（クライアント側へコールバックする）
  const handleClose = useCallback(
    (submit: boolean, targetName?: string) => {
      setSubmitAvailable(false)
      onClose?.(submit, targetName)
    },
    [onClose, setSubmitAvailable]
  )

  return (
    <Dialog open={open} onClose={() => handleClose(false)}>
      <DialogTitle sx={{ m: 0, p: 2 }}>
        本当に削除しますか？
        <IconButton
          aria-label="close"
          onClick={() => handleClose(false)}
          sx={{
            position: 'absolute',
            right: 8,
            top: 8,
            color: (theme) => theme.palette.grey[500],
          }}
        >
          <CloseIcon />
        </IconButton>
      </DialogTitle>

      <DialogContent>
        <DialogContentText>
          <Typography gutterBottom>
            <b>{targetName}</b>{' '}
            を削除すると、リソースに紐づくすべての情報が削除されます。
            この処理を取り消すことはできません。
          </Typography>
          <Typography gutterBottom>
            本当に削除するのであれば、<b>{targetName}</b> と入力してください。
          </Typography>
        </DialogContentText>
        <TextField
          autoFocus
          autoComplete="off"
          size="small"
          margin="dense"
          fullWidth
          onChange={(e) => handleInputChange(e.target.value)}
        />
        <Button
          disabled={!submitAvailable}
          variant="contained"
          color="error"
          sx={{ width: '100%' }}
          onClick={() => handleClose(true, targetName)}
        >
          本当にこのリソースを削除します！
        </Button>
      </DialogContent>
    </Dialog>
  )
}
{{< /code >}}

`props` の `targetName` で渡されるテキストが、ユーザーに入力してもらうテキストになります。
ユーザーが入力エリアでタイピングするたびに `handleInputChange` が呼び出され、入力内容が `targetName` の値と等しくなったときに削除ボタンを有効化しています。

### ダイアログを使う側のサンプルコード

上で実装した `AreYouSureDialog` コンポーネントを使う側のサンプルコードです。
Next.js のページ (`NextPage`) を想定していますが、React のコンポーネントであれば同様に実装できます。

{{< code lang="tsx" title="pages/sample.tsx" >}}
import { useCallback, useState } from 'react'
import { NextPage } from 'next'
import { Button } from '@mui/material'

import { AreYouSureDialog } from '../components/common/AreYouSureDialog'

/** テスト用のページ */
const SamplePage: NextPage = () => {
  const [open, setOpen] = useState(false) // ダイアログの開閉状態
  const [result, setResult] = useState('') // 結果確認用

  // AreYouSureDialog からの Close イベントをハンドル
  const handleClose = useCallback(
    (submit: boolean, targetName?: string) => {
      // ダイアログを閉じる
      setOpen(false)

      // 実際にはここでアプリ固有の処理を行う★
      setResult(submit ? `submitted with ${targetName ?? ''}` : 'canceled')
    },
    [setOpen, setResult]
  )

  return (
    <>
      <Button onClick={() => setOpen(true)}>Open Dialog</Button>
      <div>Result = {result}</div>
      <AreYouSureDialog
        open={open}
        targetName="your-resource"
        onClose={handleClose}
      />
    </>
  )
}

export default SamplePage
{{< /code >}}

ページ上に配置された `Open Dialog` ボタンを押すと `open` 変数が `true` にセットされ、ダイアログが表示されます。
ユーザーがダイアログを閉じると `handleClose` が呼び出されます。
ここでは、結果を表示しているだけ（コードの★の部分）ですが、実際にはそこでリソースの削除処理などを行います。

