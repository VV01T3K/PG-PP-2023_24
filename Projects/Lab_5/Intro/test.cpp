
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* function to generate and return random numbers */
int *getRandom() {
    int r[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

    return r;
}

/* main function to call above defined function */
int main() {
    /* a pointer to an int */
    int *p;
    int i;

    p = getRandom();

    for (i = 0; i < 10; i++) {
        printf("*(p + %d) : %d\n", i, *(p + i));
    }

    return 0;
}