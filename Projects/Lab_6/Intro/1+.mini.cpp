#include <stdio.h>
#include <string.h>
int f(char r) {
    char s[] = "IVXLCDM";
    int v[] = {1, 5, 10, 50, 100, 500, 1000}, i = 0;
    while (s[i] != '\0') {
        if (r == s[i]) {
            return v[i];
        }
        i++;
    }
}
int t(char *r) {
    int y = 0, e = 0, i;
    for (i = strlen(r) - 1; i >= 0; i--) {
        if (f(r[i]) < e) {
            y -= f(r[i]);
        } else {
            y += f(r[i]);
        }
        e = f(r[i]);
    }
    return y;
}
void pr(int num) {
    int a[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const char *r[] = {"M",  "CM", "D",  "CD", "C",  "XC", "L",
                       "XL", "X",  "IX", "V",  "IV", "I"};
    int i = 0, j = 0, k = 0;
    char tmp_r[100];
    while (num > 0) {
        if (num >= a[i]) {
            j = 0;
            while (r[i][j] != '\0') {
                tmp_r[k + j] = r[i][j];
                j++;
            }
            k += j;
            num -= a[i];
        } else {
            i++;
        }
    }
    tmp_r[k] = '\0';
    printf("%s", tmp_r);
}
int calc(int a, int b, char op) {
    switch (op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        default:
            return -1;
    }
}
int main() {
    char r1[100];
    char r2[100];
    char op;
    int y, a, b;
    while (scanf("%s %c %s", r1, &op, r2) == 3) {
        a = t(r1);
        b = t(r2);
        y = calc(a, b, op);
        if (y <= 0 || y > 3999) {
            printf("-");
        } else {
            pr(y);
        }
        printf("\n");
    }
    return 0;
}