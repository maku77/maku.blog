---
title: "React コンポーネントで入力フォームを作成する (2) react-hook-form 編"
url: "/p/k7it5fp"
date: "2022-02-06"
tags: ["React", "TypeScript"]
weight: 302
---

React Hook Form とは
----

React アプリで入力フォームを自力で作ろうとすると、各入力エリアのステート管理などが [意外と大変だったりします](/p/tdpybmw)。
React Hook Form ライブラリ (`react-hook-form`) を使用すると、そのあたりの定型処理をシンプルに記述することができます。

- [React Hook Form](https://react-hook-form.com/) / [npm](https://www.npmjs.com/package/react-hook-form) / [GitHub](https://github.com/react-hook-form/react-hook-form)

React Hook Form は次のような特徴を備えています。

- 軽量（別のライブラリに依存しない）
- TypeScript をサポート
- パフォーマンスがよい（不要なレンダリングを軽減）
- [HTML 標準のフォームバリデーション](https://developer.mozilla.org/ja/docs/Learn/Forms/Form_validation) との互換性
    - required / min / max / minLength / maxLength / pattern / validate
- React Native でも使える

2022 年 2 月時点で活発に開発が進められており、npm のダウンロード数は右肩上がりに増えています。
GitHub のスター数も 25,000 を超えているため、しばらくは安心して使えそうなライブラリです。


React Hook Form を導入する
----

`react-hook-form` パッケージは、`npm` あるいは `yarn` で簡単にインストールできます。
既存の React プロジェクト内で次のように実行してください。

```console
$ npm install react-hook-form
あるいは
$ yarn add react-hook-form
```


React Hook Form の基本的な使い方
----

次の `MyForm` コンポーネントは、1 つのテキスト入力フィールドと、1 つの数値入力フィールドを持つフォームの実装例です。
ここでは TypeScript を使い、フォームの入力要素の型を `FormData` と定義しています。

{{< code lang="tsx" title="components/MyForm.tsx" >}}
import { FC } from 'react'
import { SubmitHandler, useForm } from 'react-hook-form'

/** フォームの各要素の名前と型を定義 */
type FormData = {
  name: string
  age: number
}

/** Submit ボタンを押したときの処理 */
const onSubmit: SubmitHandler<FormData> = (data) => {
  // data には FormData 型の値が入っている
  alert(JSON.stringify(data, null, 2))
}

export const MyForm: FC = () => {
  const { handleSubmit, register } = useForm<FormData>()

  return (
    <form onSubmit={handleSubmit(onSubmit)}>
      <input {...register('name')} placeholder="Name" />
      <input type="number" {...register('age')} placeholder="Age" />
      <button type="submit">OK</button>
    </form>
  )
}
{{< /code >}}

<center>
  表示イメージ<br/>
  <input placeholder="Name" />
  <input type="number" placeholder="Age" />
  <button>OK</button>
</center>

React Hook Form の機能は、__`useForm`__ フックの形で提供されているので、使いたい機能（関数）を、コンポーネントの先頭で次のように取得します。

```tsx
const { handleSubmit, register } = useForm<FormData>()
```

あとは、各入力要素の属性に、__`register`__ 関数の戻り値をセットしてやれば OK です（戻り値には `onChange` や `name`、`ref` などが含まれています）。
他の属性（`type` や `placeholder` など）は、これまで通り一緒に指定できます。

```tsx
<input {...register('name')} placeholder="Name" />
<input type="number" {...register('age')} placeholder="Age" />
```

このように記述するだけで、入力要素が React Hook Form に登録され、内部で入力内容が管理されるようになります。
もっとも、実装者はこのあたりの動きを意識する必要はなく、通常は Submit ボタンを押したときにフォーム入力値を参照するだけで済みます。

```tsx
const onSubmit: SubmitHandler<FormData> = ({ name, age }) => {
  console.log(name, age)
}
```

なお、`register` 関数に渡す名前は、自分で定義した `FormData` 型のプロパティ名に合わせる必要があることに注意してください。
上記の例の場合は、`name` と `age` 以外の名前を渡そうとするとエラーになります。
TypeScript を使っていることで、このような指定ミスを早期に発見できます。


入力制限と Validation 処理
----

React Hook Forms で各フィールドの Validation 処理を行うには、`register` 関数の第 2 引数 (`options`) のオプションプロパティを使用します。
例えば、下記の入力フィールドは、値の入力が必須 (required) で、8 文字以上なければいけない (minLength) ことを示しています。

```
<input {...register('name', { required: true, minLength: 8 })} />
```

ユーザーが不適切な値を入力した場合は、`useForm` 関数が返すオブジェクトの、__`formState.errors`__ にエラー情報が格納されるので、これを使ってユーザーに修正を促すことができます。

### 入力必須のフィールド

入力必須なフィールドを作成するには、`register` 関数の第 2 引数 (`options`) に渡すオブジェクトの __`required`__ プロパティに、`true` あるいは文字列を指定します。

{{< code lang="tsx" title="components/MyForm.tsx" >}}
import { FC } from 'react'
import { useForm } from 'react-hook-form'

type FormData = { firstName: string }

export const MyForm: FC = () => {
  const {
    formState: { errors },
    handleSubmit,
    register,
  } = useForm<FormData>()

  return (
    <form onSubmit={handleSubmit((d) => alert(JSON.stringify(d)))}>
      <label>
        First name
        <input
          aria-invalid={errors.firstName ? 'true' : 'false'}
          {...register('firstName', { required: true })}
        />
      </label>
      {errors.firstName && <span role="alert">このフィールドは必須です</span>}
      <button type="submit">OK</button>
    </form>
  )
}
{{< /code >}}

上記の例では、`required` プロパティに `true` を指定していますが、代わりにエラー時に表示したいテキストを指定しておくこともできます。
このテキストは、エラー発生時（Invalid 時）にエラーオブジェクトの __`message`__ プロパティで参照できるようになります。
次のコードは、前述のコードと同様に振る舞います。

```tsx
<input
  aria-invalid={errors.firstName ? 'true' : 'false'}
  {...register('firstName', { required: 'このフィールドは必須です' })}
/>
{errors.firstName && <span role="alert">{errors.firstName.message}</span>}
```

なお、デフォルトでは、ユーザーが一度も Submit ボタンを押していない状態では、入力必須フィールドに何も入力されていなくてもエラー状態にはなりません。
これは、フォーム表示時に最初からエラー状態にならないようにするためです。

### 数値範囲や文字数の制約

入力文字数に制約を持たせたいときは __`minLength`__/__`maxLength`__ オプションを使用します。
次の例では、8 〜 20 文字での入力を必須にしています。
`required` プロパティを同時に指定しておかないと、何も入力しなかったときに Valid（妥当）だと判断されてしまうので注意してください。

```tsx
<label>
  First name
  <input
    aria-invalid={errors.firstName ? 'true' : 'false'}
    {...register('firstName', {
      required: '名前の入力は必須です',
      minLength: { value: 8, message: '8文字以上必要です' },
      maxLength: { value: 20, message: '20文字以下にしてください' },
    })}
  />
</label>
{errors.firstName && <span role="alert">{errors.firstName.message}</span>}
```

数値の範囲に制約を持たせたいときは __`min`__/__`max`__ オプションを指定します。
使い方は同様です。

```tsx
<label>
  Age
  <input
    type="number"
    min={0}
    max={200}
    aria-invalid={errors.age ? 'true' : 'false'}
    {...register('age', {
      required: '年齢の入力は必須です',
      min: { value: 0, message: '年齢が不正です' },
      max: { value: 200, message: '年齢が不正です' },
    })}
  />
</label>
{errors.age && <span role="alert">{errors.age.message}</span>}
```

{{% note %}}
ここでは、`register` 関数のオプションオブジェクトだけではなく、`input` 要素自体の `min` / `max` 属性を指定していますが、これらを指定することで、ブラウザ本来の入力制限機能が有効になります。
両方指定すると煩雑になりそうですが、実際の振る舞いを見て、どちらがユーザビリティが高いか判断するのがよさそうです。
`required`、`minLength`、`maxLength`、`pattern` などに関しても同様です。
{{% /note %}}

### 正規表現パターンによる制約

次の例では、メールアドレス形式の文字列がちゃんと入力されているかをチェックしています。

```tsx
<label>
  Email
  <input
    type="email"
    aria-invalid={errors.email ? 'true' : 'false'}
    {...register('email', {
      pattern: {
        value: /\S+@\S+\.\S+/,
        message: 'メールアドレスが不正です',
      },
    })}
  />
</label>
{errors.email && <span role="alert">{errors.email.message}</span>}
```

### 入力エラー時の handleSubmit の振る舞い

フォームが正しく入力されていない状態（Invalid 状態）のときに Submit ボタンが押された場合、`handleSubmit` の第 1 引数に指定したハンドラ関数は呼び出されません。

```tsx
<form onSubmit={handleSubmit(onSubmit)}>
```

なので、上記の `onSubmit` 関数は、正しい入力値（妥当だと判断された `FormData` オブジェクト）が渡されてくるという前提で実装して構いません。
もし、Invalid 状態で何らかのハンドラ関数を呼び出して欲しいのであれば、`handleSubmit` の第 2 引数 (`onInvalid`) に追加のハンドラ関数を渡せます。

```tsx
<form onSubmit={handleSubmit(onSubmit, onValidationFailed)}>
```


入力値をリセットする
----

フォームの入力値を初期状態に戻すには、`useForm` フックから返された、__`reset`__ メソッドを呼び出します。
次の例では、Reset ボタンを押すことでフォーム要素をクリアするようにしています。
フォーム内に配置した `button` 要素はデフォルトでサブミットボタンとして機能するため、このような特殊用途で使う場合は、`type="button"` の指定が必要であることに注意してください。

```tsx
const { handleSubmit, register, reset } = useForm<FormData>()
// ...
<button type="button" onClick={() => reset()}>Reset</button>
<button type="submit">OK</button>
```

ただし、フォームには明示的なリセットボタンは配置しないほうがよいとされています。
リセットボタンは OK ボタンと間違えて押されてしまうことの方が多く、ユーザーをイラつかせてしまうためです。

