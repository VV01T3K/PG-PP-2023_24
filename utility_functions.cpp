// --------------------------------------------------
/** @brief Calculates the length of a string.
 * @param str Pointer to the string.
 * @return int Length of the string.*/
int strlen(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}
// --------------------------------------------------
/** @brief Reverses a string in place.
 * @param str Pointer to the string to be reversed.
 * @return char* Pointer to the reversed string.*/
char *strrev(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char tmp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = tmp;
    }
    return str;
}
// --------------------------------------------------
/** @brief Reverses a word in place.
 * @param word Array containing the word to be reversed.
 * @param size Size of the word array.*/
void reverse_word(char word[], int size) {
    for (int i = 0; i < size / 2; i++) {
        int tmp = word[i];
        word[i] = word[size - i - 1];
        word[size - i - 1] = tmp;
    }
}
// --------------------------------------------------
/** @brief Returns a reversed copy of an array.
 * @param rev_array Array to store the reversed copy.
 * @param array Original array.
 * @param size Size of the original array.
 * @return int* Pointer to the reversed array.*/
int *return_reverse_array(int rev_array[], const int array[], int size) {
    for (int i = 0; i < size; i++) {
        rev_array[i] = array[size - i - 1];
    }
    return rev_array;
}
// --------------------------------------------------
/** @brief Reverses an array in place.
 * @param array Array to be reversed.
 * @param size Size of the array.*/
void reverse_array(int array[], int size) {
    for (int i = 0; i < size / 2; i++) {
        int tmp = array[i];
        array[i] = array[size - i - 1];
        array[size - i - 1] = tmp;
    }
}

// TODO: Implement bubble sort

// TODO: Implement selection sort

// TODO: Implement insertion sort

// TODO: Implement merge sort

// TODO: Implement quick sort