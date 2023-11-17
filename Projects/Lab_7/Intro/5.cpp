#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

void readLine(char *str) {
    fgets(str, MAX, stdin);
    str[strlen(str) - 1] = '\0';
}
int calcSide(char *str);
void spiltSides(char *str, char *left, char *right);
int compareSides(char *str);
void placeSigns(char *str, char *signs, char *signed_str);
int countSpaces(char *str);
void printResult(char *str, int len) {
    for (int i = 0; i < len; i++) {
        printf("%c", str[i]);
    }
    printf("\n");
}

int main() {
    int T;
    scanf("%d", &T);

    for (int t = 0; t < T + 1; t++) {
        char str[MAX];
        readLine(str);

        char signs[MAX];

        int space = countSpaces(str);

        for (int i = 0; i < space; i++) {
            for (int j = 0; j < space; j++) {
                if (j == i) {
                    signs[j] = '=';
                } else {
                    signs[j] = '+';
                }
            }
            char signed_str[MAX];
            placeSigns(str, signs, signed_str);

            if (compareSides(signed_str)) {
                printResult(signed_str, strlen(str));
            }

            for (int j = 0; j < space; j++) {
                if (j != i) {
                    signs[j] = '-';
                    char signed_str[MAX];
                    placeSigns(str, signs, signed_str);

                    if (compareSides(signed_str)) {
                        printResult(signed_str, strlen(str));
                    }
                    signs[j] = '+';
                }
            }
        }
    }

    return 0;
}

int calcSide(char *str) {
    int result = atoi(&str[0]);
    int i = 1;
    while (str[i] != '\0') {
        if (isdigit(str[i])) {
            switch (str[i - 1]) {
                case '+':
                    result += atoi(&str[i]);
                    break;
                case '-':
                    result -= atoi(&str[i]);
                    break;
            }
        }
        i++;
    }
    return result;
}

void spiltSides(char *str, char *left, char *right) {
    int i = 0, j = 0;
    while (str[i] != '=') {
        left[j++] = str[i];
        i++;
    }
    left[j] = '\0';

    i++;
    j = 0;

    while (str[i] != '\0') {
        right[j++] = str[i];
        i++;
    }
    right[j] = '\0';
}
int compareSides(char *str) {
    char left[MAX], right[MAX];

    spiltSides(str, left, right);

    return calcSide(left) == calcSide(right);
}
void placeSigns(char *str, char *signs, char *signed_str) {
    int i = 0, j = 0;
    while (str[i] != '\0') {
        if (str[i] == ' ') {
            signed_str[i] = signs[j++];
        } else {
            signed_str[i] = str[i];
        }
        i++;
    }
}
int countSpaces(char *str) {
    int i = 0, count = 0;
    while (str[i] != '\0') {
        if (str[i] == ' ') {
            count++;
        }
        i++;
    }
    return count;
}