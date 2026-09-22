#include <stdio.h>

//================================================================================================================

// void PrintMtx1(int data[][], size_t sizeX, size_t sizeY);
void PrintMtx2(int* data, size_t sizeX, size_t sizeY);
void PrintMtx3(int* data, size_t sizeX, size_t sizeY);
void PrintTriangleArray(int* data, size_t lenOfSide);

//================================================================================================================

int main() {

    int data[5] = {10, 20, 30};

    printf("%d <- data[2]\n", data[2]);
    printf("%d <- *(data + 2)\n", *(data + 2));
    printf("%d <- *(int*)((size_t)data + 2 * sizeof(int))\n", *(int*)((size_t)data + 2 * sizeof(int)));
    printf("\n");

    int matrix[5][4] = {
                        {10, 11, 12, 13},
                        {20, 21, 22, 23},
                        {30, 31, 32, 33},
                        {40, 41, 42, 43},
                        {50, 51, 52, 53}
                       };

    printf("%d <- matrix[4][2]\n", matrix[4][2]);
    printf("%d <- *((int*)matrix + 4 * sizeof(matrix[0]) + 2)\n", *((int*)matrix + 4 * sizeof(matrix[0]) / sizeof(int) + 2));
    printf("\n");

    // PrintMtx1(matrix, 5, 4);
    PrintMtx2((int*)matrix, 4, 5);
    printf("\n");
    PrintMtx3((int*)matrix, 4, 5);


    return 0;
}

//================================================================================================================

/*
void PrintMtx1(int data[][], size_t sizeX, size_t sizeY) {
    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            printf("%d ", data[y][x]); // data[y][x] = *((int*)data + y * unknownSizeX + x)
        }
        printf("\n");
    }
}
*/

void PrintMtx2(int* data, size_t sizeX, size_t sizeY) {
    for (size_t y = 0; y < sizeY; y++) {
        for (size_t x = 0; x < sizeX; x++) {
            printf("%d ", *(data + y * sizeX + x));
        }
        printf("\n");
    }
}

void PrintMtx3(int* data, size_t sizeX, size_t sizeY) {
    for (size_t y = 0; y < sizeY; y++) {
        for (size_t x = 0; x < sizeX; x++) {
            printf("%d ", data[y * sizeX + x]);
        }
        printf("\n");
    }
}

//----------------------------------------------------------------------------------------------------------------

void PrintTriangleArray(int* data, size_t lenOfSide) {
    for (size_t y = 0; i < lenOfSide; i++) {
        for (size_t x = ) {
            printf(" ");
        }
    }
}

// TODO сделать вывод бинома ньютона
