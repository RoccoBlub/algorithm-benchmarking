#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include "dataset.h"
#include "benchmark.h"
#include "comparators.h"
#include "logging.h"

#define INITIAL_CAPACITY 100

typedef struct {
    Dataset1Row *dataset;
    size_t rows;
    Comparator comparison;
    const char *sortType;
    const char *datasetName;
    BenchmarkResult *results;
    size_t *resultIndex;
    size_t datasetIndex;
    pthread_mutex_t *mutex;
} ThreadArgs;

const char *formatTime(double time) {
    static char buffer[16];
    if (time < 0) {
        return "Skipped";
    }
    snprintf(buffer, sizeof(buffer), "%.2f ms", time);
    return buffer;
}

void displayResults(BenchmarkResult results[], size_t resultCount, const char *sortTypes[], size_t sortTypeCount, const char *datasets[], size_t datasetCount) {
    logMessage("+-----------------+-------------------+----------------+\n");
    logMessage("| Algorithm       | Dataset           | Time taken (ms)|\n");
    logMessage("+-----------------+-------------------+----------------+\n");

    for (size_t datasetIdx = 0; datasetIdx < datasetCount; datasetIdx++) {
        const char *datasetName = datasets[datasetIdx];
        // Extract the descriptive part of the dataset name
        const char *shortName = strrchr(datasetName, '/'); // Find the last '/' character
        if (shortName) {
            shortName++; // Move past '/'
        } else {
            shortName = datasetName; // Use the full name if no '/'
        }

        // Remove the file extension (".csv")
        char formattedName[50];
        snprintf(formattedName, sizeof(formattedName), "%.*s", (int)(strrchr(shortName, '.') - shortName), shortName);

        for (size_t sortIdx = 0; sortIdx < sortTypeCount; sortIdx++) {
            double timeTaken = -1.0;

            for (size_t i = 0; i < resultCount; i++) {
                if (strcmp(results[i].sortType, sortTypes[sortIdx]) == 0 &&
                    strcmp(results[i].datasetName, datasetName) == 0) {
                    timeTaken = results[i].timeTaken;
                    break;
                    }
            }

            logMessage("| %-15s | %-17s | %-14s |\n",
                       sortTypes[sortIdx],
                       formattedName,
                       (timeTaken >= 0) ? formatTime(timeTaken) : "-");
        }
        logMessage("+-----------------+-------------------+----------------+\n");
    }
}


void *benchmarkDataset(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;

    logMessage("Thread started for Dataset: %s, Algorithm: %s\n", args->datasetName, args->sortType);

    BenchmarkResult result;
    benchmark(args->dataset, args->rows, args->comparison, "Sorting by Value", args->sortType, args->datasetName, &result);

    pthread_mutex_lock(args->mutex);
    size_t idx = (*args->resultIndex)++;
    args->results[idx] = result;
    pthread_mutex_unlock(args->mutex);

    free(args);
    return NULL;
}

int main() {
    const char *numericDatasets[] = {
        "datasets/sorted.csv",
        "datasets/reverse_sorted.csv",
        "datasets/nearly_sorted.csv",
        "datasets/heavy_duplication.csv"
    };

    const char *sortTypes[] = {"HeapSort", "Qsort", "MergeSort", "RadixSort", "TimSort", "CountingSort"};
    size_t datasetCount = sizeof(numericDatasets) / sizeof(numericDatasets[0]);
    size_t sortTypeCount = sizeof(sortTypes) / sizeof(sortTypes[0]);
    size_t totalResults = datasetCount * sortTypeCount;

    BenchmarkResult *results = (BenchmarkResult *)malloc(totalResults * sizeof(BenchmarkResult));
    if (!results) {
        perror("Failed to allocate memory for results");
        return 1;
    }

    pthread_t threads[totalResults];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    size_t resultIndex = 0;

    for (size_t datasetIdx = 0; datasetIdx < datasetCount; datasetIdx++) {
        size_t capacity = INITIAL_CAPACITY;
        Dataset1Row *dataset = (Dataset1Row *)malloc(capacity * sizeof(Dataset1Row));
        if (!dataset) {
            perror("Memory allocation failed");
            free(results);
            pthread_mutex_destroy(&mutex);
            return 1;
        }

        size_t rows = parseCSV(numericDatasets[datasetIdx], processLineDataset1, &dataset, &capacity, sizeof(Dataset1Row));
        if (rows <= 0) {
            fprintf(stderr, "Dataset %s failed to parse or is empty.\n", numericDatasets[datasetIdx]);
            free(dataset);
            continue;
        }
        logMessage("Dataset %s parsed successfully with %zu rows.\n", numericDatasets[datasetIdx], rows);

        for (size_t sortIdx = 0; sortIdx < sortTypeCount; sortIdx++) {
            ThreadArgs *args = (ThreadArgs *)malloc(sizeof(ThreadArgs));
            if (!args) {
                perror("Failed to allocate thread arguments");
                free(dataset);
                free(results);
                pthread_mutex_destroy(&mutex);
                return 1;
            }

            args->dataset = dataset;
            args->rows = rows;
            args->comparison = compareNumeric;
            args->sortType = sortTypes[sortIdx];
            args->datasetName = numericDatasets[datasetIdx];
            args->results = results;
            args->resultIndex = &resultIndex;
            args->datasetIndex = datasetIdx;
            args->mutex = &mutex;

            if (pthread_create(&threads[datasetIdx * sortTypeCount + sortIdx], NULL, benchmarkDataset, args) != 0) {
                perror("Failed to create thread");
                free(dataset);
                free(args);
                free(results);
                pthread_mutex_destroy(&mutex);
                return 1;
            }
        }
    }

    for (size_t i = 0; i < totalResults; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    logMessage("\nBenchmarking completed.\n");
    displayResults(results, resultIndex, sortTypes, sortTypeCount, numericDatasets, datasetCount);

    free(results);
    return 0;
}
