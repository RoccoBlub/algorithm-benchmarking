#include <stdlib.h>
#include <string.h>

#include "mergesort.h"

// Merge two subarrays into a sorted array
void merge(Dataset1Row arr[], size_t left, size_t mid, size_t right, Comparator comparator) {
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;

    // Temporary arrays for left and right halves
    Dataset1Row* leftArr = (Dataset1Row*)malloc(n1 * sizeof(Dataset1Row));
    Dataset1Row* rightArr = (Dataset1Row*)malloc(n2 * sizeof(Dataset1Row));

    // Copy data to temporary arrays
    for (size_t i = 0; i < n1; i++) {
        leftArr[i] = arr[left + i];
    }
    for (size_t j = 0; j < n2; j++) {
        rightArr[j] = arr[mid + 1 + j];
    }

    // Merge temporary arrays back into the original array
    size_t i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (comparator(&leftArr[i], &rightArr[j]) <= 0) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // Copy any remaining elements from leftArr
    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    // Copy any remaining elements from rightArr
    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }

    // Free temporary arrays
    free(leftArr);
    free(rightArr);
}

// Recursive MergeSort function
void mergeSortHelper(Dataset1Row arr[], size_t left, size_t right, Comparator comparator) {
    if (left < right) {
        size_t mid = left + (right - left) / 2;

        // Sort left and right halves
        mergeSortHelper(arr, left, mid, comparator);
        mergeSortHelper(arr, mid + 1, right, comparator);

        // Merge sorted halves
        merge(arr, left, mid, right, comparator);
    }
}

// Main MergeSort function
void mergeSort(Dataset1Row arr[], size_t n, Comparator comparator) {
    if (n > 1) {
        mergeSortHelper(arr, 0, n - 1, comparator);
    }
}
