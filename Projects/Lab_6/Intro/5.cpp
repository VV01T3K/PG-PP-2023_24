#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main() {
    int occurence_even[26] = {0}, occurence_odd[26] = {0};
    char word[100];
    int i, len;
    int max_even = 0, max_odd = 0;

    while (scanf("%s", word) != EOF) {
        len = strlen(word);
        if (len % 2 == 0) {
            for (i = 0; i < len; i++) {
                if (islower(word[i])) {
                    occurence_even[word[i] - 'a']++;
                }
            }
        } else {
            for (i = 0; i < len; i++) {
                if (isupper(word[i])) {
                    occurence_odd[word[i] - 'A']++;
                }
            }
        }
    }

    for (i = 0; i < 26; i++) {
        if (occurence_even[i] > max_even) {
            max_even = occurence_even[i];
        }
        if (occurence_odd[i] > max_odd) {
            max_odd = occurence_odd[i];
        }
    }

    printf("%d %d\n", max_even, max_odd);

    return 0;
}