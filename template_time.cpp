#include <stdio.h>

#define TIME 0
#if TIME
#include <time.h>
clock_t begin = clock();
#endif

int main() {
    // ----------------------------------------------------------------
    // code
    // code
    // code
    // ----------------------------------------------------------------
#if TIME
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time spent: %f\n", time_spent);
#endif
    return 0;
}