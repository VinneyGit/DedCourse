#include <stdio.h>
#include <stdbool.h>

//================================================================================================================

void PrintArray(int array[], size_t size);

void SwapInt(int* x, int* y);
void BubbleSort(int array[], size_t size);

void QuickSort(int array[], size_t leftEdge, size_t rightEdge);

//================================================================================================================

int main() {

    int arr[] = {1, 7, 5, 3, 9, 6, 7, 5, 2};
    size_t size = sizeof(arr) / sizeof(arr[0]);


    PrintArray(arr, size);
    BubbleSort(arr, size);
    PrintArray(arr, size);

    return 0;
}

//================================================================================================================

void PrintArray(int array[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

//----------------------------------------------------------------------------------------------------------------

void SwapInt(int* x, int* y) {
    int temp = *y;
    *y = *x;
    *x = temp;
}

void BubbleSort(int array[], size_t size) {
    for (size_t n = 0; n < size; n++) {
        bool wasSwapped = false;
        for (size_t i = 0; i < size - n - 1; i++) {
            if (array[i] > array[i + 1]) {
                wasSwapped = true;
                // printf("\narray: ");
                // PrintArray(array, size);

                // printf("swap: %d, %d\n", array[i], array[i + 1]);
                SwapInt(&array[i], &array[i + 1]);
            }
        }
        if (!wasSwapped) {
            break;
        }
    }
}
