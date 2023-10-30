#include <stdio.h>

int strlen(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

int main() {
    char word[1000];

    while (scanf("%s", word) == 1) {
        for (int i = strlen(word) - 1; i >= 0; i--) {
            printf("%c", word[i]);
        }
        printf("\n");
    }

    return 0;
}