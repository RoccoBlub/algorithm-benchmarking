#include "countingsort.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

void countingSort(Dataset1Row *arr, size_t n, Comparator comparator) {
    if (n == 0) return;

    // Find the range of values (min and max)
    int min = (int)arr[0].value;
    int max = (int)arr[0].value;

    for (size_t i = 1; i < n; i++) {
        if ((int)arr[i].value < min) min = (int)arr[i].value;
        if ((int)arr[i].value > max) max = (int)arr[i].value;
    }

    int range = max - min + 1;

    // Allocate memory for count array and initialize to 0
    int *count = calloc(range, sizeof(int));
    if (!count) {
        printf("Failed to allocate memory for Counting Sort");
        return;
    }

    // Count occurrences of each value
    for (size_t i = 0; i < n; i++) {
        count[(int)arr[i].value - min]++;
    }

    // Calculate cumulative counts
    for (int i = 1; i < range; i++) {
        count[i] += count[i - 1];
    }

    // Allocate memory for output array
    Dataset1Row *output = malloc(n * sizeof(Dataset1Row));
    if (!output) {
        perror("Failed to allocate memory for Counting Sort output");
        free(count);
        return;
    }

    // Build the sorted array
    for (size_t i = n; i > 0; i--) {
        int index = (int)arr[i - 1].value - min;
        output[count[index] - 1] = arr[i - 1];
        count[index]--;
    }

    // Copy the sorted data back to the original array
    for (size_t i = 0; i < n; i++) {
        arr[i] = output[i];
    }

    // Free memory
    free(count);
    free(output);
}
