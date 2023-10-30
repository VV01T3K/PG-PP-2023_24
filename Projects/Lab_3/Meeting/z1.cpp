// statystyka cyfr

// statystyka liter TODO

#define SIZE 'z' - 'a' + 1

#include <stdio.h>

int main() {
    int cyfry[10] = {0};
    int c;

    while ((c = fgetc(stdin)) != EOF) {
        // cyfry[c - 48]++;
        if (c >= '0' || c <= '9') {
            cyfry[c - '0']++;
        }
    }

    printf("Statystyka cyfr:\n");
    for (int i = 0; i < 10; i++) {
        printf("%d: %d\n", i, cyfry[i]);
    }

    return 0;
}