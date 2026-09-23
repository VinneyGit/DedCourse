#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "charmanip.c"


// TODO функция удаления элемента массива
// TODO добавить функцию удаления всех заданных символов из строки

//================================================================================================================

int Puts(const char* str);

//----------------------------------------------------------------------------------------------------------------

char* Strcpy(char* destination, const char* source);
char* Strcat(char* destination, const char* source);
char* Strdup(const char* str);

size_t Strlen(const char* str);
int Strcmp(const char* leftstr, const char* rightstr); // After using always use free()
// TODO Strchr

//----------------------------------------------------------------------------------------------------------------

char* Strncpy(char* destination, const char* source, size_t count);
char* Strncat(char* destination, const char* source, size_t count);
char* Strndup(const char* str, size_t count);

size_t Strnlen(const char* str, size_t count);
int Strncmp(const char* leftstr, const char* rightstr, size_t count); // After using always use free()
// TODO Strnchr

//----------------------------------------------------------------------------------------------------------------

int Atoi(const char* str);
double Atof(const char* str);

//================================================================================================================

void print_hidden_string(const char *str); // TODO нагажено, убрать

/*
int main() {
    printf("%d %d\n", Strcmp("wf", "af"), strcmp("wf", "af"));
    printf("%d %d\n", Strcmp("wf", "wf"), strcmp("wf", "wf"));
    printf("%d %d\n", Strcmp("af", "wf"), strcmp("af", "wf"));
    printf("%d %d\n", Strcmp("wf", "wff"), strcmp("af", "wf"));


    return 0;
}
*/

//================================================================================================================

int Puts(const char* str) {
    int ch = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if ((ch = putchar(str[i])) == EOF) {
            return EOF;
        }
    }

    ch = putchar('\n');

    return ch;
}

//================================================================================================================

char* Strcpy(char* destination, const char* source) {
    for (int i = 0; (destination[i] = source[i]) != '\0'; i++) {;}

    return destination;
}

char* Strcat(char* destination, const char* source) {
    size_t i = 0;
    size_t lenOfDestination = Strlen(destination);

    for (i = 0; i < lenOfDestination; i++) {;}

    for (size_t j = 0; (destination[i + j] = source[j]) != '\0'; j++) {;}

    return destination;
}

char* Strdup(const char* str) {
    size_t size = Strlen(str) + 1;

    char* duplicate = (char*)calloc(size, sizeof(char));

    Strcpy(duplicate, str);

    return duplicate;
}

//----------------------------------------------------------------------------------------------------------------

size_t Strlen(const char* str) {
    size_t i = 0;

    for (; str[i] != '\0'; i++) {;}

    return i;
}

int Strcmp(const char* l, const char* r) {
    int i = 0;

    for (; l[i] == r[i] && l[i] != '\0' && r[i] != '\0'; i++) {;}

    return (l[i] > r[i]) - (l[i] < r[i]); // add len catcher and make return to (a < b) - (a > b)
}

//================================================================================================================

char* Strncpy(char* destination, const char* source, size_t count) {
    for (size_t i = 0; i < count && (destination[i] = source[i]) != '\0'; i++) {;}

    return destination;
}

char* Strncat(char* destination, const char* source, size_t count) {
    size_t i = 0;
    size_t lenOfDestination = Strlen(destination);

    for (i = 0; i < lenOfDestination ; i++) {;}

    for (size_t j = 0; j < count && (destination[i + j] = source[j]) != '\0'; j++) {;}

    return destination;
}

char* Strndup(const char* str, size_t count) {
    size_t size = Strnlen(str, count);

    char* duplicate = (char*)calloc(size + 1, sizeof(char));

    Strncpy(duplicate, str, size);

    return duplicate;
}

//----------------------------------------------------------------------------------------------------------------

size_t Strnlen(const char* str, size_t count) {
    size_t i = 0;

    for (; str[i] != '\0' && i < count; i++) {;}

    return i;
}

int Strncmp(const char* l, const char* r, size_t count) {
    if(count == 0) {
        return 0;
    }

    size_t i = 0;

    for (; i < count && l[i] == r[i] && l[i] != '\0' && r[i] != '\0'; i++) {;}

    return (l[i] > r[i]) - (l[i] < r[i]); // add len catcher and make return to (a < b) - (a > b)
}

//================================================================================================================

int Atoi(const char* str) {
    int number = 0;
    int i = 0;
    int sign = 1;

    while (Isspace(str[i])) {
        i++;
    }

    if (str[i] == '-') {
        sign = -1;
        i++;
    }
    else if (str[i] == '+') {
        i++;
    }

    while (Isdigit(str[i])) {
        number *= 10;
        number += str[i] - '0';
        i++;
    }

    return sign * number;
}


double Atof(const char* str) {
    double number = 0.0;

    int i = 0;
    float sign = 1;

    while (Isspace(str[i])) {
        i++;
    }

    if (str[i] == '-') {
        sign = -1;
        i++;
    }
    else if (str[i] == '+') {
        i++;
    }

    if ((ToLower(str[i]) == 'n')
        && (ToLower(str[i + 1]) == 'a')
        && (ToLower(str[i + 2]) == 'n')) {


        return sign > 0 ? NAN : -NAN;
    }

    if ((ToLower(str[i]) == 'i')
        && (ToLower(str[i + 1]) == 'n')
        && (ToLower(str[i + 2]) == 'f')) {

        if ((ToLower(str[i + 3]) == 'i')
            && (ToLower(str[i + 4]) == 'n')
            && (ToLower(str[i + 5]) == 'i')
            && (ToLower(str[i + 6]) == 't')
            && (ToLower(str[i + 7]) == 'y')) {

                return sign * INFINITY;
            }
        return sign * INFINITY;
    }

    while (Isdigit(str[i])) {
        number *= 10;
        number += str[i] - '0';
        i++;
    }

    if (str[i] == '.') {
        i++;
        int numbersAfterDot = 0;
        while (Isdigit(str[i])) {
            numbersAfterDot++;
            number += (str[i] - '0') / pow(10, numbersAfterDot);
            i++;
        }
    }

    int exponent = 0;
    if (str[i] == 'e' || str[i] == 'E') {
        i++;

        int signOfExp = 1;

        if (str[i] == '-') {
            signOfExp = -1;
            i++;
        }
        else if (str[i] == '+') {
            i++;
        }

        if (Isdigit(str[i])) {
            exponent = 1;
        }

        while (Isdigit(str[i])) {
            exponent *= 10;
            exponent += str[i] - '0';
            i++;
        }

        return sign * number * pow(10, signOfExp * exponent);
    }

    return sign * number;
}

/*
// TODO getline 4 через scanf %m???
*/


void print_hidden_string(const char *str) { // TODO нагажено, убрать
    if (!str) return;

    while (*str) {
        switch (*str) {
            case '\n': printf("\033[1;31m\\n\033[0m");  break; // Перевод строки
            case '\t': printf("\033[1;31m\\t\033[0m");  break; // Табуляция
            case '\r': printf("\033[1;31m\\r\033[0m");  break; // Возврат каретки
            case '\b': printf("\033[1;31m\\b\033[0m");  break; // Забой (Backspace)
            case '\a': printf("\033[1;31m\\a\033[0m");  break; // Гудок (Alert)
            case '\v': printf("\033[1;31m\\v\033[0m");  break; // Вертикальная табуляция
            case '\f': printf("\033[1;31m\\f\033[0m");  break; // Перевод страницы
            case '\\': printf("\033[1;31m\\\\\033[0m"); break; // Сам бэкслеш
            case ' ':
                // Опционально: можно подсветить пробел, чтобы видеть их количество
                printf(" ");
                break;
            default:
                // Если символ непечатный (ASCII < 32), выводим его код в восьмеричном формате
                if (*str < 32) {
                    printf("\033[1;31m\\%03o\033[0m", (unsigned char)*str);
                } else {
                    putchar(*str); // Обычный печатный символ
                }
                break;
        }
        str++;
    }
    printf("\n"); // Перевод строки в конце вывода всей функции
}

