#include <stdio.h>
#include <string.h>
#define H r ==
int f(char r) {
    return H 'I'   ? 1
           : H 'V' ? 5
           : H 'X' ? 10
           : H 'L' ? 50
           : H 'C' ? 100
           : H 'D' ? 500
           : H 'M' ? 1000
                   : -1;
}
int t(char *r) {
    int y, e, i;
    for (i = strlen(r) - 1; i >= 0; e = f(r[i--]))
        y += e > f(r[i]) ? -f(r[i]) : f(r[i]);
    return y;
}
void g(int w) {
    int i = 0, j = 0, k = 0,
        a[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    char z[100], *r[] = {"M",  "CM", "D",  "CD", "C",  "XC", "L",
                         "XL", "X",  "IX", "V",  "IV", "I"};
    while (w > 0) {
        if (w < a[i]) {
            i++;
            continue;
        }
        for (j = 0; r[i][j] != '\0'; j++, k++) z[k] = r[i][j];
        w -= a[i];
    }
    z[k] = '\0';
    printf("%s", z);
}
int q(int a, int b, char r) {
    return r == '+' ? a + b
           : H '-'  ? a - b
           : H '*'  ? a * b
           : H '/'  ? a / b
                    : -1;
}
int main() {
    char r1[100], r2[100], h;
    int y, a, b;
    while (scanf("%s %c %s", r1, &h, r2) == 3) {
        a = t(r1);
        b = t(r2);
        y = q(a, b, h);
        if (y <= 0 || y > 3999) {
            printf("-");
        } else {
            g(y);
        }
        printf("\n");
    }
    return 0;
}