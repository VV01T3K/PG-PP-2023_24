#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 150
#define MAX_PEOPLE 1000

// diff plik_1 plik_2

struct PERSON {
    char name[MAX_NAME_LEN];
    int age;
};

int porownanieWiek(const void* a, const void* b) {
    // rosnaco
    return ((struct PERSON*)a)->age - ((struct PERSON*)b)->age;
    // malejaco
    // return  ((struct PERSON*)b)->age - ((struct PERSON*)a)->age;
}

int porownanieImie(const void* a, const void* b) {
    // rosnaco
    return strcmp(((struct PERSON*)a)->name, ((struct PERSON*)b)->name);
    // malejaco
    // return strcmp(((struct PERSON*)b)->name, ((struct PERSON*)a)->name);
}

int porownanie(const void* a, const void* b) {
    int wynik = strcmp(((struct PERSON*)a)->name, ((struct PERSON*)b)->name);
    if (wynik == 0) {
        return ((struct PERSON*)a)->age - ((struct PERSON*)b)->age;
    }
    return wynik;
}

int wczytaj(struct PERSON* people) {
    int count = 0;
    while (scanf("%s %d", people[count].name, &people[count].age) == 2) {
        count++;
    }
    return count;
}

void wypisz(struct PERSON* people, int count) {
    for (int i = 0; i < count; i++) {
        printf("%s %d\n", people[i].name, people[i].age);
    }
}

int main() {
    struct PERSON people[MAX_PEOPLE];

    int count = wczytaj(people);

    // qsort(people, count, sizeof(struct PERSON), porownanieImie);
    // qsort(people, count, sizeof(struct PERSON), porownanieWiek);
    qsort(people, count, sizeof(struct PERSON), porownanie);

    wypisz(people, count);

    return 0;
}