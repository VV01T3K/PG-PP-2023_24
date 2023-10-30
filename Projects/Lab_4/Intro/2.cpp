#include <stdio.h>

// from 33 till 126 ASCII
// https://www.asciitable.com/

#define wSpace(x) (x == ' ' || x == '\n' || x == '\t')

#define valid_char(x)                                    \
    ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') || \
     (x >= '0' && x <= '9') || (x == '_'))

#define valid_first_char(x) \
    ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') || (x == '_'))

int main() {
    int c;

    int words = 0;
    int word_state = 1;
    int valid = 1;
    int first_char = 1;

    while ((c = fgetc(stdin)) != EOF) {
        if (wSpace(c)) {
            if (word_state == 1) {
                if (valid == 1) {
                    words++;
                }
                valid = 1;
                first_char = 1;
            }
            word_state = 0;
        } else {
            word_state = 1;
            if (first_char) {
                first_char = 0;
                if (!valid_first_char(c)) {
                    valid = 0;
                }
            }

            if (!valid_char(c)) {
                valid = 0;
            }
        }
        if (c == '\n') {
            printf("%d\n", words);
            words = 0;
        }
    }
    return 0;
}
