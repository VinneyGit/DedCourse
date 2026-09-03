#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

size_t Strlen(const char* str);

int Puts(const char* str);

char* Strcpy(char* destination, const char* source);

char* Strcat(char* destination, const char* source);

int Strcmp(const char* leftstr, const char* rightstr);

int Isdigit(int ch);

int Isspace(int ch);

int Atoi(const char* str);

double Atof(const char* str);

char* Strdup(const char* str); // After using always use free()


int main() {
    // char str[80] = "\0";
    // scanf("%s", str);
    printf("TEST: %g\n", atof("iNffe"));
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


int Isdigit(int ch) {
    return (ch >= '0' && ch <= '9') ? 1 : 0;
}


int Isspace(int ch) {
    if ((ch >= '\t' && ch <= '\r') || ch == ' ') {
        return 1;
    }
    return 0;
}


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


double Atof(const char* str) { // in process 1. add inf and nan handling 2. add
    double number = 0.0;
    int i = 0;
    int sign = 1;

    while (Isspace(str[i])) {
        i++;
    }

    if (str[i] == '-') {
        sign = -1;
        i++;
    }

    return sign * number;
}


char* Strdup(const char* str) {
    char* duplicate = (char*)malloc(sizeof(str));
    Strcpy(duplicate, str);
    return duplicate;
}
/*
getline 4
+n
*/
