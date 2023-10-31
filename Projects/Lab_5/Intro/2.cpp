#include <stdio.h>
#include <string.h>

int convRom(char roman) {
    switch (roman) {
        case 'I':
            return 1;
            break;
        case 'V':
            return 5;
            break;
        case 'X':
            return 10;
            break;
        case 'L':
            return 50;
            break;
        case 'C':
            return 100;
            break;
        case 'D':
            return 500;
            break;
        case 'M':
            return 1000;
            break;
    }
    return -1;
}

int roman_to_arabic(char *roman) {
    int flag = 1;
    int result = 0;
    for (int i = strlen(roman) - 1; i >= 0; i--) {
        if (flag) {
            result += convRom(roman[i]);
        } else {
            result -= convRom(roman[i]);
        }
        if (convRom(roman[i]) > convRom(roman[i - 1])) {
            flag = 0;
        }
    }
    return result;
}

void print_arabic_as_roman(int num) {
    int arabic[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const char *roman[] = {"M",  "CM", "D",  "CD", "C",  "XC", "L",
                           "XL", "X",  "IX", "V",  "IV", "I"};
    int i = 0;
    while (num > 0) {
        if (num >= arabic[i]) {
            printf("%s", roman[i]);
            num -= arabic[i];
        } else {
            i++;
        }
    }
}
int calc(int a, int b, char op) {
    switch (op) {
        case '+':
            return a + b;
            break;
        case '-':
            return a - b;
            break;
        case '*':
            return a * b;
            break;
        case '/':
            return a / b;
            break;
    }
    return 0;
}

int main() {
    char roman[100];
    char op;
    int result, a, b;
    int next = 1;

    while (next) {
        scanf("%s", roman);
        a = roman_to_arabic(roman);
        scanf(" %c", &op);
        scanf("%s", roman);
        b = roman_to_arabic(roman);

        result = calc(a, b, op);

        if (result < 0 || result > 3999) {
            printf("-");
        } else {
            print_arabic_as_roman(result);
        }
        printf("\n");

        if (fgetc(stdin) == EOF) {
            next = 0;
        }
    }

    return 0;
}