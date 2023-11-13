#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void swap(unsigned char* a, unsigned char* b) {
    unsigned char tmp = *a;
    *a = *b;
    *b = tmp;
}

int compare(const void* a, const void* b) {
    return (*(unsigned char*)a - *(unsigned char*)b);
}

int main() {
    unsigned char n, k, i, tmp;
    float tmp2;
    scanf("%hhd %hhd", &n, &k);

    unsigned char* tablica = (unsigned char*)malloc(n * sizeof(unsigned char));

    for (i = 0; i < n; i++) {
        scanf("%hhd", &tmp);
        tmp2 = tmp % k;
        tablica[i] = (int)floor(tmp2);
    }

    qsort(tablica, n, sizeof(unsigned char), compare);

    for (i = 0; i < n; i++) {
        printf("%hhd ", tablica[i]);
    }

    free(tablica);

    return 0;
}