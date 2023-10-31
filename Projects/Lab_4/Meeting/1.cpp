#include <stdio.h>

// wskaźniki
int main() {
    int a = 2;
    int* c = 0;

    printf("adr(a) = %p\n", &a);
    printf("adr(c) = %p\n", &c);

    c = &a;
    printf("val(c) = %p\n", c);

    *c = 5;
    printf("val(a) = %d\n", a);

    return 0;
}