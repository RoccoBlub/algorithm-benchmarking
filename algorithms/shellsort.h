#ifndef SHELLSORT_H
#define SHELLSORT_H

#include <stddef.h>
#include "../dataset.h"
#include "../comparators.h"

// Function to perform Shell Sort on Dataset1Row array
void shellSort(Dataset1Row *arr, size_t n, Comparator comparator);

#endif // SHELLSORT_H
