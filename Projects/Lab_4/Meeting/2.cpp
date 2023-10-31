#include <stdio.h>

int main() {
    int i;
    int* c = 0;
    int A[5] = {0, 2, 4, 6, 8};

    printf("A = %p\n", A);

    printf("0-------------------------\n");
    for (i = 0; i < 5; i++) {
        printf("adr A[%d] = %p\n", i, &A[i]);
        printf("val A[%d] = %d\n", i, A[i]);
    }
    printf("1-------------------------\n");
    for (i = 0, c = A; i < 5; i++, c++) {
        printf("adr A[%d] = %p\n", i, c);
        printf("val A[%d] = %d\n", i, *c);
    }
    printf("2-------------------------\n");
    for (c = A; c < A + 5; c++) {
        printf("adr A[*] = %p\n", c);
        printf("val A[*] = %d\n", *c);
    }
    printf("3-------------------------\n");
    for (i = 0; i < 5; i++) {
        printf("adr A[%d] = %p\n", i, A + i);
        printf("val A[%d] = %d\n", i, *(A + i));
    }

    return 0;
}