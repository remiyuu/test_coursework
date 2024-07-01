#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS

#include <stdio.h>
#include <locale.h>

// Function declarations
__declspec(naked) void func(signed char* a, signed char* b, signed char* c, signed char* d, signed char* e, signed char* y);
__declspec(naked) void ascbin(char* str, signed char* result);
__declspec(naked) void binasc(signed char num, char* result);

__declspec(naked) void func(signed char* a, signed char* b, signed char* c, signed char* d, signed char* e, signed char* y) {
    __asm {
        push ebp
        mov ebp, esp

        mov al, byte ptr [ebp + 12]  // e
        mov bl, byte ptr [ebp + 8]   // b
        mov cl, byte ptr [ebp + 16]  // c
        mov dl, byte ptr [ebp + 20]  // d
        imul cl, dl
        cbw
        idiv bl
        add al, cl
        mov bl, byte ptr [ebp + 10]  // a
        sub al, bl
        mov byte ptr [ebp + 24], al  // y

        mov esp, ebp
        pop ebp
        ret
    }
}

__declspec(naked) void ascbin(char* str, signed char* result) {
    __asm {
        push ebp
        mov ebp, esp

        mov dl, 1
        xor bl, bl
        xor eax, eax
        mov ecx, [ebp + 8]
        cmp byte ptr [ecx], '-'
        jne loop1
        neg dl
    loop1:
        inc ecx
        mov bl, [ecx]
        test bl, bl
        je endAscbin
        sub bl, 0x30
        imul eax, eax, 10
        add al, bl
        jmp loop1
    endAscbin:
        imul al, dl
        mov ebx, [ebp + 12]
        mov [ebx], al

        mov esp, ebp
        pop ebp
        ret
    }
}

__declspec(naked) void binasc(signed char num, char* result) {
    __asm {
        push ebp
        mov ebp, esp

        mov edi, [ebp + 12]
        mov al, [ebp + 8]
        mov bl, 10
        add edi, 4
        mov byte ptr [edi], 0
        dec edi
        mov byte ptr sign, '+'
        cmp al, 0
        jnl loop2
        mov byte ptr sign, '-'
        neg al
    loop2:
        test al, al
        jz endBinasc
        cbw
        idiv bl
        add ah, 0x30
        mov [edi], ah
        dec edi
        jmp loop2
    endBinasc:
        mov al, sign
        mov [edi], al

        mov esp, ebp
        pop ebp
        ret
    }
}

signed char numin, sign;
char* res;

void main(void) {
    setlocale(LC_ALL, "Rus");
    char a[5], b[5], c[5], d[5], e[5], numouta[5], numoutb[5], numoutc[5], numoutd[5], numoute[5];
    signed char ac, bc, cc, dc, ec, y, rin;

    printf("Вычисляемая функция: y=a/b+c*d-e\n");
    printf("Числа вводить в формате ЗнЦЦЦЦ...Ц\nГде Зн - знак + или -, а Ц - цифра от 0 до 9\n");
    printf("Введите число a: ");
    scanf("%s", a);
    printf("Введите число b: ");
    scanf("%s", b);
    printf("Введите число c: ");
    scanf("%s", c);
    printf("Введите число d: ");
    scanf("%s", d);
    printf("Введите число e: ");
    scanf("%s", e);

    __asm {
        lea eax, rin
        push eax
        lea eax, a
        push eax
        call ascbin
        add esp, 8
        mov bl, rin
        mov ac, bl

        lea eax, rin
        push eax
        lea eax, b
        push eax
        call ascbin
        add esp, 8
        mov bl, rin
        mov bc, bl

        lea eax, rin
        push eax
        lea eax, c
        push eax
        call ascbin
        add esp, 8
        mov bl, rin
        mov cc, bl

        lea eax, rin
        push eax
        lea eax, d
        push eax
        call ascbin
        add esp, 8
        mov bl, rin
        mov dc, bl

        lea eax, rin
        push eax
        lea eax, e
        push eax
        call ascbin
        add esp, 8
        mov bl, rin
        mov ec, bl

        lea eax, y
        push eax
        lea eax, ec
        push eax
        lea eax, dc
        push eax
        lea eax, cc
        push eax
        lea eax, bc
        push eax
        lea eax, ac
        push eax
        call func
        add esp, 24

        mov al, ac
        mov numin, al
        lea eax, numouta
        push eax
        mov eax, numin
        push eax
        call binasc
        add esp, 8
        mov eax, numouta

        mov al, bc
        mov numin, al
        lea eax, numoutb
        push eax
        mov eax, numin
        push eax
        call binasc
        add esp, 8
        mov eax, numoutb

        mov al, cc
        mov numin, al
        lea eax, numoutc
        push eax
        mov eax, numin
        push eax
        call binasc
        add esp, 8
        mov eax, numoutc

        mov al, dc
        mov numin, al
        lea eax, numoutd
        push eax
        mov eax, numin
        push eax
        call binasc
        add esp, 8
        mov eax, numoutd

        mov al, ec
        mov numin, al
        lea eax, numoute
        push eax
        mov eax, numin
        push eax
        call binasc
        add esp, 8
        mov eax, numoute
    }

    printf("\na(digital) = %d\n", ac);
    printf("b(digital) = %d\n", bc);
    printf("c(digital) = %d\n", cc);
    printf("d(digital) = %d\n", dc);
    printf("e(digital) = %d\n", ec);

    printf("\ny(result) = %d\n", y);

    printf("a(char) = %s\n", numouta);
    printf("b(char) = %s\n", numoutb);
    printf("c(char) = %s\n", numoutc);
    printf("d(char) = %s\n", numoutd);
    printf("e(char) = %s\n", numoute);

    getchar();
    return;
}
