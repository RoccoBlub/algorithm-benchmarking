#ifndef HYBRIDQUICKSORT_H
#define HYBRIDQUICKSORT_H

#include <stddef.h>
#include "../dataset.h"
#include "../comparators.h"

// Function to perform Hybrid QuickSort on Dataset1Row array
void hybridQuickSort(Dataset1Row *arr, size_t n, Comparator comparator);

#endif // HYBRIDQUICKSORT_H
