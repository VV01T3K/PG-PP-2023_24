#include <stdio.h>

int main() {
    int n, tmp;
    scanf("%d", &n);

    double avg = 0;

    for (int i = 0; i < n; i++) {
        scanf("%d", &tmp);
        avg += tmp;
    }
    avg /= n;
    rewind(stdin);
    scanf("%d", &n);
    int closest = 1000;
    for (int i = 0; i < n; i++) {
        scanf("%d", &tmp);
        if (tmp >= avg && tmp < closest) {
            closest = tmp;
        }
    }
    rewind(stdin);
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &tmp);
        if (tmp == closest) {
            printf("%d ", i + 1);
        }
    }
    return 0;
}