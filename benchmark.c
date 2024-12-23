#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/resource.h>
#include "benchmark.h"

void benchmark(Dataset1Row *data, size_t n, Comparator comparator, const char *description, const char *sortType, const char *datasetName, BenchmarkResult *result) {
    // Skip InsertionSort for datasets larger than 100 thousand rows
    if (strcmp(sortType, "InsertionSort") == 0 && n > 100000) {
        result->timeTaken = -1.0; // Mark as skipped
        snprintf(result->description, sizeof(result->description), "%s -", description);
        snprintf(result->sortType, sizeof(result->sortType), "%s", sortType);
        snprintf(result->datasetName, sizeof(result->datasetName), "%s", datasetName);
        return;
    }

    int qsortComparator(const void *a, const void *b) {
        return comparator((const Dataset1Row *)a, (const Dataset1Row *)b);
    }

    struct timespec start, end;

    Dataset1Row *dataCopy = malloc(n * sizeof(Dataset1Row));
    if (!dataCopy) {
        perror("malloc");
        return;
    }
    memcpy(dataCopy, data, n * sizeof(Dataset1Row));

    clock_gettime(CLOCK_MONOTONIC, &start);

    if (strcmp(sortType, "HeapSort") == 0) {
        heapSort(dataCopy, n, comparator);
    } else if (strcmp(sortType, "QuickSort") == 0) {
        quickSort(dataCopy, n, comparator);
    } else if (strcmp(sortType, "MergeSort") == 0) {
        mergeSort(dataCopy, n, comparator);
    } else if (strcmp(sortType, "RadixSort") == 0) {
        if (strcmp(description, "Sorting by Value") == 0) {
            radixSort(dataCopy, n, comparator);
        } else {
            result->timeTaken = -1.0; // Mark as skipped
            snprintf(result->description, sizeof(result->description), "%s", description);
            snprintf(result->sortType, sizeof(result->sortType), "%s", sortType);
            snprintf(result->datasetName, sizeof(result->datasetName), "%s", datasetName);
            free(dataCopy);
            return;
        }
    } else if (strcmp(sortType, "TimSort") == 0) {
        timSort(dataCopy, n, comparator);
    } else if (strcmp(sortType, "ShellSort") == 0) {
        shellSort(dataCopy, n, comparator);
    } else if (strcmp(sortType, "HybridQuickSort") == 0) {
        hybridQuickSort(dataCopy, n, comparator);
    } else if (strcmp(sortType, "CountingSort") == 0) {
        if (strcmp(description, "Sorting by Value") == 0) {
            countingSort(dataCopy, n, comparator);
        } else {
            result->timeTaken = -1.0; // Mark as skipped
            snprintf(result->description, sizeof(result->description), "%s", description);
            snprintf(result->sortType, sizeof(result->sortType), "%s", sortType);
            snprintf(result->datasetName, sizeof(result->datasetName), "%s", datasetName);
            free(dataCopy);
            return;
        }
    } else if (strcmp(sortType, "InsertionSort") == 0) {
        insertionSort(dataCopy, n, comparator);
    } else if (strcmp(sortType, "BubbleSort") == 0) {
        if (n > 100000) { // Skip for datasets larger than 1 million rows
            result->timeTaken = -1.0; // Mark as skipped
            snprintf(result->description, sizeof(result->description), "%s", description);
            snprintf(result->sortType, sizeof(result->sortType), "%s", sortType);
            snprintf(result->datasetName, sizeof(result->datasetName), "%s", datasetName);
            free(dataCopy);
            return;
        }
        bubbleSort(dataCopy, n, comparator);
    } else if (strcmp(sortType, "Qsort") == 0) {
        qsort(dataCopy, n, sizeof(Dataset1Row), qsortComparator);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    // Convert elapsed time to milliseconds
    double elapsedTimeInSeconds = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    result->timeTaken = elapsedTimeInSeconds * 1000.0; // Convert to milliseconds

    snprintf(result->description, sizeof(result->description), "%s", description);
    snprintf(result->sortType, sizeof(result->sortType), "%s", sortType);
    snprintf(result->datasetName, sizeof(result->datasetName), "%s", datasetName);

    free(dataCopy);
}
