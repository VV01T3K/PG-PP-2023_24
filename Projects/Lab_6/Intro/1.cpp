#include <stdio.h>
#include <string.h>

int f(char roman) {
    char romans[] = "IVXLCDM";
    int values[] = {1, 5, 10, 50, 100, 500, 1000}, i = 0;
    while (romans[i] != '\0') {
        if (roman == romans[i]) {
            return values[i];
        }
        i++;
    }
}

int roman_to_arabic(char *roman) {
    int result = 0, prev = 0;
    for (int i = strlen(roman) - 1; i >= 0; i--) {
        if (f(roman[i]) < prev) {
            result -= f(roman[i]);
        } else {
            result += f(roman[i]);
        }
        prev = f(roman[i]);
    }
    return result;
}

void print_arabic_as_roman(int num) {
    int arabic[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const char *roman[] = {"M",  "CM", "D",  "CD", "C",  "XC", "L",
                           "XL", "X",  "IX", "V",  "IV", "I"};
    int i = 0, j = 0, k = 0;
    char tmp_roman[100];
    while (num > 0) {
        if (num >= arabic[i]) {
            j = 0;
            while (roman[i][j] != '\0') {
                tmp_roman[k + j] = roman[i][j];
                j++;
            }
            k += j;
            num -= arabic[i];
        } else {
            i++;
        }
    }
    tmp_roman[k] = '\0';
    printf("%s", tmp_roman);
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
    char roman1[100];
    char roman2[100];

    char op;
    int result, a, b;

    while (scanf("%s %c %s", roman1, &op, roman2) == 3) {
        a = roman_to_arabic(roman1);
        b = roman_to_arabic(roman2);

        result = calc(a, b, op);

        if (result <= 0 || result > 3999) {
            printf("-");
        } else {
            print_arabic_as_roman(result);
        }
        printf("\n");
    }

    return 0;
}