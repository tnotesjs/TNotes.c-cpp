/**
 * demos/3.c
 */

#include <stdio.h>
#include <limits.h>

int main(void) {
    printf("整型的取值范围\n");
    printf("char:          %d 到 %d\n", CHAR_MIN, CHAR_MAX);
    printf("unsigned char: 0 到 %u\n", UCHAR_MAX);
    printf("short:         %d 到 %d\n", SHRT_MIN, SHRT_MAX);
    printf("int:           %d 到 %d\n", INT_MIN, INT_MAX);
    printf("unsigned int:  0 到 %u\n", UINT_MAX);
    printf("long:          %ld 到 %ld\n", LONG_MIN, LONG_MAX);
    printf("long long:     %lld 到 %lld\n", LLONG_MIN, LLONG_MAX);
    printf("\n");
    
    return 0;
}

/* 
输出结果：
整型的取值范围
char:          -128 到 127
unsigned char: 0 到 255
short:         -32768 到 32767
int:           -2147483648 到 2147483647
unsigned int:  0 到 4294967295
long:          -9223372036854775808 到 9223372036854775807
long long:     -9223372036854775808 到 9223372036854775807
*/
