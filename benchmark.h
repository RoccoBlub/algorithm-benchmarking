#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "dataset.h"
#include "algorithms/heapsort.h"
#include "algorithms/quicksort.h"
#include "algorithms/mergesort.h"
#include "algorithms/radixsort.h"
#include "algorithms/timsort.h"
#include "algorithms/insertionsort.h"
#include "algorithms/shellsort.h"
#include "algorithms/hybridquicksort.h"
#include "algorithms/countingsort.h"
#include "algorithms/bubblesort.h"
#include "logging.h"


typedef struct {
    char sortType[50];
    char datasetName[100];
    char description[100];
    double timeTaken;
    size_t datasetIndex;
} BenchmarkResult;

void benchmark(Dataset1Row *data, size_t n, Comparator comparator, const char *description, const char *sortType, const char *datasetName, BenchmarkResult *result);


#endif