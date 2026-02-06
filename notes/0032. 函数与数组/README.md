# [0032. 函数与数组](https://github.com/tnotesjs/TNotes.c-cpp/tree/main/notes/0032.%20%E5%87%BD%E6%95%B0%E4%B8%8E%E6%95%B0%E7%BB%84)

<!-- region:toc -->

- [1. 🎯 本节内容](#1--本节内容)
- [2. 🫧 评价](#2--评价)
- [3. 🤔 数组作为函数参数时会发生什么？](#3--数组作为函数参数时会发生什么)
- [4. 🤔 为什么函数无法直接获取传入数组的长度？](#4--为什么函数无法直接获取传入数组的长度)
- [5. 🆚 传递一维数组 vs 传递多维数组：语法和语义有何不同？](#5--传递一维数组-vs-传递多维数组语法和语义有何不同)
- [6. 🤔 如何在函数中安全地处理数组参数？](#6--如何在函数中安全地处理数组参数)
- [7. 🤔 为什么不能通过 sizeof(arr) 在函数内获取数组大小？](#7--为什么不能通过-sizeofarr-在函数内获取数组大小)
- [8. 🤔 如何设计函数接口以支持不同长度的数组？](#8--如何设计函数接口以支持不同长度的数组)
- [9. 🤔 结构体封装数组是否能解决长度丢失问题？](#9--结构体封装数组是否能解决长度丢失问题)
- [10. 🤔 常见的数组传参错误有哪些？如何避免？](#10--常见的数组传参错误有哪些如何避免)
- [11. 🔗 引用](#11--引用)

<!-- endregion:toc -->

## 1. 🎯 本节内容

- 数组形参退化为指针
- 数组长度参数的必要性
- 一维数组与多维数组传参差异
- `sizeof` 在形参与实参上的差别
- 指针加长度的安全接口模式
- 结构体封装数组的接口设计
- 常见数组传参错误与规避

## 2. 🫧 评价

- 数组传参的坑几乎都来自退化规则与长度丢失，只要你坚持指针加长度这条规则，就能避开大多数问题。
- 多维数组传参的语法看起来怪，但它本质是在把布局信息交给编译器做类型检查，别用强转偷懒。

## 3. 🤔 数组作为函数参数时会发生什么？

在 C 语言中，数组作为函数参数时，形参会退化为指针。

也就是说。

- 你在函数形参里写 `int a[]`
- 实际上等价于 `int *a`

这会带来两个直接结果。

1. 函数内部拿到的是地址，不再是数组类型
2. 数组长度信息不会自动传入函数

::: code-group

```c [形参里的 a 是指针]
#include <stdio.h>

static void f(int a[]) {
  printf("%zu\n", sizeof(a));
}

int main(void) {
  int x[10] = { 0 };
  printf("%zu\n", sizeof(x));
  f(x);
  return 0;
}
```

:::

上例里，`sizeof(x)` 是整个数组大小，`sizeof(a)` 是指针大小。

## 4. 🤔 为什么函数无法直接获取传入数组的长度？

原因不是函数做不到，而是类型信息在参数位置已经丢失。

当你把数组传给函数时，实际上传入的是首元素地址。

函数内部只能看到一个指针。

- 指针只记录地址
- 不记录元素个数

因此，数组长度必须由调用者显式提供，或者由协议间接提供。

常见策略。

1. 指针加长度参数
2. 使用哨兵值结尾，例如以 `\0` 结尾的字符串
3. 结构体封装指针与长度
4. 对固定长度数组，使用指向数组的指针把长度写进类型

## 5. 🆚 传递一维数组 vs 传递多维数组：语法和语义有何不同？

一维数组形参退化为 `T*`，多维数组传参必须保留除第一维以外的维度信息。

| 特性             | 一维数组              | 多维数组                      |
| ---------------- | --------------------- | ----------------------------- |
| 形参写法         | `int a[]` 或 `int *a` | `int a[][N]` 或 `int (*a)[N]` |
| 是否需要列数信息 | 不需要写进类型        | 必须写进类型或使用变长数组    |
| 常见用途         | 各类线性数组          | 矩阵，表格数据                |
| 常见坑           | 长度丢失              | 把二维数组当成 `int**`        |

::: code-group

```c [一维数组加长度]
static int sum_ints(const int *a, size_t n) {
  if (a == NULL) {
    return 0;
  }
  int s = 0;
  for (size_t i = 0; i < n; i++) {
    s += a[i];
  }
  return s;
}
```

```c [二维数组传参，列数写进类型]
static int sum2x3(const int a[][3]) {
  int s = 0;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      s += a[i][j];
    }
  }
  return s;
}
```

```c [C99 变长数组，运行时列数]
#include <stddef.h>

static int sum_matrix(size_t rows, size_t cols, const int a[rows][cols]) {
  int s = 0;
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      s += a[i][j];
    }
  }
  return s;
}
```

:::

⚠️ 二维数组是连续内存布局，不是 `int**`。

## 6. 🤔 如何在函数中安全地处理数组参数？

安全处理数组参数的关键是把边界写清楚，并把检查放在函数入口。

建议固定一个接口模式。

1. `const T *data` 或 `T *data`
2. `size_t len`

并遵循这些规则。

- `data` 允许为空时，`len` 必须为 0
- `len` 参与循环边界，禁止硬编码常量
- 不对外暴露内部缓冲区地址的临时对象

::: code-group

```c [✅ 指针加长度的安全遍历]
#include <stddef.h>

static int find_first(const int *a, size_t n, int target, size_t *idx) {
  if (idx == NULL) {
    return 0;
  }
  if (a == NULL) {
    return n == 0;
  }

  for (size_t i = 0; i < n; i++) {
    if (a[i] == target) {
      *idx = i;
      return 1;
    }
  }
  return 0;
}
```

:::

⚠️ 如果数组来自动态内存，还需要明确谁负责 `free`。

## 7. 🤔 为什么不能通过 sizeof(arr) 在函数内获取数组大小？

因为在函数形参里，`arr` 不是数组类型，而是指针。

`sizeof(arr)` 计算的是指针大小，而不是数组大小。

::: code-group

```c [sizeof 在实参和形参的差异]
#include <stdio.h>

static void g(int arr[]) {
  printf("%zu\n", sizeof(arr));
}

int main(void) {
  int a[10] = { 0 };
  printf("%zu\n", sizeof(a));
  g(a);
  return 0;
}
```

:::

你唯一能在编译期得到数组元素个数的地方，通常是数组还没有退化的位置。

例如。

- 在同一个作用域里对数组对象使用 `sizeof`
- 使用宏或泛型辅助把长度算出来再传入

## 8. 🤔 如何设计函数接口以支持不同长度的数组？

最通用的设计就是。

- 指针加长度

如果你希望接口更不容易被误用，可以再加入。

- 输出参数与返回状态码
- 长度的单位约定，例如元素个数而不是字节数

::: code-group

```c [通用接口：指针加长度]
#include <stddef.h>

static int sum_ints2(const int *a, size_t n, int *out) {
  if (out == NULL) {
    return 0;
  }
  if (a == NULL) {
    return n == 0;
  }
  int s = 0;
  for (size_t i = 0; i < n; i++) {
    s += a[i];
  }
  *out = s;
  return 1;
}
```

:::

如果是字符串类数组，推荐明确约束。

- 以 `\0` 结束
- 或者显式传长度

不要依赖隐含约定。

## 9. 🤔 结构体封装数组是否能解决长度丢失问题？

可以。

把数据指针与长度放进同一个结构体，本质是把协议打包，减少参数散落。

这种设计常用于库接口与模块边界。

::: code-group

```c [结构体封装指针与长度]
#include <stddef.h>

struct IntSpan {
  const int *data;
  size_t len;
};

static int sum_span(struct IntSpan s, int *out) {
  if (out == NULL) {
    return 0;
  }
  if (s.data == NULL) {
    return s.len == 0;
  }
  int v = 0;
  for (size_t i = 0; i < s.len; i++) {
    v += s.data[i];
  }
  *out = v;
  return 1;
}
```

:::

⚠️ 结构体封装并不会自动变安全。

- 仍然要处理 `NULL` 与边界
- 仍然要明确所有权与生命周期

## 10. 🤔 常见的数组传参错误有哪些？如何避免？

常见错误包括。

1. 只传指针不传长度
2. 在函数里对形参用 `sizeof` 误判长度
3. 把二维数组强转成 `T**`
4. 忘记 `const`，导致无意修改输入
5. 传入的长度单位混乱，把元素个数当成字节数

::: code-group

```c [❌ 把二维数组当成 int**]
static void bad(int **pp) {
  (void)pp;
}

int main(void) {
  int a[2][3] = { 0 };
  /* ❌ 类型不匹配 */
  /* bad((int**)a); */
  return 0;
}
```

```c [✅ 指针加长度]
#include <stddef.h>

static void fill(int *a, size_t n, int v) {
  if (a == NULL) {
    return;
  }
  for (size_t i = 0; i < n; i++) {
    a[i] = v;
  }
}
```

:::

经验上，只要你坚持。

- 一维数组永远配长度
- 多维数组传参永远保留列数信息

就能规避绝大多数坑。

## 11. 🔗 引用

- [cppreference：Array to pointer decay][1]
- [cppreference：Arrays][2]
- [cppreference：sizeof operator][3]
- [cppreference：Variable Length Array][4]
- [cppreference：Pointers][5]

[1]: https://en.cppreference.com/w/c/language/array#Array_to_pointer_decay
[2]: https://en.cppreference.com/w/c/language/array
[3]: https://en.cppreference.com/w/c/language/sizeof
[4]: https://en.cppreference.com/w/c/language/array#Variable-length_arrays
[5]: https://en.cppreference.com/w/c/language/pointer
