---
title: "Android開発: ダイアログ表示時の背景の暗転（ディミング）を防ぐ"
linkTitle: "ダイアログ表示時の背景の暗転（ディミング）を防ぐ"
url: "/p/voanw2h"
date: "2020-03-06"
tags: ["Android"]
---

Android でダイアログを表示するときに `DialogFragment` クラスなどを使用すると、デフォルトではダイアログの後ろは暗くなって、下の UI がうっすらと見える状態になります。
このような効果を抑制して、背景が明るいままにするには、ウィンドウから **`FLAG_DIM_BEHIND`** フラグをクリアします。

{{< code lang="kotlin" >}}
class MyDialogFragment : DialogFragment() {
    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // ダイアログの中身を独自レイアウトにする
        return inflater.inflate(R.layout.my_dialog, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        disableBackgroundDimming()
    }

    /** ダイアログの背景が暗くなるのを防ぐ */
    private fun disableBackgroundDimming() {
        dialog?.window?.clearFlags(WindowManager.LayoutParams.FLAG_DIM_BEHIND)
    }
}
{{< /code >}}

