#include <stdio.h>
#include <string.h>

void strrev(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

int main() {
    char str[] = "Hello world";  // initializing a char array

    printf("The string is : %s\n", str);  // printing the actual array

    strrev(str);  // reversing the char array

    printf("The string after using function strrev() is : %s\n",
           str);  // printing the reversed array
    return 0;
}