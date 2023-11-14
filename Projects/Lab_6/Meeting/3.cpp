#include <stdio.h>
#include <stdlib.h>

#define SIZE 200

int wczytaj(int* A, int s) {
    int i = 0, c;
    while ((c = fgetc(stdin)) != s) {
        A[i] = c - '0';
        i++;
    }
    return i;
}

void drukuj(int* A, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d", A[i]);
    }
}

void odwroc(int* A, int* R, int size) {
    for (int i = 0; i < size; i++) {
        R[i] = A[size - i - 1];
    }
}
int zwieksz(int* X, int size) {
    int i = 0;
    while (1) {
        if (X[i] < 9) {
            X[i]++;
            return size;
        }
        X[i] = 0;
        i++;
        if (i == size) {
            X[i] = 1;
            return size + 1;
        }
    }
}

int main() {
    int n, m, odp;
    int A[SIZE], R[SIZE];

    n = wczytaj(A, '\n');

    odwroc(A, R, n);

    m = zwieksz(R, n);

    odwroc(R, A, m);

    drukuj(A, m);

    return 0;
}
