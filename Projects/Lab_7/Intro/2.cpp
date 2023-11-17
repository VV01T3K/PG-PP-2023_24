#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM_LENGTH 10000000000

struct Number {
    char num[MAX_NUM_LENGTH];
    int size;
    int baseX;
    int baseY;
    long long value;
};

void readLine(struct Number* num) {
    scanf("%s %d %d", num->num, &num->baseX, &num->baseY);
    num->size = strlen(num->num);
    num->value = 0;
}

void revStr(struct Number* num) {
    int j = 0;
    char tmp;
    while (j < num->size / 2) {
        tmp = num->num[j];
        num->num[j] = num->num[num->size - j - 1];
        num->num[num->size - j - 1] = tmp;
        j++;
    }
}
void _baseXto10(struct Number* num) {
    if (num->baseX == 10) {
        num->value = atoi(num->num);
        return;
    }
    for (int i = num->size - 1; i >= 0; i--) {
        if (num->num[i] <= '9') {
            num->value +=
                (num->num[i] - '0') * pow(num->baseX, num->size - i - 1);
        } else {
            num->value +=
                (num->num[i] - 'A' + 10) * pow(num->baseX, num->size - i - 1);
        }
    }
}

void _10tobaseY(struct Number* num) {
    int i = 0;
    while (num->value > 0) {
        if (num->value % num->baseY <= 9) {
            num->num[i] = num->value % num->baseY + '0';
        } else {
            num->num[i] = num->value % num->baseY - 10 + 'A';
        }
        num->value /= num->baseY;
        i++;
    }
    num->num[i] = '\0';
    num->size = i;
    revStr(num);
}

struct Number num;
int main() {
    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        readLine(&num);

        _baseXto10(&num);

        if (num.baseY == 10) {
            printf("%lld\n", num.value);
            continue;
        }

        _10tobaseY(&num);

        printf("%s\n", num.num);
    }

    return 0;
}