#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void push(long long int liczba, long long int array[], int &wskaznik) {
    if (wskaznik == 10) {
        printf(":(\n");
    } else {
        array[wskaznik] = liczba;
        wskaznik++;
        printf(":)\n");
    }
}

void pop(long long int array[], int &wskaznik) {
    if (wskaznik == 0) {
        printf(":(\n");
    } else {
        printf("%lld\n", array[wskaznik - 1]);
        wskaznik--;
    }
}

long long int liczba;
long long int array[10] = {0};
int main() {
    char c;
    int wskaznik = 0;

    while (scanf("%c%*c", &c) == 1) {
        if (c == '+') {
            scanf("%lld%*c", &liczba);
            push(liczba, array, wskaznik);
        }
        if (c == '-') {
            pop(array, wskaznik);
        }
    }
    return 0;
}