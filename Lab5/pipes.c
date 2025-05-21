#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h> // для mkdir()

// Функция для нахождения наибольшего общего делителя (НОД)
long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Функция для обработки одной пары файлов
void process_file_pair(const char* folder, int file_num) {
    char input_path[256];
    char output_path[256];
    
    // Формируем пути к файлам
    snprintf(input_path, sizeof(input_path), "%s/input%02d.txt", folder, file_num);
    snprintf(output_path, sizeof(output_path), "%s/output%02d.txt", folder, file_num);

    // Открываем входной файл
    FILE *input_file = fopen(input_path, "r");
    if (input_file == NULL) {
        printf("Не удалось открыть файл %s: %s\n", input_path, strerror(errno));
        return;
    }

    // Читаем данные
    long long N, M;
    if (fscanf(input_file, "%lld %lld", &N, &M) != 2) {
        printf("Ошибка чтения данных из файла %s\n", input_path);
        fclose(input_file);
        return;
    }
    fclose(input_file);

    // Вычисляем результат
    long long numerator = M * N;
    long long denominator = M - N;
    long long common_divisor = gcd(numerator, denominator);
    numerator /= common_divisor;
    denominator /= common_divisor;

    // Записываем результат
    FILE *output_file = fopen(output_path, "w");
    if (output_file == NULL) {
        printf("Не удалось создать файл %s: %s\n", output_path, strerror(errno));
        return;
    }

    fprintf(output_file, "%lld/%lld\n", numerator, denominator);
    fclose(output_file);

    printf("Обработан файл %s -> %s\n", input_path, output_path);
}

int main() {
    const int MAX_FILES = 100; // Максимальное количество файлов для обработки
    const char* folder = "pipe"; // Имя подпапки с файлами

    // Создаём папку, если её нет
    struct stat st = {0};
    if (stat(folder, &st) == -1) {
        mkdir(folder, 0755); // Создаём директорию с правами rwxr-xr-x
    }

    for (int i = 1; i <= MAX_FILES; i++) {
        // Формируем путь к входному файлу
        char input_path[256];
        snprintf(input_path, sizeof(input_path), "%s/input%02d.txt", folder, i);

        // Проверяем существование входного файла
        FILE *test_file = fopen(input_path, "r");
        if (test_file == NULL) {
            if (i == 1) {
                printf("В папке %s не найдено ни одного входного файла\n", folder);
            }
            break;
        }
        fclose(test_file);

        // Обрабатываем файлы
        process_file_pair(folder, i);
    }

    return 0;
}