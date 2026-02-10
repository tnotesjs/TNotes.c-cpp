# [0023. 指针](https://github.com/tnotesjs/TNotes.c/tree/main/notes/0023.%20%E6%8C%87%E9%92%88)

<!-- region:toc -->

- [1. 🎯 本节内容](#1--本节内容)
- [2. 🫧 评价](#2--评价)
- [3. 🤔 什么是指针？它的本质是什么？](#3--什么是指针它的本质是什么)
- [4. 🤔 如何声明、初始化和使用指针变量？](#4--如何声明初始化和使用指针变量)
- [5. 🤔 指针与地址运算符 &、解引用运算符 `*` 有何关系？](#5--指针与地址运算符-解引用运算符--有何关系)
- [6. 🤔 什么是空指针（NULL）？如何安全使用它？](#6--什么是空指针null如何安全使用它)
- [7. 🆚 指针 vs 数组：它们在内存和语法上有何异同？](#7--指针-vs-数组它们在内存和语法上有何异同)
- [8. 🤔 指针可以进行哪些运算？加减整数的含义是什么？](#8--指针可以进行哪些运算加减整数的含义是什么)
- [9. 🤔 什么是指针的类型？为什么类型对指针很重要？](#9--什么是指针的类型为什么类型对指针很重要)
- [10. 🤔 常见的指针错误有哪些？如何避免野指针和悬空指针？](#10--常见的指针错误有哪些如何避免野指针和悬空指针)
- [11. 🔗 引用](#11--引用)

<!-- endregion:toc -->

## 1. 🎯 本节内容

- 指针的本质与使用场景
- 指针的声明、初始化与解引用
- 空指针与判空习惯
- 指针与数组的关系与差异
- 指针运算与边界
- 常见指针错误与规避方法

## 2. 🫧 评价

- 指针是 C 语言最强的能力之一，也是最容易制造未定义行为的入口，写对的关键在于边界与生命周期。
- 初学阶段别急着玩复杂技巧，先把三件事练熟：指针类型、指向对象是谁、可用范围到哪里。

## 3. 🤔 什么是指针？它的本质是什么？

指针是一个变量，它的值是某个对象在内存中的地址。

更准确地说，指针表达的是：

- 我指向哪一块内存
- 这块内存里存放的对象类型是什么

指针常见用途有：

- 在函数之间共享与修改数据
- 访问数组、字符串、结构体等数据
- 进行动态内存管理

::: code-group

```c [指针保存地址]
#include <stdio.h>

int main(void) {
  int x = 42;
  int *p = &x;
  printf("x=%d\n", x);
  printf("p=%p\n", (void*)p);
  return 0;
}
```

:::

注意：地址本身只是一个数值，指针类型决定了它应该如何被解读与解引用。

## 4. 🤔 如何声明、初始化和使用指针变量？

指针声明的形式是：`T *p`，表示 `p` 指向一个 `T` 类型对象。

初始化通常来自三类来源：

- 取变量地址 `&x`
- 指向数组首元素
- 动态内存分配返回值

::: code-group

```c [声明与初始化]
#include <stdio.h>

int main(void) {
  int x = 10;
  int *px = &x;

  int a[3] = { 1, 2, 3 };
  int *pa = a;

  printf("*px=%d\n", *px);
  printf("pa[1]=%d\n", pa[1]);
  return 0;
}
```

:::

未初始化的指针非常危险，它可能指向任意位置。

::: code-group

```c [❌ 未初始化指针]
int main(void) {
  int *p;
  /* ❌ 未定义行为：p 没有指向有效对象 */
  /* *p = 123; */
  return 0;
}
```

:::

更稳的习惯是：指针声明后立刻初始化，至少初始化为 `NULL`。

## 5. 🤔 指针与地址运算符 &、解引用运算符 `*` 有何关系？

`&` 取地址，`*` 解引用。

- `&x` 产生一个指向 `x` 的指针
- `*p` 访问 `p` 指向的对象

它们在语义上是互逆的，但要满足前提：指针必须指向一个有效对象。

::: code-group

```c [取地址与解引用]
#include <stdio.h>

int main(void) {
  int x = 7;
  int *p = &x;

  *p = 8;
  printf("x=%d\n", x);
  printf("&x=%p\n", (void*)&x);
  printf("p=%p\n", (void*)p);
  return 0;
}
```

:::

注意：`*` 在声明里表示指针类型，在表达式里表示解引用，这是同一个符号的两种语境。

## 6. 🤔 什么是空指针（NULL）？如何安全使用它？

空指针通常用 `NULL` 表示，意思是它不指向任何有效对象。

安全使用的核心是：

- 解引用前先判空
- 释放后把指针置空

::: code-group

```c [判空再使用]
#include <stdio.h>

static void print_int(const int *p) {
  if (p == NULL) {
    return;
  }
  printf("%d\n", *p);
}

int main(void) {
  int x = 3;
  print_int(&x);
  print_int(NULL);
  return 0;
}
```

:::

`NULL` 只能表示空，不代表非法指针的其他形态。很多野指针不是 `NULL`，因此判空不是万能的，生命周期管理同样重要。

## 7. 🆚 指针 vs 数组：它们在内存和语法上有何异同？

指针和数组关系很密切，但不是同一个概念。

| 特性 | 指针 | 数组 |
| --- | --- | --- |
| 本质 | 保存地址的变量 | 一段连续的元素序列 |
| 大小 | `sizeof(p)` 是指针大小 | `sizeof(a)` 是数组总字节数 |
| 可修改性 | 指针变量可以改指向 | 数组名不能被赋值 |
| 退化规则 | 不适用 | 多数表达式中数组名会退化为指向首元素的指针 |
| 典型用途 | 间接访问、动态结构 | 连续存储、固定容量 |

::: code-group

```c [数组名退化与 sizeof 差异]
#include <stdio.h>

int main(void) {
  int a[3] = { 1, 2, 3 };
  int *p = a;

  printf("sizeof(a)=%zu\n", sizeof(a));
  printf("sizeof(p)=%zu\n", sizeof(p));
  printf("a=%p\n", (void*)a);
  printf("p=%p\n", (void*)p);
  return 0;
}
```

:::

记住一句话：数组在表达式里经常表现得像指针，但它仍然是数组。

## 8. 🤔 指针可以进行哪些运算？加减整数的含义是什么？

常见的指针运算包括：

- 与 `NULL` 比较
- 同类型指针相减得到元素间距
- 指针加减整数用于移动若干个元素

指针加减整数的含义不是按字节移动，而是按元素大小移动。

::: code-group

```c [指针加一移动一个元素]
#include <stdio.h>

int main(void) {
  int a[3] = { 10, 20, 30 };
  int *p = &a[0];

  printf("p=%p *p=%d\n", (void*)p, *p);
  p = p + 1;
  printf("p=%p *p=%d\n", (void*)p, *p);
  return 0;
}
```

:::

⚠️ 指针运算只能在同一个数组对象的范围内进行，越界哪怕只移动到尾后位置之外都属于未定义行为。

## 9. 🤔 什么是指针的类型？为什么类型对指针很重要？

指针类型决定了两件事：

- 解引用时读写多少字节
- 指针加减时跨过多少字节

例如 `char*` 每次加一通常移动 1 字节，`int*` 每次加一通常移动 4 字节。

::: code-group

```c [同一块内存用不同类型解读会有风险]
#include <stdio.h>

int main(void) {
  int x = 0x11223344;
  unsigned char *pc = (unsigned char*)&x;
  printf("byte0=%u\n", (unsigned)pc[0]);
  return 0;
}
```

:::

当你需要表达通用指针时可以用 `void*`，但 `void*` 不能直接解引用，必须先转换成具体类型。

## 10. 🤔 常见的指针错误有哪些？如何避免野指针和悬空指针？

常见错误与对应习惯：

1. 未初始化指针
2. 解引用空指针
3. 悬空指针，也就是对象已失效但指针还在用
4. 越界指针运算
5. 类型不匹配导致错误解读

悬空指针最常见来源是返回局部变量地址。

::: code-group

```c [❌ 返回局部变量地址]
static int* f(void) {
  int x = 1;
  return &x;
}
```

```c [返回值通过出参写回]
static void f(int *out) {
  if (out == NULL) {
    return;
  }
  *out = 1;
}
```

:::

另一个常见来源是 `free` 后继续使用。

::: code-group

```c [⚠️ 释放后置空]
#include <stdlib.h>

int main(void) {
  int *p = (int*)malloc(sizeof(int));
  if (p == NULL) {
    return 1;
  }

  *p = 1;
  free(p);
  p = NULL;
  return 0;
}
```

:::

最后，凡是指针参与的接口，尽量把这些信息写清楚：

- 指针是否允许为 `NULL`
- 指针指向的对象个数与边界
- 指针指向对象的生命周期由谁管理

## 11. 🔗 引用

- [cppreference：Pointers][1]
- [cppreference：Array to pointer decay][2]
- [cppreference：Pointer arithmetic][3]
- [cppreference：NULL][4]

[1]: https://en.cppreference.com/w/c/language/pointer
[2]: https://en.cppreference.com/w/c/language/array#Array_to_pointer_decay
[3]: https://en.cppreference.com/w/c/language/operator_arithmetic#Pointer_arithmetic
[4]: https://en.cppreference.com/w/c/types/NULL
