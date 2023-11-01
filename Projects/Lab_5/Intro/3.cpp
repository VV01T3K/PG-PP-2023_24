#include <stdio.h>
int main() {
    char c;
    char list[200][500] = {};
    char array[3][4] = {0};

    int row = 0, col;
    do {
        scanf("%s%c", list[row], &c);
        row++;
    } while (c != '\n');

    for (row = 0, col = 0; row < 3; col++, (col == 4) ? col = 0, row++ : 0) {
        scanf("%c", &c);
        if (c == '\n') scanf("%c", &c);
        array[row][col] = c;
    }

    // →
    // for (row = 0, col = 0; row < 3; (col == 4) ? col = 0, row++ : 0) {
    //     while (col < 4) {
    //         printf("%c", array[row][col++]);
    //     }
    //     printf("\n");
    // }

    // ←
    // for (row = 0, col = 3; row < 3; (col == -1) ? col = 3, row++ : 0) {
    //     while (col >= 0) {
    //         printf("%c", array[row][col--]);
    //     }
    //     printf("\n");
    // }

    // ↓
    // for (row = 0, col = 0; col < 4; (row == 3) ? row = 0, col++ : 0) {
    //     while (row < 3) {
    //         printf("%c", array[row++][col]);
    //     }
    //     printf("\n");
    // }

    // ↑
    // for (row = 2, col = 0; col < 4; (row == -1) ? row = 2, col++ : 0) {
    //     while (row >= 0) {
    //         printf("%c", array[row--][col]);
    //     }
    //     printf("\n");
    // }

    return 0;
}