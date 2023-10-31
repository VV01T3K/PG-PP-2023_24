#include <stdio.h>

int main() {
    int n;
    scanf("%d", &n);

    int max_width = 2 * n;

    for (int segment = 1; segment <= n; segment++) {
        int width = 2 * segment;

        for (int row = 0; row < width; row++) {
            for (int i = 0; i < max_width - width; i++) {
                printf(" ");
            }
            if (row == width - 1) {
                for (int i = 0; i < width; i++) {
                    printf("^");
                }
                for (int i = 0; i < width; i++) {
                    printf("^");
                }
                // printf("\n");
            } else {
                for (int i = 1; i < width - row; i++) {
                    printf(" ");
                }
                printf("^");
                for (int i = 0; i < row - 1; i++) {
                    printf(" ");
                }
                if (row != 0) {
                    printf("^");
                }

                for (int i = 0; i < row; i++) {
                    if (i == 0) {
                        printf("^");
                    } else {
                        printf(" ");
                    }
                }
                printf("^");
            }

            for (int i = 1; i < width - row; i++) {
                printf(" ");
            }
            // if (segment == n && row == width - 1) {
            //     printf(" ");
            //     break;
            // }
            printf("\n");
        }
    }

    return 0;
}