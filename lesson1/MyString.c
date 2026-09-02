#include <assert.h>
#include <stdio.h>
#include <string.h>

size_t Strlen(const char* str);
int Puts(const char* str);
char* Strcpy(char* destination, const char* source);
char* Strcat(char* destination, const char* source);
int Strcmp(const char* leftstr, const char* rightstr);

// TODO удалить ассерты

int main() {
    printf("TEST: %d\n", strcmp("aa35", "atg35"));
    return 0;
}

size_t Strlen(const char* str) {
    assert(str);

    size_t i = 0;
    for (; str[i] != '\0'; i++) {;}
    return i;
}

int Puts(const char* str) { // TODO сделать проверку на ошибки
    assert(str);

    for (int i = 0; str[i] != '\0'; i++) {
        putchar(str[i]);
    }
    putchar('\n');
    return EOF;
}

char* Strcpy(char* destination, const char* source) {
    assert(destination);
    assert(source);

    for (int i = 0; source[i] != '\0' && destination[i] != '\0'; i++) {
        destination[i] = source[i];
    }
    return destination;
}

char* Strcat(char* destination, const char* source) { // TODO strlen в переменную
    assert(destination);
    assert(source);

    size_t i = 0;
    for (i = 0; i < Strlen(destination); i++) {;}
    for (size_t j = 0; (destination[i + j] = source[j]) != '\0'; j++) {;}

    return destination;
}

int Strcmp(const char* leftstr, const char* rightstr) {
    assert(leftstr);
    assert(rightstr);

    for (; leftstr[i] == rightstr[i] && leftstr[i] != '\0' && rightstr[i] != '\0'; i++) {;}

    return leftstr[i] - rightstr[i];
}
/*
strdup 3
getline 4
atoi 1
atof 2
+n
*/
