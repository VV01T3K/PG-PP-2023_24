#include <stdio.h>
#include <string.h>

#define listS 500
#define wordS 1000

void check_word(char *word, char word_list[listS][wordS], int list_len,
                char *exist_list) {
    for (int i = 0; i < list_len; i++) {
        if (strstr(word, word_list[i]) != NULL) {
            exist_list[i] = 1;
        }
    }
}

int main() {
    char c;
    char word_list[listS][wordS] = {};
    char exist_list[listS] = {0};
    int list_len = 0;
    char array[200][200] = {0};
    int row = 0, col, k;
    char word[wordS] = {0};

    // ustalenie wielkości arraya
    int rowS = 0, colS = 0;
    do {
        scanf("%s%c", word_list[list_len], &c);
    } while (c != '\n');
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
    for (row = 0, col = 0; row < rowS;
         col++, (col == colS) ? col = 0, row++ : 0) {
        scanf("%c", &c);
        if (c == '\n') scanf("%c", &c);
        array[row][col] = c;
    }

    // →
    for (row = 0, col = 0; row < rowS; (col == colS) ? col = 0, row++ : 0) {
        k = 0;
        while (col < colS) {
            word[k++] = array[row][col++];
        }
        word[k] = '\0';
        check_word(word, word_list, list_len, exist_list);
    }

    // ←
    for (row = 0, col = colS - 1; row < rowS;
         (col == -1) ? col = colS - 1, row++ : 0) {
        k = 0;
        while (col >= 0) {
            word[k++] = array[row][col--];
        }
        word[k] = '\0';
        check_word(word, word_list, list_len, exist_list);
    }

    // ↓
    for (row = 0, col = 0; col < colS; (row == rowS) ? row = 0, col++ : 0) {
        k = 0;
        while (row < rowS) {
            word[k++] = array[row++][col];
        }
        word[k] = '\0';
        check_word(word, word_list, list_len, exist_list);
    }

    // ↑
    for (row = rowS - 1, col = 0; col < colS;
         (row == -1) ? row = rowS - 1, col++ : 0) {
        k = 0;
        while (row >= 0) {
            word[k++] = array[row--][col];
        }
        word[k] = '\0';
        check_word(word, word_list, list_len, exist_list);
    }

    for (int i = 0; i < list_len; i++) {
        if (exist_list[i]) {
            printf("%s ", word_list[i]);
        }
    }

    return 0;
}