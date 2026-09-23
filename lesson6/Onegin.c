#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys\stat.h>

#include "../lesson1/MyString.c"
#include "../lesson4/FunctionPointer.c"


const char* INPUT_PATH = "formatted_onegin.txt";
const char* OUTPUT_PATH = "sortedOnegin.txt";


#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

struct String {
    char* str;
    size_t len;
};

// TODO добавить структуру с данными файла свою????
// TODO добавить в компаратор первые буквы цифр????
// TODO REMAKE TO WITHOUT F OR FIX \r????
// TODO myfree()
// TODO ИДЕЯ ПЕРЕДАВАТЬ ЧИТАЕМЫЙ ПАРАМЕТР ИЗ СТАТС КАК ПЕРЕМЕННУЮ????

//================================================================================================================

size_t ReadFileSize(const char* name);
size_t ReadFromFileToBuffer(const char* name, char* buffer, const size_t bufferSize);

//----------------------------------------------------------------------------------------------------------------

size_t BufferSplit(char* buffer);
void LinesIndexing(char* buffer, size_t bufferSize, String* indexes, int* maxLen);
// TODO cutting last space lines???

//----------------------------------------------------------------------------------------------------------------

void MakeOutputFile(const char* name);
void WriteToFile(const char* name, size_t linesCount, const String* index, int maxLen);

//----------------------------------------------------------------------------------------------------------------

int ComparatorBegin(const void* ptr_a, const void* ptr_b);
int ComparatorEnd(const void* ptr_a, const void* ptr_b);
int ComparatorOriginal(const void* ptr_a, const void* ptr_b);

//================================================================================================================

int main() {

    size_t bufferSize = ReadFileSize(INPUT_PATH);
    printf("Size of file in bytes is: %zu\n", bufferSize);

    char* buffer = (char*)calloc(bufferSize, sizeof(char));
    bufferSize = ReadFromFileToBuffer(INPUT_PATH, buffer, bufferSize);


    size_t linesCount = BufferSplit(buffer);
    struct String* indexes = (String*)calloc(linesCount, sizeof(String));

    printf("linesCount: %5zu\n======================================\n", linesCount);

    int maxLineLen = 0;

    LinesIndexing(buffer, bufferSize, indexes, &maxLineLen);

    MakeOutputFile(OUTPUT_PATH);

    VoidBubbleSort(indexes, linesCount, sizeof(String), &ComparatorBegin);
    WriteToFile(OUTPUT_PATH, linesCount, indexes, 0);

    VoidBubbleSort(indexes, linesCount, sizeof(String), &ComparatorEnd);
    WriteToFile(OUTPUT_PATH, linesCount, indexes, maxLineLen);

    VoidBubbleSort(indexes, linesCount, sizeof(String), &ComparatorOriginal);
    WriteToFile(OUTPUT_PATH, linesCount, indexes, 0);

    free(buffer);
    free(indexes);

    return 0;
}

//================================================================================================================

size_t ReadFileSize(const char* name) {
    assert(name);

    struct stat filestats;
    stat(name, &filestats);

    return filestats.st_size;
}

size_t ReadFromFileToBuffer(const char* name, char* buffer, size_t bufferSize) {
    assert(name);
    assert(buffer);

    FILE* file = fopen(name, "r");

    if (file == NULL) {
        printf("ERROR WHILE OPENING INPUT FILE\n");
        return 0;
    }


    size_t charRead = fread(buffer, sizeof(char), bufferSize, file);

    fclose(file);

    return charRead;
}

//================================================================================================================

size_t BufferSplit(char* buffer) {
    assert(buffer);

    size_t linesCount = 0;

    while((buffer = strchr(buffer, '\n')) != NULL) {
        *buffer = '\0';
        buffer++;

        linesCount++;
    }

    return linesCount;
}

void LinesIndexing(char* buffer, size_t bufferSize, String* indexes, int* maxLen) {
    assert(buffer);
    assert(indexes);

    size_t position = 0;
    size_t stringNum = 0;

    while (position < bufferSize) {
        int len = 0;
        indexes[stringNum].str = buffer + position;

        while (buffer[position] != '\0') {
            position++;
            len++;
        }
        position++;

        indexes[stringNum].len = len;

        *maxLen = max(len, *maxLen);

        stringNum++;
    }
}

//================================================================================================================

void MakeOutputFile(const char* name) {
    assert(name);

    FILE* file = fopen(name, "w");

    if(file == NULL) {
        printf("ERROR WHILE CREATING OUTPUT FILE\n");
    }

    fclose(file);
}

void WriteToFile(const char* name, size_t linesCount, const String* indexes, int maxLen) {
    assert(name);
    assert(indexes);

    FILE* file = fopen(name, "a");

    if(file == NULL) {
        printf("ERROR WHILE OPENING OUTPUT FILE\n");
    }

    for (size_t i = 0; i < linesCount; i++) {
        if (indexes[i].len == 0) {
            continue;
        }
        fprintf(file, "%*s\n", maxLen, indexes[i].str);
    }

    fprintf(file, "============================================================================\n");

    fclose(file);
}

//================================================================================================================

int ComparatorBegin(const void* ptr_a, const void* ptr_b) {
    assert(ptr_a);
    assert(ptr_b);

    const String a = *(const String*)ptr_a;
    const String b = *(const String*)ptr_b;

    const char* str_a = a.str;
    const char* str_b = b.str;

    const size_t len_a = a.len;
    const size_t len_b = b.len;

    if (len_a == 0) {
        return 1;
    }

    else if (len_b == 0) {
        return -1;
    }

    else if (len_a == 0 && len_b == 0) {
        return 0;
    }

    size_t i_a = 0;
    size_t i_b = 0;

    while (i_a < len_a && i_b < len_b) {
        if (!isalpha(str_a[i_a])) {
            i_a++;
            continue;
        }
        if (!isalpha(str_b[i_b])) {
            i_b++;
            continue;
        }
        if (ToLower(str_a[i_a]) != ToLower(str_b[i_b])) {
            break;
        }
        i_a++;
        i_b++;
    }

    return (ToLower(str_a[i_a]) > ToLower(str_b[i_b])) - (ToLower(str_a[i_a]) < ToLower(str_b[i_b]));
}

int ComparatorEnd(const void* ptr_a, const void* ptr_b) {
    assert(ptr_a);
    assert(ptr_b);

    const String a = *(const String*)ptr_a;
    const String b = *(const String*)ptr_b;

    const char* str_a = a.str;
    const char* str_b = b.str;

    const size_t len_a = a.len;
    const size_t len_b = b.len;

    if (len_a == 0) {
        return 1;
    }

    else if (len_b == 0) {
        return -1;
    }

    else if (len_a == 0 && len_b == 0) {
        return 0;
    }

    size_t i_a = len_a - 1;
    size_t i_b = len_b - 1;

    while (i_a > 0 && i_b > 0) {
        if (!isalpha(str_a[i_a])) {
            i_a--;
            continue;
        }
        if (!isalpha(str_b[i_b])) {
            i_b--;
            continue;
        }
        if (ToLower(str_a[i_a]) != ToLower(str_b[i_b])) {
            break;
        }
        i_a--;
        i_b--;
    }

    return (ToLower(str_a[i_a]) > ToLower(str_b[i_b])) - (ToLower(str_a[i_a]) < ToLower(str_b[i_b]));
}

int ComparatorOriginal(const void* ptr_a, const void* ptr_b) {
    assert(ptr_a);
    assert(ptr_b);

    const String a = *(const String*)ptr_a;
    const String b = *(const String*)ptr_b;

    const char* str_a = a.str;
    const char* str_b = b.str;

    return (str_a > str_b) - (str_a < str_b);

    return 0;
}
