#include <stdlib.h>
#include <string.h>

#include "heapsort.h"

void heapify(Dataset1Row arr[], size_t n, size_t i, Comparator comparator) {
    size_t largest = i;
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;

    if (left < n && comparator(&arr[left], &arr[largest]) > 0) {
        largest = left;
    }
    if (right < n && comparator(&arr[right], &arr[largest]) > 0) {
        largest = right;
    }

    if (largest != i) {
        Dataset1Row temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        heapify(arr, n, largest, comparator);
    }
}

void heapSort(Dataset1Row arr[], size_t n, Comparator comparator) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i, comparator);
    }

    for (size_t i = n - 1; i > 0; i--) {
        Dataset1Row temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0, comparator);
    }
}