# [0014. 输入与输出](https://github.com/tnotesjs/TNotes.c/tree/main/notes/0014.%20%E8%BE%93%E5%85%A5%E4%B8%8E%E8%BE%93%E5%87%BA)

<!-- region:toc -->

- [1. 🎯 本节内容](#1--本节内容)
- [2. 🫧 评价](#2--评价)
- [3. 🤔 C 语言如何实现基本的输入与输出？](#3--c-语言如何实现基本的输入与输出)
- [4. 🤔 printf 函数的工作原理是什么？](#4--printf-函数的工作原理是什么)
- [5. 🤔 scanf 函数如何读取用户输入？有哪些注意事项？](#5--scanf-函数如何读取用户输入有哪些注意事项)
- [6. 🆚 printf vs puts / putchar 在输出字符串时有何区别？](#6--printf-vs-puts--putchar-在输出字符串时有何区别)
- [7. 🆚 scanf vs gets / fgets 在读取字符串时有何安全差异？](#7--scanf-vs-gets--fgets-在读取字符串时有何安全差异)
- [8. 🤔 格式说明符（如 %d, %s, %f）如何匹配变量类型？](#8--格式说明符如-d-s-f如何匹配变量类型)
- [9. 🤔 为什么 scanf 后常出现“输入被跳过”的问题？](#9--为什么-scanf-后常出现输入被跳过的问题)
- [10. 🤔 输入缓冲区是什么？它如何影响 I/O 行为？](#10--输入缓冲区是什么它如何影响-io-行为)

<!-- endregion:toc -->

## 1. 🎯 本节内容

- 标准输入输出
- `stdin`、`stdout`、`stderr`
- 格式化输出
- 格式化输入
- 输入缓冲区
- 换行与字符串读取
- 安全读取字符串
- 常见 I/O 陷阱

## 2. 🫧 评价

- `printf` 和 `scanf` 看似简单，但格式说明符与缓冲区细节很容易踩坑。
- 实战里优先用 `fgets` 读整行，再解析，比直接堆 `scanf` 更稳定。

## 3. 🤔 C 语言如何实现基本的输入与输出？

C 语言的基本输入与输出由标准库 `stdio.h` 提供。最常见的交互方式是通过三条标准流完成：

- `stdin`：标准输入，通常来自键盘。
- `stdout`：标准输出，通常输出到终端。
- `stderr`：标准错误输出，通常也输出到终端，但语义上用于错误信息。

你可以把它理解为：程序从 `stdin` 读数据，向 `stdout` 输出正常结果，向 `stderr` 输出告警与错误。

::: code-group

```c [最小示例]
#include <stdio.h>

int main(void) {

  puts("Hello, world");
  fprintf(stderr, "warning：%s\n", "something happened");
  return 0;
}
```

:::

注意：`stdout` 和 `stderr` 在很多环境中有不同的缓冲策略，这会影响你看到输出的时机。

## 4. 🤔 printf 函数的工作原理是什么？

`printf` 用于格式化输出。它会根据格式字符串中的格式说明符，把后续参数按指定方式转换为文本，并写入 `stdout`。

格式说明符由 `%` 开头，后面可能包含宽度、精度、长度修饰符等信息，例如：`%d`、`%8.2f`、`%zu`。

::: code-group

```c [格式化输出示例]
#include <stdio.h>

int main(void) {

  int age = 18;
  double pi = 3.14159;
  const char* name = "Alice";

  printf("name：%s\n", name);
  printf("age：%d\n", age);
  printf("pi：%.2f\n", pi);
  return 0;
}
```

:::

常见注意点：

- 格式说明符必须与参数类型匹配，否则可能导致未定义行为。
- `printf` 返回成功写出的字符数，失败时返回负值。
- 输出是否立刻可见与缓冲区有关，尤其是没有换行时。

## 5. 🤔 scanf 函数如何读取用户输入？有哪些注意事项？

`scanf` 用于按格式从 `stdin` 读取数据，并写入你提供的变量地址。

它的基本模式是：

- 用格式字符串描述期望的输入形态。
- 用参数列表提供变量地址，例如读 `int` 就传 `&x`。

::: code-group

```c [读取整数与浮点数]
#include <stdio.h>

int main(void) {

  int x;
  double y;

  printf("请输入一个整数和一个小数：");
  if (scanf("%d %lf", &x, &y) != 2) {
    fprintf(stderr, "输入格式不正确。\n");
    return 1;
  }

  printf("x=%d，y=%.2f\n", x, y);
  return 0;
}
```

:::

使用 `scanf` 的关键注意事项：

- 必须传地址：例如 `%d` 对应 `int*`，要传 `&x`。
- 要检查返回值：它表示成功匹配并赋值的项数。
- `%s` 读字符串会在空白处停止，而且存在缓冲区溢出风险。
- `scanf` 常与输入缓冲区问题绑定出现，尤其是混用读数字与读整行文本时。

## 6. 🆚 printf vs puts / putchar 在输出字符串时有何区别？

`printf`、`puts`、`putchar` 都能输出，但侧重点不同。

| 特性               | `printf`     | `puts`       | `putchar`    |
| ------------------ | ------------ | ------------ | ------------ |
| 输出类型           | 格式化输出   | 输出字符串   | 输出单个字符 |
| 是否支持格式说明符 | 支持         | 不支持       | 不支持       |
| 是否自动追加换行   | 不会自动追加 | 会自动追加   | 不会自动追加 |
| 典型用途           | 拼接与格式化 | 输出一行文本 | 输出字符流   |

::: code-group

```c [三者对比]
#include <stdio.h>

int main(void) {

  printf("A");
  putchar('B');
  puts("C");
  return 0;
}
```

:::

经验上：

- 只想输出一行固定文本，用 `puts` 更简洁。
- 要格式化内容，用 `printf`。
- 输出单个字符或逐字符处理，用 `putchar`。

## 7. 🆚 scanf vs gets / fgets 在读取字符串时有何安全差异？

读取字符串时，风险最大的点是缓冲区溢出。

| 特性         | `scanf("%s", ...)` | `gets`       | `fgets`          |
| ------------ | ------------------ | ------------ | ---------------- |
| 是否可能溢出 | 可能               | 必然风险很高 | 不会，受长度限制 |
| 是否读取空格 | 否                 | 是           | 是               |
| 是否保留换行 | 不适用             | 不适用       | 可能保留换行     |
| 推荐程度     | 谨慎使用           | 不推荐       | 推荐             |

结论很简单：不要用 `gets`。在实际开发中，优先用 `fgets` 读一整行，再自己处理末尾换行，并按需解析。

::: code-group

```c [✅ 使用 fgets 读取一行]
#include <stdio.h>
#include <string.h>

int main(void) {

  char buf[128];

  printf("请输入一行：");
  if (fgets(buf, sizeof buf, stdin) == NULL) {
    return 1;
  }

  size_t n = strlen(buf);
  if (n > 0 && buf[n - 1] == '\n') {
    buf[n - 1] = '\0';
  }

  printf("你输入的是：%s\n", buf);
  return 0;
}
```

:::

## 8. 🤔 格式说明符（如 %d, %s, %f）如何匹配变量类型？

格式说明符必须与变量类型严格对应，否则可能读错值、打印错值，甚至触发未定义行为。

下面是一些最常用的对应关系：

| 目标类型 | `printf` 常用 | `scanf` 常用 | 备注 |
| --- | --- | --- | --- |
| `int` | `%d` | `%d` | 读入要传 `int*` |
| `unsigned int` | `%u` | `%u` | 注意范围 |
| `long` | `%ld` | `%ld` | 与平台相关 |
| `long long` | `%lld` | `%lld` |  |
| `double` | `%f` | `%lf` | `scanf` 读 `double` 用 `%lf` |
| `char` | `%c` | `%c` | `scanf` 读字符常受空白影响 |
| 字符串 `char[]` | `%s` | `%s` | 有溢出风险，要限制宽度 |
| `size_t` | `%zu` | `%zu` | C99 引入 |

关于 `%s` 的一个关键技巧是限制最大读取长度，避免溢出。

::: code-group

```c [⚠️ %s 的溢出风险与限制宽度]
#include <stdio.h>

int main(void) {
  char name[8];

  // ⚠️ 错误写法：用户输入超过 7 个字符会溢出。
  // scanf("%s", name);

  // ✅ 正确写法：为 name 预留 1 个字节存放结尾 '\0'。
  if (scanf("%7s", name) != 1) {
    return 1;
  }

  printf("name：%s\n", name);
  return 0;
}
```

:::

## 9. 🤔 为什么 scanf 后常出现“输入被跳过”的问题？

最常见的场景是：先用 `scanf` 读数字，然后再用 `fgets` 读一行字符串。

原因在于：`scanf` 读完数字后，通常会把你敲下的换行符 `\n` 留在输入缓冲区里。随后 `fgets` 看到缓冲区里已经有一个换行，就会立刻读到一个“空行”，看起来像是输入被跳过。

::: code-group

```c [⚠️ 典型问题复现]
#include <stdio.h>

int main(void) {

  int age;
  char line[64];

  printf("age：");
  scanf("%d", &age);

  printf("name：");
  fgets(line, sizeof line, stdin);
  printf("line=%s", line);
  return 0;
}
```

```c [✅ 两种常见修复方式]
#include <stdio.h>
#include <string.h>

static void discard_line(void) {

  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF) {
  }
}

int main(void) {

  int age;
  char name[64];

  printf("age：");
  if (scanf("%d", &age) != 1) {
    return 1;
  }
  discard_line();

  printf("name：");
  if (fgets(name, sizeof name, stdin) == NULL) {
    return 1;
  }
  size_t n = strlen(name);
  if (n > 0 && name[n - 1] == '\n') {
    name[n - 1] = '\0';
  }

  printf("age=%d，name=%s\n", age, name);
  return 0;
}
```

:::

经验上，如果输入是“人类交互式输入”，用 `fgets` 读整行，再用 `sscanf` 或手写解析提取数字，通常更稳。

## 10. 🤔 输入缓冲区是什么？它如何影响 I/O 行为？

输入缓冲区可以理解为：系统把用户输入先暂存到一块内存里，程序的读取函数再从这块内存取数据。

它带来的现象包括：

- 你敲下的换行符可能被保留在缓冲区里，影响下一次读取。
- 对于 `scanf` 这类按格式匹配的读取，可能只消费一部分字符，剩余字符仍在缓冲区。
- 对于输出，`stdout` 在终端上通常是行缓冲，遇到换行或缓冲区满才刷新。

当你遇到 I/O 相关的异常行为时，先问自己两个问题：

1. 上一次读取到底消费了哪些字符，留下了哪些字符。
2. 下一次读取的函数规则是什么，是否会跳过空白，是否会读到换行就停止。

把这两点搞清楚，大部分输入输出问题都能定位。
