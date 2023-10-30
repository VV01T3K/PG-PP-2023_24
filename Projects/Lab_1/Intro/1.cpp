#include <stdio.h>

int main() {
    int n, tmp;
    scanf("%d\n", &n);
    int min = 1000, max = -1000;

    for (int i = 0; i < n; i++) {
        scanf("%d", &tmp);
        if (tmp < min) min = tmp;
        if (tmp > max) max = tmp;
    }

    rewind(stdin);

    scanf("%d\n", &n);

    for (int i = 0; i < n; i++) {
        scanf("%d", &tmp);
        if (tmp == min) {
            tmp = max;
        } else {
            if (tmp == max) tmp = min;
        }

        printf("%d ", tmp);
    }

    return 0;
}