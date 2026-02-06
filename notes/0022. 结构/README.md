# [0022. 结构](https://github.com/tnotesjs/TNotes.c-cpp/tree/main/notes/0022.%20%E7%BB%93%E6%9E%84)

<!-- region:toc -->

- [1. 🎯 本节内容](#1--本节内容)
- [2. 🫧 评价](#2--评价)
- [3. 🤔 什么是结构体（struct）？它的设计目的是什么？](#3--什么是结构体struct它的设计目的是什么)
- [4. 🤔 如何定义和声明结构体类型与变量？](#4--如何定义和声明结构体类型与变量)
- [5. 🤔 结构体成员如何初始化和访问？](#5--结构体成员如何初始化和访问)
- [6. 🤔 结构体可以包含哪些类型的成员？能否嵌套自身？](#6--结构体可以包含哪些类型的成员能否嵌套自身)
- [7. 🆚 直接初始化 vs 成员逐个赋值有何区别？](#7--直接初始化-vs-成员逐个赋值有何区别)
- [8. 🤔 什么是结构体的内存布局和对齐规则？](#8--什么是结构体的内存布局和对齐规则)
- [9. 🤔 为什么 sizeof(struct) 通常大于成员大小之和？](#9--为什么-sizeofstruct-通常大于成员大小之和)
- [10. 🤔 如何将结构体作为函数参数或返回值传递？](#10--如何将结构体作为函数参数或返回值传递)
- [11. 🔗 引用](#11--引用)

<!-- endregion:toc -->

## 1. 🎯 本节内容

- 结构体的设计目的与使用场景
- 结构体类型与变量的定义方式
- 成员初始化与访问规则
- 结构体嵌套与自引用指针
- 结构体内存布局与对齐
- 结构体的传参与返回

## 2. 🫧 评价

- 结构体语法不难，难在把接口设计清楚，字段含义、长度、所有权和生命周期要一开始就写明白。
- 结构体的对齐与填充经常被忽略，但它会影响 `sizeof` 结果和序列化布局，建议养成用 `sizeof`、`offsetof` 验证的习惯。

## 3. 🤔 什么是结构体（struct）？它的设计目的是什么？

结构体 `struct` 用来把多个字段组合成一个整体，表达一个更贴近业务语义的对象。

它的设计目的通常是：

- 把多个相关数据作为一个整体传递与返回
- 在内存中紧凑地存储一条记录
- 用字段名表达含义，提升可读性与可维护性

::: code-group

```c [结构体表达一条记录]
struct User {
  int id;
  int age;
};
```

:::

当你发现自己在函数参数里总是成组地传 `id`、`age`、`name` 这类相关字段时，通常就该引入结构体了。

## 4. 🤔 如何定义和声明结构体类型与变量？

结构体类型的定义通常有两种写法。

第一种是只定义类型，通过 `struct TypeName` 来引用。

::: code-group

```c [定义结构体类型]
struct Point {
  int x;
  int y;
};

int main(void) {
  struct Point p = { 1, 2 };
  return 0;
}
```

:::

第二种是定义类型的同时定义变量。

::: code-group

```c [定义类型并定义变量]
struct Point {
  int x;
  int y;
} p1, p2;
```

:::

如果你经常觉得 `struct Point` 太长，可以配合 `typedef` 起别名。

::: code-group

```c [typedef 简化类型名]
typedef struct {
  int x;
  int y;
} Point;

int main(void) {
  Point p = { 1, 2 };
  return 0;
}
```

:::

注意：是否使用 `typedef` 取决于团队风格，核心是统一。

## 5. 🤔 结构体成员如何初始化和访问？

结构体成员初始化常见方式有：

- 位置初始化
- 指定初始化器

::: code-group

```c [位置初始化]
#include <stdio.h>

struct User {
  int id;
  int age;
};

int main(void) {
  struct User u = { 1001, 18 };
  printf("id=%d age=%d\n", u.id, u.age);
  return 0;
}
```

```c [指定初始化器更稳]
#include <stdio.h>

struct User {
  int id;
  int age;
};

int main(void) {
  struct User u = { .age = 18, .id = 1001 };
  printf("id=%d age=%d\n", u.id, u.age);
  return 0;
}
```

:::

成员访问规则：

- 结构体对象用 `.` 访问成员
- 结构体指针用 `->` 访问成员

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

## 6. 🤔 结构体可以包含哪些类型的成员？能否嵌套自身？

结构体成员可以是：

- 基本类型
- 数组
- 指针
- 其他结构体

::: code-group

```c [结构体成员类型示例]
struct Header {
  unsigned short type;
  unsigned short len;
};

struct Packet {
  struct Header h;
  unsigned char data[8];
};
```

:::

结构体不能直接包含自身类型的成员，因为那会导致类型大小无限递归。

::: code-group

```c [❌ 不能直接包含自身]
struct Node {
  int value;
  /* struct Node next; */
};
```

```c [用指针实现自引用]
struct Node {
  int value;
  struct Node *next;
};
```

:::

自引用指针是链表、树等数据结构的基础。

## 7. 🆚 直接初始化 vs 成员逐个赋值有何区别？

两者的核心差异在于发生时机和适用场景。

| 特性     | 直接初始化               | 成员逐个赋值                   |
| -------- | ------------------------ | ------------------------------ |
| 发生时机 | 定义变量时               | 变量已存在后                   |
| 常用方式 | 位置初始化与指定初始化器 | `.` 或 `->` 给成员赋值         |
| 适用场景 | 初始值明确               | 需要分步骤构造或依赖运行期数据 |
| 风险点   | 位置初始化易写错顺序     | 忘记给某些字段赋值             |

::: code-group

```c [直接初始化]
struct User {
  int id;
  int age;
};

int main(void) {
  struct User u = { .id = 1001, .age = 18 };
  return 0;
}
```

```c [成员逐个赋值]
struct User {
  int id;
  int age;
};

int main(void) {
  struct User u;
  u.id = 1001;
  u.age = 18;
  return 0;
}
```

:::

如果结构体里有数组成员，逐个赋值时也要注意数组不能整体赋值，字符串写入更推荐 `snprintf`。

## 8. 🤔 什么是结构体的内存布局和对齐规则？

结构体的内存布局由成员顺序与对齐规则共同决定。

对齐的直观效果是：编译器可能在成员之间或结构体末尾插入填充字节，让某些成员从特定的字节边界开始存放。

你可以用 `offsetof` 来观察成员偏移。

::: code-group

```c [用 offsetof 观察布局]
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

对齐规则与平台、编译器和编译选项有关，不要依赖某个固定数值。

## 9. 🤔 为什么 sizeof(struct) 通常大于成员大小之和？

原因通常是填充字节。

例如当 `char` 后面跟 `int` 时，`int` 往往要求 4 字节对齐，编译器可能在中间插入 3 个字节的填充。

另外，结构体整体大小通常也会被补齐到最大对齐的整数倍，这样结构体数组里每个元素都能保持对齐。

::: code-group

```c [成员顺序会影响 sizeof]
#include <stdio.h>

struct A {
  char c;
  int x;
};

struct B {
  int x;
  char c;
};

int main(void) {
  printf("sizeof(A)=%zu\n", sizeof(struct A));
  printf("sizeof(B)=%zu\n", sizeof(struct B));
  return 0;
}
```

:::

经验建议：如果你关心内存占用，可以调整字段顺序把大对齐的类型放前面，但不要为了省几个字节牺牲可读性。

## 10. 🤔 如何将结构体作为函数参数或返回值传递？

常见传递方式有两种。

1. 按值传递与按值返回
2. 传结构体指针

小结构体按值传递通常很方便。

::: code-group

```c [结构体按值传参与返回]
#include <stdio.h>

struct Point {
  int x;
  int y;
};

static struct Point make_point(int x, int y) {
  struct Point p = { .x = x, .y = y };
  return p;
}

static int sum_xy(struct Point p) {
  return p.x + p.y;
}

int main(void) {
  struct Point p = make_point(1, 2);
  printf("%d\n", sum_xy(p));
  return 0;
}
```

:::

当结构体较大或你希望避免拷贝时，可以传指针，并用 `const` 表达只读。

::: code-group

```c [结构体指针加 const]
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

## 11. 🔗 引用

- [cppreference：Struct declaration][1]
- [cppreference：Initialization][2]
- [cppreference：sizeof 运算符][3]
- [cppreference：offsetof 宏][4]

[1]: https://en.cppreference.com/w/c/language/struct
[2]: https://en.cppreference.com/w/c/language/initialization
[3]: https://en.cppreference.com/w/c/language/sizeof
[4]: https://en.cppreference.com/w/c/types/offsetof
