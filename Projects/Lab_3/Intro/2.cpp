#include <ctype.h>
#include <stdio.h>

int main() {
    char line[1000000];
    int words, ints, i;
    while (fgets(line, 1000000, stdin)) {
        words = 0;
        ints = 0;
        i = 0;
        while (line[i] != '\0') {
            if (isdigit(line[i])) {
                ints++;
                while (isdigit(line[i])) {
                    i++;
                }
            } else if (isalpha(line[i])) {
                words++;
                while (isalpha(line[i])) {
                    i++;
                }
            } else {
                i++;
            }
        }
        printf("%d %d", ints, words);
        printf("\n");
    }

    return 0;
}