#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys\stat.h>

#include "../lesson1/MyString.c"
#include "../lesson4/FunctionPointer.c"


//==============================================================================

#define max(a, b) ((a) > (b) ? (a) : (b))

struct String {
    char*   str;
    size_t  len;
};

struct Text {
    char*   buffer;
    size_t  bufferSize;
    size_t  linesAmount;
    size_t  f;
};

// TODO ??? add windows CRLF
// TODO ??? Filedescriptor
// TODO make struct for buffer with number of lines and other

//==============================================================================

size_t  ReadFileSize         (const char* fileName);
size_t  ReadFromFileToBuffer (const char* fileName, char* buffer,
                              const size_t bufferSize            );

//------------------------------------------------------------------------------

size_t  BufferSplit          (char* buffer);
void    LinesIndexing        (char* buffer, const size_t bufferSize,
                              String* lines, int* maxLineLen        );

//------------------------------------------------------------------------------

void    CreateOutputFile     (const char* fileName);
void    WriteToFile          (const char* fileName, const size_t linesAmount,
                              const String* lines, int maxLineLen            );

//------------------------------------------------------------------------------

int     ComparatorBegin      (const void* ptr_a, const void* ptr_b);
int     ComparatorEnd        (const void* ptr_a, const void* ptr_b);
int     ComparatorOriginal   (const void* ptr_a, const void* ptr_b);

//==============================================================================

int main(int argc, char** argv) {

    const char* INPUT_PATH = *(argv + 1);
    const char* OUTPUT_PATH = *(argv + 2);

    if (argc == 1) {
        printf("Please, put path for INPUT and OUTPUT files\n");
        return -1;
    }

    else if (argc == 2) {
        printf("Please, put path for OUTPUT file\n");
        return -1;
    }

    assert(INPUT_PATH);
    assert(OUTPUT_PATH);

    printf("Input: %s\n", INPUT_PATH);
    printf("Output: %s\n", OUTPUT_PATH);



    size_t bufferSize = ReadFileSize(INPUT_PATH);
    printf("Size of file in bytes is: %zu\n", bufferSize);

    char* buffer = (char*)calloc(bufferSize, sizeof(char));
    bufferSize = ReadFromFileToBuffer(INPUT_PATH, buffer, bufferSize);

    size_t linesAmount = BufferSplit(buffer);
    struct String* lines = (String*)calloc(linesAmount, sizeof(String));

    printf("linesAmount: %5zu\n============================\n", linesAmount);



    int maxLineLen = 0;
    LinesIndexing(buffer, bufferSize, lines, &maxLineLen);

    CreateOutputFile(OUTPUT_PATH);

    VoidBubbleSort(lines, linesAmount, sizeof(String), &ComparatorBegin);
    WriteToFile(OUTPUT_PATH, linesAmount, lines, 0);

    VoidBubbleSort(lines, linesAmount, sizeof(String), &ComparatorEnd);
    WriteToFile(OUTPUT_PATH, linesAmount, lines, maxLineLen);

    VoidBubbleSort(lines, linesAmount, sizeof(String), &ComparatorOriginal);
    WriteToFile(OUTPUT_PATH, linesAmount, lines, 0);



    free(buffer);
    free(lines);

    return 0;
}

//==============================================================================

size_t ReadFileSize(const char* fileName) {
    assert(fileName);

    struct stat fileStats;
    int returnValue = stat(fileName, &fileStats);

    assert(returnValue != -1);

    return fileStats.st_size;
}

size_t ReadFromFileToBuffer(const char* fileName, char* buffer,
                            const size_t bufferSize            ) {
    assert(fileName);
    assert(buffer);

    FILE* file = fopen(fileName, "r");

    if (file == NULL) {
        printf("ERROR WHILE OPENING INPUT FILE\n");
        return 0;
    }


    size_t charRead = fread(buffer, sizeof(char), bufferSize, file);

    fclose(file);

    return charRead;
}

//==============================================================================

size_t BufferSplit(char* buffer) { // TODO UNITE indexing and \r catching
    assert(buffer);

    size_t linesAmount = 0;

    while((buffer = strchr(buffer, '\n')) != NULL) {
        *buffer = '\0';
        buffer++;

        linesAmount++;
    }

    return linesAmount;
}

void LinesIndexing(char* buffer, const size_t bufferSize, String* lines,
                   int* maxLineLen                                      ) {
    assert(buffer);
    assert(lines);

    size_t position = 0;
    size_t stringNum = 0;

    while (position < bufferSize) {
        int len = 0;
        lines[stringNum].str = buffer + position;

        while (buffer[position] != '\0') {
            position++;
            len++;
        }
        position++;

        lines[stringNum].len = len;

        *maxLineLen = max(len, *maxLineLen);

        stringNum++;
    }
}

//==============================================================================

void CreateOutputFile(const char* fileName) {
    assert(fileName);

    FILE* file = fopen(fileName, "w");

    if(file == NULL) {
        printf("ERROR WHILE CREATING OUTPUT FILE\n");
    }

    fclose(file);
}

void WriteToFile(const char* fileName, const size_t linesAmount,
                 const String* lines, int maxLineLen            ) { // TODO add null string print and name of sorting
    assert(fileName);
    assert(lines);

    FILE* file = fopen(fileName, "a");

    if(file == NULL) {
        printf("ERROR WHILE OPENING OUTPUT FILE\n");
    }

    for (size_t i = 0; i < linesAmount; i++) {
        if (lines[i].len == 0) {
            continue;
        }
        fprintf(file, "%*s\n", maxLineLen, lines[i].str);
    }

    fprintf(file, "========================================================\n");

    fclose(file);
}

//==============================================================================

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

    return ToLower(str_a[i_a]) - ToLower(str_b[i_b]);
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

    return ToLower(str_a[i_a]) - ToLower(str_b[i_b]);
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
