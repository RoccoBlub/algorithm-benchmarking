#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include "dataset.h"
#include "benchmark.h"
#include "comparators.h"

#define INITIAL_CAPACITY 100

typedef struct {
    Dataset1Row *dataset;
    size_t rows;
    Comparator *comparisons;
    const char **descriptions;
    const char **sortTypes;
    size_t descriptionCount;
    size_t sortTypeCount;
    const char *datasetName;
    BenchmarkResult *results;
    size_t *resultIndex;
    size_t *progressCounter;  // Shared progress counter
    size_t totalTasks;        // Total number of tasks
    pthread_mutex_t *mutex;
} ThreadArgs;

const char *formatTime(double time) {
    static char buffer[16];
    if (time < 0) {
        return "Skipped";
    }
    snprintf(buffer, sizeof(buffer), "%.2f", time);
    return buffer;
}

void displayResults(BenchmarkResult results[], int count) {
    printf("+-----------------+-------------+----------------+----------------+------------------+\n");
    printf("| Algorithm       | Dataset     | Integer (Time) | Strings (Time) | Composite (Time) |\n");
    printf("+-----------------+-------------+----------------+----------------+------------------+\n");

    bool processed[count];
    for (int i = 0; i < count; i++) {
        processed[i] = false;
    }

    for (int i = 0; i < count; i++) {
        if (processed[i]) continue;

        double timeByValue = -1.0, timeByLocation = -1.0, timeByComposite = -1.0;

        for (int j = 0; j < count; j++) {
            if (!processed[j] &&
                strcmp(results[i].datasetName, results[j].datasetName) == 0 &&
                strcmp(results[i].sortType, results[j].sortType) == 0) {

                if (strcmp(results[j].description, "Sorting by Value") == 0) {
                    timeByValue = results[j].timeTaken;
                }
                if (strcmp(results[j].description, "Sorting by Location") == 0) {
                    timeByLocation = results[j].timeTaken;
                }
                if (strcmp(results[j].description, "Sorting by Composite") == 0) {
                    timeByComposite = results[j].timeTaken;
                }

                processed[j] = true;
                }
        }

        processed[i] = true;

        printf("| %-15s | %-11s | %-14s | %-14s | %-16s |\n",
               results[i].sortType,
               results[i].datasetName,
               (timeByValue >= 0) ? formatTime(timeByValue) : "-",
               (timeByLocation >= 0) ? formatTime(timeByLocation) : "-",
               (timeByComposite >= 0) ? formatTime(timeByComposite) : "-");
    }

    printf("+-----------------+-------------+----------------+----------------+------------------+\n");
}

void displayProgressBar(size_t current, size_t total) {
    int barWidth = 50;
    double progress = (double)current / total;

    printf("\r[");
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) printf("=");
        else if (i == pos) printf(">");
        else printf(" ");
    }
    printf("] %d%%", (int)(progress * 100));
    fflush(stdout);
}

void *benchmarkDataset(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;

    for (size_t j = 0; j < args->descriptionCount; j++) {
        for (size_t k = 0; k < args->sortTypeCount; k++) {
            BenchmarkResult result;
            benchmark(args->dataset, args->rows, args->comparisons[j],
                      args->descriptions[j], args->sortTypes[k],
                      args->datasetName, &result);

            pthread_mutex_lock(args->mutex);
            size_t idx = (*args->resultIndex)++;
            args->results[idx] = result;

            (*args->progressCounter)++;
            displayProgressBar(*args->progressCounter, args->totalTasks);
            pthread_mutex_unlock(args->mutex);
        }
    }

    free(args->dataset);
    free(args);

    return NULL;
}

int main() {
    const char *filenames[] = {
//        "datasets/dataset_10k.csv",
        "datasets/dataset_100k.csv",
//        "datasets/dataset_500k.csv",
//        "datasets/dataset_1m.csv",
//        "datasets/dataset_1m_sorted.csv",
//        "datasets/dataset_10m.csv",
//        "datasets/dataset_10m_sorted.csv",
    };

    const char *descriptions[] = {
        "Sorting by Value",
        "Sorting by Location",
        "Sorting by Composite"
    };

    Comparator comparisons[] = {
        compareNumeric,
        compareString,
        compareComposite
    };

    const char *sortTypes[] = {"HeapSort", "QuickSort", "MergeSort", "RadixSort", "TimSort", "ShellSort", "HybridQuickSort", "CountingSort", "InsertionSort", "BubbleSort"};

    size_t datasetCount = sizeof(filenames) / sizeof(filenames[0]);
    size_t descriptionCount = sizeof(descriptions) / sizeof(descriptions[0]);
    size_t sortTypeCount = sizeof(sortTypes) / sizeof(sortTypes[0]);
    size_t totalResults = datasetCount * descriptionCount * sortTypeCount;

    BenchmarkResult *results = (BenchmarkResult *)malloc(totalResults * sizeof(BenchmarkResult));
    if (!results) {
        perror("Failed to allocate memory for results");
        return 1;
    }

    pthread_t threads[datasetCount];
    size_t resultIndex = 0;
    size_t progressCounter = 0;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    size_t totalTasks = datasetCount * descriptionCount * sortTypeCount;

    for (size_t i = 0; i < datasetCount; i++) {
        size_t capacity = INITIAL_CAPACITY;
        Dataset1Row *dataset = (Dataset1Row *)malloc(capacity * sizeof(Dataset1Row));
        if (!dataset) {
            perror("Memory allocation failed");
            free(results);
            pthread_mutex_destroy(&mutex);
            return 1;
        }

        size_t rows = parseCSV(filenames[i], processLineDataset1, &dataset, &capacity, sizeof(Dataset1Row));
        if (rows < 0) {
            fprintf(stderr, "Failed to process file: %s\n", filenames[i]);
            free(dataset);
            continue;
        }

        ThreadArgs *args = (ThreadArgs *)malloc(sizeof(ThreadArgs));
        if (!args) {
            perror("Failed to allocate thread arguments");
            free(dataset);
            free(results);
            pthread_mutex_destroy(&mutex);
            return 1;
        }

        char datasetName[100];
        sprintf(datasetName, "Dataset %ld", i + 1);

        args->dataset = dataset;
        args->rows = rows;
        args->comparisons = comparisons;
        args->descriptions = descriptions;
        args->sortTypes = sortTypes;
        args->descriptionCount = descriptionCount;
        args->sortTypeCount = sortTypeCount;
        args->datasetName = datasetName;
        args->results = results;
        args->resultIndex = &resultIndex;
        args->progressCounter = &progressCounter;
        args->totalTasks = totalTasks;
        args->mutex = &mutex;

        if (pthread_create(&threads[i], NULL, benchmarkDataset, args) != 0) {
            perror("Failed to create thread");
            free(dataset);
            free(args);
            free(results);
            pthread_mutex_destroy(&mutex);
            return 1;
        }
    }

    for (size_t i = 0; i < datasetCount; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("\nBenchmarking completed.\n");
    displayResults(results, resultIndex);

    free(results);
    return 0;
}
