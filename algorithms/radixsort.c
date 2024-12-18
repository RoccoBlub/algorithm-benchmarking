#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../comparators.h"
#include "radixsort.h"

static size_t getMaxStringLength(Dataset1Row arr[], size_t n) {
    size_t maxLength = 0;
    for (size_t i = 0; i < n; i++) {
        size_t length = strlen(arr[i].location);
        if (length > maxLength) {
            maxLength = length;
        }
    }
    return maxLength;
}

static void displayProgress(size_t current, size_t total) {
    int percentage = (int)((double)current / total * 100);
    printf("RadixSort Progress: %d%%\r", percentage);
    fflush(stdout);
}

static void countingSortStrings(Dataset1Row arr[], size_t n, size_t pos) {
    Dataset1Row* output = (Dataset1Row*)malloc(n * sizeof(Dataset1Row));
    if (!output) {
        perror("Failed to allocate output array");
        exit(EXIT_FAILURE);
    }
    int count[256] = {0}; // ASCII characters (256 possible values)

    for (size_t i = 0; i < n; i++) {
        char c = pos < strlen(arr[i].location) ? arr[i].location[strlen(arr[i].location) - pos - 1] : 0;
        count[(unsigned char)c]++;
    }

    for (int i = 1; i < 256; i++) {
        count[i] += count[i - 1];
    }

    for (ssize_t i = n - 1; i >= 0; i--) {
        char c = pos < strlen(arr[i].location) ? arr[i].location[strlen(arr[i].location) - pos - 1] : 0;
        output[count[(unsigned char)c] - 1] = arr[i];
        count[(unsigned char)c]--;
    }

    for (size_t i = 0; i < n; i++) {
        arr[i] = output[i];
    }

    free(output);
}

static void countingSortIntegers(Dataset1Row arr[], size_t n, size_t offset) {
    Dataset1Row* output = (Dataset1Row*)malloc(n * sizeof(Dataset1Row));
    if (!output) {
        perror("Failed to allocate output array");
        exit(EXIT_FAILURE);
    }

    int count[10] = {0};
    int max = 0;

    for (size_t i = 0; i < n; i++) {
        int value = *((int*)((char*)&arr[i] + offset));
        if (value > max) {
            max = value;
        }
    }

    for (int exp = 1; max / exp > 0; exp *= 10) {
        memset(count, 0, sizeof(count));

        for (size_t i = 0; i < n; i++) {
            int value = *((int*)((char*)&arr[i] + offset));
            int digit = (value / exp) % 10;
            count[digit]++;
        }

        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        for (ssize_t i = n - 1; i >= 0; i--) {
            int value = *((int*)((char*)&arr[i] + offset));
            int digit = (value / exp) % 10;
            output[count[digit] - 1] = arr[i];
            count[digit]--;
        }

        for (size_t i = 0; i < n; i++) {
            arr[i] = output[i];
        }

        displayProgress(exp, max);
    }

    free(output);
    printf("\n");
}

void radixSortStrings(Dataset1Row arr[], size_t n) {
    size_t maxLength = getMaxStringLength(arr, n);

    for (size_t pos = 0; pos < maxLength; pos++) {
        countingSortStrings(arr, n, pos);
        displayProgress(pos + 1, maxLength);
    }
    printf("\n");
}

void radixSortComposite(Dataset1Row arr[], size_t n) {
    countingSortIntegers(arr, n, offsetof(Dataset1Row, transaction_count));
    size_t maxLength = getMaxStringLength(arr, n);
    for (size_t pos = 0; pos < maxLength; pos++) {
        countingSortStrings(arr, n, pos);
        displayProgress(pos + 1, maxLength);
    }
    printf("\n");
}

void radixSort(Dataset1Row arr[], size_t n, Comparator comparator) {
    if (comparator == compareNumeric) {
        countingSortIntegers(arr, n, offsetof(Dataset1Row, value));
    } else if (comparator == compareString) {
        radixSortStrings(arr, n);
    } else {
        radixSortComposite(arr, n);
    }
}
