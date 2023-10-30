#include <stdio.h>

// 0123456789
// 6784329150

// ala314s43
// ala473s34

// fputs !!!!

int main() {
    int T[10] = {6, 7, 8, 4, 3, 2, 9, 1, 5, 0};
    int c;

    while ((c = fgetc(stdin)) != EOF) {
        if (c >= '0' && c <= '9') {
            fputc(T[c - '0'] + '0', stdout);
        } else
            fputc(c, stdout);
    }

    return 0;
}