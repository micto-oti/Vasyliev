/* #include <stdio.h>
#include <string.h>

int main(){
    long long M, N;

    File *file = fopen("input01.txt", "r")

    for (M=0;)




    long long numerator = M * N;
    long long denominator = M - N;
    


    return 0;
} */

#include <stdio.h>
#include <stdlib.h> // для system()
#include <errno.h>  // для errno

long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int main() {
    // Диагностика текущей директории
    printf("Текущая директория: ");
    system("pwd");
    printf("Содержимое директории:\n");
    system("ls -l");
    
    long long N, M;
    
    printf("Пытаюсь открыть input.txt...\n");
    FILE *input_file = fopen("input.txt", "r");
    
    if (input_file == NULL) {
        perror("Ошибка при открытии файла");
        printf("Код ошибки: %d\n", errno);
        return 1;
    }
    
    // Остальной код без изменений...
    fscanf(input_file, "%lld %lld", &N, &M);
    fclose(input_file);
    
    long long numerator = M * N;
    long long denominator = M - N;
    
    long long common_divisor = gcd(numerator, denominator);
    numerator /= common_divisor;
    denominator /= common_divisor;
    
    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        perror("Ошибка при создании файла");
        return 1;
    }
    
    fprintf(output_file, "%lld/%lld\n", numerator, denominator);
    fclose(output_file);
    
    printf("Программа успешно завершена. Результат в output.txt\n");
    return 0;
}