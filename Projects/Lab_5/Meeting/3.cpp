#include <stdio.h>

// void swap(int x, int y) {
//     int tmp = x;
//     x = y;
//     y = tmp;
// }
void swap(int* x, int* y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}
// c++ --------------
// int& alias
// void swap(int& x, int& y) {
//     int tmp = x;
//     x = y;
//     y = tmp;
// }
// c++ --------------

int main() {
    int a = 3, b = 2;
    int tmp;

    printf("a = %d, b = %d\n", a, b);

    // swap(a, b);
    swap(&a, &b);

    printf("a = %d, b = %d\n", a, b);

    return 0;
}