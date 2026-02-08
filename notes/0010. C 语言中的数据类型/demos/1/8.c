/**
 * demos/8.c
 */

#include <stdio.h>

int main(void) {
    printf("浮点数精度差异演示\n");
    float f = 1.0f / 3.0f;
    double d = 1.0 / 3.0;
    long double ld = 1.0L / 3.0L;
    
    printf("float:       %.20f\n", f);
    printf("double:      %.20lf\n", d);
    printf("long double: %.20Lf\n", ld);
    printf("\n");
    
    return 0;
}

/* 
输出结果：
浮点数精度差异演示
float:       0.33333334326744079590
double:      0.33333333333333331483
long double: 0.33333333333333331483
*/
