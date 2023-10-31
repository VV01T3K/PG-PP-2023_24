#include <iostream>
using namespace std;

int main() {
    int numbers[3] = {};
    int tmp_number;

    cin >> numbers[0] >> numbers[1] >> numbers[2];

    int arr_length = sizeof(numbers) / sizeof(numbers[0]);

    for (int i = 0; i < arr_length; i++) {
        for (int j = 0; j < arr_length; j++) {
            if (numbers[i] < numbers[j]) {
                tmp_number = numbers[i];
                numbers[i] = numbers[j];
                numbers[j] = tmp_number;
            }
        }
    }

    cout << numbers[0] << endl << numbers[1] << endl << numbers[2] << endl;

    return 0;
}