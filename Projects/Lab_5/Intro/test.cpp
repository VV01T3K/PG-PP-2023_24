#include <stdio.h>
#include <string.h>

#define listS 500
#define wordS 1000

int main() {
    char c;
    char word_list[listS][wordS] = {};
    char exist_list[listS] = {0};
    int list_len = 0;
    char array[200][200] = {0};
    int row = 0, col, k;
    char word[wordS] = {0};
    // pobranie listy słów
    do {
        scanf("%s%c", word_list[list_len], &c);
        list_len++;
    } while (c != '\n');

    // TODO - dodać opcje różno wielkościowych arrayów
    // pobranie arraya liter
    // for (row = 0, col = 0; row < rowS;
    //      col++, (col == colS) ? col = 0, row++ : 0) {
    //     scanf("%c", &c);
    //     if (c == '\n') scanf("%c", &c);
    //     array[row][col] = c;
    // }
    int rowS = 0, colS = 0;

    while (scanf("%s", word) == 1) {
        rowS++;
    }
    colS = strlen(word);

    printf("%d %d\n", rowS, colS);
    return 0;
}