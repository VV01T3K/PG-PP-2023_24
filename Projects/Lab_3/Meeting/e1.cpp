#include <stdio.h>

#define typ long long

int main() {
    typ A[5] = {0, 1, 2};

    // printf("add(a) = %d\n", A[0]);
    // printf("add(a) = %p\n", &(A[1]));
    // printf("add(a) = %p\n", &A);
    printf("%p\n", &A[0]);
    printf("%p\n", &A[1]);
    // printf("%p\n", &A[2]);
    // printf("%p\n", &A[3]);
    printf("%d\n", (int)sizeof(typ));
    // printf("\n");
    // printf("%p\n", &A[3]);
    // printf("%p\n", &A[3]);

    return 0;
}