#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void sort(int* tablica, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (tablica[i] < tablica[j]) {
                swap(tablica + i, tablica + j);
            }
        }
    }
}

int main() {
    unsigned int n, k, i;
    scanf("%d %d", &n, &k);

    int* tablica = (int*)malloc(n * sizeof(int));

    for (i = 0; i < n; i++) {
        scanf("%d", &tablica[i]);
        tablica[i] = tablica[i] % k;
    }

    sort(tablica, n);

    for (i = 0; i < n; i++) {
        if (i == n - 1) {
            printf("%d", tablica[i]);
        } else {
            printf("%d ", tablica[i]);
        }
    }

    return 0;
}