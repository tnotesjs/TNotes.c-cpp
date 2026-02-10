# [0020. 数组与结构](https://github.com/tnotesjs/TNotes.c/tree/main/notes/0020.%20%E6%95%B0%E7%BB%84%E4%B8%8E%E7%BB%93%E6%9E%84)

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
- [10. 🤔 结构体可以嵌套吗？如何访问嵌套成员？](#10--结构体可以嵌套吗如何访问嵌套成员)
- [11. 🆚 一维数组模拟二维数组 vs 真二维数组有何区别？](#11--一维数组模拟二维数组-vs-真二维数组有何区别)
- [12. 🤔 如何将数组或结构体作为函数参数传递？](#12--如何将数组或结构体作为函数参数传递)
- [13. 🤔 为什么不能直接用 == 比较两个数组或结构体？](#13--为什么不能直接用--比较两个数组或结构体)
- [14. 🤔 如何实现数组或结构体的复制与初始化？](#14--如何实现数组或结构体的复制与初始化)
- [15. 🤔 什么是结构体的内存对齐？为什么会影响 sizeof 结果？](#15--什么是结构体的内存对齐为什么会影响-sizeof-结果)
- [16. 🔗 引用](#16--引用)

<!-- endregion:toc -->

## 1. 🎯 本节内容

- 数组内存布局
- 数组定义与初始化
- 多维数组与变长数组
- 数组名与指针的关系
- 结构体定义与成员访问
- 数组与结构体的组合使用
- 结构体嵌套与成员访问
- 一维数组模拟二维数组
- 数组与结构体的函数传参
- 数组与结构体的比较与复制
- 结构体内存对齐与填充

## 2. 🫧 评价

- 数组与结构体是 C 语言里最常用的两种数据组织方式，真正的难点在于边界、传参和内存布局。
- 你如果能清楚说出数组退化规则、结构体对齐规则，再配合 `sizeof`、`offsetof` 做验证，很多诡异 bug 会自动消失。
- 写接口时，优先把长度和所有权说清楚，别把"看起来像数组"的指针当数组用。
- 多维数组和结构体数组看似简单，最常踩坑的是类型和传参形式，尤其是二维数组的列数与数组退化规则。

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

注意：数组越界访问是未定义行为，不会自动报错，很多时候还会"看似正常"，但会埋下难以定位的问题。

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

多维数组本质是"数组的数组"。例如 `int m[2][3]` 表示 2 行 3 列，它在内存中是按行连续存放的。

::: code-group

```c [二维数组定义与遍历]
#include <stdio.h>

int main(void) {
  int a[2][3] = {
    { 1, 2, 3 },
    { 4, 5, 6 },
  };

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      printf("a[%d][%d]=%d\n", i, j, a[i][j]);
    }
  }

  return 0;
}
```

:::

多维数组常见的初始化方式有：

- 全量花括号初始化
- 部分初始化，剩余元素自动补 0
- 线性初始化，按行从左到右填充

::: code-group

```c [多种初始化方式]
int a[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
int b[2][3] = { { 1 }, { 4 } };
int c[2][3] = { 1, 2, 3, 4, 5, 6 };
```

:::

如果你需要运行期决定行数或列数，可以考虑 C99 的变长数组，也可以用动态内存配合一维数组模拟二维数组。

::: code-group

```c [⚠️ 变长数组需要 C99]
#include <stdio.h>

static int sum_matrix(int rows, int cols, const int a[rows][cols]) {
  int sum = 0;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      sum += a[i][j];
    }
  }
  return sum;
}

int main(void) {
  int rows = 2;
  int cols = 3;
  int m[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
  printf("%d\n", sum_matrix(rows, cols, m));
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
2. `&a` 的类型是"指向整个数组的指针"，不是"指向首元素的指针"。

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

结构体用于把多个不同类型的字段组合成一个整体，表达"一个对象由多种属性组成"。

它解决的问题是：当你需要把多个字段当作一个整体来传递、返回、存储、排序时，结构体能让你的数据结构更贴近业务语义。

::: code-group

```c [结构体表达一个实体]
struct Point {
  int x;
  int y;
};
```

:::

对比数组：数组更像是"同类型的集合"，结构体更像是"异类型字段的组合"。

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

它们的核心差异是"同质序列"与"异质组合"。

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
- 需要"多条记录"，常见模型是"结构体数组"。

## 9. 🤔 结构体可以包含数组吗？数组可以包含结构体吗？

两种组合都可以，而且非常常见。

### 9.1. 结构体包含数组

例如固定长度的名字、字节缓冲区、坐标序列。

关键注意点有三个：

1. 数组成员不能被直接赋值
2. 数组成员常常需要配套一个长度字段
3. 字符数组用于字符串时，必须保证以 `\0` 结尾

::: code-group

```c [⚠️ 数组成员不能直接赋值]
struct User {
  int id;
  char name[16];
};

int main(void) {
  struct User u = { 1, "alice" };

  /* ❌ 错误：数组不能被赋值 */
  /* u.name = "bob"; */

  return 0;
}
```

```c [✅ 用 snprintf 写入字符串更稳]
#include <stdio.h>

struct User {
  int id;
  char name[16];
};

int main(void) {
  struct User u = { 1, "alice" };
  snprintf(u.name, sizeof(u.name), "%s", "bob");
  printf("%s\n", u.name);
  return 0;
}
```

:::

如果数组成员表达的是字节缓冲区，务必把长度当成协议的一部分，避免隐式约定。

### 9.2. 数组包含结构体

结构体数组适合表达多条记录的集合，例如学生列表、点集、订单列表。

::: code-group

```c [结构体数组遍历]
#include <stdio.h>

struct Student {
  int id;
  int score;
};

int main(void) {
  struct Student students[] = {
    { 1001, 95 },
    { 1002, 88 },
    { 1003, 76 },
  };

  int n = (int)(sizeof(students) / sizeof(students[0]));
  for (int i = 0; i < n; i++) {
    printf("id=%d score=%d\n", students[i].id, students[i].score);
  }

  return 0;
}
```

:::

实战中更推荐把数组长度作为独立变量或常量管理，而不是在多个地方重复写字面量。

## 10. 🤔 结构体可以嵌套吗？如何访问嵌套成员？

结构体可以嵌套结构体，也可以嵌套数组。

::: code-group

```c [结构体嵌套与访问]
#include <stdio.h>

struct Point {
  int x;
  int y;
};

struct Rect {
  struct Point lt;
  struct Point rb;
};

int main(void) {
  struct Rect r = { .lt = { 0, 0 }, .rb = { 10, 20 } };
  printf("(%d,%d) (%d,%d)\n", r.lt.x, r.lt.y, r.rb.x, r.rb.y);
  return 0;
}
```

:::

当嵌套层级较深时，可以考虑把关键子结构体拆出独立变量或函数，降低调用点的复杂度。

## 11. 🆚 一维数组模拟二维数组 vs 真二维数组有何区别？

两者都可以表达二维数据，但类型系统、传参方式和可维护性不同。

| 特性     | 一维数组模拟二维数组        | 真二维数组                         |
| -------- | --------------------------- | ---------------------------------- |
| 存储     | 连续                        | 连续                               |
| 类型     | `T*` 或 `T[]`               | `T[rows][cols]`                    |
| 索引     | 需要手动计算 `i * cols + j` | 直接 `a[i][j]`                     |
| 传参     | 传 `T*` 和 `cols`           | 需要已知 `cols` 或使用变长数组     |
| 动态维度 | 方便                        | 需要变长数组或动态分配二维指针结构 |
| 易错点   | 下标计算与越界              | 列数不匹配与退化规则               |

::: code-group

```c [一维数组模拟二维数组]
#include <stdio.h>

static int idx(int cols, int i, int j) {
  return i * cols + j;
}

int main(void) {
  int rows = 2;
  int cols = 3;
  int a[6] = { 1, 2, 3, 4, 5, 6 };

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%d\n", a[idx(cols, i, j)]);
    }
  }

  return 0;
}
```

```c [真二维数组]
#include <stdio.h>

int main(void) {
  int a[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
  printf("%d\n", a[1][2]);
  return 0;
}
```

:::

经验上，如果维度固定且以栈上数组为主，真二维数组更直观。如果维度来自运行期，或需要动态分配，很多项目会选择一维数组模拟二维数组。

## 12. 🤔 如何将数组或结构体作为函数参数传递？

数组作为参数会退化为指针，因此必须显式传入长度或维度信息。

::: code-group

```c [✅ 数组传参要带长度]
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

结构体作为参数有两种常见方式：

- 小结构体可以按值传递，调用点简单
- 大结构体建议传指针，并用 `const` 约束只读

::: code-group

```c [结构体按值传递]
#include <stdio.h>

struct Point {
  int x;
  int y;
};

static int manhattan(struct Point p) {
  return (p.x >= 0 ? p.x : -p.x) + (p.y >= 0 ? p.y : -p.y);
}

int main(void) {
  struct Point p = { 3, -4 };
  printf("%d\n", manhattan(p));
  return 0;
}
```

```c [✅ 结构体指针加 const]
#include <stdio.h>

struct Point {
  int x;
  int y;
};

static void print_point(const struct Point *p) {
  printf("(%d,%d)\n", p->x, p->y);
}

int main(void) {
  struct Point p = { 1, 2 };
  print_point(&p);
  return 0;
}
```

:::

## 13. 🤔 为什么不能直接用 == 比较两个数组或结构体？

在 C 语言里，数组不能直接用 `==` 比较内容。

- 数组名在表达式里会退化为指针
- `a == b` 比较的是两个指针地址，不是元素序列

结构体也不能直接用 `==` 比较。

- C 语言没有为结构体定义内建的相等运算

正确做法通常是：

- 数组逐元素比较
- 字符串用 `strcmp`
- 结构体逐字段比较

::: code-group

```c [✅ 比较两个整型数组]
#include <stdio.h>

static int array_equal(const int *a, const int *b, int n) {
  for (int i = 0; i < n; i++) {
    if (a[i] != b[i]) {
      return 0;
    }
  }
  return 1;
}

int main(void) {
  int a[] = { 1, 2, 3 };
  int b[] = { 1, 2, 3 };
  int n = (int)(sizeof(a) / sizeof(a[0]));
  printf("%d\n", array_equal(a, b, n));
  return 0;
}
```

```c [✅ 结构体按字段比较]
#include <stdio.h>

struct Point {
  int x;
  int y;
};

static int point_equal(struct Point a, struct Point b) {
  return a.x == b.x && a.y == b.y;
}

int main(void) {
  struct Point p1 = { 1, 2 };
  struct Point p2 = { 1, 2 };
  printf("%d\n", point_equal(p1, p2));
  return 0;
}
```

:::

不要轻易用 `memcmp` 比较结构体整体，结构体可能包含填充字节，未初始化的填充字节会让比较结果不稳定。

## 14. 🤔 如何实现数组或结构体的复制与初始化？

数组复制常见方式有两种：

- 循环逐元素复制
- `memcpy` 按字节复制

::: code-group

```c [数组逐元素复制]
#include <stdio.h>

static void copy_ints(int *dst, const int *src, int n) {
  for (int i = 0; i < n; i++) {
    dst[i] = src[i];
  }
}

int main(void) {
  int a[] = { 1, 2, 3 };
  int b[3] = { 0 };
  copy_ints(b, a, 3);
  printf("%d\n", b[2]);
  return 0;
}
```

```c [memcpy 复制数组]
#include <stdio.h>
#include <string.h>

int main(void) {
  int a[] = { 1, 2, 3 };
  int b[3];
  memcpy(b, a, sizeof(a));
  printf("%d\n", b[2]);
  return 0;
}
```

:::

结构体在 C 语言里可以直接赋值复制，属于按字节拷贝整个对象。

::: code-group

```c [✅ 结构体可以直接赋值]
#include <stdio.h>

struct User {
  int id;
  char name[16];
};

int main(void) {
  struct User u1 = { 1, "alice" };
  struct User u2 = u1;

  printf("%d %s\n", u2.id, u2.name);
  return 0;
}
```

:::

初始化方面，推荐使用带字段名的指定初始化器，它能显著降低字段顺序变化带来的维护成本。

::: code-group

```c [✅ 指定初始化器]
struct User u = { .id = 1001, .name = "alice" };
```

:::

## 15. 🤔 什么是结构体的内存对齐？为什么会影响 sizeof 结果？

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

## 16. 🔗 引用

- [cppreference：Arrays][1]
- [cppreference：Multidimensional arrays][2]
- [cppreference：struct 与成员访问][3]
- [cppreference：Initialization][4]
- [cppreference：sizeof 运算符][5]
- [cppreference：offsetof 宏][6]
- [cppreference：memcpy][7]
- [cppreference：memcmp][8]
- [cppreference：strcmp][9]

[1]: https://en.cppreference.com/w/c/language/array
[2]: https://en.cppreference.com/w/c/language/array#Multidimensional_arrays
[3]: https://en.cppreference.com/w/c/language/struct
[4]: https://en.cppreference.com/w/c/language/initialization
[5]: https://en.cppreference.com/w/c/language/sizeof
[6]: https://en.cppreference.com/w/c/types/offsetof
[7]: https://en.cppreference.com/w/c/string/byte/memcpy
[8]: https://en.cppreference.com/w/c/string/byte/memcmp
[9]: https://en.cppreference.com/w/c/string/byte/strcmp
