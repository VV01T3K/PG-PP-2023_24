#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main() {
    int n;
    char word[1000000];
    char lowerCase[1000000];
    scanf("%d", &n);
    getchar();

    while (fgets(word, 1000000, stdin)) {
        int word_length = strlen(word);
        int lower_case_len = 0;
        for (int i = 0; i < word_length; i++) {
            if (islower(word[i])) {
                lowerCase[lower_case_len++] = word[i];
            }
        }
        for (int i = 0; i < word_length; i++) {
            if (islower(word[i])) {
                word[i] = lowerCase[--lower_case_len];
            }
        }
        printf("%s", word);
    }
    printf("\n");

    return 0;
}