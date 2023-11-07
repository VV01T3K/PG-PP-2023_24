#include <stdio.h>
#include <stdlib.h>

#define SIZE 1000000

int* utworz_tablice(int n) {
    int* T = (int*)malloc(n * sizeof(int));
    return T;
}
void zmien(int* T, int size, int value) {
    for (int i = 0; i < size; i++) T[i] = value;
}

void wyswietl(int* T, int n) {
    for (int i = 0; i < n; i++) printf("A[%d] = %d\n", i, T[i]);
}

int main() {
    int* A = 0;

    int n = SIZE;

    A = utworz_tablice(n);

    zmien(A, n, 3);

    wyswietl(A, 5);

    free(A);

    while (1)
        ;

    // terminal
    // & w tle
    // top wyswietla procesy
    // kill -9 PID

    return 0;
}