#include <stdio.h>

int main() {
    char word[1000];
    char letters[26];
    char big_letters[26];
    for (int i = 0; i < 26; i++) {
        letters[i] = 'a' + i;
        big_letters[i] = 'A' + i;
    }
    int stats[26] = {0};

    int first_letter = 0;
    while (scanf("%s", word) == 1) {
        first_letter = word[0];
        for (int i = 0; i < 26; i++) {
            if (first_letter == letters[i] || first_letter == big_letters[i]) {
                stats[i]++;
            }
        }
    }
    for (int i = 0; i < 26; i++) {
        if (stats[i] != 0) {
            printf("%c %d\n", letters[i], stats[i]);
        }
    }

    return 0;
}