#include <stdio.h>

int main() {
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char cipher[28];
    scanf("%s", cipher);

    getchar();
    char letter;
    while ((letter = getchar()) != EOF) {
        for (int i = 0; i < 28; i++) {
            if (letter == alphabet[i]) {
                letter = cipher[i];
                break;
            }
        }
        printf("%c", letter);
    }

    return 0;
}