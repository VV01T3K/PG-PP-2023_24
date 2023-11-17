#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 1001
#define MAX_WORD_LENGTH 1001

struct WORD {
    char word[MAX_WORD_LENGTH];
    int count;
};

int main() {
    struct WORD words[MAX_WORDS] = {0};
    char tmp_word[MAX_WORD_LENGTH];

    int end = 0;
    while (scanf("%s", tmp_word) == 1) {
        int found = 0;
        for (int i = 0; i < end; i++) {
            if (strcmp(words[i].word, tmp_word) == 0) {
                words[i].count++;
                found = 1;
            }
        }
        if (!found) {
            strcpy(words[end].word, tmp_word);
            words[end].count = 1;
            end++;
        }
    }

    int max = 0;
    for (int i = 0; i < end; i++) {
        if (strlen(words[i].word) % 2 == 0 && words[i].count > max) {
            max = words[i].count;
        }
    }
    printf("%d\n", max);

    return 0;
}