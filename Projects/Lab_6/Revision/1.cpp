#include <stdio.h>
#include <stdlib.h>

#define SIZE 10000000000
int porownaj(int* A, int* B, int size);
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

void dodaj(int* X, int* Y, int size, int* wynik) {
    int tmp;
    for (int i = 0; i < size; i++) {
        tmp = X[i] + Y[i];
        if (tmp > 9) {
            wynik[i] = tmp - 10;
            X[i + 1]++;
        } else {
            wynik[i] = tmp;
        }
    }
}
int A[SIZE], B[SIZE];
int AR[SIZE], BR[SIZE];
int wynik[SIZE];

int main() {
    int n, m, odp, t;
    int bigger = 0;

    scanf("%d", &t);
    getchar();

    n = wczytaj(A, ' ');
    m = wczytaj(B, '\n');

    odwroc(A, AR, n);
    odwroc(B, BR, m);

    if (n < m) {
        bigger = m;
    } else if (n > m) {
        bigger = n;
    } else {
        bigger = n;
        odp = porownaj(A, B, n);
        if (odp == -1) {
            zwieksz(AR, n);
        } else if (odp == 1) {
            zwieksz(BR, m);
        } else {
            zwieksz(AR, n);
        }
    }

    dodaj(AR, BR, bigger, wynik);

    odwroc(wynik, wynik, bigger);

    drukuj(wynik, bigger);
    n = 0;
    m = 0;
    n = wczytaj(A, ' ');
    m = wczytaj(B, ' ');
    drukuj(A, n);
    drukuj(A, m);

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