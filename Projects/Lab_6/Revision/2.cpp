#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10000000

#define valid_number(x) (x >= '0' && x <= '9')

#define valid_letter(x) \
    ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') || (x == '_'))

#define whitespace(x) (x == ' ' || x == '\t' || x == '\n')

char *strrev(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char tmp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = tmp;
    }
    return str;
}

int main() {
    char c;
    int i, valid, commented = 0;
    printf(">");
    while ((c = fgetc(stdin)) != EOF) {
        // my hacky way to check for whitespace
        if (whitespace(c)) {
            printf("%c", c);
            continue;
        } else {
            ungetc(c, stdin);
        }
        // read word
        char *word = (char *)malloc(SIZE * sizeof(char));
        scanf("%s%c", word, &c);
        i = 0, valid = 1;

        // check word is valid
        int comm = 0;
        while (word[i] != '\0') {
            if (comm) {
                ungetc(word[i], stdin);
                continue;
            }
            if (word[i] == '/' && word[i + 1] == '/') {
                comm = 1;
                commented = 1;
            }
            if (i == 0 && !valid_letter(word[i]))
                valid = 0;
            else if (!valid_letter(word[i]) && !valid_number(word[i]))
                valid = 0;
            if (!valid) break;
            i++;
        }

        // check word is identifier
        if (commented == 0) {
            if (valid) {
                printf("%s", word);
            }
        } else {
            printf("%s", word);
        }

        free(word);

        // bring back whitespace character
        printf("%c", c);
        if (c == '\n') {
            c = fgetc(stdin);
            if (c == EOF) break;
            ungetc(c, stdin);
            printf(">");
            commented = 0;
        }
    }

    return 0;
}