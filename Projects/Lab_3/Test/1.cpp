#include <stdio.h>

int print_flag(int n, int flag[1000][1000]) {
    for (int i = 0; i < n / 2; i++) {
        for (int j = 0; j < n; j++) {
            if (flag[i][j] == 1) {
                printf("*");
            } else if (flag[i][j] == 2) {
                printf(" ");
            } else if (flag[i][j] == 3) {
                printf("x");
            }
        }
        printf("\n");
    }
    return 0;
}

int main() {
    int k, s, n;
    scanf("%d", &k);
    scanf("%d", &s);
    n = (4 * k) + 14;
    int flag[1000][1000] = {0};

    for (int row = 0; row < n / 2; row++) {
        for (int col = 0; col < n; col++) {
            flag[row][col] = 2;
            flag[0][col] = 1;
            flag[n / 2 - 1][col] = 1;
            flag[row][0] = 1;
            flag[row][n - 1] = 1;
            flag[row][n / 2] = 1;
            flag[row][n / 2 - 1] = 1;
        }
    }

    flag[n / 4][n / 4] = 1;
    flag[n / 4 - 1][n / 4] = 1;
    flag[n / 4 + 1][n / 4] = 1;
    flag[n / 4][n / 4 - 1] = 1;
    flag[n / 4][n / 4 + 1] = 1;

    int center = n - (n / 4) - 1;
    flag[n / 4][center] = 1;
    flag[n / 4 + 1][center] = 1;
    flag[n / 4 - 1][center] = 1;
    flag[n / 4][center + 1] = 1;
    flag[n / 4][center - 1] = 1;

    switch (s) {
        case 1:
            flag[n / 4][n / 4] = 2;
            flag[n / 4 - 1][n / 4] = 2;
            flag[n / 4 + 1][n / 4] = 2;
            flag[n / 4][n / 4 - 1] = 2;
            flag[n / 4][n / 4 + 1] = 2;
            print_flag(n, flag);
            flag[n / 4][n / 4] = 1;
            flag[n / 4 - 1][n / 4] = 1;
            flag[n / 4 + 1][n / 4] = 1;
            flag[n / 4][n / 4 - 1] = 1;
            flag[n / 4][n / 4 + 1] = 1;
            break;
        case 2:
            flag[n / 4][center] = 2;
            flag[n / 4 + 1][center] = 2;
            flag[n / 4 - 1][center] = 2;
            flag[n / 4][center + 1] = 2;
            flag[n / 4][center - 1] = 2;
            print_flag(n, flag);
            center = n - (n / 4) - 1;
            flag[n / 4][center] = 1;
            flag[n / 4 + 1][center] = 1;
            flag[n / 4 - 1][center] = 1;
            flag[n / 4][center + 1] = 1;
            flag[n / 4][center - 1] = 1;
            break;
        case 3:
            print_flag(n, flag);
            flag[n / 4][n / 4] = 2;
            flag[n / 4 - 1][n / 4] = 2;
            flag[n / 4 + 1][n / 4] = 2;
            flag[n / 4][n / 4 - 1] = 2;
            flag[n / 4][n / 4 + 1] = 2;
            break;
        case 4:
            print_flag(n, flag);
            flag[n / 4][center] = 2;
            flag[n / 4 + 1][center] = 2;
            flag[n / 4 - 1][center] = 2;
            flag[n / 4][center + 1] = 2;
            flag[n / 4][center - 1] = 2;
            break;
    }
    print_flag(n, flag);

    return 0;
}
