#include <stdio.h>
#include <stdlib.h>

#include "../lesson1/MyString.c"
#include "../lesson4/FunctionPointer.c"

const int MAX_STRINGS_COUNT = 10000;
const int BUFFER_SIZE = 10000;

const char* INPUT_PATH = "formatted_onegin.txt";
const char* OUTPUT_PATH = "sortedOnegin.txt";

//================================================================================================================

int ReadFromFile(const char* path, char** text, char** index);
void WriteToFile(const char* path, int linesCount, char** index);

int StrComparator(cvoid* ptr_a, cvoid* ptr_b);

//================================================================================================================

int main() {
    char* text[MAX_STRINGS_COUNT] = {};
    char* index[MAX_STRINGS_COUNT] = {};

    int linesCount = ReadFromFile(INPUT_PATH, text, index);

    for (int i = 0; i < linesCount; i++) {
        printf("%s\n", *(index + i));
    }

    printf("SORTING...\n");

    VoidBubbleSort(index, linesCount, sizeof(char*), &StrComparator);

    for (int i = 0; i < linesCount; i++) {
        printf("%s\n", *(index + i));
    }

    WriteToFile(OUTPUT_PATH, linesCount, index);


    return 0;
}

//================================================================================================================

int ReadFromFile(const char* path, char** text, char** index) { // TODO add errors opening files
    FILE* file = fopen(path, "r");

    if (file == NULL) {
        printf("NOT INPUT FILE\n");
        return 0;
    }

    int linesCount = 0;
    char* buffer = (char*)calloc(BUFFER_SIZE, sizeof(char));

    while(linesCount < MAX_STRINGS_COUNT && (fgets(buffer, BUFFER_SIZE, file) != NULL)) {
        size_t lenOfLine = Strlen(buffer);

        if (lenOfLine > 0 && buffer[lenOfLine - 1] == '\n') {
            buffer[lenOfLine - 1] = '\0';
        }

        text[linesCount] = Strdup(buffer);
        index[linesCount] = text[linesCount];

        linesCount++;
    }


    free(buffer);
    fclose(file);

    return linesCount;
}

void WriteToFile(const char* path, int linesCount, char** index) {
    FILE* file = fopen(path, "w");

    for (int i = 0; i < linesCount; i++) {
        fprintf(file, "%s\n", index[i]);
    }

    fclose(file);
}


int StrComparator(cvoid* ptr_a, cvoid* ptr_b) {
    const char* str_a = *(const char**)ptr_a;
    const char* str_b = *(const char**)ptr_b;

    printf("str_a: %s, str_b: %s\n", str_a, str_b);

    return strcmp(str_a, str_b);
}
