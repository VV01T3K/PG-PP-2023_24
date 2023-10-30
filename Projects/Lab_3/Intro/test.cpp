#include <ctype.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 1000

int main() {
    char line[MAX_LINE_LENGTH];
    int num_lines = 0, num_words = 0, num_chars = 0;
    while (fgets(line, MAX_LINE_LENGTH, stdin)) {
        num_lines++;
        int i = 0;
        while (line[i] != '\0') {
            if (isalpha(line[i])) {
                num_chars++;
                while (isalpha(line[i])) {
                    i++;
                }
                num_words++;
            } else {
                num_chars++;
                i++;
            }
        }
    }
    printf("%d %d %d\n", num_lines, num_words, num_chars);
    return 0;
}
