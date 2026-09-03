#include <assert.h>
#include <stdio.h>
#include <string.h>

size_t Strlen(const char* str);
int Puts(const char* str);
char* Strcpy(char* destination, const char* source);
char* Strcat(char* destination, const char* source);
int Strcmp(const char* leftstr, const char* rightstr);

int main() {
    printf("TEST: %d, %d\n", Strcmp("atg35", "aa35"), strcmp("atg35", "aa35"));
    return 0;
}


size_t Strlen(const char* str) {
    size_t i = 0;

    for (; str[i] != '\0'; i++) {;}

    return i;
}


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


char* Strcpy(char* destination, const char* source) {
    for (int i = 0; source[i] != '\0' && destination[i] != '\0'; i++) {
        destination[i] = source[i];
    }

    return destination;
}


char* Strcat(char* destination, const char* source) {
    size_t i = 0;
    size_t lenOfDestination = Strlen(destination);

    for (i = 0; i < lenOfDestination; i++) {;}

    for (size_t j = 0; (destination[i + j] = source[j]) != '\0'; j++) {;}

    return destination;
}


int Strcmp(const char* leftstr, const char* rightstr) {
    int i = 0;

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
