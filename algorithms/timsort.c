#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "timsort.h"
#include "insertionsort.h"

#define RUN 32  // Size of a run

// Merge two sorted subarrays (static to limit scope)
static void merge(Dataset1Row *arr, size_t left, size_t mid, size_t right, Comparator comparator) {
    size_t len1 = mid - left + 1, len2 = right - mid;
    Dataset1Row *leftArr = malloc(len1 * sizeof(Dataset1Row));
    Dataset1Row *rightArr = malloc(len2 * sizeof(Dataset1Row));

    if (!leftArr || !rightArr) {
        printf("Failed to allocate memory for merge");
        free(leftArr);
        free(rightArr);
        return;
    }

    // Copy data to temporary arrays
    memcpy(leftArr, arr + left, len1 * sizeof(Dataset1Row));
    memcpy(rightArr, arr + mid + 1, len2 * sizeof(Dataset1Row));

    size_t i = 0, j = 0, k = left;
    while (i < len1 && j < len2) {
        if (comparator(&leftArr[i], &rightArr[j]) <= 0) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }

    // Copy remaining elements
    while (i < len1) arr[k++] = leftArr[i++];
    while (j < len2) arr[k++] = rightArr[j++];

    free(leftArr);
    free(rightArr);
}

// TimSort main function
void timSort(Dataset1Row *arr, size_t n, Comparator comparator) {
    // Step 1: Sort small runs using Insertion Sort
    for (size_t i = 0; i < n; i += RUN) {
        size_t right = (i + RUN - 1 < n - 1) ? i + RUN - 1 : n - 1;
        insertionSort(arr + i, right - i + 1, comparator);
    }

    // Step 2: Merge sorted runs
    for (size_t size = RUN; size < n; size *= 2) {
        for (size_t left = 0; left < n; left += 2 * size) {
            size_t mid = left + size - 1;
            size_t right = (left + 2 * size - 1 < n - 1) ? left + 2 * size - 1 : n - 1;

            if (mid < right) {
                merge(arr, left, mid, right, comparator);
            }
        }
    }
}
