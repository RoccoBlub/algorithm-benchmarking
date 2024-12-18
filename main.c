#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dataset.h"
#include "benchmark.h"
#include "comparators.h"

#define INITIAL_CAPACITY 100

void displayResults(BenchmarkResult results[], int count) {
    // Print the header
    printf("+-----------+-------------+----------------+----------------+------------------+\n");
    printf("| Algorithm | Dataset     | Integer (Time) | Strings (Time) | Composite (Time) |\n");
    printf("+-----------+-------------+----------------+----------------+------------------+\n");

    // Track processed combinations of algorithm and dataset
    bool processed[count];
    for (int i = 0; i < count; i++) {
        processed[i] = false;
    }

    for (int i = 0; i < count; i++) {
        if (processed[i]) {
            continue; // Skip already processed entries
        }

        double timeByValue = 0, timeByLocation = 0, timeByComposite = 0;

        // Iterate through results to gather data for this dataset and algorithm
        for (int j = 0; j < count; j++) {

            if (!processed[j] && strcmp(results[i].datasetName, results[j].datasetName) == 0 && strcmp(results[i].sortType, results[j].sortType) == 0) {

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

        // Print the result
        printf("| %-9s | %-11s | %-14.2f | %-14.2f | %-16.2f |\n",
               results[i].sortType,
               results[i].datasetName,
               timeByValue,
               timeByLocation,
               timeByComposite
        );
    }

    // Print the footer
    printf("+-----------+-------------+----------------+----------------+------------------+\n");
}

void displayProgressBar(size_t current, size_t total) {
    int barWidth = 50; // Width of the progress bar
    double progress = (double)current / total;

    printf("[");
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) printf("=");
        else if (i == pos) printf(">");
        else printf(" ");
    }
    printf("] %d%%\r", (int)(progress * 100));
    fflush(stdout);
}

int main() {
    size_t capacity = INITIAL_CAPACITY;

    const char *filenames[] = {
        "datasets/dataset_10k.csv",
        "datasets/dataset_100k.csv",
        "datasets/dataset_500k.csv",
        "datasets/dataset_1m.csv",
        "datasets/dataset_1m_sorted.csv",
        "datasets/dataset_10m.csv",
        "datasets/dataset_10m_sorted.csv",
    };

    Dataset1Row *datasets[7] = {NULL};
    size_t rows[7] = {0};

    for (int i = 0; i < 7; i++) {
        datasets[i] = malloc(capacity * sizeof(Dataset1Row));
        if (!datasets[i]) {
            perror("Memory allocation failed");
            for (int j = 0; j < i; j++) free(datasets[j]);
            return 1;
        }

        rows[i] = parseCSV(filenames[i], processLineDataset1, &datasets[i], &capacity, sizeof(Dataset1Row));
        if (rows[i] < 0) {
            fprintf(stderr, "Failed to process file: %s\n", filenames[i]);
            for (int j = 0; j <= i; j++) free(datasets[j]);
            return 1;
        }
    }

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

    // InsertionSort not suitable for datasets >= 1million rows
    // RadixSort, TimSort and InsertionSort are still a work in progress.
    const char *sortTypes[] = {"HeapSort", "QuickSort", "MergeSort"};

    BenchmarkResult results[24];
    int resultIndex = 0;
    int datasetCount = 7;
    int descriptionCount = 3;
    int sortTypeCount = 3;

    size_t totalTasks = datasetCount * descriptionCount * sortTypeCount; // Total datasets x descriptions x sort types
    size_t currentTask = 0;

    for (int i = 0; i < datasetCount; i++) {
        for (int j = 0; j < descriptionCount; j++) {
            for (int k = 0; k < sortTypeCount; k++) {
                char dataset[256];
                snprintf(dataset, sizeof(dataset), "Dataset %d", i + 1);

                benchmark(datasets[i], rows[i], comparisons[j], descriptions[j], sortTypes[k], dataset, &results[resultIndex]);
                resultIndex++;
                currentTask++;

                // Update the progress bar
                displayProgressBar(currentTask, totalTasks);
            }
        }
    }

    printf("\nBenchmarking completed.\n");

    displayResults(results, resultIndex);

    for (int i = 0; i < 4; i++) {
        free(datasets[i]);
    }

    return 0;
}
