#include <stdio.h>
#include <stdlib.h>

#define SIZE 1000000

#define mmin(a, b) ((a) < (b) ? (a) : (b))

// int A[SIZE];

int main() {
    // int A[SIZE];

    int n = SIZE;
    int* A = 0;
    A = (int*)malloc(n * sizeof(int));

    // malloc(bajty) - memory allocation (HEAP) | zwraca adres poczatku
    // zaalokowanej pamieci free - zwalnianie pamieci

    // flagi zanim spoj: -ansi -Werror

    // top - ram linux check on alpine

    printf("HELLO!\n");

    for (int i = 0; i < SIZE; i++) A[i] = 1;

    for (int i = 0; i < mmin(5, SIZE); i++) printf("A[%d] = %d\n", i, A[i]);

    return 0;
}