#include <stdio.h>

#define valid_number(x) (x >= '0' && x <= '9')

#define valid_letter(x) \
    ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') || (x == '_'))

int main() {
    char word[200];
    char c;
    int identifiers = 0;
    int i, valid;
    while (scanf("%s%c", word, &c) != EOF) {
        i = 0, valid = 1;
        // check if word is valid
        while (word[i] != '\0') {
            if (i == 0 && !valid_letter(word[i]))
                valid = 0;
            else if (!valid_letter(word[i]) && !valid_number(word[i]))
                valid = 0;
            if (!valid) break;
            i++;
        }
        // check if word is identifier
        if (valid) identifiers++;

        // check if c is newline
        if (c == '\n') {
            printf("%d\n", identifiers);
            identifiers = 0;
        }
    }

    return 0;
}