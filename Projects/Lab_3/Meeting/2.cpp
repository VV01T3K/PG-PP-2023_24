#include <stdio.h>

#define WHITE(x) (x == ' ' || x == '\n' || x == '\t')

int main() {
    int c, nl = 0;

    int cw = 0;
    int B = 0;  // 0 - stan słowa | 1 - białe znaki

    c = fgetc(stdin);
    if (WHITE(c)) {
        /* code */
    } else {
        B = 0;
        cw++;
    }

    while ((c = fgetc(stdin)) != EOF) {
        // if (c == '\n') {
        //     nl++;
        // }
        if (WHITE(c)) {
            if (B == 0) B = 1;
            // B == 1 ==> nic nie zmieniamy
        } else {
            if (B == 1) {
                B = 0;
                cw++;
            }
        }
    }
    printf("%d\n", cw);
    return 0;
}
