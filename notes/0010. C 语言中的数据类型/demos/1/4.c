#include <float.h>
#include <stdio.h>

int main(void) {
  printf("浮点型的大小和精度\n");
  printf("sizeof(float)       = %zu 字节\n", sizeof(float));
  printf("sizeof(double)      = %zu 字节\n", sizeof(double));
  printf("sizeof(long double) = %zu 字节\n", sizeof(long double));
  printf("float 精度:         约 %d 位有效数字\n", FLT_DIG);
  printf("double 精度:        约 %d 位有效数字\n", DBL_DIG);
  printf("long double 精度:   约 %d 位有效数字\n", LDBL_DIG);
  printf("\n");

  return 0;
}

/*
输出结果：
浮点型的大小和精度
sizeof(float)       = 4 字节
sizeof(double)      = 8 字节
sizeof(long double) = 8 字节
float 精度:         约 6 位有效数字
double 精度:        约 15 位有效数字
long double 精度:   约 15 位有效数字
*/
