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
int porownaj(int* A, int* B, int size);

int main() {
    int n, m, odp;
    int A[SIZE], B[SIZE];

    n = wczytaj(A, ' ');
    m = wczytaj(B, '\n');

    if (n < m) {
        printf("<\n");
    } else if (n > m) {
        printf(">\n");
    } else {
        odp = porownaj(A, B, n);
        if (odp == -1) {
            printf("<\n");
        } else if (odp == 1) {
            printf(">\n");
        } else {
            printf("=\n");
        }
    }

    return 0;
}

int porownaj(int* A, int* B, int size) {
    int i = 0;
    while (1) {
        if (A[i] != B[i]) break;
        i++;
        if (i == size) return 0;
    }
    if (A[i] < B[i])
        return -1;
    else
        return 1;
    return 0;
}