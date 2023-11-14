#include <iostream>
using namespace std;

int count(int *a, int n) {
    if (n > 1)
        return count(a + 1, n - 1) + (a[0] > a[1] ? 1 : 0);
    else
        return 0;
}
int main() {
    int x[] = {3, 5, 8, 4, 6, 3, 3, 9, 6, 3, 7, 6, 1};
    cout << count(x, (sizeof x) / (sizeof x[1]));
    cout << count(&(x[2]), 7);
    return 0;
}