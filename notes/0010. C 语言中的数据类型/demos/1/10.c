#include <stdio.h>

int main(void) {
  printf("当前平台数据模型\n");
  if (sizeof(int) == 4 && sizeof(long) == 4 && sizeof(void*) == 4) {
    printf("数据模型: ILP32 (int/long/pointer 都是 32 位)\n");
  } else if (sizeof(int) == 4 && sizeof(long) == 8 && sizeof(void*) == 8) {
    printf("数据模型: LP64 (long/pointer 是 64 位)\n");
  } else {
    printf("数据模型: 其他\n");
  }
  printf("\n");

  return 0;
}

/*
输出结果：
当前平台数据模型
数据模型: LP64 (long/pointer 是 64 位)
*/
