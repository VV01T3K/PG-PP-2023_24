#include <stdio.h>

int main() {
    int n, tmp;
    double avg = 0, var = 0;

    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &tmp);
        avg += tmp;
    }
    avg /= n;
    rewind(stdin);
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        scanf("%d", &tmp);
        var += (tmp - avg) * (tmp - avg);
    }
    var /= n;

    printf("%.2f", var);

    return 0;
}