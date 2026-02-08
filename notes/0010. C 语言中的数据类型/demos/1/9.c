/**
 * demos/9.c
 */

#include <stdio.h>

int main(void) {
    printf("各类型变量的实际使用示例\n");
    short age = 25;
    unsigned int count = 1000U;
    long distance = 384400L;
    long long population = 8000000000LL;
    float pi_f = 3.14159f;
    double pi_d = 3.141592653589793;
    
    printf("short age = %d\n", age);
    printf("unsigned int count = %u\n", count);
    printf("long distance = %ld km\n", distance);
    printf("long long population = %lld\n", population);
    printf("float pi = %.5f\n", pi_f);
    printf("double pi = %.15lf\n", pi_d);
    printf("\n");
    
    return 0;
}

/* 
输出结果：
各类型变量的实际使用示例
short age = 25
unsigned int count = 1000
long distance = 384400 km
long long population = 8000000000
float pi = 3.14159
double pi = 3.141592653589793
*/
