#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS

int main() {
    int a, b, c, d, e, n, h;

    printf("enter a: ");
    scanf("%d", &a);

    printf("enter b: ");
    scanf("%d", &b);

    printf("enter c: ");
    scanf("%d", &c);

    printf("enter d: ");
    scanf("%d", &d);

    printf("enter e: ");
    scanf("%d", &e);

    printf("\nenter amount of iterations: ");
    scanf("%d", &n);

    printf("\nenter step h: ");
    scanf("%d", &h);

    int* argument = (int*)malloc(sizeof(int) * n * 2);
    int* function = (int*)malloc(sizeof(int) * n * 2);

    __asm {
        mov ecx, n          // кол-во итераций
        mov ebx, c          // исходное значение аргумента c
        mov esi, argument   // указатель на массив аргументов
        mov edi, function   // указатель на массив функций

        loop_start :
        // y = (a + b + c) / d * e
        mov eax, a
            add eax, b
            add eax, ebx
            cdq
            idiv d
            imul eax, e

            // сохранение значений аргумента и функции в массивы
            mov[esi], ebx
            add esi, 4
            mov[edi], eax
            add edi, 4

            // сохранение счетчика итераций
            mov[esi], ecx
            add esi, 4
            mov[edi], ecx
            add edi, 4

            // увеличение аргумента c на шаг h
            add ebx, h

            // уменьшение счетчика и проверка условия завершения цикла
            loop loop_start
    }

    printf("\nargument | counter\n");
    for (int i = 0; i < n * 2; i = i + 2){
        printf("%8d|%8d|\n", *(argument + i), i / 2);
    }

    printf("\nfunction | counter\n");
    for (int i = 0; i < n * 2; i = i + 2){
        printf("%8d|%8d|\n", *(function + i), i / 2);
    }

    getchar();

    free(argument);
    free(function);

    getchar();
    return 0;
}
