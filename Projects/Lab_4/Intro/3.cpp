#include <stdio.h>

int main() {
    int n1, n2;
    scanf("%d %d", &n1, &n2);

    int arr1[120], arr2[120], merged[250];

    // pobranie arrayów
    for (int i = 0; i < n1; i++) {
        scanf("%d", &arr1[i]);
    }
    for (int i = 0; i < n2; i++) {
        scanf("%d", &arr2[i]);
    }

    // dodawanie najmniejszego elementu z każdego arraya po kolei
    int i, j, a;
    while (i < n1 && j < n2) {
        if (arr1[i] < arr2[j]) {
            merged[a++] = arr1[i++];
        } else {
            merged[a++] = arr2[j++];
        }
    }

    // dodanie reszty elementów z arraya, który nie został w całości dodany
    while (i < n1) {
        merged[a++] = arr1[i++];
    }
    while (j < n2) {
        merged[a++] = arr2[j++];
    }

    // wypisanie wyniku
    for (int i = 0; i < a; i++) {
        printf("%d ", merged[i]);
    }

    return 0;
}