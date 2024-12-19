#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "insertionsort.h"

void insertionSort(Dataset1Row *arr, size_t n, Comparator comparator) {
    for (size_t i = 1; i < n; i++) {
        Dataset1Row temp = arr[i];
        size_t j = i;

        // Shift elements of arr[0..i-1], that are greater than temp, to one position ahead
        while (j > 0 && comparator(&arr[j - 1], &temp) > 0) {
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = temp;
    }
}

