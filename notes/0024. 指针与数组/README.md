# [0024. 指针与数组](https://github.com/tnotesjs/TNotes.c/tree/main/notes/0024.%20%E6%8C%87%E9%92%88%E4%B8%8E%E6%95%B0%E7%BB%84)

<!-- region:toc -->

- [1. 🎯 本节内容](#1--本节内容)
- [2. 🫧 评价](#2--评价)
- [3. 🤔 数组名在什么情况下会退化为指针？](#3--数组名在什么情况下会退化为指针)
- [4. 🤔 指针和数组在内存布局上有何异同？](#4--指针和数组在内存布局上有何异同)
- [5. 🆚 使用下标访问 `arr[i]` vs 指针算术 `*(p + i)` 有何区别？](#5--使用下标访问-arri-vs-指针算术-p--i-有何区别)
- [6. 🤔 为什么 `sizeof(arr)` 和 `sizeof(ptr)` 的结果不同？](#6--为什么-sizeofarr-和-sizeofptr-的结果不同)
- [7. 🤔 如何将数组传递给函数？形参应声明为数组还是指针？](#7--如何将数组传递给函数形参应声明为数组还是指针)
- [8. 🤔 多维数组与指向数组的指针如何对应？](#8--多维数组与指向数组的指针如何对应)
- [9. 🤔 什么是数组指针和指针数组？它们的语法和用途有何不同？](#9--什么是数组指针和指针数组它们的语法和用途有何不同)
- [10. 🤔 常见的指针与数组误用有哪些？如何避免？](#10--常见的指针与数组误用有哪些如何避免)
- [11. 🔗 引用](#11--引用)

<!-- endregion:toc -->

## 1. 🎯 本节内容

- 数组名退化规则
- 数组与指针的内存与类型差异
- 下标访问与指针算术等价性
- `sizeof` 与 `&arr` 的关键区别
- 数组传参的退化与长度约定
- 数组指针与指针数组

## 2. 🫧 评价

- 指针与数组的关系是 C 语言的必考点，也是实际工程里最容易踩出未定义行为的地方。
- 你只要抓住三件事就能写稳：数组退化规则、指针类型含义、边界与长度如何传递。

## 3. 🤔 数组名在什么情况下会退化为指针？

数组名在大多数表达式中会退化为指向首元素的指针。

例如：

- `int *p = a;` 等价于 `int *p = &a[0];`
- `a[i]` 等价于 `*(a + i)`

::: code-group

```c [数组名在表达式中退化]
#include <stdio.h>

int main(void) {
  int a[3] = { 1, 2, 3 };
  int *p = a;
  printf("%d %d\n", a[1], *(p + 1));
  return 0;
}
```

:::

但有几个重要例外不会退化：

1. `sizeof(a)`，此时 `a` 表示整个数组对象
2. `&a`，此时得到的是指向整个数组的指针
3. 字符串字面量用于初始化数组时

这些例外正是很多 `sizeof` 与指针类型错误的根源。

## 4. 🤔 指针和数组在内存布局上有何异同？

它们的本质不同：

- 数组是一段连续的同类型元素序列
- 指针是一个保存地址的变量

内存布局上的典型差异：

- 数组的元素连续，数组对象本身占用 `N * sizeof(T)` 字节
- 指针变量本身只占用一个指针大小，例如 8 字节或 4 字节

::: code-group

```c [数组与指针的大小差异]
#include <stdio.h>

int main(void) {
  int a[3] = { 1, 2, 3 };
  int *p = a;

  printf("sizeof(a)=%zu\n", sizeof(a));
  printf("sizeof(p)=%zu\n", sizeof(p));
  return 0;
}
```

:::

你可以把数组理解为一块固定大小的内存区域，把指针理解为指向某块区域的引用。

## 5. 🆚 使用下标访问 `arr[i]` vs 指针算术 `*(p + i)` 有何区别？

在语义上，只要 `p` 指向同一个数组对象的首元素，二者等价。

| 特性   | `arr[i]`             | `*(p + i)`           |
| ------ | -------------------- | -------------------- |
| 可读性 | 更直观               | 更偏底层             |
| 约束   | 需要数组或可下标对象 | 需要指针指向有效元素 |
| 易错点 | 越界                 | 越界与类型不匹配     |

::: code-group

```c [二者等价]
#include <stdio.h>

int main(void) {
  int a[3] = { 10, 20, 30 };
  int *p = a;

  for (int i = 0; i < 3; i++) {
    printf("%d %d\n", a[i], *(p + i));
  }
  return 0;
}
```

:::

实战建议：优先用 `a[i]`，更清晰，也更不容易写错。

## 6. 🤔 为什么 `sizeof(arr)` 和 `sizeof(ptr)` 的结果不同？

原因是 `arr` 和 `ptr` 的类型不同。

- `arr` 是数组类型，例如 `int[3]`
- `ptr` 是指针类型，例如 `int*`

因此：

- `sizeof(arr)` 得到整个数组对象的字节数
- `sizeof(ptr)` 得到指针变量本身的字节数

::: code-group

```c [sizeof 差异]
#include <stdio.h>

int main(void) {
  int a[3] = { 1, 2, 3 };
  int *p = a;

  printf("sizeof(a)=%zu\n", sizeof(a));
  printf("sizeof(p)=%zu\n", sizeof(p));
  return 0;
}
```

:::

注意：如果数组作为函数参数传入，形参会退化为指针，此时 `sizeof` 得到的是指针大小。

## 7. 🤔 如何将数组传递给函数？形参应声明为数组还是指针？

在 C 语言里，函数形参写成 `T a[]` 和 `T *a` 是等价的。

关键点不是写法，而是必须把长度作为参数传入，或者在协议上保证边界。

::: code-group

```c [数组传参要带长度]
#include <stdio.h>

static int sum(const int *a, int n) {
  int s = 0;
  for (int i = 0; i < n; i++) {
    s += a[i];
  }
  return s;
}

int main(void) {
  int a[] = { 1, 2, 3 };
  int n = (int)(sizeof(a) / sizeof(a[0]));
  printf("%d\n", sum(a, n));
  return 0;
}
```

:::

如果函数只读数组，建议把参数写成 `const T*` 或 `const T a[]`。

## 8. 🤔 多维数组与指向数组的指针如何对应？

二维数组 `int a[2][3]` 的一行是 `int[3]`，因此它退化后是指向一行的指针，也就是 `int (*)[3]`。

::: code-group

```c [二维数组与指向数组的指针]
#include <stdio.h>

int main(void) {
  int a[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
  int (*p)[3] = a;

  printf("%d\n", p[1][2]);
  return 0;
}
```

:::

最常见的错误是把二维数组当成 `int**` 传递。

::: code-group

```c [❌ 二维数组不是 int**]
static void f(int **p) {
  (void)p;
}

int main(void) {
  int a[2][3] = { 0 };
  /* ❌ 类型不匹配 */
  /* f((int**)a); */
  return 0;
}
```

:::

要传二维数组，你需要把列数写进类型里，或者使用 C99 变长数组形参。

## 9. 🤔 什么是数组指针和指针数组？它们的语法和用途有何不同？

这两个概念很容易混淆，关键在于括号。

- 数组指针：指向数组的指针，例如 `int (*p)[3]`
- 指针数组：元素是指针的数组，例如 `int *p[3]`

::: code-group

```c [数组指针 vs 指针数组]
#include <stdio.h>

int main(void) {
  int a[3] = { 1, 2, 3 };
  int (*pa)[3] = &a;

  int x = 10;
  int y = 20;
  int z = 30;
  int *ap[3] = { &x, &y, &z };

  printf("%d\n", (*pa)[1]);
  printf("%d\n", *ap[2]);
  return 0;
}
```

:::

用途上：

- 数组指针常用于多维数组传参
- 指针数组常用于保存多个对象的地址，例如字符串表

## 10. 🤔 常见的指针与数组误用有哪些？如何避免？

常见误用与建议：

1. 把数组当指针看待，丢失长度
2. 用 `sizeof` 在函数内计算数组长度
3. 指针运算越界
4. 把二维数组当成 `T**`

::: code-group

```c [⚠️ 形参里的数组已经退化]
#include <stdio.h>

static void f(int a[]) {
  printf("sizeof(a)=%zu\n", sizeof(a));
}

int main(void) {
  int x[10] = { 0 };
  f(x);
  return 0;
}
```

:::

最稳的工程做法是把长度写进函数签名，并在调用点用 `sizeof` 计算长度后传入。

## 11. 🔗 引用

- [cppreference：Arrays][1]
- [cppreference：Pointers][2]
- [cppreference：Array to pointer decay][3]
- [cppreference：Pointer arithmetic][4]
- [cppreference：Multidimensional arrays][5]

[1]: https://en.cppreference.com/w/c/language/array
[2]: https://en.cppreference.com/w/c/language/pointer
[3]: https://en.cppreference.com/w/c/language/array#Array_to_pointer_decay
[4]: https://en.cppreference.com/w/c/language/operator_arithmetic#Pointer_arithmetic
[5]: https://en.cppreference.com/w/c/language/array#Multidimensional_arrays
