#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "dataset.h"
#include "algorithms/heapsort.h"
#include "algorithms/quicksort.h"
#include "algorithms/mergesort.h"
#include "algorithms/radixsort.h"

typedef struct {
    char datasetName[32];
    char sortType[32];
    char description[256];
    double timeTaken;
} BenchmarkResult;

void benchmark(Dataset1Row *data, size_t n, Comparator comparator, const char *description, const char *sortType, const char *datasetName, BenchmarkResult *result);


#endif