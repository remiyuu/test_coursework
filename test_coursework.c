#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include <wchar.h>

#define MAX_NAME_LENGTH 100
#define MAX_PHONE_LENGTH 20
#define MAX_NOTE_LENGTH 200

const char* FNAME = "phonebook";

typedef struct {
    char name[MAX_NAME_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    char note[MAX_NOTE_LENGTH];
} bookRecord;

bookRecord recs[1000];
int recCount = 0;

int loadFromFile(const char* fn) {
    FILE* fin = fopen(fn, "r");
    if (!fin) {
        return 0;
    }

    while (fscanf(fin, "%99[^\n]\n%19[^\n]\n%199[^\n]\n", recs[recCount].name, recs[recCount].phone, recs[recCount].note) == 3) {
        recCount++;
    }

    fclose(fin);
    return 1;
}

int saveToFile(const char* fn) {
    FILE* fout = fopen(fn, "w");
    if (!fout) {
        return 0;
    }

    for (int i = 0; i < recCount; i++) {
        fprintf(fout, "%s\n%s\n%s\n", recs[i].name, recs[i].phone, recs[i].note);
    }

    fclose(fout);
    return 1;
}

void writeRecord(int i) {
    printf("%d. %s\t%s | ", i + 1, recs[i].name, recs[i].phone);
    int sz = 60 - strlen(recs[i].name) - strlen(recs[i].phone);
    if (strlen(recs[i].note) > sz) {
        printf("%.60s...", recs[i].note);
    }
    else {
        printf("%s", recs[i].note);
    }
    printf("\n");
}

int findRecord() {
    int sel;
    printf("Выберите способ поиска записи:\n");
    printf("1. По порядковому номеру\n");
    printf("2. По имени, номеру телефона или заметке\n\n > ");
    scanf("%d", &sel);
    printf("\n");

    if (sel == 2) {
        char val[200];
        printf("Введите значение для поиска: ");
        getchar(); // очистить новую строку от буфера
        fgets(val, sizeof(val), stdin);

        for (int i = 0; i < recCount; i++) {
            if (strstr(recs[i].name, val) || strstr(recs[i].note, val) || strcmp(recs[i].phone, val) == 0) {
                writeRecord(i);
            }
        }

        printf("\n");
    }

    int number = 0;
    printf("Введите порядковый номер записи: ");
    scanf("%d", &number);
    getchar(); // очистить новую строку от буфера

    return (number < 1 || number > recCount) ? -1 : number - 1;
}

void printSortMenu() {
    printf("\n\t\t[ Сортировка записей ]\n");
    printf("1. Сортировать по алфавиту\n");
    printf("2. Сортировать против алфавита\n");
    printf("3. Сортировать по номеру телефона\n");
    printf("4. Сортировать обратно номеру телефона\n");
    printf("5. Вернуться в главное меню\n\n > ");
}

// функция сравнения для сортировки по алфавиту
int compareByNameAZ(const void* a, const void* b) {
    const bookRecord* recA = (const bookRecord*)a;
    const bookRecord* recB = (const bookRecord*)b;
    return strcmp(recA->name, recB->name);
}

// функция сравнения для сортировки по имени обратно алфавиту
int compareByNameZA(const void* a, const void* b) {
    const bookRecord* recA = (const bookRecord*)a;
    const bookRecord* recB = (const bookRecord*)b;
    return -strcmp(recA->name, recB->name);
}

// функция сравнения для сортировки по номеру телефона
int compareByPhoneAZ(const void* a, const void* b) {
    const bookRecord* recA = (const bookRecord*)a;
    const bookRecord* recB = (const bookRecord*)b;
    int result = strcmp(recA->phone, recB->phone);
    if (result != 0)
        return result;
    // если номера телефонов одинаковы, возвращаем отрицательное значение, чтобы сохранить порядок
    return -1;
}

// Функция сравнения для сортировки по номеру телефона (Z до A)
int compareByPhoneZA(const void* a, const void* b) {
    const bookRecord* recA = (const bookRecord*)a;
    const bookRecord* recB = (const bookRecord*)b;
    int result = strcmp(recA->phone, recB->phone);
    if (result != 0)
        return -result; // инвертируем результат, чтобы сортировка была в обратном порядке
    // если номера телефонов одинаковы, возвращаем положительное значение, чтобы сохранить порядок
    return 1;
}

void sortRecordsByNameAZ() {
    qsort(recs, recCount, sizeof(bookRecord), compareByNameAZ);
}

void sortRecordsByNameZA() {
    qsort(recs, recCount, sizeof(bookRecord), compareByNameZA);
}

void sortRecordsByPhoneAZ() {
    qsort(recs, recCount, sizeof(bookRecord), compareByPhoneAZ);
}

void sortRecordsByPhoneZA() {
    qsort(recs, recCount, sizeof(bookRecord), compareByPhoneZA);
}


void writeRecords() {
    printf("Всего %d записей:\n", recCount);
    for (int i = 0; i < recCount; i++) {
        writeRecord(i);
    }
    int sel;
    while (true) {
        printSortMenu();
        scanf("%d", &sel);
        getchar(); // очистить новую строку от буфера
        printf("\n");

        switch (sel) {
        case 1:
            sortRecordsByNameAZ();
            printf("Записи отсортированы по алфавиту:\n");
            for (int i = 0; i < recCount; i++) {
                writeRecord(i);
            }
            break;
        case 2:
            sortRecordsByNameZA();
            printf("Записи отсортированы против алфавита:\n");
            for (int i = 0; i < recCount; i++) {
                writeRecord(i);
            }
            break;
        case 3:
            sortRecordsByPhoneAZ();
            printf("Записи отсортированы по номеру телефона:\n");
            for (int i = 0; i < recCount; i++) {
                writeRecord(i);
            }
            break;
        case 4:
            sortRecordsByPhoneZA();
            printf("Записи отсортированы обратно номеру телефона:\n");
            for (int i = 0; i < recCount; i++) {
                writeRecord(i);
            }
            break;
        case 5:
            return; // возвращаемся в меню просмотра записей
        default:
            printf("Неверный ввод. Попробуйте снова.\n");
            break;
        }
    }
}

void addRecord() {
    bookRecord r;
    printf("\n\n\t\t[ Добавление новой записи ]\n\n");

    printf("Введите номер телефона: ");
    fgets(r.phone, sizeof(r.phone), stdin);
    r.phone[strcspn(r.phone, "\n")] = '\0';

    printf("Введите имя записи: ");
    fgets(r.name, sizeof(r.name), stdin);
    r.name[strcspn(r.name, "\n")] = '\0';

    printf("Напишите примечание к записи: ");
    fgets(r.note, sizeof(r.note), stdin);
    r.note[strcspn(r.note, "\n")] = '\0';

    recs[recCount++] = r;
    printf("Новая запись с порядковым номером %d добавлена!\n", recCount);
    getchar();
}

void editRecord() {
    printf("\n\n\t\t[ Изменение записи ]\n\n");

    int id = findRecord();
    if (id == -1) {
        addRecord();
        return;
    }

    char temp[200] = "";

    printf("Примечание: Чтобы оставить поле без изменений, просто нажмите Ввод.\n\n");

    printf("Введите новый номер телефона: ");
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) {
        strcpy(recs[id].phone, temp);
    }

    printf("Введите новое имя записи: ");
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) {
        strcpy(recs[id].name, temp);
    }

    printf("Напишите новое примечание к записи: ");
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) {
        strcpy(recs[id].note, temp);
    }

    printf("Запись изменена!\n");
    getchar();
}

void deleteRecord() {
    printf("\n\n\t\t[ Удаление записи ]\n\n");

    char ans = 'n';
    int id = findRecord();
    if (id == -1) {
        printf("Удаление не удалось!\n");
        return;
    }

    printf("\n");
    writeRecord(id);
    printf("\nВы уверены? (y/n)? ");
    scanf("%c", &ans);

    if (ans == 'y' || ans == 'Y') {
        for (int i = id; i < recCount - 1; i++) {
            recs[i] = recs[i + 1];
        }
        recCount--;
        printf("Запись удалена!\nУчтите, что все порядковые номера других записей сдвинулись!\n");
        getchar();
    }
}

int thinkMenu() {
    int sel;

    printf("\n\n\t\t[ Главное меню ]\n\n");
    printf("1. Посмотреть список записей (%d)\n", recCount);
    printf("2. Добавить новую запись\n");
    printf("3. Отредактировать запись\n");
    printf("4. Удалить запись\n");
    printf("5. Сохранить телефонную книжку\n");
    printf("6. Сохранить и выйти\n\n > ");
    scanf("%d", &sel);
    getchar();
    printf("\n");

    switch (sel) {
    case 1: writeRecords(); break;
    case 2: addRecord(); break;
    case 3: editRecord(); break;
    case 4: deleteRecord(); break;
    case 5:
        printf("%s\n", (saveToFile(FNAME) ? "Сохранено!" : "Ошибка при сохранении!"));
        getchar();
        break;
    case 6: return 0; break;
    default:  break;
    }

    return 1;
}

int main() {
    setlocale(LC_ALL, "Rus");

    loadFromFile(FNAME);

    while (thinkMenu());

    int doTryAgain = 1;
    while (!saveToFile(FNAME) && doTryAgain) {
        printf("\n\n\t\t[ Внимание! ]\n\n");
        printf("Возникла ошибка при сохранении файла телефонной книжки: %s\n", FNAME);
        printf("Хотите попробовать сохранить еще раз (y/n)? ");
        char ans = 'n';
        scanf("%c", &ans);
        doTryAgain = (ans == 'y' || ans == 'Y');
        getchar();
    }
    getchar();
    return 0;
}