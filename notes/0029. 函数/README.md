# [0029. 函数](https://github.com/tnotesjs/TNotes.c/tree/main/notes/0029.%20%E5%87%BD%E6%95%B0)

<!-- region:toc -->

- [1. 🎯 本节内容](#1--本节内容)
- [2. 🫧 评价](#2--评价)
- [3. 🤔 什么是函数？它的基本组成部分有哪些？](#3--什么是函数它的基本组成部分有哪些)
- [4. 🤔 如何定义和调用一个函数？](#4--如何定义和调用一个函数)
- [5. 🤔 什么是函数声明（原型）？为什么需要它？](#5--什么是函数声明原型为什么需要它)
- [6. 🤔 函数参数是如何传递的？C 语言支持哪些传参方式？](#6--函数参数是如何传递的c-语言支持哪些传参方式)
- [7. 🆚 按值传递 vs 按指针传递：语义和效果有何不同？](#7--按值传递-vs-按指针传递语义和效果有何不同)
- [8. 🤔 函数可以返回哪些类型的值？能否返回数组或结构体？](#8--函数可以返回哪些类型的值能否返回数组或结构体)
- [9. 🤔 什么是递归函数？使用时需要注意什么？](#9--什么是递归函数使用时需要注意什么)
- [10. 🤔 main 函数的特殊性是什么？它的返回值有何意义？](#10--main-函数的特殊性是什么它的返回值有何意义)
- [11. 🔗 引用](#11--引用)

<!-- endregion:toc -->

## 1. 🎯 本节内容

- 函数的组成与声明
- 函数的定义与调用
- 形参与实参传递规则
- 按值传递与按指针传递对比
- 返回值类型与返回结构体
- 递归函数的基本写法与风险
- `main` 函数的约定与返回码

## 2. 🫧 评价

- 函数是 C 语言组织代码的最小单元，写得好能显著降低重复与耦合。
- 绝大多数函数相关的 Bug 并非语法问题，而是参数语义不清与边界处理缺失。

## 3. 🤔 什么是函数？它的基本组成部分有哪些？

函数是一段可复用的代码，通过名字对外提供一组输入与一个输出。

在 C 语言中，一个函数通常由这些部分组成。

- 返回类型
- 函数名
- 形参列表
- 函数体
- `return` 语句

::: code-group

```c [函数的基本形态]
int add(int a, int b) {
  return a + b;
}
```

:::

你可以把函数看作一个接口。

- 形参列表描述输入
- 返回类型描述输出
- 函数体描述实现细节

## 4. 🤔 如何定义和调用一个函数？

最常见的组织方式是先写函数声明，再写函数定义，然后在 `main` 或其他函数里调用。

::: code-group

```c [定义与调用]
#include <stdio.h>

static int add(int a, int b);

int main(void) {
  int r = add(1, 2);
  printf("%d\n", r);
  return 0;
}

static int add(int a, int b) {
  return a + b;
}
```

:::

⚠️ 如果你在调用之前没有可见的声明，编译器将无法正确进行类型检查。

## 5. 🤔 什么是函数声明（原型）？为什么需要它？

函数声明也叫函数原型，用来告诉编译器。

- 这个函数叫什么
- 需要哪些参数
- 返回什么类型

函数声明的典型位置是头文件或源文件顶部。

为什么需要函数声明。

1. 允许你先调用再定义
2. 让编译器对实参与形参做类型检查
3. 支持跨源文件调用

::: code-group

```c [函数声明与定义]
int add(int a, int b);

int add(int a, int b) {
  return a + b;
}
```

:::

建议把对外暴露的声明放在 `.h` 文件中，把实现放在 `.c` 文件中。

## 6. 🤔 函数参数是如何传递的？C 语言支持哪些传参方式？

在 C 语言里，函数参数只有一种语义。

- 按值传递

也就是说，调用时会把实参的值拷贝给形参。

当你把指针传给函数时，本质仍然是按值传递，只不过被拷贝的值是一个地址。

因此。

- 你可以通过指针修改指针指向的数据
- 你无法通过指针形参修改调用者的指针变量本身

::: code-group

```c [指针形参仍然是按值传递]
#include <stdio.h>

static void set_first(int *p) {
  if (p == NULL) {
    return;
  }
  *p = 42;
}

int main(void) {
  int x = 0;
  set_first(&x);
  printf("%d\n", x);
  return 0;
}
```

:::

如果你要修改调用者的指针变量，需要使用二重指针，这是上一节刚讲过的重点。

## 7. 🆚 按值传递 vs 按指针传递：语义和效果有何不同？

| 特性 | 按值传递 | 按指针传递 |
| --- | --- | --- |
| 传入的数据 | 数据副本 | 地址副本 |
| 函数内能否修改调用者变量 | 不行 | 可以修改指向的数据 |
| 常见用途 | 小对象，纯计算 | 修改对象，输出结果，避免大对象拷贝 |
| 风险 | 语义不清导致误用 | `NULL` 检查与生命周期风险 |

下面用交换函数演示两者差异。

::: code-group

```c [❌ 按值传递无法交换调用者变量]
static void swap_wrong(int a, int b) {
  int t = a;
  a = b;
  b = t;
}
```

```c [用指针交换]
static void swap_int(int *a, int *b) {
  if (a == NULL || b == NULL) {
    return;
  }
  int t = *a;
  *a = *b;
  *b = t;
}
```

:::

⚠️ 指针版要先判空，另外也要注意不要传入无效地址。

## 8. 🤔 函数可以返回哪些类型的值？能否返回数组或结构体？

函数返回值可以是。

- 基本类型，例如 `int`、`double`
- 指针类型，例如 `char*`
- 结构体类型，例如 `struct Point`

结构体可以按值返回。

::: code-group

```c [返回结构体]
struct Point {
  int x;
  int y;
};

static struct Point make_point(int x, int y) {
  struct Point p = { x, y };
  return p;
}
```

:::

数组不能按值返回。

如果你需要返回一段连续元素，常见做法有三种。

1. 返回动态分配的内存指针，并约定由调用者 `free`
2. 由调用者传入输出缓冲区指针与长度
3. 返回指向静态存储期数组的指针，但要谨慎对待线程安全与可重入性

::: code-group

```c [返回动态数组指针]
#include <stdlib.h>

static int *make_ints(size_t n) {
  int *a = malloc(n * sizeof *a);
  if (a == NULL) {
    return NULL;
  }
  return a;
}
```

:::

⚠️ 禁止返回局部数组或局部变量的地址，否则会产生悬空指针。

## 9. 🤔 什么是递归函数？使用时需要注意什么？

递归函数是指函数直接或间接调用自身。

写递归时必须明确。

- 递归终止条件
- 每一步如何缩小问题规模

否则就会无限递归，最终栈溢出。

::: code-group

```c [递归求阶乘]
static unsigned long long fact(unsigned int n) {
  if (n <= 1) {
    return 1;
  }
  return (unsigned long long)n * fact(n - 1);
}
```

:::

⚠️ 递归的常见风险。

- 深度过大导致栈溢出
- 重复计算导致性能差，例如朴素斐波那契
- 结果溢出，需要提前定义输入上限

当递归深度可能很大时，通常建议改写为循环或显式栈。

## 10. 🤔 main 函数的特殊性是什么？它的返回值有何意义？

`main` 是程序入口函数，通常有两种常见签名。

::: code-group

```c [main 的常见签名]
int main(void);
int main(int argc, char **argv);
```

:::

返回值表示进程退出状态。

- 返回 0 通常表示成功
- 非 0 通常表示失败或异常

更推荐使用 `EXIT_SUCCESS` 与 `EXIT_FAILURE`，语义更明确。

::: code-group

```c [返回退出码]
#include <stdlib.h>

int main(void) {
  return EXIT_SUCCESS;
}
```

:::

如果使用带参数的 `main`。

- `argc` 是参数个数
- `argv` 是指针数组，保存每个参数字符串

## 11. 🔗 引用

- [cppreference：Function declaration][1]
- [cppreference：Functions][2]
- [cppreference：main function][3]
- [cppreference：EXIT_SUCCESS，EXIT_FAILURE][4]
- [cppreference：Recursion][5]

[1]: https://en.cppreference.com/w/c/language/function_declaration
[2]: https://en.cppreference.com/w/c/language/function
[3]: https://en.cppreference.com/w/c/language/main_function
[4]: https://en.cppreference.com/w/c/program/EXIT_status
[5]: https://en.cppreference.com/w/c/language/recursion
