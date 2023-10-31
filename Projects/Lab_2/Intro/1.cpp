#include <iostream>
using namespace std;

int print_flag(int n, int flag[100][100]) {
    for (int i = 0; i < n / 2; i++) {
        for (int j = 0; j < n; j++) {
            if (flag[i][j] == 1) {
                cout << "*";
            } else if (flag[i][j] == 2) {
                cout << " ";
            } else if (flag[i][j] == 3) {
                cout << "x";
            }
        }
        cout << endl;
    }
    return 0;
}

int main() {
    int n, k;
    cin >> k;
    n = (4 * k) + 14;
    int flag[100][100] = {0};

    for (int row = 0; row < n / 2; row++) {
        for (int col = 0; col < n; col++) {
            flag[row][col] = 2;
            flag[0][col] = 1;
            flag[n / 2 - 1][col] = 1;
            flag[row][0] = 1;
            flag[row][n - 1] = 1;
            flag[row][n / 2] = 1;
            flag[row][n / 2 - 1] = 1;
        }
    }

    flag[n / 4][n / 4] = 1;
    flag[n / 4 - 1][n / 4] = 1;
    flag[n / 4 + 1][n / 4] = 1;
    flag[n / 4][n / 4 - 1] = 1;
    flag[n / 4][n / 4 + 1] = 1;

    int center = n - (n / 4) - 1;
    flag[n / 4][center] = 1;
    flag[n / 4 + 1][center] = 1;
    flag[n / 4 - 1][center] = 1;
    flag[n / 4][center + 1] = 1;
    flag[n / 4][center - 1] = 1;

    print_flag(n, flag);
    print_flag(n, flag);

    return 0;
}