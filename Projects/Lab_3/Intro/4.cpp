#include <ctype.h>
#include <stdio.h>

int main() {
    int lines = 0, words = 0, chars = 0;
    char letter;
    while ((letter = getchar()) != EOF) {
        if (isalpha(letter)) {
            while (isalpha(letter) && letter != '.') {
                letter = getchar();
                chars++;
            }
            words++;
        }
        chars++;
    }
    rewind(stdin);
    char line[1000000];
    while (fgets(line, 1000000, stdin)) {
        lines++;
    }
    printf("%d %d %d", lines, words, chars);

    return 0;
}