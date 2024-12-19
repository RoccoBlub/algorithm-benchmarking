#ifndef RADIXSORT_H
#define RADIXSORT_H

#include <stdlib.h>
#include "../dataset.h"
#include "../comparators.h"

// Function to perform Radix Sort on Dataset1Row array
void radixSort(Dataset1Row *arr, size_t n, Comparator comparator);

#endif // RADIXSORT_H
