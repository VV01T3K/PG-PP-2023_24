#include <stdio.h>
#include <string.h>

#define valid_number(x) (x >= '0' && x <= '9')

#define valid_letter(x) \
    ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') || (x == '_'))

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
    char word[500];
    char c;
    int i, valid;
    while (scanf("%s%c", word, &c) != EOF) {
        i = 0, valid = 1;

        // check word is valid
        while (word[i] != '\0') {
            if (i == 0 && !valid_letter(word[i]))
                valid = 0;
            else if (!valid_letter(word[i]) && !valid_number(word[i]))
                valid = 0;
            if (!valid) break;
            i++;
        }

        // check word is identifier
        if (valid) {
            printf("%s", strrev(word));
        } else {
            printf("%s", word);
        }
        // bring back whitespace character
        printf("%c", c);
    }

    return 0;
}