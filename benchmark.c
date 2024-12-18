#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/resource.h>
#include "benchmark.h"

void benchmark(Dataset1Row *data, size_t n, Comparator comparator, const char *description, const char *sortType, const char *datasetName, BenchmarkResult *result) {
    // Allocate memory for a copy of the dataset
    Dataset1Row *dataCopy = (Dataset1Row*)malloc(n * sizeof(Dataset1Row));
    if (!dataCopy) {
        perror("malloc");
        return;
    }
    memcpy(dataCopy, data, n * sizeof(Dataset1Row));

    // Start the timer
    clock_t start = clock();

    // Perform the sorting
    if (strcmp(sortType, "HeapSort") == 0) {
        heapSort(dataCopy, n, comparator);
    } else if (strcmp(sortType, "QuickSort") == 0) {
        quickSort(dataCopy, n, comparator);
    } else if (strcmp(sortType, "MergeSort") == 0) {
        mergeSort(dataCopy, n, comparator);
    }

    // Stop the timer
    clock_t end = clock();

    // Record benchmark results
    result->timeTaken = (double)(end - start) / CLOCKS_PER_SEC;
    snprintf(result->description, sizeof(result->description), "%s", description);
    snprintf(result->sortType, sizeof(result->sortType), "%s", sortType);
    snprintf(result->datasetName, sizeof(result->datasetName), "%s", datasetName);

    // Clean up
    free(dataCopy);
}