#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void sieve_of_eratosthenes(int n) {
    if (n < 2) {
        printf("Simple digits until %d not found\n", n);
        return;
    }

    // Создаем массив и инициализируем его
    int* P = (int*)malloc((n + 1) * sizeof(int));
    if (P == NULL) {
        printf("Memory allocation error\n");
        return;
    }

    // Инициализация массива: 1 - простое, 0 - составное
    for (int i = 2; i <= n; i++) {
        P[i] = 1;
    }

    // Просеивание
    for (int p = 2; p * p <= n; p++) {
        if (P[p] == 1) {
            for (int i = p * p; i <= n; i += p) {
                P[i] = 0;
            }
        }
    }

    // Вывод простых чисел
    printf("Simple digits until: %d:\n", n);
    for (int i = 2; i <= n; i++) {
        if (P[i] == 1) {
            printf("%d ", i);
        }   
    }
    printf("\n");

    free(P);
}

int main() {
    int n;
    printf("Input n: ");
    scanf("%d", &n);

    sieve_of_eratosthenes(n);

    return 0;
}