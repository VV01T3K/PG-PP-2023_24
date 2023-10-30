#include <stdio.h>

// from 33 till 126 ASCII
// https://www.asciitable.com/

#define valid_char(x)                                   \
    (x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') || \
        (x >= '0' && x <= '9') || (x == '_')

int main() {
    char c;
    int words = 0;
    int word_state = 0;

    while ((c = fgetc(stdin)) != EOF) {
        if (c == 'w') {
            word_state = 1;
        } else {
            word_state = 0;
        }
    }
    printf("%d\n", words);

    return 0;
}