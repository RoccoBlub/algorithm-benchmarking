#include "bubblesort.h"
#include <stdbool.h> // Include for bool, true, false


void bubbleSort(Dataset1Row *arr, size_t n, Comparator comparator) {
    for (size_t i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (size_t j = 0; j < n - i - 1; j++) {
            if (comparator(&arr[j], &arr[j + 1]) > 0) {
                // Swap arr[j] and arr[j + 1]
                Dataset1Row temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        // If no elements were swapped, the array is already sorted
        if (!swapped) break;
    }
}
