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

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

void main(void) {
    char src_num[] = "-15678";
    char res_num[50];
    int num = 0;
    char* pstr;
    char digit;
    bool negative = false;
    int decimal_num = 0;
    int base = 1;

    pstr = src_num;

    __asm {
        push eax;
        push ebx;
        push ecx;
        push edx;

        mov num, 0
            lea eax, src_num
            mov pstr, eax

            LabelStart1 :
        // Проверка конца строки
        mov eax, pstr
            mov cl, byte ptr[eax]
            cmp cl, '-'
            jne LabelContinueCalc
            mov negative, 1
            jmp LabelNextChar
            LabelContinueCalc :
        test cl, cl
            je LabelEnd1
            // Преобразование символа в число
            cmp cl, '0'
            jl LabelNextChar
            cmp cl, '8'
            jg LabelNextChar
            sub cl, '0'
            mov digit, cl
            // Формирование числа num
            mov eax, num
            mov ebx, 9
            mul ebx
            xor ebx, ebx
            mov bl, digit
            add eax, ebx
            mov num, eax
            // Переход к следующему символу строки
            LabelNextChar :
        inc pstr
            jmp LabelStart1

            LabelEnd1 :
        // Преобразование числа в строку
        lea eax, res_num;
        add eax, 49;
        mov pstr, eax;
        mov byte ptr[eax], 0;

    LabelStart2:
        // Проверка окончания цикла
        cmp num, 0
            jle LabelEnd2
            // Получение младшей цифры числа
            mov eax, num
            xor edx, edx
            mov ebx, 2
            div ebx
            mov num, eax

            // Преобразование цифры в символ
            add dl, '0'
            mov digit, dl
            dec pstr
            mov eax, pstr
            mov byte ptr[eax], dl

            // Update decimal_num and base
            mov eax, dword ptr[decimal_num]
            mov ecx, dword ptr[base]
            mul ecx
            add eax, edx
            mov dword ptr[decimal_num], eax
            mov eax, dword ptr[base]
            mov ecx, 2
            mul ecx
            mov dword ptr[base], eax

            jmp LabelStart2;

    LabelEnd2:
        // If the number is negative, we need to subtract the decimal equivalent from 2^32.
        cmp byte ptr[negative], 0
        je LabelPrint
        mov eax, 1
        mov ecx, 32
        LabelPow:
            mul eax
            loop LabelPow
        mov ecx, dword ptr[decimal_num]
        sub eax, ecx
        mov dword ptr[decimal_num], eax

    LabelPrint:
        // Print the results
        pop edx;
        pop ecx;
        pop ebx;
        pop eax;

        printf("9 -> 2\nsource = ");
        for (int i = 0; src_num[i] != '\0'; i++) {
            printf("%c", src_num[i]);
        }
        printf("\nresult = ");
        printf(negative ? "-%s\n" : "%s\n", pstr);
        printf("Decimal equivalent = %d\n", decimal_num);
    }
}
