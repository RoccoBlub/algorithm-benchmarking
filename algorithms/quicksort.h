#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <stddef.h>

#include "../dataset.h"
#include "../comparators.h"

// Quicksort function
void quickSort(Dataset1Row *base, size_t n, Comparator comparator);

#endif // QUICKSORT_H