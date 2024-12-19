#include "shellsort.h"

void shellSort(Dataset1Row *arr, size_t n, Comparator comparator) {
    // Start with a large gap, reduce the gap each iteration
    for (size_t gap = n / 2; gap > 0; gap /= 2) {
        // Perform gapped insertion sort for this gap size
        for (size_t i = gap; i < n; i++) {
            Dataset1Row temp = arr[i];
            size_t j = i;

            // Shift earlier gap-sorted elements until the correct location for temp is found
            while (j >= gap && comparator(&arr[j - gap], &temp) > 0) {
                arr[j] = arr[j - gap];
                j -= gap;
            }

            // Place temp at its correct location
            arr[j] = temp;
        }
    }
}
