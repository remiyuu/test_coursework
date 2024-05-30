LabelConvertDecimal:
    mov al, byte ptr[ecx]
    test al, al
    jz LabelEndConvert
    sub al, '0'
    imul ebx, 10
    add ebx, eax
    inc ecx
    jmp LabelConvertDecimal

LabelConvertDecimal:
    mov al, byte ptr[ecx]
    test al, al
    jz LabelEndConvert
    sub al, '0'
    imul ebx, 10
    add ebx, eax
    inc ecx
    jmp LabelConvertDecimal

LabelEndConvert:
    test negative, 1
    jz LabelEnd3
    neg ebx
    LabelEnd3:
    mov decimal_num, ebx

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


To convert the string `src_num` to an integer using assembly language, you can use a loop to process each character in the string. Here's how you can modify the program to do this:

```c
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
    int decimal_num = 0; // new variable to store the decimal number

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

            jmp LabelStart2;

    LabelEnd2:
        // Convert src_num to an integer and store it in decimal_num
        mov eax, 0
        mov ebx, 1
        lea ecx, src_num
        cmp byte ptr[ecx], '-'
        jne LabelConvertDecimal
        mov negative, 1
        inc ecx
        LabelConvertDecimal:
        mov al, byte ptr[ecx]
        test al, al
        jz LabelEndConvert
        sub al, '0'
        imul ebx, 10
        add eax, ebx
        mov ebx, eax
        inc ecx
        jmp LabelConvertDecimal
        LabelEndConvert:
        test negative, 1
        jz LabelEnd3
        neg ebx
        LabelEnd3:
        mov decimal_num, ebx

        pop edx;
        pop ecx;
        pop ebx;
        pop eax;
    }

    printf("9 -> 2\nsource = %d (decimal)\n", decimal_num); // print the decimal number
    for (int i = 0; src_num[i] != '\0'; i++) {
        printf("%c", src_num[i]);
    }
    printf("\nresult = ");
    printf(negative ? "-%s\n" : "%s\n", pstr);
}
```

In this modified program, the assembly language code converts the string `src_num` to an integer and stores it in `decimal_num`. The code uses a loop (`LabelConvertDecimal`) to process each character in the string, multiplying the current value of `decimal_num` by 10 and adding the value of the current character. If the string starts with a '-' character, the code sets the `negative` flag to true and skips the first character. After the loop, if the `negative` flag is true, the code negates the value of `decimal_num`.