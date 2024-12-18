#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "insertionsort.h"

void insertionSort(Dataset1Row arr[], size_t n, Comparator comparator) {
    size_t progress = 0; // Track progress for logging
    for (size_t i = 1; i < n; i++) {
        Dataset1Row key = arr[i];
        size_t j = i;

        // Move elements of arr[0..i-1], that are greater than key, one position ahead of their current position
        while (j > 0 && comparator(&arr[j - 1], &key) > 0) {
            arr[j] = arr[j - 1];
            j--;
        }

        arr[j] = key;

        // Log progress at intervals
        if (i > progress + n / 100) { // Update progress every 1% of the dataset
            printf("InsertionSort: %zu%% complete\n", (i * 100) / n);
            progress = i;
            fflush(stdout);
        }
    }
}
