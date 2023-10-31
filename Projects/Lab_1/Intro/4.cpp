#include <iostream>

using namespace std;

int main() {
    int n;

    cin >> n;

    int positive_sum = 0, negative_sum = 0;

    int tmp_number;

    for (int i = 0; i < n; i++) {
        cin >> tmp_number;
        if (tmp_number >= 0) {
            positive_sum += tmp_number;
        } else {
            negative_sum += tmp_number;
        }
    }

    cout << positive_sum << " " << negative_sum << endl;

    return 0;
}
