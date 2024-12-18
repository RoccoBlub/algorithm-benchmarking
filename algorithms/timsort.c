#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "timsort.h"

#define RUN 32

static Dataset1Row* leftArr = NULL;
static Dataset1Row* rightArr = NULL;
static size_t currentBufferSize = 0;

static void ensureBufferSize(size_t size) {
    if (size > currentBufferSize) {
        free(leftArr);
        free(rightArr);
        leftArr = (Dataset1Row*)malloc(size * sizeof(Dataset1Row));
        rightArr = (Dataset1Row*)malloc(size * sizeof(Dataset1Row));
        if (!leftArr || !rightArr) {
            perror("Failed to allocate memory for merge buffers");
            exit(EXIT_FAILURE);
        }
        currentBufferSize = size;
    }
}

static void insertionSort(Dataset1Row arr[], size_t left, size_t right, Comparator comparator) {
    for (size_t i = left + 1; i <= right; i++) {
        Dataset1Row key = arr[i];
        size_t j = i;
        while (j > left && comparator(&arr[j - 1], &key) > 0) {
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = key;
    }
}

static void merge(Dataset1Row arr[], size_t left, size_t mid, size_t right, size_t n, Comparator comparator) {
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;

    ensureBufferSize(n1 > n2 ? n1 : n2);

    printf("Merging: left = %zu, mid = %zu, right = %zu, n = %zu\n", left, mid, right, n);
    printf("Buffer sizes: n1 = %zu, n2 = %zu\n", n1, n2);

    if (!leftArr || !rightArr) {
        printf("Error: Buffers not allocated properly\n");
        exit(EXIT_FAILURE);
    }

    // Copy data into buffers
    for (size_t i = 0; i < n1; i++) {
        if (left + i >= n) {
            printf("Error: Out-of-bounds access in leftArr at left + i = %zu, n = %zu\n", left + i, n);
            exit(EXIT_FAILURE);
        }
        leftArr[i] = arr[left + i];
    }
    for (size_t j = 0; j < n2; j++) {
        if (mid + 1 + j >= n) {
            printf("Error: Out-of-bounds access in rightArr at mid + 1 + j = %zu, n = %zu\n", mid + 1 + j, n);
            exit(EXIT_FAILURE);
        }
        rightArr[j] = arr[mid + 1 + j];
    }

    // Merge the arrays
    size_t i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (k >= n) {
            printf("Error: Out-of-bounds write while merging: k = %zu, n = %zu\n", k, n);
            exit(EXIT_FAILURE);
        }
        arr[k++] = (comparator(&leftArr[i], &rightArr[j]) <= 0) ? leftArr[i++] : rightArr[j++];
    }

    while (i < n1) {
        if (k >= n) {
            printf("Error: Out-of-bounds write while copying leftArr: k = %zu, n = %zu\n", k, n);
            exit(EXIT_FAILURE);
        }
        arr[k++] = leftArr[i++];
    }

    while (j < n2) {
        if (k >= n) {
            printf("Error: Out-of-bounds write while copying rightArr: k = %zu, n = %zu\n", k, n);
            exit(EXIT_FAILURE);
        }
        arr[k++] = rightArr[j++];
    }
}


void timSort(Dataset1Row arr[], size_t n, Comparator comparator) {
    for (size_t i = 0; i < n; i += RUN) {
        size_t right = (i + RUN - 1 < n) ? i + RUN - 1 : n - 1;
        insertionSort(arr, i, right, comparator);
        printf("InsertionSort progress: %zu%%\n", (i * 100) / n);
    }

    for (size_t size = RUN; size < n; size *= 2) {
        for (size_t left = 0; left < n; left += 2 * size) {
            size_t mid = left + size - 1;
            size_t right = (left + 2 * size - 1 < n) ? left + 2 * size - 1 : n - 1;

            if (mid < right) {
                printf("Merging left: %zu, mid: %zu, right: %zu\n", left, mid, right);
                merge(arr, left, mid, right, n, comparator); // Pass n
            }
        }
    }

    free(leftArr);
    free(rightArr);
}

