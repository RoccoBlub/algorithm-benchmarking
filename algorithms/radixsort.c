#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "radixsort.h"

void countingSortForRadix(Dataset1Row *arr, size_t n, int place, Comparator comparator) {
    Dataset1Row *output = malloc(n * sizeof(Dataset1Row));
    if (!output) {
        printf("Failed to allocate memory for counting sort");
        return;
    }

    int count[10] = {0};

    // Count occurrences of each digit in the current place value
    for (size_t i = 0; i < n; i++) {
        int digit = ((int)arr[i].value / place) % 10; // Adjust if using `transaction_count` or another numeric field
        count[digit]++;
    }

    // Compute cumulative count
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    // Place elements into the output array in sorted order
    for (int i = n - 1; i >= 0; i--) {
        int digit = ((int)arr[i].value / place) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    // Copy the sorted elements back to the original array
    for (size_t i = 0; i < n; i++) {
        arr[i] = output[i];
    }

    free(output);
}

void radixSort(Dataset1Row *arr, size_t n, Comparator comparator) {
    // Find the maximum value to determine the number of digits
    int max = (int)arr[0].value;
    for (size_t i = 1; i < n; i++) {
        if ((int)arr[i].value > max) {
            max = (int)arr[i].value;
        }
    }

    // Perform counting sort for each digit place (1s, 10s, 100s, ...)
    for (int place = 1; max / place > 0; place *= 10) {
        countingSortForRadix(arr, n, place, comparator);
    }
}
