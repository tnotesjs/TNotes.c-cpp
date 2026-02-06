# [0020. 数组与结构](https://github.com/tnotesjs/TNotes.c-cpp/tree/main/notes/0020.%20%E6%95%B0%E7%BB%84%E4%B8%8E%E7%BB%93%E6%9E%84)

<!-- region:toc -->

- [1. 🎯 本节内容](#1--本节内容)
- [2. 🫧 评价](#2--评价)
- [3. 🤔 什么是数组？它的内存布局有何特点？](#3--什么是数组它的内存布局有何特点)
- [4. 🤔 如何定义和初始化一维及多维数组？](#4--如何定义和初始化一维及多维数组)
- [5. 🤔 数组名与指针有何关系？何时等价，何时不同？](#5--数组名与指针有何关系何时等价何时不同)
- [6. 🤔 什么是结构体（struct）？它解决了什么问题？](#6--什么是结构体struct它解决了什么问题)
- [7. 🤔 如何定义、初始化和访问结构体成员？](#7--如何定义初始化和访问结构体成员)
- [8. 🆚 数组 vs 结构体：在数据组织上有何本质区别？](#8--数组-vs-结构体在数据组织上有何本质区别)
- [9. 🤔 结构体可以包含数组吗？数组可以包含结构体吗？](#9--结构体可以包含数组吗数组可以包含结构体吗)
  - [9.1. 结构体包含数组](#91-结构体包含数组)
  - [9.2. 数组包含结构体](#92-数组包含结构体)
- [10. 🤔 什么是结构体的内存对齐？为什么会影响 sizeof 结果？](#10--什么是结构体的内存对齐为什么会影响-sizeof-结果)
- [11. 🔗 引用](#11--引用)

<!-- endregion:toc -->

## 1. 🎯 本节内容

- 数组内存布局
- 数组定义与初始化
- 数组名与指针的关系
- 结构体定义与成员访问
- 数组与结构体的组合使用
- 结构体内存对齐与填充

## 2. 🫧 评价

- 数组与结构体是 C 语言里最常用的两种数据组织方式，真正的难点在于边界、传参和内存布局。
- 你如果能清楚说出数组退化规则、结构体对齐规则，再配合 `sizeof`、`offsetof` 做验证，很多诡异 bug 会自动消失。
- 写接口时，优先把长度和所有权说清楚，别把“看起来像数组”的指针当数组用。

## 3. 🤔 什么是数组？它的内存布局有何特点？

数组是一段连续的同类型元素序列。

它的内存布局特点是：

- 连续：`a[i]` 与 `a[i + 1]` 在内存中相邻。
- 定长：数组一旦定义，元素个数固定。
- 同质：所有元素类型相同，大小相同。

连续内存带来的直接收益是：

- 下标访问是 $O(1)$。
- 遍历具有良好的缓存局部性。

::: code-group

```c [数组元素地址相邻]
#include <stdio.h>

int main(void) {
  int a[3] = { 10, 20, 30 };
  printf("&a[0]=%p\n", (void*)&a[0]);
  printf("&a[1]=%p\n", (void*)&a[1]);
  printf("&a[2]=%p\n", (void*)&a[2]);
  return 0;
}
```

:::

注意：数组越界访问是未定义行为，不会自动报错，很多时候还会“看似正常”，但会埋下难以定位的问题。

## 4. 🤔 如何定义和初始化一维及多维数组？

一维数组的定义与初始化：

::: code-group

```c [一维数组]
int a[3];
int b[3] = { 1, 2, 3 };
int c[] = { 1, 2, 3, 4 };  /* 自动推导长度 */
int d[5] = { 1, 2 };       /* 其余元素补 0 */
```

:::

多维数组本质是“数组的数组”。例如 `int m[2][3]` 表示 2 行 3 列。

::: code-group

```c [二维数组初始化]
int m1[2][3] = {
  { 1, 2, 3 },
  { 4, 5, 6 },
};

int m2[2][3] = { 1, 2, 3, 4, 5, 6 };
```

:::

多维数组在函数参数中最容易写错的是第二维及后续维度必须明确。

::: code-group

```c [✅ 传二维数组需要明确列数]
#include <stdio.h>

static int sum2x3(const int a[2][3]) {
  int sum = 0;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      sum += a[i][j];
    }
  }
  return sum;
}

int main(void) {
  int m[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
  printf("%d\n", sum2x3(m));
  return 0;
}
```

:::

## 5. 🤔 数组名与指针有何关系？何时等价，何时不同？

数组名在大多数表达式中会退化为指向首元素的指针。

::: code-group

```c [数组名退化为指针]
int a[3] = { 1, 2, 3 };
int *p = a;          /* 等价于 &a[0] */
int x = a[1];        /* 等价于 *(a + 1) */
```

:::

但它们并不完全等价，至少有两个关键例外：

1. `sizeof(a)` 是数组总字节数，而 `sizeof(p)` 是指针大小。
2. `&a` 的类型是“指向整个数组的指针”，不是“指向首元素的指针”。

::: code-group

```c [⚠️ 数组不是指针]
#include <stdio.h>

int main(void) {
  int a[3] = { 1, 2, 3 };
  int *p = a;

  printf("sizeof(a)=%zu\n", sizeof(a));
  printf("sizeof(p)=%zu\n", sizeof(p));
  printf("a=%p\n", (void*)a);
  printf("&a=%p\n", (void*)&a);
  return 0;
}
```

:::

函数形参如果写成 `int a[]`，它本质上仍然是 `int *a`。

::: code-group

```c [⚠️ 形参的数组会退化为指针]
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

## 6. 🤔 什么是结构体（struct）？它解决了什么问题？

结构体用于把多个不同类型的字段组合成一个整体，表达“一个对象由多种属性组成”。

它解决的问题是：当你需要把多个字段当作一个整体来传递、返回、存储、排序时，结构体能让你的数据结构更贴近业务语义。

::: code-group

```c [结构体表达一个实体]
struct Point {
  int x;
  int y;
};
```

:::

对比数组：数组更像是“同类型的集合”，结构体更像是“异类型字段的组合”。

## 7. 🤔 如何定义、初始化和访问结构体成员？

定义与初始化：

::: code-group

```c [定义与初始化]
#include <stdio.h>

struct User {
  int id;
  char name[16];
};

int main(void) {
  struct User u1 = { 1001, "alice" };
  struct User u2 = { .id = 1002, .name = "bob" };

  printf("u1.id=%d u1.name=%s\n", u1.id, u1.name);
  printf("u2.id=%d u2.name=%s\n", u2.id, u2.name);
  return 0;
}
```

:::

成员访问规则：

- 结构体对象用 `.` 访问成员，例如 `u.id`。
- 结构体指针用 `->` 访问成员，例如 `pu->id`。

::: code-group

```c [点运算符与箭头运算符]
#include <stdio.h>

struct Point {
  int x;
  int y;
};

int main(void) {
  struct Point p = { 1, 2 };
  struct Point *pp = &p;

  printf("%d\n", p.x);
  printf("%d\n", pp->x);
  return 0;
}
```

:::

## 8. 🆚 数组 vs 结构体：在数据组织上有何本质区别？

它们的核心差异是“同质序列”与“异质组合”。

| 特性     | 数组             | 结构体                 |
| -------- | ---------------- | ---------------------- |
| 元素类型 | 必须同类型       | 可以不同类型           |
| 访问方式 | 下标 `a[i]`      | 字段 `s.field`         |
| 典型用途 | 列表、表、缓冲区 | 一个对象、多字段记录   |
| 内存布局 | 连续且等大小步进 | 受对齐影响，可能有填充 |
| 组合关系 | 常用于批量数据   | 常用于数据建模         |

实战建议：

- 需要存储大量同类型数据，优先数组。
- 需要表达一个实体的多个属性，优先结构体。
- 需要“多条记录”，常见模型是“结构体数组”。

## 9. 🤔 结构体可以包含数组吗？数组可以包含结构体吗？

两种组合都可以，而且非常常见。

### 9.1. 结构体包含数组

例如固定长度的名字、字节缓冲区、坐标序列。

::: code-group

```c [结构体中包含数组]
#include <stdio.h>

struct Packet {
  unsigned char data[8];
  unsigned int len;
};

int main(void) {
  struct Packet p = { .data = { 1, 2, 3 }, .len = 3 };
  printf("len=%u data0=%u\n", p.len, (unsigned)p.data[0]);
  return 0;
}
```

:::

### 9.2. 数组包含结构体

例如学生列表、点集、订单记录表。

::: code-group

```c [结构体数组]
#include <stdio.h>

struct Point {
  int x;
  int y;
};

int main(void) {
  struct Point pts[3] = {
    { 0, 0 },
    { 1, 2 },
    { 3, 4 },
  };

  for (int i = 0; i < 3; i++) {
    printf("(%d,%d)\n", pts[i].x, pts[i].y);
  }
  return 0;
}
```

:::

## 10. 🤔 什么是结构体的内存对齐？为什么会影响 sizeof 结果？

结构体的内存对齐指的是：编译器为了让某些成员按特定字节边界存放，会在成员之间或结构体末尾插入填充字节。

这会导致：

- `sizeof(struct)` 可能大于所有成员 `sizeof` 之和。
- 成员的偏移量可能不是紧挨着递增。

::: code-group

```c [对齐与填充示例]
#include <stddef.h>
#include <stdio.h>

struct A {
  char c;
  int x;
};

int main(void) {
  printf("sizeof(struct A)=%zu\n", sizeof(struct A));
  printf("offsetof(c)=%zu\n", offsetof(struct A, c));
  printf("offsetof(x)=%zu\n", offsetof(struct A, x));
  return 0;
}
```

:::

为什么会这样：

- `int` 往往需要按 4 字节对齐。
- 当 `char` 放在前面时，`int` 可能需要从下一个 4 字节边界开始，于是中间会出现填充。

注意：对齐规则与平台、编译器、编译选项有关，不要写死具体数值，应该用 `sizeof` 与 `offsetof` 去验证。

## 11. 🔗 引用

- [cppreference：Arrays][1]
- [cppreference：struct 与成员访问][2]
- [cppreference：sizeof 运算符][3]
- [cppreference：offsetof 宏][4]

[1]: https://en.cppreference.com/w/c/language/array
[2]: https://en.cppreference.com/w/c/language/struct
[3]: https://en.cppreference.com/w/c/language/sizeof
[4]: https://en.cppreference.com/w/c/types/offsetof
