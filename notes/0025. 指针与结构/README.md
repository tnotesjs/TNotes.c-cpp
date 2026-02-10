# [0025. 指针与结构](https://github.com/tnotesjs/TNotes.c/tree/main/notes/0025.%20%E6%8C%87%E9%92%88%E4%B8%8E%E7%BB%93%E6%9E%84)

<!-- region:toc -->

- [1. 🎯 本节内容](#1--本节内容)
- [2. 🫧 评价](#2--评价)
- [3. 🤔 为什么需要指向结构体的指针？](#3--为什么需要指向结构体的指针)
- [4. 🤔 如何声明和初始化结构体指针？](#4--如何声明和初始化结构体指针)
- [5. 🤔 使用 . 和 -> 访问成员有何区别？](#5--使用--和---访问成员有何区别)
- [6. 🤔 如何通过指针传递结构体给函数？有什么优势？](#6--如何通过指针传递结构体给函数有什么优势)
- [7. 🆚 按值传递结构体 vs 按指针传递：性能和语义有何不同？](#7--按值传递结构体-vs-按指针传递性能和语义有何不同)
- [8. 🤔 结构体指针可以进行算术运算吗？何时有意义？](#8--结构体指针可以进行算术运算吗何时有意义)
- [9. 🤔 如何实现结构体的动态分配与释放？](#9--如何实现结构体的动态分配与释放)
- [10. 🤔 链表等数据结构如何利用结构体指针实现？](#10--链表等数据结构如何利用结构体指针实现)
- [11. 🔗 引用](#11--引用)

<!-- endregion:toc -->

## 1. 🎯 本节内容

- 结构体指针的必要性
- 结构体指针的声明与初始化
- `.` 与 `->` 的成员访问规则
- 指针传参的语义与性能
- 结构体数组与指针算术
- 结构体的动态分配与释放
- 基于结构体指针的链表

## 2. 🫧 评价

- 结构体指针是 C 语言建模与写接口的核心工具，掌握它基本就能把参数传递、动态内存、链表这几块连起来。
- 最容易翻车的点不是语法，而是生命周期与所有权，一旦写错就会出现悬空指针、重复释放等高风险问题。

## 3. 🤔 为什么需要指向结构体的指针？

结构体指针的常见动机有三个。

1. 避免拷贝大结构体
2. 允许函数修改调用者的结构体对象
3. 用自引用指针构建链表、树等动态数据结构

::: code-group

```c [指针让函数修改结构体]
#include <stdio.h>

struct User {
  int id;
  int age;
};

static void set_age(struct User *u, int age) {
  if (u == NULL) {
    return;
  }
  u->age = age;
}

int main(void) {
  struct User u = { 1001, 18 };
  set_age(&u, 20);
  printf("%d\n", u.age);
  return 0;
}
```

:::

当你想表达某个对象可以不存在时，也更适合用指针，例如参数允许为 `NULL`。

## 4. 🤔 如何声明和初始化结构体指针？

结构体指针的声明形式是 `struct TypeName *p`。

初始化常见来源：

- 取地址 `&obj`
- 动态分配 `malloc`
- 指向结构体数组的某个元素

::: code-group

```c [取地址初始化]
struct Point {
  int x;
  int y;
};

int main(void) {
  struct Point p = { 1, 2 };
  struct Point *pp = &p;
  return 0;
}
```

```c [⚠️ 指针初始化为 NULL]
struct Point {
  int x;
  int y;
};

int main(void) {
  struct Point *pp = NULL;
  (void)pp;
  return 0;
}
```

:::

不要把未初始化指针当成可用指针。

::: code-group

```c [❌ 未初始化结构体指针]
struct Point {
  int x;
  int y;
};

int main(void) {
  struct Point *pp;
  /* ❌ 未定义行为：pp 未指向有效对象 */
  /* pp->x = 1; */
  return 0;
}
```

:::

## 5. 🤔 使用 . 和 -> 访问成员有何区别？

区别只在于操作数类型。

- `.` 用于结构体对象
- `->` 用于结构体指针

它们满足关系：`p->x` 等价于 `(*p).x`。

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
  printf("%d\n", (*pp).x);
  return 0;
}
```

:::

⚠️ 优先写 `pp->x`，不要为了炫技到处写 `(*pp).x`，可读性更差。

## 6. 🤔 如何通过指针传递结构体给函数？有什么优势？

常见做法是传 `struct T*`，如果函数只读就传 `const struct T*`。

优势主要是：

- 避免拷贝，尤其是结构体较大时
- 明确表达函数会不会修改对象

::: code-group

```c [只读访问用 const 指针]
#include <stdio.h>

struct User {
  int id;
  int age;
};

static void print_user(const struct User *u) {
  if (u == NULL) {
    return;
  }
  printf("id=%d age=%d\n", u->id, u->age);
}

int main(void) {
  struct User u = { 1001, 18 };
  print_user(&u);
  return 0;
}
```

:::

如果你把结构体按值传递给函数，函数内部修改的是副本，调用者看不到变化。

## 7. 🆚 按值传递结构体 vs 按指针传递：性能和语义有何不同？

两者差异主要体现在拷贝成本与修改语义。

| 特性       | 按值传递               | 按指针传递               |
| ---------- | ---------------------- | ------------------------ |
| 是否拷贝   | 会拷贝结构体           | 不拷贝结构体本体         |
| 修改可见性 | 修改副本，调用者不可见 | 修改对象本体，调用者可见 |
| 可空性     | 不可为空               | 可以用 `NULL` 表示无对象 |
| 接口表达   | 更简单                 | 需要明确生命周期与所有权 |

::: code-group

```c [按值传递修改无效]
#include <stdio.h>

struct Point {
  int x;
  int y;
};

static void set_x(struct Point p, int x) {
  p.x = x;
}

int main(void) {
  struct Point p = { 1, 2 };
  set_x(p, 10);
  printf("%d\n", p.x);
  return 0;
}
```

```c [按指针传递修改可见]
#include <stdio.h>

struct Point {
  int x;
  int y;
};

static void set_x(struct Point *p, int x) {
  if (p == NULL) {
    return;
  }
  p->x = x;
}

int main(void) {
  struct Point p = { 1, 2 };
  set_x(&p, 10);
  printf("%d\n", p.x);
  return 0;
}
```

:::

性能上不要拍脑袋判断，结构体很小的时候按值传递也很常见，关键是语义清晰。

## 8. 🤔 结构体指针可以进行算术运算吗？何时有意义？

结构体指针也可以做指针算术，但只在两种场景有意义。

1. 指向结构体数组的元素
2. 指向连续分配的一段结构体内存

::: code-group

```c [结构体数组与指针算术]
#include <stdio.h>

struct Point {
  int x;
  int y;
};

int main(void) {
  struct Point pts[3] = { { 0, 0 }, { 1, 2 }, { 3, 4 } };
  struct Point *p = &pts[0];

  printf("%d\n", (p + 2)->x);
  return 0;
}
```

:::

⚠️ 指针算术只能在同一个数组对象范围内进行，越界就是未定义行为。

## 9. 🤔 如何实现结构体的动态分配与释放？

动态分配结构体通常用 `malloc`，释放用 `free`。

核心要点：

- 检查 `malloc` 返回值是否为 `NULL`
- 用完必须 `free`
- `free` 后把指针置为 `NULL`

::: code-group

```c [动态分配与释放]
#include <stdio.h>
#include <stdlib.h>

struct User {
  int id;
  int age;
};

int main(void) {
  struct User *u = (struct User*)malloc(sizeof(struct User));
  if (u == NULL) {
    return 1;
  }

  u->id = 1001;
  u->age = 18;
  printf("id=%d\n", u->id);

  free(u);
  u = NULL;
  return 0;
}
```

:::

如果结构体内部还包含动态分配的成员，释放时要遵循相同的所有权规则，先释放内部成员，再释放结构体本体。

## 10. 🤔 链表等数据结构如何利用结构体指针实现？

链表的基本思想是：每个节点包含数据和指向下一个节点的指针。

::: code-group

```c [单向链表节点]
struct Node {
  int value;
  struct Node *next;
};
```

:::

下面是一个最小可运行的链表头插示例。

::: code-group

```c [链表头插与遍历]
#include <stdio.h>
#include <stdlib.h>

struct Node {
  int value;
  struct Node *next;
};

static struct Node* push_front(struct Node *head, int v) {
  struct Node *n = (struct Node*)malloc(sizeof(struct Node));
  if (n == NULL) {
    return head;
  }
  n->value = v;
  n->next = head;
  return n;
}

static void free_list(struct Node *head) {
  while (head != NULL) {
    struct Node *next = head->next;
    free(head);
    head = next;
  }
}

int main(void) {
  struct Node *head = NULL;
  head = push_front(head, 1);
  head = push_front(head, 2);
  head = push_front(head, 3);

  for (struct Node *p = head; p != NULL; p = p->next) {
    printf("%d\n", p->value);
  }

  free_list(head);
  return 0;
}
```

:::

链表相关代码最重要的是释放逻辑与异常分支处理，建议每次改动后都跑一遍内存检查工具。

## 11. 🔗 引用

- [cppreference：struct 与成员访问][1]
- [cppreference：Pointers][2]
- [cppreference：malloc][3]
- [cppreference：free][4]

[1]: https://en.cppreference.com/w/c/language/struct
[2]: https://en.cppreference.com/w/c/language/pointer
[3]: https://en.cppreference.com/w/c/memory/malloc
[4]: https://en.cppreference.com/w/c/memory/free
