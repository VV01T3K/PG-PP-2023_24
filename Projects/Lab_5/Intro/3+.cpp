#include <stdio.h>
#include <string.h>

#define listS 500
#define wordS 1000
char *strrev(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
    return str;
}

void check_word(char *word, char word_list[listS][wordS], int list_len,
                char *exist_list) {
    for (int i = 0; i < list_len; i++) {
        if (strstr(word, word_list[i]) != NULL ||
            strstr(strrev(word), word_list[i]) != NULL) {
            exist_list[i] = 1;
        }
    }
}

int main() {
    char c;
    char word_list[listS][wordS] = {0};
    char exist_list[listS] = {0};
    int list_len = 0;
    char array[200][200] = {0};
    int row = 0, col, k;
    char word[wordS] = {0};

    // ustalenie wielkości arraya
    int rowS = 0, colS = 0;
    fgets(word, wordS, stdin);
    while (scanf("%s", word) == 1) {
        rowS++;
    }
    colS = strlen(word);
    rewind(stdin);

    // pobranie listy słów
    do {
        scanf("%s%c", word_list[list_len], &c);
        list_len++;
    } while (c != '\n');

    // pobranie arraya liter

    for (row = 0; row < rowS; row++) {
        for (col = 0; col < colS; col++) {
            scanf("%c", &c);
            if (c == '\n') scanf("%c", &c);
            array[row][col] = c;
        }
    }

    // ↓ ↑
    for (col = 0; col < colS; col++) {
        for (row = 0; row < rowS; row++) {
            word[row] = array[row][col];
        }
        word[row] = '\0';
        check_word(word, word_list, list_len, exist_list);
    }

    // → ←
    for (int i = 0; i < row; i++) {
        // strcpy(word, array[i]);
        check_word(array[i], word_list, list_len, exist_list);
    }

    // wypisanie rezultatów
    for (int i = 0; i < list_len; i++) {
        if (exist_list[i]) {
            printf("%s ", word_list[i]);
        }
    }

    return 0;
}