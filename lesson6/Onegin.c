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
    size_t  charRead;
    String* lines;
    size_t  linesAmount;
    int     maxLineLen;
};

enum error {
    OK = 0,
    ERROR = -1,
};

// TODO ??? add windows CRLF
// TODO ??? Filedescriptor

//==============================================================================

error   GetFileNames         (int argc, char ** argv,
                              char** inputPath, char** outputPath);

//------------------------------------------------------------------------------

error   ReadFileSize         (const char* fileName, Text* text);
error   ReadFromFileToBuffer (const char* fileName, Text* text);

//------------------------------------------------------------------------------

error   LinesIndexing        (Text* text);

//------------------------------------------------------------------------------

error   CreateOutputFile     (const char* fileName, FILE** fileToWrite);
error   WriteToFile          (FILE* file, Text* text, int maxLineLen);

//------------------------------------------------------------------------------

int     ComparatorBegin      (const void* ptr_a, const void* ptr_b);
int     ComparatorEnd        (const void* ptr_a, const void* ptr_b);
int     ComparatorOriginal   (const void* ptr_a, const void* ptr_b);

//==============================================================================

int main(int argc, char** argv) {

    char* INPUT_PATH = NULL;
    char* OUTPUT_PATH = NULL;

    if (GetFileNames(argc, argv, &INPUT_PATH, &OUTPUT_PATH) == ERROR) {
        return -1;
    }
    printf("Input: %s\n", INPUT_PATH);
    printf("Output: %s\n", OUTPUT_PATH);


    Text text = {};


    if (ReadFileSize(INPUT_PATH, &text) == ERROR) {
        return -1;
    }
    printf("Size of file in bytes is: %zu\n", text.bufferSize);

    if (ReadFromFileToBuffer(INPUT_PATH, &text) == ERROR) {
        return -1;
    }

    if (LinesIndexing(&text) == ERROR) {
        return -1;
    }

    printf("linesAmount: %5zu\n==========================\n", text.linesAmount);


    FILE* OutputFile = NULL;

    if (CreateOutputFile(OUTPUT_PATH, &OutputFile) == ERROR) {
        return -1;
    }

    qsort(text.lines, text.linesAmount, sizeof(String), &ComparatorBegin);
    if (WriteToFile(OutputFile, &text, 0) == ERROR) {
        return -1;
    }

    qsort(text.lines, text.linesAmount, sizeof(String), &ComparatorEnd);
    if (WriteToFile(OutputFile, &text, text.maxLineLen) == ERROR) {
        return -1;
    }

    VoidBubbleSort(text.lines, text.linesAmount,
                                        sizeof(String), &ComparatorOriginal);
    if (WriteToFile(OutputFile, &text, 0) == ERROR) {
        return -1;
    }

    fclose(OutputFile);


    free(text.buffer);
    free(text.lines);

    return 0;
}

//==============================================================================

error GetFileNames (int argc, char ** argv, char** inputPath, char** outputPath) {
    if (argc == 1) {
        printf("Please, put path for INPUT and OUTPUT files\n");
        return ERROR;
    }

    else if (argc == 2) {
        printf("Please, put path for OUTPUT file\n");
        return ERROR;
    }

    *inputPath = *(argv + 1);
    *outputPath = *(argv + 2);

    return OK;
}

//------------------------------------------------------------------------------

error ReadFileSize(const char* fileName, Text* text) {
    assert(fileName);

    struct stat fileStats;
    int returnValue = stat(fileName, &fileStats);

    if (returnValue == -1) {
        printf("ERROR WHILE READING FILE'S DATA\n");
        return ERROR;
    }

    text->bufferSize = fileStats.st_size;

    return OK;
}

error ReadFromFileToBuffer(const char* fileName, Text* text) {
    assert(fileName);
    assert(text);

    text->buffer = (char*)calloc(text->bufferSize, sizeof(char));

    if (text->buffer == NULL) {
        printf("ERROR WITH MEMORY ALLOCATION FOR BUFFER\n");
        return ERROR;
    }

    FILE* file = fopen(fileName, "r");

    if (file == NULL) {
        printf("ERROR WHILE OPENING INPUT FILE\n");
        return ERROR;
    }

    text->charRead = fread(text->buffer, sizeof(char), text->bufferSize, file);

    fclose(file);

    return OK;
}

//==============================================================================

error LinesIndexing(Text* text) {
    assert(text);
    assert(text->buffer);
    assert(text->bufferSize);

    size_t linesAmount = 0;

    size_t sizeOfLinesArray = 1;
    if ((text->lines = (String*)calloc(sizeOfLinesArray,
                                                    sizeof(String))) == NULL) {
        printf("ERROR WITH CALLOC\n");
        return ERROR;
    }

    size_t position = 0;
    size_t stringNum = 0;

    String* newLines = NULL;
    size_t newSizeOfLinesArray = 0;

    while (position < text->bufferSize && text->buffer[position] != '\0') {
        if (sizeOfLinesArray - linesAmount <= 2) {
            newSizeOfLinesArray = sizeOfLinesArray * 2;
            if((newLines = (String*)realloc(text->lines,
                               newSizeOfLinesArray * sizeof(String))) == NULL) {
                printf("ERROR WITH REALLOC\n");
                return ERROR;
            }
        }

        text->lines = newLines;
        sizeOfLinesArray = newSizeOfLinesArray;

        int len = 0;
        text->lines[stringNum].str = text->buffer + position;

        while (text->buffer[position] != '\n') {
            position++;
            len++;
        }
        text->buffer[position] = '\0';

        position++;

        linesAmount++;

        text->lines[stringNum].len = len;

        text->maxLineLen = max(len, text->maxLineLen);

        stringNum++;
    }

    text->linesAmount = linesAmount;

    return OK;
}

//==============================================================================

error CreateOutputFile(const char* fileName, FILE** fileToWrite) {
    assert(fileName);

    FILE* file = fopen(fileName, "w");

    if (file == NULL) {
        printf("ERROR WHILE CREATING OUTPUT FILE\n");
        return ERROR;
    }

    *fileToWrite = file;

    return OK;
}
// TODO add null string print and name of sorting
error WriteToFile(FILE* file, Text* text, int maxLineLen) {
    assert(file);
    assert(text->lines);

    if (file == NULL) {
        printf("ERROR WHILE OPENING OUTPUT FILE\n");
        return ERROR;
    }

    for (size_t i = 0; i < text->linesAmount; i++) {
        if (text->lines[i].len == 0) {
            continue;
        }
        fprintf(file, "%*s\n", maxLineLen, text->lines[i].str);
    }

    fprintf(file, "========================================================\n");

    return OK;
}

//==============================================================================

int ComparatorBegin(const void* ptr_a, const void* ptr_b) {
    assert(ptr_a);
    assert(ptr_b);

    const String a = *(const String*)ptr_a;
    const String b = *(const String*)ptr_b;

    if (a.len == 0 || b.len == 0) {
        return (a.len > b.len) - (a.len < b.len);
    }

    size_t i_a = 0;
    size_t i_b = 0;

    while (i_a < a.len && i_b < b.len) {
        if (!isalpha(a.str[i_a])) {
            i_a++;
            continue;
        }
        if (!isalpha(b.str[i_b])) {
            i_b++;
            continue;
        }
        if (ToLower(a.str[i_a]) != ToLower(b.str[i_b])) {
            return ToLower(a.str[i_a]) - ToLower(b.str[i_b]);
        }
        i_a++;
        i_b++;
    }

    return (a.len > b.len) - (a.len < b.len);
}

int ComparatorEnd(const void* ptr_a, const void* ptr_b) {
    assert(ptr_a);
    assert(ptr_b);

    const String a = *(const String*)ptr_a;
    const String b = *(const String*)ptr_b;

    if (a.len == 0 || b.len == 0) {
        return (a.len > b.len) - (a.len < b.len);
    }

    size_t i_a = a.len - 1;
    size_t i_b = b.len - 1;

    while (i_a > 0 && i_b > 0) {
        if (!isalpha(a.str[i_a])) {
            i_a--;
            continue;
        }
        if (!isalpha(b.str[i_b])) {
            i_b--;
            continue;
        }
        if (ToLower(a.str[i_a]) != ToLower(b.str[i_b])) {
            return ToLower(a.str[i_a]) - ToLower(b.str[i_b]);
        }
        i_a--;
        i_b--;
    }

    return (a.len > b.len) - (a.len < b.len);
}

int ComparatorOriginal(const void* ptr_a, const void* ptr_b) {
    assert(ptr_a);
    assert(ptr_b);

    const String a = *(const String*)ptr_a;
    const String b = *(const String*)ptr_b;

    const char* str_a = a.str;
    const char* str_b = b.str;

    return (str_a > str_b) - (str_a < str_b);
}
