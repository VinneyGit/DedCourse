#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t Strlen(const char* str);

int Puts(const char* str);

char* Strcpy(char* destination, const char* source);

char* Strcat(char* destination, const char* source);

int Strcmp(const char* leftstr, const char* rightstr);

int Isdigit(int ch);

int Isspace(int ch);

int ToLower(int ch);

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

    return leftstr[i] - rightstr[i]; // add len catcher and make return to (a < b) - (a > b)
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

int ToLower(int ch) {
    if (ch >= 'A' && ch <= 'Z') {
        return ch - 'A' + 'a';
    }
    return ch;
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


double Atof(const char* str) {
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
        i++

        if (Isdigit(str[i])) {
            exponent = 1;
        }

        while (Isdigit(str[i])) {
            exponent *= 10;
            exponent += str[i] - '0';
            i++;
        }

        return sign * number * exponent;
    }

    return sign * number;
}


char* Strdup(const char* str) {
    size_t size = Strlen(str) + 1;

    char* duplicate = (char*)malloc(size * sizeof(char));

    Strcpy(duplicate, str);

    return duplicate;
}
/*
getline 4 через scanf %m??? 
+n
*/
