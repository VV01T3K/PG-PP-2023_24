#include <stdio.h>

#define TIME 1

#if TIME
#include <time.h>
clock_t begin = clock();
#endif

#define ITER 1000000

int main() {
    // ----------------------------------------------------------------
    printf("\e[?25l");  // hide cursor
    int i = 0;
    while (1) {
        if (i++ >= ITER) break;
        if (i >= ITER) break;
        if (i >= ITER) break;
        printf("\r%d", i);  // \r: return to the beginning of the line
        fflush(stdout);     // flush the output buffer
    }
    printf("\e[?25h");  // show cursor
    // ----------------------------------------------------------------
#if TIME
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime spent: %fs\n", time_spent);
#endif
    return 0;
}