#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>


typedef const int  cint;
typedef const void cvoid;

//================================================================================================================

void PrintArray(int array[], size_t size);

int ComparatorVer1(const int a, const int b);
int ComparatorVer2(cint* ptr_a, cint* ptr_b);
int ComparatorVer3(cvoid* ptr_a, cvoid* ptr_b);

void SwapInt(int* x, int* y);
void BubbleSort(int array[], size_t size, int (*Comparator)(cvoid* a, cvoid* b));

void VoidBubbleSort(void* data, size_t size, size_t sizeOfElement, int Comparator(cvoid* a, cvoid* b));

//================================================================================================================

/*
int main() {
    srand((unsigned int)time(NULL));

    int arr[10] = {0};
    size_t size = sizeof(arr) / sizeof(arr[0]);

    printf("Size of array: %zd\n", size);

    for (size_t i = 0; i < size; i++) {
        *(arr + i) = rand() % 100;
    }


    printf("Unsorted array:\t\t");
    PrintArray(arr, size);

    VoidBubbleSort((void*)arr, size, sizeof(int), &ComparatorVer3);

    printf("Sorted array:\t\t");
    PrintArray(arr, size);

    return 0;
}
*/

//================================================================================================================

void PrintArray(int array[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

//----------------------------------------------------------------------------------------------------------------

int ComparatorVer1(cint a, cint b) {
    return (a > b) - (a < b);
}

int ComparatorVer2(cint* ptr_a, cint* ptr_b) {
    const int a = *ptr_a;
    const int b = *ptr_b;

    return (a > b) - (a < b);
}

int ComparatorVer3(cvoid* ptr_a, cvoid* ptr_b) {
    const int a = *(const int*)ptr_a;
    const int b = *(const int*)ptr_b;

    return (a > b) - (a < b);
}

//----------------------------------------------------------------------------------------------------------------

void SwapInt(int* x, int* y) {
    int temp = *y;
    *y = *x;
    *x = temp;
}

void BubbleSort(int array[], size_t size, int (*Comparator)(cvoid* a, cvoid* b)) {
    for (size_t n = 0; n < size; n++) {
        bool wasSwapped = false;
        for (size_t i = 0; i < size - n - 1; i++) {
            if ((*Comparator)(array + i, array + i + 1) > 0) {
                wasSwapped = true;
                SwapInt(array + i, array + i + 1);
            }
        }
        if (!wasSwapped) {
            break;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------

void VoidBubbleSort(void* array, size_t size, size_t sizeOfElement, int Comparator(cvoid* a, cvoid* b)) {
    for (size_t n = 0; n < size; n++) {
        bool wasSwapped = false;
        for (size_t i = 0; i < size - n - 1; i++) {

            void* ptr_a = (void*)((uint8_t*)array + i * sizeOfElement);
            void* ptr_b = (void*)((uint8_t*)array + (i + 1) * sizeOfElement);

            if ((*Comparator)(ptr_a, ptr_b) > 0) {
                wasSwapped = true;

                void* temp = calloc(1, sizeOfElement);
                memcpy(temp, ptr_a, sizeOfElement);
                memcpy(ptr_a, ptr_b, sizeOfElement);
                memcpy(ptr_b, temp, sizeOfElement);

                free(temp);
            }
        }
        if (!wasSwapped) {
            break;
        }
    }
}
