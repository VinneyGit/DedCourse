#include <stdio.h>

//======================================================================================================================

#define maxOnegin(a, b) ((a) > (b) ? (a) : (b))

typedef struct {
    char*   str;
    size_t  len;
} String;

typedef struct {
    char*   buffer;
    size_t  bufferSize;
    size_t  charRead;
    String* lines;
    size_t  linesAmount;
    int     maxLineLen;
} Text;

typedef enum {
    OK      = 0,
    ERROR   = -1
} error;

//======================================================================================================================

error   GetFileNames            (int argc, char ** argv, char** inputPath, char** outputPath);

//----------------------------------------------------------------------------------------------------------------------

error   ReadFileSize            (const char* fileName, Text* text);
error   ReadFromFileToBuffer    (const char* fileName, Text* text);

//----------------------------------------------------------------------------------------------------------------------

void    BufferSplit             (Text* text);
error   LinesIndexing           (Text* text);

//======================================================================================================================

error GetFileNames(int argc, char ** argv, char** inputPath, char** outputPath) {
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

//----------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------

void BufferSplit(Text* text) { //TODO EASIER
    assert(text);

    char* substr = text->buffer;

    while ((substr = strchr(substr, '\n')) != NULL) {
        *substr = '\0';
        substr++;

        text->linesAmount++;
    }
}

error LinesIndexing(Text* text) {
    assert(text);

    text->lines = (String*)calloc(text->linesAmount, sizeof(String));

    if (text->lines == NULL) {
        printf("ERROR WITH MEMORY ALLOCATION FOR LINES\n");
        return ERROR;
    }

    size_t position = 0;
    size_t stringNum = 0;

    while (position < text->bufferSize) {
        int len = 0;
        text->lines[stringNum].str = text->buffer + position;

        while (text->buffer[position] != '\0') {
            position++;
            len++;
        }
        position++;

        text->lines[stringNum].len = len;

        text->maxLineLen = maxOnegin(len, text->maxLineLen);

        stringNum++;
    }

    return OK;
}
