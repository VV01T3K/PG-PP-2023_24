#include <iostream>
using namespace std;

int main() {
    int number = 0;
    while (cin >> number) {
        if (number == 42) break;
        cout << number << endl;
    }
    return 0;
}