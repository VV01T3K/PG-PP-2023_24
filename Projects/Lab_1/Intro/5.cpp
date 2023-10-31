#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    int n;

    cin >> n;

    double tmp_number, avg, sum = 0;

    for (int i = 0; i < n; i++) {
        cin >> tmp_number;
        sum += tmp_number;
    }

    avg = sum / n;

    cout << fixed << setprecision(2) << avg << endl;

    // or
    // printf("%.2f\n", avg);

    return 0;
}
