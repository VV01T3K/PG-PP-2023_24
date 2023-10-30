#include <stdio.h>

int main() {
    int n, l, u, tmp, sub_len = 0;
    int seqence[150] = {0};
    int sub_seqence[150] = {0};

    scanf("%d %d %d", &n, &l, &u);

    for (int i = 0; i < n; i++) {
        scanf("%d", &tmp);
        if (tmp >= l && tmp <= u) {
            sub_seqence[sub_len++] = tmp;
        }
        seqence[i] = tmp;
    }

    for (int i = 0; i < n; i++) {
        if (seqence[i] >= l && seqence[i] <= u) {
            printf("%d ", sub_seqence[--sub_len]);
        } else {
            printf("%d ", seqence[i]);
        }
    }

    return 0;
}